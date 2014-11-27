#include "gramme.h"
#include "hmm.h"
#include "viterbi.h"
#include "structures.h"

void Gramme(corpus *Corp,hmm* h,MlData *data,categorie *Categories){
	int i,j,k,l,m;
	/*phrase test;
	double compteur2 = 0;*/


	int **num = NULL;
	int **occ = NULL;

	num = (int**)malloc(sizeof(int*)*(h->nbe*h->nbe));
	if(num == NULL){
		fprintf(stderr,"Erreur d'allocation.\n");
		exit(1);
	}
	for(i=0;i<(h->nbe*h->nbe);i++){
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
	for(i=0;i<(h->nbe);i++){
		occ[i] = (int*) malloc(sizeof(int)*h->nbe);
		if(occ[i] == NULL){
			fprintf(stderr,"Erreur d'allocation.\n");
			exit(1);
		}
	}

	/*Initialisation
	for (i=0;i<h->nbe;i++){
		for (j=0; j<h->nbe;j++){
			for (k=0;k<h->nbe;k++){
				h->T2[i*h->nbe+j][k]=0;
				h->T2[i*h->nbe+j][k]=0;
				h->T2[i*h->nbe+j][k]=0;
			}
		}
	}
	 */

	/*allimentation de h->T2*/
	/*
	for (i=0;i<data->train_samples_count;i++){
		test= Corp->phrases[data->train_samples_id[i]];
		for (j=2;j<Corp->phrases[data->train_samples_id[i]].nb_mots;j++){
			h->T2[test.categories[j-2]->id*(h->nbe) +test.categories[j-1]->id][test.categories[j]->id]++;
		}
	}

	for (i=0;i<(h->nbe*h->nbe);i++){
		for (j=0;j<h->nbe;j++){
			compteur2=compteur2 + h->T2[i][j];
		}
		for (j=0;j<h->nbe;j++){
			if (compteur2 !=0 && h->T2[i][j]!=0){
				h->T2[i][j] = (double)log(h->T2[i][j]/compteur2);
			}
			else{
				h->T2[i][j]= MINUS_INF;
			}
		}
		compteur2 = 0;
	}
	 */
	/*
	for(i=0;i<h->nbe;i++){
		for(j=0;j<data->train_samples_count;j++){
			for(k=1;k<Corp->phrases[data->train_samples_id[j]].nb_mots;k++){
				if(Corp->phrases[data->train_samples_id[j]].categories[k-1]->id == i){
					occ[i]++;
				}
			}
		}
	}
	 */
	for(i=0;i<h->nbe;i++){
		for(j=0;j<h->nbe;j++){
			for(k=0;k<data->train_samples_count;k++){
				for(l=2; l < (Corp->phrases[data->train_samples_id[k]].nb_mots) ;l++){
					if( (Corp->phrases[data->train_samples_id[k]].categories[l-1]->id == i) && (Corp->phrases[data->train_samples_id[k]].categories[l-2]->id == j)){
						occ[i][j]++;
					}
				}
			}
		}
	}

	for(i=0;i<h->nbe;i++){
		for(j=0;j<h->nbe;j++){
			for(m=0;m<h->nbe;m++){
				for(k=0;k<data->train_samples_count;k++){
					for(l=2; l < (Corp->phrases[data->train_samples_id[k]].nb_mots);l++){
						if( (Corp->phrases[data->train_samples_id[k]].categories[l]->id == i) && (Corp->phrases[data->train_samples_id[k]].categories[l-1]->id == j) && (Corp->phrases[data->train_samples_id[k]].categories[l-2]->id == m)){
							num[m*h->nbe+j][i]++;
						}
					}
				}
			}
		}
	}

	for(i=0;i<(h->nbe * h->nbe);i++){
		for(j=0;j<h->nbe;j++){
			if(num[i][j] == 0 || occ[i%h->nbe][i/h->nbe] == 0)
				h->T2[i][j] = MINUS_INF;
			else
				h->T2[i][j] = log((double)((double)num[i][j] / (double)occ[i%h->nbe][i/h->nbe]));
		}
	}

	/*
	for (i=0;i<(h->nbe*h->nbe);i++){
		for (j=0;j<h->nbe;j++){
			if(compteur2 != 0 && h->T2[i][j] == 0)
				h->T2[i][j]= log((double)(h->T2[i][j]/(double)compteur2));
			else
				h->T2[i][j] = 0;
		}
	}
	 */

	for(i=0;i<(h->nbe*h->nbe);i++){
		free(num[i]);
	}
	for(i=0;i<h->nbe;i++){
		free(occ[i]);
	}
	free(num);
	free(occ);
}

