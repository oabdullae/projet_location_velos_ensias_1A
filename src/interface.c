#include <stdio.h>
#include "interface.h"

void print_interface() {
    printf("interface");
}

void print_menu() {
    // chaque option peut avoir son propre mini menu avant d'effectuer
    // l'operation, et elle peut meme avoir des menus en cours de l'operation
    printf("\t+----------------------------------------------------------+\n");
    printf("\t| +------------------------------------------------------+ |\n");
    printf("\t| | >> Saisir x pour afficher ce menu si vous en avez << | |\n"); 
    printf("\t| | >>        plus tard dans le programme             << | |\n");
    printf("\t| +------------------------------------------------------+ |\n");
    printf("\t| A. Gestion des vélos:                                    |\n");
    printf("\t|   a. Ajouter un nouveau vélo                             |\n");
    printf("\t|   b. Supprimer un vélo                                   |\n");
    printf("\t|   c. Modifier les données d'un vélo                      |\n");
    printf("\t|   d. Louer un vélo                                       |\n");
    printf("\t|   e. Retourner un vélo                                   |\n");
    printf("\t| B. Gestion des clients:                                  |\n");
    printf("\t|   f. Ajouter un client                                   |\n");
    printf("\t|   g. Supprimer un client                                 |\n");
    printf("\t|   h. Modifier les données d'un client                    |\n");
    printf("\t| C. Recherche:                                            |\n");
               // recherche par un parametre: par id? par marque? par type? ... 
    printf("\t|   i. Recherche linéaire d'un vélo                        |\n");
    printf("\t|   j. Recherche par dichotomie d'un vélo                  |\n");
            // recherche par un parametre: par id? par nom? par prenom? etc ...
    printf("\t|   k. Recherche linéaire d'un client                      |\n");
    printf("\t|   l. Recherche par dichotomie d'un client                |\n");
    printf("\t| D. Tri paramétré                                         |\n");
    printf("\t|   m. Tri des vélos                                       |\n");
    printf("\t|   n. Tri des clients                                     |\n");
    printf("\t| E. Affichage des tables                                  |\n");
    printf("\t|   o. Table des vélos                                     |\n");
    printf("\t|   p. Table des clients                                   |\n");
    printf("\t| F. Sauvegarder/Importer                                  |\n"); 
                // prompt for tables with associated files
    printf("\t|   q. Sauvegarder                                         |\n");
                // sauvegarde automatique ou prompt(if no previous file is set
                //up for the current table) selon le parametrage
    printf("\t|   r. Importer                                            |\n");
    printf("\t| G. Statistiques                                          |\n");
    printf("\t|  (a implementer plus tard)                               |\n");
    printf("\t| H. Outils                                                |\n");
    printf("\t|  s. Réafficher le menu principal                         |\n");
               // sauvegarde automatique ou non
    printf("\t|  t. Parametres                                           |\n");
        // sauvegarde automatique ou prompt selon le parametrage avant quitter
    printf("\t|  u. Quitter le Programme                                 |\n");
    printf("\t+----------------------------------------------------------+\n");
}

