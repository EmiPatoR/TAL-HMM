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


int** addition_mat(int** M1, int** M2,int nbe, int c){
	int i,j;
	int** res = NULL;
	res = (int**) malloc(sizeof(int*) * nbe);
	if(res == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		res[i] = (int*)malloc(sizeof(int)*c);
		if(res[i] == NULL){
			fprintf(stderr,"Erreur d'allocation.\n");
			exit(1);
		}
		for(j=0;j<c;j++){
			res[i][j] = M1[i][j] + M2[i][j];
		}
	}
	free_Mat(M1,nbe);
	free_Mat(M2,nbe);
	return res;
}

void inverse_mat(int** M,int nbe,int c){
	int i,j;
	for(i=0;i<nbe;i++){
		for(j=0;j<c;j++){
			M[i][j] = -M[i][j];
		}
	}
}

void Perceptron(int I , corpus Corp,hmm *h){
	/*
	int compteur = 0;
	categorie* Cc = NULL;
	categorie* C = NULL;

	int i,k;

	while(compteur <I){
		for(i=0;i<Corp.nb_phrases;i++){
			Cc = Viterbi(h,Corp.phrases[i]);
		}
		compteur++;
	}
	*/
}
