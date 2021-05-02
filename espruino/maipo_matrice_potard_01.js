// sur la matrice:
/*
  pour
  B13 : CLOCK
  B14 : LOAD  / CS
  B15 : DIN

  le MAX7219 nécessite 5V.
  qd branché à l'usb il faut brancher le VCC de la matrice au VOUT du Pico

  pour TM1640
  CLK D5 -> B13
  DIN D7 -> B15
     3V3 -> 3V3
      5V -> Vout
     GND -> GND


  A la reinitialisation de la position de la chaine, le buffer graphique ne s'initialise pas correctement
  *v03: segmentation de la chaine de texte en bouts de 10 caractères (+ rapide et pas d'erreur de buffer)
  *v08: reboot() à chaque fin de texte (resoud le pb de l'afficheur?)
  */

/*
var g; // TM1640
*/
// crée le buffer graphique 8x8 en 1 bit
let g = Graphics.createArrayBuffer(8,8,1);
// connecte en spi
let disp;
// pour la vitesse de déilement
var duree;
g.flip = function() { disp.raw(g.buffer); }; // To send to the display Max7219

const texte = "Voici une phrase qui s'affiche plus ou moins vite en fonction du potentiomètre";

let phrase = texte.substr(0,12);
let sDerCar = phrase.substr(10,2);
let cPhrase = 0;
let compteur = 8;
// let longT = 0;
// const lengthT = texte.length;
let longP = 0;
let decalage = 0;
let debut = false;
let potar;
let speed = 0;

function sendData() {
  //console.log("sendData");
  //t1 = getTime();
  // raccorde le bout de ligne suivant
  if (compteur < -longP+decalage+1){
    if (debut) {
      digitalPulse(LED2,1,20);
      console.log("redemarre ");
      //setTimeout(E.reboot, 5000);
      compteur = 8;
      debut = false;
    } else {
      compteur = 0;
    }
    cPhrase++;
    // si au bout du texte
    phrase= texte.substr(cPhrase*10,12);
    sDerCar = phrase.substr(10,2);
    decalage = g.stringWidth(sDerCar);
    //console.log("decalage:",decalage);
    digitalPulse(LED1,1,10);

    if (decalage === 0) {
      // on est en fin de texte, à la prochaine boucle, on reprend le texte au début
      cPhrase=-1;
      debut = true;
      console.log("redemarrage texte");
    }
  }

  // reset le buffer, ajoute la font, ecrit la chaine de caractères
  g.clear(); // efface le buffer (TM1640)
  g.setFont4x6();
  /* une fois font utilisée, on connait la longueur de la chaine*/
  longP = g.stringWidth(phrase);
  // calcul le decalage (en pix) entre chaque bout de phrase
  decalage = g.stringWidth(sDerCar);
  g.drawString(phrase,compteur,0);
  compteur--; // pour deplacer le texte
  g.setContrast(2); /* val entre 0 et 7 - TM1640*/
  g.setRotation(1,true); // TM1640
  // mise à jour du display
  g.flip();
  /* lecture du potar en A5*/
  potar = analogRead(A5); // valeur entre 0 et 1
  speed = 10+(potar*100);
  //console.log(speed);
  changeInterval( duree, speed);
}

function onInit() {
  require("Font4x6").add(Graphics);
  // spi mosi=DIN, sck=CLOCK
  g = require("TM1640").connect({din: B15, clk: B13});
  // efface l'afficheur
  g.clear();
  g.flip();
  console.log("init programme affichage matrice v2.0 - mrbbp ©2021");
  console.log("mém. used: "+Math.round(process.memory().usage/process.memory().total*100)+"%");
  //lance l'ecriture sur la matrice
  duree = setInterval(sendData, 500);
}

USB.setConsole();
