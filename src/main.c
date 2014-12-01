#include <stdio.h>
#include <stdlib.h>

#include "mldata.h"
#include "traintestsplit.h"
#include "structures.h"
#include "hmm.h"
#include "generatif.h"
#include "viterbi.h"
#include "perceptron.h"
#include "gramme.h"

int main(int argc, char* argv[])
{
	int i;
	int nb_mots_test = 0;
	double eval;
	int *erreursCat = NULL;
	int nbMots = 0;
	int nbCategories = 0;
	TrainTestSplit* tts = NULL;
	MlData* data = NULL;
	mot* Mots = NULL;
	categorie* Categories = NULL;
	corpus* Corp = NULL;
	hmm *h = NULL;

	phrase *test = NULL;

	/*Viterbi*/
	categorie** Cc = NULL;

	printf("-------------- Initialisations --------------------- \n");


	Mots = init_mots("data/vocab",&nbMots);
	Categories = init_categories("data/categ",&nbCategories);
	Corp = init_phrases("data/train/ftb.train.encode",Mots,Categories);

	tts = allocate_traintestsplit();
	//load_count(tts,9780,0);
	load_portion(tts,0.8,0);
	data = allocate_mldata(Corp->nb_phrases,tts);

	h = allocate_hmm(nbCategories,nbMots);
	//h = allocate_hmm_trigramme(nbCategories,nbMots);

	printf("-------------- Detection mots inconnus --------------------- \n");
	detect_mots_inconnus(Corp,data);
	printf("Les mots inconnues ont ete marques.\n");
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
	printf("--------------- Apprentissage -------------------- \n");

	/*Modele generatif*/
		init_hmm_inf(h);
		/*h = load_hmm("percep_tr90_i1");*/
		calc_PI(h,Corp,data);
		calc_E(h,Corp,data);
		calc_T(h,Corp,data);
		//Gramme(Corp,h,data,Categories);
		/*print_hmm(h,"TestMulti");*/

	/*Modele discriminant*/

		/*Perceptron Normal*/
			/*intialize_hmm(h);*/
			/*h = load_hmm_perceptron("percep_multi_tr90_i20");*/
			/*h = load_hmm("percep_tr90_i1");*/
			/*Perceptron(1,Corp,h,data,Categories);*/
			/*print_hmm_perceptron(h,"percep_multi_tr90_i25");*/

		/*Perceptron Multi*/
			/*intialize_hmm(h);*/
			/*h = load_hmm_perceptron("percep_multi_tr90_i25");
			Perceptron_multi_thread(15,Corp,h,data,Categories);
			print_hmm_perceptron(h,"percep_multi_tr90_i40");*/

	printf("-------------- Phrase de test --------------------- \n");
	Cc = Viterbi(h,Corp->phrases[data->test_samples_id[0]],Categories);
	test = malloc(sizeof(phrase));
	test->categories = Cc;
	test->nb_mots = Corp->phrases[data->test_samples_id[0]].nb_mots;
	test->id = Corp->phrases[data->test_samples_id[0]].id;
	test->mots = Corp->phrases[data->test_samples_id[0]].mots;
	printf("Contenu de la phrase :\n");
	afficher_phrase(Corp->phrases[data->test_samples_id[0]]);
	printf("\n");
	printf("Categories correctes de la phrase :\n");
	afficher_categories_phrase(Corp->phrases[data->test_samples_id[0]]);
	printf("\n");
	printf("Categories predites de la phrase :\n");
	afficher_categories_phrase(*test);
	printf("\n");
	for(i=0;i<test->nb_mots;i++){
		printf(" %i " ,test->mots[i]->inconnu);
	}
	printf("\n");
	printf("-------------- Performances --------------------------- \n");
	erreursCat = (int*) calloc(h->nbe,sizeof(int));
	eval = eval_Corpus(Corp,data,h,Categories,erreursCat,&nb_mots_test);
	printf ("Performance : %f %% \n",(100-(eval*100)));
	printf ("Taux d'erreur : %f %% \n",eval*100);
	printf ("Nombre d'erreurs : %i sur %i mots.\n",(int)(eval*nb_mots_test),nb_mots_test);
	printf ("Details :\n");
	for(i=0;i<h->nbe;i++){
		printf("\t - Erreur sur %s : %i (%f %%)\n",Categories[i].nom,erreursCat[i],((double)erreursCat[i]/(double)nb_mots_test));
	}
	printf("\n");
	free(erreursCat);
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
