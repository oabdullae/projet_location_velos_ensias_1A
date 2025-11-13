#include <malloc.h>
#include <stdio.h>
#include "velo.h"

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
    bd->velos.tab_velo[bd->velos.size].disponible = 1; // disponible quand nouveau
    bd->velos.tab_velo[bd->velos.size].loue_par_client_id = 0; // non loué quand nouveau
    ++bd->velos.increm_id;
    ++bd->velos.size;
    return 0;
}

// TODO: to be upgraded to accept any parameter not just id
void afficher_velo_par_id(Base_Donnee_Location *bd, int id) {
    int i;
    for (i = 0; i < bd->velos.size; ++i) {
        if (bd->velos.tab_velo[i].id == id) {
            break;
        }
    }
    if (i != bd->velos.size) { // meaning we didnt run the entire loop till the last i
                               // increment, which means velo of "id" was found
        printf("\tid = %d\n", bd->velos.tab_velo[i].id);
        printf("\tmarque = %s\n", bd->velos.tab_velo[i].marque);
        printf("\ttype = %s\n", bd->velos.tab_velo[i].type);
        printf("\tprix par heure = %.2lf\n", bd->velos.tab_velo[i].prix_par_heure);
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

int doubler_taille_tab_velo(Base_Donnee_Location *bd) {
    bd->velos.capacity *= 2;
    Velo *tmp_velo = realloc(bd->velos.tab_velo, bd->velos.capacity);
    if (tmp_velo == NULL) {
        return -1;
    }
    bd->velos.tab_velo = tmp_velo;
    return 0;
}