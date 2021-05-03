#!/bin/bash
# affiche_param.sh

cd -- "$(dirname "$0")"
# nettoie l'affichage
reset

start=$(date +%s)

echo "liste les fichiers .png, dans le dossier, et compresse en raw 200x128"
# liste les fichiers .mov dans le dossier dans l'array "fichiers"
fichiers=($(find . -name "*.png"))

# nom de base des fichiers traités sans extension
baseFichier=$(basename ${fichiers[0]} .mov)

#nombre de fichiers trouvés
len=${#fichiers[*]}
# affiche le nombre de fichiers trouvés
echo "${len} fichiers à traiter"

# boucle sur la liste "fichiers"
for fichier in "${fichiers[@]}" ; do
	((num++))
  	echo "compresse "$fichier
  	ffmpeg -i $fichier -vf scale=200:128 -vcodec rawvideo -f rawvideo -pix_fmt rgb565 $(basename $fichier .png).raw
done

end=$(date +%s)
runtime=$((end-start))
sleep .1s
#affiche la durée du traitement
echo "Traitement terminé en ${runtime} sec."
exit 0
