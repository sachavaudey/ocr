#ifndef XOR_H
#define XOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

#define NB_INPUT 2
#define NB_HIDDEN_NODE 2
#define NB_OUTPUT 2

double sigmoid(double x);
double dsigmoid(double x);
double init_weight();
void softmax(double* input, double* output, size_t length);
void shuffle(int *array, size_t n);
void predict(double input1, double input2, double hiddenWeight[NB_INPUT][NB_HIDDEN_NODE], 
             double outputWeight[NB_HIDDEN_NODE][NB_OUTPUT], double hiddenLayerBias[NB_HIDDEN_NODE], 
             double outputLayerBias[NB_OUTPUT], double* outputLayer);
int process_xor();

#endif