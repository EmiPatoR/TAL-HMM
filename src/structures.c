#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

#define LIGNE_BUFFER 100


mot* init_mots(char* fichier_vocab, int* nombreMots){

	//Declarations

	FILE* fichier = NULL;

	char buffer[LIGNE_BUFFER];

	int nbMots = 0;

	int vu = 0;

	mot *res = NULL;

	int i,j;

	fichier = fopen(fichier_vocab,"r+");
	if ( fichier == NULL){
		fprintf(stderr,"erreur d'ouverture du fichier vocabulaire.");
		exit(1);
	}
	else{

		/* Recuperation du nombre de mots */
		while( fgets(buffer,LIGNE_BUFFER,fichier) != NULL){
			if(buffer[0] != '#') /* on ne compte pas les commentaires comme des mots */
				nbMots++;
		}

		*nombreMots = nbMots;

		//on alloue tous les mots
		res = (mot*)malloc(sizeof(mot) * nbMots);
		if(res == NULL){
			fprintf(stderr,"Erreur d'allocation memoire");
			fclose(fichier);
			exit(1);
		}

		fseek(fichier,0,SEEK_SET); // On revient au debut du fichier

		for(i=0;i<nbMots;i++){
			fgets(buffer,LIGNE_BUFFER,fichier);
			//Allocation du mot
			res[i].caractere = (char*)malloc(sizeof(char) * (strlen(buffer) - 1) );
			if(res[i].caractere == NULL){
				fprintf(stderr,"Erreur d'allocation");
				fclose(fichier);
				exit(1);
			}

			//copie du mot (sans le \n) :

			for(j=0;j<strlen(buffer);j++){
				if(buffer[j] == '\n')
					vu = 1;
				else
					res[i].caractere[j-vu] = buffer[j];
			}
			res[i].id = i;
			res[i].longueur = strlen(buffer) - 1;
			vu = 0;
		}

		fclose(fichier);
	}
	return res;
}

corpus* init_phrases(char* fichier_train, mot* Mots){

	//Declarations

	corpus* res = NULL;
	FILE* fichier = NULL;
	char buffer[LIGNE_BUFFER];
	int nbMotsCorpus = 0;
	int nbPhrases = 0;
	int numPhrase = 0;
	int numMot = 0;
	int idMot;
	int idCategorie;
	int termine = 0;

	fichier = fopen(fichier_train,"r+");
	if(fichier == NULL){
		fprintf(stderr,"Erreur lors de l'ouverture du fichier : %s.\n",fichier_train);
		exit(1);
	}
	else{

		/* Recuperation du nombre de phrases */
		while(!termine){
			if(fgets(buffer,LIGNE_BUFFER,fichier) == NULL){
				nbPhrases++;
				termine = 1;
			}
			else{
				if(buffer[0] == '\n' )
					nbPhrases++;
			}
		}

		fseek(fichier,0,SEEK_SET); // On revient au debut du fichier


		//Allocations

		//Allocation du corpus
		res = (corpus*)malloc(sizeof(corpus));
		if(res == NULL){
			fprintf(stderr,"Erreur d'allocation memoire.\n");
			exit(1);
		}

		res->nb_phrases = nbPhrases;
		res->id = 0;

		res->phrases = (phrase*)malloc(sizeof(phrase) * (res->nb_phrases));
		if(res->phrases == NULL){
			fprintf(stderr,"Erreur d'allocation memoire.\n");
			exit(1);
		}
		//________________________

		termine = 0;
		/* Recuperation du nombre de mots par phrase */
		while(!termine){
			if(fgets(buffer,LIGNE_BUFFER,fichier) == NULL){
				//Allocation des phrases
				res->phrases[numPhrase].id = numPhrase;
				res->phrases[numPhrase].nb_mots = numMot;
				res->phrases[numPhrase].categories = (categorie*)malloc(sizeof(categorie)*numMot);
				if(res->phrases[numPhrase].categories == NULL){
					fprintf(stderr,"Erreur d'allocation memoire.\n");
					exit(1);
				}
				res->phrases[numPhrase].mots = (mot**) malloc(sizeof(mot*)*numMot);
				if(res->phrases[numPhrase].mots == NULL){
					fprintf(stderr,"Erreur d'allocation memoire.\n");
					exit(1);
				}
				termine = 1;
			}
			else{

				if(buffer[0] != '\n'){
					numMot++;
				}
				else{
					//Allocation des phrases
					res->phrases[numPhrase].id = numPhrase;
					res->phrases[numPhrase].nb_mots = numMot;
					res->phrases[numPhrase].categories = (categorie*)malloc(sizeof(categorie)*numMot);
					if(res->phrases[numPhrase].categories == NULL){
						fprintf(stderr,"Erreur d'allocation memoire.\n");
						exit(1);
					}
					res->phrases[numPhrase].mots = (mot**) malloc(sizeof(mot*)*numMot);
					if(res->phrases[numPhrase].mots == NULL){
						fprintf(stderr,"Erreur d'allocation memoire.\n");
						exit(1);
					}
					numMot = 0;
					numPhrase++;
				}

			}

		}

		//Revient au debut du fichier
		fseek(fichier,0,SEEK_SET);
		numMot = 0;
		numPhrase = 0;
		termine = 0;
		//Affectation des valeurs de phrase
		while(!termine){
			if(fgets(buffer,LIGNE_BUFFER,fichier) == NULL){
				termine = 1;
			}
			else{
				if(buffer[0] != '\n'){
					sscanf(buffer,"%i %i",&idMot,&idCategorie);
					//fprintf(stderr,"[Debug] : numMot = %i/%i | numPhrase = %i/%i | Contenu : %i \n",numMot,res->phrases[numPhrase].nb_mots,numPhrase,res->nb_phrases,idCategorie);
					res->phrases[numPhrase].categories[numMot] = idCategorie;
					res->phrases[numPhrase].mots[numMot] = &Mots[idMot];
					numMot++;
					nbMotsCorpus++;
				}
				else{
					numMot = 0;
					numPhrase++;
				}
			}


		}

		res->nb_mots = nbMotsCorpus;

		fclose(fichier);
	}

	return res;
}

void afficher_phrase(phrase p){
	int i;
	printf("Phrase %i : ",p.id);
	for(i=0;i<p.nb_mots;i++){
		printf("%s ",p.mots[i]->caractere);
	}
	printf("\n");
}

void free_mots(mot* Mots,int nombreMots){
	int i;
	for(i=0;i<nombreMots;i++){
		free(Mots[i].caractere);
	}
	free(Mots);
	printf("Mots liberes en memoire.\n");
}

void free_phrases(corpus* Corp){
	int i;
	for(i=0;i<Corp->nb_phrases;i++){
		free(Corp->phrases[i].mots);
	}
	free(Corp->phrases);
	free(Corp);
	printf("Corpus libere en memoire.\n");
}