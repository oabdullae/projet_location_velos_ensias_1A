#ifndef VELO_H
#define VELO_H

#include "structs.h"

/* 
typedef struct Velo {
    int id;
    char *marque; // marque du velo
    char *type; // (VTT, route, electrique ...)
    double prix_par_heure; // tarif de location
    int disponible; // 1 = disponible, 0 = loué
    int loue_par_client_id; // -1 s'il n'est pas loué
} Velo;
*/

int ajouter_nouveau_velo(Base_Donnee_Location *bd, char *marque, char *type,
                          double prix_par_heure);
int doubler_taille_tab_velo(Base_Donnee_Location *bd);
void afficher_velo_par_id(Base_Donnee_Location *bd, int id);
#endif
