#include "symbole_table.h"

#define SHT_SYMTAB 2
#define MODE_BIG_ENDIAN 2










Elf32_Sym createObjectSymbolHeader(char* nameFile, int index, Elf32_Shdr shdr) {
    Elf32_Sym sym;
    FILE* fichierAnalyse = fopen(nameFile, "r");
    Elf32_Ehdr elfHdr = createObjectEnteteELF(nameFile);
    fseek(fichierAnalyse, shdr.sh_offset + index * sizeof (Elf32_Sym), SEEK_SET);

    fread(&sym.st_name, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&sym.st_value, sizeof (Elf32_Addr), 1, fichierAnalyse);
    fread(&sym.st_size, sizeof (Elf32_Word), 1, fichierAnalyse);
    fread(&sym.st_info, sizeof (unsigned char), 1, fichierAnalyse);
    fread(&sym.st_other, sizeof (unsigned char), 1, fichierAnalyse);
    fread(&sym.st_shndx, sizeof (Elf32_Half), 1, fichierAnalyse);

    if (elfHdr.e_ident[5] == MODE_BIG_ENDIAN) { 
        // 5 correspondant à l'octet étant le big ou little
        sym.st_name = __bswap_32(sym.st_name);
        sym.st_value = __bswap_32(sym.st_value);
        sym.st_size = __bswap_32(sym.st_size);
        // on ne swap pas les unsigned char parce qu'ils ne sont pas en big endian
        sym.st_shndx = __bswap_16(sym.st_shndx);
    }

    fclose(fichierAnalyse);
    return sym;
}


void afficher_table_symb(char * filename){
    Elf32_Shdr symbtab,strsymbtab;
    Elf32_Ehdr elfHdr = createObjectEnteteELF(filename);
    Elf32_Shdr* sectiontab = malloc(elfHdr.e_shnum * sizeof (Elf32_Shdr));
    createAllObjectSectionHeader(filename, sectiontab, elfHdr.e_shnum);
    int i;
    for(i = 0;i < elfHdr.e_shnum;i++){
        if(sectiontab[i].sh_type==SHT_SYMTAB){
            symbtab = sectiontab[i];
            strsymbtab = sectiontab[i+1];
        }
    }
    
    FILE* fichier = fopen(filename, "r");
    fseek(fichier, strsymbtab.sh_offset, SEEK_SET);
    char * str = malloc(strsymbtab.sh_size);
    for (i = 0; i < strsymbtab.sh_size; i++) {
        str[i] = fgetc(fichier);
    }
    fclose(fichier);
    
    
    int k = symbtab.sh_size/sizeof(Elf32_Sym);
    for(i=0;i<k;i++){
        Elf32_Sym symbole = createObjectSymbolHeader(filename, i, symbtab);
        printf("symbole numero %i :\n",i);
        printf("nom : ");
        int i = symbole.st_name;
        while (str[i] != '\0') {
            printf("%c", str[i]);
            i++;
        }
        printf("\n");
        printf("valeur : %i\n",symbole.st_value);
        printf("taille : %i\n",symbole.st_size);
        printf("symbole utilise dans la section %i\n",symbole.st_shndx);
        
        
        
//#define ELF32_ST_BIND(i)    ((i)>>4)
//#define ELF32_ST_TYPE(i)    ((i)&0xf)
//#define ELF32_ST_INFO(b,t)  (((b)<<4)+((t)&0xf)) 
        printf("\n");
    }
}