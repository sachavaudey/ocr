#include "predict.h"


double weights_input_hidden[INPUT_SIZE][HIDDEN_SIZE];
double weights_hidden_output[HIDDEN_SIZE][OUTPUT_SIZE];
double hidden_bias[HIDDEN_SIZE];
double output_bias[OUTPUT_SIZE];

//function which load data in file
void load_hidden_bias(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        errx(EXIT_FAILURE,
            "Erreur lors de l'ouverture du fichier %s", filename);
    }

    for (int i = 0; i < HIDDEN_SIZE; i++) {
        if (fscanf(file, "%lf ", &hidden_bias[i]) != 1) {
            fprintf(stderr,
                "Erreur : lecture insuffisante dans le fichier %s.\n",
                filename);
            fclose(file);

            errx(EXIT_FAILURE, "Error %s", filename);

        }
    }

    fclose(file);
}

//function which load data in file
void load_output_bias(char* filename)
{
    FILE *file = fopen(filename, "r");
    

    
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (fscanf(file, "%lf", &output_bias[i]) != 1)
        {  // Lecture en tant que double
            printf("Erreur: fichier %s contient moins de %d valeurs.\n",
                    filename, HIDDEN_SIZE);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

//function which load data in file
void load_weight_hidden_output(char *filename)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s.\n",
                filename);
        return;
    }

    
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            if (fscanf(file, "%lf",
                        &weights_hidden_output[i][j]) != 1)
            {  // Lecture en tant que double
printf("Erreur: fichier %s contient moins de %d valeurs à la ligne %d.\n",
                    filename, HIDDEN_SIZE, i + 1);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}

//function which load data in file
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
            if (fscanf(file, "%lf", &weights_input_hidden[i][j]) != 1)
            {  // Lecture en tant que double
    printf("Erreur: fichier %s contient moins de %d valeurs à la ligne %d.\n",
            filename, HIDDEN_SIZE, i + 1);
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}

//function of activation
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

//function which predict the output
void predict(double input[INPUT_SIZE],
            double hiddenWeight[INPUT_SIZE][HIDDEN_SIZE], 
            double outPutWeight[HIDDEN_SIZE][OUTPUT_SIZE],
            double hiddenLayerBias[HIDDEN_SIZE], 
            double outputLayerBias[OUTPUT_SIZE], double* outputLayer)
{
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








//function which search the size of grid
int* search_size(int* res)
{
    
    int c=0;
    
    while (1){
        char* var=malloc(FILENAME_SIZE*sizeof(char));
        //snprintf(var,FILENAME_SIZE,"../results_grid-2/0.%d.png",c); 
        snprintf(var,FILENAME_SIZE,"data/results_gride/0.%d.png",c); 
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
        snprintf(var,FILENAME_SIZE,"data/results_gride/%d.0.png",c);   
        FILE *file = fopen(var, "r");
        if (file==NULL) 
        {
            res[1]=c;
            break;
        }
        fclose(file);
        c++;
    } 
    
    return res;

    
}


//function which create the grid
void create_grid(char* tab,int a,int b)
{
    FILE *file = fopen("data/grid", "w");
    
    int c=0;

    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            fprintf(file, "%c", tab[c++]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}


//function which create the word
void create_word(char* tab,int* a,int b)
{
    FILE *file = fopen("data/word", "w");
    
    int c=0;

    for (int i = 0; i < b; i++) {
        for (int j = 0; j < a[i]; j++) {
            fprintf(file, "%c", tab[c++]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}


int pro_word()
{

    load_hidden_bias("neuron_network/save_value_prime/hiddenLayerBias.txt");
    load_output_bias("neuron_network/save_value_prime/OutputLayerBias.txt");
    load_weight_hidden_output(
            "neuron_network/save_value_prime/weight_hidden_output.txt");
    load_weight_hidden_input(
            "neuron_network/save_value_prime/weight_hidden_input.txt");
    
    int* wor=search_size_word();
    
    
    int taille1=0;
    int taille2=0;
    int i=0;
    //while(wor[i]!=NULL)
    while(wor[i]!=0)
    {
        if (wor[i]!=0) {
            taille1++;
            taille2+=wor[i];
        }
        i++;
    }

    
    
    char* res[taille2];
    int c=0;
    int t=1;
    for (int i = 0; i < taille1; i++)
    {
        t=0;
        printf("%d\n",wor[i]);
        for (int j = 0; j < wor[i]; j++)
        {
            
            res[c]=malloc(100*sizeof(char));
           
            snprintf(res[c++], FILENAME_SIZE,
                    "neuron_network/other/words/%d.%d.png",i,t++);
            //printf("%s\n",res[c-1]);
        }
    }
    
    
    
    char* r=malloc(taille2*sizeof(char));
    for (int i = 0; i < taille2; i++)
    {
            double new_input[INPUT_SIZE];
            
            double* resultats = traitements(res[i]);
            for (size_t j = 0; j < INPUT_SIZE; j++) 
            {
                new_input[j] = resultats[j];
            }               
            double prediction[26];
            predict(new_input, weights_input_hidden, weights_hidden_output, 
            hidden_bias, output_bias, prediction);
            char lettre[52]={'A','B','C','D','E','F','G','H','I','J','K',
            'L','M','N','O','P','Q','R','S','T','U','V','W','X'
            ,'Y','Z','a','b','c','d','e','f','g','h','i','j','q','l','m','n',
            'o','p','q','r','s','t','u','v','w','x','y','z'};
            
        
        
        
        int j=0;
        double max=prediction[0];
        for (int a = 0; a < OUTPUT_SIZE; a++)
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
        printf("La lettre %d est %c\n",i+1,lettre[j]);
        printf("\n\n");
        if (prediction[j]<0.65 || lettre[j] == 'O' ||
                (lettre[j] == 'E' && prediction[j] < 0.9) || 
                (lettre[j] == 'D' && prediction[j] < 0.85))
            r[i]='0';
        else
            r[i]=lettre[j];        
    }
    create_word(r,wor,taille1);

    free (r);

    return 0;










    



}

int pro_grid()
{
    load_hidden_bias("neuron_network/save_value_prime/hiddenLayerBias.txt");
    load_output_bias("neuron_network/save_value_prime/OutputLayerBias.txt");
    load_weight_hidden_output(
            "neuron_network/save_value_prime/weight_hidden_output.txt");
    load_weight_hidden_input(
            "neuron_network/save_value_prime/weight_hidden_input.txt");
    

    
    int* size=malloc(2*sizeof(int));
    search_size(size);
    printf("%d,%d\n",size[0],size[1]);
    size_t taille=size[1]*size[0];
    process_transforme_grid(size[0],size[1]);
    
    char* res[taille];
    int c=0;
    //int t=1;
    for (int i = 0; i < size[1]; i++)
    {
        for (int j = 0; j < size[0]; j++)
        {
            res[c]=malloc(100*sizeof(char));
           
            snprintf(res[c++], FILENAME_SIZE,
                    "neuron_network/other/output/%d.%d.png",i,j);
            //printf("%s\n",res[c-1]);
        }
    } 
    char* r=malloc(taille*sizeof(char));
    for (size_t i = 0; i < taille; i++)
    {
            double new_input[INPUT_SIZE];
            
            double* resultats = traitements(res[i]);
            for (size_t j = 0; j < INPUT_SIZE; j++) 
            {
                new_input[j] = resultats[j];
            }               
            double prediction[26];
            predict(new_input, weights_input_hidden, weights_hidden_output, 
            hidden_bias, output_bias, prediction);
            char lettre[52]={'A','B','C','D','E','F','G','H','I','J','K',
            'L','M','N','O','P','Q','R','S','T','U','V','W','X'
            ,'Y','Z','a','b','c','d','e','f','g','h','i','j','q','l','m',
            'n','o','p','q','r','s','t','u','v','w','x','y','z'};
        // Afficher les résultats de la prédiction
        
        int j=0;
        double max=prediction[0];
        for (int a = 0; a < OUTPUT_SIZE; a++)
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
        printf("La lettre %zu est %c\n",i+1,lettre[j]);
        printf("\n\n");
        
        if (prediction[j]<0.65 || lettre[j] == 'O' ||
                (lettre[j] == 'E' && prediction[j] < 0.9) || 
                (lettre[j] == 'D' && prediction[j] < 0.85))
            r[i]='0';
        else
            r[i]=lettre[j];       
    }
    create_grid(r,size[1],size[0]);
    free(r);
    return 0;
}
