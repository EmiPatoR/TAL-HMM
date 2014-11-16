#include <stdio.h>
#include <stdlib.h>

#include "mldata.h"
#include "traintestsplit.h"

int main()
{
	TrainTestSplit* tts = allocate_traintestsplit();
	load_count(tts,50,0);

	MlData* data = load_from_file("data/vocab",*tts);

    printf("Hello world!\n");

    free_traintestsplit(tts);
    free_data(data);

    return 0;
}
