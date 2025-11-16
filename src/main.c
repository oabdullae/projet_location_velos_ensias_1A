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

            // =================================================================

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
                switch (supprimer_velo_par_id(&bd_courante, id)) {
                    case ERR_VELO_NOT_FOUND:
                        printf("\n### ERREUR: Echéc de suppression du vélo, "
                            "vélo introuvable\n\n");
                        break;
                    case ERR_VELO_DEJA_LOUE:
                        printf("\n### ERREUR: Echéc de suppression du vélo, "
                            "vélo déjà loué, il doit être retourné d'abord\n\n");
                        break;
                    case 0:
                        printf("Suppression réussie.\n\n");
                        break;
                    default:
                        printf("#### erreur du programmeur ####\n");
                }
            }
                break;

            // =================================================================

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

            // =================================================================

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
                            printf("#### erreur du programmeur ####\n");
                    }
                }
            }
                break;

            // =================================================================

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
                } while (duree_location < 0.0);
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
                            printf("#### erreur du programmeur ####\n");
                }
            }
                break;

            // =================================================================

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

            // =================================================================

            case 'g': case 'G':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: maybe add possibility of deleting a range of clients or a
                // set of clients later of course
                printf("Suppression d'un client\n");
                // preparer les arguments
                int id;
                do {
                    id = lire_entier("\tSaisir l'ID>0: ");
                } while (id <= 0);
                switch (supprimer_client_par_id(&bd_courante, id)) {
                    case ERR_CLIENT_NOT_FOUND:
                        printf("\n### ERREUR: Echéc de suppression du client, "
                            "client introuvable\n\n");
                        break;
                    case ERR_CLIENT_LOUE_DEJA_1VELO:
                        printf("\n### ERREUR: Echéc de suppression du client, "
                            "client déjà loue un vélo, il doit le retourner d'abord"
                            "\n\n");
                        break;
                    case 0:
                        printf("Suppression réussie.\n\n");
                        break;
                    default:
                        printf("#### erreur du programmeur ####\n");
                }
            }
                break;

            // =================================================================

            case 'h': case 'H':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                printf("Modification des donnees d'un client\n");
                printf("\tQue voulez-vous modifier?\n");
                printf("\t  a. Nom\n");
                printf("\t  b. Prenom\n");
                printf("\t  c. Telephone\n");
                char choix;
                do {
                    choix = lire_caractere("\t> ");
                } while (choix != 'a' && choix != 'b' && choix != 'c');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                
                char str_choix[32];
                switch (choix) {
                    case 'a':
                        strcpy(str_choix, "nom");
                        break;
                    case 'b':
                        strcpy(str_choix, "prénom");
                        break;
                    case 'c':
                        strcpy(str_choix, "téléphone");
                        break;
                }

                char *str, prompt[1024];
                sprintf(prompt, "Saisir le nouveau %s:", str_choix);
                do {
                    str = lire_chaine_dynamique(prompt);
                } while (strlen(str) == 0);
                int id;
                do {
                    id = lire_entier("\tSaisir l'ID>0: ");
                } while (id <= 0);
                switch (modifier_client(&bd_courante, id, (int)(choix - 'a'),
                    str)) {
                    case ERR_CLIENT_NOT_FOUND:
                        printf("\n### ERREUR: Echéc de modification du client, "
                            "client introuvable\n\n");
                        break;
                    case 0:
                        printf("Modification réussie.\n\n");
                        break;
                    default:
                        printf("#### erreur du programmeur ####\n");
                }
            }
                break;

            // =================================================================

            case 'i': case 'I': // AFFICHAGE TABLE DES VELOS
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                afficher_table_des_velos(&bd_courante.velos);
            }
                break;

            // =================================================================

            case 'j': case 'J': // AFFICHAGE TABLE DES CLIENTS
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                afficher_table_des_clients(&bd_courante.clients);
            }
                break;

            // =================================================================

            case 'k': case 'K':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                Table_Velo *results;

                char choix;
                printf("Recherche linéaire des vélos\n");
                printf("Saisir le paramètre de recherche parmi les suivants\n");
                printf("\t1. ID du vélo\n");
                printf("\t2. Marque\n");
                printf("\t3. Type\n");
                printf("\t4. Prix par heure\n");
                printf("\t5. Disponibilité\n");
                printf("\t6. Locataire (ID)\n");
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '6');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        int id;
                        do {
                            id = lire_entier("\tSaisir la valeur de l'ID: ");
                        } while (id <= 0);
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            ID_VELO, &id);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '2':
                        char *temp_marque;
                        do {
                            temp_marque =
                                lire_chaine_dynamique("\tSaisir la marque: ");
                        } while (strlen(temp_marque) == 0);
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            MARQUE, temp_marque);
                        // display result
                        afficher_table_des_velos(results);

                        free(temp_marque);
                        break;
                    case '3':
                        char *temp_type;
                        do {
                            temp_type =
                                lire_chaine_dynamique("\tSaisir le type: ");
                        } while (strlen(temp_type) == 0);
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            TYPE, temp_type);
                        // display result
                        afficher_table_des_velos(results);
                        
                        free(temp_type);
                        break;
                    case '4':
                        double pph;
                        do {
                            pph = lire_reel("\tSaisir la valeur du prix par "
                                "heure: ");
                        } while (pph <= 0.0);
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            PRIX_PAR_HEURE, &pph);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '5':
                        int dispo;
                        printf("\tRechercher les vélos disponibles?\n"
                            "\t\t0. Non\n"
                            "\t\t1. Oui\n");
                        do {
                            dispo = lire_caractere("\t> ");
                        } while (dispo != '0' && dispo != '1');
                        dispo -= '0'; // to convert it back to int values of 0/1
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            DISPONIBLE, &dispo);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '6':
                        int id_locataire;
                        do {
                            id_locataire = lire_entier("\tSaisir la valeur du "
                                "ID du locataire: ");
                        } while (id_locataire <= 0);
                        // call search
                        results = rechercher_velo_par_parametre(&bd_courante, 
                            LOUE_PAR_CLIENT, &id_locataire);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                
                }

                // free the result pointer that we used to get search output
                free(results);
            }
                break;

            // =================================================================

            case 'l': case 'L':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Recherche par dichotomie des vélos\n(attention cette "
                    "recherche trie le tableau en ordre croissant par le "
                    "paramétre\nde votre recherche avant de commencer la "
                    "recherche)\n\n");
                Table_Velo *results;

                char choix;
                printf("Saisir le paramètre de recherche parmi les suivants\n");
                printf("\t1. ID du vélo\n");
                printf("\t2. Marque\n");
                printf("\t3. Type\n");
                printf("\t4. Prix par heure\n");
                printf("\t5. Disponibilité\n");
                printf("\t6. Locataire (ID)\n");
               
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '6');
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading


                switch (choix) {
                    case '1':
                        int id;
                        do {
                            id = lire_entier("\tSaisir la valeur de l'ID: ");
                        } while (id <= 0);
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, ID_VELO, &id);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '2':
                        char *temp_marque;
                        do {
                            temp_marque =
                                lire_chaine_dynamique("\tSaisir la marque: ");
                        } while (strlen(temp_marque) == 0);
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, MARQUE, temp_marque);
                        // display result
                        afficher_table_des_velos(results);

                        free(temp_marque);
                        break;
                    case '3':
                        char *temp_type;
                        do {
                            temp_type =
                                lire_chaine_dynamique("\tSaisir le type: ");
                        } while (strlen(temp_type) == 0);
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, TYPE, temp_type);
                        // display result
                        afficher_table_des_velos(results);
                        
                        free(temp_type);
                        break;
                    case '4':
                        double pph;
                        do {
                            pph = lire_reel("\tSaisir la valeur du prix par "
                                "heure: ");
                        } while (pph <= 0.0);
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, PRIX_PAR_HEURE, &pph);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '5':
                        int dispo;
                        printf("\tRechercher les vélos disponibles?\n"
                            "\t\t0. Non\n"
                            "\t\t1. Oui\n");
                        do {
                            dispo = lire_caractere("\t> ");
                        } while (dispo != '0' && dispo != '1');
                        dispo -= '0'; // to convert it back to int values of 0/1
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, DISPONIBLE, &dispo);
                        // display result
                        afficher_table_des_velos(results);

                        break;
                    case '6':
                        int id_locataire;
                        do {
                            id_locataire = lire_entier("\tSaisir la valeur du "
                                "ID du locataire: ");
                        } while (id_locataire <= 0);
                        // call search
                        results = recherche_dichotomique_velo_par_parametre(
                            &bd_courante, LOUE_PAR_CLIENT, &id_locataire);
                        // display result
                        afficher_table_des_velos(results);
                        break;
                
                }

                // free the result pointer that we used to get search output
                free(results);

            }
                break;


            // =================================================================

            case 'm': case 'M':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                Table_Client *results;

                char choix;
                printf("Recherche linéaire des clients\n");
                printf("Saisir le paramètre de recherche parmi les suivants\n");
                printf("\t1. ID du client\n");
                printf("\t2. Nom\n");
                printf("\t3. Prénom\n");
                printf("\t4. Téléphone\n");
                printf("\t5. Vélo loué (ID)\n");
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '5');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        int id;
                        do {
                            id = lire_entier("\tSaisir la valeur de l'ID: ");
                        } while (id <= 0);
                        // call search
                        results = rechercher_client_par_parametre(&bd_courante, 
                            ID_CLIENT, &id);
                        // display result
                        afficher_table_des_clients(results);

                        break;
                    case '2':
                        char *temp_nom;
                        do {
                            temp_nom =
                                lire_chaine_dynamique("\tSaisir le Nom: ");
                        } while (strlen(temp_nom) == 0);
                        // call search
                        results = rechercher_client_par_parametre(&bd_courante, 
                            NOM, temp_nom);
                        // display result
                        afficher_table_des_clients(results);

                        free(temp_nom);
                        break;
                    case '3':
                        char *temp_prenom;
                        do {
                            temp_prenom =
                                lire_chaine_dynamique("\tSaisir le Prénom: ");
                        } while (strlen(temp_prenom) == 0);
                        // call search
                        results = rechercher_client_par_parametre(&bd_courante, 
                            PRENOM, temp_prenom);
                        // display result
                        afficher_table_des_clients(results);
                        
                        free(temp_prenom);
                        break;
                    case '4':
                        char *temp_telephone;
                        do {
                            temp_telephone =
                                lire_chaine_dynamique("\tSaisir le téléphone:"
                                    " ");
                        } while (strlen(temp_telephone) == 0);
                        // call search
                        results = rechercher_client_par_parametre(&bd_courante, 
                            TELEPHONE, temp_telephone);
                        // display result
                        afficher_table_des_clients(results);

                        free(temp_telephone);
                        break;
                    case '5':
                        int id_velo_loue;
                        do {
                            id_velo_loue = lire_entier("\tSaisir la valeur du "
                                "ID du vélo loué: ");
                        } while (id_velo_loue <= 0);
                        // call search
                        results = rechercher_client_par_parametre(&bd_courante, 
                            LOUE_PAR_CLIENT, &id_velo_loue);
                        // display result
                        afficher_table_des_clients(results);

                        break;
                
                }

                // free the result pointer that we used to get search output
                free(results);
            }
                break;

            // =================================================================

            case 'n': case 'N':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Recherche par dichotomie des clients\n(attention cette "
                    "recherche trie le tableau en ordre croissant par le "
                    "paramétre\nde votre recherche avant de commencer la "
                    "recherche)\n\n");
                Table_Client *results;

                char choix;
                printf("Saisir le paramètre de recherche parmi les suivants\n");
                printf("\t1. ID du client\n");
                printf("\t2. Nom\n");
                printf("\t3. Prénom\n");
                printf("\t4. Téléphone\n");
                printf("\t5. Vélo loué (ID)\n");
               
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '5');
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading


                switch (choix) {
                    case '1':
                        int id;
                        do {
                            id = lire_entier("\tSaisir la valeur de l'ID: ");
                        } while (id <= 0);
                        // call search
                        results = recherche_dichotomique_client_par_parametre(
                            &bd_courante, ID_CLIENT, &id);
                        // display result
                        afficher_table_des_clients(results);

                        break;
                    case '2':
                        char *temp_nom;
                        do {
                            temp_nom =
                                lire_chaine_dynamique("\tSaisir le Nom: ");
                        } while (strlen(temp_nom) == 0);
                        // call search
                        results = recherche_dichotomique_client_par_parametre(
                            &bd_courante, NOM, temp_nom);
                        // display result
                        afficher_table_des_clients(results);

                        free(temp_nom);
                        break;
                    case '3':
                        char *temp_prenom;
                        do {
                            temp_prenom =
                                lire_chaine_dynamique("\tSaisir le Prénom: ");
                        } while (strlen(temp_prenom) == 0);
                        // call search
                        results = recherche_dichotomique_client_par_parametre(
                            &bd_courante, PRENOM, temp_prenom);
                        // display result
                        afficher_table_des_clients(results);
                        
                        free(temp_prenom);
                        break;
                    case '4':
                        char *temp_telephone;
                        do {
                            temp_telephone =
                                lire_chaine_dynamique("\tSaisir le téléphone:"
                                    " ");
                        } while (strlen(temp_telephone) == 0);
                        // call search
                        results = recherche_dichotomique_client_par_parametre(
                            &bd_courante, TELEPHONE, temp_telephone);
                        // display result
                        afficher_table_des_clients(results);

                        free(temp_telephone);
                        break;
                    case '5':
                        int id_velo_loue;
                        do {
                            id_velo_loue = lire_entier("\tSaisir la valeur du "
                                "ID du vélo loué: ");
                        } while (id_velo_loue <= 0);
                        // call search
                        results = recherche_dichotomique_client_par_parametre(
                            &bd_courante, LOUE_PAR_CLIENT, &id_velo_loue);
                        // display result
                        afficher_table_des_clients(results);

                        break;
                }

                // free the result pointer that we used to get search output
                free(results);

            }
                break;

            // =================================================================
            
            case 'o': case 'O':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Tri paramétré des vélos\n");
                // parametre d'ordre (croissant, decroissant)
                // parametre du tri (id, marque, type, pph, "dispo", locataire)
                printf("\tTri par ordre:\n"
                    "\t\t1. Croissant\n"
                    "\t\t2. Décroissant\n");
                char ordre;
                do {
                    ordre = lire_caractere("\t> ");
                } while (ordre != '1' && ordre != '2');
                ordre -= '0'; // to convert it back to int values of 1/2

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Saisir le paramètre de tri parmi les suivants\n");
                printf("\t1. ID du vélo\n");
                printf("\t2. Marque\n");
                printf("\t3. Type\n");
                printf("\t4. Prix par heure\n");
                printf("\t5. Disponibilité\n");
                printf("\t6. Locataire (ID)\n");
                char choix;
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '6');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        trier_tableau_des_velos(&bd_courante, ordre,  ID_VELO);
                        break;
                    case '2':
                        trier_tableau_des_velos(&bd_courante, ordre, MARQUE);
                        break;
                    case '3':
                        trier_tableau_des_velos(&bd_courante, ordre, TYPE);
                        break;
                    case '4':
                        trier_tableau_des_velos(&bd_courante, ordre, 
                            PRIX_PAR_HEURE);
                        break;
                    case '5':
                        trier_tableau_des_velos(&bd_courante, ordre,
                            DISPONIBLE);
                        break;
                    case '6':
                        trier_tableau_des_velos(&bd_courante, ordre,
                            LOUE_PAR_CLIENT);
                        break;
                
                }
                printf("Tri de la table des vélos réussi.\n\n");
            }
                break;
                
            // =================================================================

            case 'p': case 'P':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Tri paramétré des clients\n");
                // parametre d'ordre (croissant, decroissant)
                // parametre du tri (id, nom, prenom, telephone, duree_acc, 
                // montant_acc, velo_loue)

                printf("\tTri par ordre:\n"
                    "\t\t1. Croissant\n"
                    "\t\t2. Décroissant\n");
                char ordre;
                do {
                    ordre = lire_caractere("\t> ");
                } while (ordre != '1' && ordre != '2');
                ordre -= '0'; // to convert it back to int values of 1/2

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                printf("Saisir le paramètre de tri parmi les suivants\n");
                printf("\t1. ID du client\n");
                printf("\t2. Nom\n");
                printf("\t3. Prénom\n");
                printf("\t4. Téléphone\n");
                printf("\t5. Durée accumulée de location\n");
                printf("\t6. Montant accumulée de location\n");
                char choix;
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '6');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        trier_tableau_des_clients(&bd_courante, ordre,
                            ID_CLIENT);
                        break;
                    case '2':
                        trier_tableau_des_clients(&bd_courante, ordre, NOM);
                        break;
                    case '3':
                        trier_tableau_des_clients(&bd_courante, ordre, PRENOM);
                        break;
                    case '4':
                        trier_tableau_des_clients(&bd_courante, ordre,
                            TELEPHONE);
                        break;
                    case '5':
                        trier_tableau_des_clients(&bd_courante, ordre,
                            DUREE_ACC);
                        break;
                    case '6':
                        trier_tableau_des_clients(&bd_courante, ordre,
                            MONTANT_ACC);
                        break;
                    case '7':
                        trier_tableau_des_clients(&bd_courante, ordre,
                            VELO_LOUE_ID);
                        break;
                
                }
                printf("Tri de la table des clients réussi.\n\n");
            }
                break;

            // =================================================================

            case 'q': case 'Q':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                char choix;
                printf("Déduplication des vélos\n");
                printf("Saisir le paramètre de déduplication parmi les "
                    "suivants \n(/!\\attention la déduplication ne garde que la "
                    "première occcurence)\n");
                printf("\t1. Marque\n");
                printf("\t2. Type\n");
                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '2');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        char *temp_marque;
                        do {
                            temp_marque =
                                lire_chaine_dynamique("\tSaisir la marque: ");
                        } while (strlen(temp_marque) == 0);
                        switch (dedupliquer_velo_par_parametre(&bd_courante, 
                            MARQUE, temp_marque)) {
                            case ERR_VELO_NOT_FOUND:
                                printf("\n### ERREUR: Echéc de déduplication "
                                    "du vélo, vélo introuvable\n\n");
                                break;
                            case ERR_VELO_DEJA_LOUE:
                                printf("\n### ALERTE: Quelques vélos ne sont "
                                    "pas supprimés car ils sont déjà loués, "
                                    "ils doivent être retournés d'abord\n\n");
                                break;
                            case 0:
                                printf("Déduplication réussie.\n\n");
                                    
                        }

                        free(temp_marque);
                        break;
                    case '2':
                        char *temp_type;
                        do {
                            temp_type =
                                lire_chaine_dynamique("\tSaisir le type: ");
                        } while (strlen(temp_type) == 0);
                        switch (dedupliquer_velo_par_parametre(&bd_courante, 
                            TYPE, temp_type)) {
                            case ERR_VELO_NOT_FOUND:
                                printf("\n### ERREUR: Echéc de déduplication "
                                    "du vélo, vélo introuvable\n\n");
                                break;
                            case ERR_VELO_DEJA_LOUE:
                                printf("\n### ALERTE: Quelques vélos ne sont "
                                    "pas supprimés car ils sont déjà loués, \n"
                                    "ils doivent être retournés d'abord\n\n");
                                break;
                            case 0:
                                printf("Déduplication réussie.\n\n");
                                    
                        }

                        free(temp_type);
                        break;
                }

            }
                break;

            // =================================================================

            case 'r': case 'R':
            {
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                char choix;
                printf("Déduplication des clients\n");
                printf("Saisir le paramètre de déduplication parmi les "
                    "suivants \n(/!\\attention la déduplication ne garde que la "
                    "première occcurence)\n");
                printf("\t1. Nom\n");
                printf("\t2. Prénom\n");

                do {
                    choix = lire_caractere("\t> ");
                } while (choix < '1' ||  choix > '2');

                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading

                switch (choix) {
                    case '1':
                        char *temp_nom;
                        do {
                            temp_nom =
                                lire_chaine_dynamique("\tSaisir le Nom: ");
                        } while (strlen(temp_nom) == 0);
                        switch (dedupliquer_client_par_parametre(&bd_courante, 
                            NOM, temp_nom)) {
                            case ERR_CLIENT_NOT_FOUND:
                                printf("\n### ERREUR: Echéc de déduplication "
                                    "du client, client introuvable\n\n");
                                break;
                            case ERR_CLIENT_LOUE_DEJA_1VELO:
                                printf("\n### ALERTE: Quelques clients ne sont "
                                    "pas supprimés car ils louent des vélos, \n"
                                    "ils doivent les retourner d'abord\n\n");
                                break;
                            case 0:
                                printf("Déduplication réussie.\n\n");
                                    
                        }

                        free(temp_nom);
                        break;
                    case '2':
                        char *temp_prenom;
                        do {
                            temp_prenom =
                                lire_chaine_dynamique("\tSaisir le Prénom: ");
                        } while (strlen(temp_prenom) == 0);
                        switch (dedupliquer_client_par_parametre(&bd_courante, 
                            PRENOM, temp_prenom)) {
                            case ERR_CLIENT_NOT_FOUND:
                                printf("\n### ERREUR: Echéc de déduplication "
                                    "du client, client introuvable\n\n");
                                break;
                            case ERR_CLIENT_LOUE_DEJA_1VELO:
                                printf("\n### ALERTE: Quelques clients ne sont "
                                    "pas supprimés car ils louent des vélos, \n"
                                    "ils doivent les retourner d'abord\n\n");
                                break;
                            case 0:
                                printf("Déduplication réussie.\n\n");
                                    
                        }

                        free(temp_prenom);
                        break;
                }

            }
                break;

            // =================================================================

            case 't': case 'T':

            // =================================================================

            case 's': case 'S':
            
            // =================================================================

            case 'u': case 'U':

            // =================================================================

            case 'v': case 'V':
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                // TODO: move this out of here
                printf("Affichage d'un velo:\n");
                int id = lire_entier("Saisir l'ID: ");
                afficher_velo_par_id(&bd_courante, id);
                break;

            // =================================================================

            case 'w': case 'W':
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

            // =================================================================

            case 'x': case 'X':
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
                    if (bd_courante.clients.tab_client[i].velo_loue_id != 0)
                        printf("\tid du vélo loué = %d\n",
                            bd_courante.clients.tab_client[i].velo_loue_id);
                }
                printf("\n\n");
                break;

            // =================================================================

            case 'y': case 'Y':
                printf("--- Au revoir ---\n\n");
                rester = 0;
                break;


            // =================================================================

            case 'z': case 'Z':
                while ((c = getchar()) != '\n' && c != EOF)
                    ;  // flush buffer before reading
                print_menu();
                
        }
    } while (rester);
    return 0;
}
