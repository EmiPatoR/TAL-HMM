#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mldata.h"
#include "traintestsplit.h"

MlData* allocate_mldata(int nbFeatures, TrainTestSplit *tts){
	int i;
	int id = 0;
	MlData* data = NULL;
	data = (MlData*)malloc(sizeof(MlData));
	if(data == NULL){
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}
	data->samples_count = nbFeatures;
	data->train_samples_count = 0;
	data->test_samples_count = 0;
	data->test_samples_id = NULL;
	data->train_samples_id = NULL;

	/* Initialisation et allocation du tableau de l'ensemble d'entrainement et de test */
	if (tts->train_sample_part_mode == 0){
		data->train_samples_id = (int*) malloc(sizeof(int) * (tts->train_sample_part.count));
		data->train_samples_count = tts->train_sample_part.count;
		data->test_samples_id = (int*) malloc(sizeof(int) * ((data->samples_count) - (tts->train_sample_part.count)));
		data->test_samples_count = (data->samples_count - tts->train_sample_part.count);
	}
	else{
		data->train_samples_id = (int*) malloc(sizeof(int) * (ceil((tts->train_sample_part.portion * data->samples_count))));
		data->train_samples_count = ceil((tts->train_sample_part.portion * data->samples_count));
		data->test_samples_id = (int*) malloc(sizeof(int) * (floor(((1 - tts->train_sample_part.portion) * data->samples_count))));
		data->test_samples_count = floor(((1 - tts->train_sample_part.portion) * data->samples_count));
	}
	if(data->train_samples_id == NULL || data->test_samples_id == NULL){ /* Verification des alocations */
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}

	/*Radomization ou pas*/

	/*Cas 1 : pas de random (facile)*/
	for(i=0;i<data->train_samples_count;i++){
		data->train_samples_id[i] = i;
	}


	for(i=data->train_samples_count;i<data->samples_count;i++){
		data->test_samples_id[id] = i;
		id++;
	}

	printf("Echantillons(Entrainement/Test) correctement alloues en memoire.\n");
	return data;
}

void free_data(MlData* data){
	free(data->train_samples_id);
	free(data->test_samples_id);
	free(data);
	printf("Echantillons(Entrainement/Test) liberes en memoire.\n");
}
