#!/bin/bash

cd ../Examples_loader
for i in `seq 1 5`;
do
	echo -e "\033[33m============================"
	echo -e "Etape $i :"
        echo -e "============================\033[0m"
	for files in $(ls *.o);
	do
		echo -e "\033[36mFichier : $files\033[0m"
		cd ../Examples_loader
        	../sources_perso/phase_un $files 1 1 $i
		if [ $? -eq 0 ]
		then
		  echo -e "\033[32mSuccess\033[0m\n"
		else
		  echo -e "\033[31mFailure\033[0m\n"
		fi
	done
done
