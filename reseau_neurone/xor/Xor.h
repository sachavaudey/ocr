#ifndef XOR_H
#define XOR_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double sigmoid(double x);
double dsigmoid(double x);
double init_weight();
void softmax(double* input, double* output, size_t length);
void shuffle(int *array, size_t n);
void predict(double input1, double input2, double hiddenWeight[nbInput][nbHiddenNode], 
             double outPutWeight[nbHiddenNode][output], double hiddenLayerBias[nbHiddenNode], 
             double outputLayerBias[output], double* outputLayer);
int process_xor(void);


#endif