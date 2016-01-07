#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "en_tete_elf.h"
#include <sys/stat.h>


int main(int argc,char* argv[]){
    
    
    Elf32_Ehdr objet = createObjectEnteteELF(argv[1]);
    
    afficheTableEnTete(objet);
    
    return(0);
    
}