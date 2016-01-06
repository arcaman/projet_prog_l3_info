/* 
 * File:   symbole_table.h
 * Author: rocheroh
 *
 * Created on 6 janvier 2016, 13:36
 */

#ifndef SYMBOLE_TABLE_H
#define	SYMBOLE_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <byteswap.h>
#include "../etape2/read_header.h"
#include "../en_tete_elf.h"

Elf32_Sym createObjectSymbolHeader(char* nameFile, int index, Elf32_Shdr shdr);
void afficher_table_symb(char * filename);

#endif	/* SYMBOLE_TABLE_H */

