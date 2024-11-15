// point.h
#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include "../other/predict.h"

char (*lire_grille(char *nom_fichier, int *lignes, int *colonnes))[100];
int search_right(char** matrice, char* word, int i, int j, int x);
int search_left(char** matrice, char* word, int i, int j, int x);
int search_up(char** matrice, char* word, int i, int j, int x);
int search_down(char** matrice, char* word, int i, int j, int x);
int search_up_left(char** matrice, char* word, int i, int j, int x);
int search_up_right(char** matrice, char* word, int i, int j, int x);
int search_down_left(char** matrice, char* word, int i, int j, int x);
int search_down_right(char** matrice, char* word, int i, int j, int x);
void solver(char *nom_fichier, char *word);
int process_solver(char *filename, char *word);

#endif // POINT_H
