#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traitement_image.c"

#define INPUT_SIZE 900         
#define HIDDEN_SIZE 16        
#define OUTPUT_SIZE 26         
#define BATCH_SIZE 26           
#define LEARNING_RATE 0.01     
#define EPOCHS 10000          


//ne pas oublier de passer les images en noir

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}


double sigmoid_derivative(double x) {
    return x * (1 - x);
}

// Initialisation aléatoire des poids
void init_weights(double weights[][HIDDEN_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
}

// Initialisation aléatoire des biais
void init_bias(double bias[], int size) {
    for (int i = 0; i < size; i++) {
        bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

// Propagation avant pour un lot d'images
void forward_batch(double batch_input[][INPUT_SIZE], double weights_input_hidden[][HIDDEN_SIZE], 
                   double hidden_bias[], double batch_hidden[][HIDDEN_SIZE], 
                   double weights_hidden_output[][OUTPUT_SIZE], double output_bias[], 
                   double batch_output[][OUTPUT_SIZE], int batch_size) {

    for (int img = 0; img < batch_size; img++) {
        // Propagation de l'image vers la couche cachée
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            batch_hidden[img][i] = hidden_bias[i]; // Ajout du biais
            for (int j = 0; j < INPUT_SIZE; j++) {
                batch_hidden[img][i] += batch_input[img][j] * weights_input_hidden[j][i];
            }
            batch_hidden[img][i] = sigmoid(batch_hidden[img][i]);
        }

        // Propagation de la couche cachée vers la couche de sortie
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            batch_output[img][i] = output_bias[i]; // Ajout du biais
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                batch_output[img][i] += batch_hidden[img][j] * weights_hidden_output[j][i];
            }
            batch_output[img][i] = sigmoid(batch_output[img][i]);
        }
    }
}

// Rétropropagation pour un lot d'images
void backpropagation_batch(double batch_input[][INPUT_SIZE], double batch_hidden[][HIDDEN_SIZE], 
                           double batch_output[][OUTPUT_SIZE], double batch_target[][OUTPUT_SIZE], 
                           double weights_input_hidden[][HIDDEN_SIZE], double weights_hidden_output[][OUTPUT_SIZE], 
                           double hidden_bias[], double output_bias[], int batch_size) {

    double output_errors[BATCH_SIZE][OUTPUT_SIZE];
    double hidden_errors[BATCH_SIZE][HIDDEN_SIZE];

    // Calcul des erreurs de la couche de sortie pour chaque image du lot
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            output_errors[img][i] = (batch_target[img][i] - batch_output[img][i]) * sigmoid_derivative(batch_output[img][i]);
        }
    }

    // Calcul des erreurs de la couche cachée pour chaque image du lot
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            hidden_errors[img][i] = 0;
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                hidden_errors[img][i] += output_errors[img][j] * weights_hidden_output[i][j];
            }
            hidden_errors[img][i] *= sigmoid_derivative(batch_hidden[img][i]);
        }
    }

    // Mise à jour des poids entre la couche cachée et la sortie
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                weights_hidden_output[i][j] += LEARNING_RATE * output_errors[img][j] * batch_hidden[img][i];
            }
        }
    }

    // Mise à jour des biais de la couche de sortie
    for (int img = 0; img < batch_size; img++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            output_bias[j] += LEARNING_RATE * output_errors[img][j];
        }
    }

    // Mise à jour des poids entre l'entrée et la couche cachée
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < INPUT_SIZE; i++) {
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                weights_input_hidden[i][j] += LEARNING_RATE * hidden_errors[img][j] * batch_input[img][i];
            }
        }
    }

    // Mise à jour des biais de la couche cachée
    for (int img = 0; img < batch_size; img++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            hidden_bias[j] += LEARNING_RATE * hidden_errors[img][j];
        }
    }
}

// Fonction Softmax
void softmax(double* input, double* output, size_t length) {
    double max = input[0];
    for (size_t i = 1; i < length; i++) {
        if (input[i] > max) max = input[i];  
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < length; i++) {
        output[i] = exp(input[i] - max);  
        sum += output[i];
    }

    for (size_t i = 0; i < length; i++) {
        output[i] /= sum;
    }
}

// Prédiction
void predict(double input[INPUT_SIZE], double hiddenWeight[INPUT_SIZE][HIDDEN_SIZE], 
             double outPutWeight[HIDDEN_SIZE][OUTPUT_SIZE], double hiddenLayerBias[HIDDEN_SIZE], 
             double outputLayerBias[OUTPUT_SIZE], double* outputLayer) {
    double hiddenLayer[HIDDEN_SIZE];

    // Propagation avant : Couche cachée
    for (size_t j = 0; j < HIDDEN_SIZE; j++) {
        double activation = hiddenLayerBias[j];
        for (size_t k = 0; k < INPUT_SIZE; k++) {
            activation += input[k] * hiddenWeight[k][j];
        }
        hiddenLayer[j] = sigmoid(activation);
    }

    // Propagation avant : Couche de sortie (Softmax)
    double outputRaw[OUTPUT_SIZE];
    for (size_t j = 0; j < OUTPUT_SIZE; j++) {
        double activation = outputLayerBias[j];
        for (size_t k = 0; k < HIDDEN_SIZE; k++) {
            activation += hiddenLayer[k] * outPutWeight[k][j];
        }
        outputRaw[j] = activation;
    }

    // Appliquer softmax
    softmax(outputRaw, outputLayer, OUTPUT_SIZE);
}

void remplirTestAvecImages(double test[BATCH_SIZE][INPUT_SIZE], char* images[BATCH_SIZE]) {
    for (size_t i = 0; i < BATCH_SIZE; i++) 
    {
        double* resultats = traitements(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) 
        {
            test[i][j] = resultats[j];
        }
        free(resultats); 
    }
}

void remplirTestAvecImages_black(double test[BATCH_SIZE][INPUT_SIZE], char* images[BATCH_SIZE]) {
    for (size_t i = 0; i < BATCH_SIZE; i++) 
    {
        double* resultats = traitements_test(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) 
        {
            test[i][j] = resultats[j];
        }
        free(resultats); 
    }
}


void save_weights(double hiddenWeight[INPUT_SIZE][HIDDEN_SIZE], 
                  double outPutWeight[HIDDEN_SIZE][OUTPUT_SIZE], 
                  double hiddenLayerBias[HIDDEN_SIZE], 
                  double outputLayerBias[OUTPUT_SIZE]) {
    FILE *file = fopen("hiddenWeight.txt", "w");
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            // Écrire chaque élément de la matrice dans le fichier
            fprintf(file, "%d ", hiddenWeight[i][j]);
        }
        // Ajouter un saut de ligne après chaque ligne de la matrice
        fprintf(file, "\n");
    }

    FILE *file2 = fopen("outPutWeight", "w");
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            // Écrire chaque élément de la matrice dans le fichier
            fprintf(file2, "%d ", outPutWeight[i][j]);
        }
        // Ajouter un saut de ligne après chaque ligne de la matrice
        fprintf(file, "\n");
    }

    FILE *file3 = fopen("hiddenLayerBias", "w");
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        
            // Écrire chaque élément de la matrice dans le fichier
        fprintf(file3, "%d ", hiddenLayerBias[i]);
    }

    FILE *file4 = fopen("OutputLayerBias", "w");
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        
        fprintf(file4, "%d ", outputLayerBias[i]);
    }






    

    fclose(file);
    fclose(file2);
    fclose(file3);
    fclose(file4);
}




int main(int argc,char** argv) {
    // Initialisation des poids et des biais
    printf(argv[1]);
    double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
    double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
    double hidden_bias[HIDDEN_SIZE];
    double output_bias[OUTPUT_SIZE];

    // Initialisation aléatoire des poids
    
    init_weights(weights_input_hidden, INPUT_SIZE, HIDDEN_SIZE);
    init_weights(weights_hidden_output, HIDDEN_SIZE, OUTPUT_SIZE);
    init_bias(hidden_bias, HIDDEN_SIZE);
    init_bias(output_bias, OUTPUT_SIZE);

    // Remplissage des données d'entrée et des cibles pour l'entraînement
    /*double batch_input[BATCH_SIZE][INPUT_SIZE] = { { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.00,1.00,1.00,1.00,1.00,0.87,0.83,0.83,0.80,0.77,0.77,0.73,0.73,0.77,0.73,0.73,0.63,0.60,0.60,0.57,0.77,0.73,0.73,0.77,0.80,0.87,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.87,0.83,0.83,0.80,0.77,0.77,0.73,0.73,0.77,0.73,0.73,0.63,0.60,0.60,0.57,0.77,0.73,0.73,0.77,0.80,0.87,1.00,1.00,1.00,1.00 },
                                                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.00,1.00,1.00,1.00,1.00,0.70,0.60,0.60,0.80,0.80,0.80,0.77,0.63,0.63,0.60,0.60,0.77,0.77,0.77,0.77,0.60,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.70,0.60,0.60,0.80,0.80,0.80,0.77,0.63,0.63,0.60,0.60,0.77,0.77,0.77,0.77,0.60,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00},
                                                    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.00,1.00,1.00,0.77,0.67,0.60,0.60,0.67,0.70,0.73,0.83,0.87,0.87,0.87,0.87,0.87,0.77,0.77,0.70,0.70,0.57,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.77,0.67,0.60,0.60,0.67,0.70,0.73,0.83,0.87,0.87,0.87,0.87,0.87,0.77,0.77,0.70,0.70,0.57,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00 },
                                                    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.00,1.00,1.00,1.00,0.73,0.63,0.60,0.60,0.67,0.73,0.77,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.77,0.70,0.57,0.60,0.63,0.67,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.73,0.63,0.60,0.60,0.67,0.73,0.77,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.77,0.70,0.57,0.60,0.63,0.67,1.00,1.00,1.00,1.00,1.00 }
                                                     };*/
    double batch_target[BATCH_SIZE][OUTPUT_SIZE] = {    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
 
                                                      
                                                      
                                                      
                                                       };


    double batch_input[BATCH_SIZE][INPUT_SIZE];

    char* images[BATCH_SIZE]=
    {
        "images_test/A.PNG",
        "images_test/B.PNG",
        "images_test/C.PNG",
        "images_test/D.PNG",
        "images_test/E.PNG",
        "images_test/F.PNG",
        "images_test/G.PNG",
        "images_test/H.PNG",
        "images_test/I.PNG",
        "images_test/J.PNG",
        "images_test/K.PNG",
        "images_test/L.PNG",
        "images_test/M.PNG",
        "images_test/N.PNG",
        "images_test/O.PNG",
        "images_test/P.PNG",
        "images_test/Q.PNG",
        "images_test/R.PNG",
        "images_test/S.PNG",
        "images_test/T.PNG",
        "images_test/U.PNG",
        "images_test/V.PNG",
        "images_test/W.PNG",
        "images_test/X.PNG",
        "images_test/Y.PNG",
        "images_test/Z.PNG",
        
        
        

        
    };

    char* images2[BATCH_SIZE]=
    {
        "images_test/AA.PNG",
        "images_test/BB.PNG",
        "images_test/CC.PNG",
        "images_test/DD.PNG",
        "images_test/EE.PNG",
        "images_test/FF.PNG",
        "images_test/GG.PNG",
        "images_test/HH.PNG",
        "images_test/II.PNG",
        "images_test/JJ.PNG",
        "images_test/KK.PNG",
        "images_test/LL.PNG",
        "images_test/MM.PNG",
        "images_test/NN.PNG",
        "images_test/OO.PNG",
        "images_test/PP.PNG",
        "images_test/QQ.PNG",
        "images_test/RR.PNG",
        "images_test/SS.PNG",
        "images_test/TT.PNG",
        "images_test/UU.PNG",
        "images_test/VV.PNG",
        "images_test/WW.PNG",
        "images_test/XX.PNG",
        "images_test/YY.PNG",
        "images_test/ZZ.PNG",
        
    };

    /*char* images3[BATCH_SIZE]=
    {
        "images_test/dataset/a/AAA.PNG",
        "images_test/dataset/b/BBB.PNG",
        "images_test/dataset/c/CCC.PNG",
        "images_test/dataset/d/DDD.PNG",
        "images_test/dataset/e/EEE.PNG",
        "images_test/dataset/f/FFF.PNG",
        "images_test/dataset/g/GGG.PNG",
        "images_test/dataset/h/HHH.PNG",
        "images_test/dataset/i/III.PNG",
        "images_test/dataset/j/JJJ.PNG",
        "images_test/dataset/k/K6.PNG",
        "images_test/dataset/l/L6.PNG",
        "images_test/dataset/m/M6.PNG",
        "images_test/dataset/n/N6.PNG",
        "images_test/dataset/o/O6.PNG",
        "images_test/dataset/p/P6.PNG",
        "images_test/dataset/q/Q6.PNG",
        "images_test/dataset/r/R6.PNG",
        "images_test/dataset/s/S6.PNG",
        "images_test/dataset/t/T6.PNG",
        "images_test/dataset/u/U6.PNG",
        "images_test/dataset/v/V6.PNG",
        "images_test/dataset/w/W6.PNG",
        "images_test/dataset/x/X6.PNG",
        "images_test/dataset/y/Y6.PNG",
        "images_test/dataset/z/Z6.PNG",
        

        
    };*/


    double batch_input2[BATCH_SIZE][INPUT_SIZE];
    double batch_input3[BATCH_SIZE][INPUT_SIZE];
    //remplirTestAvecImages_black(batch_input3,images3);
    remplirTestAvecImages(batch_input2,images2);
    remplirTestAvecImages(batch_input,images);



    // Entraînement du réseau
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double batch_hidden[BATCH_SIZE][HIDDEN_SIZE];
        double batch_output[BATCH_SIZE][OUTPUT_SIZE];
        if (epoch%100==0) printf("%d\n",EPOCHS-epoch);
        forward_batch(batch_input, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        forward_batch(batch_input2, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input2, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        /*forward_batch(batch_input3, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input3, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        */
    
    }

    // Prédiction sur un nouvel exemple
    //double new_input[INPUT_SIZE] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1.00,1.00,1.00,1.00,0.73,0.63,0.60,0.60,0.67,0.73,0.77,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.77,0.70,0.57,0.60,0.63,0.67,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,0.73,0.63,0.60,0.60,0.67,0.73,0.77,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.77,0.70,0.57,0.60,0.63,0.67,1.00,1.00,1.00,1.00,1.00 


    save_weights(weights_input_hidden, weights_hidden_output,  hidden_bias, output_bias);



    double new_input[INPUT_SIZE];

    


    double* resultats = traitements_test(argv[1]);
        for (size_t j = 0; j < INPUT_SIZE; j++) 
        {
            new_input[j] = resultats[j];
        }
        


                                                
                   
    double prediction[OUTPUT_SIZE];
    predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);
    char lettre[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    
    
    // Afficher les résultats de la prédiction
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
    }

    return 0;
}
