#ifndef _PERCEPTRON_H
#define _PERCEPTRON_H

#include "hmm.h"
#include "structures.h"

double** PhyT(int k, categorie* C, int nbe);
double** PhyE(int k, categorie* C, phrase M, int nbe, int nbo);
double* PhyPi(categorie* C, int nbe);


void Perceptron(int I , corpus Corp,hmm *h);


void free_Mat(double** M,int nbe);
void free_PI(double* PI);

void addition_mat(double** M1, double** M2,int nbe, int c);
void inverse_mat(double** M,int nbe,int c);
void inverse_vect( double* V, int nbe );
void addition_vect( double* V1 , double* V2 , int nbe);


#endif
