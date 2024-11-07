#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traitement_image.c"

#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 80       
#define OUTPUT_SIZE 52       
#define BATCH_SIZE 52        
#define LEARNING_RATE 0.1 
#define NBTEST 70    
#define EPOCHS 1000
//Au moins 1h

// Fonction d'activation
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double sigmoid_derivative(double x) { return x * (1 - x); }

// Initialisation des poids avec des tableaux dynamiques
void init_weights(double** weights, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
}

// Initialisation des biais avec un tableau dynamique
void init_bias(double* bias, int size) {
    for (int i = 0; i < size; i++) {
        bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

// Initialisation de la cible avec un tableau dynamique
void init_target(double** batch_target) {
    for (int i = 0; i < BATCH_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            batch_target[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

// Propagation avant pour un lot d'images avec des tableaux dynamiques
void forward_batch(double** batch_input, double** weights_input_hidden, 
                   double* hidden_bias, double** batch_hidden, 
                   double** weights_hidden_output, double* output_bias, 
                   double** batch_output, int batch_size) {

    for (int img = 0; img < batch_size; img++) {
        // Vers couche cachée
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            batch_hidden[img][i] = hidden_bias[i];
            for (int j = 0; j < INPUT_SIZE; j++) {
                batch_hidden[img][i] += batch_input[img][j] * weights_input_hidden[j][i];
            }
            batch_hidden[img][i] = sigmoid(batch_hidden[img][i]);
        }

        // Vers couche de sortie
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            batch_output[img][i] = output_bias[i];
            for (int j = 0; j < HIDDEN_SIZE; j++) {
                batch_output[img][i] += batch_hidden[img][j] * weights_hidden_output[j][i];
            }
            batch_output[img][i] = sigmoid(batch_output[img][i]);
        }
    }
}

// Rétropropagation avec des tableaux dynamiques
void backpropagation_batch(double** batch_input, double** batch_hidden, 
                           double** batch_output, double** batch_target, 
                           double** weights_input_hidden, double** weights_hidden_output, 
                           double* hidden_bias, double* output_bias, int batch_size) {

    double** output_errors = malloc(batch_size * sizeof(double*));
    double** hidden_errors = malloc(batch_size * sizeof(double*));
    for (int i = 0; i < batch_size; i++) {
        output_errors[i] = malloc(OUTPUT_SIZE * sizeof(double));
        hidden_errors[i] = malloc(HIDDEN_SIZE * sizeof(double));
    }

    // Calcul des erreurs de sortie
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            output_errors[img][i] = (batch_target[img][i] - batch_output[img][i]) * sigmoid_derivative(batch_output[img][i]);
        }
    }

    // Calcul des erreurs cachées
    for (int img = 0; img < batch_size; img++) {
        for (int i = 0; i < HIDDEN_SIZE; i++) {
            hidden_errors[img][i] = 0;
            for (int j = 0; j < OUTPUT_SIZE; j++) {
                hidden_errors[img][i] += output_errors[img][j] * weights_hidden_output[i][j];
            }
            hidden_errors[img][i] *= sigmoid_derivative(batch_hidden[img][i]);
        }
    }

    // Mise à jour des poids et des biais
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

    // Libération de la mémoire
    for (int i = 0; i < batch_size; i++) {
        free(output_errors[i]);
        free(hidden_errors[i]);
    }
    free(output_errors);
    free(hidden_errors);
}

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

void predict(double* input, double** hiddenWeight, double** outPutWeight, double* hiddenLayerBias, 
             double* outputLayerBias, double* outputLayer) {
    double* hiddenLayer = (double*)malloc(HIDDEN_SIZE * sizeof(double));

    for (size_t j = 0; j < HIDDEN_SIZE; j++) {
        double activation = hiddenLayerBias[j];
        for (size_t k = 0; k < INPUT_SIZE; k++) {
            activation += input[k] * hiddenWeight[k][j];
        }
        hiddenLayer[j] = sigmoid(activation);
    }

    double* outputRaw = (double*)malloc(OUTPUT_SIZE * sizeof(double));
    for (size_t j = 0; j < OUTPUT_SIZE; j++) {
        double activation = outputLayerBias[j];
        for (size_t k = 0; k < HIDDEN_SIZE; k++) {
            activation += hiddenLayer[k] * outPutWeight[k][j];
        }
        outputRaw[j] = activation;
    }

    softmax(outputRaw, outputLayer, OUTPUT_SIZE);

    free(hiddenLayer);
    free(outputRaw);
}

void remplirTestAvecImages_black(double** test, char** images) {
    for (size_t i = 0; i < BATCH_SIZE; i++) {
        double* resultats = traitements_test(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) {
            test[i][j] = resultats[j];
        }
        free(resultats);
    }
}

void remplirTestAvecImages(double** test, char** images, size_t batch_size, size_t input_size) {
    for (size_t i = 0; i < BATCH_SIZE; i++) {
        double* resultats = traitements(images[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) {
            test[i][j] = resultats[j];
        }
        free(resultats);
    }
}

void remplir_chemins_images(char** images, const char* prefixe, const char* suffixe) {
    char* lettres_min[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                           "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", 
                           "aa", "bb", "cc", "dd", "ee", "ff", "gg", "hh", "ii", "jj", "kk", 
                           "ll", "mm", "nn", "oo", "pp", "qq", "rr", "ss", "tt", "uu", "vv", 
                           "ww", "xx", "yy", "zz"};
    
    char lettres[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    for (size_t i = 0; i < BATCH_SIZE; i++) {
        images[i] = (char*)malloc(FILENAME_SIZE * sizeof(char));
        snprintf(images[i], FILENAME_SIZE, "%s/%s/%c%s.PNG", prefixe, lettres_min[i], lettres[i], suffixe);
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

void save_weights(double** hiddenoutput, double** outPutWeight, double* hiddenLayerBias, 
                  double* outputLayerBias) {
    FILE *file = fopen("../save_value/weight_hidden_output.txt", "w");
    if (file == NULL) {
        perror("Error opening file for hidden-output weights");
        return;
    }
    for (size_t i = 0; i < HIDDEN_SIZE; i++) {
        for (size_t j = 0; j < OUTPUT_SIZE; j++) {
            fprintf(file, "%f ", hiddenoutput[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    FILE *file2 = fopen("../save_value/weight_hidden_input.txt", "w");
    if (file2 == NULL) {
        perror("Error opening file for hidden-input weights");
        return;
    }
    for (size_t i = 0; i < INPUT_SIZE; i++) {
        for (size_t j = 0; j < HIDDEN_SIZE; j++) {
            fprintf(file2, "%f ", outPutWeight[i][j]);
        }
        fprintf(file2, "\n");
    }
    fclose(file2);

    FILE *file3 = fopen("../save_value/hiddenLayerBias.txt", "w");
    if (file3 == NULL) {
        perror("Error opening file for hidden layer bias");
        return;
    }
    for (size_t i = 0; i < HIDDEN_SIZE; i++) {
        fprintf(file3, "%f ", hiddenLayerBias[i]);
    }
    fprintf(file3, "\n");
    fclose(file3);

    FILE *file4 = fopen("../save_value/OutputLayerBias.txt", "w");
    if (file4 == NULL) {
        perror("Error opening file for output layer bias");
        return;
    }
    for (size_t i = 0; i < OUTPUT_SIZE; i++) {
        fprintf(file4, "%f ", outputLayerBias[i]);
    }
    fprintf(file4, "\n");
    fclose(file4);
}

int main(int argc, char** argv) {
    // Initialisation des poids et des biais (allocation dynamique)
    double** weights_input_hidden = malloc(INPUT_SIZE * sizeof(double*));
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights_input_hidden[i] = malloc(HIDDEN_SIZE * sizeof(double));
    }

    double** weights_hidden_output = malloc(HIDDEN_SIZE * sizeof(double*));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        weights_hidden_output[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    double* hidden_bias = malloc(HIDDEN_SIZE * sizeof(double));
    double* output_bias = malloc(OUTPUT_SIZE * sizeof(double));

    double** batch_target = malloc(BATCH_SIZE * sizeof(double*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_target[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    // Initialisation aléatoire des poids
    init_weights(weights_input_hidden, INPUT_SIZE, HIDDEN_SIZE);
    init_weights(weights_hidden_output, HIDDEN_SIZE, OUTPUT_SIZE);
    init_bias(hidden_bias, HIDDEN_SIZE);
    init_bias(output_bias, OUTPUT_SIZE);
    init_target(batch_target);

    double*** batch_inputs = malloc(NBTEST * sizeof(double**));
    for (int i = 0; i < NBTEST; i++) {
        batch_inputs[i] = malloc(BATCH_SIZE * sizeof(double*));
        for (int j = 0; j < BATCH_SIZE; j++) {
            batch_inputs[i][j] = malloc(INPUT_SIZE * sizeof(double));
        }
    }

    char*** images = malloc(NBTEST * sizeof(char**));
    for (int i = 0; i < NBTEST; i++) {
        images[i] = malloc(BATCH_SIZE * sizeof(char*));
        for (int j = 0; j < BATCH_SIZE; j++) {
            images[i][j] = malloc(256 * sizeof(char)); 
        }
    }

    for (int i = 0; i < NBTEST; i++) {
        char index[3];
        snprintf(index, sizeof(index), "%d", i + 1);
        remplir_chemins_images(images[i], "../images_test/dataset", index);
    }

    for (int i = 0; i < NBTEST; i++) {
        remplirTestAvecImages_black(batch_inputs[i], images[i]);
    }

    for (size_t i = 0; i < NBTEST; i++) {
        for (size_t j = 0; j < BATCH_SIZE; j++) {
            free(images[i][j]);
        }
        free(images[i]);
    }
    free(images);

    int trainingsetorder[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                              26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52};
    // Entraînement du réseau
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        shuffle(trainingsetorder, OUTPUT_SIZE);
        double** batch_hidden = malloc(BATCH_SIZE * sizeof(double*));
        for (int i = 0; i < BATCH_SIZE; i++) {
            batch_hidden[i] = malloc(HIDDEN_SIZE * sizeof(double));
        }

        double** batch_output = malloc(BATCH_SIZE * sizeof(double*));
        for (int i = 0; i < BATCH_SIZE; i++) {
            batch_output[i] = malloc(OUTPUT_SIZE * sizeof(double));
        }

        if (epoch % 10 == 0) printf("%d\n", EPOCHS - epoch);

        for (int i = 0; i < NBTEST; i++) {
            forward_batch(batch_inputs[i], weights_input_hidden, hidden_bias, batch_hidden,
                          weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
            backpropagation_batch(batch_inputs[i], batch_hidden, batch_output, batch_target,
                                  weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        }

        // Libération de batch_hidden et batch_output après utilisation
        for (int i = 0; i < BATCH_SIZE; i++) {
            free(batch_hidden[i]);
            free(batch_output[i]);
        }
        free(batch_hidden);
        free(batch_output);
    }




    char** res = malloc(BATCH_SIZE * sizeof(char*));
    for (int i = 0; i < BATCH_SIZE; i++) {
        res[i] = malloc(256 * sizeof(char));
    }
    remplir_chemins_images(res, "../images_test/dataset", "40");

    int pourc = 0;
    for (size_t i = 0; i < OUTPUT_SIZE; i++) {
        double* new_input = malloc(INPUT_SIZE * sizeof(double));
        double* resultats = traitements_test(res[i]);
        for (size_t j = 0; j < INPUT_SIZE; j++) {
            new_input[j] = resultats[j];
        }
        double* prediction = malloc(OUTPUT_SIZE * sizeof(double));
        predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);

        char lettre[52] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                           'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                           'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

        int j = 0;
        double max = prediction[0];
        for (size_t a = 0; a < OUTPUT_SIZE; a++) {
            if (max < prediction[a]) {
                max = prediction[a];
                j = a;
            }
        }

        printf("La lettre %c = %c\n", lettre[i], lettre[j]);
        if (lettre[i] == lettre[j]) pourc++;

        free(new_input);
        free(prediction);
    }
    printf("Le pourcentage de réussite est de %d\n", pourc * 100 / OUTPUT_SIZE);

    for (int i = 0; i < BATCH_SIZE; i++) {
        free(res[i]);
    }
    free(res);

    save_weights(weights_hidden_output, weights_input_hidden, hidden_bias, output_bias);

    // Libération de la mémoire
    for (int i = 0; i < INPUT_SIZE; i++) {
        free(weights_input_hidden[i]);
    }
    free(weights_input_hidden);

    for (int i = 0; i < HIDDEN_SIZE; i++) {
        free(weights_hidden_output[i]);
    }
    free(weights_hidden_output);

    free(hidden_bias);
    free(output_bias);

    for (int i = 0; i < BATCH_SIZE; i++) {
        free(batch_target[i]);
    }
    free(batch_target);

    for (int i = 0; i < NBTEST; i++) {
        for (int j = 0; j < BATCH_SIZE; j++) {
            free(batch_inputs[i][j]);
        }
        free(batch_inputs[i]);
    }
    free(batch_inputs);

    return 0;
}