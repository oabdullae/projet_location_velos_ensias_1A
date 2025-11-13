#ifndef CLIENT_H
#define CLIENT_H

typedef struct Client {
    int id;
    char *nom, *prenom, *telephone;
    double duree_location;
    double montant_location;
    double velo_loue_id;
} Client;


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

#endif
