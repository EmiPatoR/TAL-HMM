#ifndef __MLDATA__
#define __MLDATA__

#include "traintestsplit.h"

#define LIGNE_BUFFER 100

#include <stdio.h>
#include <stdlib.h>

typedef struct{

	int samples_count;
	int train_samples_count;
	int test_samples_count;

	int *train_samples_id;
	int *test_samples_id;

}MlData;

MlData* allocate_mldata(int nbFeatures, TrainTestSplit *tts);
void free_data(MlData* data);

#endif
