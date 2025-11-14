#include <malloc.h>
#include <stdio.h>
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
    bd->clients.tab_client[i].velo_loue_id = id_velo;
    return 0;
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
    Velo *tmp_velo = realloc(bd->velos.tab_velo, bd->velos.capacity * 2);
    if (tmp_velo == NULL) {
        return -1;
    }
    bd->velos.capacity *= 2;
    bd->velos.tab_velo = tmp_velo;
    return 0;
}