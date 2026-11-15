#ifndef INTERACTION_H
#define INTERACTION_H

#include "jeu.h"

void deplacer_pion(t_jeu *jeu);
bool placement_barriere(char plateau[TAILLE][TAILLE], t_barriere *bar, t_jeu *jeu);
void action(t_jeu *jeu);
void lancer_nouvelle_partie(t_jeu *jeu);
void continuer_partie(t_jeu *jeu);

#endif
