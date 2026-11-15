#include "interaction.h"
#include "affichage.h"
#include "utile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // Necessaire pour _getch()

void deplacer_pion(t_jeu *jeu) {
    char touche;
    t_joueur *joueur_actuel = &jeu->joueurs[jeu->joueur_actuel];

    printf("Tour de %s (%c).\n", joueur_actuel->nom, joueur_actuel->pion);
    printf("Utilisez Z (haut), S (bas), Q (gauche), D (droite) pour deplacer. Appuyez sur Echap pour quitter.\n");

    while (1) {
        touche = _getch();
        t_position nouvelle_position = joueur_actuel->position;
        int deplacement_valide = 1;

        if (toupper(touche) == 'S') {
            // Descendre
            if (nouvelle_position.y + 2 < TAILLE && jeu->plateau[nouvelle_position.y + 1][nouvelle_position.x] !=
                'B') {
                nouvelle_position.y += 2;
            } else {
                printf("Erreur : Deplacement impossible (barriere presente).\n");
                deplacement_valide = 0;
            }
        } else if (toupper(touche) == 'Z') {
            // Monter
            if (nouvelle_position.y - 2 >= 0 && jeu->plateau[nouvelle_position.y - 1][nouvelle_position.x] != 'B') {
                nouvelle_position.y -= 2;
            } else {
                printf("Erreur : Deplacement impossible (barriere presente).\n");
                deplacement_valide = 0;
            }
        } else if (toupper(touche) == 'D') {
            // Droite
            if (nouvelle_position.x + 2 < TAILLE && jeu->plateau[nouvelle_position.y][nouvelle_position.x + 1] !=
                'B') {
                nouvelle_position.x += 2;
            } else {
                printf("Erreur : Deplacement impossible (barriere presente).\n");
                deplacement_valide = 0;
            }
        } else if (toupper(touche) == 'Q') {
            // Gauche
            if (nouvelle_position.x - 2 >= 0 && jeu->plateau[nouvelle_position.y][nouvelle_position.x - 1] != 'B') {
                nouvelle_position.x -= 2;
            } else {
                printf("Erreur : Deplacement impossible (barriere presente).\n");
                deplacement_valide = 0;
            }
        } else if (touche == 27) {
            printf("Fin du jeu.\n");
            exit(0);
        } else {
            printf("Touche invalide.\n");
            continue;
        }

        if (deplacement_valide &&
            nouvelle_position.x >= 0 && nouvelle_position.x < TAILLE &&
            nouvelle_position.y >= 0 && nouvelle_position.y < TAILLE &&
            jeu->plateau[nouvelle_position.y][nouvelle_position.x] == '.') {
            joueur_actuel->position = nouvelle_position;
            joueur_actuel->deplacements++;

            // En cas de victoire on ne passe pas la main : action() termine la partie
            if (!verifier_victoire(jeu)) {
                jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nb_joueurs; // Change de joueur
            }
            break;
        } else {
            printf("Deplacement invalide. Reessayez.\n");
        }
    }
}

bool placement_barriere(char plateau[TAILLE][TAILLE], t_barriere *bar, t_jeu *jeu) {
    // Demande des coordonnees et de la direction
    printf("Ou souhaitez-vous placer la barriere ? (Exemple: D1 D2 G): ");
    if (scanf(" %c%d %c%d %c", &bar->bar_col_1, &bar->bar_lig_1, &bar->bar_col_2, &bar->bar_lig_2,
              &bar->direction) != 5) {
        nettoyer_tampon();
        printf("Erreur : Saisie invalide.\n");
        return false;
    }

    // Conversion des coordonnees (Ex: D1 -> colonne = 6, ligne = 0)
    int col1 = (toupper(bar->bar_col_1) - 'A') * 2; // Majuscule pour eviter erreurs
    int col2 = (toupper(bar->bar_col_2) - 'A') * 2;
    int lig1 = (bar->bar_lig_1 - 1) * 2;
    int lig2 = (bar->bar_lig_2 - 1) * 2;

    // Verification des limites du plateau
    if (col1 < 0 || col1 >= TAILLE || lig1 < 0 || lig1 >= TAILLE ||
        col2 < 0 || col2 >= TAILLE || lig2 < 0 || lig2 >= TAILLE) {
        printf("Erreur : Barriere en dehors des limites du plateau.\n");
        return false;
    }

    // Verification des cases voisines
    if (!(abs(col1 - col2) == 2 && lig1 == lig2) && !(abs(lig1 - lig2) == 2 && col1 == col2)) {
        printf("Erreur : Les cases doivent etre voisines et alignees horizontalement ou verticalement.\n");
        return false;
    }

    // Verification des barrieres restantes pour le joueur
    if (jeu->joueurs[jeu->joueur_actuel].bar_rest <= 0) {
        printf("Erreur : Vous n'avez plus de barrieres restantes !\n");
        return false;
    }

    // Verification si l'emplacement est deja occupe par une autre barriere
    if (plateau[lig1][col1] == 'B' || plateau[lig2][col2] == 'B') {
        printf("Erreur : Une barriere existe deja sur cette position.\n");
        return false;
    }

    // Placement de la barriere
    if (toupper(bar->direction) == 'H') {
        if (lig1 > 0 && plateau[lig1 - 1][col1] == ' ' && plateau[lig2 - 1][col2] == ' ') {
            plateau[lig1 - 1][col1] = 'B';
            plateau[lig2 - 1][col2] = 'B';
        } else {
            printf("Placement impossible en haut.\n");
            return false;
        }
    } else if (toupper(bar->direction) == 'B') {
        if (lig1 < TAILLE - 1 && plateau[lig1 + 1][col1] == ' ' && plateau[lig2 + 1][col2] == ' ') {
            plateau[lig1 + 1][col1] = 'B';
            plateau[lig2 + 1][col2] = 'B';
        } else {
            printf("Erreur : Placement en bas impossible.\n");
            return false;
        }
    } else if (toupper(bar->direction) == 'G') {
        if (col1 > 0 && plateau[lig1][col1 - 1] == ' ' && plateau[lig2][col2 - 1] == ' ') {
            plateau[lig1][col1 - 1] = 'B';
            plateau[lig2][col2 - 1] = 'B';
        } else {
            printf("Erreur : Placement a gauche impossible.\n");
            return false;
        }
    } else if (toupper(bar->direction) == 'D') {
        if (col1 < TAILLE - 1 && plateau[lig1][col1 + 1] == ' ' && plateau[lig2][col2 + 1] == ' ') {
            plateau[lig1][col1 + 1] = 'B';
            plateau[lig2][col2 + 1] = 'B';
        } else {
            printf("Erreur : Placement a droite impossible.\n");
            return false;
        }
    } else {
        printf("Erreur : Direction invalide.\n");
        return false;
    }

    // Mise a jour des barrieres restantes pour le joueur
    jeu->joueurs[jeu->joueur_actuel].bar_rest--;
    printf("Barriere placee avec succes par %s.\n", jeu->joueurs[jeu->joueur_actuel].nom);
    jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nb_joueurs;
    return true;
}

void action(t_jeu *jeu) {
    int victoire = 0;

    while (!victoire) {
        afficher_plateau(jeu->plateau, jeu->joueurs, jeu);
        printf("C'est au tour de %s (%c).\n", jeu->joueurs[jeu->joueur_actuel].nom,
               jeu->joueurs[jeu->joueur_actuel].pion);

        printf("Actions possibles :\n");
        printf("1 - Déplacer le pion\n");
        printf("2 - Poser une barrière\n");
        printf("3 - Passer son tour\n");
        printf("4 - Quitter\n");
        printf("Votre choix : ");

        int choix;
        if (scanf("%d", &choix) != 1) {
            nettoyer_tampon();
            printf("Choix invalide. Veuillez réessayer.\n");
            continue;
        }

        switch (choix) {
            case 1:
                deplacer_pion(jeu);
                break;

            case 2: {
                t_barriere barriere;
                if (!placement_barriere(jeu->plateau, &barriere, jeu)) {
                    printf("Action invalide : Impossible de poser une barrière.\n");
                }
                break;
            }

            case 3:
                printf("%s passe son tour.\n", jeu->joueurs[jeu->joueur_actuel].nom);
                jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nb_joueurs;
                break;

            case 4: {
                printf("Voulez-vous sauvegarder la partie avant de la quitter ? (O/N) : ");
                char reponse;
                scanf(" %c", &reponse);
                if (toupper(reponse) == 'O') {
                    sauvegarder_partie_complete(jeu);
                    printf("Partie sauvegardée. À bientôt !\n");
                    attendre_touche();
                    return;
                }
                if (toupper(reponse) == 'N') {
                    printf("Partie non sauvegardée. À bientôt !\n");
                    attendre_touche();
                    return;
                }
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
            }

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }

        // Vérifier la victoire après chaque action
        if (verifier_victoire(jeu)) {
            victoire = 1;
            jeu->joueurs[jeu->joueur_actuel].score += POINTS_VICTOIRE;
            system("cls");
            printf("Bravo %s ! Vous avez atteint le cote oppose !\n", jeu->joueurs[jeu->joueur_actuel].nom);

            sauvegarder_scores(jeu, FICHIER_SCORES);
            sauvegarder_historique(jeu, FICHIER_HISTORIQUE);

            afficher_resume_joueurs(jeu);
            attendre_touche();
        }
    }
}

void lancer_nouvelle_partie(t_jeu *jeu) {
    memset(jeu, 0, sizeof(t_jeu));
    initialiser_plateau(jeu->plateau);
    initialiser_joueurs(jeu->joueurs, jeu);
    initialiser_positions_joueurs(jeu->joueurs, jeu->nb_joueurs);
    charger_scores(jeu, FICHIER_SCORES);
    jeu->joueur_actuel = gen_aleatoire(jeu->nb_joueurs); // Tirage au sort du premier joueur
    system("cls");
    action(jeu);
}

void continuer_partie(t_jeu *jeu) {
    system("cls");
    printf("Reprise de la partie.\n");
    action(jeu);
}
