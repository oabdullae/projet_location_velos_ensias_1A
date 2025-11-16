#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum {
    CROISSANT = 1,
    DECROISSANT
} PARAM_TRI; // used by both velo and client tables

typedef struct Client {
    int id;
    char *nom, *prenom, *telephone;
    double duree_accumulee;
    double montant_accumule;
    int velo_loue_id; // 0 si le client ne loue aucun velo
} Client;

typedef struct Velo {
    int id;
    char *marque; // marque du velo
    char *type; // (VTT, route, electrique ...)
    double prix_par_heure; // tarif de location
    int disponible; // 1 = disponible, 0 = loué
    int loue_par_client_id; // 0 s'il n'est pas loué, car les IDs commencent par 1
} Velo;

// #include "client.h"
// #include "velo.h"

typedef struct {
    Velo *tab_velo;
    int capacity, size, increm_id;
} Table_Velo;

typedef struct {
    Client *tab_client;
    int capacity, size, increm_id;
} Table_Client;

typedef struct {
    Table_Velo velos;
    Table_Client clients;
    int fichier_associe; // booléen, = 1 si la base de donnee est deja associe
                         // a un fichier, et = 0 sinon
} Base_Donnee_Location;

#endif
