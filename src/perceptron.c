#include <stdio.h>
#include <stdlib.h>

#include "perceptron.h"
#include "hmm.h"
#include "viterbi.h"

void free_Mat(double** M,int nbe){
	int i;
	for(i=0;i<nbe;i++){
		free(M[i]);
	}
	free(M);
}

void free_PI(double* PI){
	free(PI);
}

double** PhyT(int k, categorie* C, int nbe){
	int i,j;
	double** T = NULL;
	T = (double**)malloc(sizeof(double*) * nbe);
	if(T == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		T[i] = (double*) malloc(sizeof(double) * nbe);
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

double** PhyE(int k, categorie* C, phrase M, int nbe, int nbo){
	int i,j;
	double** E = NULL;
	E = (double**)malloc(sizeof(double*) * nbe);
	if(E == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		E[i] = (double*) malloc(sizeof(double) * nbo);
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

double* PhyPi(categorie* C, int nbe){
	int i;
	double* PI = NULL;
	PI = (double*)malloc(sizeof(double) * nbe);
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


void addition_mat(double** M1, double** M2,int nbe, int c){
	int i,j;
	for(i=0;i<nbe;i++){
		for(j=0;j<c;j++){
			M1[i][j] = M1[i][j] + M2[i][j];
		}
	}
	free_Mat(M2,nbe);
}

void inverse_mat(double** M,int nbe,int c){
	int i,j;
	for(i=0;i<nbe;i++){
		for(j=0;j<c;j++){
			M[i][j] = -M[i][j];
		}
	}
}
void inverse_vect( double* V, int nbe ){
	int i;
	for (i=0;i<nbe;i++){
	V[i]=-V[i];
	}
}

void addition_vect( double* V1 , double* V2 , int nbe){
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
	double **M = NULL;
	double *V = NULL;

	while(compteur <I){
		for(i=0;i<Corp.nb_phrases;i++){
			Cc = Viterbi(h,Corp.phrases[i]);
			C = Corp.phrases[i].categories;
			for (k=0;k<Corp.phrases[i].nb_mots;i++){
				M = PhyT(k,Cc,h->nbe);
				inverse_mat(M,h->nbe,h->nbe);
                addition_mat(h->T,M,h->nbe,h->nbe);

                addition_mat(h->T,PhyT(k,C,h->nbe),h->nbe,h->nbe);

                M = PhyE(k,Cc,Corp.phrases[i],h->nbe,h->nbo);
                inverse_mat(M,h->nbe,h->nbe);
                addition_mat(h->E,M,h->nbe,h->nbo);

                addition_mat(h->E,PhyE(k,C,Corp.phrases[i],h->nbe,h->nbo),h->nbe,h->nbo);
            }
            addition_vect(h->PI,PhyPi(C,h->nbe),h->nbe);
            V=PhyPi(Cc,h->nbe);
            inverse_vect(V,h->nbe);
            addition_vect(h->PI,V,h->nbe);
		}
		compteur++;
	}
}
