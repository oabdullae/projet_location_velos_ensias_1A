#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "interface.h"
#include "structs.h"
#include "velo.h"
#include "client.h"
#include "file_io.h"
#include "io.h"
#include "bd.h"

int main() {
    printf("\n\n--- Bienvenue dans le programme de gestion du centre de "
            "location des vélos ---\n\n");
    print_menu();
    char operation;
    int rester = 1;
    Base_Donnee_Location bd_courante;
    init_base_donnee(&bd_courante);
    // import_base_donnee(); // later
    printf("F/f affiche un velo par id (temporairement pour cette premiere version\n");
    int c;
    do {
        operation = lire_caractere("Veuillez saisir la lettre correspondante "
                       "à l'opération que vous voulez effectuer\n\t>");
        switch (operation) {
            case 'a': case 'A':
                while ((c = getchar()) != '\n' && c != EOF) // flush buffer before reading
                    ;
                printf("Ajout d'un nouveau vélo\nVeuillez remplir les "
                       "informations suivantes:\n");
                // preparer les arguments
                    // id, marque, type, prix_par_heure, disponible, loue_par_client_id
                char *marque = NULL;
                do {
                    marque = lire_chaine_dynamique("\tSaisir la marque: ");
                } while (strlen(marque) == 0);
                char *type;
                do {
                    type = lire_chaine_dynamique("\tSaisir le type: ");
                } while (strlen(type) == 0);
                double prix_par_heure = 0;
                do {
                    prix_par_heure = lire_reel("\tSaisir le prix par heure: ");
                } while (prix_par_heure == 0);
                ajouter_nouveau_velo(&bd_courante, marque, type, prix_par_heure);
                break;
            case 'b': case 'B':
                printf("Suppression d'un vélo\n");
                // preparer les arguments
                // supprimer_velo();
                break;
            case 'c': case 'C':
                printf("Modification d'un vélo\n");
                // preparer les arguments
                // modifier_velo();
                break;
            case 'd': case 'D':
                printf("Location d'un vélo\n");
                // preparer les arguments
                // louer_velo()
                break;
            case 'e': case 'E':
                printf("Retour d'un vélo\n");
                // preparer les arguments
                // retourner_velo();
                break;
            case 'f': case 'F':
                while ((c = getchar()) != '\n' && c != EOF) // flush buffer before reading
                    ;
                printf("Affichage d'un velo:\n");
                int id = lire_entier("Saisir l'ID: ");
                afficher_velo_par_id(&bd_courante, id);

                break;
            case 'g': case 'G':

                break;
            case 'h': case 'H':
            case 'i': case 'I':
            case 'j': case 'J':
            case 'k': case 'K':
            case 'l': case 'L':
            case 'm': case 'M':
            case 'n': case 'N':
            case 'o': case 'O':
            case 'p': case 'P':
            case 'q': case 'Q':
            case 'r': case 'R':
            case 's': case 'S':
            case 't': case 'T':
            case 'u': case 'U':
                printf("--- Au revoir ---\n\n");
                rester = 0;
                break;
            case 'v': case 'V':
            case 'w': case 'W':
            case 'x': case 'X':
            case 'y': case 'Y':
            case 'z': case 'Z':
        }
    } while (rester);
    return 0;
}
