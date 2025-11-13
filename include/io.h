#ifndef IO_H
#define IO_H

#include <stdio.h>

int lire_entier(const char *prompt);
double lire_reel(const char *prompt);
char lire_caractere(const char *prompt);
void lire_chaine_caractere(const char *prompt, char *output_str);
char *lire_chaine_dynamique(const char *prompt);

#endif
