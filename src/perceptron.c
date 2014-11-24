#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "perceptron.h"
#include "hmm.h"
#include "viterbi.h"
#include "structures.h"


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
double** PhyT(int k, phrase m, int nbe){
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
			if(m.categories[k]->id == i && m.categories[k+1]->id == j){ // ATENTION
				T[i][j] = 1;
			}
			else{
				T[i][j] = 0;
			}
		}
	}
	return T;
}
double** PhyE(int k, phrase m, int nbe, int nbo){
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
			if(m.categories[k]->id == i && m.mots[k]->id == j){
				E[i][j] = 1;
			}
			else{
				E[i][j] = 0;
			}
		}
	}
	return E;
}
double* PhyPi(phrase m, int nbe){
	int i;
	double* PI = NULL;
	PI = (double*)malloc(sizeof(double) * nbe);
	if(PI == NULL){
		fprintf(stderr,"Erreur d'allocation memoire.\n");
		exit(1);
	}
	for(i=0;i<nbe;i++){
		if(m.categories[0]->id == i){
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

void Perceptron(int I , corpus *Corp,hmm *h, MlData *data, categorie *Categories){
	int compteur = 0;
	int i,k;
	double **M = NULL;
	double *V = NULL;
	phrase test;
	while(compteur <I){
		for(i=0;i<data->train_samples_count;i++){
			//printf("phrase = %i\n",i);
			//Phrase de test
			test.categories = Viterbi(h,Corp->phrases[data->train_samples_id[i]],Categories);
			//test.categories = Corp->phrases[data->train_samples_id[i]].categories;
			test.mots = Corp->phrases[data->train_samples_id[i]].mots;
			test.id = Corp->phrases[data->train_samples_id[i]].id;
			test.nb_mots = Corp->phrases[data->train_samples_id[i]].nb_mots;
			//fprintf(stderr,"[DEBUG] : ERREUR : i = %i \n",i);

			//Pour PI
			V = PhyPi(Corp->phrases[data->train_samples_id[i]],h->nbe);
			addition_vect(h->PI,V,h->nbe);
			V = PhyPi(test,h->nbe);
			inverse_vect(V,h->nbe);
			addition_vect(h->PI,V,h->nbe);

			//Pour T
			for (k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots)-1;k++){
				M = PhyT(k,Corp->phrases[data->train_samples_id[i]],h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
				M = PhyT(k,test,h->nbe);
				inverse_mat(M,h->nbe,h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
			}

			//POUR E
			for(k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots);k++){
				M = PhyE(k,Corp->phrases[data->train_samples_id[i]],h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
				M = PhyE(k,test,h->nbe,h->nbo);
				inverse_mat(M,h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
			}

			free(test.categories); // On libere Cc
			printf("phrase %i \n",i);
		}
		compteur++;
	}
	/*for(k=0;k<h->nbe;k++){
		h->PI[k] = log(h->PI[k]);
	}*/
}

void Perceptron_multi_thread(int I , corpus *Corp,hmm *h, MlData *data, categorie *Categories){
	int compteur = 0;
	int i,k;
	double **M = NULL;
	double *V = NULL;
	phrase test;

	pthread_t tache;

	//Repartition des donnees:


	while(compteur <I){
		for(i=0;i<data->train_samples_count;i++){
			//printf("phrase = %i\n",i);
			//Phrase de test
			test.categories = Viterbi(h,Corp->phrases[data->train_samples_id[i]],Categories);
			//test.categories = Corp->phrases[data->train_samples_id[i]].categories;
			test.mots = Corp->phrases[data->train_samples_id[i]].mots;
			test.id = Corp->phrases[data->train_samples_id[i]].id;
			test.nb_mots = Corp->phrases[data->train_samples_id[i]].nb_mots;
			//fprintf(stderr,"[DEBUG] : ERREUR : i = %i \n",i);

			//Pour PI
			V = PhyPi(Corp->phrases[data->train_samples_id[i]],h->nbe);
			addition_vect(h->PI,V,h->nbe);
			V = PhyPi(test,h->nbe);
			inverse_vect(V,h->nbe);
			addition_vect(h->PI,V,h->nbe);

			//Pour T
			for (k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots)-1;k++){
				M = PhyT(k,Corp->phrases[data->train_samples_id[i]],h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
				M = PhyT(k,test,h->nbe);
				inverse_mat(M,h->nbe,h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
			}

			//POUR E
			for(k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots);k++){
				M = PhyE(k,Corp->phrases[data->train_samples_id[i]],h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
				M = PhyE(k,test,h->nbe,h->nbo);
				inverse_mat(M,h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
			}

			free(test.categories); // On libere Cc
			printf("phrase %i \n",i);
		}
		compteur++;
	}
	/*for(k=0;k<h->nbe;k++){
		h->PI[k] = log(h->PI[k]);
	}*/
}
