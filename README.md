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
### bash/compVideo.command : 
Le script recherche tous les fichiers png dans le dossier encours, et les compresse en format raw vidéo (datas brutes sans entête).
Les fichiers doivent mesurer 200 * 128 px ou its ne s'afficheront pas correctement ave le code en arduino

## arduino/player_attendre_11_20210429.ino
le script est une adaptation du script de [Pawel A. Hernik](https://github.com/cbm80amiga/ST7735_SDVideoPlayback/) qui permet de lire des vidéos sur un écran 160*128 de type ST7735, muni d'un lecteur de carte SD (trouvé sur eBay ou aliexpress) avec une BluePill STM32F103.

Pour compiler le script et l'uploader sur le micro-contrôleur, à partIr de l'IDE' Arduino, il faut installer au préalable la distribution de [Roger Clark](https://github.com/rogerclarkmelbourne/Arduino_STM32) (un fork de la version officielle pour le STM32) et les lib SdFat, Adafruit_GFX_Library et Arduino_ST7735_STM

### espruino/clignotement_potard_A5_01.js
est un simple fichier de test du montage de l'espruino avec le potard placé en A5, il fait clignoter plus ou moins vite, la led verte de l'espruino pico.

### espruino/maipo_matrice_potard_01.js
script de base pour afficher une phrase avec un défilement droite gauche avec une vitesse variable (le potard en A5)

### IMPORTANT
- [FFMPEG] le logiciel ffmpeg doit être installé sur la machine (tonton goog pour l'installation)
- [STM32] La version officielle des cartes STM32 (par STMElectronics) pour arduino n'est pas compatible avec le script de lecture vidéo qui utilise la lib rapide SD->Ecran via SPI de Pawel A. Hernik.
- [STM32] il faut installer la distrib de [Roger Clark](https://github.com/rogerclarkmelbourne/Arduino_STM32) (un fork de la version officielle)... il faut chercher sur internet. et notamment sur le forum [STM32duino](https://www.stm32duino.com)
- [STM32] Peut être utile de lire: https://www.stm32duino.com/viewtopic.php?f=2&t=301.
- [STM32] À ce jour, la nouvelle IDE 2.0 d'Arduino ne voit pas la distribution de Roger Clark mais celle de [DanDrown](https://github.com/ddrown/Arduino_STM32) qui dispose d'un fichier d'install ( index.json) compatible avec la nouvelle IDE (ça compile aussi avec cette version)
- [STM32] la version de SDFat compatible avec le projet est la version 1.1.4 (la 2.xx ne compile pas)
- [STM32] la version de Adafruit_GFX compatible est la 1.5.0 (j'ai du downgrader pour que  le fichier comile)
- [STM32] pour uploader les fichiers sur la blue pill vous devez disposer d'un boitier STLink (qlq euros sur ebay avec une bluepill) (ça ne fonctionne pas via le port usb)
