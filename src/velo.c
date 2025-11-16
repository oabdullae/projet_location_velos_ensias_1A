#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "velo.h"
#include "client.h"
#include "error_codes.h"



// typedef enum {
//     ID
// } PARAM_VELO;

int ajouter_nouveau_velo(Base_Donnee_Location *bd, char *marque, char *type,
                          double prix_par_heure) {
    if (bd->velos.size >= bd->velos.capacity - 1) {
        int status = doubler_taille_tab_velo(bd);
        if (status != 0) {
            return -1;
        }
    }
    bd->velos.tab_velo[bd->velos.size].id = bd->velos.increm_id;
    bd->velos.tab_velo[bd->velos.size].marque = marque;
    bd->velos.tab_velo[bd->velos.size].type = type;
    bd->velos.tab_velo[bd->velos.size].prix_par_heure = prix_par_heure;
    bd->velos.tab_velo[bd->velos.size].disponible = 1; // disponible quand 
                                                       // nouveau
    bd->velos.tab_velo[bd->velos.size].loue_par_client_id = 0; // non loué quand
                                                               // nouveau
    ++bd->velos.increm_id;
    ++bd->velos.size;
    return 0;
}

int supprimer_velo_par_id(Base_Donnee_Location *bd, int id) {
    int i = trouver_velo_par_id(bd, id);
    
    if (i == bd->velos.size)
        return ERR_VELO_NOT_FOUND;

    if (bd->velos.tab_velo[i].disponible == 0) // velo is still rented
        return ERR_VELO_DEJA_LOUE; // velo can't be deleted, it should be
                                           // returned first

    // shifts back by one velo if necessary
    for (int j = i; j < bd->velos.size - 1; ++j) {//this loop won't execute if i 
        bd->velos.tab_velo[j] = bd->velos.tab_velo[j+1]; // was last i.e. size-1
    }

    /* stupid me, hear me out, there was no reason to realloc here, just leave 
    the array as it is, doubling the size when needed takes care of its
    allocation, you only have the right to decrement the size */
    // Velo *tmp_velo = realloc(bd->velos.tab_velo, bd->velos.size-1);
    // if (tmp_velo == NULL) {
    //     return -1;
    // }
    // bd->velos.tab_velo = tmp_velo;

    --bd->velos.size;
    return 0;
}

int modifier_tarif_velo(Base_Donnee_Location *bd, int id, double nouveau_tarif)
{
    int i = trouver_velo_par_id(bd, id);
    
    if (i == bd->velos.size)
        return ERR_VELO_NOT_FOUND;

    bd->velos.tab_velo[i].prix_par_heure = nouveau_tarif;
    return 0;
}

int louer_velo_par_client(Base_Donnee_Location *bd, int id_velo, int id_client){
    int i = trouver_velo_par_id(bd, id_velo);

    if (i == bd->velos.size)
        return ERR_VELO_NOT_FOUND; // velo not found error

    int j = trouver_client_par_id(bd, id_client);

    if (j == bd->clients.size)
        return ERR_CLIENT_NOT_FOUND; // client not found error

    if (bd->velos.tab_velo[i].disponible == 0)
        return ERR_VELO_DEJA_LOUE; // already rented error

    if (bd->clients.tab_client[j].velo_loue_id != 0)
        return ERR_CLIENT_LOUE_DEJA_1VELO;
    
    bd->velos.tab_velo[i].loue_par_client_id = id_client;
    bd->velos.tab_velo[i].disponible = 0; // loué donc non disponible
    bd->clients.tab_client[j].velo_loue_id = id_velo;
    return 0;
}

void afficher_table_des_velos(Table_Velo *velos_array) {
    if (velos_array->size == 0)
        printf("La table est vide\n\n");
    else {
        printf("Affichage de la table des vélos\n");
        printf("+----+--------------------+--------------------+------------+--"
            "--------+-------+\n");
        printf("| ID |       Marque       |        Type        |PrixParHeure|Di"
            "sponible|LouéPar|\n");
        printf("+----+--------------------+--------------------+------------+--"
            "--------+-------+\n");
        for (int i = 0; i < velos_array->size; ++i) {
            printf("|%3d ", velos_array->tab_velo[i].id);
            // ---------------------------------------------------------
            int marque_length = strlen(velos_array->tab_velo[i].marque);
            char etc = ' ';
            if (marque_length > 20) {
                marque_length = 19;
                etc = '>';
            }
            printf("|%.*s", marque_length,
                velos_array->tab_velo[i].marque);
            if (marque_length < 19)
                for (int j = 0; j < 19 - marque_length; ++j)
                    putchar(' ');
            putchar(etc);
            // ---------------------------------------------------------
            int type_length = strlen(velos_array->tab_velo[i].type);
            etc = ' ';
            if (type_length > 20) {
                type_length = 19;
                etc = '>';
            }
            printf("|%.*s", type_length,
                velos_array->tab_velo[i].type);
            if (type_length < 19)
                for (int j = 0; j < 19 - type_length; ++j)
                    putchar(' ');
            putchar(etc);
            // ---------------------------------------------------------
            printf("| %-11.2lf",
                velos_array->tab_velo[i].prix_par_heure);
            // ---------------------------------------------------------
            printf("|   %s    ",
                (velos_array->tab_velo[i].disponible)?"oui":"non");
            // ---------------------------------------------------------
            if (!velos_array->tab_velo[i].disponible)
                printf("|  %3d  |\n",
                    velos_array->tab_velo[i].loue_par_client_id);
            else
                printf("|   --  |\n");
        }
        printf("+----+--------------------+--------------------+------------+--"
            "--------+-------+\n");
    }

}

// returns a struct that contains a dynamically allocated Velo array that stores
// all the matches to be then passed to the display function
Table_Velo *rechercher_velo_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre) {
    // les parametres possibles sont:
        // MARQUE = 0,
        // TYPE,
        // PRIX_PAR_HEURE,
        // DISPONIBLE,
        // ID_VELO,
        // LOUE_PAR_CLIENT
    Table_Velo *results = malloc(sizeof(Table_Velo));
    results->capacity = 1;
    results->increm_id = 1;
    results->tab_velo = NULL;
    results->size = 0;
    switch (type_parametre) {
        case MARQUE:
            char *str_marque = (char *)parametre;
            for (int i = 0; i < bd->velos.size; ++i) {
                if (!strcmp(str_marque, bd->velos.tab_velo[i].marque)) {
                    ++results->size;
                    results->tab_velo = realloc(results->tab_velo, 
                        results->size * sizeof(Velo));
                    results->tab_velo[results->size-1] =
                        bd->velos.tab_velo[i];
                }
            }
            break;
        case TYPE:
            char *str_type = (char *)parametre;
            for (int i = 0; i < bd->velos.size; ++i) {
                if (!strcmp(str_type, bd->velos.tab_velo[i].type)) {
                    ++results->size;
                    results->tab_velo = realloc(results->tab_velo, 
                        results->size * sizeof(Velo));
                    results->tab_velo[results->size-1] =
                        bd->velos.tab_velo[i];
                }
            }
            break;
        case PRIX_PAR_HEURE:
        double *pph = (double *)parametre;
        for (int i = 0; i < bd->velos.size; ++i) {
            if (*pph == bd->velos.tab_velo[i].prix_par_heure) {
                ++results->size;
                results->tab_velo = realloc(results->tab_velo, 
                    results->size * sizeof(Velo));
                results->tab_velo[results->size-1] =
                    bd->velos.tab_velo[i];
            }
        }
        break;
        case DISPONIBLE:
            int *dispo = (int *)parametre;
            for (int i = 0; i < bd->velos.size; ++i) {
                if (*dispo == bd->velos.tab_velo[i].disponible) {
                    ++results->size;
                    results->tab_velo = realloc(results->tab_velo, 
                        results->size * sizeof(Velo));  // add one more 
                                                                // slot
                    results->tab_velo[results->size-1] =
                        bd->velos.tab_velo[i]; // store matching velo in results
                }
            }
            break;
        case ID_VELO:
            int *id = (int *)parametre;
            for (int i = 0; i < bd->velos.size; ++i) {
                if (*id == bd->velos.tab_velo[i].id) {
                    ++results->size;
                    results->tab_velo = realloc(results->tab_velo, 
                        results->size * sizeof(Velo));  // add one more 
                                                                // slot
                    results->tab_velo[results->size-1] =
                        bd->velos.tab_velo[i]; // store matching velo in results
                    break; // cuz id is unique, no need to keep iterating
                }
            }
            break;
        case LOUE_PAR_CLIENT:
            int *loue_par_client = (int *)parametre;
            for (int i = 0; i < bd->velos.size; ++i) {
                if (*loue_par_client == 
                    bd->velos.tab_velo[i].loue_par_client_id) {
                    ++results->size;
                    results->tab_velo = realloc(results->tab_velo, 
                        results->size * sizeof(Velo));  // add one more 
                                                                // slot
                    results->tab_velo[results->size-1] =
                        bd->velos.tab_velo[i]; // store matching velo in results
                    break; // cuz a bike can be rented by only one client 
                }
            }
            break;
    }
    return results;
}

// TODO: to be upgraded to accept any parameter not just id
void afficher_velo_par_id(Base_Donnee_Location *bd, int id) {
    int i = trouver_velo_par_id(bd, id);

    if (i != bd->velos.size) { // meaning we didnt run the entire loop till the 
                               // last i increment, which means velo of "id" was
                               //  found
        printf("\tid = %d\n", bd->velos.tab_velo[i].id);
        printf("\tmarque = %s\n", bd->velos.tab_velo[i].marque);
        printf("\ttype = %s\n", bd->velos.tab_velo[i].type);
        printf("\tprix par heure = %.2lf\n",
               bd->velos.tab_velo[i].prix_par_heure);
        printf("\tdiponible? = %s\n",
            (bd->velos.tab_velo[i].disponible) ? "oui" : "non");
        if (!bd->velos.tab_velo[i].disponible)
            printf("\tloué par le client avec id = %d\n",
                   bd->velos.tab_velo[i].loue_par_client_id);
    }
    else {
        printf("Velo avec id=\"%d\" non trouvé\n", id);
    }
}

int trouver_velo_par_id(Base_Donnee_Location *bd, int id) {
    int i;
    for (i = 0; i < bd->velos.size; ++i) {
        if (bd->velos.tab_velo[i].id == id) {
            break;
        }
    }
    return i;
}

int doubler_taille_tab_velo(Base_Donnee_Location *bd) {
    Velo *tmp_velo = realloc(bd->velos.tab_velo,
        bd->velos.capacity * 2 * sizeof(Velo));
    if (tmp_velo == NULL) {
        return -1;
    }
    bd->velos.capacity *= 2;
    bd->velos.tab_velo = tmp_velo;
    return 0;
}