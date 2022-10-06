/* @(#)extract_frame.c	2.0 8/9/93 */
/*
 * Updated by Sibomana@topo.ucl.ac.be for ECAT 7.0 support
 * CAUTION : Scan3D Subheader is stored in 2 blocks
 */

#include "matrix.h"
#define ERROR -1

static char sccsid[]="@(#)extract_frame.c	2.0 6/95";
static char prgnam[]="extract_frame_blk";

static usage() { crash( "usage: %s infile outfile frame,nframes bed,nbeds\n", prgnam);}

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *mp1, *mp2;
	MatrixData *mat=NULL, *mat2=NULL;
	int i, matnum, frame=1, nframes=1;
	int j, bed=0, nbeds=1;
	int blk, nblks, in_blk, out_blk;
	u_char buf[MatBLKSIZE];
	Main_header mh;
	struct MatDir matdir, dir_entry ;

	if (argc<4) usage();
	if (argc > 3) 
		if (sscanf( argv[3], "%d,%d", &frame, &nframes) < 1) usage();
	if (argc > 4) 
		if (sscanf( argv[4], "%d,%d", &bed, &nbeds) < 1) usage();
	mp1 = matrix_open( argv[1], MAT_READ_ONLY, Short3dSinogram);
	if (!mp1) matrix_perror( argv[0]);
	mh = *mp1->mhptr;
	mh.num_frames = nframes;
	mp2 = matrix_create( argv[2], MAT_CREATE, &mh);
	if (!mp2) matrix_perror( argv[0]);
	for (i=1; i<=nframes; i++, frame++) {
	for (j=1; j<=nbeds; j++, bed++) {
	  	matnum = mat_numcod( frame, 1, 1, 0, bed);
		if (matrix_find(mp1,matnum,&matdir) == ERROR) 
			crash("frame %d,1,1,0,%d not found\n",frame,bed);
		nblks = matdir.endblk - matdir.strtblk;
		in_blk = matdir.strtblk+2;
	  	mat = matrix_read( mp1, matnum, MAT_SUB_HEADER);
		if (matrix_find(mp2, matnum, &matdir) == ERROR) {
			blk = mat_enter(mp2->fptr, mp2->mhptr, matnum, nblks) ;
			dir_entry.matnum = matnum ;
			dir_entry.strtblk = blk ;
			dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
			dir_entry.matstat = 1 ;
			insert_mdir(dir_entry, mp2->dirlist) ;
			matdir = dir_entry;
		}
		mat_write_Scan3D_subheader(mp2->fptr, mp2->mhptr, matdir.strtblk,
			mat->shptr);
		out_blk = matdir.strtblk+2;
		if (fseek(mp2->fptr,(out_blk-1)*MatBLKSIZE, 0) == EOF) 
			crash("%s : fseek error\n",mp2->fname);
		if (!mp1->acs)
			if (fseek(mp1->fptr,(in_blk-1)*MatBLKSIZE, 0) == EOF)
				crash("%s : fseek error\n",mp1->fname);
		for (blk=2; blk<=nblks; blk++, in_blk++) {
			if (mp1->acs) rtsRblk(mp1->fname, in_blk, buf);
			else if (fread(buf,1,MatBLKSIZE,mp1->fptr) != MatBLKSIZE)
				crash("%s read error\n",mp1->fname);
			if (fwrite(buf,1,MatBLKSIZE,mp2->fptr) != MatBLKSIZE)
				crash("%s write error\n",mp2->fname);
		}
		matrix_free(mat);
	}
	}
	matrix_close(mp1);
	matrix_close(mp2);
	exit(0);
}
