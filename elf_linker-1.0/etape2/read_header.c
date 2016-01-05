#include "read_header.h"
#define MODE_BIG_ENDIAN 2

Elf32_Ehdr createObjectEnteteELF(char* nameFile) {
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

Elf32_Shdr createObjectSectionheader(char* nameFile, int index){
    Elf32_Shdr shdr;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    fread(&shdr.sh_name, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_type, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_flags, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_addr, sizeof (Elf32_Addr), 1, fichierAnalyse);
    fread(&shdr.sh_offset, sizeof (Elf32_Off), 1, fichierAnalyse);
    fread(&shdr.sh_size, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_link, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_info, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_addralign, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&shdr.sh_entsize, sizeof (Elf32_Word), 1, fichierAnalyse);
    
    fclose(fichierAnalyse);
    return shdr;
}

void read_section_header(char * filename, size_t size){
    FILE* ElfFile = NULL;
    char* SectNames = NULL;
    Elf32_Ehdr elfHdr;
    Elf32_Shdr sectHdr;
    uint32_t idx;
    ElfFile = fopen(filename, "r");
    // read ELF header, first thing in the file
    elfHdr = createObjectEnteteELF(filename);
    printf("nb sections : %i\n",elfHdr.e_shnum); 
    
    
    
    // read section name string table
    // first, read its header.
    fseek(ElfFile, elfHdr.e_shoff + elfHdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&sectHdr, 1, sizeof(Elf32_Shdr), ElfFile);
    // next, read the section, string data
    //printf("sh_size = %llu\n", (long long unsigned int)sectHdr.sh_size);
    SectNames = malloc(sectHdr.sh_size);
    fseek(ElfFile, sectHdr.sh_offset, SEEK_SET);
    fread(SectNames, 1, sectHdr.sh_size, ElfFile);

    
    // read all section headers
    for (idx = 0; idx < elfHdr.e_shnum; idx++){
        fseek(ElfFile, elfHdr.e_shoff + idx * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&sectHdr, 1, sizeof(Elf32_Shdr), ElfFile);
        //printf("section numero %i : \n",idx);
    }
}

