#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "display_content.h"
#include <sys/stat.h>

int main(int argc, char* argv[]) {


    if (argc != 4) {
        printf("Il n y a pas suffisamment d arguments\n");
        return 0;
    }

    char* nameFile = argv[1];
    char* indiceOrNameSection = argv[2];
    int isInt = atoi(argv[3]);

    int idSection = getIndexSectionByNameOrIndex(nameFile, indiceOrNameSection, isInt);

    unsigned char* tableauOctetsSection = createSectionContent(nameFile, idSection);

    displaySectionContent(tableauOctetsSection, nameFile, idSection);

    return (0);




}