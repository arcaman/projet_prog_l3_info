#!/bin/bash

for i in `seq 1 5`;
do
        ./phase_un ../Examples_loader/example1.o 1 1 $i
	
	if [ $? -eq 0 ]
	then
	  echo -e "\033[32mSuccess\033[0m\n"
	else
	  echo -e "\033[33mFailure\033[0m\n"
	fi
done
