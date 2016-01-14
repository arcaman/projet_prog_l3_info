#!/bin/bash

log1=""
log2=""
cd ../Examples_loader
#echo -e "Etape 1 :"
for files in $(ls *.o);
do
        #echo -e "\033[36mFichier : $files\033[0m" > log.txt
	cd ../Examples_loader
	log11=$(../sources_perso/phase_un $files 1 1 1)
	log1=$log1$log11
	cd ../sources_perso
	log22=$(readelf -h ../Examples_loader/$files)
	log2=$log2$log22
done
#supprime les espaces
log1Clean=$(echo $log1 | tr -d " \t\n\r")
log2Clean=$(echo $log2 | tr -d " \t\n\r")
#supprime les accents du readelf
noaccents=$(echo $log2Clean | iconv -f utf8 -t ascii//TRANSLIT)

#compare les versions readelf et perso
if [ ! -z "$(diff <(echo "$log1Clean") <(echo "$noaccents"))" ]
then
	echo -e "\033[31mNon conforme à la commande readelf !\033[0m\n"		
else
	echo -e "\033[32mConforme à la commande readelf !\033[0m\n"
fi

#crée des fichiers de logs
#echo $log1Clean > test_log.txt
#echo $noaccents >> test_log.txt
