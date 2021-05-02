#!/bin/bash
# affiche_param.sh

# si gsed n'existe pas
# brew install coreutils findutils gnu-tar gnu-sed gawk gnutls gnu-indent gnu-getopt
cd -- "$(dirname "$0")"
reset

echo "liste les fichiers .png, dans le dossier, et compresse en raw 200x128"
# echo $(pwd)
# liste les fichiers .mov dans le dossier dans l'array "fichiers"
fichiers=($(find . -name "*.png"))

# nom de base des fichiers traités sans extension
baseFichier=$(basename ${fichiers[0]} .mov)

#nombre de fichiers trouvés
len=${#fichiers[*]}
echo "${len} fichiers à traiter"

# boucle sur l'array "fichiers"
for fichier in "${fichiers[@]}" ; do
	((num++))
	# echo $(basename $fichier .mov).raw
  echo "compresse "$fichier
  # nettoie $(basename $fichier .xhtml).html $(basename $fichier) $num
  ffmpeg -i $fichier -vf scale=200:128 -vcodec rawvideo -f rawvideo -pix_fmt rgb565 $(basename $fichier .png).raw
done

end=$(date +%s)
runtime=$((end-start))
sleep .1s
echo "Traitement terminé en ${runtime} sec."
exit 0
