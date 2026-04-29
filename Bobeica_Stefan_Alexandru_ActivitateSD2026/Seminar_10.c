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

struct NodArbore {
	Masina info;
	struct NodArbore* st;
	struct NodArbore* dr;
};
typedef struct NodArbore NodArbore;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;
	m1.id = -1;
	m1.model = NULL;
	m1.numeSofer = NULL;

	if (fgets(buffer, 100, file) == NULL) {
		return m1;
	}

	char* aux = strtok(buffer, sep);
	if (aux == NULL) {
		return m1;
	}

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
	if (masina.model != NULL) {
		printf("Model: %s\n", masina.model);
	}
	else {
		printf("Model: Lipsa\n");
	}
	if (masina.numeSofer != NULL) {
		printf("Nume sofer: %s\n", masina.numeSofer);
	}
	else {
		printf("Nume sofer: Lipsa\n");
	}
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->st, masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&(*radacina)->dr, masinaNoua);
		}
	}
	else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		*radacina = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	if (f) {
		while (1) {
			Masina m = citireMasinaDinFisier(f);
			if (m.id == -1) {
				break;
			}
			adaugaMasinaInArbore(&radacina, m);
		}
		fclose(f);
	}
	return radacina;
}

void afisareInOrdine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdine(radacina->dr);
	}
}

void afisareMasiniDinArborePreordine(NodArbore* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->st);
		afisareMasiniDinArborePreordine(radacina->dr);
	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&(*radacina)->st);
		dezalocareArboreDeMasini(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free((*radacina));
		*radacina = NULL;
	}
}

Masina getMasinaByID(NodArbore* radacina, int id) {
	if (radacina == NULL) {
		Masina m;
		m.id = -1;
		m.nrUsi = 0;
		m.pret = 0.0;
		m.model = NULL;
		m.numeSofer = NULL;
		m.serie = '-';
		return m;
	}
	else if (id < radacina->info.id) {
		return getMasinaByID(radacina->st, id);
	}
	else if (id > radacina->info.id) {
		return getMasinaByID(radacina->dr, id);
	}
	else {
		Masina m = radacina->info;
		m.model = malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);
		m.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(NodArbore* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	else {
		return 0;
	}
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	if (radacina) {
		int inaltimeStanga = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dr);
		if (inaltimeStanga > inaltimeDreapta) {
			return 1 + inaltimeStanga;
		}
		else {
			return 1 + inaltimeDreapta;
		}
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(NodArbore* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
	}
	else {
		return 0;
	}
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	if (radacina) {
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			return radacina->info.pret + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
		}
		else {
			return calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);
		}
	}
	return 0;
}

int main() {
	NodArbore* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInOrdine(radacina);
	afisareMasina(getMasinaByID(radacina, 0));
	printf("\nNumarul de noduri din graf este %d.\n", determinaNumarNoduri(radacina));
	printf("Inaltimea arborelui este %d.\n", calculeazaInaltimeArbore(radacina));
	printf("Pretul total al tuturor masinilor este %5.2f.\n", calculeazaPretTotal(radacina));
	printf("Pretul masinilor lui Ionescu este %5.2f.\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));
	dezalocareArboreDeMasini(&radacina);
	return 0;
}