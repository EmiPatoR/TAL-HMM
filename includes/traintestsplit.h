#ifndef __TRAINTESTSPLIT__
#define __TRAINTESTSPLIT__

typedef struct{

	union
	{
		int count;
		float portion;
	} train_sample_part;

	int train_sample_part_mode;
	int bool_mix;

}TrainTestSplit;

TrainTestSplit* allocate_traintestsplit(void);
void free_traintestsplit(TrainTestSplit* data);
void load_portion(TrainTestSplit* data, float tr_sample_portion, int bool);
void load_count(TrainTestSplit* data, int tr_sample_count, int bool);

#endif
