#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img_traitement.c"
#include "transpose_image.c"
//#include "predict.h"


//create ./o ou compile 
//clang $(pkg-config --cflags --libs sdl2 SDL2_image) -g -o t predict.c && clear && ./t



#define FILENAME_SIZE 100 
#define INPUT_SIZE 900         
#define HIDDEN_SIZE 60       
#define OUTPUT_SIZE 26       
#define BATCH_SIZE 26        
#define LEARNING_RATE 0.1 
#define NBTEST 70    
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

void remplir_chemins_images(char* images[BATCH_SIZE], const char* prefixe, const char* suffixe) {
    char* lettres_min[BATCH_SIZE] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                                "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    
    char lettres[BATCH_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    
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
            snprintf(res[c], FILENAME_SIZE,"../results/%zu_%zu.png",i,j );
            printf("%s et c= %d\n",res[c],c);
            c++;

        }
        
    }
    
}


int* search_size(int* res)
{
    //int* res=malloc(2*sizeof(int));
    int c=0;
    
    while (1){
        char* var=malloc(FILENAME_SIZE*sizeof(char));
        snprintf(var,FILENAME_SIZE,"../results-2/0.%d.png",c);   
        FILE *file = fopen(var, "r");
        if (file==NULL) {
            res[0]=c;
            break;}
        fclose(file);
        c++;
    } 

     c=0;
    
    while (1){
        char* var=malloc(FILENAME_SIZE*sizeof(char));
        snprintf(var,FILENAME_SIZE,"../results-2/%d.0.png",c);   
        FILE *file = fopen(var, "r");
        if (file==NULL) 
        {
            res[1]=c;
            break;
        }
        fclose(file);
        c++;
    } 
    printf("%d,%d",res[0],res[1]);
    return res;

    
}



void create_grid(char* tab,int a,int b)
{
    FILE *file = fopen("../solver/grid", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    int c=0;

    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            fprintf(file, "%c", tab[c++]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}







int main()
{
    process_transforme(8);
    load_hidden_bias("../save_value_prime/hiddenLayerBias.txt");
    load_output_bias("../save_value_prime/OutputLayerBias.txt");
    load_weight_hidden_output("../save_value_prime/weight_hidden_output.txt");
    load_weight_hidden_input("../save_value_prime/weight_hidden_input.txt");
    
    int* size=malloc(2*sizeof(int));
    search_size(size);

    
    
    
    
    //create_grid(r,13,2);
    
    
    
    
    //printf("%s,%s",res[0],res[1]);
    //printf("%d\n",search_size("../results/"));
    
    
    



    /*char* v[10];
    int c=0;
    int t=0;
    char* y[10];
    for (size_t j = 0; j < 10; j++)
        {
            v[c]=malloc(100*sizeof(char));
            y[c]=malloc(100*sizeof(char));
            //snprintf(res[c++], FILENAME_SIZE,"../results/%d_%d.png",i,j);
            //snprintf(res[c++], FILENAME_SIZE,"output/%d_%d.png",i,j);
            
            snprintf(v[c], FILENAME_SIZE,"../results-2/%d.0.png",t);
            snprintf(v[c++], FILENAME_SIZE,"output/%d.0.png",t++);
            process_image(v[j],y[j]);
            //printf("%s\n",res[c-1]);
        }*/
    


    
    char* res[10];
    int c=0;
    int t=1;
    //for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            res[c]=malloc(100*sizeof(char));
            //snprintf(res[c++], FILENAME_SIZE,"../results/%d_%d.png",i,j);
            //snprintf(res[c++], FILENAME_SIZE,"output/%d_%d.png",i,j);
            snprintf(res[c++], FILENAME_SIZE,"output/%d.0.png",t++);
            printf("%s\n",res[c-1]);
        }
    }
    //res[0]="../images_test/dataset/B/B1.png";
    
    int pourc=0;
    char* r=malloc(52*sizeof(char));
    for (size_t i = 0; i < 10; i++)
    {
            double new_input[INPUT_SIZE];
            
            double* resultats = traitements_test(res[i]);
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
            printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
            
        }
        //printf("La lettre %c = %c\n",lettre[i],lettre[j]);
        printf("La lettre %c\n",lettre[j]);
        printf("\n\n");
        if (lettre[i]==lettre[j]) pourc++;
        r[i]=lettre[j];        
    }
    printf("Le pourcentage de réussite est de %d",pourc*100/52);


    create_grid(r,size[0],size[1]);

    



    return 0;
}



