#include <stdio.h>
#include <stdlib.h>
#include "io.h"

int lire_entier_old(const char *prompt) {
    int n;
    printf("%s", prompt);
    scanf(" %d", &n);
    return n;
}

int lire_entier(const char *prompt) {
    char *temp_str = malloc(1024*sizeof(char)); // prompts will never exceed 1024
    // sprintf(temp_str, "%s", prompt);
    char *n_str = lire_chaine_dynamique(prompt);
    free(temp_str);
    int n = atoi(n_str);
    free(n_str);
    return n;
}

double lire_reel2(const char *prompt) {
    double x;
    printf("%s", prompt);
    scanf(" %lf", &x);
    return x;
}

double lire_reel(const char *prompt) {
    char *temp_str = malloc(1024*sizeof(char)); // prompts will never exceed 1024
    sprintf(temp_str, "%s", prompt);
    char *x_str = lire_chaine_dynamique(temp_str);
    free(temp_str);
    double x = atof(x_str);
    free(x_str);
    return x;
}


void lire_chaine_caractere(const char *prompt, char *output_str) {
    printf("%s", prompt);
    scanf(" %s", output_str);
}

char lire_caractere(const char *prompt) {
    char c;
    printf("%s", prompt);
    scanf(" %c", &c);
    // while ((c = getchar()) != '\n' && c != EOF) // flush buffer when first char consumed
    //     ;
    return c;
}

char *lire_chaine_dynamique(const char *prompt) {
    int c;
    // while ((c = getchar()) != '\n' && c != EOF) // flush buffer before reading
    //     ;
    printf("%s", prompt);
    size_t capacity = 16, size = 0;
    char *str = malloc(capacity * sizeof(char));
    while ((c = getchar()) != '\n' && c != EOF) {
        // the correct condition is size >= capacity -1 cuz if we look at its complement
        // it is size < capacity - 1, which means don't resize as long as size didnt hit 
        // capacity - 1 and the last byte we gonna write is capacity - 2, 
        // and capacity - 1 will be left for the \0
        if (size >= capacity - 1) { 
            capacity *= 2;
            str = realloc(str, capacity);
            // char *tmp = realloc(str, capacity);
            // if (tmp == NULL) {
            //     perror("tmp");
            //     exit(-200);
            // }
            // str = tmp;
        }
        str[size++] = c;
    }
    str[size] = '\0';
    str = realloc(str, size+1);
    return str;
}