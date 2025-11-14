#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "structs.h"
#include "client.h"
#include "velo.h"
#include "bd.h"
#include "file_io.h"
#include "io.h"
#include "interface.h"
#include "error_codes.h"



int main() {
    printf("\n\n--- Bienvenue dans le programme de gestion du centre de "
        "location des vélos ---\n\n");
    print_menu();
    char operation;
    int rester = 1;
    Base_Donnee_Location bd_courante;
    init_base_donnee(&bd_courante);
    // import_base_donnee(); // later
    printf("F/f affiche un velo par id (temporairement pour cette "
        "premiere version\net T/t pour tous les velos\nS/s for clients\n\n");
    int c;
    do {
        operation = lire_caractere("Veuillez saisir la lettre correspondante "
            "à l'opération que vous voulez effectuer\n\t> ");
        switch (operation) {
            case 'a': case 'A':
            // blocks by curly braces to use local variables for each case
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                    
                printf("Ajout d'un nouveau vélo\nVeuillez remplir les "
                    "informations suivantes:\n");
                // preparer les arguments
                    // marque, type, prix_par_heure
                char *marque;
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
                } while (prix_par_heure <= 0);
                if (ajouter_nouveau_velo(&bd_courante, marque, type, 
                    prix_par_heure) != 0) {
                    printf("\n### ERREUR: Echéc d'ajout d'un nouveau vélo\n\n");
                }
                else {
                    printf("Ajout d'un nouveau vélo réussi.\n\n");
                }
            }
                break;
            case 'b': case 'B':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: maybe add possibility of deleting a range of velos or a
                // set of velos later of course
                printf("Suppression d'un vélo\n");
                // preparer les arguments
                int id;
                do {
                    id = lire_entier("\tSaisir l'ID>0: ");
                } while (id <= 0);
                if (supprimer_velo_par_id(&bd_courante, id) != 0) {
                    printf("\n### ERREUR: Echéc de suppression du vélo, "
                        " vélo introuvable\n\n");
                }
                else {
                    printf("Suppression réussie.\n\n");
                }
            }
                break;
            case 'c': case 'C':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                printf("Modification du tarif d'un vélo\n");
                // preparer les arguments: id et le nouveau tarif
                int id;
                do {
                    id = lire_entier("\tSaisir l'ID>0: ");
                } while (id <= 0);
                double nouveau_prix_par_heure = 0;
                do {
                    nouveau_prix_par_heure = lire_reel("\tSaisir le nouveau "
                                                       "prix par heure: ");
                } while (nouveau_prix_par_heure <= 0);
                if (modifier_tarif_velo(&bd_courante, id, 
                                        nouveau_prix_par_heure) != 0) {
                    printf("\n### ERREUR: Echéc de modification du tarif du "
                        "vélo, vélo introuvable\n\n");
                }
                else {
                    printf("Modification du tarif réussie.\n\n");
                }
            }
                break;
            case 'd': case 'D':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                printf("Location d'un vélo\n");
                // preparer les arguments: id du velo + id du client
                if (bd_courante.clients.size == 0) { // TODO: maybe this is
                // redundant cuz the function checks if the client exists or not
                    printf("\n### ERREUR: La table des clients est vide\n");
                    printf("Ajoutez au moins un client pour effectuer "
                        "une location\n\n");
                }
                else {
                    int id_velo;
                    do {
                        id_velo = lire_entier("\tSaisir l'ID du vélo à louer: ");
                    } while (id_velo <= 0);
                    int id_client;
                    do {
                        id_client = lire_entier("\tSaisir l'ID du client: ");
                    } while (id_client <= 0);
                    switch (louer_velo_par_client(&bd_courante, id_velo, 
                        id_client)) {
                        case ERR_VELO_NOT_FOUND:
                            printf("\n### ERREUR: Echéc de location du vélo, "
                                "vélo introuvable\n\n");
                            break;
                        case ERR_CLIENT_NOT_FOUND:
                            printf("\n### ERREUR: Echéc de location du vélo, "
                                "client introuvable\n\n");
                            break;
                        case ERR_VELO_DEJA_LOUE:
                            printf("\n### ERREUR: Echéc de location du vélo, "
                                "vélo déjà loué\n\n");
                            break;
                        case ERR_CLIENT_LOUE_DEJA_1VELO:
                            printf("\n### ERREUR: Echéc de location du vélo, "
                                "ce client loue déjà un vélo\n\n");
                            break;
                        case 0:
                            printf("Location du vélo réussie.\n\n");
                            break;
                        default:
                            printf("erreur du programmeur");
                    }
                }
            }
                break;
            case 'e': case 'E':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                printf("Retour d'un vélo\n");
                // preparer les arguments: id du client, duree de la location
                int id_client;
                do {
                    id_client = lire_entier("\tSaisir l'ID du client: ");
                } while (id_client <= 0);
                double duree_location;
                do {
                    duree_location = lire_reel("\tSaisir la durée de cette "
                        "location: ");
                } while (duree_location <= 0.0);
                double montant;
                switch (retourner_velo_par_client(&bd_courante, id_client,
                    duree_location, &montant)) {
                    case ERR_CLIENT_NOT_FOUND:
                        printf("\n### ERREUR: Echéc de retour du vélo, "
                            "client introuvable\n\n");
                        break;
                    case ERR_CLIENT_NOT_RENTING:
                        printf("\n### ERREUR: Echéc de retour du vélo, "
                            "ce client n'a pas encore loué un vélo\n\n");
                        break;
                    case 0:
                        printf("Votre montant à payer est %g\n", 
                            montant);
                        printf("Retour du vélo réussie.\n\n");
                        break;
                    default:
                            printf("erreur du programmeur");
                }
            }
                break;
            case 'f': case 'F':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                    
                printf("Ajout d'un nouveau client\nVeuillez remplir les "
                    "informations suivantes:\n");
                // preparer les arguments
                    // nom, prenom, telephone
                char *nom;
                do {
                    nom = lire_chaine_dynamique("\tSaisir le nom: ");
                } while (strlen(nom) == 0);
                char *prenom;
                do {
                    prenom = lire_chaine_dynamique("\tSaisir le prenom: ");
                } while (strlen(prenom) == 0);
                char *telephone;
                do {
                    telephone = lire_chaine_dynamique("\tSaisir le telephone: ");
                } while (strlen(telephone) == 0);
                if (ajouter_nouveau_client(&bd_courante, nom, prenom, telephone)
                    != 0){
                    printf("\n### ERREUR: Echéc d'ajout d'un nouveau "
                        "client\n\n");
                }
                else {
                    printf("Ajout d'un nouveau client réussi.\n\n");
                }
            }

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
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: move this out of here
                printf("Affichage d'un velo:\n");
                int id = lire_entier("Saisir l'ID: ");
                afficher_velo_par_id(&bd_courante, id);
                break;
            case 't': case 'T':
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: move this out of here
                // TODO: make this a separate function
                if (bd_courante.velos.size != 0)
                    printf("Affichage de la table des velos\n");
                else
                    printf("Le tableau des velos est vide\n");
                for (int i = 0; i < bd_courante.velos.size; ++i) {
                    printf("\n------\n\n");
                    printf("\tid = %d\n", bd_courante.velos.tab_velo[i].id);
                    printf("\tmarque = %s\n",
                           bd_courante.velos.tab_velo[i].marque);
                    printf("\ttype = %s\n", bd_courante.velos.tab_velo[i].type);
                    printf("\tprix par heure = %.2lf\n",
                           bd_courante.velos.tab_velo[i].prix_par_heure);
                    printf("\tdiponible? = %s\n",
                        (bd_courante.velos.tab_velo[i].disponible)?"oui":"non");
                    if (!bd_courante.velos.tab_velo[i].disponible)
                        printf("\tloué par le client avec id = %d\n",
                            bd_courante.velos.tab_velo[i].loue_par_client_id);
                }
                printf("\n\n");
                break;
            case 's': case 'S':
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: move this out of here
                // TODO: make this a separate function
                if (bd_courante.clients.size != 0)
                    printf("Affichage de la table des clients\n");
                else
                    printf("Le tableau des clients est vide\n");
                for (int i = 0; i < bd_courante.clients.size; ++i) {
                    printf("\n------\n\n");
                    printf("\tid = %d\n", bd_courante.clients.tab_client[i].id);
                    printf("\tnom = %s\n",
                        bd_courante.clients.tab_client[i].nom);
                    printf("\tprenom = %s\n",
                        bd_courante.clients.tab_client[i].prenom);
                    printf("\ttelephone = %s\n",
                        bd_courante.clients.tab_client[i].telephone);
                    printf("\tduree accumulee de tous les locations = %.2lf\n",
                        bd_courante.clients.tab_client[i].duree_accumulee);
                    if (bd_courante.clients.tab_client[i].velo_loue_id)
                        printf("\tid du vélo loué = %d\n",
                            bd_courante.clients.tab_client[i].velo_loue_id);
                }
                printf("\n\n");
                break;
            
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
