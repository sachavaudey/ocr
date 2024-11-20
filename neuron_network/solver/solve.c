#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void solver(char *nom_fichier,char *word) 
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
            //printf("%c",matrice[i][j]);

        }
        //printf("\n");
        
    }
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
                        printf("(%d,%d),(%d,%d)",j,i,-(j-x+1),(i+x-1));
                        return;
                    }
                }
            }
        }
    }
    printf(" Not Found ");
    return;
    
    
    
    
}   
    
    
char **read_fichier(const char *nom_fichier, int *nombre_mots) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    // Initialisation des variables
    char **mots = NULL;
    char buffer[100];
    *nombre_mots = 0;

    // Lecture des mots et stockage dans le tableau dynamique
    while (fgets(buffer, 100, fichier)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Supprime le retour à la ligne

        
        char **temp = realloc(mots, (*nombre_mots + 1) * sizeof(char *));
        
        mots = temp;

        mots[*nombre_mots] = malloc(strlen(buffer) + 1);
        
        strcpy(mots[*nombre_mots], buffer);
        (*nombre_mots)++;
    }

    fclose(fichier);
    for (size_t i = 0; i < 4; i++)
    {
        printf("%s\n",mots[i]);
    }
    
    return mots;
}

int process_solver(char* filename, char* word) {   
    solver(filename,word);
    return 0;
}


int main(int argc, char* argv[]){
    (void)argc;
    char** grid=malloc(1*sizeof(char*));
    grid[0]=malloc(4*sizeof(char));
    sprintf(grid[0],"grid");
    
    

    int* n=malloc(sizeof(int));
    n[0]=4;
    
    char** word=read_fichier("word",n);


    


    for (size_t i = 0; i < 4; i++)
    {
        //printf("\n%s\n",word[i]);
        process_solver(grid[0],word[i]);
        printf("\n");
    }
    

    

    return 0;
    

}