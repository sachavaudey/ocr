#ifndef WORD_SEARCH_H
#define WORD_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read the grid from a file
char (*lire_grille(char *nom_fichier, int *lignes, int *colonnes))[100];

// Search functions
int search_right(char **matrice, char *word, int i, int j, int x);
int search_left(char **matrice, char *word, int i, int j, int x);
int search_up(char **matrice, char *word, int i, int j, int x);
int search_down(char **matrice, char *word, int i, int j, int x);
int search_up_left(char **matrice, char *word, int i, int j, int x);
int search_up_right(char **matrice, char *word, int i, int j, int x);
int search_down_left(char **matrice, char *word, int i, int j, int x);
int search_down_right(char **matrice, char *word, int i, int j, int x);

// Solver function
void solver(char *nom_fichier, char *word);

// Functions to handle word list
char **read_fichier(const char *nom_fichier, int *nombre_mots);
int compterLignes(const char *nomFichier);

// Main process handler
int process_solver(char *filename, char *word);

#endif // WORD_SEARCH_H
