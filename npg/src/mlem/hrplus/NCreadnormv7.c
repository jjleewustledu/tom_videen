/* module NCreadnormv7 */

/*
 general information: 
 This module reads the information from a V70 matrix normalization file and
 expands it to a normalization sinogram, to be applied to a single 2D
 emission sinogram.
 */

/******************************************************************************

* topic History
  This routine is directly derived from 
		make_2dnorm
		December 15, 1997, C. Michel

  Modification: main program converted into module which operates on a
                single sinogram. January 16 1998,  J. Nuyts

  Author(s)     : C. Michel, J. Nuyts
  Date          : jan 1998
  Modifications :
* endtopic History

******************************************************************************/
#include <stdio.h>  /*NOINC*/
#include <string.h>  /*NOINC*/
#include <stdlib.h>  /*NOINC*/
#include <math.h>  /*NOINC*/
#include <ctype.h>  /*NOINC*/
#include "osem.h"  /*NOINC*/
#include "ecat_model.h"  /*NOINC*/
#include "matrix.h"  /*NOINC*/
#include "matrix_extra.h"  /*NOINC*/

/*enddeclars*/

#define RMAX 32		/* Number of crystal rings supported */

/* These macros compute the crystal pair associated with a particular
 view and element.  For oblique projections det1 is always associated
 with ring_a while det2 is associated with ring_b.  */

#define ve_to_det1(e,v,nv) ( (v + (e >> 1) + 2*nv) % (2*nv) )
#define ve_to_det2(e,v,nv) ( (v - ((e+1) >> 1) +3*nv) % (2*nv) )

/* Some of the variables are intended to be global!
 * They cannot be moved into the main program.
 ******/

static int num_planes;
static int corrections_applied=0;

static int num_r_elements;  /* Number of radial elements in a projection */
static int num_angles;	    /* Number of angles in a sinogram */
static int number_of_views; /* Number of unmashed angles in a sinogram */
static float binsize;	    /* binsize in cm */
static int mash=1;	    /* Number of angles added together. 1, 2, 4, ... */
static int transaxial_crystals;  /* Number of transaxial crystals in a block */
static int axial_crystals;	 /* Number of axial crystals in a block */
static int block_rings;	    /* Number of rings of blocks */
static int crystal_rings;   /* Number of rings of crystals */
static int crystals_per_ring;	 /* Number of rings of crystals */
static int span;	    /* ring differences to accept in a projection */
static int max_acceptance;  /* Maximum ring difference to accept */
static int blocks_per_bucket;	 /* Number of blocks in a bucket */
static int crystals_per_bucket;  /* Nr of transaxial crystals in a bucket */
static int buckets_per_ring;	 /* Number of buckets in a ring */
static float ring_radius;	 /* ring radius in cm */
static int septa_in;	      /* One if septa is in field of view, 0 if out */
static int *ringnums;	     /* Points to an array of number_of_planes by 
                             span+2 containing the number of ring pairs 
                             and the ring pairs for each plane. */
static float *dtime_efficiency;  /* Points to a list of deadtime corrected
                                crystal efficiencies. One entry per
                                crystal. This gets loaded for each frame. */
static float *crys_efficiency;	/* Points to a list of crystal efficiencies.
                       One entry per crystal. Gets loaded at initalization. */
static float *crystal_interference; /* Points to a transaxial_crystals by 
                                num_r_elements array that contains the crystal
                                interference efficiencies. */
static float *geometric;  /* Points to a num_r_elements by number_of_planes
                             array that contains the geometric efficiency for
                             each plane. */
static float *axial_t1;	  /* Contains the paralyzing deadtimes for each axial
                             crystal */
static float *axial_t2;	  /* Contains the non-paralyzing deadtimes for each
                             axial crystal */
static float *trans_t1;	  /* Contains the non-paralyzing deadtimes for each
                             transaxial crystal in a block. */



static int intrinsicFlag=1, geometricFlag=1, deadtimeFlag=1;

static SINOGRAM sino=0;		/*sinogram */
static Scan3D_subheader *ssh=0;
static Norm3D_subheader *nsh=0;
static MatrixData *scan_matrix=0;
static MatrixData *norm_matrix=0;
static MatrixData *volume=0;

static int Verbose = 0;

/*============================================================*/
SINOGRAM alloc_sino()
{	int y;
	int nblks;
	SINOGRAM ret = (float**)calloc(num_angles,sizeof(float*));
	nblks = (num_angles*num_r_elements*sizeof(float)+511)/512;
        /* allocated data space is expected to be in blocks */
	ret[0] = (float*)calloc(nblks,512);	
	for (y=1; y<num_angles; y++) ret[y] = ret[0]+y*num_r_elements;
	return ret;
}

/*============================================================*/
processing_status(corrections_applied)
int corrections_applied;
{
/* print processing status */
if(corrections_applied == 0) 
  {
  if (Verbose) fprintf(stderr, "  Scan is uncorrected  \n");
  }
else 
  {
  if (Verbose) fprintf(stderr, "  Scan is corrected for: ");
  if (corrections_applied & 0x0001) fprintf(stderr, "normalization,"); 
  if (corrections_applied & 0x0002) fprintf(stderr, "measured attenuation,"); 
  if (corrections_applied & 0x0004) fprintf(stderr, "calculated attenuation,"); 
  if (corrections_applied & 0x0008) fprintf(stderr, "x smoothed,"); 
  if (corrections_applied & 0x0010) fprintf(stderr, "y smoothed,"); 
  if (corrections_applied & 0x0020) fprintf(stderr, "z smoothed,"); 
  if (corrections_applied & 0x0040) fprintf(stderr, "scatter in 2D,"); 
  if (corrections_applied & 0x0080) fprintf(stderr, "scatter in 3D,"); 
  if (corrections_applied & 0x0100) fprintf(stderr, "curvature,"); 
  if (corrections_applied & 0x0200) fprintf(stderr, "decay"); 
  fprintf(stderr, "\n");
  }
}

/*============================================================*/
matrix_float(matrix)
MatrixData *matrix;
{
	float scalef, *fdata;
	short *sdata;
	int i, np = matrix->xdim*matrix->ydim;

	matrix->data_type = IeeeFloat;
	fdata = (float*)calloc(np,sizeof(float));
	sdata = (short*)matrix->data_ptr;
	scalef = matrix->scale_factor;
	matrix->data_ptr = (caddr_t)fdata;
	for (i=0; i<np;i++) fdata[i] = scalef * sdata[i];
	matrix->scale_factor = 1.0;
	free(sdata);
}

/*============================================================*/
MatrixData *read_sino(mptr,data, matnum)
MatrixFile *mptr;
float data[];
int matnum;
{
	int i,j,vmatnum;
	MatrixData *volume;
	MatrixData *matrix;
	float *p;
	float scf;
	struct Matval val;

	if (Verbose) fprintf(stderr,"  Reading sinogram matrix %d...",matnum);
	
	if (mptr->mhptr->file_type == Sinogram ) {
		if ((matrix = matrix_read( mptr, matnum, GENERIC)) == NULL)
			crash( "  matrix %d not found\n",matnum);
	 	if (Verbose) fprintf(stderr,"  %dx%dx%d done\n", 
                                     matrix->xdim,matrix->ydim, matrix->zdim);
		matrix_float(matrix);
	}
	else {
		mat_numdoc(matnum,&val);
		vmatnum = mat_numcod(val.frame,1,val.gate,val.data,val.bed);
		if ((volume= matrix_read( mptr, vmatnum, MAT_SUB_HEADER)) 
                    == NULL)
			crash( " volume %d not found\n",vmatnum);
		if (Verbose) 
                  fprintf(stderr,"  reading slice %d ...",val.plane-1);
		if ((matrix = matrix_read_slice(mptr, volume,val.plane-1,0))
                    == NULL)
   			crash( "  slice %d not found\n",val.plane-1);
	 	if (Verbose) 
                  fprintf(stderr,"  %dx%dx%d done\n", matrix->xdim,
                          matrix->ydim, matrix->zdim);
		if (mptr->mhptr->file_type == Short3dSinogram ) 
                  matrix_float(matrix);
		free_matrix_data(volume);
	}

	p = (float*)matrix->data_ptr;
	for (i=0; i<num_angles; i++)
          for (j=0; j<num_r_elements; j++) 
            data[i*num_r_elements+j] = *p++;
	free_matrix_data(matrix);
}

/*============================================================*/
one_sino(sino)
	float sino[];
{
	int i;
	for (i=0; i<num_r_elements*num_angles; i++) {
		*sino = 1.;
		sino++;
	}
}

/*============================================================*/
norm_sino(sino,z)
	float sino[];
	int z;
{
	int crys_a;
	int crys_b;
	int elem;
	int view;
	int mashed_view;
	int *zptr;	
	int npairs;
	int pair;
	float lor_efficiency;
	float view_efficiency;
	float *geoptr;
	float *interferptr;
	int start_view;
	int end_view;
	
	zptr = &ringnums[z*(span+2)];
	npairs = zptr[0];

	for (mashed_view = 0; mashed_view < num_angles; mashed_view++)
	{
	start_view = mashed_view * mash;
        end_view = start_view + mash;
        geoptr = &geometric[z*num_r_elements];
        interferptr = crystal_interference;
        for( elem=-num_r_elements/2; elem < num_r_elements/2; elem++)
          {   
          view_efficiency = 0.;
          for( view = start_view; view < end_view; view++)
            {
            lor_efficiency = 0.;
            crys_a=ve_to_det1(elem,view,number_of_views);
            crys_b=ve_to_det2(elem,view,number_of_views);
            for( pair = 1; pair <= npairs*2; pair += 2)
              lor_efficiency += dtime_efficiency[crys_a+crystals_per_ring
                                                *zptr[pair]]
                     *dtime_efficiency[crys_b+crystals_per_ring*zptr[pair+1]];
            if(intrinsicFlag) 
              view_efficiency += lor_efficiency * 
                                 interferptr[view % transaxial_crystals];
            else 		/* isolated deadtime correction */
              view_efficiency += lor_efficiency/npairs/mash;
            }
          *sino /=  view_efficiency;
          interferptr += transaxial_crystals;
          
          if(geometricFlag) 
            {
            *sino /=  *geoptr; 
            geoptr++;
            }
          sino++;
          }
	}
	return 1;
}

/*============================================================*/
/* Make_ring_numbers returns a pointer to an array containing the ring
 pairs for each plane. The array is ringnums[nplanes][span+2]. The
 first entry is number of pairs followed by the ring_a, ring_b pairs
 for each lor in the plane. If no memory is available, NULL is
 returned.  */
#include <malloc.h> /* NOINC */
#include <stdio.h> /* NOINC */

int *make_ring_numbers(rmax,span,amax)

	int	rmax;		/* Number of crystal rings. (May be virtual) */
	int	span;		/* Sum of the number of cross planes in odd
                                   and even planes. */
	int	amax;		/* Maximum number of cross planes to accept
			 	 * for all projections. (Acceptance angle) */

  {
  int	dmax;		/* Maximum number of cross planes in a projection */
  int	nmax;		/* Maximum angular projection index. */
  int	zindex[ RMAX ];	/* The index of the begining of the
                         * positive projection.	*/
  int	zsize[ RMAX ];	/* The z offset to the begining of the projection. */
  int	nvdp;		/* The number of virtual direct planes */
  int	i;
  int	r1,r2;		/* Ring numbers. */
  int	nabs;		/* Projection index. */
  int	plane;		/* The virtual plane number */
  int	*ringnums;	/* The array to hold the ring numbers */
  int	*plnptr;	/* points into a row */
  int	nplanes;	/* The number of planes */
  
  /* Compute plane offsets at the begining of the projections and 
   * the size of the projections.	*/
  
  span = (span < 3) ? 3 : span;
  dmax = span/2+1;
  amax = (amax < rmax) ? amax : rmax-1;
  nmax = (amax+1)/span;
  nvdp = 2*rmax-1;
  
  /* Get the number of planes and make the array */
  
  nplanes = number_of_planes(rmax,span,amax);
  ringnums = (int *) malloc( sizeof(int)*nplanes*(span+2) );
  if( ringnums == NULL )
    {
    fprintf(stderr," ERROR: make_ring_nums: Not enough memory for ringnums array.\n");
    return 0;
    }
  for( i=0; i< nplanes*(span+2); i++)
    ringnums[i]=0;
  
  /* Compute plane offsets at the begining of the projections and 
   * the size of the projections.	*/
  
  zsize[0] = 0;
  zindex[0] = 0;
  zsize[1] = dmax;
  zindex[1] = nvdp;
  for( i=2; i <= nmax; i++)
    {
    zindex[i] = zindex[i-1]+2*(nvdp-2*zsize[i-1]);
    zsize[i] = (2*i-1)*dmax-i+1;
    }
  
  /* Compute the plane number for each pair of crystals   
   * and stuff in the ringnums array. */
  
  for (r1=0; r1 < rmax; r1++)
    for(r2=0; r2 < rmax; r2++)
      {
      nabs = ( abs(r1-r2) + dmax - 1 ) / span;
      if (nabs <= nmax)
        {
        plane = (r1 + r2) - zsize[nabs];
        plane += zindex[nabs];
        if ( ((r1 - r2) < 0) && (nabs > 0) )
          plane += nvdp-2*zsize[nabs];
        if( plane >= nplanes)
          {
          fprintf(stderr," Plane %d is larger than %d\n", plane,nplanes);
          return 0;
          }
        plnptr = &ringnums[plane*(span+2)];
        i=plnptr[0];
        plnptr[2*i+1]=r1;
        plnptr[2*i+2]=r2;
        plnptr[0]++;
        }
      }
  return ringnums;
  }
  
/*============================================================*/
int number_of_planes(rmax,span,amax)
	int	rmax;	/* Number of crystal rings. (May be virtual) */
	int	span;	/* Sum of the number of cross planes in odd and even
			 * planes. */
	int	amax;	/* Maximum number of cross planes to accept */

{
int	nvdp;	/* The number of virtual direct planes */
int	nplanes;/* Collects the number of planes */  
int	i;

span = (span < 3) ? 3 : span;
amax = (amax < rmax) ? amax : rmax-1;
nvdp = (2*rmax-1);
nplanes = nvdp;
for (i=1; i <= (amax+1)/span; i++)
  nplanes = nplanes + 2*( nvdp -( (2*i-1)*span + 1 ));
return nplanes;
}
								 
/*============================================================*/
/* Called to initalize the efficiencies for each crystal. Allocates
 * memory if none is allocated.  Routine needs the crystal deadtimes
 * initalized by init_deadtime(). Returns a 1 for success, a zero if
 * memory could not be allocated.  */

load_deadtime_array(singles)
	float *singles;

{
int ring;
int crystal;
int bucket;
float rate;
int gantry_crystal;

if (dtime_efficiency == NULL)
  {
  dtime_efficiency = (float *) malloc( sizeof( float) * crystal_rings 
                                       * crystals_per_ring);
  if (dtime_efficiency == NULL)
  return 0;
  }
for (ring = 0; ring < crystal_rings; ring++)
  for (crystal=0; crystal < crystals_per_ring; crystal++)
    {
    bucket = crystal/crystals_per_bucket + buckets_per_ring
             *(ring/axial_crystals);
    rate = singles[bucket] / (float) blocks_per_bucket;
    gantry_crystal = ring*crystals_per_ring+crystal;
    if (intrinsicFlag)
    dtime_efficiency[gantry_crystal] = crys_efficiency[gantry_crystal];
    else                                /* isolated deadtime correction */
      dtime_efficiency[gantry_crystal] = 1.;
    if (deadtimeFlag)			/* factorizable deadtime corr */
    /* Exponential model: better use polynomial model!
      dtime_efficiency[gantry_crystal] 
        *= exp( -axial_t1[ring]*rate/(1.+axial_t2[ring]*rate) )
        /( (1.+axial_t2[ring]*rate)
           *(1.+trans_t1[crystal % transaxial_crystals]*rate) );
    */ 
    /* Polynomial model */
    dtime_efficiency[gantry_crystal] *=
      (1. + axial_t1[ring]*rate + axial_t2[ring]*rate*rate)
    * (1.+trans_t1[crystal % transaxial_crystals]*rate);
      
    }
return 1;
}

/*============================================================*/
init_deadtime()
  {
  int i;
  if (Verbose) { 
  fprintf(stderr,"  number_of_elements:\t%d\n",num_r_elements);
  fprintf(stderr,"  number_of_views:\t%d\n",number_of_views);
  fprintf(stderr,"  mash:\t%d\n",mash);
  fprintf(stderr,"  transaxial_crystals:\t%d\n",transaxial_crystals);
  fprintf(stderr,"  axial_crystals:\t%d\n",axial_crystals);
  fprintf(stderr,"  crystal_rings:\t%d\n",crystal_rings);
  fprintf(stderr,"  block_rings:\t%d\n",block_rings);
  fprintf(stderr,"  crystals_per_ring:\t%d\n",crystals_per_ring);
  fprintf(stderr,"  span:\t%d\n",span);	 
  fprintf(stderr,"  max_acceptance:\t%d\n",max_acceptance);
  fprintf(stderr,"  blocks_per_bucket:\t%d\n",blocks_per_bucket);
  fprintf(stderr,"  crystals_per_bucket:\t%d\n",crystals_per_bucket);
  fprintf(stderr,"  buckets_per_ring:\t%d\n",buckets_per_ring);
  }
  ringnums = make_ring_numbers( crystal_rings, span, max_acceptance );
  if (ringnums == NULL) return 0;
  return 1;
  }

/*============================================================*/
/* global */
float*  NCreadnormv7(
  char*      sino_file,
  char*      norm_file,
  int        plane,
  int        frame,
  int        gate,
  int        bed,
  int        verbose,
  int        corr_intrinsic,
  int        corr_geometric,
  int        corr_deadtime)

{
int            c,i,z;
int            v,r;		/*indices in sinogram*/
/*ECAT stuff*/
struct Matval matval;
struct MatDir matdir, dir_entry;

int            matnum=0;
int            nbg;
int            nblks, data_size, blkno;
int            testFlag=0;
int            normFlag=1;   /* 1 = 2D normalization, 0 = normalized scan */
Main_header    *smh=NULL, *nmh=NULL;
MatrixFile     *sinofp=NULL, *normfp=NULL;
time_t         t;

EcatModel      *model_info;

extern int     optind, opterr;


/* Initializations
******************/
Verbose             = verbose;
corrections_applied = 0;
mash                = 1;
intrinsicFlag = corr_intrinsic;
geometricFlag = corr_geometric;
deadtimeFlag  = corr_deadtime;

/* Open the sinogram file
****************/
sinofp = matrix_open(sino_file, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
if (!sinofp) crash( "  can't open file '%s'\n", sino_file);
smh = sinofp->mhptr;

/* input and output matrix number */
matnum = mat_numcod(frame,1,1,0,bed);

/* get scan subheader */
scan_matrix    = matrix_read(sinofp, matnum, MAT_SUB_HEADER);
ssh            = (Scan3D_subheader*)scan_matrix->shptr;
num_angles     = ssh->num_angles;
num_r_elements = ssh->num_r_elements;
num_planes     = ssh->num_z_elements[0];
binsize        = ssh->x_resolution;	/* binsize in cm */
if (Verbose)
  {
  fprintf(stderr,"3D norm: reading %s and %s\n", sino_file, norm_file);
  fprintf(stderr,"  frame %d, bed %d\n", frame, bed);
  fprintf(stderr,"  Number of radial pixels is %d \n",num_r_elements);
  fprintf(stderr,"  Number of projections is   %d \n",num_angles);
  fprintf(stderr,"  Number of planes is        %d \n",num_planes);
  fprintf(stderr,"  Bin size is %f \n",binsize);
  }

/* Allocate space */
sino=alloc_sino();

for (i=1; i <= smh->angular_compression; i++) mash *= 2;
span           = ssh->axial_compression;
max_acceptance = ssh->ring_difference;
if (Verbose)
  fprintf(stderr,"  Mash is %d Span is %d Ring Difference is %d\n",
          mash, span, max_acceptance);
number_of_views   = num_angles*mash;	/* unmashed views */
crystals_per_ring =  2*number_of_views; /* necessary for arc correction */

septa_in = (smh->septa_state == 0) ? 1 : 0;
if(Verbose)
  fprintf(stderr,"  Sinogram acquisition mode is %s\n", septa_in? "2D" : "3D");

corrections_applied = ssh->corrections_applied;
processing_status(corrections_applied);

/* Test if requested correction is allowed */
testFlag = (corrections_applied & 0x0001); /* normalization */
if (testFlag)
	fprintf(stderr," Scan is already normalized\n");

if(Verbose) fprintf(stderr,"  Scanner model is %d \n",smh->system_type);
model_info = ecat_model(smh->system_type);	/* info for normalization */

normfp = matrix_open(norm_file, MAT_READ_ONLY, Norm3d);
if (!normfp)
	crash("  Can't open normalization file %s\n", norm_file);
nmh = normfp->mhptr;
t   = nmh->scan_start_time;
if(Verbose)
  {
  fprintf(stderr,"  Normalization TOD  : %s",ctime(&t));
  fprintf(stderr,"  Calibration factor : %e \n",nmh->calibration_factor);
  fprintf(stderr,"  Calibration units  : %d \n",nmh->calibration_units);
  fprintf(stderr,"  Calibration units labels : %d \n",
          nmh->calibration_units_label);
  }

if (Verbose)
  {
  printf("fname   %s\n", normfp->fname);
  printf("dirlist %d\n",       normfp->dirlist);
  printf("FILE    %d\n",       normfp->fptr);
  }

norm_matrix =  matrix_read(normfp, mat_numcod(1,1,1,0,0), GENERIC);
if (!norm_matrix) 
	crash("Normalization matrix %s,1,1,1,0,0 not found\n",normfp->fname);

transaxial_crystals = model_info->angularCrystalsPerBlock;
axial_crystals      = model_info->axialCrystalsPerBlock;
block_rings         = model_info->rings*model_info->axialBlocksPerBucket;
crystal_rings       = block_rings * axial_crystals;
blocks_per_bucket   = model_info->transBlocksPerBucket;
crystals_per_bucket = blocks_per_bucket*transaxial_crystals;
buckets_per_ring    = crystals_per_ring / crystals_per_bucket;

init_deadtime();
nsh                  = (Norm3D_subheader *)norm_matrix->shptr;
geometric            = (float*)norm_matrix->data_ptr;
crystal_interference = geometric + nsh->num_geo_corr_planes*num_r_elements;
crys_efficiency      = crystal_interference 
                        + transaxial_crystals*num_r_elements;
axial_t1             = nsh->ring_dtcor1;
axial_t2             = nsh->ring_dtcor2;
trans_t1             = nsh->crystal_dtcor;
if ((load_deadtime_array(ssh->uncor_singles)) != 1)
  crash("Error...in loading singles");

data_size =  scan_matrix->xdim * scan_matrix->ydim * scan_matrix->zdim
             *sizeof(float);	/* same as original */
nblks = (data_size+511)/512;

if(Verbose)
  {
  if (normFlag) 	
    fprintf(stderr,"  Normalization includes ");
  else 
    fprintf(stderr,"  Scan is normalized with ");
  if (intrinsicFlag) fprintf(stderr,"intrinsic "); 
  if (deadtimeFlag) fprintf(stderr,"deadtime "); 
  if (geometricFlag) fprintf(stderr,"geometric "); 
    fprintf(stderr,"corrections \n");
  }

if(Verbose) fprintf(stderr,"Processing plane %d\n", plane);

matnum = mat_numcod(frame, plane, 1, 0, bed);

if (normFlag) 	
  one_sino(sino[0]);
else  
  read_sino(sinofp, sino[0], matnum);

norm_sino(sino[0], plane-1);	

if(Verbose) fprintf(stderr,"Normalization unfolded!\n");

free_matrix_data(scan_matrix);
free_matrix_data(norm_matrix);
matrix_close(sinofp);
matrix_close(normfp);

return sino[0];
}
