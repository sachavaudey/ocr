#include <stdio.h>
#include <stdlib.h>
//#include <string.h>


char (*lire_grille(char *nom_fichier, int *lignes, int *colonnes))[100] {
    FILE *fichier = fopen(nom_fichier, "r");
    

    static char grille[100][100];
    
    *lignes = 0;
    *colonnes = 0;
    char c;
    int col = 0;

    while ((c = fgetc(fichier)) != EOF) {
        if (c == '\n') {
            grille[*lignes][col] = '\0';  // end of line
            (*lignes)++;
            if (*colonnes == 0) {
                *colonnes = col;  
            }
            col = 0;
        } else {
            grille[*lignes][col++] = c;
        }
    }
    fclose(fichier);


    return grille;
}
int search_right(char** matrice, char* word ,int i,int j,int x)
    {
        int c=0;
        int q=j;
        for (int t = 0; t < x; t++)
        {
            if (matrice[i][q]==word[t]) q++;
            else break;
            c++;
        }
        if (c==x) return 1;
        else return 0;           
    }
int search_left(char** matrice, char* word ,int i,int j,int x)
{
    int c=0;
    int q=j;
    for (int t = 0; t < x; t++)
    {
        if (matrice[i][q]==word[t]) q--;
        else break;
        c++;
    }
    if (c==x) return 1;
    return 0;
}                    
int search_up(char** matrice, char* word ,int i,int j,int x)
{
int c=0;
                    int q=i;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][j]==word[t]) q--;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}
int search_down(char** matrice, char* word ,int i,int j,int x)
{
int c=0;
                    int q=i;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][j]==word[t]) q++;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}
int search_up_left(char** matrice, char* word ,int i,int j,int x)
{
    int c=0;
                    int q=i;
                    int l=j;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][l]==word[t]) q--,l--;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}
int search_up_right(char** matrice, char* word ,int i,int j,int x)
{
    int c=0;
                    int q=i;
                    int l=j;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][l]==word[t]) q--,l++;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}
int search_down_left(char** matrice, char* word ,int i,int j,int x)
{
int c=0;
                    int q=i;
                    int l=j;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][l]==word[t]) q++,l--;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}
int search_down_right(char** matrice, char* word ,int i,int j,int x)
{
    int c=0;
                    int q=i;
                    int l=j;
                    for (int t = 0; t < x; t++)
                    {
                        if (matrice[q][l]==word[t]) q++,l++;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}

void solver(char *nom_fichier,char **word) 
{
    int lignes, colonnes;
    char (*ma)[100] = lire_grille(nom_fichier, &lignes, &colonnes); 
    int x=0;
    while (word[x]!=0) 
    {
        if (word[x]>96 && word[x]<123) word[x]-=32;
        
        x++;
    }
    int n=1;
    while(ma[n-1][0]!=0) n++;
    // size of second dim
    int m=1;
    while(ma[0][m-1]!=0) m++;

    char** matrice=malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++)
    {
        matrice[i]=malloc(m*sizeof(char*));
        for (int j = 0; j < m; j++)
        {
            matrice[i][j]=ma[i][j];
            printf("%c",matrice[i][j]);

        }
        printf("\n");
        
    }
    printf("word=%s",word);
    //printf("n=%d, m=%d",n,m);
    for (int i = 0; i < n; i++)        // start of search
    {            
        
       for (int j = 0; j < m; j++)
        {
            
             if (matrice[i][j]==word[0] || matrice[i][j]+32==word[0])
            {        
                if (j-x+1>=0) //search_left
                {       
                    if (search_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i);
                        return;
                    }
                }
                if (j+x+1<=m) //search_right 
                {
                    if (search_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j+x-1,i);
                        return;
                    } 
                    
                }
                if (i-x+1>=0) //search_up
                {
                    
                    if (search_up(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j,i+1-x);
                        return;
                    }
                    
                    
                    
                }
                if (i+x+1<=n) //search_down 
                {
                    if (search_down(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j,i+x-1);
                        return;
                    }
                    
                }
                if (i-x+1>=0 && j-x+1>=0) //search_up_left
                {
                    if (search_up_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i-x+1);
                        return;
                    }
                    
                }
                if (i-x+1>=0 && j+x+1<=m) //search_up_right
                {
                    if (search_up_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j+x-1,i-x+1);
                        return;
                    }
                    
                }
                if (i+x+1<=n && j-x+1>=0) //search_down_left
                {
                    if (search_down_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i+x-1);
                        return;
                    }
                }
                if (i+x+1<=n && j+x+1<=m) //search_down_right
                {
                    if (search_down_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,-(j-x+1),-(i-x-1));
                        return;
                    }
                }
            }
        }
    }
    printf(" Not Found ");
    return;
}   


#define MAX_MOTS 4        
#define MAX_TAILLE_MOT 50
/*char** lire_mots(const char* nom_fichier, int nombre_mots) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    char** mots = malloc(nombre_mots * sizeof(char*));
    if (mots == NULL) {
        perror("Erreur d'allocation de mémoire pour le tableau");
        fclose(fichier);
        return NULL;
    }

    for (int i = 0; i < nombre_mots; i++) {
        mots[i] = malloc(MAX_TAILLE_MOT * sizeof(char));
        if (mots[i] == NULL) {
            perror("Erreur d'allocation de mémoire pour un mot");
            for (int j = 0; j < i; j++) {
                free(mots[j]);
            }
            free(mots);
            fclose(fichier);
            return NULL;
        }

        if (fscanf(fichier, "%49s", mots[i]) != 1) {  // Lecture d'un mot
            fprintf(stderr, "Erreur de lecture du mot %d\n", i + 1);
            for (int j = 0; j <= i; j++) {
                free(mots[j]);
            }
            free(mots);
            fclose(fichier);
            return NULL;
        }
    }

    fclose(fichier);
    return mots;
}*/
    




    
int process_solver(char* filename, char* word) {   
    solver(filename,word);
    return 0;
}




int main(int argc, char* argv[]){
    (void)argc;

    /*const char* nom_fichier = "word";

    // Lire les mots depuis le fichier
    char** mots = lire_mots(nom_fichier, MAX_MOTS);
    if (mots == NULL) {
        fprintf(stderr, "Erreur lors du chargement des mots\n");
        return EXIT_FAILURE;
    }

    // Afficher les mots chargés
    for (int i = 0; i < MAX_MOTS; i++) {
        printf("Mot %d: %s\n", i + 1, mots[i]);
        free(mots[i]); // Libérer chaque mot après utilisation
    }
    free(mots); // Libérer le tableau de pointeurs*/
    process_solver(argv[1], argv[2]);

    /*int taille = 3;
    int count = 0;

    char **tableau = malloc(taille * sizeof(char*));
    
    tableau[count] = strdup("grid");
    count++;

    
    //tableau[count] = strdup("JK");
    tableau[count]=malloc(2*sizeof(char));
    tableau[count]="Za";
    count++;
    tableau[count]=strdup("word");
    char* grid=malloc(4*sizeof(char));
    char* word=malloc(2*sizeof(char));
    printf("1=%s, 2=%s",tableau[0],tableau[1]);*/
    //process_solver(tableau[0],tableau[1]);
    //read_word(tableau[1]);

    //process_solver("grid","ae");
}