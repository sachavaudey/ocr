#include "Xor.h"


#define nbInput 2
#define nbHiddenNode 2
#define output 2  
#define nbTest 4


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





void predict(double input1, double input2, double hiddenWeight[nbInput][nbHiddenNode], 
             double outPutWeight[nbHiddenNode][output], double hiddenLayerBias[nbHiddenNode], 
             double outputLayerBias[output], double* outputLayer) {
    double hiddenLayer[nbHiddenNode];

    // Propagation avant : Couche cachée
    for (size_t j = 0; j < nbHiddenNode; j++) {
        double activation = hiddenLayerBias[j];
        activation += input1 * hiddenWeight[0][j] + input2 * hiddenWeight[1][j];
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


int process_xor(void) {
    size_t repet = 10000;
    const double lr = 0.1f;

    double hiddenLayer[nbHiddenNode];
    double outputLayer[output];
    double hiddenLayerBias[nbHiddenNode];
    double outputLayerBias[output];

    double hiddenWeight[nbInput][nbHiddenNode];
    double outPutWeight[nbHiddenNode][output];

    double training_input[nbTest][nbInput] = {{0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {0.0f, 1.0f},
                                              {1.0f, 1.0f}};

    double training_output[nbTest][output] = {{1.0f, 0.0f},  
                                              {0.0f, 1.0f},  
                                              {0.0f, 1.0f},  
                                              {1.0f, 0.0f}}; 

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

            // Impression des résultats périodiquement
            if (epoch % 2000 == 0) {
                printf("Epoch: %zu, Input: %g %g, Output: %g %g, Predicted output: %g %g\n",
                       epoch, training_input[i][0], training_input[i][1],
                       outputLayer[0], outputLayer[1],
                       training_output[i][0], training_output[i][1]);
            }

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

    double input1, input2;
    printf("Enter two values (0 or 1): ");
    scanf("%lf %lf", &input1, &input2);
    
    double predictedOutput[output];
    predict(input1, input2, hiddenWeight, outPutWeight, hiddenLayerBias, outputLayerBias, predictedOutput);
    if (predictedOutput[0]<0.1) printf("Predicted output: 0\n");
    else printf("Predicted output: 1\n");
    
    
    

    return 0;
}