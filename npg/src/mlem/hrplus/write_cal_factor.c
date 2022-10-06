#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

static usage(prog_name) 
char *prog_name;
{
  fprintf(stderr,"usage : %s -f matrix_file -e ecat_cal_factor\n", prog_name);
  exit(1);
}

main( argc, argv)
  int argc;
  char **argv;
{
  char* fname=NULL;
  float dosage  = -1.0;
  float weight  = -1.0;
  float ecf     = -1.0;
  int dose_time = -1;
  int c;

  MatrixFile *mptr;
  extern char *optarg;

  while ((c = getopt (argc, argv, "e:f:")) != EOF)
  {
    switch (c)
    {
      case 'f' :
        fname = optarg;
      break;

      case 'e' :
        sscanf(optarg,"%g",&ecf);
    }
  }
  if (fname==NULL) usage(argv[0]);
    mptr = matrix_open(fname, MAT_OPEN_EXISTING, MAT_UNKNOWN_FTYPE);
  if (!mptr)
    crash( "%s: can't open file '%s'\n", argv[0], fname);
  if (mptr->mhptr->file_type == 14)
  {
    mptr->mhptr->file_type = PetVolume;
    mptr->mhptr->bin_size = 0;
  }
  if (ecf       >= 0)
    mptr->mhptr->calibration_factor = ecf;
  mat_write_main_header(mptr->fptr,mptr->mhptr);
  matrix_close(mptr);
}








