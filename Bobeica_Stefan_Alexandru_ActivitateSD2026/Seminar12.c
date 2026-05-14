#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
struct NodPrincipal {
	//	Masina masinaNoua;
	Masina info;

	NodPrincipal* next;
	NodPrincipal* vecini;
};

struct NodSecundar {
	NodSecundar* next;
	NodPrincipal* info;

};
//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareListaP(NodPrincipal** graf, Masina masinaNoua) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*graf) {
		NodPrincipal* aux = *graf;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}

}

void inserareListaS(NodSecundar** listaSecundara, NodPrincipal* info) {
	NodSecundar* nod = (NodSecundar*)malloc(sizeof(NodSecundar));
	nod->info = info;
	nod->next = NULL;
	if (*listaSecundara) {
		NodSecundar* aux = (*listaSecundara);
		while (aux->next) {
			aux = aux->next;

		}
		aux->next = nod;
	}
	else
		*listaSecundara = nod;
}
//3.
//functie de cautarea in lista principala dupa ID
void* cautaNodDupaID(NodPrincipal* listaPrincipala, int id) {
	while ((listaPrincipala) && (listaPrincipala->info.id != id)) {
		listaPrincipala = listaPrincipala->next;
	}
	return listaPrincipala;
}

//4.
//inserare muchie
void inserareMuchie(NodPrincipal* listaPrincipala, int idStart, int idStop) {
	if (listaPrincipala) {
		NodPrincipal* nodStart = cautaNodDupaID(listaPrincipala, idStart);
		NodPrincipal* nodSfarsit = cautaNodDupaID(listaPrincipala, idStop);
		if (nodStart && nodSfarsit) {
			inserareListaS(&(nodStart)->vecini, nodSfarsit);
			inserareListaS(&(nodSfarsit)->vecini, nodStart);

		}
	}
}


NodPrincipal* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* listaPrincipala = NULL;
	if (f) {
		while (!feof(f)) {
			inserareListaP(&(listaPrincipala), citireMasinaDinFisier(f));

		}
	}
	fclose(f);
	return listaPrincipala;
}

void citireMuchiiDinFisier(NodPrincipal* listaPrincipala, const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		int idStart;
		int idStop;
		fscanf(file, "%d %d", &idStart, &idStop);
		printf("%d %d ", idStart, idStop);
	}
	fclose(file);
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
	if (listaPrincipala == NULL || listaPrincipala == NULL)
		return;
	NodPrincipal* curentP = listaPrincipala;
	while (curentP != NULL) {
		NodSecundar* curentS = curentP->vecini;
		while (curentS != NULL) {
			NodSecundar* auxS = curentS;
			curentS = curentS->next;
			free(auxS);
		}
		free(curentP->info.model);
		free(curentP->info.numeSofer);
		NodPrincipal* auxP = curentP;
		curentP = curentP->next;
		free(auxP);
	}
	listaPrincipala = NULL;
}

void afisareLista(NodPrincipal* graf, int id) {
	NodPrincipal* nod = cautaNodDupaID(graf, id);
	NodSecundar* cap = nod->vecini;
	while (cap) {
		afisareMasina(cap->info->info);
		cap = cap->next;
	}
}
int main() {
	NodPrincipal* graf = NULL;
	graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	afisareLista(graf, 1);
	return 0;
}