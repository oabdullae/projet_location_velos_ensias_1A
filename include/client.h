#ifndef CLIENT_H
#define CLIENT_H

#include "structs.h"

typedef enum {
    NOM = 0,
    PRENOM,
    TELEPHONE,
    ID_CLIENT,
    VELO_LOUE_ID,
    DUREE_ACC,
    MONTANT_ACC
} DONNEES_CLIENT;

/* 
typedef struct Client {
    int id;
    char *nom, *prenom, *telephone;
    double duree_location;
    double montant_location;
    double velo_loue_id;
} Client; 
*/


int trouver_client_par_id(Base_Donnee_Location *bd, int id_client);
int retourner_velo_par_client(Base_Donnee_Location *bd, int id_client,
    double duree, double *montant);
int doubler_taille_tab_client(Base_Donnee_Location *bd);
int ajouter_nouveau_client(Base_Donnee_Location *bd, char *nom, char *prenom,
    char *telephone);
int supprimer_client_par_id(Base_Donnee_Location *bd, int id);
int modifier_client(Base_Donnee_Location *bd, int id, int donnee_a_modifier,
    char *nouvelle_donnee);
void afficher_table_des_clients(Table_Client *tc);

Table_Client *rechercher_client_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);
void trier_tableau_des_clients(Base_Donnee_Location *bd, int ordre,
    int type_parametre);
Table_Client *recherche_dichotomique_client_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);
int dedupliquer_client_par_parametre(Base_Donnee_Location *bd,
    int type_parametre, void *parametre);

#endif
