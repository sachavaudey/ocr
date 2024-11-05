#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traitement_image.c"


#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 65       
#define OUTPUT_SIZE 52       
#define BATCH_SIZE 52        
#define LEARNING_RATE 0.1 
#define NBTEST 45    
#define EPOCHS 10000


double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
double hidden_bias[HIDDEN_SIZE];
double output_bias[OUTPUT_SIZE];

void load_hidden_bias(char* filename)
{
    FILE *file = fopen(filename, "r");
    

    
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        if (fscanf(file, "%lf", &hidden_bias[i]) != 1) {  // Lecture en tant que double
            printf("Erreur: fichier %s contient moins de %d valeurs.\n", filename, HIDDEN_SIZE);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void load_output_bias(char* filename)
{
    FILE *file = fopen(filename, "r");
    

    
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (fscanf(file, "%lf", &output_bias[i]) != 1) {  // Lecture en tant que double
            printf("Erreur: fichier %s contient moins de %d valeurs.\n", filename, HIDDEN_SIZE);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void load_weight_hidden_output(char *filename)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s.\n", filename);
        return;
    }

    
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            if (fscanf(file, "%lf", &weights_hidden_output[i][j]) != 1) {  // Lecture en tant que double
                printf("Erreur: fichier %s contient moins de %d valeurs à la ligne %d.\n", filename, HIDDEN_SIZE, i + 1);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}

void load_weight_hidden_input(char *filename)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s.\n", filename);
        return;
    }

    // On suppose que le fichier contient plusieurs lignes de valeurs, 
    // et que chaque ligne a exactement HIDDEN_SIZE valeurs.
    for (int i = 0; i < INPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            if (fscanf(file, "%lf", &weights_input_hidden[i][j]) != 1) {  // Lecture en tant que double
                printf("Erreur: fichier %s contient moins de %d valeurs à la ligne %d.\n", filename, HIDDEN_SIZE, i + 1);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}


double sigmoid(double x) {
    return 1 / (1 + exp(-x));
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




#define h 2
#define l 2


void read_grid(char* res[4])
{
    int c=0;
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < l; j++)
        {
            res[c]=malloc(20*sizeof(char));
            snprintf(res[c], FILENAME_SIZE,"../results/%d_%d.png",i,j );
            printf("%s et c= %d\n",res[c],c);
            c++;

        }
        
    }
    
}






int main()
{
    load_hidden_bias("../save_value/hiddenLayerBias.txt");
    load_output_bias("../save_value/OutputLayerBias.txt");
    load_weight_hidden_output("../save_value/weight_hidden_output.txt");
    load_weight_hidden_input("../save_value/weight_hidden_input.txt");
    
    
    /*char* res[BATCH_SIZE];
    remplir_chemins_images(res,"../images_test/dataset","40");

    int pourc=0;
    for (size_t i = 0; i < OUTPUT_SIZE; i++)
    {
            double new_input[INPUT_SIZE];
            
            double* resultats = traitements_test(res[i]);
            for (size_t j = 0; j < INPUT_SIZE; j++) 
            {
                new_input[j] = resultats[j];
            }               
            double prediction[OUTPUT_SIZE];
            predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);
            char lettre[52]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X'
            ,'Y','Z','a','b','c','d','e','f','g','h','i','j','q','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
            
        
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
        for (int i = 0; i < OUTPUT_SIZE; i++) 
        {
            //printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
        }
        printf("La lettre %c = %c\n",lettre[i],lettre[j]);
        if (lettre[i]==lettre[j]) pourc++;
        
    }
    printf("Le pourcentage de réussite est de %d",pourc*100/52);*/
    /*char* res[h*l];
    
    read_grid(res);
    
    int pourc=0;
    for (size_t i = 0; i < h*l; i++)
    {
            double new_input[INPUT_SIZE];
            printf("res = %s",res[i]);
            double* resultats = traitements(res[i]);
            for (size_t j = 0; j < INPUT_SIZE; j++) 
            {
                new_input[j] = resultats[j];
            }               
            double prediction[4];
            predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);
            char lettre[52]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X'
            ,'Y','Z','a','b','c','d','e','f','g','h','i','j','q','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
            
        
        // Afficher les résultats de la prédiction
        
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
        for (int i = 0; i < 26; i++) 
        {
            //printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
        }
        printf("La lettre est un %c\n",lettre[j]);
        if (lettre[i]==lettre[j]) pourc++;
        
    }
    printf("Le pourcentage de réussite est de %d",pourc*100/52);
    */

    



    return 0;
}



