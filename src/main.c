#include <stdio.h>
#include <stdlib.h>

#include "mldata.h"
#include "traintestsplit.h"
#include "structures.h"

int main()
{
	int nbMots = 0;

	//TrainTestSplit* tts = allocate_traintestsplit();
	//load_count(tts,50,0);

	//MlData* data = load_from_file("data/vocab",*tts);

	mot* Mots = NULL;
	Mots = init_mots("data/vocab",&nbMots);

	corpus* Corp = NULL;
	Corp = init_phrases("data/dev/ftb.dev.encode",Mots);

   // printf("La taille du fichier d'entrainement est : %i \n",tts->train_sample_part.count);
   // printf("La taille du fichier de test est : %i \n",data->samples_count - tts->train_sample_part.count);


    printf("Les mots ont ete correctements charges en memoire. \n");

    printf("Il y a %i mots dans le fichier vocabulaire. \n",nbMots);
    printf("Il y a %i mots dans le fichier corpus. \n",Corp->nb_mots);
    printf("Il y a %i phrases dans le corpus.\n",Corp->nb_phrases);

    afficher_phrase(Corp->phrases[56]);




    free_phrases(Corp);
    free_mots(Mots,nbMots);
    //free_traintestsplit(tts);
    //free_data(data);

    return 0;
}
