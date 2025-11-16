#include <malloc.h>
#include <string.h>

#include "client.h"
#include "structs.h"
#include "velo.h"
#include "error_codes.h"

int retourner_velo_par_client(Base_Donnee_Location *bd, int id_client,
    double duree, double *montant) {
    // find the client
    int i = trouver_client_par_id(bd, id_client);

    if (i == bd->clients.size)
        return ERR_CLIENT_NOT_FOUND; // client not found error

    if (bd->clients.tab_client[i].velo_loue_id != 0)
        return ERR_CLIENT_LOUE_DEJA_1VELO;

    

    int j = trouver_velo_par_id(bd, bd->clients.tab_client[i].velo_loue_id);

    // if (j == bd->velos.size) // impossible, if client got its bike through 
                            // the src/velo.c:louer_velo_par_client() function
    //     return ERR_VELO_NOT_FOUND; // velo not found error
        
    // also velo will be not "disponible", for sure if renting happened through
    // the src/velo.c:louer_velo_par_client() function
    
    *montant = duree * bd->velos.tab_velo[j].prix_par_heure;
    bd->clients.tab_client[i].duree_accumulee += duree;
    bd->clients.tab_client[i].montant_accumule += *montant;
    bd->clients.tab_client[i].velo_loue_id = 0; // no rented bike
    bd->velos.tab_velo[j].disponible = 1; // bike available
    bd->velos.tab_velo[j].loue_par_client_id = 0; // par personne
    return 0;
}

// int ajouter_nouveau_client() {
//     client.velo_loue_id should be initialized at 0, meaning no rented bike by
//     default
//     init duree_accumulee by 0.0
//     init montant_accumule by 0.0
// }
int ajouter_nouveau_client(Base_Donnee_Location *bd, char *nom, char *prenom,
    char *telephone) {
    if (bd->clients.size >= bd->clients.capacity - 1) {
        int status = doubler_taille_tab_client(bd);
        if (status != 0) {
            return -1;
        }
    }

    bd->clients.tab_client[bd->clients.size].id = bd->clients.increm_id;
    bd->clients.tab_client[bd->clients.size].nom = nom;
    bd->clients.tab_client[bd->clients.size].prenom = prenom;
    bd->clients.tab_client[bd->clients.size].telephone = telephone;
    bd->clients.tab_client[bd->clients.size].duree_accumulee = 0.0;
    bd->clients.tab_client[bd->clients.size].montant_accumule = 0.0;
    bd->clients.tab_client[bd->clients.size].velo_loue_id = 0;
    ++bd->clients.increm_id;
    ++bd->clients.size;
    return 0;
}

int supprimer_client_par_id(Base_Donnee_Location *bd, int id) {
    int i = trouver_client_par_id(bd, id);
    
    if (i == bd->clients.size)
        return ERR_CLIENT_NOT_FOUND;
    
    
    if (bd->clients.tab_client[i].velo_loue_id != 0) // client is still renting
                                                    // a bike
        return ERR_CLIENT_LOUE_DEJA_1VELO; // client can't be deleted, they 
                                           // should return the bike first

    // shifts back by one client if necessary
    for (int j = i; j < bd->clients.size - 1; ++j) {//this loop won't execute if i 
        bd->clients.tab_client[j] = bd->clients.tab_client[j+1]; // was last i.e. size-1
    }

    --bd->clients.size;
    return 0;
}

int modifier_client(Base_Donnee_Location *bd, int id, int donnee_a_modifier,
    char *nouvelle_donnee) {
    int i = trouver_client_par_id(bd, id);
    
    if (i == bd->clients.size)
        return ERR_CLIENT_NOT_FOUND;

    switch (donnee_a_modifier) {
        case NOM:
            free(bd->clients.tab_client[i].nom);
            bd->clients.tab_client[i].nom = nouvelle_donnee;
            break;
        case PRENOM:
            free(bd->clients.tab_client[i].prenom);
            bd->clients.tab_client[i].prenom = nouvelle_donnee;
            break;
        case TELEPHONE:
            free(bd->clients.tab_client[i].telephone);
            bd->clients.tab_client[i].telephone = nouvelle_donnee;
            break;
    }
    return 0;
}

void afficher_table_des_clients(Table_Client *clients_array) {
    if (clients_array->size == 0)
        printf("La table est vide\n\n");
    else {
        printf("Affichage de la table des clients\n");
        printf("+----+-------------+-------------+-------------+--------+------"
            "----+-----------+\n");
        printf("| ID |     Nom     |    Prenom   |  Telephone  |DureeAcc|Montan"
            "tAcc|LocataireDe|\n");
        printf("+----+-------------+-------------+-------------+--------+------"
            "----+-----------+\n");
        for (int i = 0; i < clients_array->size; ++i) {
            printf("|%3d ", clients_array->tab_client[i].id);
            // ---------------------------------------------------------
            int nom_length = strlen(clients_array->tab_client[i].nom);
            char etc = ' ';
            if (nom_length > 13) {
                nom_length = 12;
                etc = '>';
            }
            printf("|%.*s", nom_length,
                clients_array->tab_client[i].nom);
            if (nom_length < 12)
                for (int j = 0; j < 12 - nom_length; ++j)
                    putchar(' ');
            putchar(etc);
            // ---------------------------------------------------------
            int prenom_length = strlen(clients_array->tab_client[i].prenom);
            etc = ' ';
            if (prenom_length > 13) {
                prenom_length = 12;
                etc = '>';
            }
            printf("|%.*s", prenom_length,
                clients_array->tab_client[i].prenom);
            if (prenom_length < 12)
                for (int j = 0; j < 12 - prenom_length; ++j)
                    putchar(' ');
            putchar(etc);
            // ---------------------------------------------------------
            int telephone_length = 
                strlen(clients_array->tab_client[i].telephone);
            etc = ' ';
            if (telephone_length > 13) {
                telephone_length = 12;
                etc = '>';
            }
            printf("|%.*s", telephone_length,
                clients_array->tab_client[i].telephone);
            if (telephone_length < 12)
                for (int j = 0; j < 12 - telephone_length; ++j)
                    putchar(' ');
            putchar(etc);
            // ---------------------------------------------------------
            printf("| %-7.2lf",
                clients_array->tab_client[i].duree_accumulee);
            // ---------------------------------------------------------
            printf("| %-9.2lf",
                clients_array->tab_client[i].montant_accumule);
            // ---------------------------------------------------------
            if (clients_array->tab_client[i].velo_loue_id != 0)
                printf("|   %5d   |\n",
                    clients_array->tab_client[i].velo_loue_id);
            else
                printf("|    ---    |\n");
        }
        printf("+----+-------------+-------------+-------------+--------+------"
            "----+-----------+\n");
    }

}

Table_Client *rechercher_client_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre) {
    // les parametres possibles sont:
        // NOM = 0,
        // PRENOM,
        // TELEPHONE,
        // ID_CLIENT,
        // VELO_LOUE_ID
    Table_Client *results = malloc(sizeof(Table_Client));
    results->capacity = 1;
    results->increm_id = 1;
    results->tab_client = NULL;
    results->size = 0;
    switch (type_parametre) {
        case NOM:
            char *str_nom = (char *)parametre;
            for (int i = 0; i < bd->clients.size; ++i) {
                if (!strcmp(str_nom, bd->clients.tab_client[i].nom)) {
                    ++results->size;
                    results->tab_client = realloc(results->tab_client, 
                        results->size * sizeof(Client));
                    results->tab_client[results->size-1] =
                        bd->clients.tab_client[i];
                }
            }
            break;
        case PRENOM:
            char *str_prenom = (char *)parametre;
            for (int i = 0; i < bd->clients.size; ++i) {
                if (!strcmp(str_prenom, 
                    bd->clients.tab_client[i].prenom)) {
                    ++results->size;
                    results->tab_client = realloc(results->tab_client, 
                        results->size * sizeof(Client));
                    results->tab_client[results->size-1] =
                        bd->clients.tab_client[i];
                }
            }
            break;
        case TELEPHONE:
            char *str_telephone = (char *)parametre;
            for (int i = 0; i < bd->clients.size; ++i) {
                if (!strcmp(str_telephone, 
                    bd->clients.tab_client[i].telephone)) {
                    ++results->size;
                    results->tab_client = realloc(results->tab_client, 
                        results->size * sizeof(Client));
                    results->tab_client[results->size-1] =
                        bd->clients.tab_client[i];
                }
            }
            break;
        case ID_CLIENT:
            int *id = (int *)parametre;
            for (int i = 0; i < bd->clients.size; ++i) {
                if (*id == bd->clients.tab_client[i].id) {
                    ++results->size;
                    results->tab_client = realloc(results->tab_client, 
                        results->size * sizeof(Client));    // add one more 
                                                            // slot
                    results->tab_client[results->size-1] =
                        bd->clients.tab_client[i];  // store matching client in 
                                                    // results
                    break; // cuz id is unique, no need to keep iterating
                }
            }
            break;
        case VELO_LOUE_ID:
            int *velo_loue = (int *)parametre;
            for (int i = 0; i < bd->clients.size; ++i) {
                if (*velo_loue == 
                    bd->clients.tab_client[i].velo_loue_id) {
                    ++results->size;
                    results->tab_client = realloc(results->tab_client, 
                        results->size * sizeof(Client));    // add one more 
                                                            // slot
                    results->tab_client[results->size-1] =
                        bd->clients.tab_client[i];  // store matching client in 
                                                    // results
                    break; // cuz client can rent only one bike at a time
                }
            }
            break;
    }
    return results;
}



int trouver_client_par_id(Base_Donnee_Location *bd, int id_client) {
    int i;
    for (i = 0; i < bd->clients.size; ++i) {
        if (bd->clients.tab_client[i].id == id_client) {
            break;
        }
    }
    return i;
}

int doubler_taille_tab_client(Base_Donnee_Location *bd) {
    Client *tmp_client = realloc(bd->clients.tab_client,
        bd->clients.capacity * 2 * sizeof(Client));
    if (tmp_client == NULL) {
        return -1;
    }
    bd->clients.capacity *= 2;
    bd->clients.tab_client = tmp_client;
    return 0;
}