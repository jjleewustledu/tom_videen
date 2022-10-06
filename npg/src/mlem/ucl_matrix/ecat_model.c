#include "ecat_model.h"


static EcatModel _ecat_model_961 = {
 "961", 3, 42, 8, 1,  8, 4, 8, 7, 8192, 3072,
 4, 3, 0, 24, 11, 7, 17, 2, 30.9, 2, 336, 392, 1,
 15.00, 51.4, 41.2, 195.0, 650, 350, 0, 0.3125, 0.165, 1.16e-6, 1.04,
  0, 1.0, 0.0, -12.86, 0, -1, -1, -1, 5, 0, 1, 0.0, 1.2e-6, 1.02, 71, 79}; 

static EcatModel _ecat_model_962 = {
 "962", 1, 24, 3, 4, 12, 4, 8, 8, 8192, 5632,
 4, 1, 1, 32, 15, 9, 22, 2, 30.9, 2, 288, 288, 1,
 15.52, 58.3, 41.2, 236.2, 650, 350, 0, 0.2425, 0.225, 3.0e-07, 1.00,
 12, 1.0, 0.0,      0, 0, -1, -1, -1, 5, 0, 1, 0.0, 1.2e-6, 1.02, 71, 79};

  /*
   * This structure is identical to the structure returned by
   * db_getmodelinfo, except for the 6 last items. These have been
   * replaced by 5 items: BLOCKDEPTH, MINELEMENTS, BKTSINCOINCIDENCE,
   * CORMASK, ANALOGASICBUCKET.
   *
   * For the 962, we simply copied the six last values from the 961 record.
   */

EcatModel *ecat_model(system_type)
int system_type;
{
	switch(system_type) {
		case 961:
			return (&_ecat_model_961);
		case 911:
			return 0;
		case 962:
			return (&_ecat_model_962);
	}
	return 0;
}
/*
#include <stdio.h>
main() {
	EcatModel* model = ecat_model(961);
	printf("model number %s\n", model->number);
}
*/
