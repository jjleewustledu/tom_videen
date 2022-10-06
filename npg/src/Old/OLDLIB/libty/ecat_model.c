#include "ecat_model.h"


static EcatModel _ecat_model_961 = {
	"961", 3, 42, 8, 1, 8, 4, 8, 7, 8192, 3072,
	4, 3, 0, 24, 11, 7, 17, 2, 30.9, 2, 336, 392, 1,
	15.0, 51.4, 41.2, 195.0, 650, 350, 0, 0.3125, 0.165, 1.16e-6, 1.04,
	0, 1.0, 0.0, 13, 0, -1, -1, -1, 5, 0, 1, 0.0, 1.2e-6, 1.02, 71, 79}; 


EcatModel *ecat_model(system_type)
int system_type;
{
	switch(system_type) {
		case 961:
			return (&_ecat_model_961);
		case 911:
			return 0;
		case 962:
			return 0;
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
