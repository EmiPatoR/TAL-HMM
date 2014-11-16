#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mldata.h"
#include "traintestsplit.h"

MlData* load_from_file(const char* filename, TrainTestSplit tts){

	MlData* data = NULL;
	char buffer[LIGNE_BUFFER];
	int i;

	data = (MlData*)malloc(sizeof(MlData));
	if(data == NULL){
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}
	data->samples_count = 0;
	data->active_samples = 0;
	data->samples_id = NULL;
	data->test_samples_id = NULL;
	data->train_samples_id = NULL;

	FILE* fichier = NULL;
	fichier = fopen(filename,"r");
	if (fichier == NULL){
		fprintf(stderr, "Erreur d'ouverture du fichier d'apprentissage\n");
		exit(1);
	}

	/* Recuperation du nombre d'exemples */
	while( fgets(buffer,LIGNE_BUFFER,fichier) != NULL){
		if(buffer[0] != '#') /* on ne compte pas les commentaires comme des exemples */
			data->samples_count++;
	}
	data->active_samples = data->samples_count;


	/* Initialisation et allocation du tableau de tous les exemples */
	data->samples_id = (int*) malloc(sizeof(int) * data->samples_count);
	if(data->samples_id == NULL){ /* Verification de l'alocation */
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}

	/* Initialisation */
	for(i=0;i<data->samples_count;i++){
		data->samples_id[i] = i;
	}

	/* Initialisation et allocation du tableau de l'ensemble d'entrainement et de test */
	if (tts.train_sample_part_mode == 0){
		data->train_samples_id = (int*) malloc(sizeof(int) * tts.train_sample_part.count);
		data->test_samples_id = (int*) malloc(sizeof(int) * (data->samples_count - tts.train_sample_part.count));
	}
	else{
		data->train_samples_id = (int*) malloc(sizeof(int) * (tts.train_sample_part.portion * data->samples_count));
		data->test_samples_id = (int*) malloc(sizeof(int) * ((1 - tts.train_sample_part.portion) * data->samples_count));
	}

	if(data->train_samples_id == NULL || data->test_samples_id == NULL){ /* Verification des alocations */
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}

	printf("Il y a %i exemples dans le fichier.\n",data->samples_count);

	fclose(fichier);
	return data;
}

void free_data(MlData* data){
	free(data->samples_id);
	free(data->train_samples_id);
	free(data->test_samples_id);
	free(data);
}
