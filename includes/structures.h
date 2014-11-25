#ifndef __STRUCTURES__
#define __STRUCTURES__

#include "mldata.h"
#include "hmm.h"
//typedef int categorie;

typedef struct{
	int id;
	char* nom;
} categorie;


typedef struct{
	int longueur;
	char* caractere;
	int inconnu;
	int id;
}mot;

typedef struct{
	int nb_mots;
	mot** mots;
	categorie** categories;
	int id;
} phrase;

typedef struct{
	int nb_phrases;
	int nb_mots;
	int id;
	phrase* phrases;
}corpus;

corpus* init_phrases(char* fichier_train, mot* Mots, categorie* Categories);
void afficher_phrase(phrase p);
void afficher_categories_phrase(phrase p);
void free_phrases(corpus* Corp);

mot* init_mots(char* fichier_vocab,int* nombreMots);
void free_mots(mot* Mots,int nombreMots);

void detect_mots_inconnus(corpus *Corp,MlData *data);
double eval_Corpus(corpus *Corp,MlData *data, hmm *h, categorie *Categories);
categorie* init_categories(char* fichier_vocab,int* nombreCategories);
void free_categories(categorie* Categories,int nombreCategories);

#endif
