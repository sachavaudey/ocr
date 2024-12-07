#include "solve.h"

#include <gtk/gtk.h>
#include <stdio.h>
//trouver le nombre de mots


char (*lire_grille(int *lignes, int *colonnes))[100] {
    FILE *fichier = fopen("data/grid", "r");
    

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
            if (matrice[i][q]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q++;
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
        if (matrice[i][q]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q--;
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
                        if (matrice[q][j]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q--;
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
                        if (matrice[q][j]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q++;
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
                        if (matrice[q][l]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q--,l--;
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
                        if (matrice[q][l]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q--,l++;
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
                        if (matrice[q][l]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q++,l--;
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
                        if (matrice[q][l]==word[t] || matrice[q][j]=='0' ||word[t]=='0') q++,l++;
                        else break;
                        c++;
                    }
                    if (c==x) return 1;
                    return 0;
}



void solver(char *word) 
{   
    
    FILE *file = fopen("data/coordo", "a");
    //fprintf(file,"hello");
    if (file == NULL) {
    perror("Erreur d'ouverture du fichier COORD");
    g_print("lmlm\n");
    
      
}
    
    int lignes, colonnes;
    char (*ma)[100] = lire_grille(&lignes, &colonnes); 
    
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
    
    
    for (int i = 0; i < n; i++)        // start of search
    {            
        
       for (int j = 0; j < m; j++)
        {
            
             if (matrice[i][j]==word[0] || matrice[i][j]+32==word[0] || word[0]=='0')
            {        
                if (j-x+1>=0) //search_left
                {       
                    if (search_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j-x+1,i);
                        fclose(file);
                        return;
                    }
                }
                if (j+x+1<=m) //search_right 
                {
                    if (search_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j+x-1,i);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j+x-1,i);
                        fclose(file);
                        return;
                    } 
                    
                }
                if (i-x+1>=0) //search_up
                {
                    
                    if (search_up(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j,i+1-x);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j,i+1-x);
                        fclose(file);
                        return;
                    }
                    
                    
                    
                }
                if (i+x+1<=n) //search_down 
                {
                    if (search_down(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j,i+x-1);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j,i+x-1);
                        fclose(file);
                        return;
                    }
                    
                }
                if (i-x+1>=0 && j-x+1>=0) //search_up_left
                {
                    if (search_up_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i-x+1);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j-x+1,i-x+1);
                        fclose(file);
                        return;
                    }
                    
                }
                if (i-x+1>=0 && j+x+1<=m) //search_up_right
                {
                    if (search_up_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j+x-1,i-x+1);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j+x-1,i-x+1);
                        fclose(file);
                        return;
                    }
                    
                }
                if (i+x+1<=n && j-x+1>=0) //search_down_left
                {
                    if (search_down_left(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,j-x+1,i+x-1);
                        fprintf(file, "%d,%d %d,%d\n", j,i,j-x+1,i+x-1);
                        fclose(file);
                        return;
                    }
                }
                if (i+x+1<=n && j+x+1<=m) //search_down_right
                {
                    if (search_down_right(matrice, word ,i, j, x)==1)
                    {
                        printf("(%d,%d),(%d,%d)",j,i,(j+x-1),(i+x-1));
                        fprintf(file, "%d,%d %d,%d\n", j,i,(j+x-1),(i+x-1));
                        fclose(file);
                        return;
                    }
                }
            }
        }
    }
    printf(" Not Found ");
    
    fprintf(file, "%d,%d %d,%d\n", 0,0,0,0);
    
    fclose(file);
    
    return;
    
    
    
    
}   
    
    
char **read_fichier( int *nombre_mots) {
    FILE *fichier = fopen("data/word", "r");
    if (!fichier) {
        perror("Erreur d'ouverture duuu fichier");
        return NULL;
    }

    char **mots = NULL;
    char buffer[100];
    *nombre_mots = 0;

    while (fgets(buffer, 100, fichier)) {
        buffer[strcspn(buffer, "\n")] = '\0'; 

        
        char **temp = realloc(mots, (*nombre_mots + 1) * sizeof(char *));
        
        mots = temp;

        mots[*nombre_mots] = malloc(strlen(buffer) + 1);
        
        strcpy(mots[*nombre_mots], buffer);
        (*nombre_mots)++;
    }

    fclose(fichier);
    
    for (int i = 0; i < nombre_mots[0]; i++)
    {
        //printf("%s\n",mots[i]);
    }
    
    return mots;
}

int compterLignes() {
    
    FILE *fichier = fopen("data/word", "r");
    if (fichier==NULL) g_print("eerr\n");
    int nbLignes = 0;
    char caractere;
    while ((caractere = fgetc(fichier)) != EOF) {
        if (caractere == '\n') {
            nbLignes++;
        }
    }
    fclose(fichier);
    return nbLignes;
}

int process_solver( char* word) {   
    solver(word);
    return 0;
}


int pro_solv()
{
    
    
    //process_solver(argv[1],argv[2]);
    remove("data/coordo");
    char** grid=malloc(2*sizeof(char*));
    grid[0]=malloc(5*sizeof(char));
    sprintf(grid[0],"grid");
    
    

    int* n=malloc(sizeof(int));
    
    grid[1]=malloc(5*sizeof(char));
    sprintf(grid[1],"word");

    n[0]=compterLignes();
    

    char** word=read_fichier(n);


    
    
    

    for (int i = 0; i < n[0]; i++)
    {
        
        
        process_solver(word[i]);
        
        
    }
    
    free(grid[0]);
    free(grid[1]);
    free(grid);
    for (int i = 0; i < n[0]; i++)
    {
        free(word[i]);
    }
    free(word);
    
    free(n);
    

    

    return 0;
    

}
