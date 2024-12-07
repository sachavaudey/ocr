#ifndef PREDICT_H
#define PREDICT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "transpose_image.h"
#include "img_traitement.h"


#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 60       
#define OUTPUT_SIZE 26       
#define BATCH_SIZE 26        
#define LEARNING_RATE 0.1 
#define NBTEST 70    
#define EPOCHS 10000


void load_hidden_bias(const char* filename);
void load_output_bias(char* filename);
void load_weight_hidden_output(char *filename);
void load_weight_hidden_input(char *filename);
double sigmoid(double x);
void softmax(double* input, double* output, size_t length);
void predict(double input[INPUT_SIZE], double hiddenWeight[INPUT_SIZE][HIDDEN_SIZE], 
             double outPutWeight[HIDDEN_SIZE][OUTPUT_SIZE], double hiddenLayerBias[HIDDEN_SIZE], 
             double outputLayerBias[OUTPUT_SIZE], double* outputLayer);
void read_grid(char* res[4]);
int* search_size(int* res);
void create_grid(char* tab, int a, int b);
void create_word(char* tab, int* a, int b);
int pro_word();
int pro_grid();

#endif // PREDICT_H
