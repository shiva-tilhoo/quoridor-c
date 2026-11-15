#ifndef UTILE_H
#define UTILE_H

#include <stdbool.h>

// Fonction pour changer la couleur du texte dans la console
void Color(int couleurDuTexte, int couleurDeFond);

// Générer un nombre aléatoire entre 0 et un maximum (exclus)
int gen_aleatoire(int max);

// Pause pour attendre une touche de l'utilisateur
void attendre_touche();

// Nettoyer le tampon d'entrée
void nettoyer_tampon();

// Vérifier si une chaîne est vide
bool est_chaine_vide(const char *chaine);

#endif // UTILE_H
