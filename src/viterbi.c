#include "structures.h"
#include "hmm.h"

categorie* Viterbi(hmm* h, phrase p){

	//déclaration de variables

	int i,j,k;
	int val_max = 0, i_max = 0;

	int valeur_actuelle = 0;

	double **T1;
	double **T2;

	categorie tmp;


	// Allocations

	//Alloc de la valeur de retour
	categorie* res = (categorie*)malloc(sizeof(categorie)*p.nb_mots); // On crée un tableau de la taille du nombre de mots dans la phrase
	if(res == NULL){
		fprintf(stderr,"Erreur d'allocation !");
		exit(1);
	}

	//Alloc de T1
	T1 = (double**)malloc(sizeof(double*)*h->nbe);
	if(T1 == NULL){
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}
	for(i=0;i<h->nbe;i++){
		T1[i] = (double*)malloc(sizeof(double) * (p.nb_mots));
		if(T1[i] == NULL){
			fprintf(stderr, "erreur d'allocation\n");
			exit(1);
		}
	}

	//Alloc de T2
	T2 = (double**)malloc(sizeof(double*)*h->nbe);
	if(T2 == NULL){
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}
	for( i=0;i<h->nbe;i++){
		T2[i] = (double*)malloc(sizeof(double) *(p.nb_mots));
		if(T2[i] == NULL){
			fprintf(stderr, "erreur d'allocation\n");
			exit(1);
		}
	}


	//Initialisations
	for(i=0;i<h->nbe;i++)
		res[i] = i;

	for(i=0;i<h->nbe;i++){ // Pour chaque etat
		T1[i][0] = h->PI[i] + h->E[i][p.mots[0]->id];
		T2[i][0] = 0.0;
	}

	// Calculs de T1 et T2
	for(i=1;i<p.nb_mots;i++){
		for(j=0;j<h->nbe;j++){
			for(k=0;k<h->nbe;k++){
				valeur_actuelle = T1[k][i-1] + h->T[k][j] + h->E[j][p.mots[i]->id];
				if(valeur_actuelle > val_max){
					val_max = valeur_actuelle;
					i_max = k;
				}
			}
			T1[j][i] = val_max;
			T2[j][i] = i_max;
		}
	}


	// Recuperation de la solution
	val_max = 0;
	for(i=0;i<h->nbe;i++){
		if(T1[i][p.nb_mots -1] > val_max){
			val_max = T1[i][p.nb_mots -1];
			tmp = i;
		}
	}

	res[p.nb_mots - 1] = tmp;
	for(i=p.nb_mots-1;i>0;i--){
		tmp = T2[tmp][i];
		res[i-1] = tmp;
	}

	return res;
}
