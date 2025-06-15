#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

// Fonction pour hacher le contenu d'un fichier et l'écrire dans un autre fichier
int hashFile(char* source, char* dest) {
    char command[512];  // Increase buffer size to prevent truncation
    snprintf(command, sizeof(command), "sha256sum \"%s\" > \"%s\"", source, dest);
    return system(command);
}

//Fonction pour renvoyer le hachage d'un fichier
char* sha256file(char* file) {
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[1000];
    strcpy(fname, template);
    int fd = mkstemp(fname);
    if (fd == -1) return NULL;

    close(fd);  // Fermeture du descripteur de fichier 

    char command[512];  // 
    snprintf(command, sizeof(command), "sha256sum \"%s\" > \"%s\"", file, fname);
    system(command);

    FILE* f = fopen(fname, "r");
    if (!f) return NULL;

    char* hash = malloc(65);
    if (!hash) {
        fclose(f);
        return NULL;
    }

    fscanf(f, "%64s", hash);
    fclose(f);
    unlink(fname);
    return hash;
}

// Initialisation d'une liste vide
typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

List* initList() {
    List* L = (List*)malloc(sizeof(List));
    *L = NULL;
    return L;
}

// Allocation et retour d'une cellule
Cell* buildCell(char* ch) {
    Cell* new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->data = strdup(ch);
    new_cell->next = NULL;
    return new_cell;
}

// Ajout d’un élément en tête d’une liste
void insertFirst(List *L, Cell* C) {
    C->next = *L;
    *L = C;
}

// Transformation d'une cellule et d'une liste en chaîne de caractères
char* ctos(Cell* c) {
    return c->data;
}

char* ltos(List* L) {
    int len = 0;
    for (Cell* temp = *L; temp != NULL; temp = temp->next) {
        len += strlen(temp->data) + 1;
    }
    char* result = (char*)malloc(len);
    result[0] = '\0';

    for (Cell* temp = *L; temp != NULL; temp = temp->next) {
        strcat(result, temp->data);
        if (temp->next != NULL) strcat(result, "|");
    }
    return result;
}

// Récupération du ième élément d’une liste
Cell* listGet(List* L, int i) {
    Cell* current = *L;
    int count = 0;
    while (current != NULL) {
        if (count == i) return current;
        count++;
        current = current->next;
    }
    return NULL;
}

// Recherche d’un élément dans une liste
Cell* searchList(List* L, char* str) {
    for (Cell* temp = *L; temp != NULL; temp = temp->next) {
        if (strcmp(temp->data, str) == 0) return temp;
    }
    return NULL;
}

// Transformation d'une chaîne de caractères en liste
List* stol(char* s) {
    List* L = initList();
    char* token = strtok(s, "|");
    while (token != NULL) {
        Cell* C = buildCell(token);
        insertFirst(L, C);
        token = strtok(NULL, "|");
    }
    return L;
}

// Écriture d'une liste dans un fichier et lecture d'une liste à partir d'un fichier
void ltof(List* L, char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) return;

    for (Cell* temp = *L; temp != NULL; temp = temp->next) {
        fprintf(file, "%s\n", temp->data);
    }
    fclose(file);
}

List* ftol(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) return NULL;

    List* L = initList();
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Supprimer le caractère de nouvelle ligne
        Cell* C = buildCell(buffer);
        insertFirst(L, C);
    }
    fclose(file);
    return L;
}

// Liste des fichiers et répertoires dans un répertoire donné
List* listdir(char* root_dir) {
    DIR* dp = opendir(root_dir);
    if (dp == NULL) return NULL;

    List* L = initList();
    struct dirent* ep;
    while ((ep = readdir(dp)) != NULL) {
        Cell* C = buildCell(ep->d_name);
        insertFirst(L, C);
    }
    closedir(dp);
    return L;
}

// Vérification de l'existence d'un fichier dans le répertoire courant
int file_exists(char* file) {
    List* files = listdir(".");
    Cell* found = searchList(files, file);
    return found != NULL ? 1 : 0;
}

// Copie du contenu d'un fichier vers un autre
void cp(char* to, char* from) {
    FILE* src = fopen(from, "r");
    if (src == NULL) return;

    FILE* dest = fopen(to, "w");
    if (dest == NULL) {
        fclose(src);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), src)) {
        fputs(buffer, dest);
    }

    fclose(src);
    fclose(dest);
}

// Conversion d'un hash en chemin de fichier
char* hashToPath(char* hash) {
    static char path[512];  
    snprintf(path, sizeof(path), "%c%c/%s", hash[0], hash[1], hash + 2);
    return path;
}

// Enregistrement d’un instantané du fichier donné en entrée
void blobFile(char* file) {
    char* hash = sha256file(file);
    if (hash == NULL) return;

    char* path = hashToPath(hash);
    char dir[256];
    snprintf(dir, sizeof(dir), "%c%c", hash[0], hash[1]);
    char command[512];  
    snprintf(command, sizeof(command), "mkdir -p %s", dir);
    system(command);

    cp(path, file);
}


int main() {
    // Exemple dusage de  blobFile
    char* file_to_hash = "exemple.txt";
    blobFile(file_to_hash);

}
