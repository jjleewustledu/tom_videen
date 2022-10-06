/* @(#)extract_frame.c	1.5 8/9/93 */

#include <ecat/matrix.h>

static char sccsid[]="@(#)extract_frame.c	1.5 8/9/93 -> corrected UCL 2/94";
static char prgnam[]="extract_frame";

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mp1, *mp2;
	MatrixData *mat=NULL, *mat2=NULL;
	int i, matnum, frame, nplanes, nframes, j, sframe, nv;
	short int *p1, *p2, maxval;
	float *sum=0;
	Main_header *mh;
	Scan_subheader *sh, *sh2;
	char matstr[256];


	if (argc<4)
	  crash( "%s\nusage: %s infile outfile frame,nplanes,nframes\n", sccsid, prgnam);
	mp1 = matrix_open( argv[1], MAT_READ_ONLY, MAT_SCAN_DATA);
	if (!mp1) matrix_perror( argv[0]);
	mh = matrix_init_main_header( argv[2], SCAN_DATA, mp1->mhptr);
	mh->num_frames=1;
	mp2 = matrix_create( argv[2], MAT_CREATE, mh);
	if (!mp2) matrix_perror( argv[0]);
	frame = 1;
	nplanes = 256;
	nframes = 1;
	if (argc > 3) sscanf( argv[3], "%d,%d,%d", &frame, &nplanes, &nframes);
	for (i=0; i<nplanes; i++)
	{
	  if (mat) matrix_free( mat);
	  matnum = mat_numcod( frame, i+1, 1, 0, 0);
	  mat = matrix_read( mp1, matnum, MAT_WORD);
	  if (mat && (nframes > 1))
	  {
	    p1 = (short int*) mat->data_ptr;
	    sh = (Scan_subheader*) mat->shptr;
	    nv = sh->dimension_1 * sh->dimension_2;
	    if (!sum) sum=(float*) malloc( nv*sizeof(float));
	    for (j=0; j<nv; j++)
		sum[j] = sh->scale_factor*p1[j]*sh->loss_correction_fctr;
	    for (sframe=1; sframe<nframes; sframe++)
	    {
		printf("."); fflush(stdout);
		if (mat2) matrix_free( mat2);
		matnum = mat_numcod( frame+sframe, i+1, 1, 0, 0);
		mat2 = matrix_read( mp1, matnum, MAT_WORD);
		if (!mat2) matrix_perror( argv[0]);
	        sh2 = (Scan_subheader*) mat2->shptr;
		p2 = (short int*) mat2->data_ptr;
		for (j=0; j<nv; j++)
		  sum[j] += (sh2->scale_factor*p2[j]*sh2->loss_correction_fctr);
		sh->frame_duration += sh2->frame_duration;
		sh->frame_duration_sec +=  sh2->frame_duration_sec;
	    }
	    sh->loss_correction_fctr =1.0;
	    convert_float_scan( mat, sum);
	  }
	  matnum = mat_numcod( 1, i+1, 1, 0, 0);
	  if (mat) matrix_write( mp2, matnum, mat);
	}
	printf("complete\n");
	matrix_close( mp1);
	matrix_close( mp2);
	exit(0);
}
