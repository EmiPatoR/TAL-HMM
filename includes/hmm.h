#ifndef __HMM__
#define __HMM__

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/*#include"matrices.h"*/
/*#include"somme_log.h"*/

#define LONG_LIGNE 100
#define MINUS_INF -999999999.0


typedef struct
{
  int nbe; /* nombre d'etats */
  int nbo; /* nombre d'observables */
  
  double **T; /* matrice de transition T[i][j] = P(j | i) */
              /* probabilite d'aller de l'etat j depuis l'etat i*/
  double **E; /* matrice d'emission E[i][j] = P(j | i)*/
	      /* probabilite d'emettre le symbole j depuis l'etat i*/
  double *PI; /* matrice des probabilites initiales PI[i] = P(X0 = i)*/
              /* probabilite que le hmm se trouve dans l'etat i au debut */ 
              /* de la generation */
} hmm;


hmm* allocate_hmm(int nbe, int nbo);

void intialize_hmm(hmm *h);
void init_hmm_inf(hmm *h);
void print_hmm(hmm *h, char *file_name);
void print_hmm_verbose(hmm *h, char *file_name);

hmm* load_hmm(char *file_name);
int* charge_observables(char *file_name, int T);
void free_hmm(hmm *h);

#endif

