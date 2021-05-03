#include <Arduino_ST7735_STM.h>
#include <itoa.h>

#include <SdFatConfig.h>
#include <sdios.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <BlockDriver.h>
#include <SysCall.h>

// ST7735 library example
// Requires SdFat, Arduino_ST7735_STM libraries and stm32duino
// (C)2019 Pawel A. Hernik
// YT video: https://youtu.be/o3AqITHf0mo

/*
 ST7735 128x160 1.8" LCD pinout (header at the top, from left):
 #1 LED   -> 3.3V
 #2 SCK   -> SCL/D13/PA5
 #3 SDA   -> MOSI/D11/PA7
 #4 A0/DC -> D8/PA1  or any digital
 #5 RESET -> D9/PA0  or any digital
 #6 CS    -> D10/PA2 or any digital
 #7 GND   -> GND
 #8 VCC   -> 3.3V
           SPI2/SPI1
 SD_SCK  - PB13/PA5
 SD_MISO - PB14/PA6
 SD_MOSI - PB15/PA7
 SD_CS   - PB12/PA4
*/

/*
 STM32 SPI1/SPI2 pins:

 SPI1 MOSI PA7
 SPI1 MISO PA6
 SPI1 SCK  PA5
 SPI1 CS   PA4
 SPI2 MOSI PB15
 SPI2 MISO PB14
 SPI2 SCK  PB13
 SPI2 CS   PB12
*/

/*
 Comments:
 SD uses faster STM32 SPI1 interface which supports 36 Mbps
 Not all SD cards work at 36MBps
 Slow card at 18Mbps gives 22-23fps
 Fast card at 18Mbps gives 25-26fps
 Fast card at 36Mbps gives 33-34fps
 SdFat library uses DMA for SPI transfer
 Big buffer in RAM is used to speed up SPI/DMA transfer
 SPI1 is shared between LCD and SD card
*/

#include <SPI.h>
#include <Adafruit_GFX.h>

#if (__STM32F1__) // bluepill
// ATTENTION spécifique à la version noire, j'ai inversé deux cables
// inversion de CS/DC -> A1/A2 par rapport à la version officielle
//#define TFT_CS  PA1
//#define TFT_DC  PA2

/* pour la version officielle */
#define TFT_CS  PA2
#define TFT_DC  PA1
/* fin de la version officielle */
#define TFT_RST PA0
#include <Arduino_ST7735_STM.h>
#else
#define TFT_CS 10
#define TFT_DC  8
#define TFT_RST 9
#endif

#define SCR_WD 128
#define SCR_HT 160
Arduino_ST7735 lcd = Arduino_ST7735(TFT_DC, TFT_RST, TFT_CS);

#define USE_SDIO 0
const uint8_t SD_CS = PA4;
SdFat sd1;
// serial output stream
ArduinoOutStream sout(Serial);
#define sdErrorMsg(msg) sd1.errorPrint(F(msg));

SdFile file;

void lcdSPI() { SPI.beginTransaction(SPISettings(36000000, MSBFIRST, SPI_MODE3, DATA_SIZE_16BIT));}
// use 18 if your SD card doesn't work // for old sdcard
#define SD_SPEED 36
// #define SD_SPEED 18
void sdSPI() { SPI.beginTransaction(SD_SCK_MHZ(SD_SPEED));}

// --------------------------------------------------------------------------
/* this 2lines are from SDMediaPlayer
https://github.com/cbm80amiga/ST7735_SDMediaPlayer/blob/master/ST7735_SDMediaPlayer.ino
*/
#define NLINES 32
// attention depend de la largeur de la video
#define BUF_WD 200
uint16_t buf[BUF_WD*NLINES] __attribute__((aligned(4)));

char txt[30];

/*---------------*/
/* pour le debug */
bool debug = true;
/*---------------*/

// Params:
// name - file name
// x,y - start x,y on the screen
// wd,ht - width, height of the video (raw data has no header with such info)
// nl - num lines read in one operation (nl*wd*2 bytes are loaded)
// skipFr - num frames to skip
void showRaw(char *name, int x, int y, int wd, int ht, int nl, int skipFr) {
  lcd.setTextColor(YELLOW,BLACK);
  // si pb de fichier écran JAUNE
  if(!file.open(name, O_CREAT | O_RDONLY)) {
    lcdSPI(); lcd.fillScreen(YELLOW);
    sd1.errorHalt(F("Erreur d'ouverture de fichier"));
  }
  file.seekSet(0);
  unsigned long sdStartTime,frTime,lcdTime,sdTime=0;
  while(file.available()) {
    sdTime = 0;
    frTime = millis();
    for(int i=0;i<ht/nl;i++) {
      sdStartTime = millis();
      int rd = file.read(buf,wd*2*nl);
      sdTime += millis()-sdStartTime;
      lcdSPI();
      for(int j=0;j<nl;j++) lcd.drawImage(x,y+(i*nl+j),wd,1,buf+20+j*wd);
    }
    frTime = millis()-frTime;
    lcdTime = frTime-sdTime;
    // afficahge du nom du fichier en mode debug
    if (debug) {
      lcd.setCursor(0,lcd.height()-7);
      lcdSPI();
      lcd.fillRect(0,lcd.height()-8,lcd.width()/3,8,BLACK);
      snprintf(txt,30,"%s",name);
      lcdSPI(); lcd.print(txt);
    }
    // si skipFrame (passe des images -> se déplace dans le fichier)
    if(skipFr>0) file.seekCur(wd*ht*2*skipFr); // hauteur * largeur * couleur en 16bits (2 octets)
  }
  file.close();
}
// --------------------------------------------------------------------------

void setup(void) {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  lcd.init();
  lcd.setRotation(0);
  lcd.fillScreen(BLACK);
  lcd.setCursor(10,52); lcd.print("eric choisy - 2021");
  delay(300);
  lcd.setCursor(10,72); lcd.print("Poeme");
  delay(500);
  lcd.setCursor(10,84); lcd.print("ATTENDRE");
  delay(500);
  lcd.setCursor(2,150); lcd.print("v11-210429");
  lcd.setRotation(3);
  delay(3000);
  // ecran rouge si pb format carte SD
  if(!sd1.cardBegin(SD_CS, SD_SCK_MHZ(SD_SPEED))) {
    lcdSPI(); lcd.fillScreen(RED);
    sdErrorMsg("\nSD Card initialization failed.\n");
  }
  // ecran magenta si pb d'init du système
  if(!sd1.fsBegin()) {
    lcdSPI(); lcd.fillScreen(MAGENTA);
    sdErrorMsg("\nFile System initialization failed.\n");
    return;
  }
}
/* composition du "poème" */
bool bDebut = true;
int compteur = 0;
// numéro du poeme
int compteurP = 0;
// nombre de phrases par cycle (ap. "attendre")
int nPh=random(2,5);
// extension du fichier (fin de la chaine fichier)
const char nom[10] = ".raw";
// numero du fichier
int num;
int nTirages = 0;
// numeroe en String du fichier
char snum[4];
// mémoire des tirages
int nO1, nO2,nO3,nO4 = 0;
// nombre total de fichiers pour le tirage
const int nbrRaw1 = 213;
const int nbrRaw2 = 269;
const int nbrRaw3 = 316;
int liste[nbrRaw3] = {};  // ne pas oublier de changer la taille de la table avec le contenu max
int type = 0;
int nbrType1 = nbrRaw2 - nbrRaw1;
int nbrType2 = nbrRaw3 - nbrRaw2;
// --------------------------------------------------------------------------
// Params:
// showRaw( fichier, x, y, wd, ht, nl, skipFrame)
// fichier - file name
// x,y - start x,y on the screen
// wd,ht - width, height of the video (raw data has no header with such info)
// nl - num lines read in one operation (nl*wd*2 bytes are loaded)
// skipFr - num frames to skip

void loop() {
  if (bDebut) {
    // affiche le numéro du poeme
    bDebut = false;
    // transforme int en string
    itoa(compteurP%250,snum,10);
    char chaine[8] = {};
    if (compteur%2==0) {
      strncat(chaine,"ni_",3);   // noir/blanc
    } else {
      strncat(chaine,"n_",3);   // blanc/noir
    }
    strncat(chaine,snum,4); // ajoute le numéro de la video
    strncat(chaine,nom,10); // ajoute ".raw"
    // Serial.print("appelle ");
    // Serial.println(chaine);
    //affiche le fichier avec le numéro
    showRaw(chaine,-20,0,200,128,32,0);
    delay(2000);
  }
  if (compteur%(nPh)==0) {// "attendre"
    if (compteur%2==0) {
      showRaw("t_0.raw",-20,0,200,128,32,0); // noir/blanc
    } else {
      showRaw("i_0.raw",-20,0,200,128,32,0); // blanc/noir
    }
    if (type==0) {
      // tente un type différent en début de cycle
      switch (random(10)) {
        // type par défaut 85%
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          type = 0;
          break;        
        case 17:        
        case 18:
          // 10%
          type = 1;
          break;
        case 19:
          // 5% des cas
          type = 2;  
       }
    }      
    delay(850);
  } else { // tire une phrase (sauf à la première de la série)
    switch (type) {
      case 0:
        // cas normal "attendre que…"
        while(liste[num]== 1) {
          num = random(1,nbrRaw1);
        }  
        break;
      case 1:
        // si tirage spécial -> "attendre le/la/les…"
        while(liste[num]== 1) {
          num = random(nbrRaw1,nbrRaw2);
        }
        nbrType1--;
        break;     
      default:     
        // si tirage spécial -> "attendre le/la/les…"
        while(liste[num]== 1) {
          num = random(nbrRaw2,nbrRaw3);
        } 
        nbrType2--;
        break;
    }
    liste[num]= 1;
    // Serial.print(bSpecial);
    // Serial.print(" - ");
    // Serial.println(num);
    nTirages++;
    // transforme int en string
    itoa(num,snum,10);

    char chaine[8] = {};
    if (compteur%2==0) {
      strncat(chaine,"t_",2);   // noir/blanc
    } else {
      strncat(chaine,"i_",2);   // blanc/noir
    }
    strncat(chaine,snum,4); // ajoute le numéro de la video
    strncat(chaine,nom,10); // ajoute ".raw"
    // Serial.print("appelle ");
    // Serial.println(chaine);
    //affiche le fichier tiré
    showRaw(chaine,-20,0,200,128,32,0);
    if (compteur%nPh == nPh-1) { // tire un nouveau modele de phrase en fin de cycle de phrase
      nPh=random(2,5);
      type = 0;
      compteur = -1;
    }
    // delai pour la lecture
    delay(1400);
  }
  //incrémente les compteurs
  compteur++;

  // vide la table de test si pleine
  if (nTirages >=nbrRaw1-1 || nbrType1<=1 || nbrType2<=1) {
    // affiche "fin du poeme"
    if (compteur%2==0) {
      showRaw("fin_i.raw",-20,0,200,128,32,0);
    } else {
      showRaw("fin.raw",-20,0,200,128,32,0);
    }
    // vide la liste des tirages
    memset(liste, 0, sizeof(liste));
    // reset les variables (c'est un micro-contrôleur)
    compteur = 0;
    nTirages = 0;
    type = 0;
    nbrType1 = nbrRaw2 - nbrRaw1;
    nbrType2 = nbrRaw3 - nbrRaw2;
    delay(1000);
    bDebut = true;
    compteurP ++;
  }
  /* TODO
    g
  DEBUGGER :
    parfois une suite de + de 6 phrases sans "Attendre" (DONE reste du compteur à chaque retirage de nPh)

  FAIT:
    - ajouter une image "fin" en fin de poème
    - ajouter des images de numéro du poème
    - mise en place de typologies de phrase cycle 
        attendre que. (DONE)
        attendre le/la/les (DONE)
        attendre pour + infintif
    déclare la fin du poème quand les types (1 et 2) ont tous été tirés -> type = 0;
    - correction de la limite du numero de Poeme (> 250)
    - ajout d'un debug a l'écran du nom du fichier lu (si image jaune avoir le nom du fichier appelé qui pose pb)
  */
    
}
// ------------------------------------------------
