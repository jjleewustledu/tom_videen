#include "load_volume.h"

matrix_info(mh, matrix)
Main_header *mh;
MatrixData *matrix;
{
	struct Matval mat;
	int i, nvoxels;
	char *units;
	u_char *b_data;
	short *s_data;
	double total=0, mean=0;
	int data_unit = mh->calibration_units_label;
	float ecf = mh->calibration_factor;
	
	mat_numdoc(matrix->matnum, &mat);
	printf("%d,%d,%d,%d,%d\n",
		mat.frame, mat.plane, mat.gate, mat.data, mat.bed);
	nvoxels = matrix->xdim *matrix->ydim*matrix->zdim ;
	switch(matrix->data_type) {
		case ByteData:
			b_data = (u_char*)matrix->data_ptr;
			for (i=0; i<nvoxels; i++) total += *b_data++;
			break;
		default:		/* short */
			s_data = (short*)matrix->data_ptr;
			for (i=0; i<nvoxels; i++) total += *s_data++;
	}
	mean = total/nvoxels;
	printf("Dimensions := %dx%dx%d\n",matrix->xdim,matrix->ydim,matrix->zdim);
	if (mh->calibration_units == Uncalibrated || ecf <= 1.0 ||
		data_unit>numDisplayUnits)  data_unit = 0;
	if (!data_unit) {
		printf("Minimum := %g\n", matrix->data_min);
		printf("Maximum := %g\n", matrix->data_max);
		printf("Mean    := %g\n", mean*matrix->scale_factor);
		printf("Total   := %g\n", total*matrix->scale_factor);
	} else {
		if (data_unit == 1) { /* convert Ecat counts to Bq/ml */
			matrix->data_min *= ecf;
			matrix->data_max *= ecf;
			matrix->scale_factor *= ecf;
			data_unit = 2;
		}
		units = customDisplayUnits[data_unit];
		printf("Minimum := %g %s\n", matrix->data_min,units);
		printf("Maximum := %g %s\n", matrix->data_max,units);
		printf("Mean    := %g %s\n", mean*matrix->scale_factor,units);
		printf("Total   := %g %s\n", total*matrix->scale_factor,units);
	}		
}

main(argc, argv)
  int argc;
  char **argv;
{
	MatDirNode *node;
	MatrixFile *mptr;
	MatrixData *matrix;
	struct Matval mat;
	char fname[150];
	int i, ftype, frame = -1, matnum=0, cubic=0;

	if (argc < 2) crash("usage : %s matspec\n",argv[0]);
	matspec( argv[1], fname, &matnum);
	mptr = matrix_open(fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	ftype = mptr->mhptr->file_type;
	if (ftype <0 || ftype >= NumDataSetTypes)
		crash("%s : unkown file type\n",fname);
	printf( "%s file type  : %s\n", fname, datasettype[ftype]);
	if (!mptr) matrix_perror(fname);
	if (matnum) {
		matrix = matrix_read(mptr,matnum, UnknownMatDataType);
		if (!matrix) crash("%d,%d,%d,%d,%d not found\n",
			mat.frame, mat.plane, mat.gate, mat.data, mat.bed);
		matrix_info(mptr->mhptr,matrix);
	} else {
		 node = mptr->dirlist->first;
		while (node)
		{
			mat_numdoc(node->matnum, &mat);
			if (ftype == PetImage) {
				if (frame != mat.frame) {
					frame = mat.frame;
					matrix = load_volume(mptr,frame);
					matrix_info(matrix);
				}
			} else {
				matrix = matrix_read(mptr,node->matnum,UnknownMatDataType);
				if (!matrix) crash("%d,%d,%d,%d,%d not found\n",
            		mat.frame, mat.plane, mat.gate, mat.data, mat.bed);
					matrix_info(matrix);
			}
			node = node->next;
		}
	}
	matrix_close(mptr);
}
