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
    printf("\t| | >> Saisir Z pour afficher ce menu si vous en avez << | |\n"); 
    printf("\t| | >>          plus tard dans le programme.          << | |\n");
    printf("\t| +------------------------------------------------------+ |\n");
    printf("\t| A. Gestion des vélos:                                    |\n");
    printf("\t|   a. Ajouter un nouveau vélo                             |\n");
    printf("\t|   b. Supprimer un vélo                                   |\n");
    printf("\t|   c. Modifier le tarif d'un vélo                         |\n");
    printf("\t|   d. Louer un vélo                                       |\n");
    printf("\t|   e. Retourner un vélo                                   |\n");
    printf("\t| B. Gestion des clients:                                  |\n");
    printf("\t|   f. Ajouter un client                                   |\n");
    printf("\t|   g. Supprimer un client                                 |\n");
    printf("\t|   h. Modifier les données d'un client                    |\n");
    printf("\t| C. Affichage des tables:                                 |\n");
    printf("\t|   i. Affichage de la table des vélos                     |\n");
    printf("\t|   j. Affichage de la table des clients                   |\n");
    printf("\t| D. Recherche:                                            |\n");
               // recherche par un parametre: par id? par marque? par type? ... 
    printf("\t|   k. Recherche linéaire d'un vélo                        |\n");
    printf("\t|   l. Recherche par dichotomie d'un vélo                  |\n");
            // recherche par un parametre: par id? par nom? par prenom? etc ...
    printf("\t|   m. Recherche linéaire d'un client                      |\n");
    printf("\t|   n. Recherche par dichotomie d'un client                |\n");
    printf("\t| E. Tri paramétré                                         |\n");
    printf("\t|   o. Tri des vélos                                       |\n");
    printf("\t|   p. Tri des clients                                     |\n");
    printf("\t| F. Déduplication selon un parametre                      |\n"); 
    printf("\t|   q. Déduplication dans la table des vélos               |\n");
    printf("\t|   r. Déduplication dans la table des clients             |\n");
    printf("\t| G. Sauvegarder/Importer                                  |\n"); 
    // prompt for tables with associated files
    printf("\t|   s. Sauvegarder                                         |\n");
    // sauvegarde automatique ou prompt(if no previous file is set
    //up for the current table) selon le parametrage
    printf("\t|   t. Importer                                            |\n");
    printf("\t| H. Statistiques                                          |\n");
    printf("\t|  (a implementer plus tard) u v w                         |\n");
    printf("\t| I. Outils                                                |\n");
        // sauvegarde automatique ou non
    printf("\t|  x. Parametres                                           |\n");
    printf("\t|  y. Réafficher le menu principal                         |\n");
        // sauvegarde automatique ou prompt selon le parametrage avant quitter
    printf("\t|  z. Quitter le Programme                                 |\n");
    printf("\t+----------------------------------------------------------+\n");
}

