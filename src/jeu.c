#include "jeu.h"
#include "utile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialiser_plateau(char plateau[TAILLE][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                plateau[i][j] = '.'; // Cases principales
            } else {
                plateau[i][j] = ' '; // Espaces pour les barrieres
            }
        }
    }
}

void initialiser_barrieres(t_joueur *joueurs, int nb_joueurs) {
    int barriere_par_joueur;

    if (nb_joueurs == 2) {
        barriere_par_joueur = 10; // Chaque joueur obtient 10 barrières
    } else if (nb_joueurs == 4) {
        barriere_par_joueur = 5; // Chaque joueur obtient 5 barrières
    } else {
        printf("Erreur : Nombre de joueurs invalide. Seuls 2 ou 4 joueurs sont autorisés.\n");
        return;
    }

    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].bar_rest = barriere_par_joueur;
    }
}

void initialiser_joueurs(t_joueur *joueur, t_jeu *jeu) {
    do {
        Color(3, 0);
        printf("Entrez le nombre de joueurs (2 ou 4) : ");
        if (scanf("%d", &jeu->nb_joueurs) != 1) {
            printf("Erreur : entree invalide. Veuillez entrer un nombre.\n");
            while (getchar() != '\n'); // Nettoie le tampon
            continue;
        }
        while (getchar() != '\n'); // Nettoie les residus
    } while (jeu->nb_joueurs != 2 && jeu->nb_joueurs != 4);

    for (int i = 0; i < jeu->nb_joueurs; i++) {
        int entree_valide = 0; // Variable pour suivre si l'entrée est valide
        do {
            printf("Entrez le nom du joueur %d : ", i + 1);

            if (scanf("%39s", joueur[i].nom) == 1) {
                // Si un mot est correctement lu
                entree_valide = 1; // Marquer l'entrée comme valide
            } else {
                printf("Erreur : entree invalide. Veuillez entrer un nom.\n");
                while (getchar() != '\n'); // Nettoie le tampon
            }
        } while (!entree_valide); // Répète jusqu'à une entrée valide

        while (getchar() != '\n'); // Nettoie les résidus du tampon

        printf("Entrez son pion (caractere unique) : ");
        if (scanf(" %c", &joueur[i].pion) != 1) {
            printf("Erreur : entree invalide pour le pion.\n");
            while (getchar() != '\n'); // Nettoie le tampon
            i--; // Refaire l'entree pour ce joueur
            continue;
        }
        while (getchar() != '\n'); // Nettoie le tampon
        joueur[i].score = 0;
        joueur[i].deplacements = 0;
        initialiser_barrieres(jeu->joueurs, jeu->nb_joueurs);
    }
}

void initialiser_positions_joueurs(t_joueur joueurs[], int nb_joueurs) {
    if (nb_joueurs == 2) {
        joueurs[0].position.x = TAILLE / 2;
        joueurs[0].position.y = 0;

        joueurs[1].position.x = TAILLE / 2;
        joueurs[1].position.y = TAILLE - 1;
    } else if (nb_joueurs == 4) {
        joueurs[0].position.x = TAILLE / 2;
        joueurs[0].position.y = 0;

        joueurs[1].position.x = TAILLE / 2;
        joueurs[1].position.y = TAILLE - 1;

        joueurs[2].position.x = 0;
        joueurs[2].position.y = TAILLE / 2;

        joueurs[3].position.x = TAILLE - 1;
        joueurs[3].position.y = TAILLE / 2;
    }
}

// Vérifie si le joueur actuel a atteint le côté opposé à son point de départ
bool verifier_victoire(t_jeu *jeu) {
    t_joueur *joueur = &jeu->joueurs[jeu->joueur_actuel];

    if (jeu->joueur_actuel == 0 && joueur->position.y == TAILLE - 1) {
        return true;
    } else if (jeu->joueur_actuel == 1 && joueur->position.y == 0) {
        return true;
    } else if (jeu->nb_joueurs == 4) {
        if (jeu->joueur_actuel == 2 && joueur->position.x == TAILLE - 1) {
            return true;
        } else if (jeu->joueur_actuel == 3 && joueur->position.x == 0) {
            return true;
        }
    }
    return false;
}

void sauvegarder_partie(t_jeu *jeu) {
    FILE *fichier = fopen(FICHIER_PLATEAU, "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier de sauvegarde.\n");
        return;
    }

    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            fprintf(fichier, "%c", jeu->plateau[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

bool reprendre_partie(t_jeu *jeu) {
    int c;
    FILE *fichier = fopen(FICHIER_PLATEAU, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier de sauvegarde.\n");
        return false;
    }
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            c = fgetc(fichier);
            if (c == EOF) {
                printf("Erreur de fichier");
                fclose(fichier);
                return false;
            }
            jeu->plateau[i][j] = (char) c;
        }
        fgetc(fichier); // Saut de ligne en fin de rangée
    }
    fclose(fichier);
    return true;
}

void sauvegarder_joueur(t_jeu *jeu) {
    FILE *fichier = fopen(FICHIER_JOUEURS, "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier de sauvegarde.\n");
        return;
    }

    fprintf(fichier, "%d %d\n", jeu->nb_joueurs, jeu->joueur_actuel);
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        fprintf(fichier, "%s %c %d %d %d %d %d\n",
                jeu->joueurs[i].nom,
                jeu->joueurs[i].pion,
                jeu->joueurs[i].bar_rest,
                jeu->joueurs[i].score,
                jeu->joueurs[i].position.x,
                jeu->joueurs[i].position.y,
                jeu->joueurs[i].deplacements);
    }
    fclose(fichier);
    fichier = NULL;
    printf("Fichier sauvegarde avec succes.\n");
}

bool reprendre_joueur(t_jeu *jeu) {
    FILE *fichier = fopen(FICHIER_JOUEURS, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier de sauvegarde.\n");
        return false;
    }

    int nb_joueurs = 0;
    int joueur_actuel = 0;
    if (fscanf(fichier, "%d %d\n", &nb_joueurs, &joueur_actuel) != 2 ||
        (nb_joueurs != 2 && nb_joueurs != 4) ||
        joueur_actuel < 0 || joueur_actuel >= nb_joueurs) {
        printf("Erreur : Fichier de joueurs invalide.\n");
        fclose(fichier);
        return false;
    }

    int nb_lus = 0;
    while (nb_lus < nb_joueurs &&
           fscanf(fichier, "%39s %c %d %d %d %d %d\n",
                  jeu->joueurs[nb_lus].nom,
                  &jeu->joueurs[nb_lus].pion,
                  &jeu->joueurs[nb_lus].bar_rest,
                  &jeu->joueurs[nb_lus].score,
                  &jeu->joueurs[nb_lus].position.x,
                  &jeu->joueurs[nb_lus].position.y,
                  &jeu->joueurs[nb_lus].deplacements) == 7) {
        // Vérifier si le nom du joueur est valide (non vide)
        if (strlen(jeu->joueurs[nb_lus].nom) == 0) {
            printf("Erreur : Nom de joueur vide détecté. Arrêt de la lecture.\n");
            break;
        }
        nb_lus++;
    }
    fclose(fichier);

    if (nb_lus != nb_joueurs) {
        printf("Erreur : Fichier de joueurs incomplet.\n");
        return false;
    }
    jeu->nb_joueurs = nb_joueurs;
    jeu->joueur_actuel = joueur_actuel;
    return true;
}

void sauvegarder_partie_complete(t_jeu *jeu) {
    sauvegarder_partie(jeu);
    sauvegarder_joueur(jeu);
}

bool reprise_partie_complete(t_jeu *jeu) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            jeu->plateau[i][j] = ' ';
        }
    }
    jeu->nb_joueurs = 0;
    if (!reprendre_partie(jeu) || !reprendre_joueur(jeu)) {
        jeu->nb_joueurs = 0;
        return false;
    }
    printf("Reprise partie avec succès\n");
    return true;
}

void sauvegarder_scores(t_jeu *jeu, const char *fichier) {
    // Charger les scores existants dans une structure temporaire
    char noms_existants[100][40];
    int scores_existants[100];
    int nb_scores_existants = 0;

    FILE *fp = fopen(fichier, "r");
    if (fp != NULL) {
        while (nb_scores_existants < 100 &&
               fscanf(fp, "%39s %d", noms_existants[nb_scores_existants],
                      &scores_existants[nb_scores_existants]) == 2) {
            nb_scores_existants++;
        }
        fclose(fp);
    }

    // Mettre à jour les scores des joueurs actuels
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        int joueur_trouve = 0;
        for (int j = 0; j < nb_scores_existants; j++) {
            if (strcmp(jeu->joueurs[i].nom, noms_existants[j]) == 0) {
                scores_existants[j] = jeu->joueurs[i].score; // Mise à jour du score
                joueur_trouve = 1;
                break;
            }
        }
        if (!joueur_trouve && nb_scores_existants < 100) {
            // Ajouter un nouveau joueur si non trouvé
            strcpy(noms_existants[nb_scores_existants], jeu->joueurs[i].nom);
            scores_existants[nb_scores_existants] = jeu->joueurs[i].score;
            nb_scores_existants++;
        }
    }

    // Réécrire tout le fichier
    fp = fopen(fichier, "w");
    if (fp == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier de sauvegarde.\n");
        return;
    }
    for (int i = 0; i < nb_scores_existants; i++) {
        fprintf(fp, "%s %d\n", noms_existants[i], scores_existants[i]);
    }

    fclose(fp);
    printf("Scores sauvegardés dans %s.\n", fichier);
}

void charger_scores(t_jeu *jeu, const char *fichier) {
    FILE *fp = fopen(fichier, "r");
    if (fp == NULL) {
        printf("Aucun fichier de sauvegarde trouvé. Les scores commencent à zéro.\n");
        return;
    }

    char nom[40];
    int score;
    while (fscanf(fp, "%39s %d", nom, &score) == 2) {
        for (int i = 0; i < jeu->nb_joueurs; i++) {
            if (strcmp(nom, jeu->joueurs[i].nom) == 0) {
                jeu->joueurs[i].score = score; // Associer le score existant
                break;
            }
        }
    }

    fclose(fp);
    printf("Scores charges depuis %s.\n", fichier);
}

void sauvegarder_historique(t_jeu *jeu, const char *fichier_historique) {
    FILE *fp = fopen(fichier_historique, "a");
    if (fp == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier d'historique.\n");
        return;
    }

    fprintf(fp, "Nouvelle partie:\n");
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        fprintf(fp, "Joueur: %s, Score: %d, Murs restants: %d, Déplacements: %d\n",
                jeu->joueurs[i].nom,
                jeu->joueurs[i].score,
                jeu->joueurs[i].bar_rest,
                jeu->joueurs[i].deplacements);
    }
    fprintf(fp, "Plateau final:\n");
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            fprintf(fp, "%c", jeu->plateau[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "-----------------------------\n");

    fclose(fp);
    printf("Historique sauvegardé avec succès.\n");
}
