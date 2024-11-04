#include <stdio.h>
#include "traitement_image.c" 
//#include "data.c"


#define nbInput 30
#define nbHiddenNode 16
#define output 4
#define nbTest 4

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}
void softmax(double* input, double* Output, size_t length) {
    double max = input[0];
    for (size_t i = 1; i < length; i++) {
        if (input[i] > max) max = input[i];  
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < length; i++) {
        Output[i] = exp(input[i] - max);  
        sum += Output[i];
    }

    for (size_t i = 0; i < length; i++) {
        Output[i] /= sum;
    }
}

// Prédiction
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

void load_weights(const char* filename, double hiddenWeight[nbInput][nbHiddenNode], 
                  double outPutWeight[nbHiddenNode][output], 
                  double hiddenLayerBias[nbHiddenNode], 
                  double outputLayerBias[output]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file for reading");
        return;
    }

    // Charger les poids de la couche cachée
    for (size_t i = 0; i < nbInput; i++) {
        for (size_t j = 0; j < nbHiddenNode; j++) {
            fscanf(file, "%lf", &hiddenWeight[i][j]);
        }
    }

    // Charger les poids de la couche de sortie
    for (size_t i = 0; i < nbHiddenNode; i++) {
        for (size_t j = 0; j < output; j++) {
            fscanf(file, "%lf", &outPutWeight[i][j]);
        }
    }

    // Charger les biais de la couche cachée
    for (size_t i = 0; i < nbHiddenNode; i++) {
        fscanf(file, "%lf", &hiddenLayerBias[i]);
    }

    // Charger les biais de la couche de sortie
    for (size_t i = 0; i < output; i++) {
        fscanf(file, "%lf", &outputLayerBias[i]);
    }

    fclose(file);
}


int main(void) {
    // Déclaration des tableaux de poids et biais
     double weights_input_hidden[nbInput][nbHiddenNode];
    double weights_hidden_output[nbHiddenNode][output];
    double hidden_bias[nbHiddenNode];
    double output_bias[output];

    // Charger les poids et les biais depuis un fichier
    load_weights("re", weights_input_hidden, weights_hidden_output, hidden_bias, output_bias);

    // Exemple de test d'entrée, à remplacer par votre propre logique d'entrée
    double new_input[nbInput];

    


    double* resultats = traitements_test("images_test/dataset/a/AAA.PNG");
        for (size_t j = 0; j < nbInput; j++) 
        {
            new_input[j] = resultats[j];
        }
        


                                                
                   
    double prediction[output];
    predict(new_input, weights_input_hidden, weights_hidden_output, hidden_bias, output_bias, prediction);
    char lettre[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    // Afficher les résultats de la prédiction
    for (int i = 0; i < output; i++) {
        printf("Prediction for class %c: %f\n", lettre[i], prediction[i]);
    }

    int u;
    double max=prediction[0];
    for (size_t i = 0; i < 26; i++)
    {
        if (max<prediction[i])
        {
            u=i;
            max=prediction[i];
        }
    }
    printf("La lettre est un %c\n",lettre[u]);

    

    return 0;
}



