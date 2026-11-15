#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h> // Pour _getch()
#include "jeu.h"
#include "interaction.h"
#include "affichage.h"
#include "utile.h"

#define UP_ARROW 72
#define DOWN_ARROW 80
#define ENTER 13

int main() {
    int choixActuel = 0;
    int nbOptions = 5;
    int running = 1;

    srand((unsigned int) time(NULL));

    t_jeu *jeu = (t_jeu *) malloc(sizeof(t_jeu));
    if (!jeu) {
        printf("Erreur d'allocation mémoire pour le jeu.\n");
        return 1;
    }
    memset(jeu, 0, sizeof(t_jeu)); // Initialisation de la structure

    while (running) {
        afficherMenu(choixActuel);
        char touche = _getch();

        switch (touche) {
            case UP_ARROW: // Flèche haut
                choixActuel = (choixActuel - 1 + nbOptions) % nbOptions;
                break;

            case DOWN_ARROW: // Flèche bas
                choixActuel = (choixActuel + 1) % nbOptions;
                break;

            case ENTER: // Touche Entrée
                system("cls");
                switch (choixActuel) {
                    case 0: // Nouvelle partie
                        lancer_nouvelle_partie(jeu);
                        break;

                    case 1: // Reprendre une partie sauvegardée
                        if (reprise_partie_complete(jeu)) {
                            continuer_partie(jeu);
                        } else {
                            printf("Impossible de reprendre : aucune partie sauvegardee.\n");
                            attendre_touche();
                        }
                        break;

                    case 2: // Afficher l'aide
                        afficher_aide();
                        break;

                    case 3: // Afficher les scores
                        afficher_scores(FICHIER_SCORES);
                        break;

                    case 4: // Quitter le jeu
                        printf("Quitter le jeu. Merci d'avoir joue !\n");
                        running = 0;
                        break;
                }
                break;
        }
    }

    free(jeu);
    return 0;
}
