#include <stdio.h>
#include <stdlib.h>

#include "perceptron.h"
#include "hmm.h"
#include "viterbi.h"

void free_Mat(int** M,int nbe){
	int i;
	for(i=0;i<nbe;i++){
		free(M[i]);
	}
	free(M);
}

void free_PI(int* PI){
	free(PI);
}

int** PhyT(int k, categorie* C, int nbe){
	int i,j;
	int** T = NULL;
	T = (int**)malloc(sizeof(int*) * nbe);
	if(T == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		T[i] = (int*) malloc(sizeof(int) * nbe);
		if(T[i] == NULL){
			fprintf(stderr,"Erreur d'allocation memoire.\n");
			exit(1);
		}
	}

	for(i=0;i<nbe;i++){
		for(j=0;j<nbe;j++){
			if(C[k] == i && C[k+1] == j){ // ATENTION
				T[i][j] = 1;
			}
			else{
				T[i][j] = 0;
			}
		}
	}
	return T;
}

int** PhyE(int k, categorie* C, phrase M, int nbe, int nbo){
	int i,j;
	int** E = NULL;
	E = (int**)malloc(sizeof(int*) * nbe);
	if(E == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		E[i] = (int*) malloc(sizeof(int) * nbo);
		if(E[i] == NULL){
			fprintf(stderr,"Erreur d'allocation memoire.\n");
			exit(1);
		}

	}

	for(i=0;i<nbe;i++){
		for(j=0;j<nbo;j++){
			if(C[k] == i && (M.mots[k]->id) == j){
				E[i][j] = 1;
			}
			else{
				E[i][j] = 0;
			}
		}
	}

	return E;
}

int* PhyPi(categorie* C, int nbe){
	int i;
	int* PI = NULL;
	PI = (int*)malloc(sizeof(int) * nbe);
	if(PI == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}

	for(i=0;i<nbe;i++){
		if(C[1] == i){
			PI[i] = 1;
		}
		else{
			PI[i] = 0;
		}
	}

	return PI;
}


void addition_mat(int** M1, int** M2,int nbe, int c){
	int i,j;
	for(i=0;i<nbe;i++){
		for(j=0;j<c;j++){
			M1[i][j] = M1[i][j] + M2[i][j];
		}
	}
	free_Mat(M2,nbe);
}

void inverse_mat(int** M,int nbe,int c){
	int i,j;
	for(i=0;i<nbe;i++){
		for(j=0;j<c;j++){
			M[i][j] = -M[i][j];
		}
	}
}
void inverse_vect( int* V, int nbe ){
	int i;
	for (i=0;i<nbe;i++){
	V[i]=-V[i];
	}
}

void addition_vect( int* V1 , int* V2 , int nbe){
	int i;
	for (i=0;i<nbe;i++){
	V1[i] = V1[i] + V2[i];
	}
free_PI(V2);
}

void Perceptron(int I , corpus Corp,hmm *h){
	int compteur = 0;
	categorie* Cc = NULL;
	categorie* C = NULL;

	int i,k;

	while(compteur <I){
		for(i=0;i<Corp.nb_phrases;i++){
			Cc = Viterbi(h,Corp.phrases[i]);
			C = Corp.phrases[i].categories;
			for (k=0;k<Corp.phrases[i].nb_mots;i++){
                addition_mat(h->T,inverse(PhyT(k,Cc,h->nbe),h->nbe,h->nbe),h->nbe,h->nbe);
                addition_mat(h->T,PhyT(k,C,h->nbe),h->nbe,h->nbe);
                addition_mat(h->E,inverse(PhyE(k,Cc,Corp.phrases[i],h->nbe,h->nbo),h->nbe,h->nbe),h->nbe,h->nbo);
                addition_mat(h->E,PhyE(k,C,Corp.phrases[i],h->nbe,h->nbo),h->nbe,h->nbo);
            }
            addition_vect(h->Pi,PhyPi(C,h->nbe));
            addition_vect(h->Pi,inverse_vect(PhyPi(Cc,h->nbe),h->nbe);
		}
		compteur++;
	}
}
