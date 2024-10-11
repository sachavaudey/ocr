#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

// Fonction pour obtenir l'extension du fichier
const char* get_file_extension(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot;
}

// Fonction pour renommer les fichiers dans un sous-dossier donné
void rename_images_in_folder(const char* folder, char letter) {
    DIR* dir;
    struct dirent* entry;
    int counter = 0;  // Initialisation du compteur à 0 pour chaque sous-dossier
    char old_path[1024];
    char new_path[1024];
    
    // Ouvrir le dossier
    if ((dir = opendir(folder)) == NULL) {
        perror("opendir() error");
        return;
    }

    // Lire les fichiers du dossier
    while ((entry = readdir(dir)) != NULL) {
        // Ignorer les dossiers "." et ".."
        if (entry->d_type == DT_REG) {
            // Construire le chemin complet du fichier actuel
            snprintf(old_path, sizeof(old_path), "%s/%s", folder, entry->d_name);
            
            // Récupérer l'extension du fichier
            const char* extension = get_file_extension(entry->d_name);
            
            // Créer le nouveau nom du fichier, par exemple "A0.png", "A1.png", ...
            snprintf(new_path, sizeof(new_path), "%s/%c%d%s", folder, letter, counter, extension);
            counter++;  // Incrémenter le compteur pour le fichier suivant
            
            // Renommer le fichier
            if (rename(old_path, new_path) != 0) {
                perror("rename() error");
            } else {
                printf("Renommé: %s -> %s\n", old_path, new_path);
            }
        }
    }
    
    // Fermer le dossier
    closedir(dir);
}

int main() {
    char base_folder[] = "images_test/dataset";  // Dossier racine contenant les sous-dossiers A, B, C, ...
    char subfolder[1024];
    
    // Parcourir les lettres de l'alphabet
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        // Créer le chemin vers chaque sous-dossier (e.g., dataset/A)
        snprintf(subfolder, sizeof(subfolder), "%s/%c", base_folder, letter);
        
        // Renommer les images dans ce sous-dossier
        rename_images_in_folder(subfolder, letter);
    }
    
    return 0;
}
