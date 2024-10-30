#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traitement_image.c"

#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 20       
#define OUTPUT_SIZE 52       
#define BATCH_SIZE 52        
#define LEARNING_RATE 0.01 
#define NBTEST 21    
#define EPOCHS 10

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1 - x);
}

void init_weights(double** weights, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
}

void init_bias(double* bias, int size) {
    for (int i = 0; i < size; i++) {
        bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

void init_target(double** batch_target) {
    for (int i = 0; i < BATCH_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            batch_target[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

// Forward propagation for a batch of images
void forward_batch(double** batch_input, double** weights_input_hidden, 
                   double* hidden_bias, double** batch_hidden, 
                   double** weights_hidden_output, double* output_bias, 
                   double** batch_output, int batch_size) {

    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            batch_hidden[img][i] = hidden_bias[i]; 
            for (int j = 0; j < INPUT_SIZE; j++) {
                batch_hidden[img][i] += batch_input[img][j] * weights_input_hidden[j][i];
            }
            batch_hidden[img][i] = sigmoid(batch_hidden[img][i]);
        }

        for (int i = 0; i < OUTPUT_SIZE; i++) {
            batch_output[img][i] = output_bias[i]; 
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                batch_output[img][i] += batch_hidden[img][j] * weights_hidden_output[j][i];
            }
            batch_output[img][i] = sigmoid(batch_output[img][i]);
        }
    }
}

// Backpropagation for a batch of images
void backpropagation_batch(double** batch_input, double** batch_hidden, 
                           double** batch_output, double** batch_target, 
                           double** weights_input_hidden, double** weights_hidden_output, 
                           double* hidden_bias, double* output_bias, int batch_size) {

    double output_errors[BATCH_SIZE][OUTPUT_SIZE];
    double hidden_errors[BATCH_SIZE][HIDDEN_SIZE];

    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            output_errors[img][i] = (batch_target[img][i] - batch_output[img][i]) * sigmoid_derivative(batch_output[img][i]);
        }
    }

    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            hidden_errors[img][i] = 0;
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                hidden_errors[img][i] += output_errors[img][j] * weights_hidden_output[i][j];
            }
            hidden_errors[img][i] *= sigmoid_derivative(batch_hidden[img][i]);
        }
    }

    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                weights_hidden_output[i][j] += LEARNING_RATE * output_errors[img][j] * batch_hidden[img][i];
            }
        }
    }

    for (int img = 0; img < batch_size; img++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            output_bias[j] += LEARNING_RATE * output_errors[img][j];
        }
    }

    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < INPUT_SIZE; i++) {
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                weights_input_hidden[i][j] += LEARNING_RATE * hidden_errors[img][j] * batch_input[img][i];
            }
        }
    }

    for (int img = 0; img < batch_size; img++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            hidden_bias[j] += LEARNING_RATE * hidden_errors[img][j];
        }
    }
}

// Softmax function
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

// Prediction function
void predict(double* input, double** hiddenWeight, 
             double** outPutWeight, double* hiddenLayerBias, 
             double* outputLayerBias, double* outputLayer) {
    double hiddenLayer[HIDDEN_SIZE];

    for (size_t j = 0; j < HIDDEN_SIZE; j++) {
        double activation = hiddenLayerBias[j];
        for (size_t k = 0; k < INPUT_SIZE; k++) {
            activation += input[k] * hiddenWeight[k][j];
        }
        hiddenLayer[j] = sigmoid(activation);
    }

    double outputRaw[OUTPUT_SIZE];
    for (size_t j = 0; j < OUTPUT_SIZE; j++) {
        double activation = outputLayerBias[j];
        for (size_t k = 0; k < HIDDEN_SIZE; k++) {
            activation += hiddenLayer[k] * outPutWeight[k][j];
        }
        outputRaw[j] = activation;
    }

    softmax(outputRaw, outputLayer, OUTPUT_SIZE);
}

// Function to fill the test set with images
void remplirTestAvecImages(double** test, char** images) {
    for (size_t i = 0; i < BATCH_SIZE; i++) {
        double* resultats = traitements(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) {
            test[i][j] = resultats[j];
        }
        free(resultats);
    }
}

// Function to fill the test set with black images
void remplirTestAvecImages_black(double** test, char** images) {
    for (size_t i = 0; i < BATCH_SIZE; i++) {
        double* resultats = traitements_test(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) {
            test[i][j] = resultats[j];
        }
        free(resultats);
    }
}

// Save weights and biases to files
void save_weights(double** hiddenoutput, double** outPutWeight, 
                  double* hiddenLayerBias, double* outputLayerBias) {
    
    FILE *file = fopen("save_value/weight_hidden_output.txt", "w");
    if (file == NULL) {
        perror("Error opening file for hidden-output weights");
        return;
    }
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            fprintf(file, "%f ", hiddenoutput[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file); 

    FILE *file2 = fopen("save_value/weight_hidden_input.txt", "w");  
    if (file2 == NULL) {
        perror("Error opening file for hidden-input weights");
        return;
    }
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            fprintf(file2, "%f ", outPutWeight[i][j]);
        }
        fprintf(file2, "\n");  
    }
    fclose(file2);  

    FILE *file3 = fopen("save_value/hiddenLayerBias.txt", "w");  
    if (file3 == NULL) {
        perror("Error opening file for hidden layer bias");
        return;
    }
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        fprintf(file3, "%f ", hiddenLayerBias[i]);
    }
    fclose(file3);  

    FILE *file4 = fopen("save_value/outputLayerBias.txt", "w");  
    if (file4 == NULL) {
        perror("Error opening file for output layer bias");
        return;
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        fprintf(file4, "%f ", outputLayerBias[i]);
    }
    fclose(file4);  
}

int main() {
    double** hiddenWeight = malloc(INPUT_SIZE * sizeof(double*));
    for (int i = 0; i < INPUT_SIZE; i++) {
        hiddenWeight[i] = malloc(HIDDEN_SIZE * sizeof(double));
    }

    double** outPutWeight = malloc(HIDDEN_SIZE * sizeof(double*));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        outPutWeight[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    double* hiddenLayerBias = malloc(HIDDEN_SIZE * sizeof(double));
    double* outputLayerBias = malloc(OUTPUT_SIZE * sizeof(double));

    double** batch_input = malloc(BATCH_SIZE * sizeof(double*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_input[i] = malloc(INPUT_SIZE * sizeof(double));
    }

    double** batch_target = malloc(BATCH_SIZE * sizeof(double*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_target[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    double** batch_hidden = malloc(BATCH_SIZE * sizeof(double*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_hidden[i] = malloc(HIDDEN_SIZE * sizeof(double));
    }

    double** batch_output = malloc(BATCH_SIZE * sizeof(double*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_output[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    init_weights(hiddenWeight, INPUT_SIZE, HIDDEN_SIZE);
    init_weights(outPutWeight, HIDDEN_SIZE, OUTPUT_SIZE);
    init_bias(hiddenLayerBias, HIDDEN_SIZE);
    init_bias(outputLayerBias, OUTPUT_SIZE);

    // Load images into the batch_input array.
    char** images = malloc(BATCH_SIZE * sizeof(char*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        images[i] = malloc(FILENAME_SIZE * sizeof(char)); // allocate space for image file names
    }

    // Fill the images array with file names (you can change these names as needed).
    for (int i = 0; i < BATCH_SIZE; i++) {
        snprintf(images[i], FILENAME_SIZE, "image_%d.txt", i); // sample file naming
    }

    // Populate test images
    remplirTestAvecImages(batch_input, images);
    
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        init_target(batch_target); 
        forward_batch(batch_input, hiddenWeight, hiddenLayerBias, 
                      batch_hidden, outPutWeight, outputLayerBias, 
                      batch_output, BATCH_SIZE);

        backpropagation_batch(batch_input, batch_hidden, batch_output, 
                              batch_target, hiddenWeight, outPutWeight, 
                              hiddenLayerBias, outputLayerBias, BATCH_SIZE);
    }

    double* outputLayer = malloc(OUTPUT_SIZE * sizeof(double));
    // Prediction phase
    for (int i = 0; i < NBTEST; i++) {
        double** testImages = malloc(BATCH_SIZE * sizeof(double*));
        for (int j = 0; j < BATCH_SIZE; j++) {
            testImages[j] = malloc(INPUT_SIZE * sizeof(double));
        }

        char** testImagesPaths = malloc(BATCH_SIZE * sizeof(char*));
        for (int j = 0; j < BATCH_SIZE; j++) {
            testImagesPaths[j] = malloc(FILENAME_SIZE * sizeof(char));
            snprintf(testImagesPaths[j], FILENAME_SIZE, "test_image_%d.txt", i * BATCH_SIZE + j);
        }

        remplirTestAvecImages_black(testImages, testImagesPaths);
        for (int j = 0; j < BATCH_SIZE; j++) {
            predict(testImages[j], hiddenWeight, outPutWeight, hiddenLayerBias, outputLayerBias, outputLayer);
            printf("Predictions for image %d: ", i * BATCH_SIZE + j);
            for (int k = 0; k < OUTPUT_SIZE; k++) {
                printf("%f ", outputLayer[k]);
            }
            printf("\n");
        }

        for (int j = 0; j < BATCH_SIZE; j++) {
            free(testImages[j]);
            free(testImagesPaths[j]);
        }
        free(testImages);
        free(testImagesPaths);
    }

    save_weights(hiddenWeight, outPutWeight, hiddenLayerBias, outputLayerBias);

    // Freeing allocated memory
    for (int i = 0; i < INPUT_SIZE; i++) {
        free(hiddenWeight[i]);
    }
    free(hiddenWeight);

    for (int i = 0; i < HIDDEN_SIZE; i++) {
        free(outPutWeight[i]);
    }
    free(outPutWeight);

    free(hiddenLayerBias);
    free(outputLayerBias);

    for (int i = 0; i < BATCH_SIZE; i++) {
        free(batch_input[i]);
        free(batch_target[i]);
        free(batch_hidden[i]);
        free(batch_output[i]);
    }
    free(batch_input);
    free(batch_target);
    free(batch_hidden);
    free(batch_output);

    for (int i = 0; i < BATCH_SIZE; i++) {
        free(images[i]);
    }
    free(images);
    
    free(outputLayer);

    return 0;
}
