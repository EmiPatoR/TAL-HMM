#ifndef __MLDATA__
#define __MLDATA__

#define LIGNE_BUFFER 100

#include <stdio.h>
#include <stdlib.h>

typedef struct{

	int samples_count;

	int *train_samples_id;
	int *test_samples_id;

}MlData;

MlData* load_from_file(const char* filename);

#endif
