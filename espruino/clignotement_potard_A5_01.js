/*  lit la valeur du potentiometre (en A5) et en déduit une durée
    change la durée du setIntervalen fonction
    =============================================================
    modifie la vitesse de clignotement de la LED verte du pico
*/
var etat = 1;
function onInit() {
  var duree = setInterval(function() {
    var lecture = analogRead(A5)*500+50;
    console.log(lecture);
    LED2.write(etat);
    etat = !etat;
    changeInterval(duree , lecture);
  }, 100);
}
