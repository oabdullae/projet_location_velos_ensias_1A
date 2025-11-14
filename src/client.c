#include <malloc.h>

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

    if (bd->clients.tab_client[i].velo_loue_id == 0)
        return ERR_CLIENT_NOT_RENTING;

    

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
    Client *tmp_client = realloc(bd->clients.tab_client, bd->clients.capacity * 2);
    if (tmp_client == NULL) {
        return -1;
    }
    bd->clients.capacity *= 2;
    bd->clients.tab_client = tmp_client;
    return 0;
}