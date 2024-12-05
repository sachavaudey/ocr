#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H


#include "img_traitement.h"


#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 90       
#define OUTPUT_SIZE 26       
#define BATCH_SIZE 26        
#define LEARNING_RATE 0.1 
#define NBTEST 70    
#define EPOCHS 10000


double sigmoid_aux(double x);
double sigmoid_derivative_aux(double x);

void init_weights(double** weights, int rows, int cols);
void init_bias(double* bias, int size);
void init_target(double** batch_target);

void forward_batch(double** batch_input, double** weights_input_hidden, 
                   double* hidden_bias, double** batch_hidden, 
                   double** weights_hidden_output, double* output_bias, 
                   double** batch_output, int batch_size);

void backpropagation_batch(double** batch_input, double** batch_hidden, 
                           double** batch_output, double** batch_target, 
                           double** weights_input_hidden, double** weights_hidden_output, 
                           double* hidden_bias, double* output_bias, int batch_size);

void softmax_aux(double* input, double* output, size_t length);

void predict_aux(double* input, double** hiddenWeight, double** outPutWeight, double* hiddenLayerBias, 
                 double* outputLayerBias, double* outputLayer);

void remplirTestAvecImages_black(double** test, char** images);
void remplir_chemins_images(char** images, const char* prefixe, const char* suffixe);

void shuffle(int *array, size_t n);

void save_weights(double** hiddenoutput, double** outPutWeight, double* hiddenLayerBias, 
                  double* outputLayerBias);

int process_train();

#endif
