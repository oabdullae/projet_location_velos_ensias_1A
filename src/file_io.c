#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "structs.h"

int export_bd(Base_Donnee_Location *bd) {
    FILE *file = fopen("saved_bd.loc", "w");
    if (file  == NULL) {
        return -1;
    }
    fprintf(file, "Table_Velo\n");
    fprintf(file, "capacity=%d\n", bd->velos.capacity);
    fprintf(file, "size=%d\n", bd->velos.size);
    fprintf(file, "increm_id=%d\n", bd->velos.increm_id);
    fprintf(file, "Table: ID\\n Marque\\n Type\\n Prix par heure\\nDisponible"
        "\\n Loué par client\\n---\n");
    for (int i = 0; i < bd->velos.size; ++i) {
        fprintf(file, "%d\n", bd->velos.tab_velo[i].id);
        fprintf(file, "%s\n", bd->velos.tab_velo[i].marque);
        fprintf(file, "%s\n", bd->velos.tab_velo[i].type);
        fprintf(file, "%lf\n", bd->velos.tab_velo[i].prix_par_heure);
        fprintf(file, "%d\n", bd->velos.tab_velo[i].disponible);
        fprintf(file, "%d\n", bd->velos.tab_velo[i].loue_par_client_id);
        fprintf(file, "---\n");
    }


    fprintf(file, "===\n");


    fprintf(file, "Table_Client\n");
    fprintf(file, "capacity=%d\n", bd->clients.capacity);
    fprintf(file, "size=%d\n", bd->clients.size);
    fprintf(file, "increm_id=%d\n", bd->clients.increm_id);
    fprintf(file, "Table: ID\\n Nom\\n Prénom\\n Téléphone\\n Durée accumulée"
        "\\n Montant accumulée\\n Vélo loué\\n---\n");
    for (int i = 0; i < bd->clients.size; ++i) {
        fprintf(file, "%d\n", bd->clients.tab_client[i].id);
        fprintf(file, "%s\n", bd->clients.tab_client[i].nom);
        fprintf(file, "%s\n", bd->clients.tab_client[i].prenom);
        fprintf(file, "%s\n", bd->clients.tab_client[i].telephone);
        fprintf(file, "%lf\n", bd->clients.tab_client[i].duree_accumulee);
        fprintf(file, "%lf\n", bd->clients.tab_client[i].montant_accumule);
        fprintf(file, "%d\n", bd->clients.tab_client[i].velo_loue_id);
        fprintf(file, "---\n");
    }
    fclose(file);
    return 0;
}

int import_bd(Base_Donnee_Location *bd) {
    FILE *file = fopen("saved_bd.loc", "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[256];

    // velos metadata
    fgets(line, sizeof(line), file); // skip Table_Velo

    fgets(line, sizeof(line), file);
    sscanf(line, "capacity=%d", &bd->velos.capacity);

    fgets(line, sizeof(line), file);
    sscanf(line, "size=%d", &bd->velos.size);

    fgets(line, sizeof(line), file);
    sscanf(line, "increm_id=%d", &bd->velos.increm_id);

    // skip table header line
    fgets(line, sizeof(line), file);

    // allocate velos array
    bd->velos.tab_velo = malloc(bd->velos.capacity * sizeof(Velo));

    // read velos
    for (int i = 0; i < bd->velos.size; ++i) {
        fgets(line, sizeof(line), file);
        bd->velos.tab_velo[i].id = atoi(line);

        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0;
        bd->velos.tab_velo[i].marque = malloc(strlen(line) + 1);
        strcpy(bd->velos.tab_velo[i].marque, line);

        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0;
        bd->velos.tab_velo[i].type = malloc(strlen(line) + 1);
        strcpy(bd->velos.tab_velo[i].type, line);

        fgets(line, sizeof(line), file);
        bd->velos.tab_velo[i].prix_par_heure = atof(line);

        fgets(line, sizeof(line), file);
        bd->velos.tab_velo[i].disponible = atoi(line);

        fgets(line, sizeof(line), file);
        bd->velos.tab_velo[i].loue_par_client_id = atoi(line);

        fgets(line, sizeof(line), file); // skip ---
    }

    // skip === and Table_Client
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    // clients metadata
    fgets(line, sizeof(line), file);
    sscanf(line, "capacity=%d", &bd->clients.capacity);

    fgets(line, sizeof(line), file);
    sscanf(line, "size=%d", &bd->clients.size);
    
    fgets(line, sizeof(line), file);
    sscanf(line, "increm_id=%d", &bd->clients.increm_id);

    fgets(line, sizeof(line), file); // skip table header

    // allocate clients array
    bd->clients.tab_client = malloc(bd->clients.capacity * sizeof(Client));

    // read clients
    for (int i = 0; i < bd->clients.size; ++i) {
        fgets(line, sizeof(line), file);
        bd->clients.tab_client[i].id = atoi(line);

        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0;
        bd->clients.tab_client[i].nom = malloc(strlen(line) + 1);
        strcpy(bd->clients.tab_client[i].nom, line);

        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0;
        bd->clients.tab_client[i].prenom = malloc(strlen(line) + 1);
        strcpy(bd->clients.tab_client[i].prenom, line);

        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0;
        bd->clients.tab_client[i].telephone = malloc(strlen(line) + 1);
        strcpy(bd->clients.tab_client[i].telephone, line);

        fgets(line, sizeof(line), file);
        bd->clients.tab_client[i].duree_accumulee = atof(line);

        fgets(line, sizeof(line), file);
        bd->clients.tab_client[i].montant_accumule = atof(line);

        fgets(line, sizeof(line), file);
        bd->clients.tab_client[i].velo_loue_id = atoi(line);

        fgets(line, sizeof(line), file); // skip ---
    }

    fclose(file);
    return 0;
}