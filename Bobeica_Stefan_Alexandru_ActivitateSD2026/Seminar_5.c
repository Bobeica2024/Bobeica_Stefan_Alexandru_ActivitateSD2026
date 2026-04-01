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
struct Nod {
	struct Nod* prev;
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;
struct ListaDubla {
	Nod* first;
	Nod* last;

};
typedef struct ListaDubla ListaDubla;
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

void afisareListaMasiniDeLaInceput(ListaDubla listaDubla) {
	Nod* aux = listaDubla.first;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->next;
	}

}
void afisareListaMasiniDeLaSfarsit(ListaDubla listaDubla) {
	Nod* aux = listaDubla.last;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}
void adaugaMasinaInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = listaDubla->last;
	if (listaDubla->last) {
		listaDubla->last->next = nodNou;
		listaDubla->last = nodNou;
	}
	else {
		listaDubla->first = nodNou;
	}
	listaDubla->last = nodNou;
	//listaDubla->last->next = nodNou;
}

void adaugaLaInceputInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->prev = NULL;
	nodNou->next = listaDubla->first;
	if (listaDubla->first) {
		listaDubla->first->prev = nodNou;
	}
	else {

		listaDubla->last = nodNou;
	}
	(*listaDubla).first = nodNou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	FILE* fisier = fopen(numeFisier, "r");
	Masina masina;
	ListaDubla listaMasini;
	listaMasini.first = NULL;

	while (!feof(fisier)) {
		masina = citireMasinaDinFisier(fisier);
		adaugaMasinaInLista(&listaMasini, masina);
	}
	fclose(fisier);
	return listaMasini;
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void dezalocareLDMasini(/*lista dubla de masini*/) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
}

float calculeazaPretMediu(/*lista de masini*/) {
	//calculeaza pretul mediu al masinilor din lista.
	return 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	ListaDubla listaMasini = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(listaMasini);

	return 0;
}