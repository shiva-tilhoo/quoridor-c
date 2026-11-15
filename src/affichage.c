#include "affichage.h"
#include "utile.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Necessaire pour _getch()

void afficher_plateau(char plateau[TAILLE][TAILLE], t_joueur joueurs[], t_jeu *jeu) {
    printf("   Joueur actif : %s  | Jeton : %c  | Murs restants : %d\n\n\n",
           joueurs[jeu->joueur_actuel].nom,
           joueurs[jeu->joueur_actuel].pion,
           joueurs[jeu->joueur_actuel].bar_rest);

    printf("    ");
    for (int i = 0; i < TAILLE; i++) {
        if (i % 2 == 0) {
            printf("%c ", 'A' + i / 2); // Affichage des lettres pour les colonnes
        } else {
            printf("    ");
        }
    }
    printf("\n");

    for (int i = 0; i < TAILLE; i++) {
        if (i % 2 == 0) {
            printf("%2d ", (i / 2) + 1); // Affichage des chiffres pour les lignes
        } else {
            printf("   ");
        }

        for (int j = 0; j < TAILLE; j++) {
            int joueur_present = 0;
            for (int k = 0; k < jeu->nb_joueurs; k++) {
                if (i == joueurs[k].position.y && j == joueurs[k].position.x) {
                    Color(4 + k, 15); // Differencier les couleurs pour les joueurs
                    printf(" %c ", joueurs[k].pion);
                    Color(15, 0);
                    joueur_present = 1;
                    break;
                }
            }
            if (!joueur_present) {
                Color(15, 0);
                printf(" %c ", plateau[i][j]);
            }
        }
        printf("\n");
    }
}

void afficher_resume_joueurs(t_jeu *jeu) {
    printf("\nResume des joueurs :\n");
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        printf("Joueur : %s\n", jeu->joueurs[i].nom);
        printf("  - Murs restants : %d\n", jeu->joueurs[i].bar_rest);
        printf("  - Score : %d\n", jeu->joueurs[i].score);
        printf("  - Deplacements effectues : %d\n", jeu->joueurs[i].deplacements);
    }
    printf("\n");
}

void afficherMenu(int choixActuel) {
    system("cls"); // Efface l'écran (Windows)
    printf("\n===== MENU DU JEU QUORIDOR =====\n");
    printf("\n Utilisez les fleches HAUT/BAS pour naviguer, et ENTER pour selectionner.\n\n");

    char *options[] = {
        "1. Lancer une nouvelle partie",
        "2. Reprendre une partie sauvegardee",
        "3. Afficher l'aide",
        "4. Afficher les scores des joueurs",
        "5. Quitter le jeu"
    };
    int nbOptions = sizeof(options) / sizeof(options[0]);

    for (int i = 0; i < nbOptions; i++) {
        if (i == choixActuel) {
            printf(" > %s <\n", options[i]); // Mise en surbrillance
        } else {
            printf("   %s\n", options[i]);
        }
    }
}

void afficher_aide() {
    system("cls");
    printf("\n===== AIDE DU JEU =====\n");
    printf("\nLe jeu Quoridor se joue avec un plateau de 9x9 cases.\n");
    printf("Chaque joueur doit atteindre le cote oppose de sa position initiale.\n");
    printf("Vous pouvez vous deplacer ou poser des barrieres pour gener vos adversaires.\n");
    printf("\nAppuyez sur une touche pour revenir au menu...");
    _getch();
}

void afficher_scores(const char *fichier) {
    FILE *fp = fopen(fichier, "r");
    if (fp == NULL) {
        printf("Aucun score enregistre pour l'instant.\n");
        printf("\nAppuyez sur une touche pour revenir au menu...");
        _getch();
        return;
    }

    system("cls");
    printf("\n===== SCORES DES JOUEURS =====\n");
    char nom[40];
    int score;
    while (fscanf(fp, "%39s %d", nom, &score) == 2) {
        printf("Joueur: %s | Score: %d\n", nom, score);
    }

    fclose(fp);
    printf("\nAppuyez sur une touche pour revenir au menu...");
    _getch();
}
