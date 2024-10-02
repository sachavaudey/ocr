#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "traitement_image.c"


//faire une moyenne de 10 valeurs pour les convertir en double et rentrer dans l'input test





double sigmoid(double x) { return 1/(1+exp(-x)); }
double dsigmoid(double x) { return x * (1 - x); }

double init_weight() { return ((double)rand()) / ((double)RAND_MAX); }

// Fonction softmax
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

void shuffle(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

#define nbInput 30
#define nbHiddenNode 16
#define output 4
#define nbTest 4

void predict(double input[nbInput], double hiddenWeight[nbInput][nbHiddenNode], 
             double outPutWeight[nbHiddenNode][output], double hiddenLayerBias[nbHiddenNode], 
             double outputLayerBias[output], double* outputLayer) {
    double hiddenLayer[nbHiddenNode];

    // Propagation avant : Couche cachée
    for (size_t j = 0; j < nbHiddenNode; j++) {
        double activation = hiddenLayerBias[j];
        for (size_t k = 0; k < nbInput; k++) {
            activation += input[k] * hiddenWeight[k][j];
        }
        hiddenLayer[j] = sigmoid(activation);
    }

    // Propagation avant : Couche de sortie (Softmax)
    double outputRaw[output];
    for (size_t j = 0; j < output; j++) {
        double activation = outputLayerBias[j];
        for (size_t k = 0; k < nbHiddenNode; k++) {
            activation += hiddenLayer[k] * outPutWeight[k][j];
        }
        outputRaw[j] = activation;
    }

    // Appliquer softmax
    softmax(outputRaw, outputLayer, output);
}

void save_weights(const char* filename, double hiddenWeight[nbInput][nbHiddenNode], 
                  double outPutWeight[nbHiddenNode][output], 
                  double hiddenLayerBias[nbHiddenNode], 
                  double outputLayerBias[output]) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for writing");
        return;
    }

    // Sauvegarder les poids de la couche cachée
    fwrite(hiddenWeight, sizeof(double), nbInput * nbHiddenNode, file);
    
    // Sauvegarder les poids de la couche de sortie
    fwrite(outPutWeight, sizeof(double), nbHiddenNode * output, file);
    
    // Sauvegarder les biais de la couche cachée
    fwrite(hiddenLayerBias, sizeof(double), nbHiddenNode, file);
    
    // Sauvegarder les biais de la couche de sortie
    fwrite(outputLayerBias, sizeof(double), output, file);

    fclose(file);
}



void remplirTestAvecImages(double test[nbTest][nbInput], char* images[nbTest]) {
    for (size_t i = 0; i < nbTest; i++) 
    {
        double* resultats = traitements(images[i]);
        for (size_t j = 0; j < nbInput; j++) 
        {
            test[i][j] = resultats[j];
        }
        free(resultats); 
    }
}

int main(void) {
    int repet = 10000000;
    const double lr = 0.1f;

    double hiddenLayer[nbHiddenNode];
    double outputLayer[output];
    double hiddenLayerBias[nbHiddenNode];
    double outputLayerBias[output];

    double hiddenWeight[nbInput][nbHiddenNode];
    double outPutWeight[nbHiddenNode][output];

    


    double training_input[nbTest][nbInput] = {{ 1.00,1.00,1.00,1.00,1.00,0.87,0.83,0.83,0.80,0.77,0.77,0.73,0.73,0.77,0.73,0.73,0.63,0.60,0.60,0.57,0.77,0.73,0.73,0.77,0.80,0.87,1.00,1.00,1.00,1.00 },
                                               { 1.00,1.00,1.00,1.00,1.00,0.70,0.60,0.60,0.80,0.80,0.80,0.77,0.63,0.63,0.60,0.60,0.77,0.77,0.77,0.77,0.60,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00},
                                                { 1.00,1.00,1.00,0.77,0.67,0.60,0.60,0.67,0.70,0.73,0.83,0.87,0.87,0.87,0.87,0.87,0.77,0.77,0.70,0.70,0.57,0.60,0.67,0.73,1.00,1.00,1.00,1.00,1.00,1.00},
                                                { 1.00,1.00,1.00,1.00,0.73,0.63,0.60,0.60,0.67,0.73,0.77,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.73,0.77,0.70,0.57,0.60,0.63,0.67,1.00,1.00,1.00,1.00,1.00}};


                                             
    double training_output[nbTest][output] = {{1,0,0,0},  // Classe 0
                                              {0,1,0,0},  // Classe 1
                                              {0,0,1,0},  // Classe 1
                                              {0,0,0,1}}; // Classe 0




    double test[nbTest][nbInput];

    // Liste des images à traiter
    char* images[nbTest] = {
        "images_test/A.PNG",
        "images_test/B.PNG",
        "images_test/C.PNG",
        "images_test/D.PNG"
    };

    
    remplirTestAvecImages(test, images);












    // Initialisation des poids
    for (size_t i = 0; i < nbInput; i++) {
        for (size_t j = 0; j < nbHiddenNode; j++) {
            hiddenWeight[i][j] = init_weight();
        }
    }

    for (size_t i = 0; i < nbHiddenNode; i++) {
        for (size_t j = 0; j < output; j++) {
            outPutWeight[i][j] = init_weight();
        }
    }

    // Initialisation des biais
    for (size_t i = 0; i < output; i++) {
        outputLayerBias[i] = init_weight();
    }

    for (size_t i = 0; i < nbHiddenNode; i++) {
        hiddenLayerBias[i] = init_weight();
    }

    int trainingsetorder[] = {0, 1, 2, 3};

    for (size_t epoch = 0; epoch < repet; epoch++) {
        shuffle(trainingsetorder, nbTest);
        for (size_t x = 0; x < nbTest; x++) {
            int i = trainingsetorder[x];

            if (epoch%1000==0) printf("%zu\n",repet-epoch);
            // Propagation avant : Couche cachée
            for (size_t j = 0; j < nbHiddenNode; j++) {
                double activation = hiddenLayerBias[j];
                for (size_t k = 0; k < nbInput; k++) {
                    activation += training_input[i][k] * hiddenWeight[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }

            // Propagation avant : Couche de sortie (Softmax)
            double outputRaw[output];
            for (size_t j = 0; j < output; j++) {
                double activation = outputLayerBias[j];
                for (size_t k = 0; k < nbHiddenNode; k++) {
                    activation += hiddenLayer[k] * outPutWeight[k][j];
                }
                outputRaw[j] = activation;
            }

            // Appliquer softmax
            softmax(outputRaw, outputLayer, output);

            // Rétropropagation : Couche de sortie
            double DeltaOuput[output];
            for (size_t j = 0; j < output; j++) {
                double error = training_output[i][j] - outputLayer[j];
                DeltaOuput[j] = error;
            }

            // Rétropropagation : Couche cachée
            double deltaHidden[nbHiddenNode];
            for (size_t j = 0; j < nbHiddenNode; j++) {
                double error = 0.0f;
                for (size_t k = 0; k < output; k++) {
                    error += DeltaOuput[k] * outPutWeight[j][k];
                }
                deltaHidden[j] = error * dsigmoid(hiddenLayer[j]);
            }

            // Mise à jour des poids et des biais de la couche de sortie
            for (size_t j = 0; j < output; j++) {
                outputLayerBias[j] += DeltaOuput[j] * lr;
                for (size_t k = 0; k < nbHiddenNode; k++) {
                    outPutWeight[k][j] += hiddenLayer[k] * DeltaOuput[j] * lr;
                }
            }

            // Mise à jour des poids et des biais de la couche cachée
            for (size_t j = 0; j < nbHiddenNode; j++) {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for (size_t k = 0; k < nbInput; k++) {
                    hiddenWeight[k][j] += training_input[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }

    
    
    
    
    

    


    for (size_t j = 0; j < 4; j++)
    {
       double* input=test[j];
       double predictedOutput[output];
      predict(input, hiddenWeight, outPutWeight, hiddenLayerBias, outputLayerBias, predictedOutput);
    
    
    const char* labels[] = {"A", "B", "C", "D"};
    int predictedClass = 0;
    double maxProb = predictedOutput[0];

    for (int i = 1; i < output; i++) {
        if (predictedOutput[i] > maxProb) {
            maxProb = predictedOutput[i];
            predictedClass = i;
        }
    }

    printf("Predicted class: %s\n", labels[predictedClass]);

    
    }
    return 0;
                                            
    
   
}
