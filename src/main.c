#include <stdio.h>
#include <stdlib.h>

#include "mldata.h"
#include "traintestsplit.h"
#include "structures.h"

int main()
{
	int nbMots = 0;
	int nbPhrases = 0;

	TrainTestSplit* tts = allocate_traintestsplit();
	load_count(tts,50,0);

	MlData* data = load_from_file("data/vocab",*tts);

	mot* Mots = NULL;
	Mots = init_mots("data/vocab",&nbMots);

	phrase* p = NULL;
	p = init_phrases("data/dev/ftb.dev.encode",&nbPhrases);

    printf("La taille du fichier d'entrainement est : %i \n",tts->train_sample_part.count);
    printf("La taille du fichier de test est : %i \n",data->samples_count - tts->train_sample_part.count);


    printf("Les mots ont ete correctements charges en memoire. \n");
    printf("Il y a %i mots . \n",nbMots);
    printf("Mot 11502 = %s / id : %i / longueur : %i \n",Mots[18234].caractere,Mots[18234].id,Mots[18234].longueur);

    printf("Il y a %i phrases .\n",nbPhrases);


    free_mots(Mots,nbMots);
    printf("Mots liberes en memoire.\n");
    free_traintestsplit(tts);
    free_data(data);
    printf("Echantillons liberes en memoire.\n");

    return 0;
}
