/* @(#)scanmath.c	1.1 4/26/91 */

#ifndef	lint
static char sccsid[]="@(#)scanmath.c	1.1 4/26/91 Copyright 1990 CTI Pet Systems, Inc.";
#endif	lint

/*
 * Feb-1996 :Updated by Sibomana@topo.ucl.ac.be for ECAT 7.0 support
 */
   

#include "matrix.h"
#include <math.h>

static usage(char *pgm) {
	fprintf(stderr,"usage: %s scana scanb scanc c1,c2,c3 op1,op2\n%s%s",pgm,
		 "\twhere op1={+,-,*,/} and op2={log,exp,sqr}\n",
		 "\tscanc=op2(c1+(c2*scana)op1(c3*scanb))\n");
	fprintf(stderr,"\tscanb may be a matrix_spec or a constant value\n");
	exit(1);
}

main( argc, argv)
  int argc;
  char **argv;
{
	MatrixFile *file1=NULL, *file2=NULL, *file3=NULL;
	MatrixData *scan1=NULL, *scan2=NULL, *scan3=NULL;
	MatrixData *slice1=NULL, *slice2=NULL;
	Scan3D_subheader *sh=NULL;
	Main_header mh;
	Attn_subheader ah;
	struct MatDir matdir, dir_entry;
	float *scana, *scanb, *scanc;
	float valb;
	short int *sdata;
	float *fdata;
	float maxval, minval, scalef;
	int i,j, matnum, blkno, nblks;
	char op1 = ' ', op2=' ',  *ptr, fname[256];
	float c1=0,c2=1,c3=1;
	int view, nviews, nprojs, nplanes, npixels;
	int offset1, offset2, offset3;

	if (argc<6) usage(argv[0]);

	if (!matspec( argv[1], fname, &matnum)) matnum = mat_numcod(1,1,1,0,0);
	file1 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!file1)
	  crash( "%s: can't open file '%s'\n", argv[0], fname);
	scan1 = matrix_read(file1,matnum,MAT_SUB_HEADER);
	if (!scan1) crash( "%s: scan '%s' not found\n", argv[0], argv[1]);
	if (file1->mhptr->file_type != Short3dSinogram &&
		file1->mhptr->file_type != AttenCor)
			crash("%s is not a Sinogram nor an Attenuation\n", argv[1]);
	matrix_find(file1, matnum, &matdir);
    offset1 = matdir.strtblk*MatBLKSIZE;

	if (matspec( argv[2], fname, &matnum)) matnum = mat_numcod(1,1,1,0,0);
	file2 = matrix_open( fname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!file2) {		/* check constant value argument */
	  if (sscanf(argv[2],"%g",&valb) != 1)
			crash( "%s: can't open file '%s'\n", argv[0], fname);
	}
	if (file2) {
		if (file2->mhptr->file_type != Short3dSinogram &&
			file2->mhptr->file_type != AttenCor)
				crash("%s is not a Sinogram nor an Attenuation\n", argv[1]);
		scan2 = matrix_read(file2,matnum,MAT_SUB_HEADER);
		if (!scan2) crash( "%s: scan '%s' not found\n", argv[0], argv[2]);
		if (scan1->xdim != scan2->xdim || scan1->ydim != scan2->ydim ||
			scan1->zdim != scan2->zdim)
			crash("%s and %s are not compatible\n",argv[1], argv[2]);
		matrix_find(file2, matnum, &matdir);
    	offset2 = matdir.strtblk*MatBLKSIZE;
	}

	if(file1->mhptr->file_type == AttenCor) {
		memcpy(&ah, scan1->shptr,sizeof(Attn_subheader));
		nviews = ah.num_angles;
		nprojs = ah.num_r_elements;
		nplanes = ah.z_elements[0];
	} else {
		sh = (Scan3D_subheader*)scan1->shptr;
		ah.num_dimensions = 3;
    	ah.x_resolution = sh->x_resolution;
    	ah.data_type = IeeeFloat;
    	ah.scale_factor = 1.0;
    	ah.num_z_elements = ah.z_elements[0] = sh->num_z_elements[0];
		ah.num_angles = nviews = sh->num_r_elements;
		ah.num_r_elements = nprojs = sh->num_r_elements;
		ah.z_elements[0] = nplanes = sh->num_z_elements[0];
    }
	npixels = nprojs*nplanes;

/* get scanc specification and write header */
	if (matspec( argv[3], fname, &matnum)) matnum = mat_numcod(1,1,1,0,0);
	memcpy(&mh, file1->mhptr, sizeof(Main_header));
	mh.file_type = AttenCor;
	mh.num_frames = 1;
	mh.num_planes = nplanes;
	file3 = matrix_create( fname, MAT_OPEN_EXISTING, &mh);
	if (!file3) crash( "%s: can't open file '%s'\n", argv[0], fname);
	scan3 = (MatrixData*)calloc(1, sizeof(MatrixData));
	memcpy(scan3, scan1, sizeof(MatrixData));
	nblks = (npixels*nviews*sizeof(float)+511)/512;
	if (matrix_find(file3, matnum, &matdir) == -1) {
        blkno = mat_enter(file3->fptr, file3->mhptr, matnum, nblks) ;
        dir_entry.matnum = matnum ;
        dir_entry.strtblk = blkno ;
        dir_entry.endblk = dir_entry.strtblk + nblks - 1 ;
        dir_entry.matstat = 1 ;
        insert_mdir(dir_entry, file3->dirlist) ;
        matdir = dir_entry ;
    } else {
        fprintf(stderr,"\7warning : existing matrix overwritten\n");
        blkno = matdir.strtblk;
    }
    offset3 = matdir.strtblk*MatBLKSIZE;
    mat_write_attn_subheader(file3->fptr, file3->mhptr, matdir.strtblk, &ah);

	scan3->shptr = (caddr_t)&ah;
	scan3->data_type = ah.data_type = IeeeFloat;
	scan3->data_size = nblks*512;
	scan3->scale_factor = ah.scale_factor  = 1;
	scanc = (float*)malloc(npixels*sizeof(float));
	scana = (float*)malloc(npixels*sizeof(float));
	scanb = (float*)malloc(npixels*sizeof(float));

/* Decode Operators and Coefficients */
	sscanf(argv[4], "%f,%f,%f", &c1, &c2, &c3);
	ptr = argv[5];
	op1 = *ptr++;
	if (*ptr == ',') ptr++;
	if (!strcmp( ptr, "log")) op2 = 'l';
	else if (!strcmp( ptr, "exp")) op2 = 'e';
		else if (!strcmp( ptr, "sqr")) op2 = 's';

/* compute and write data */
	if (!file2) {
		for (i=0; i<npixels; i++) scanb[i] = valb;
	}
	for (view=0; view<nviews; view++) {
		if (fseek(file1->fptr,offset1,view*npixels) != 0)
			crash ("%s view %d : fseek error\n",view+1,file1->fname);
		if (file1->mhptr->file_type == AttenCor) {
			if (fread(scana,sizeof(float),npixels,file1->fptr) != npixels)
            crash ("%s view %d : fread error\n",file1->fname, view+1);
			if (ntohs(1) != 1) {
				swab(scana,scanc,npixels*sizeof(float));
				swaw(scanc,scana,npixels*sizeof(float)/2);
			}
		} else {
			if (fread(scana,sizeof(short),npixels,file1->fptr) != npixels)
            	crash("%s view %d : fread error\n",file1->fname,view+1);
			if (ntohs(1) != 1) {
				swab(scana,scanc,npixels*sizeof(float));
				sdata = (short*)scanc;
			} else sdata = (short*)scana;
			for (i=0; i<npixels; i++)
				scana[i] = sh->scale_factor*sdata[i];
		}


		if (file2) {
        	if (fseek(file2->fptr,offset2,view*npixels) != 0)
            crash ("%s view %d : fseek error\n",file2->fname, view+1);
        	if (file2->mhptr->file_type == AttenCor) {
            	if (fread(scanb,sizeof(float),npixels,file2->fptr) != npixels)
            	crash("%s view %d : read error\n",file2->fname,view+1);
            	if (ntohs(1) != 1) {
                	swab(scanb,scanc,npixels*sizeof(float));
                	swaw(scanc,scanb,npixels*sizeof(float)/2);
            	}
        	} else {
            	if (fread(scanb,sizeof(short),npixels,file2->fptr) != npixels)
            	crash("%s view %d : read error\n",file2->fname, view+1);
            	if (ntohs(1) != 1) {
                	swab(scanb,scanc,npixels*sizeof(float));
                	sdata = (short*)scanc;
            	} else sdata = (short*)scanb;
            	for (i=0; i<npixels; i++)
                	scanb[i] = sh->scale_factor*sdata[i];
        	}
		}

		switch( op1)
		{
	  	case '+':
			for (i=0; i<npixels; i++)
			  scanc[i] = c1+c2*scana[i]+c3*scanb[i];
			break;
	  	case '-':
			for (i=0; i<npixels; i++)
			  scanc[i] = c1+c2*scana[i]-c3*scanb[i];
			break;
	  	case '*':
			for (i=0; i<npixels; i++)
			  scanc[i] = c1+c2*scana[i]*c3*scanb[i];
			break;
	  	case '/':
			for (i=0; i<npixels; i++)
			  scanc[i] = c1+(scanb[i]==0.0) ? 0.0 :
				c2*scana[i]/(c3*scanb[i]);
			break;
	  	default:
			crash("%s: illegal operator \"%c\"...chose from {+,-,*,/}\n",
			argv[0], op1);
		}
		switch (op2) {
		case 'l' :
	  		for (i=0; i<npixels; i++)
	    		scanc[i] = (scanc[i] < 0.0) ? 0.0 : log(scanc[i]);
			break;
		case 'e' :
			for (i=0; i<npixels; i++)
				scanc[i] = exp(scanc[i]);
			break;
		case 's' :
	  		for (i=0; i<npixels; i++)
	    		scanc[i] = (scanc[i] < 0.0) ? 0.0 : sqrt(scanc[i]);
			break;
		}
		if (ntohs(1) != 1) {
			swab(scanc,scana,npixels*sizeof(float));
			swaw(scana,scanc,npixels*sizeof(float)/2);
		}
		if (fseek(file3->fptr,offset3+view*npixels,0) != 0)
			crash ("%s view %d : positioning error\n",file3->fname,view+1);
		if (fwrite(scanc,sizeof(float),npixels,file3->fptr) != npixels)
			crash("%s view %d : error writing view\n",file3->fname,view+1);
		if (view == 0) minval = maxval = scana[0];
		for (i=0; i<npixels; i++) {
	  		if (scanc[i]>maxval) maxval = scanc[i];
	  		if (scanc[i]<minval) minval = scanc[i];
		}
	}
	ah.attenuation_min = minval;
	ah.attenuation_max = maxval;

/* update header and close all */
    mat_write_attn_subheader(file3, matdir.strtblk, &ah);
	matrix_close(file1);
	matrix_close(file2);
	matrix_close(file3);
}
