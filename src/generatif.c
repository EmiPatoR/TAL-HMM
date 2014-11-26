#include <stdio.h>
#include <stdlib.h>

#include "hmm.h"
#include "generatif.h"
#include "structures.h"

void calc_PI(hmm *h,corpus* Corp,MlData *data){
	int i,j;
	int *num = NULL;
	num = malloc(sizeof(int)*h->nbe);
	if(num == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}

	for(i=0;i<h->nbe;i++){
		num[i] = 0;
	}

	for(i=0;i<h->nbe;i++){
		for(j=0;j<data->train_samples_count;j++){
			if(Corp->phrases[data->train_samples_id[j]].categories[0]->id == i){
				num[i]++;
			}
		}
		if(num[i] == 0)
			h->PI[i] = MINUS_INF;
		else
			h->PI[i] = log((double)(((double)num[i])/((double)data->train_samples_count)));
	}
	free(num);
}

void calc_E(hmm *h,corpus* Corp,MlData *data){
	int i,j,k;
	int **num = NULL;
	int *occ = NULL;

	num = (int**)malloc(sizeof(int*)*h->nbe);
	if(num == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}
	for(i=0;i<h->nbe;i++){
		num[i] = (int*) malloc(sizeof(int)*h->nbo);
		if(num[i] == NULL){
			fprintf(stderr,"Erreur d'allocation.\n");
			exit(1);
		}
	}

	occ = malloc(sizeof(int)*h->nbe);
	if(occ == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}

	/*Init*/
	for(i=0;i<h->nbe;i++){
		occ[i] = 0;
		for(j=0;j<h->nbo;j++){
			num[i][j] = 0;
		}
	}


	/*Calcul*/
	for(i=0;i<h->nbe;i++){
		for(j=0;j<data->train_samples_count;j++){
			for(k=0;k<Corp->phrases[data->train_samples_id[j]].nb_mots;k++){
				if(Corp->phrases[data->train_samples_id[j]].categories[k]->id == i){
					occ[i]++;
					num[i][Corp->phrases[data->train_samples_id[j]].mots[k]->id]++;
				}

			}
		}
	}

	/*remplir E*/
	for(i=0;i<h->nbe;i++){
		for(j=0;j<h->nbo;j++){
			if(num[i][j] == 0)
				h->E[i][j] = MINUS_INF;
			else
				h->E[i][j] = log((double)((double)num[i][j] / (double)occ[i]));
		}
	}

	/*liberation memoire*/

	for(i=0;i<h->nbe;i++){
		free(num[i]);
	}
	free(num);
	free(occ);
}


void calc_T(hmm *h,corpus* Corp,MlData *data){
	int i,j,k,l;
	int **num = NULL;
	int *occ = NULL;

	num = (int**)malloc(sizeof(int*)*h->nbe);
	if(num == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}
	for(i=0;i<h->nbe;i++){
		num[i] = (int*) malloc(sizeof(int)*h->nbe);
		if(num[i] == NULL){
			fprintf(stderr,"Erreur d'allocation.\n");
			exit(1);
		}
	}

	occ = malloc(sizeof(int)*h->nbe);
	if(occ == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}

	/*Init*/
	for(i=0;i<h->nbe;i++){
		occ[i] = 0;
		for(j=0;j<h->nbe;j++){
			num[i][j] = 0;
		}
	}
	/*Calcul*/
	for(i=0;i<h->nbe;i++){
		for(j=0;j<data->train_samples_count;j++){
			for(k=0;k<Corp->phrases[data->train_samples_id[j]].nb_mots;k++){
				if(Corp->phrases[data->train_samples_id[j]].categories[k]->id == i){
					occ[i]++;
				}
			}
		}
	}

	for(i=0;i<h->nbe;i++){
		for(j=0;j<h->nbe;j++){
			for(k=0;k<data->train_samples_count;k++){
				for(l=0; l < (Corp->phrases[data->train_samples_id[k]].nb_mots)-1 ;l++){
					if( (Corp->phrases[data->train_samples_id[k]].categories[l]->id == i) && (Corp->phrases[data->train_samples_id[k]].categories[l+1]->id == j)){
						num[i][j]++;
					}
				}
			}
		}
	}


	/*remplir T*/

	for(i=0;i<h->nbe;i++){
		for(j=0;j<h->nbe;j++){
			if(num[i][j] == 0)
				h->T[i][j] = MINUS_INF;
			else
				h->T[i][j] = log((double)((double)num[i][j] / (double)occ[i]));
		}
	}

	for(i=0;i<h->nbe;i++){
		free(num[i]);
	}
	free(num);
	free(occ);

}
