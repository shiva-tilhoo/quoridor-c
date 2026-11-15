#ifndef JEU_H
#define JEU_H

#include <stdbool.h>

#define TAILLE 17

#define FICHIER_PLATEAU "partie.txt"
#define FICHIER_JOUEURS "joueurs.txt"
#define FICHIER_SCORES "scores.txt"
#define FICHIER_HISTORIQUE "historique.txt"

#define POINTS_VICTOIRE 5

typedef struct {
    int x;
    int y;
} t_position;

typedef struct {
    char nom[40];
    char pion; // Symbole du joueur (ex. 'A', 'B')
    t_position position; // Position actuelle du joueur
    int bar_rest; // Nombre de barrieres restantes
    int score; // Score des parties gagnées
    int deplacements; // Nombre de déplacements effectués
} t_joueur;

typedef struct {
    char plateau[TAILLE][TAILLE];
    t_joueur joueurs[4]; // Quatre joueurs max
    int joueur_actuel; // Indique quel joueur joue (0 a 3)
    int nb_joueurs; // Nombre de joueurs (2 ou 4)
} t_jeu;

typedef struct {
    char bar_col_1;
    char bar_col_2;
    int bar_lig_1;
    int bar_lig_2;
    char direction;
} t_barriere;

// Initialisation
void initialiser_plateau(char plateau[TAILLE][TAILLE]);
void initialiser_barrieres(t_joueur *joueurs, int nb_joueurs);
void initialiser_joueurs(t_joueur *joueur, t_jeu *jeu);
void initialiser_positions_joueurs(t_joueur joueurs[], int nb_joueurs);

// Règles
bool verifier_victoire(t_jeu *jeu);

// Sauvegarde / reprise d'une partie
void sauvegarder_partie(t_jeu *jeu);
void sauvegarder_joueur(t_jeu *jeu);
void sauvegarder_partie_complete(t_jeu *jeu);
bool reprendre_partie(t_jeu *jeu);
bool reprendre_joueur(t_jeu *jeu);
bool reprise_partie_complete(t_jeu *jeu);

// Scores et historique
void charger_scores(t_jeu *jeu, const char *fichier);
void sauvegarder_scores(t_jeu *jeu, const char *fichier);
void sauvegarder_historique(t_jeu *jeu, const char *fichier_historique);

#endif
