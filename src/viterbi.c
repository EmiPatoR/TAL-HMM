#include "structures.h"
#include "hmm.h"

categorie** Viterbi(hmm* h, phrase p, categorie* Categories){

	/*déclaration de variables*/
	int i,j,k,l;
	int i_max = 0;

	double val_max = MINUS_INF;
	double valeur_actuelle = MINUS_INF;

	double **T1;
	double **T2;

	int tmp = 0;
	int tmp2 = 0;

	/* Allocations*/

	/*Alloc de la valeur de retour*/
	/* On crée un tableau de la taille du nombre de mots dans la phrase*/
	categorie** res = (categorie**)malloc(sizeof(categorie*)*p.nb_mots);
	if(res == NULL){
		fprintf(stderr,"Erreur d'allocation !");
		exit(1);
	}

	/*Alloc de T1*/
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

	/*Alloc de T2*/
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

	/*Initialisations*/
	for(i=0;i<p.nb_mots;i++)
		res[i] =  NULL;

	for(i=0;i<h->nbe;i++){ /* Pour chaque etat*/
		if(p.mots[0]->inconnu == 0)
			T1[i][0] = h->PI[i] + h->E[i][p.mots[0]->id];
		else
			T1[i][0] = h->PI[i];
		T2[i][0] = 0.0;
	}

	/* Calculs de T1 et T2*/
	for(i=1;i<p.nb_mots;i++){

		for(j=0;j<h->nbe;j++){
			if(h->trigramme == 0 || (h->trigramme == 1 && i < 2)){
				for(k=0;k<h->nbe;k++){
					if(p.mots[i]->inconnu == 0)
						valeur_actuelle = T1[k][i-1] + h->T[k][j] + h->E[j][p.mots[i]->id];
					else
						valeur_actuelle = T1[k][i-1] + h->T[k][j];
					if(valeur_actuelle >= val_max){
						val_max = valeur_actuelle;
						i_max = k;
					}
				}
			}
			else{
				/*
				for(k=0;k<h->nbe;k++){
					for(l=0;l<h->nbe;l++){
						if(p.mots[i]->inconnu == 0)
							valeur_actuelle = T1[l][i-2]+h->T[l][k]+T1[k][i-1]+ h->T2[l*h->nbe+k][j] + h->E[j][p.mots[i]->id];
						else
							valeur_actuelle = T1[l][i-2]+h->T[l][k]+T1[k][i-1]+ h->T2[l*h->nbe+k][j];
						if(valeur_actuelle >= val_max){
							val_max = valeur_actuelle;
							i_max = k+h->nbe*l;
						}
					}
				}
				 */
				for(k=0;k<h->nbe;k++){
					for(l=0;l<h->nbe;l++){
						if(p.mots[i]->inconnu == 0)
							valeur_actuelle = T1[k][i-1] + h->T[k][j] + h->E[j][p.mots[i]->id];
						else
							valeur_actuelle = T1[l][i-2]+h->T[l][k]+T1[k][i-1]+ h->T2[l*h->nbe+k][j];
						if(valeur_actuelle >= val_max){
							val_max = valeur_actuelle;
							if(p.mots[i]->inconnu)
								i_max = k+h->nbe*l;
							else
								i_max = k;
						}
					}
				}
			}
			T1[j][i] = val_max;
			T2[j][i] = i_max;
			val_max = MINUS_INF;
		}
	}

	/* Recuperation de la solution*/
	val_max = MINUS_INF;
	for(i=0;i<h->nbe;i++){
		if(T1[i][p.nb_mots -1] >= val_max){
			val_max = T1[i][p.nb_mots -1];
			tmp = i;
		}
	}


	res[p.nb_mots - 1] = &Categories[tmp];

	if(h->trigramme == 0){
		for(i=(p.nb_mots)-1;i>0;i--){
			tmp = T2[tmp][i];
			res[i-1] = &Categories[tmp];
		}
	}
	else{
		i = p.nb_mots -1;
		while(i>0){
			if(p.mots[i]->inconnu){
				if(i>1){
					tmp2 = ((int)(T2[tmp][i]))%(h->nbe);
					tmp = floor(T2[tmp][i]/h->nbe);
					res[i-1] = &Categories[tmp2];
					res[i-2] = &Categories[tmp];
					i = i - 2;
				}
				else{
					tmp = T2[tmp][i];
					res[i-1] = &Categories[tmp];
					i = i - 1;
				}
			}
			else{
				tmp = T2[tmp][i];
				res[i-1] = &Categories[tmp];
				i = i - 1;
			}

		}
		/*
		for(i=(p.nb_mots)-1;i>1;i=i-2){

			tmp2 = ((int)(T2[tmp][i]))%(h->nbe);
			tmp = floor(T2[tmp][i]/h->nbe);
			res[i-1] = &Categories[tmp2];
			res[i-2] = &Categories[tmp];
		}
		if(i == 1){
			tmp = T2[tmp][i];
			res[i-1] = &Categories[tmp];
		}
		 */
	}


	/*liberation de T1 et T2*/
	for(i=0;i<h->nbe;i++){
		free(T1[i]);
		free(T2[i]);
	}
	free(T1);
	free(T2);

	/*retour du resultat*/
	return res;
}
