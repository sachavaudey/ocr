#include "Xor.h"

double sigmoid(double x) { return 1/(1+exp(-x)); }
double dsigmoid(double x) { return x * (1 - x); }

double init_weight() { return ((double)rand()) / ((double)RAND_MAX); }

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

void predict(double input1, double input2, double hiddenWeight[NB_INPUT][NB_HIDDEN_NODE], 
             double outputWeight[NB_HIDDEN_NODE][NB_OUTPUT], double hiddenLayerBias[NB_HIDDEN_NODE], 
             double outputLayerBias[NB_OUTPUT], double* outputLayer) {
    double hiddenLayer[NB_HIDDEN_NODE];

    for (size_t j = 0; j < NB_HIDDEN_NODE; j++) {
        double activation = hiddenLayerBias[j];
        activation += input1 * hiddenWeight[0][j] + input2 * hiddenWeight[1][j];
        hiddenLayer[j] = sigmoid(activation);
    }

    double outputRaw[NB_OUTPUT];
    for (size_t j = 0; j < NB_OUTPUT; j++) {
        double activation = outputLayerBias[j];
        for (size_t k = 0; k < NB_HIDDEN_NODE; k++) {
            activation += hiddenLayer[k] * outputWeight[k][j];
        }
        outputRaw[j] = activation;
    }

    softmax(outputRaw, outputLayer, NB_OUTPUT);
}

int process_xor(void) {
    size_t repet = 10000;
    const double lr = 0.1f;

    double hiddenLayer[NB_HIDDEN_NODE];
    double outputLayer[NB_OUTPUT];
    double hiddenLayerBias[NB_HIDDEN_NODE];
    double outputLayerBias[NB_OUTPUT];

    double hiddenWeight[NB_INPUT][NB_HIDDEN_NODE];
    double outputWeight[NB_HIDDEN_NODE][NB_OUTPUT];

    double training_input[4][NB_INPUT] = {{0.0f, 0.0f},
                                          {1.0f, 0.0f},
                                          {0.0f, 1.0f},
                                          {1.0f, 1.0f}};

    double training_output[4][NB_OUTPUT] = {{1.0f, 0.0f},  
                                            {0.0f, 1.0f},  
                                            {0.0f, 1.0f},  
                                            {1.0f, 0.0f}}; 

    for (size_t i = 0; i < NB_INPUT; i++) {
        for (size_t j = 0; j < NB_HIDDEN_NODE; j++) {
            hiddenWeight[i][j] = init_weight();
        }
    }

    for (size_t i = 0; i < NB_HIDDEN_NODE; i++) {
        for (size_t j = 0; j < NB_OUTPUT; j++) {
            outputWeight[i][j] = init_weight();
        }
    }

    for (size_t i = 0; i < NB_OUTPUT; i++) {
        outputLayerBias[i] = init_weight();
    }

    for (size_t i = 0; i < NB_HIDDEN_NODE; i++) {
        hiddenLayerBias[i] = init_weight();
    }

    int trainingsetorder[] = {0, 1, 2, 3};

    for (size_t epoch = 0; epoch < repet; epoch++) {
        shuffle(trainingsetorder, 4);
        for (size_t x = 0; x < 4; x++) {
            int i = trainingsetorder[x];

            for (size_t j = 0; j < NB_HIDDEN_NODE; j++) {
                double activation = hiddenLayerBias[j];
                for (size_t k = 0; k < NB_INPUT; k++) {
                    activation += training_input[i][k] * hiddenWeight[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }

            double outputRaw[NB_OUTPUT];
            for (size_t j = 0; j < NB_OUTPUT; j++) {
                double activation = outputLayerBias[j];
                for (size_t k = 0; k < NB_HIDDEN_NODE; k++) {
                    activation += hiddenLayer[k] * outputWeight[k][j];
                }
                outputRaw[j] = activation;
            }

            softmax(outputRaw, outputLayer, NB_OUTPUT);

            if (epoch % 2000 == 0) {
                printf("Epoch: %zu, Input: %g %g, Output: %g %g, Predicted output: %g %g\n",
                       epoch, training_input[i][0], training_input[i][1],
                       outputLayer[0], outputLayer[1],
                       training_output[i][0], training_output[i][1]);
            }

            double DeltaOutput[NB_OUTPUT];
            for (size_t j = 0; j < NB_OUTPUT; j++) {
                double error = training_output[i][j] - outputLayer[j];
                DeltaOutput[j] = error;  
            }

            double deltaHidden[NB_HIDDEN_NODE];
            for (size_t j = 0; j < NB_HIDDEN_NODE; j++) {
                double error = 0.0f;
                for (size_t k = 0; k < NB_OUTPUT; k++) {
                    error += DeltaOutput[k] * outputWeight[j][k];
                }
                deltaHidden[j] = error * dsigmoid(hiddenLayer[j]);
            }

            for (size_t j = 0; j < NB_OUTPUT; j++) {
                outputLayerBias[j] += DeltaOutput[j] * lr;
                for (size_t k = 0; k < NB_HIDDEN_NODE; k++) {
                    outputWeight[k][j] += hiddenLayer[k] * DeltaOutput[j] * lr;
                }
            }

            for (size_t j = 0; j < NB_HIDDEN_NODE; j++) {
                hiddenLayerBias[j] += deltaHidden[j] * lr;
                for (size_t k = 0; k < NB_INPUT; k++) {
                    hiddenWeight[k][j] += training_input[i][k] * deltaHidden[j] * lr;
                }
            }
        }
    }

    double input1, input2;
    printf("Enter two values (0 or 1): ");
    if (scanf("%lf %lf", &input1, &input2) == 1) errx(EXIT_FAILURE, "Error due to scan f");
    
    double predictedOutput[NB_OUTPUT];
    predict(input1, input2, hiddenWeight, outputWeight, hiddenLayerBias, outputLayerBias, predictedOutput);
    if (predictedOutput[0]<0.1) printf("Predicted output: 0\n");
    else printf("Predicted output: 1\n");
    
    return 0;
}