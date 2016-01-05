/* 
 * File:   en_tete_elf.h
 * Author: ducruyy
 *
 * Created on 5 janvier 2016, 15:41
 */

#ifndef EN_TETE_ELF_H
#define	EN_TETE_ELF_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

Elf32_Ehdr createObjectEnteteELF(char* nameFile);
void afficheTableEnTete(Elf32_Ehdr enTeteHeader);

#endif	/* EN_TETE_ELF_H */