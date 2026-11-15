#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h> // Pour _getch()
#include "utile.h"

void Color(int couleurDuTexte, int couleurDeFond) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}

int gen_aleatoire(int max) {
    if (max <= 0) {
        return 0; // Retourne 0 si max est invalide
    }
    return rand() % max;
}

void attendre_touche() {
    printf("Appuyez sur une touche pour continuer...");
    _getch(); // Attend une touche sans afficher le caractère
}

void nettoyer_tampon() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Vide le tampon d'entrée
    }
}

bool est_chaine_vide(const char *chaine) {
    if (chaine == NULL || chaine[0] == '\0') {
        return true; // Chaîne nulle ou vide
    }
    for (int i = 0; chaine[i] != '\0'; i++) {
        if (chaine[i] != ' ' && chaine[i] != '\t') {
            return false; // Contient au moins un caractère non vide
        }
    }
    return true; // La chaîne est composée uniquement d'espaces ou de tabulations
}
