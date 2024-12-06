#ifndef SOLVE_H
#define SOLVE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char (*lire_grille(int *lignes, int *colonnes))[100];
int search_right(char** matrice, char* word ,int i,int j,int x);
int search_left(char** matrice, char* word ,int i,int j,int x);
int search_up(char** matrice, char* word ,int i,int j,int x);
int search_down(char** matrice, char* word ,int i,int j,int x);
int search_up_left(char** matrice, char* word ,int i,int j,int x);
int search_up_right(char** matrice, char* word ,int i,int j,int x);
int search_down_left(char** matrice, char* word ,int i,int j,int x);
int search_down_right(char** matrice, char* word ,int i,int j,int x);
void solver(char *word) ;
char **read_fichier( int *nombre_mots);
int compterLignes();
int process_solver(char* word);
int pro_solv();


#endif