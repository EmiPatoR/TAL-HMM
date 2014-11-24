#ifndef __STRUCTURES__
#define __STRUCTURES__

//typedef int categorie;

typedef struct{
	int id;
	char* nom;
} categorie;


typedef struct{
	int longueur;
	char* caractere;
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

categorie* init_categories(char* fichier_vocab,int* nombreCategories);
void free_categories(categorie* Categories,int nombreCategories);

#endif
