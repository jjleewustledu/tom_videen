#include <string.h>
#include <stdlib.h>
#include <ecat/matrix.h>
#define MAX_FRAMES 100
#define ECAT_7 70

static usage() {
	fprintf(stderr,"usage: ecat6_to_7 input_file output_file\n");
	fprintf(stderr,"			or\n");
	fprintf(stderr,"usage: ecat6_to_7 input_file,frame output_file\n");
	exit(1);
}

static char frames[MAX_FRAMES];
main( argc, argv)
	int argc;
	char **argv;
{
	MatrixFile *mptr1, *mptr2;
	MatrixData *matrix;
	MatDirNode *node;
	Main_header mhead;
	char *in_file, *p;
	int i, num_frames, matnum, frame = 0;
	struct Matval mat;

	if (argc != 3) usage();
	in_file = strtok(argv[1],",");
	if ( (p = strtok(NULL,",")) != NULL) sscanf(p,"%d",&frame);
	mptr1 = matrix_open( in_file, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!mptr1) crash( "%s: can't open file '%s'\n", argv[0], in_file);
		fprintf(stderr,"file '%s' is of type %d with %d matrices\n", in_file,
		mptr1->mhptr->file_type, mptr1->dirlist->nmats);
		
	mhead = *mptr1->mhptr;
	mhead.sw_version = ECAT_7;
	mptr2 = matrix_create( argv[2], MAT_OPEN_EXISTING, &mhead);
	if (!mptr2) matrix_perror( argv[2]);
	node = mptr1->dirlist->first;
	while (node)
	{
		mat_numdoc( node->matnum, &mat);
		if (frame == 0 || mat.frame == frame) {
			matrix = matrix_read( mptr1, node->matnum, GENERIC);
 			matrix_write( mptr2, node->matnum, matrix);
			free_matrix_data(matrix);
		}
		node = node->next;
	}
	matrix_close( mptr1);

/* update main_header */
	node =  mptr2->dirlist->first;
	memset(frames,0,MAX_FRAMES);
	while (node)
	{
		mat_numdoc( node->matnum, &mat);
		frames[mat.frame] = 1;
		node = node->next;
	}
	num_frames = 0;
	for (i=0; i< MAX_FRAMES; i++) 
		if (frames[i]) num_frames++;
	mptr2->mhptr->num_frames = num_frames;
	mat_write_main_header (mptr2->fptr, mptr2->mhptr);
	matrix_close( mptr2);
}
