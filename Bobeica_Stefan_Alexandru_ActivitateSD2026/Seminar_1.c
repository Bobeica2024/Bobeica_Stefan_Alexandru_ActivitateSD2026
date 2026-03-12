#include<stdio.h>
#include<stdlib.h>

struct Joc {
	int id;
	int nrJucatoriMin;
	char* denumire;
	float pret;
	char rating;
};

struct Joc initializare(int _id, int _nrJucatoriMin, char* _denumire, float _pret, char _rating) {
	struct Joc s;
	s.id = _id;
	s.nrJucatoriMin = _nrJucatoriMin;
	s.denumire = malloc(sizeof(char) * (strlen(_denumire) + 1));
	strcpy(s.denumire, _denumire);
	s.pret = _pret;
	s.rating = _rating;
	return s;
}

void afisare(struct Joc j) {
	printf("Id: %d\n", j.id);
	printf("Nume: %s\n", j.denumire);
	printf("Numar minim jucatori: %d\n", j.nrJucatoriMin);
	printf("Rating: %c\n", j.rating);
	printf("Pret: %5.2f\n", j.pret);
}

void modificaDenumire(struct Joc* s, char* _den) {
	free((*s).denumire);
	(*s).denumire = malloc((strlen((*s).denumire) + 1) * sizeof(char));
	strcpy((*s).denumire, _den);
}

float calcPretIndividual(struct Joc s) {
	float pret;
	pret = s.pret / s.nrJucatoriMin;
	return pret;
}

void dezalocare(struct Joc* s) {
	free((*s).denumire);
}

int main() {
	struct Joc s;
	s = initializare(1, 2, "Monopoly", 120, 'A');
	modificaDenumire(&s, "Secret Hitler");
	afisare(s);
	int pret;
	pret = calcPretIndividual(s);
	printf("%d", pret);
	int nrJocuri = 3;
	struct Joc* jocuri = malloc(sizeof(struct Joc) * nrJocuri);
	jocuri[0] = s;
	jocuri[1] = initializare(2, 2, "Catan", 60, 'A');
	jocuri[2] = initializare(2, 2, "Nu te supara frate", 50, 'B');
	afisare(*(jocuri + 1));

	dezalocare(&s);
	return 0;
}