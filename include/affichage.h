#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "jeu.h"

void afficher_plateau(char plateau[TAILLE][TAILLE], t_joueur joueurs[], t_jeu *jeu);
void afficher_resume_joueurs(t_jeu *jeu);
void afficherMenu(int choixActuel);
void afficher_aide();
void afficher_scores(const char *fichier);

#endif
