#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traitement_image.c"


#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 16       
#define OUTPUT_SIZE 26         
#define BATCH_SIZE 26           
#define LEARNING_RATE 0.1     
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
            fprintf(file, "%f ", hiddenWeight[i][j]);
        }
        // Ajouter un saut de ligne après chaque ligne de la matrice
        fprintf(file, "\n");
    }

    FILE *file2 = fopen("outPutWeight", "w");
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            // Écrire chaque élément de la matrice dans le fichier
            fprintf(file2, "%f ", outPutWeight[i][j]);
        }
        // Ajouter un saut de ligne après chaque ligne de la matrice
        fprintf(file, "\n");
    }

    FILE *file3 = fopen("hiddenLayerBias", "w");
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        
            // Écrire chaque élément de la matrice dans le fichier
        fprintf(file3, "%f ", hiddenLayerBias[i]);
    }

    FILE *file4 = fopen("OutputLayerBias", "w");
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        
        fprintf(file4, "%f ", outputLayerBias[i]);
    }

    fclose(file);
    fclose(file2);
    fclose(file3);
    fclose(file4);
}


void remplir_chemins_images(char* images[BATCH_SIZE], const char* prefixe, const char* suffixe) {
    char lettres[BATCH_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    
    for (size_t i = 0; i < BATCH_SIZE; i++) {
        
        images[i] = (char*)malloc(FILENAME_SIZE * sizeof(char));
        snprintf(images[i], FILENAME_SIZE, "%s/%c/%c%s.PNG", prefixe, lettres[i] + 32, lettres[i], suffixe);
    }
}




int main(int argc,char** argv) {
    // Initialisation des poids et des biais
    
    double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
    double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
    double hidden_bias[HIDDEN_SIZE];
    double output_bias[OUTPUT_SIZE];

    // Initialisation aléatoire des poids
    
    init_weights(weights_input_hidden, INPUT_SIZE, HIDDEN_SIZE);
    init_weights(weights_hidden_output, HIDDEN_SIZE, OUTPUT_SIZE);
    init_bias(hidden_bias, HIDDEN_SIZE);
    init_bias(output_bias, OUTPUT_SIZE);

   
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


    
    
    


    char* images1[BATCH_SIZE];
    char* images2[BATCH_SIZE];
    char* images3[BATCH_SIZE];
    char* images4[BATCH_SIZE];
    char* images5[BATCH_SIZE];
    char* images6[BATCH_SIZE];
    char* images7[BATCH_SIZE];
    char* images8[BATCH_SIZE];
    char* images9[BATCH_SIZE];
    char* images10[BATCH_SIZE];
    char* images11[BATCH_SIZE];
    char* images12[BATCH_SIZE];
    char* images13[BATCH_SIZE];
    char* images14[BATCH_SIZE];
    char* images15[BATCH_SIZE];
    char* images16[BATCH_SIZE];
    char* images17[BATCH_SIZE];
    char* images18[BATCH_SIZE];
    char* images19[BATCH_SIZE];
    char* images20[BATCH_SIZE];
    char* images21[BATCH_SIZE];
    char* images22[BATCH_SIZE];
    char* images23[BATCH_SIZE];
    char* images24[BATCH_SIZE];
    char* images25[BATCH_SIZE];
    char* images26[BATCH_SIZE];
    char* images27[BATCH_SIZE];
    char* images28[BATCH_SIZE];
    char* images29[BATCH_SIZE];
    char* images30[BATCH_SIZE];
    char* images31[BATCH_SIZE];
    char* images32[BATCH_SIZE];
    char* images33[BATCH_SIZE];
    char* images34[BATCH_SIZE];
    char* images35[BATCH_SIZE];
    char* images36[BATCH_SIZE];
    char* images37[BATCH_SIZE];
    char* images38[BATCH_SIZE];
    char* images39[BATCH_SIZE];
    char* images40[BATCH_SIZE];
    char* images41[BATCH_SIZE];




    double batch_input1[BATCH_SIZE][INPUT_SIZE];
    double batch_input2[BATCH_SIZE][INPUT_SIZE];
    double batch_input3[BATCH_SIZE][INPUT_SIZE];
    double batch_input4[BATCH_SIZE][INPUT_SIZE];
    double batch_input5[BATCH_SIZE][INPUT_SIZE];
    double batch_input6[BATCH_SIZE][INPUT_SIZE];
    double batch_input7[BATCH_SIZE][INPUT_SIZE];
    double batch_input8[BATCH_SIZE][INPUT_SIZE];
    double batch_input9[BATCH_SIZE][INPUT_SIZE];
    double batch_input10[BATCH_SIZE][INPUT_SIZE];
    double batch_input11[BATCH_SIZE][INPUT_SIZE];
    double batch_input12[BATCH_SIZE][INPUT_SIZE];
    double batch_input13[BATCH_SIZE][INPUT_SIZE];
    double batch_input14[BATCH_SIZE][INPUT_SIZE];
    double batch_input15[BATCH_SIZE][INPUT_SIZE];
    double batch_input16[BATCH_SIZE][INPUT_SIZE];
    double batch_input17[BATCH_SIZE][INPUT_SIZE];
    double batch_input18[BATCH_SIZE][INPUT_SIZE];
    double batch_input19[BATCH_SIZE][INPUT_SIZE];
    double batch_input20[BATCH_SIZE][INPUT_SIZE];
    double batch_input21[BATCH_SIZE][INPUT_SIZE];
    double batch_input22[BATCH_SIZE][INPUT_SIZE];
    double batch_input23[BATCH_SIZE][INPUT_SIZE];
    double batch_input24[BATCH_SIZE][INPUT_SIZE];
    double batch_input25[BATCH_SIZE][INPUT_SIZE];
    double batch_input26[BATCH_SIZE][INPUT_SIZE];
    double batch_input27[BATCH_SIZE][INPUT_SIZE];
    double batch_input28[BATCH_SIZE][INPUT_SIZE];
    double batch_input29[BATCH_SIZE][INPUT_SIZE];
    double batch_input30[BATCH_SIZE][INPUT_SIZE];
    double batch_input31[BATCH_SIZE][INPUT_SIZE];
    double batch_input32[BATCH_SIZE][INPUT_SIZE];
    double batch_input33[BATCH_SIZE][INPUT_SIZE];
    double batch_input34[BATCH_SIZE][INPUT_SIZE];
    double batch_input35[BATCH_SIZE][INPUT_SIZE];
    double batch_input36[BATCH_SIZE][INPUT_SIZE];
    double batch_input37[BATCH_SIZE][INPUT_SIZE];
    double batch_input38[BATCH_SIZE][INPUT_SIZE];
    double batch_input39[BATCH_SIZE][INPUT_SIZE];
    double batch_input40[BATCH_SIZE][INPUT_SIZE];
    double batch_input41[BATCH_SIZE][INPUT_SIZE];


    remplir_chemins_images(images1, "images_test/dataset", "1");
    remplir_chemins_images(images2, "images_test/dataset", "2");
    remplir_chemins_images(images3, "images_test/dataset", "3");
    remplir_chemins_images(images4, "images_test/dataset", "4");
    remplir_chemins_images(images5, "images_test/dataset", "5");
    remplir_chemins_images(images6, "images_test/dataset", "6");
    remplir_chemins_images(images7, "images_test/dataset", "7");
    remplir_chemins_images(images8, "images_test/dataset", "8");
    remplir_chemins_images(images9, "images_test/dataset", "9");
    remplir_chemins_images(images10, "images_test/dataset", "10");
    remplir_chemins_images(images11, "images_test/dataset", "11");
    remplir_chemins_images(images12, "images_test/dataset", "12");
    remplir_chemins_images(images13, "images_test/dataset", "13");
    remplir_chemins_images(images14, "images_test/dataset", "14");
    remplir_chemins_images(images15, "images_test/dataset", "15");
    remplir_chemins_images(images16, "images_test/dataset", "16");
    remplir_chemins_images(images17, "images_test/dataset", "17");
    remplir_chemins_images(images18, "images_test/dataset", "18");
    remplir_chemins_images(images19, "images_test/dataset", "19");
    remplir_chemins_images(images20, "images_test/dataset", "20");
    remplir_chemins_images(images21, "images_test/dataset", "21");
    remplir_chemins_images(images22, "images_test/dataset", "22");
    remplir_chemins_images(images23, "images_test/dataset", "23");
    remplir_chemins_images(images24, "images_test/dataset", "24");
    remplir_chemins_images(images25, "images_test/dataset", "25");
    remplir_chemins_images(images26, "images_test/dataset", "26");
    remplir_chemins_images(images27, "images_test/dataset", "27");
    remplir_chemins_images(images28, "images_test/dataset", "28");
    remplir_chemins_images(images29, "images_test/dataset", "29");
    remplir_chemins_images(images30, "images_test/dataset", "30");
    remplir_chemins_images(images31, "images_test/dataset", "31");
    remplir_chemins_images(images32, "images_test/dataset", "32");
    remplir_chemins_images(images33, "images_test/dataset", "33");
    remplir_chemins_images(images34, "images_test/dataset", "34");
    remplir_chemins_images(images35, "images_test/dataset", "35");
    remplir_chemins_images(images36, "images_test/dataset", "36");
    remplir_chemins_images(images37, "images_test/dataset", "37");
    //remplir_chemins_images(images38, "images_test/dataset", "38");
    remplir_chemins_images(images39, "images_test/dataset", "39");
    //remplir_chemins_images(images40, "images_test/dataset", "40");
    


    
    //remplirTestAvecImages_black(batch_input40,images40);
    remplirTestAvecImages_black(batch_input39,images39);
    //remplirTestAvecImages_black(batch_input38,images38);
    remplirTestAvecImages_black(batch_input37,images37);
    remplirTestAvecImages_black(batch_input36,images36);
    remplirTestAvecImages_black(batch_input35,images35);
    remplirTestAvecImages_black(batch_input34,images34);
    remplirTestAvecImages_black(batch_input33,images33);
    remplirTestAvecImages_black(batch_input32,images32);
    remplirTestAvecImages_black(batch_input31,images31);
    remplirTestAvecImages_black(batch_input30,images30);
    remplirTestAvecImages_black(batch_input29,images29);
    remplirTestAvecImages_black(batch_input28,images28);
    remplirTestAvecImages_black(batch_input27,images27);
    remplirTestAvecImages_black(batch_input26,images26);
    remplirTestAvecImages_black(batch_input25,images25);
    remplirTestAvecImages_black(batch_input24,images24);
    remplirTestAvecImages_black(batch_input23,images23);
    remplirTestAvecImages_black(batch_input22,images22);
    remplirTestAvecImages_black(batch_input21,images21);
    remplirTestAvecImages_black(batch_input20,images20);
    remplirTestAvecImages_black(batch_input19,images19);
    remplirTestAvecImages_black(batch_input18,images18);
    remplirTestAvecImages_black(batch_input17,images17);
    remplirTestAvecImages_black(batch_input16,images16);
    remplirTestAvecImages_black(batch_input15,images15);
    remplirTestAvecImages_black(batch_input14,images14);
    remplirTestAvecImages_black(batch_input13,images13);
    remplirTestAvecImages_black(batch_input12,images12);
    remplirTestAvecImages_black(batch_input11,images11);
    remplirTestAvecImages_black(batch_input10,images10);
    remplirTestAvecImages_black(batch_input9,images9);
    remplirTestAvecImages_black(batch_input8,images8);
    remplirTestAvecImages_black(batch_input7,images7);
    remplirTestAvecImages_black(batch_input6,images6);
    remplirTestAvecImages_black(batch_input5,images5);
    remplirTestAvecImages_black(batch_input4,images4);
    remplirTestAvecImages_black(batch_input3,images3);
    remplirTestAvecImages_black(batch_input2,images2);
    remplirTestAvecImages_black(batch_input1,images1);




    // Entraînement du réseau
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double batch_hidden[BATCH_SIZE][HIDDEN_SIZE];
        double batch_output[BATCH_SIZE][OUTPUT_SIZE];
        if (epoch%100==0) printf("%d\n",EPOCHS-epoch);
        
        forward_batch(batch_input1, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input1, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        forward_batch(batch_input2, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input2, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        forward_batch(batch_input3, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input3, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input4, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input4, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input5, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input5, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input6, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input6, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input7, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input7, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input8, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input8, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
         forward_batch(batch_input9, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input9, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input10, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input10, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input11, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input11, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        forward_batch(batch_input12, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input12, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
    
        forward_batch(batch_input13, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input13, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input14, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input14, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input15, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input15, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input16, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input16, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input17, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input17, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input18, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input18, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input19, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input19, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input20, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input20, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input21, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input21, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input22, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input22, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input23, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input23, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input24, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input24, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input25, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input25, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input26, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input26, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input27, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input27, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input28, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input28, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input29, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input29, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input30, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input30, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input31, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input31, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input32, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input32, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input33, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input33, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input34, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input34, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input35, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input35, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        


        forward_batch(batch_input36, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input36, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        forward_batch(batch_input37, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input37, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        
        forward_batch(batch_input39, weights_input_hidden, hidden_bias, batch_hidden,
                      weights_hidden_output, output_bias, batch_output, BATCH_SIZE);
        backpropagation_batch(batch_input39, batch_hidden, batch_output, batch_target,
                              weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, BATCH_SIZE);
        
        
        


    }

    
    save_weights(weights_input_hidden, weights_hidden_output,  hidden_bias, output_bias);

/*char* res[BATCH_SIZE]=
    {
        "images_test/dataset/A/A3.PNG",
        "images_test/dataset/B/B2.PNG",
        "images_test/dataset/C/C2.PNG",
        "images_test/dataset/D/D2.PNG",
        "images_test/dataset/E/E2.PNG",
        "images_test/dataset/F/F2.PNG",
        "images_test/dataset/G/G2.PNG",
        "images_test/dataset/H/H2.PNG",
        "images_test/dataset/I/I2.PNG",
        "images_test/dataset/J/J2.PNG",
        "images_test/dataset/K/K2.PNG",
        "images_test/dataset/L/L2.PNG",
        "images_test/dataset/M/M2.PNG",
        "images_test/dataset/N/N2.PNG",
        "images_test/dataset/O/O2.PNG",
        "images_test/dataset/P/P2.PNG",
        "images_test/dataset/Q/Q2.PNG",
        "images_test/dataset/R/R2.PNG",
        "images_test/dataset/S/S2.PNG",
        "images_test/dataset/T/T2.PNG",
        "images_test/dataset/U/U2.PNG",
        "images_test/dataset/V/V2.PNG",
        "images_test/dataset/W/W2.PNG",
        "images_test/dataset/X/X2.PNG",
        "images_test/dataset/Y/Y2.PNG",
        "images_test/dataset/Z/Z2.PNG",
        
    };*/
    char* res[BATCH_SIZE];
    remplir_chemins_images(res,"images_test/dataset","40");

    int pourc=0;
    for (size_t i = 0; i < 26; i++)
    {
            double new_input[INPUT_SIZE];
            
            double* resultats = traitements_test(res[i]);
            for (size_t j = 0; j < INPUT_SIZE; j++) 
            {
                new_input[j] = resultats[j];
            }               
            double prediction[OUTPUT_SIZE];
            predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);
            char lettre[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
            
        
        // Afficher les résultats de la prédiction
        
        int j=0;
        double max=prediction[0];
        for (size_t a = 0; a < OUTPUT_SIZE; a++)
        {
            if (max<prediction[a])
            {
                max=prediction[a];
                j=a;
            }
        }
        for (int i = 0; i < OUTPUT_SIZE; i++) {
        //printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
        }
        printf("La lettre %c = %c\n",lettre[i],lettre[j]);
        if (lettre[i]==lettre[j]) pourc++;
        
    }
    printf("Le pourcentage de réussite est de %d",pourc*100/26);
    



    /*double new_input[INPUT_SIZE];
    double* resultats = traitements(argv[1]);
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

    int j=0;
        double max=prediction[0];
        for (size_t a = 0; a < 26; a++)
        {
            if (max<prediction[a])
            {
                max=prediction[a];
                j=a;
            }
        }
        printf("\nLa lettre prédite est un %c",lettre[j]);
    */





    






    return 0;
}
