#include "read_header.h"
#include "../en_tete_elf.c"
#define MODE_BIG_ENDIAN 2

Elf32_Shdr createObjectSectionheader(char* nameFile, int index){
    Elf32_Shdr shdr;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    fseek(fichierAnalyse, elfHdr.e_shoff + index * sizeof(Elf32_Shdr), SEEK_SET);
    
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
    
    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { // 5 correspondant à l'octet étant le big ou little
        shdr.sh_name = __bswap_32(shdr.sh_name);
        shdr.sh_type = __bswap_32(shdr.sh_type);
        shdr.sh_flags = __bswap_32(shdr.sh_flags);
        shdr.sh_addr = __bswap_32(shdr.sh_addr);
        shdr.sh_offset = __bswap_32(shdr.sh_offset);
        shdr.sh_size = __bswap_32(shdr.sh_size);
        shdr.sh_link = __bswap_32(shdr.sh_link);
        shdr.sh_info = __bswap_32(shdr.sh_info);
        shdr.sh_addralign = __bswap_32(shdr.sh_addralign);
        shdr.sh_entsize = __bswap_32(shdr.sh_entsize);
    }
    
    fclose(fichierAnalyse);
    return shdr;
}

void read_section_header(char * filename, size_t size){
    FILE* ElfFile = NULL;
    char* SectNames = NULL;
    Elf32_Ehdr elfHdr;
    Elf32_Shdr sectHdr;
    Elf32_Shdr strTab;
    uint32_t idx;
    ElfFile = fopen(filename, "r");
    // read ELF header, first thing in the file
    elfHdr = createObjectEnteteELF(filename);
    printf("nb sections : %i\n",elfHdr.e_shnum); 
    strTab = createObjectSectionheader(filename, elfHdr.shstrndx);
    
    
    // read all section headers
    for (idx = 0; idx < elfHdr.e_shnum; idx++){
        
        sectHdr = createObjectSectionheader(filename, idx);
        printf("section numero %i : \n",idx);
        printf("type : %u\n",sectHdr.sh_type);
        
        
   
        
    }
}

