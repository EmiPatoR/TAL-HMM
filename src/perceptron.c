#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #define _BSD_SOURCE
  #include <unistd.h>
#endif

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
			if(m.categories[k]->id == i && m.categories[k+1]->id == j){ /* ATENTION */
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
			/*
			__asm__ volatile (
			        "lock addl %1, %0\n" // add incr to operand
			        : "=m" (M1[i][j])
			        : "r" (M2[i][j]), "m" (M1[i][j])
			    );
			 */
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
		/*__asm__ volatile (
					        "lock addl %1, %0\n" // add incr to operand
					        : "=m" (V1[i])
					        : "r" (V2[i]), "m" (V1[i])
					    );
		 */
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
			/*Phrase de test*/
			test.categories = Viterbi(h,Corp->phrases[data->train_samples_id[i]],Categories);
			test.mots = Corp->phrases[data->train_samples_id[i]].mots;
			test.id = Corp->phrases[data->train_samples_id[i]].id;
			test.nb_mots = Corp->phrases[data->train_samples_id[i]].nb_mots;

			/*Pour PI*/
			V = PhyPi(Corp->phrases[data->train_samples_id[i]],h->nbe);
			addition_vect(h->PI,V,h->nbe);
			V = PhyPi(test,h->nbe);
			inverse_vect(V,h->nbe);
			addition_vect(h->PI,V,h->nbe);

			/*Pour T*/
			for (k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots)-1;k++){
				M = PhyT(k,Corp->phrases[data->train_samples_id[i]],h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
				M = PhyT(k,test,h->nbe);
				inverse_mat(M,h->nbe,h->nbe);
				addition_mat(h->T,M,h->nbe,h->nbe);
			}

			/*POUR E*/
			for(k=0;k<(Corp->phrases[data->train_samples_id[i]].nb_mots);k++){
				M = PhyE(k,Corp->phrases[data->train_samples_id[i]],h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
				M = PhyE(k,test,h->nbe,h->nbo);
				inverse_mat(M,h->nbe,h->nbo);
				addition_mat(h->E,M,h->nbe,h->nbo);
			}

			free(test.categories); /* On libere Cc */
			printf("phrase %i \n",i);
		}
		compteur++;
	}
	/*for(k=0;k<h->nbe;k++){
		h->PI[k] = log(h->PI[k]);
	}*/
}

static void *tache(void *p_data){
	data_thread *p = p_data;
	phrase test;
	int i,k;
	double **M = NULL;
	double *V = NULL;
	for(i=p->i_debut;i<p->i_fin;i++){
		/*Phrase de test*/
		pthread_mutex_lock (&p->sh->mut);
		test.categories = Viterbi(p->sh->h,p->Corp->phrases[p->data->train_samples_id[i]],p->Categories);
		pthread_mutex_unlock (&p->sh->mut);
		test.mots = p->Corp->phrases[p->data->train_samples_id[i]].mots;
		test.id = p->Corp->phrases[p->data->train_samples_id[i]].id;
		test.nb_mots = p->Corp->phrases[p->data->train_samples_id[i]].nb_mots;
		/*Pour PI*/
		V = PhyPi(p->Corp->phrases[p->data->train_samples_id[i]],p->sh->h->nbe);
		pthread_mutex_lock (&p->sh->mut);
		addition_vect(p->sh->h->PI,V,p->sh->h->nbe);
		pthread_mutex_unlock (&p->sh->mut);

		V = PhyPi(test,p->sh->h->nbe);
		inverse_vect(V,p->sh->h->nbe);
		pthread_mutex_lock (&p->sh->mut);
		addition_vect(p->sh->h->PI,V,p->sh->h->nbe);
		pthread_mutex_unlock (&p->sh->mut);

		/*Pour T*/
		for (k=0;k<(p->Corp->phrases[p->data->train_samples_id[i]].nb_mots)-1;k++){
			M = PhyT(k,p->Corp->phrases[p->data->train_samples_id[i]],p->sh->h->nbe);
			pthread_mutex_lock (&p->sh->mut);
			addition_mat(p->sh->h->T,M,p->sh->h->nbe,p->sh->h->nbe);
			pthread_mutex_unlock (&p->sh->mut);
			M = PhyT(k,test,p->sh->h->nbe);
			inverse_mat(M,p->sh->h->nbe,p->sh->h->nbe);
			pthread_mutex_lock (&p->sh->mut);
			addition_mat(p->sh->h->T,M,p->sh->h->nbe,p->sh->h->nbe);
			pthread_mutex_unlock (&p->sh->mut);


		}

		/*POUR E*/
		for(k=0;k<(p->Corp->phrases[p->data->train_samples_id[i]].nb_mots);k++){
			M = PhyE(k,p->Corp->phrases[p->data->train_samples_id[i]],p->sh->h->nbe,p->sh->h->nbo);
			pthread_mutex_lock (&p->sh->mut);
			addition_mat(p->sh->h->E,M,p->sh->h->nbe,p->sh->h->nbo);
			pthread_mutex_unlock (&p->sh->mut);
			M = PhyE(k,test,p->sh->h->nbe,p->sh->h->nbo);
			inverse_mat(M,p->sh->h->nbe,p->sh->h->nbo);
			pthread_mutex_lock (&p->sh->mut);
			addition_mat(p->sh->h->E,M,p->sh->h->nbe,p->sh->h->nbo);
			pthread_mutex_unlock (&p->sh->mut);


		}

		free(test.categories); /* On libere Cc */
	}
	return NULL;
}

static void *anim_loading(void *p_data){
	data_thread* p = p_data;
	printf("Apprentissage en cours ");
	while(!(*(p->termine))){
		advance_cursor();

		#ifdef _WIN32
				Sleep(100);
		#else
				usleep(100000);
		#endif

	}
	printf(" - Termine - \n");
	return NULL;
}

void Perceptron_multi_thread(int I , corpus *Corp,hmm *h, MlData *data, categorie *Categories){
	int compteur = 0;
	int i;
	int termine = 0;
	int taille_bloc[NB_CORE];
	pthread_t t[NB_CORE];
	pthread_t anim;
	data_thread data_t[NB_CORE];
	data_thread data_anim;

	shared sh;
	int nb_div = 0;
	int tmp = NB_CORE;
	int quantite = data->train_samples_count;

	pthread_mutexattr_t mta;
	pthread_mutexattr_init(&mta);
	sh.h = h;
	pthread_mutex_init(&sh.mut,&mta);

	/*Repartition des donnees:*/
	data_anim.Categories = Categories;
	data_anim.Corp = Corp;
	data_anim.MatE = NULL;
	data_anim.MatT = NULL;
	data_anim.VectPI = NULL;
	data_anim.data = data;
	data_anim.id_thread = NB_CORE;
	data_anim.sh = &sh;
	data_anim.termine = &termine;


	while(tmp%2 == 0){
		nb_div++;
		tmp = tmp / 2;
	}



	repart_data(quantite,1,taille_bloc,0,nb_div);

	for(i=0;i<NB_CORE;i++){
		data_t[i].Categories = Categories;
		data_t[i].Corp = Corp;
		data_t[i].MatE = NULL;
		data_t[i].MatT = NULL;
		data_t[i].VectPI = NULL;
		data_t[i].data = data;
		data_t[i].sh = &sh;
		data_t[i].i_debut = 0;
		data_t[i].i_fin = 0;
		data_t[i].id_thread = i;
		data_t[i].termine = &termine;
		if(i==0){
			data_t[i].i_debut = 0;
			data_t[i].i_fin = taille_bloc[i] - 1;
		}
		else{
			data_t[i].i_debut = data_t[i-1].i_fin+1;
			data_t[i].i_fin = data_t[i].i_debut + (taille_bloc[i] - 1);
		}
	}

	while(compteur <I){

		pthread_create (&anim, NULL, anim_loading, &data_anim);
		for(i=0;i<NB_CORE;i++){
			pthread_create (&t[i], NULL, tache, &data_t[i]);
		}

		for(i=0;i<NB_CORE;i++){
			pthread_join (t[i], NULL);
		}

		compteur++;
	}
	termine = 1;
	pthread_join(anim,NULL);
	pthread_mutex_destroy(&sh.mut);
}

void repart_data(int quantite,int nb_div,int *taille_bloc,int num,int nb_div_total){
	if(nb_div > nb_div_total){
		taille_bloc[num] = quantite;
	}
	else{
		if(quantite%2 == 1){
			repart_data(floor(quantite/2.0),nb_div+1,taille_bloc,num,nb_div_total);
			repart_data(ceil(quantite/2.0),nb_div+1,taille_bloc,num+pow(2,nb_div_total-nb_div),nb_div_total);
		}
		else{
			repart_data((quantite/2),nb_div+1,taille_bloc,num,nb_div_total);
			repart_data((quantite/2),nb_div+1,taille_bloc,num+pow(2,nb_div_total-nb_div),nb_div_total);
		}
	}
}
