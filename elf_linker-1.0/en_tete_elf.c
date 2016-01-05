#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>


#define MODE_BIG_ENDIAN 2

Elf32_Ehdr createObjectEnteteELF(char* nameFile) {
    int k;
    Elf32_Ehdr enTeteHeader;

    FILE* fichierAnalyse = fopen(nameFile, "r");

    fread(&enTeteHeader, sizeof(Elf32_Ehdr), 1, fichierAnalyse);

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

    
    
    switch(enTeteHeader.e_ident[4]) {
        case 1 : printf("\tClasse : 32-bit\n"); break;	
        case 2 : printf("\tClasse : 64-bit\n"); break;
        default : printf("\tClasse : unknown\n");
    }
    
    switch(enTeteHeader.e_ident[5]) {
        case 1 : printf("\tDonnees : little endian\n"); break;	
        case 2 : printf("\tDonnees : big endian\n"); break;
        default : printf("\tDonnees : unknown\n");
    }
    
    printf("\tVersion : %u\n", enTeteHeader.e_ident[6]);
    
    switch(enTeteHeader.e_ident[7]) {
        case 0 : printf("\tOS/ABI : System V\n"); break;
        case 1 : printf("\tOS/ABI : HP-UX\n"); break;
        case 2 : printf("\tOS/ABI : NetBSD\n"); break;
        case 3 : printf("\tOS/ABI : Linux\n"); break;
        case 6 : printf("\tOS/ABI : Solaris\n"); break;
        case 7 : printf("\tOS/ABI : AIX\n"); break;
        case 8 : printf("\tOS/ABI : Irix\n"); break;
        case 9 : printf("\tOS/ABI : FreeBSD\n"); break;
        case 12 : printf("\tOS/ABI : OpenBSD\n"); break;
        case 13 : printf("\tOS/ABI : OpenVMS\n"); break;	
        default : printf("\tOS/ABI : unknown\n");
    }
    
    printf("\tVersion ABI : %u\n", enTeteHeader.e_ident[8]);
    
    printf("Type : %u\n", enTeteHeader.e_type);
    
    switch(enTeteHeader.e_machine) {
        case 0 : printf("Machine : No specific instruction set\n"); break;
        case 2 : printf("Machine : SPARC\n"); break;
        case 3 : printf("Machine : x86\n"); break;
        case 8 : printf("Machine : MIPS\n"); break;
        case 20 : printf("Machine : PowerPC\n"); break;
        case 40 : printf("Machine : ARM\n"); break;
        case 42 : printf("Machine : SuperH\n"); break;
        case 50 : printf("Machine : IA-64\n"); break;
        case 62 : printf("Machine : x86-64\n"); break;
        case 183 : printf("Machine : AArch64\n"); break;
        default : printf("Machine : unknown\n");
    }
    
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