#include <matrix.h>

#define span3_rd16 ((3*256)+16)
#define span5_rd17 ((5*256)+17)

Main_header mheader;

read_flip(fp,file_pos, data1, sx,sy,sz)				/*  read and flip y and z */
FILE *fp;
int file_pos;
short *data1;
int sx,sy,sz;
{
	int x,y,z,sx,sy,sz;
	short *plane0, *plane1, *line0, *line1;

       slice_blks = (sx*sy*sizeof(short)+511)/512;
       plane0 = (short*)malloc(512*slice_blks);
       for (z=0; z<sz; z++) {      /* for each planar view fixed theta */
           fseek(fp,file_pos,0);
           fread(plane0,sizeof(short),sx*sy,fp);
           file_data_to_host(plane0,slice_blks,scan3Dsub->data_type);
           file_pos += sx*sy*sizeof(short);
           for (y=0; y<sy; y++) {      /* for each line fixed plane */
               line0 =  plane0 + (sx*y);
               plane1 = data1 + (sx*sz*y);
               line1 = plane1 + (sx*z);
               memcpy(line1,line0,sx*sizeof(short));
           }
       }
       data->data_max = find_smax(data1,sx*sy*sz);
       free(plane0);
}

export_span3_rd16(mptr, matnum, mptr2)
MatrixFile *mptr1, *mptr2;
int matnum;
{
}

export_span5_rd17(mptr, matnum, mptr2)
MatrixFile *mptr1, *mptr2;
int matnum;
{
	strcut MatDir matdir;
	int file_pos, slice_blks, nblks, status,data_size ;
	Scan_subheader scansub ;
	Scan3D_subheader *scan3Dsub ;
	int x,y,z,sx,sy,sz;

	if (matrix_find(mptr,matnum,&matdir) == ERROR)
	{
       mat_errno = MAT_MATRIX_NOT_FOUND ;
       return(ERROR) ;
    }
	scan3Dsub = (Scan3D_subheader *) calloc(2, MatBLKSIZE) ;
	mat_read_Scan3D_subheader(mptr->fptr, mptr->mhptr, matdir.strtblk,
		(caddr_t)scan3Dsub) ;
	nblks =  matdir.endblk - matdir.strtblk ;
	data1 = (short*)calloc(nblks,512);
/* dimesnsions if storage_order = 0  i.e.
     (((projs x z_elements)) x num_angles) x Ringdiffs
*/
        sx = scan3Dsub->num_r_elements ;
        sz = scan3Dsub->num_angles ;
        sy = scan3Dsub->num_z_elements[0] ;
        scansub.scale_factor = scan3Dsub->scale_factor ;
        data->pixel_size = scan3Dsub->x_resolution;
        data->data_max = scan3Dsub->scan_max * scan3Dsub->scale_factor ;
        if (dtype == MAT_SUB_HEADER) break;
        data1 = (short*)data->data_ptr;
/* group 0 */
        file_pos = (matdir.strtblk+1)*MatBLKSIZE;
		read_flip(mptr->fptr,filepos,sx,sy,sz);
	

main( argc, argv)
  int argc;
  char **argv;
{
    int matnum;
    char fname[256];
    MatrixFile *mptr, *mptr2;
    MatrixData *matrix;
	Scan3D_subheader* 3d_sino;
	int storage;

    if (argc<3)
      crash( "usage    : %s input_matspec output\n", argv[0]);

    matspec( argv[1], fname, &matnum);
    mptr = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
   if (!mptr)
      crash( "%s    : can't open file '%s'\n", argv[0], fname);
	if ( mptr->mhptr->file_type != Short3dSinogram ) 
		crash ("%s : is not a 3D sinogram file\n",fname);
	if (matnum == 0) matnum = mat_numcod(1,1,1,0,0);

	mheader = *mptr1->mhptr;
	mptr2 = matrix_create(argv[2], MAT_OPEN_EXISTING, &mheader);
	if (!mptr2) matrix_perror(argv[2]);

	3d_sino = (Scan3D_subheader*) matrix->shptr;
	storage = (3d_sino->axial_compression*256)+3d_sino->ring_difference;
	switch (storage) {
		case span3_rd16:
			export_span3_rd16(mptr, matnum, mptr2);
			break;
		case span5_rd17:
			export_span5_rd17(mptr,matnum, mptr2)
			break;
		default:
			crash ("unsupported span, ring difference parameters\n");
	}
	matrix_close(mptr);
	matrix_close(mptr2);
