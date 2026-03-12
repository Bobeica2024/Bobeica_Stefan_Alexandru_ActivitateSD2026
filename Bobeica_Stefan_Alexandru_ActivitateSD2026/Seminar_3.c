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

void afisareMasina(Masina masina) {
	printf("\nID: %d", masina.id);
	printf("\nNumar usi: %d", masina.nrUsi);
	printf("\nPret: %5.2f", masina.pret);
	printf("\nModel: %s", masina.model);
	printf("\nNume sofer: %s", masina.numeSofer);
	printf("\nSerie: %c", masina.serie);
	printf("\n");


}

void afisareVectorMasini(Masina* masini, int nrMasini) {

	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++) {
		aux[i] = (*masini)[i];//shallow copy
	}
	aux[(*nrMasini)] = masinaNoua;

	free(*masini);
	*masini = aux;

	(*nrMasini)++;

}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char linie[50];
	fgets(linie, 50, file);
	char delimitator[3] = ",\n";
	m.id = atoi(strtok(linie, delimitator));
	m.nrUsi = atoi(strtok(NULL, delimitator));
	m.pret = atof(strtok(NULL, delimitator));

	char* model = strtok(NULL, delimitator);
	m.model = malloc((strlen(model) + 1) * sizeof(char));
	strcpy(m.model, model);

	char* numeSofer = strtok(NULL, delimitator);
	m.numeSofer = malloc((strlen(numeSofer) + 1) * sizeof(char));
	strcpy(m.numeSofer, numeSofer);

	char* serie = strtok(NULL, delimitator);
	m.serie = serie[0];

	return m;




}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	*nrMasiniCitite = 0;
	Masina* masini = NULL;
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini

}

int main() {

	int nrMasini = 0.;

	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);

}