#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodArbore {
    Masina info;
    struct NodArbore* left;
    struct NodArbore* right;
} NodArbore;

int maxim(int a, int b) {
    return (a > b) ? a : b;
}

// Calculeaza inaltimea unui nod
int calculeazaInaltimeArbore(NodArbore* root) {
    if (root == NULL) return 0;
    return 1 + max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right));
}

// Calculeaza gradul de dezechilibru
int verificaEchilibru(NodArbore* root) {
    if (root == NULL) return 0;
    return calculeazaInaltimeArbore(root->left) - calculeazaInaltimeArbore(root->right);
}

// Rotire la dreapta pentru echilibrare
void rotireDreapta(NodArbore** root) {
    NodArbore* aux = (*root)->left;
    (*root)->left = aux->right;
    aux->right = (*root);
    *root = aux;
}

// Rotire la stanga pentru echilibrare
void rotireStanga(NodArbore** root) {
    NodArbore* aux = (*root)->right;
    (*root)->right = aux->left;
    aux->left = (*root);
    *root = aux;
}

// Inserare cu auto-echilibrare (AVL)
void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
    if (*root == NULL) {
        NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
        if (nou) {
            nou->info = masinaNoua;
            nou->left = nou->right = NULL;
            *root = nou;
        }
        return;
    }

    if (masinaNoua.id < (*root)->info.id) {
        adaugaMasinaInArboreEchilibrat(&((*root)->left), masinaNoua);
    }
    else if (masinaNoua.id > (*root)->info.id) {
        adaugaMasinaInArboreEchilibrat(&((*root)->right), masinaNoua);
    }
    else {
        // ID-ul exista deja, eliberam memoria alocata la citire pentru a evita memory leaks
        free(masinaNoua.model);
        free(masinaNoua.numeSofer);
        return;
    }

    // Verificare si corectare echilibru
    int echilibru = verificaEchilibru(*root);

    // Cazul Stanga-Stanga
    if (echilibru > 1 && masinaNoua.id < (*root)->left->info.id) {
        rotireDreapta(root);
    }
    // Cazul Dreapta-Dreapta
    else if (echilibru < -1 && masinaNoua.id >(*root)->right->info.id) {
        rotireStanga(root);
    }
    // Cazul Stanga-Dreapta
    else if (echilibru > 1 && masinaNoua.id > (*root)->left->info.id) {
        rotireStanga(&((*root)->left));
        rotireDreapta(root);
    }
    // Cazul Dreapta-Stanga
    else if (echilibru < -1 && masinaNoua.id < (*root)->right->info.id) {
        rotireDreapta(&((*root)->right));
        rotireStanga(root);
    }
}

// Citire corecta dintr-un fisier CSV
Masina citireMasinaDinFisier(char* linie) {
    Masina m;
    char* token = strtok(linie, ",");
    m.id = atoi(token);

    token = strtok(NULL, ",");
    m.nrUsi = atoi(token);

    token = strtok(NULL, ",");
    m.pret = (float)atof(token);

    token = strtok(NULL, ",");
    m.model = (char*)malloc(strlen(token) + 1);
    strcpy(m.model, token);

    token = strtok(NULL, ",");
    m.numeSofer = (char*)malloc(strlen(token) + 1);
    strcpy(m.numeSofer, token);

    token = strtok(NULL, ",\n");
    m.serie = (unsigned char)token[0];

    return m;
}

void afisareMasina(Masina m) {
    printf("ID: %d | Model: %s | Sofer: %s | Pret: %.2f | Serie: %c\n",
        m.id, m.model, m.numeSofer, m.pret, m.serie);
}

void afisareInOrdine(NodArbore* root) {
    if (root) {
        afisareInOrdine(root->left);
        afisareMasina(root->info);
        afisareInOrdine(root->right);
    }
}

void dezalocare(NodArbore** root) {
    if (*root) {
        dezalocare(&((*root)->left));
        dezalocare(&((*root)->right));
        free((*root)->info.model);
        free((*root)->info.numeSofer);
        free(*root);
        *root = NULL;
    }
}

int main() {
    NodArbore* root = NULL;
    FILE* f = fopen("masini.txt", "r");

    if (f) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), f)) {
            // Elimina potentialul newline de la finalul liniei pentru curatare date
            buffer[strcspn(buffer, "\r\n")] = 0;
            if (strlen(buffer) > 5) { // Validare minima linie
                Masina m = citireMasinaDinFisier(buffer);
                adaugaMasinaInArboreEchilibrat(&root, m);
            }
        }
        fclose(f);
    }
    else {
        printf("Eroare la deschiderea fisierului.\n");
        return 1;
    }

    printf("Masini in arbore (sortate dupa ID):\n");
    afisareInOrdine(root);
    printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(root));

    dezalocare(&root);
    return 0;
}