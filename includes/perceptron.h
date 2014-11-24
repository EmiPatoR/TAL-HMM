#ifndef _PERCEPTRON_H
#define _PERCEPTRON_H

#define NB_CORE 8

#include "hmm.h"
#include "structures.h"
#include "mldata.h"

double** PhyT(int k, phrase m, int nbe);
double** PhyE(int k, phrase m, int nbe, int nbo);
double* PhyPi(phrase m, int nbe);
void Perceptron(int I , corpus *Corp,hmm *h, MlData *data, categorie *Categories);
void Perceptron_multi_thread(int I , corpus *Corp,hmm *h, MlData *data, categorie *Categories);
void free_Mat(double** M,int nbe);
void free_PI(double* PI);
void addition_mat(double** M1, double** M2,int nbe, int c);
void inverse_mat(double** M,int nbe,int c);
void inverse_vect( double* V, int nbe );
void addition_vect( double* V1 , double* V2 , int nbe);

typedef struct{
	int i_debut;
	int i_fin;
	int id_thread;
	corpus *Corp;
	hmm *h;
	MlData *data;
	categorie *Categories;
}data_thread;

#endif
