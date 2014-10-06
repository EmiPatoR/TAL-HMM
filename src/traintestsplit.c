#include <stdlib.h>
#include <stdio.h>
#include "traintestsplit.h"

TrainTestSplit* allocate_traintestsplit(void){
	TrainTestSplit* data = NULL;
	data = (TrainTestSplit*)malloc(sizeof(TrainTestSplit));
	if(data == NULL){
		fprintf(stderr, "erreur d'allocation\n");
		exit(1);
	}
	return data;
}

void free_traintestsplit(TrainTestSplit* data){
	free(data);
}

void load_portion(TrainTestSplit* data, float tr_sample_portion, int bool){
	data->train_sample_part.portion = tr_sample_portion;
	data->train_sample_part_mode = 1;
	data->bool_mix = bool;
}

void load_count(TrainTestSplit* data, int tr_sample_count, int bool){
	data->train_sample_part.count = tr_sample_count;
	data->train_sample_part_mode = 0;
	data->bool_mix = bool;
}
