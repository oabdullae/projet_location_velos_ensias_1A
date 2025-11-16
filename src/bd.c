#include <malloc.h>
#include "bd.h"

void init_base_donnee(Base_Donnee_Location *bd) {
    bd->velos.capacity = bd->clients.capacity = 4;
    bd->velos.size = bd->clients.size = 0;
    bd->velos.increm_id = bd->clients.increm_id = 1;
    bd->velos.tab_velo = malloc(bd->velos.capacity * sizeof(Velo));
    bd->clients.tab_client = malloc(bd->clients.capacity * sizeof(Client));
}

void deallocate_base_donnee(Base_Donnee_Location *bd) {
    for (int i = 0; i < bd->velos.size; ++i) {
        free(bd->velos.tab_velo[i].marque);
        free(bd->velos.tab_velo[i].type);
    }
    for (int i = 0; i < bd->clients.size; ++i) {
        free(bd->clients.tab_client[i].nom);
        free(bd->clients.tab_client[i].prenom);
        free(bd->clients.tab_client[i].telephone);
    }
    free(bd->velos.tab_velo);
    free(bd->clients.tab_client);
}