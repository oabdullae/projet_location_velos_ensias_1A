#ifndef VELO_H
#define VELO_H

#include "structs.h"

typedef enum {
    MARQUE = 0,
    TYPE,
    PRIX_PAR_HEURE,
    DISPONIBLE,
    ID_VELO,
    LOUE_PAR_CLIENT
} DONNEES_VELO;


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
int supprimer_velo_par_id(Base_Donnee_Location *bd, int id);
int modifier_tarif_velo(Base_Donnee_Location *bd, int id, double nouveau_tarif);
int trouver_velo_par_id(Base_Donnee_Location *bd, int id);
int louer_velo_par_client(Base_Donnee_Location *bd, int id_velo, int id_client);
void afficher_table_des_velos(Table_Velo *tv);
Table_Velo *rechercher_velo_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);
void trier_tableau_des_velos(Base_Donnee_Location *bd, int ordre,
    int type_parametre);
Table_Velo *recherche_dichotomique_velo_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);
int dedupliquer_velo_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);

#endif
