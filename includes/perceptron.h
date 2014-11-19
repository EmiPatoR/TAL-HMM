#ifndef _PERCEPTRON_H
#define _PERCEPTRON_H

#include "hmm.h"
#include "structures.h"

int** PhyT(int k, categorie* C, int nbe);
int** PhyE(int k, categorie* C, phrase M, int nbe, int nbo);
int* PhyPi(categorie* C, int nbe);


void Perceptron(int I , corpus Corp,hmm *h);


void free_Mat(int** M,int nbe);
void free_PI(int* PI);

void addition_mat(int** M1, int** M2,int nbe, int c);
void inverse_mat(int** M,int nbe,int c);
void inverse_vect( int* V, int nbe );
void addition_vect( int* V1 , int* V2 , int nbe);


#endif
