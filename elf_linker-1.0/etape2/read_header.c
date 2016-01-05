

#include "read_header.h"

void read_header(char * filename, size_t size){
    
    int fd = open(filename, O_RDONLY);
    char *p = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    Elf32_Ehdr *ehdr = (Elf32_Ehdr*)p;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(p + ehdr->e_shoff);
    printf("ca marche");
}

