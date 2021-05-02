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