# Workshop

Vous trouverez les fichiers sources (codes js, arduino, scripts Bash pour ffmpeg) du travail produit avec les étudiants de 1ère année du DNMADe Graphisme en Mouvement
Ce workshop aura/a eu lieu en Mai 2021 avec

- Lou Sarabadzic, auteure et poète
- la Maison de la poésie de Rennes
- les étudiants de première année
- Tifenn Gargam, enseignante en Humanités
- Éric Choisy-Bernard, enseignant, poste à profil numérique dans le parcours éditorial

, au lycée Bréquigny, Rennes.

## fichiers:
### compVideo.command : 
script auto executable (doubleclicable) sur MacOS.
Le script recherche tous les fichiers png dans le dossier encours, et les compresse en format raw video (datas brutes sans entête) avec FFMPEG[^3].
Les fichiers doivent mesurer 200 * 128 px ou its ne s'afficheront pas correctement ave le code en arduino

### player_attendre_11_20210429.ino
Le script est une adaptation du script de Pawel A. Hernik qui permet de lire des vidéos sur un écran 160*128 de type ST7735, muni d'un lecteur de carte SD (trouvé sur eBay ou aliexpress) avec une BluePill STM32F103.

![](https://s3-ap-southeast-1.amazonaws.com/a2.datacaciques.com/wm/NDAy/1592215164/3690029238.jpg)
![](https://makelab.es/69-large_default/stm32f103c8t6-blue-pill-stm32.jpg)

Pour un affichage correct, les images doivent mesurer 200*128px, une marge de 40 px à gauche doit être laissée. Un offset dans le programme, déplace les pixels vides hors champs (si les images sont à dimension, il est impossible d'avoir une image complete propre).

Le script nécessite la distribution STM32 de Roger Clark (un fork de la version officielle pour le STM32) et les lib SdFat [^1], Adafruit_GFX_Library[^2] et Arduino_ST7735_STM (voir les notes en bas de page)

[^1]: version 1.1.4 compile, la version 2.x non
[^2]:  version 1.5.0 est compatible avec la lib
[^3]: [FFMPEG](https://ffmpeg.org/) doit être installé sur la machine

### clignotement_potard_A5_01.js
Simple fichier js de test du montage de l'espruino pico. Un potentiomètre est placé en A5. La manipulation du potentiomètre fait varier la durée du clignotement de la led verte de l'espruino.

### maipo_matrice_potard_01.js
Ce script js permet d'afficher une phrase avec un défilement droite gauche sur un module matrice rouge 20mm pour wemos  (1€ sur ebay ou aliexpress).
Ce module  est équipé d'un driver TM1640 en i2c et se pilote avec 5 cables (+5v , +3v3, GND, SCL, SDA). La  vitesse de défilement est variable grace au potentiomètre en A5.
![](https://ae01.alicdn.com/kf/HTB1.DG_hZbI8KJjy1zdq6ze1VXav/V1-0-8x8-Matrix-LED-Shield-8-Step-Adjustable-Intensity-for-WEMOS-D1-mini.jpg)

#### IMPORTANT
- [STM32] La version officielle des cartes STM32 (par STMElectronics) pour arduino n'est pas compatible avec le script de lecteur vidéo qui utilise la lib rapide SD->Ecran via SPI de Pawel A. Hernik. Il faut installer la distrib de [Roger Clark](https://github.com/rogerclarkmelbourne/Arduino_STM32) (un fork de la version officielle)... il faut chercher sur internet. et notamment sur le forum [STM32duino](https://www.stm32duino.com)
- [STM32] Peut être utile de lire: https://www.stm32duino.com/viewtopic.php?f=2&t=301.
- [STM32] À ce jour, la nouvelle IDE 2.0 d'Arduino ne voit pas la distribution de Roger Clark mais celle de [DanDrown](https://github.com/ddrown/Arduino_STM32) qui dispose d'un fichier d'install ( index.json) compatible avec la nouvelle IDE (le script compile avec cette version sur une bluepill)
- [STM32] pour uploader les fichiers sur la bluepill vous devez disposer d'un boitier STLink (qlq euros sur ebay avec une bluepill) (ça ne fonctionne pas via le port usb)
![ST-Link](https://ae01.alicdn.com/kf/HTB1K9U6RFXXXXXDXXXXq6xXFXXX8/Programmer-ST-LINK-V2-STM8-STM32-LINK-V2-ST-Downloader-Debugger-Golden-Shell.jpg)

