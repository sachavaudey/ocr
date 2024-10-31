#ifndef SOLVER_H
#define SOLVER_H

#include <stdio.h>

// read and cread a grid
char (*lire_grille(char *nom_fichier, int *lignes, int *colonnes))[100];


void solver(char *nom_fichier, char *word);

#endif // SOLVER_H

