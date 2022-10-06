#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

static usage(prog_name) 
char *prog_name;
{
	fprintf(stderr,"usage : %s -i matrix_file [-w weight] [-d dosage]\n",
		prog_name);
	exit(1);
}

main( argc, argv)
  int argc;
  char **argv;
{
	char* fname=NULL;
	float dosage = -1.0, weight = -1.0;
	int c;
	MatrixFile *mptr;
	extern char *optarg;

	 while ((c = getopt (argc, argv, "i:d:w:")) != EOF) {
		switch (c) {
		case 'i' :
			fname = optarg;
			break;
		case 'd' :
			sscanf(optarg,"%g",&dosage);
			break;
		case 'w' :
			sscanf(optarg,"%g",&weight);
		}
	}
	if (fname==NULL) usage(argv[0]);
	mptr = matrix_open(fname, MAT_OPEN_EXISTING, MAT_UNKNOWN_FTYPE);
	if (!mptr) crash( "%s: can't open file '%s'\n", argv[0], fname);
	if (mptr->mhptr->file_type == 14) {
		mptr->mhptr->file_type = PetVolume;
		mptr->mhptr->bin_size = 0;
	}
	if (dosage>=0) mptr->mhptr->dosage = dosage;
	if (weight>=0) mptr->mhptr->patient_weight = weight;
	mat_write_main_header(mptr->fptr,mptr->mhptr);
	matrix_close(mptr);
}
