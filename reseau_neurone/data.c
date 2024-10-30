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
#define EPOCHS 1000
  


//ne pas oublier de passer les images en noir

double sigmoid(double x) {return 1 / (1 + exp(-x));}

double sigmoid_derivative(double x) {return x * (1 - x);}



void init_weights(double weights[][HIDDEN_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
}

void init_bias(double bias[], int size) {
    for (int i = 0; i < size; i++) {
        bias[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }
}

void init_target(double batch_target[BATCH_SIZE][OUTPUT_SIZE])
{
    for (int i = 0; i < BATCH_SIZE; i++) {
        // Parcourir chaque colonne (output_size)
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            // Mettre 1 uniquement à l'index correspondant à i, sinon 0
            batch_target[i][j] = (i == j) ? 1.0 : 0.0;
        }
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


void save_weights(double hiddenoutput[HIDDEN_SIZE][OUTPUT_SIZE], 
                  double outPutWeight[INPUT_SIZE][HIDDEN_SIZE], 
                  double hiddenLayerBias[HIDDEN_SIZE], 
                  double outputLayerBias[OUTPUT_SIZE]) {
    
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
    fprintf(file3, "\n");  
    fclose(file3);  

    FILE *file4 = fopen("save_value/OutputLayerBias.txt", "w"); 
    if (file4 == NULL) {
        perror("Error opening file for output layer bias");
        return;
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        fprintf(file4, "%f ", outputLayerBias[i]);
    }
    fprintf(file4, "\n");  
    fclose(file4);  
}


void remplir_chemins_images(char* images[BATCH_SIZE], const char* prefixe, const char* suffixe) {
    char* lettres_min[BATCH_SIZE] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                                "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "aa", "bb", "cc", "dd","ee","ff","gg","hh","ii","jj","kk","ll","mm","nn","oo","pp","qq","rr","ss","tt","uu","vv","ww","xx","yy","zz"};
    
    char lettres[BATCH_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
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



int main(int argc,char** argv) {
    // Initialisation des poids et des biais
    
    /*double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
    double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
    double hidden_bias[HIDDEN_SIZE];
    double output_bias[OUTPUT_SIZE];
    double batch_target[BATCH_SIZE][OUTPUT_SIZE];*/

    
    double **weights_input_hidden = malloc(INPUT_SIZE * sizeof(double *));
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights_input_hidden[i] = malloc(HIDDEN_SIZE * sizeof(double));
    }

    double **weights_hidden_output = malloc(HIDDEN_SIZE * sizeof(double *));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        weights_hidden_output[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }

    double *hidden_bias = malloc(HIDDEN_SIZE * sizeof(double));
    double *output_bias = malloc(OUTPUT_SIZE * sizeof(double));
    
    double **batch_target = malloc(BATCH_SIZE * sizeof(double *));
    for (int i = 0; i < BATCH_SIZE; i++) {
        batch_target[i] = malloc(OUTPUT_SIZE * sizeof(double));
    }


    // Initialisation aléatoire des poids
    
    init_weights(weights_input_hidden, INPUT_SIZE, HIDDEN_SIZE);
    init_weights(weights_hidden_output, HIDDEN_SIZE, OUTPUT_SIZE);
    init_bias(hidden_bias, HIDDEN_SIZE);
    init_bias(output_bias, OUTPUT_SIZE);
    init_target(batch_target);

   



    double batch_inputs[NBTEST][BATCH_SIZE][INPUT_SIZE];
    char* images[NBTEST][BATCH_SIZE]; 
        
    for (int i = 0; i < NBTEST+1; i++) {
        char index[3]; // Assurez-vous que la taille est suffisante pour le numéro d'index
        snprintf(index, sizeof(index), "%d", i + 1); 
        remplir_chemins_images(images[i], "images_test/dataset", index);
    }

    for (int i = 0; i < NBTEST; i++) { 
        remplirTestAvecImages_black(batch_inputs[i], images[i]);
    }

    for (size_t i = 0; i < NBTEST; i++)
    {
        for (size_t j = 0; j < BATCH_SIZE; j++)
        {
            free(images[i][j]);
        }
        
    }

    
    







   



    int trainingsetorder[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52};
    // Entraînement du réseau
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        shuffle(trainingsetorder, OUTPUT_SIZE);
        double batch_hidden[BATCH_SIZE][HIDDEN_SIZE];
        double batch_output[BATCH_SIZE][OUTPUT_SIZE];
        if (epoch%100==0) printf("%d\n",EPOCHS-epoch);
        
        for (int i = 0; i < NBTEST; i++) 
        {
            forward_batch(batch_inputs[i], weights_input_hidden, hidden_bias, batch_hidden,
                        weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
            backpropagation_batch(batch_inputs[i], batch_hidden, batch_output, batch_target,
                                weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        }
    }

    

    

    
    save_weights(weights_hidden_output,weights_input_hidden,   hidden_bias, output_bias);

    /*for (int i = 0; i < INPUT_SIZE; i++) {
    free(weights_input_hidden[i]);  // Free each row
}
free(weights_input_hidden);  // Free the main pointer

// Free the weights_hidden_output array
for (int i = 0; i < HIDDEN_SIZE; i++) {
    free(weights_hidden_output[i]);  // Free each row
}
free(weights_hidden_output);  // Free the main pointer

// Free the hidden_bias and output_bias arrays
free(hidden_bias);
free(output_bias);

// Free the batch_target array
for (int i = 0; i < BATCH_SIZE; i++) {
    free(batch_target[i]);  // Free each row
}
free(batch_target);*/
    



    





    






    return 0;
}
