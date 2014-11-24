#include <stdio.h>
#include <stdlib.h>

#include "mldata.h"
#include "traintestsplit.h"
#include "structures.h"
#include "hmm.h"
#include "generatif.h"
#include "viterbi.h"
#include "perceptron.h"

int main()
{
	int nbMots = 0;
	int nbCategories = 0;
	TrainTestSplit* tts = NULL;
	MlData* data = NULL;
	mot* Mots = NULL;
	categorie* Categories = NULL;
	corpus* Corp = NULL;
	hmm *h = NULL;

	phrase *test = NULL;

	//Viterbi
	categorie** Cc = NULL;

	printf("-------------- Initialisations --------------------- \n");


	Mots = init_mots("data/vocab",&nbMots);
	Categories = init_categories("data/categ",&nbCategories);
	Corp = init_phrases("data/train/ftb.train.encode",Mots,Categories);

	tts = allocate_traintestsplit();
	//load_count(tts,100,0);
	load_portion(tts,0.9,0);
	data = allocate_mldata(Corp->nb_phrases,tts);

	h = allocate_hmm(nbCategories,nbMots);



		//Modele generatif
	//init_hmm_inf(h);
	//calc_PI(h,Corp,data);
	//calc_E(h,Corp,data);
	//calc_T(h,Corp,data);

		//Modele discriminant
	intialize_hmm(h);
	Perceptron(7,Corp,h,data,Categories);

	//h = load_hmm("percep_tr90");
	print_hmm(h,"percep_tr90_i7");

	Cc = Viterbi(h,Corp->phrases[data->test_samples_id[0]],Categories);
	test = malloc(sizeof(phrase));
	test->categories = Cc;
	test->nb_mots = Corp->phrases[data->test_samples_id[0]].nb_mots;
	test->id = Corp->phrases[data->test_samples_id[0]].id;

	printf("-------------- Donnees --------------------- \n");
	printf("Il y a %i mots dans le fichier vocabulaire. \n",nbMots);
	printf("Il y a %i categories dans le fichier categories. \n",nbCategories);
	printf("-------------- Corpus --------------------- \n");
	printf("Il y a %i mots dans le fichier corpus. \n",Corp->nb_mots);
	printf("Il y a %i phrases dans le corpus.\n",Corp->nb_phrases);
	printf("---------------Entrainement/Test-------------------- \n");
	printf("Le nombre d'exemples total disponible est : %i \n",data->samples_count);
	printf("La taille de l'ensemble d'entrainement est : %i \n",data->train_samples_count);
	printf("La taille de l'ensemble de test est : %i \n",data->test_samples_count);
	printf("-------------- Phrases test --------------------- \n");

	afficher_phrase(Corp->phrases[data->test_samples_id[0]]);
	afficher_categories_phrase(Corp->phrases[data->test_samples_id[0]]);
	afficher_categories_phrase(*test);

	printf("-------------- Liberation memoire --------------------- \n");

	free(test->categories);
	free(test);
	free_phrases(Corp);
	free_mots(Mots,nbMots);
	free_categories(Categories,nbCategories);
	free_traintestsplit(tts);
	free_data(data);
	free_hmm(h);
	return 0;
}
