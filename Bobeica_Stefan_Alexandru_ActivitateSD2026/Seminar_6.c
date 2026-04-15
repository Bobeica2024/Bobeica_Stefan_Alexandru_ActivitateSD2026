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

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* lista) {
	if (lista) {
		while (lista->next) {
			afisareMasina(lista->info);
			lista = lista->next;
		}
		afisareMasina(lista->info);
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* noulNod = malloc(sizeof(Nod));
	noulNod->info = masinaNoua;
	noulNod->next = NULL;
	Nod* temp;
	temp = *lista;
	if (*lista) {
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = noulNod;
	}
	else {
		*lista = noulNod;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 3) % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int hashCode = calculeazaHash(masina.id, hash.dim);
	if (!hash.tabela[hashCode]) {
		//Nu avem coliziune
		adaugaMasinaInLista(&hash.tabela[hashCode], masina);
	}
	else {
		// AVEM COLIZIUNE?
		adaugaMasinaInLista(&hash.tabela[hashCode], masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(3);
	if (file) {
		while (!feof(file)) {
			Masina masinaCitita = citireMasinaDinFisier(file);
			inserareMasinaInTabela(hash, masinaCitita);
		}
	}
	fclose(file);
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Clusterul %d:\n", i + 1);
		afisareListaMasini(ht.tabela[i]);
		printf("\n________________________________\n");
	}
}

void dezalocareListaMasini(Nod** cap) {
	Nod* p = *cap;
	while (p) {
		Nod* temp = p;
		p = p->next;
		if (temp->info.model != NULL) {
			free(temp->info.model);
		}
		if (temp->info.numeSofer != NULL) {
			free(temp->info.numeSofer);
		}
		free(temp);
	}
	*cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareListaMasini(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float calculeazaMedieLista(Nod* cap) {
	float suma = 0;
	int nrElemente = 0;
	while (cap) {
		suma += cap->info.pret;
		nrElemente++;
		cap = cap->next;
	}
	return (nrElemente > 0 ? (suma / nrElemente) : 0);
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = NULL;
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			(*nrClustere)++;
		}
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			preturi[contor] = calculeazaMedieLista(ht.tabela[i]);
			contor++;
		}
	}
	return preturi;
}

Masina getMasinaDinLista(Nod* cap, const char* nume) {
	Masina m;
	m.id = -1;
	while (cap && strcmp(cap->info.numeSofer, nume) != 0) {
		cap = cap->next;
	}
	if (cap) {
		m = cap->info;
		m.model = (char*)malloc(strlen(cap->info.model) + 1);
		strcpy_s(m.model, strlen(cap->info.model) + 1, cap->info.model);
		m.numeSofer = (char*)malloc(strlen(cap->info.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(cap->info.numeSofer) + 1, cap->info.numeSofer);
	}
	return m;
}

Masina getMasinaDupaID(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	int hashCode = calculeazaHash(id, ht.dim);
	Nod* cautare = ht.tabela[hashCode];
	while (cautare) {
		if (cautare->info.id == id) {
			m = cautare->info;
			m.numeSofer = (char*)malloc(sizeof(strlen(cautare->info.numeSofer) + 1));
			strcpy(m.numeSofer, cautare->info.numeSofer);
			m.model = (char*)malloc(sizeof(strlen(cautare->info.model) + 1));
			strcpy(m.model, cautare->info.model);
			return m;
		}
		cautare = cautare->next;
	}
	return m;
}

int main() {

	HashTable hash = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(hash);
	Masina test = getMasinaDupaID(hash, 8);
	afisareMasina(test);
	return 0;
}