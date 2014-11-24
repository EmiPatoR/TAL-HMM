#ifndef _GENERATIF_
#define _GENERATIF_

#include "structures.h"
#include "hmm.h"
#include "mldata.h"

void calc_PI(hmm *h,corpus* Corp,MlData *data);
void calc_T(hmm *h,corpus* Corp,MlData *data);
void calc_E(hmm *h,corpus* Corp,MlData *data);
#endif
