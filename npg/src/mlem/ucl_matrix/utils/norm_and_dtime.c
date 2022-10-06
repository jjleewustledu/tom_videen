/*
 * Calling correct_for_dtime corrects a 2D projection for deadtime. The internal data must first be
 * initalized by a call to init_dtime() which loads the scan dependant parameters. For each frame,
 * load_deadtime_array() must be called to compute the crystal efficiencies.
 */
 
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define AXIAL_CRYSTAL_RINGS 32
#define TRANSAXIAL_CRYSTALS 8

#define BEG_CRYSTAL	36
#define END_CRYSTAL	52
#define ROTATION_OFFSET 44
#define CRYS_A_OFFSET 	ROTATION_OFFSET
#define CRYS_B_OFFSET	192 + ROTATION_OFFSET

/*
 * These macros compute the crystal pair associated with a particular
 * view and element. For oblique projections det1 is always associated
 * with ring_a while det2 is associated with ring_b.
 */
#define ve_to_det1(e,v,nv) ( (v + (e >> 1) + 2*nv) % (2*nv) )
#define ve_to_det2(e,v,nv) ( (v - ((e+1) >> 1) +3*nv) % (2*nv) )

#define TIMES_FILENAME "times.dat"

int number_of_elements;			/* Number of radial elements in a projection */
int number_of_views;			/* Number of unmashed polar angles in a sinogram */
int mash;				/* Number of angles added together. 1, 2, 4, ... */
int transaxial_crystals;		/* Number of transaxial crystals in a block */
int axial_crystals;			/* Number of axial crystals in a block */
int block_rings;			/* Number of rings of blocks */
int crystal_rings;			/* Number of rings of crystals */
int crystals_per_ring;			/* Number of rings of crystals */
int span;				/* Number of ring differences to accept in a projection */
int max_acceptance;			/* Maximum ring difference to accept */
int blocks_per_bucket;			/* Number of blocks in a bucket */
int crystals_per_bucket;		/* Number of transaxial crystals in a bucket */
int buckets_per_ring;			/* Number of buckets in a ring */
int septa_in;				/* One if septa is in field of view, 0 if out */
 
int *ringnums;				/* Points to an array of number_of_planes by span+2 containing
					 * the number of ring pairs and the ring pairs for each plane. */

float *dtime_efficiency;		/* Points to a list of deadtime corrected crystal efficiencies.
					 * One entry per crystal. This gets loaded for each frame. */
					 
float *crys_efficiency; 		/* Points to a list of crystal efficiencies. One entry per
					 * crystal. Gets loaded at initalization. */
					 
float *crystal_interference;		/* Points to a transaxial_crystals by number_of_elements array that
					 * contains the crystal interference efficiencies. */
					 
float *geometric;			/* Points to a number_of_elements by number_of_planes array that 
					 * contains the geometric efficiency for each plane. If septa is in
					 * the field of view, this arrray will have a row for each plane.
					 * If septa is out, this array will have only one row. */
					 
float *axial_t1;			/* Contains the paralyzing deadtimes for each axial crystal */
float *axial_t2;			/* Contains the non-paralyzing deadtimes for each axial crystal */
float *trans_t1;			/* Contains the non-paralyzing deadtimes for each transaxial
					 * crystal in a block. */

correct_for_dtime(projection,zoffset,zsize,mashed_view)
	float *projection;
	int zoffset;
	int zsize;
	int mashed_view;
	
{
	int crys_a;
	int crys_b;
	int elem;
	int view;
	int *zptr;	
	int z;
	int npairs;
	int pair;
	float lor_efficiency;
	float view_efficiency;
	float *geoptr;
	float *interferptr;
	int start_view;
	int end_view;
	
	start_view = mashed_view * mash;
	end_view = start_view + mash;
	
	zptr = &ringnums[zoffset*(span+2)];
	for( z=zoffset; z < zoffset+zsize; z++)
	{
		npairs = zptr[0];
		if (septa_in)
			geoptr = &geometric[z*number_of_elements];
		else
			geoptr = geometric;

		interferptr = crystal_interference;
		for( elem=-number_of_elements/2; elem < number_of_elements/2; elem++)
		{
			view_efficiency = 0.;
			for( view = start_view; view < end_view; view++)
			{
				lor_efficiency = 0.;
				crys_a=ve_to_det1(elem,view,number_of_views);
				crys_b=ve_to_det2(elem,view,number_of_views);
				for( pair = 1; pair <= npairs*2; pair += 2)
					lor_efficiency += dtime_efficiency[crys_a+crystals_per_ring*zptr[pair]]
						*dtime_efficiency[crys_b+crystals_per_ring*zptr[pair+1]];
				view_efficiency += lor_efficiency*interferptr[view % transaxial_crystals];
			}
			*projection++ /= (view_efficiency*( *geoptr++));
			interferptr += transaxial_crystals;
		}
		zptr += (span+2);
	}
	return 1;
}
							     
/* Calling make_art_norm makes a matrix that when multiplied by a projection 
 * produces a normalized projection.
 */	
make_art_norm(norm,zoffset,zsize)
	float *norm;
	int zoffset;
	int zsize;
{
	int crys_a;
	int crys_b;
	int elem;
	int view;
	int *zptr;
	int z;
	int npairs;
	int pair;
	int indx_a;
	int indx_b;
	float *profile;
	float *profile_count;
	float *geoptr;

	zptr = &ringnums[zoffset*(span+2)];
	profile = (float *) malloc( sizeof( float) * number_of_elements);
	if ( profile == NULL)
		return 0;
	profile_count = (float *) malloc( sizeof( float) * number_of_elements);
	if ( profile_count == NULL)
		return 0;
	for( z=zoffset; z < zoffset+zsize; z++)
	{
		npairs = zptr[0];
		geoptr = &geometric[1];
		for (elem = 0; elem < number_of_elements; elem++)
		{
			profile[elem]=0.;
			profile_count[elem]=0.;
		}
		for (crys_a = -BEG_CRYSTAL; crys_a < END_CRYSTAL; crys_a++)
			for(crys_b = -END_CRYSTAL; crys_b < BEG_CRYSTAL; crys_b++)
			{
				elem = crys_a - crys_b;
				if ( abs( elem ) < number_of_elements/2)
				{
					elem = elem + number_of_elements/2;
					for (pair = 1; pair <= npairs*2; pair += 2)
					{
						indx_a=crys_a+crystals_per_ring*zptr[pair]+CRYS_A_OFFSET;
						indx_b=crys_b+crystals_per_ring*zptr[pair+1]+CRYS_B_OFFSET;
						profile[elem] += dtime_efficiency[indx_a]*dtime_efficiency[indx_b];
					}
					profile_count[elem]++;
				}
			}
		*norm++=1.;
		for (elem = 1; elem < number_of_elements; elem++)
		{
			*norm = *geoptr++ * (profile[elem]+profile[number_of_elements-elem])
				/(profile_count[elem]+profile_count[number_of_elements-elem]);
			if (*norm <= 0. )
				*norm = 1.;
			else
				*norm = 1./(*norm);
			norm++;
		}
		zptr += (span+2);
	}
	free ( profile);
	free ( profile_count);
	return 1;
}

/* Called to initalize the efficiencies for each crystal. Allocates memory if none is allocated.
 * Routine needs the crystal deadtimes initalized by init_deadtime(). Returns a 1 for success,
 * a zero if memory could not be allocated.
 */

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
		dtime_efficiency = (float *) malloc( sizeof( float) * crystal_rings * crystals_per_ring);
		if (dtime_efficiency == NULL)
			return 0;
	}
	for (ring = 0; ring < crystal_rings; ring++)
		for (crystal=0; crystal < crystals_per_ring; crystal++)
		{
			bucket = crystal/crystals_per_bucket + buckets_per_ring*(ring/axial_crystals);
			rate = singles[bucket] / (float) blocks_per_bucket;
			gantry_crystal = ring*crystals_per_ring+crystal;
			dtime_efficiency[gantry_crystal] = crys_efficiency[gantry_crystal]
				* exp( -axial_t1[ring]*rate/(1.+axial_t2[ring]*rate) )
				/( (1.+axial_t2[ring]*rate)*(1.+trans_t1[crystal % transaxial_crystals]*rate) );
		}
	return 1;
}

		
/* Called to initalize the scan dependant data. */
/* st = call_external('norm_and_dtime.so','init_deadtime',nelems,nviews,mash,trnsaxial_crys,ax_crys,rings,span,
 *		max_ringdiff,blks_per_bkt,septa_in)
 */

init_deadtime(argc,argv)
	int argc;
	void *argv[];
{
	int *make_ring_numbers();
	
	number_of_elements =  *(int *) argv[0];
	number_of_views = *(int *) argv[1];
	mash = *(int *) argv[2];
	transaxial_crystals = *(int *) argv[3];
	axial_crystals = *(int *) argv[4];
	block_rings = *(int *) argv[5];
	crystal_rings = block_rings * axial_crystals;
	crystals_per_ring = 2*number_of_views;	/* Does not support wobble */
	span = *(int *) argv[6];
	max_acceptance = *(int *) argv[7];
	blocks_per_bucket = *(int *) argv[8];
	crystals_per_bucket = blocks_per_bucket*transaxial_crystals;
	buckets_per_ring = crystals_per_ring / crystals_per_bucket;
	septa_in = *(int *) argv[9];
	
/*	printf("number_of_elements:\t%d\n",number_of_elements);
	printf("number_of_views:\t%d\n",number_of_views);
	printf("mash:\t%d\n",mash);
	printf("transaxial_crystals:\t%d\n",transaxial_crystals);
	printf("axial_crystals:\t%d\n",axial_crystals);
	printf("crystal_rings:\t%d\n",crystal_rings);
	printf("block_rings:\t%d\n",block_rings);
	printf("crystals_per_ring:\t%d\n",crystals_per_ring);
	printf("span:\t%d\n",span);     
	printf("max_acceptance:\t%d\n",max_acceptance);
	printf("blocks_per_bucket:\t%d\n",blocks_per_bucket);
	printf("crystals_per_bucket:\t%d\n",crystals_per_bucket);
	printf("buckets_per_ring:\t%d\n",buckets_per_ring);*/
	
	ringnums = make_ring_numbers( crystal_rings, span, max_acceptance );
	if (ringnums == NULL)
		return 0;
	return 1;
}

/* IDL interface for testing:
 * st = call_external('correct_for_deadtime.so','cfdtime',projection,zoffset,zsize,mashed_view)
 */

cfdtime( argc, argv)
	int argc;
	void *argv[];
{
	float *projection;
	int zoffset;
	int zsize;
	int mashed_view;
	if( argc != 4)
		return 0;
	projection = (float *) argv[0];
	zoffset = * (int *) argv[1];
	zsize = * (int *) argv[2];
	mashed_view = * (int *) argv[3];
	return correct_for_dtime(projection, zoffset, zsize, mashed_view);
}

/* IDL interface for testing:
 * st = call_external('correct_for_deadtime.so','artnorm',norm,zoffset,zsize)
 */

artnorm( argc, argv)
	int argc;
	void *argv[];
{
	float *norm;
	int zoffset;
	int zsize;
	if( argc != 3)
		return 0;
	norm = (float *) argv[0];
	zoffset = * (int *) argv[1];
	zsize = * (int *) argv[2];
/*	printf("ART init\n");*/
	return make_art_norm(norm, zoffset, zsize);
}

/* IDL interface for testing:
 * st = call_external('correct_for_deadtime.so','ldarray',singles)
 */

ldarray( argc, argv)
	int argc;
	void *argv[];
{
	float *singles;
	float *crysdtime;
	int st,i;
	if( argc != 1)
		return 0;
	singles = (float *) argv[0];
	st = load_deadtime_array(singles);
	return st;
}

prtringnum()
{
	int pair;
	int z;
	int *zptr;
	int npairs;
	for (z=0; z<(2*crystal_rings-1); z++)
	{
		zptr = &ringnums[z*(span+2)];
		npairs=zptr[0];
		printf("%d  %d  ",z,npairs);
		for(pair=1; pair<(npairs*2); pair+=2)
			printf("(%3d,%3d) ",zptr[pair],zptr[pair+1]);
		printf("\n");
	}
	return;
}

/* IDL interface for testing. Inits the scan data and loads the norm arrays
 * st = call_external('correct_for_deadtime.so','init_arrays',geoarray,crysintf,cryseffic,dtimeffic)
 */

init_arrays(argc,argv)
	int argc;
	void *argv[];
{
	if( argc != 7)
		return 0;
	
	
	geometric = (float *) argv[0];
	crystal_interference = (float *) argv[1];
	crys_efficiency = (float *) argv[2];
	dtime_efficiency = (float *) argv[3];
	axial_t1 = (float *) argv[4];
	axial_t2 = (float *) argv[5];
	trans_t1 = (float *) argv[6];
	return 1;
}

/* Frees up allocated memory. 
 */
 
free_norm()
{
	free( dtime_efficiency );
	return 1;
}
