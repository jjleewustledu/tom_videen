/* @(#)import_2dscan.c	1.4 21/6/94 */

/* import_2dscan.c

	Prepare 2D scan data for 2D reconstruction with  bkproj3d
	---------------------------------------------------------
	Since a 2D view should fit in a 256*47 matrix (2d fft limitation), trim should be applied
	to reduce the FOV from 336 to 256.
	When part of the head is outside the 42 cm FOV (.165*256), it should be recentered
	When the object is larger than the 42 cm FOV (cardiac studies), rebin should be applied (2 is enough)
	Rotation is introduced at reconstruction but the intrinsic tilt should be introduced
	to correct the x and y offsets.

	Correct scan scale factor with 1/rebin**2 for quantification

	please note that bkproj3d allows axial smoothing ...
	
	Version 1.4: correct decay_correction factor: exact formulae rather than Taylor expansion....
*/

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "matrix.h"
#include "isotope_info.h"
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>

#ifndef lint
	static char sccsid[]="@(#)import_2dscan.c	version 1.4  21/6/94 - UCL- Chr. Michel";
#endif

static char *progname;
static int verbose = 0;
 
MatrixData*  my_matrix_read( sinfp, scan_matnum, type)
MatrixFile* sinfp;
int scan_matnum, type; 
{
	static MatrixData* buffered_matrix = NULL;
	int y,z,sx,sy,sz;
	int i, size;
	struct Matval matval1, matval0;
	MatrixData* matrix;
	Scan_subheader *ssh;
	Scan3D_subheader *scan3Dsub;
	short *data, *p0, *p1;

	if (sinfp->mhptr->file_type == Sinogram)
		return matrix_read( sinfp, scan_matnum, type);

	mat_numdoc(scan_matnum,&matval1);
	if (buffered_matrix == NULL)  {
		buffered_matrix = matrix_read( sinfp, scan_matnum, type);
if (verbose) fprintf(stderr, "Scan Volume (%d x %d x %d) loaded\n", buffered_matrix->xdim,buffered_matrix->ydim,buffered_matrix->zdim);
	} else {
		mat_numdoc(buffered_matrix->matnum,&matval0);
		if (matval1.frame !=  matval0.frame) {
			free_matrix_data(buffered_matrix);
			buffered_matrix = matrix_read( sinfp, scan_matnum, type);
if (verbose) fprintf(stderr, "Scan Volume (%d x %d x %d) loaded\n", buffered_matrix->xdim,buffered_matrix->ydim,buffered_matrix->zdim);
		}
	}
	if (!buffered_matrix) crash ("matrix read error\n");

	y = matval1.plane-1;
if (verbose) fprintf(stderr,"extracting plane %d\n",matval1.plane);
/* make subheader */
	scan3Dsub = (Scan3D_subheader *) buffered_matrix->shptr ;
	matrix = (MatrixData*)calloc(1,sizeof(MatrixData));
	ssh = (Scan_subheader*)calloc(1,sizeof(Scan_subheader));
	matrix->shptr = (caddr_t)ssh;
	ssh->data_type = scan3Dsub->data_type;
    ssh->num_dimensions = 2;
    ssh->num_r_elements = scan3Dsub->num_r_elements;
    ssh->num_angles = scan3Dsub->num_angles;
    ssh->corrections_applied  = scan3Dsub->corrections_applied;
    ssh->num_z_elements = scan3Dsub->num_z_elements[0];
    ssh->ring_difference = scan3Dsub->ring_difference;
    ssh->x_resolution = scan3Dsub->x_resolution;
    ssh->y_resolution = scan3Dsub->v_resolution;
    ssh->z_resolution = scan3Dsub->z_resolution;
    ssh->w_resolution = scan3Dsub->w_resolution;
    ssh->gate_duration = scan3Dsub->gate_duration;
    ssh->r_wave_offset  = scan3Dsub->r_wave_offset;
    ssh->num_accepted_beats = scan3Dsub->num_accepted_beats;
    ssh->scale_factor = scan3Dsub->scale_factor;
    ssh->prompts = scan3Dsub->prompts;
    ssh->delayed = scan3Dsub->delayed;
    ssh->multiples = scan3Dsub->multiples;
    ssh->net_trues = scan3Dsub->net_trues;
    ssh->tot_avg_cor = scan3Dsub->tot_avg_cor;
    ssh->tot_avg_uncor = scan3Dsub->tot_avg_uncor;
    ssh->total_coin_rate = scan3Dsub->total_coin_rate;
    ssh->frame_start_time = scan3Dsub->frame_start_time ;
    ssh->frame_duration = scan3Dsub->frame_duration;
    ssh->loss_correction_fctr = scan3Dsub->loss_correction_fctr;

/* fill data */
	sx = buffered_matrix->xdim;
	sy = buffered_matrix->ydim;
	sz = buffered_matrix->zdim;
	data = (short*)buffered_matrix->data_ptr;
	matrix->xdim = sx;
	matrix->ydim = sz;
	matrix->data_size = sx*sz*sizeof(short);
	matrix->data_ptr = malloc(matrix->data_size);
	p1 = (short*)matrix->data_ptr;
	for (z=0; z<sz; z++) {
		p0 =  data+(sx*sy*z)+(sx*y);
		memcpy(p1,p0,sx*sizeof(short));
		p1 += sx;
	}
	matrix->data_type = ssh->data_type ;
	matrix->scale_factor = ssh->scale_factor ;
	matrix->zdim = 1;
	matrix->pixel_size = ssh->x_resolution;
    matrix->data_min = ssh->scan_min = 0;
	
/* compute maximun */
	size = matrix->data_size/2;
	data = (short*)matrix->data_ptr;
	ssh->scan_max = data[0];
	for (i=0; i < size; i++) {
		if (ssh->scan_max < data[i]) ssh->scan_max = data[i];
	}
	matrix->data_max = ssh->scan_max;
	return matrix;
}

main(argc, argv)
  int 		argc;
  char 		*argv[];
{
	short int	*scand;
	int		 	numProjs, numViews, numPlanes, numFrames, trim=336, mash=1, d_rebin=1, nv, np, i, j;
	int		 	deadtimeFlag=0, decayFlag=0, arcorFlag=0, normFlag=0, attnFlag=0, hhattnFlag=0;
	int		 	frame=1, plane=1;
	int		 	arg, scan_matnum, nrm_matnum=0, attn_matnum=0, hhattn_matnum=0;
	int			TotalNviews=0, input_mash=1;
	int		scanSize;
	int 		c;

	float		 *fdata=NULL, *ftmp=NULL, *fmash=NULL;
	float		 sf=1.0, decayFactor=1.0, branchFrac=1.0, xoff=0.0, yoff=0.0, tilt=0.0, binSize;
	float		 *normd=NULL, *hhattnd=NULL, *attnd=NULL;
	float		 halfLife, dtcor, frameStartTime;
	float		 radius, gprofile[512], w, frameDuration;
	double		ln2, exp(), log(), lamdaT;

	MatrixFile	 *sinfp=NULL, *soutfp=NULL, *nfp=NULL, *hhafp=NULL, *afp=NULL;
	MatrixData	 *scan=NULL , *norm=NULL, *hhattn=NULL, *attn=NULL; 

	Main_header	  mh;
	Scan_subheader  *ssh=NULL;
	Norm_subheader  *nsh=NULL;
	Attn_subheader  *ash=NULL;
	Attn_subheader  *hhash=NULL;
	char *scanin=NULL, *scanout=NULL;
	extern char *optarg;
	extern int optind, opterr;

	progname = argv[0];
	while ( (c=getopt( argc, argv, "i:o:vf:t:m:r:c:F:n:a:h")) != -1)
	  switch (c)
	  {
		case 'v':	/* -v: verbose */
				  verbose = 1;
			break;
		case 'i' :	/* -i: input scan file */
				scanin = optarg;
				if (verbose) fprintf (stdout, "input sinogram file %s\n", scanin);
			break;
		case 'o' :	/* -o: output scan file */
				scanout = optarg;
				if (verbose) fprintf (stdout, "output sinogram file %s\n", scanout);
			break;
		case 'f':	/* -f val: frame to extract */
			frame = atoi(optarg);
				if (verbose) fprintf (stdout, "Processing frame %d\n", frame);
			break;
		case 't':	/* -t val : trimming*/
			trim = atoi(optarg);
				if (verbose) fprintf (stdout, "Trimming output sinogram to %d\n", trim);
			break;
		case 'm':	/* -m val: additional mash  */
			mash = atoi(optarg);
				if (verbose) fprintf (stdout, "Mashing output sinogram by %d\n", mash);
			break;
		case 'r':	/* -r val: rebinning*/
			d_rebin = atoi(optarg);
				if (verbose) fprintf (stdout, "Rebinning output sinogram by %d\n", d_rebin);
			break;
		case 'c':	/* -c  val,val,val: x,y re-Centering */
			sscanf( optarg, "%f,%f,%f", &xoff, &yoff);
  			if (verbose) fprintf (stdout, "Recentering sinogram with x= %f y= %f \n", xoff, yoff);
			break;
		case 'F':	/* -F val,val,val: Correction flags */
			sscanf( optarg, "%d,%d,%d", &deadtimeFlag, &decayFlag, &arcorFlag);			
				if (verbose) fprintf (stdout, "Dead_Time_Flag %d Decay_Correction_Flag %d Arc_Correction %d\n", deadtimeFlag, decayFlag, arcorFlag);
			break;
		case 'n':	/* -n : apply normalization */
			 		nfp = matrix_open( optarg, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
 				  if (!nfp) crash( "..can't open normalization file\n");
				  normFlag = 1;
			break;
		case 'a':	/* -a : apply attenuation */
				  afp = matrix_open( optarg, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
				  if (!afp) crash( "..can't open attenuation file\n");
				  attnFlag = 1;
			break;
		case 'h':	/* -h : apply hh attenuation */
				  hhafp = matrix_open( optarg, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
				  if (!hhafp) crash( "..can't open HH attenuation file\n");
				  hhattnFlag = 1;
			break;
		case '?':	/*  unknown option*/
				  crash( "Unknown option\n");
	  }

	if (scanin == NULL || scanout == NULL) {
		crash("%s\nusage: %s [-v verbose] [-f frame] [-t trim] [-m mash] [-r rebin] [-c x_offset,y_offset] [-F dead_time,decay,arc_corr] [-n Normalization] [-a attenuation] [-h hh_attenuation] -i scanin -o scanout\n", sccsid,progname);
	}
 

	if (scanin == NULL || scanout == NULL) usage();
	sinfp = matrix_open(scanin, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (!sinfp) matrix_perror( progname);
	if  (sinfp->mhptr->file_type != Sinogram &&
		sinfp->mhptr->file_type!=Short3dSinogram)
		crash("%s not a sinogram file\n",scanin);

/* fill-up main header for output file */

  mh = *sinfp->mhptr;
  mh.sw_version = 6;				/* force software version to 6 */
	mh.file_type = Sinogram;
  if(normFlag) mh.calibration_units = nfp->mhptr->calibration_units;
  soutfp = matrix_create( scanout, MAT_OPEN_EXISTING, &mh);
/*	numPlanes not filled  on ECAT 7.0 beta */
	 numPlanes = mh.num_planes;
	 numPlanes = 47;
  numFrames = mh.num_frames;
  if (verbose) fprintf (stdout,"Study has %3d  planes  and %3d frames \n", numPlanes, numFrames);

  for (plane=1; plane<=numPlanes; plane++) {
	  if (verbose) fprintf (stdout,"\nProcessing plane %d from frame %d\n", plane, frame);
	  scan_matnum = mat_numcod (frame, plane, 1, 0, 0);
	  if (scan) matrix_free( scan);
	  scan = my_matrix_read( sinfp, scan_matnum, GENERIC);
	  if (!scan) crash( "..can't read scan matrix for frame %d, plane %d\n", frame, plane);

	  scand = (short int*) scan->data_ptr;
	  ssh = (Scan_subheader *) scan->shptr;
	  numProjs = scan->xdim;
	  numViews = scan->ydim;
	  if (verbose) fprintf (stdout,"num_Projs = %3d  num_Views = %3d\n", numProjs, numViews);
	  binSize = ssh->x_resolution;
	  scanSize = numProjs*numViews;
	  if (!fdata) fdata = (float*) calloc( scanSize, sizeof(float));
	  sf = scan->scale_factor;
	  if (verbose) fprintf (stdout, "Scan scale factor = %f\n", sf);

	  /* Dead_time correction: in that case put loss_correction_fctr to 1. in subheader */

	  if (deadtimeFlag) {
		 sf *= ssh->loss_correction_fctr;
		 if (verbose) fprintf (stdout, "Dead Time Correction = %f\n", ssh->loss_correction_fctr);
		 ssh->loss_correction_fctr = 1.;
	  }

	  /* Decay correction */

	  frameStartTime = (float) (ssh->frame_start_time)/1000.;
	  frameDuration  = (float) (ssh->frame_duration)/1000.;
	  if (frameDuration <= 0.0) frameDuration = 1.0;
	  /* get branching fraction and halfLife if its not transmission scan */
	  halfLife = 0.0;
	  if (mh.acquisition_type != 2) {
		 j = 0;
		 while (j < NumberOfIsotopes && strcmp (mh.isotope_code, isotope_info[j].name) != 0) j++;
		 if (j < NumberOfIsotopes) {
	 sscanf(isotope_info[j].branch_ratio, "%f", &branchFrac);
	 halfLife = isotope_info[j].hl;
		 }
		 else {
	  fprintf (stdout, "Warning: improper or missing value for isotope(#%2d) in the main header\n", j);
			 if (decayFlag) {
				 decayFlag = 0;
				 fprintf (stdout, "Warning: decay correction has been turned off\n");
			 }
		 }
	  }

	 /* Compute decay correction factor */

	 if (decayFlag) {
		if (halfLife == 0.0) halfLife = mh.isotope_halflife;
		if (halfLife > 0) {
			ln2 = log(2.0);
			lamdaT = frameDuration * ln2/halfLife;
			decayFactor = exp(frameStartTime*ln2/halfLife)*lamdaT/(1.-exp(-lamdaT));
			sf *= decayFactor;
			if (verbose) fprintf (stdout, "Decay Correction = %f\n", decayFactor);
		}
	 }
	 for (i=0; i<scanSize; i++) fdata[i] = sf*scand[i];

	/* Compute various parameters for arc correction */

	input_mash = mh.compression_code%256;
	input_mash = input_mash? input_mash*2 : 1;
	TotalNviews = numViews * input_mash;
	if (verbose) fprintf (stdout, "Input mash= %3d\n", input_mash);
	radius = 0.0;
	if (arcorFlag) {
		switch (mh.system_type) {
			case 931:
			case 951:
				radius = 102.0/2 + 1.0;		/* 951/931 model */
				break;
			case 933:
			case 953:
				radius = 76.0/2 + 1.0;		/* 953/933 model */
				break;
			case 961:
				radius = 82.4/2 + 1.0;  	/* 961 model */
				break;
			case 921:
				radius = 82.0/2 + 1.0;  	/* 921 model */
				break;
			case 831:
				radius = 64.0/2 + 1.0;  	/* 831 model */
				break;
			default:
				arcorFlag = 0;
				radius = 0.0;
		 fprintf (stdout, "%s:Warning...Unknown system type. Turning off arc correction\n", argv[0]);
		}  /* end switch */
		if (arcorFlag && radius > 0.0) {
			for (i=0; i<numProjs; i++) gprofile[i] = numProjs/2 + radius * sin(M_PI*(i-numProjs/2)/(2*TotalNviews))/binSize;
			w = binSize * TotalNviews * 2 / M_PI /radius;
		}
	 }

	 /*  Normalization ... */

	 if (normFlag) {
		  nrm_matnum = mat_numcod (1, plane, 1, 0, 0);
		  norm = matrix_read(nfp, nrm_matnum, GENERIC);
		  if (!norm)  crash( "..can't read normalization matrix for plane %d\n", plane);
		  nsh = (Norm_subheader *) norm->shptr;
		  normd = (float *) norm->data_ptr;
		  sf = nsh->scale_factor;
		  for (i=0; i<scanSize; i++) fdata[i] *= sf*normd[i];
		  if (verbose)  fprintf(stdout, "Sinogram normalized: scale factor = %f\n", sf);	
	 }

	 /*  Attenuation ... (scale factor is 1.)*/

	 if (attnFlag) {
		  attn_matnum = mat_numcod (1, plane, 1, 0, 0);
		  attn = matrix_read(afp, attn_matnum, GENERIC);
	/* if matrix does not exist assume same as for plane 1 (calibration phantom case) */
		  if (!attn)  {
		if (verbose) fprintf(stdout,"..can't read attenuation matrix for plane %d, use plane 1 instead\n", plane);
				attn_matnum = mat_numcod (1, 1, 1, 0, 0);
		  	attn = matrix_read(afp, attn_matnum, GENERIC);
		  	if (!attn)  crash( "..can't read attenuation matrix \n");
	}
		  attnd = (float *) attn->data_ptr;
		  for (i=0; i<scanSize; i++) fdata[i] *= attnd[i];
		  if (verbose)  fprintf(stdout, "Sinogram corrected for attenuation\n"); 
	 }

	 /*  HH Attenuation ...(scale factor is 1.) */

	 if (hhattnFlag) {
		  hhattn_matnum = mat_numcod (1, plane, 1, 0, 0);
		  hhattn = matrix_read(hhafp, hhattn_matnum, GENERIC);
		  if (!hhattn)  crash( "..can't read HH attenuation matrix for plane %d\n", plane);
		  hhattnd = (float *) hhattn->data_ptr;
		  for (i=0; i<scanSize; i++) fdata[i] *= hhattnd[i];
		  if (verbose)  fprintf(stdout, "Sinogram corrected for HH attenuation\n");	
	 }

	 /* Arc correction ... */
 
	 if (arcorFlag) {
		  if (!ftmp) ftmp = (float*) calloc( scanSize, sizeof(float));
	memcpy(ftmp,fdata,scanSize*sizeof(float));
	arcCorrect (numProjs, numViews, ftmp, fdata, gprofile, w);
		  if (verbose)  fprintf(stdout, "Arc correction made \n");	
	 }

	 /* Re-center ... */

	 if (xoff != 0. || yoff !=0. ) {
		  if (!ftmp) ftmp = (float*) calloc( scanSize, sizeof(float));
	memcpy(ftmp,fdata,scanSize*sizeof(float));
		  for (i=0; i<scanSize; i++) fdata[i] = 0.;
		  if (verbose)  fprintf(stdout, "Sinogram recentered with x = %f y= %f\n",xoff, yoff);	
		  if ((numViews % 96) == 0)
			  tilt = -15.0;
		  else if ((numViews % 98) == 0)
			  tilt = -12.86;
		  else
			  tilt = 0.0;
	scan_shift(numProjs, numViews, ftmp, fdata, xoff, yoff, tilt, binSize );
	 }
 
	 /* Mashing (angular rebinning) and triming ... */

	 if (mash != 1 || trim < numProjs ) {
			nv = numViews/mash;
			np = trim;
			if (!ftmp) ftmp = (float*) malloc( scanSize*sizeof(float));
	memcpy(ftmp,fdata,scanSize*sizeof(float));
	  	mash_trim(numProjs, numViews, ftmp, trim, mash, fdata );
	numProjs = np;
	numViews = nv;
		  scan->xdim = numProjs;
		  scan->ydim = numViews;
		  if (verbose)  fprintf(stdout, "Sinogram trimmed at %d and mashed by %d\n",trim, mash);	
	  }

	 /* Spatial rebinning ... */

	 if (d_rebin != 1) {
			np = numProjs/d_rebin;
	if (numProjs%d_rebin != 0) crash(" Rebinning factor %d must divide %d\n",d_rebin,numProjs);
			if (!ftmp) ftmp = (float*) malloc( scanSize*sizeof(float));
	memcpy(ftmp,fdata,scanSize*sizeof(float));
	  	rebin(numProjs, numViews, ftmp, d_rebin, fdata );
	numProjs = np;
		  scan->xdim = numProjs;
		  scan->pixel_size = binSize*d_rebin;				/* see convert_float_scan why ssh is not updated directly */
		  if (verbose)  fprintf(stdout, "Sinogram rebinned spatially by %d\n",d_rebin);	
	  }

	  /* Convert to short and write */

	  convert_float_scan( scan, fdata);					/* ssh->sample_distance = scan->pixel_size; */
	  ssh->scale_factor *= 1.0/(mash*input_mash*d_rebin*d_rebin);	/*  similar to import_3d : keeps quantification OK */
	  matrix_write(soutfp, scan_matnum, scan);

	} /* End for */

	mat_close(sinfp);
	mat_close(soutfp);
	if(nfp) mat_close(nfp);
	if(afp) mat_close(afp);
	if(hhafp) mat_close(hhafp);
}

mash_trim(numProjs, numViews, scan_in, trim, mash, scan_out)
	int		 	numProjs, numViews,trim, mash;
	float	*scan_in, *scan_out;
{
	int		 	i, j, np, nv;
	float	*fp1,*fp2;

	nv = numViews/mash;
	np = trim;

	for (i=0; i<np*nv; i++) scan_out[i] =  0.0;

	for (i=0; i<numViews; i++)
	{
	  fp1 = scan_in+numProjs*i;
	  fp2 = scan_out+np*(i/mash);
	  for (j=0; j<np; j++)
		 fp2[j] += fp1[j+numProjs/2-np/2];
	}

}

rebin(numProjs, numViews, scan_in, d_rebin, scan_out)
	int		 	numProjs, numViews, d_rebin;
	float	*scan_in, *scan_out;
{
	int		 	i, j, k=0, np;

	np = numProjs/d_rebin;
	for (i=0; i<np*numViews; i++) {
	scan_out[i] =  0.0;
	for (j=0; j<d_rebin; j++) scan_out[i] += scan_in[k++];
  }
}

arcCorrect(numProjs, numViews, scan_in, scan_out, profile, w)
	int		 	numProjs, numViews;
	float	*scan_in, *scan_out, *profile, w;
{
	int		 	i, j, o;

	float	 	in,f1, f2;

	for (i=0; i<numProjs; i++) {
		 o = (int) profile[i];
		 f1 = profile[i] -  o;
		 f2 = 1.0 - f1;

		 if (i == numProjs/2) 
			 f2 = w;
		 for (j=0; j<numViews; j++) {
			  in = scan_in[i+j*numProjs];
			  scan_out[o+j*numProjs] += in * f2;
			  scan_out[o+j*numProjs+1] += in * f1;
		 }
	}
}

scan_shift(numProjs, numViews, scan_in, scan_out, xoff, yoff, tilt, bin_size )
	int		 	numProjs, numViews;
	float	*scan_in, *scan_out, xoff, yoff, tilt, bin_size;
{
	double	theta;
	int		i, j, k;
	float	shift, x0, y0, x, w;

/* the shift should be modified to account for the intrinsic tilt */

	theta = - M_PI*tilt/180.0;
	x0 =  xoff*cos(theta) + yoff*sin(theta);
	y0 = -xoff*sin(theta) + yoff*cos(theta);
/*	fprintf(stdout, "...in fact recentered with x = %f y= %f\n",x0, y0); */
  
	for (i=0; i<numViews; i++)
	{
		theta = M_PI*i/numViews;
		shift = (y0*sin(theta)-x0*cos(theta))/bin_size;
		for (j=0; j<numProjs; j++)
		{	
		 	x = shift + j;
		 	if (x<0.0 || x>(float)(numProjs-2)) continue;
		 	k = (int) x;
		 	w = x-k;
		 	scan_out[k+i*numProjs] += (1.0-w)*scan_in[i*numProjs+j];
		 	scan_out[k+1+i*numProjs] += w*scan_in[i*numProjs+j];
	 	}
	 }
}
