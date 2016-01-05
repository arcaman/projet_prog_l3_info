#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

#define MODE_BIG_ENDIAN 2

Elf32_Ehdr createObjectEnteteELF(char* nameFile) {

    int i = 1;
    int j;
    int k;

    Elf32_Ehdr enTeteHeader;

    FILE* fichierAnalyse = fopen(nameFile, "r");

    for (k = 0; k < EI_NIDENT; k++) {
        fread(&enTeteHeader.e_ident[k], sizeof (unsigned char), 1, fichierAnalyse);
    }

    fread(&enTeteHeader.e_type, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_machine, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_version, sizeof (Elf32_Word), 1, fichierAnalyse);

    fread(&enTeteHeader.e_entry, sizeof (Elf32_Addr), 1, fichierAnalyse);
    fread(&enTeteHeader.e_phoff, sizeof (Elf32_Off), 1, fichierAnalyse);
    fread(&enTeteHeader.e_shoff, sizeof (Elf32_Off), 1, fichierAnalyse);
    fread(&enTeteHeader.e_flags, sizeof (Elf32_Word), 1, fichierAnalyse);

    fread(&enTeteHeader.e_ehsize, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_phentsize, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_phnum, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_shentsize, sizeof (Elf32_Half), 1, fichierAnalyse);

    fread(&enTeteHeader.e_shnum, sizeof (Elf32_Half), 1, fichierAnalyse);
    fread(&enTeteHeader.e_shstrndx, sizeof (Elf32_Half), 1, fichierAnalyse);

    if (enTeteHeader.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        enTeteHeader.e_type = __bswap_16(enTeteHeader.e_type);
        enTeteHeader.e_machine = __bswap_16(enTeteHeader.e_machine);
        enTeteHeader.e_version = __bswap_32(enTeteHeader.e_version);
        enTeteHeader.e_entry = __bswap_32(enTeteHeader.e_entry);
        enTeteHeader.e_phoff = __bswap_32(enTeteHeader.e_phoff);
        enTeteHeader.e_shoff = __bswap_32(enTeteHeader.e_shoff);
        enTeteHeader.e_flags = __bswap_32(enTeteHeader.e_flags);
        enTeteHeader.e_ehsize = __bswap_16(enTeteHeader.e_ehsize);
        enTeteHeader.e_phentsize = __bswap_16(enTeteHeader.e_phentsize);
        enTeteHeader.e_phnum = __bswap_16(enTeteHeader.e_phnum);
        enTeteHeader.e_shentsize = __bswap_16(enTeteHeader.e_shentsize);
        enTeteHeader.e_shnum = __bswap_16(enTeteHeader.e_shnum);
        enTeteHeader.e_shstrndx = __bswap_16(enTeteHeader.e_shstrndx);
    }

    fclose(fichierAnalyse);

    return enTeteHeader;
}

void afficheTableEnTete(Elf32_Ehdr enTeteHeader) {

    int k;
    printf("Magique : ");
    for (k = 0; k < EI_NIDENT; k++) {
        printf("%02x ", enTeteHeader.e_ident[k]);
    }
    printf("\n");

    printf("Type : %u\n", enTeteHeader.e_type);
    printf("Machine : %u\n", enTeteHeader.e_machine);
    printf("version : 0x%x\n", enTeteHeader.e_version);
    printf("Adresse point entree : 0x%x\n", enTeteHeader.e_entry);
    printf("Debut des en-tetes de programme : %x (octets dans le fichier)\n", enTeteHeader.e_phoff);
    printf("Debut des en-tetes de section : %u (octets dans le fichier)\n", enTeteHeader.e_shoff);
    printf("Flags : 0x%x\n", enTeteHeader.e_flags);
    printf("Taille de cet en-tete : %u (bytes)\n", enTeteHeader.e_ehsize);
    printf("Taille de l en-tete du programme : %u (bytes)\n", enTeteHeader.e_phentsize);
    printf("Nombre d en-tete du programme : %u\n", enTeteHeader.e_phnum);
    printf("Taille des en-tetes de section : %u (bytes)\n", enTeteHeader.e_shentsize);
    printf("Nombre des en-tetes de section : %u\n", enTeteHeader.e_shnum);
    printf("Table d indexes des chaines d'en-tete de section : %u\n", enTeteHeader.e_shstrndx);

}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Renseignez au moins 1 fichier !\n");
        return 0;
    }
    
    Elf32_Ehdr enTeteHeader = createObjectEnteteELF(argv[1]);
    afficheTableEnTete(enTeteHeader);
    
    return 0;
}