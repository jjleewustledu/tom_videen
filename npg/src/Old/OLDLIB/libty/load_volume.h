/*$Id: load_volume.h,v 1.1 1995/10/16 17:21:36 ty7777 Exp $*/
/*$Log: load_volume.h,v $
 * Revision 1.1  1995/10/16  17:21:36  ty7777
 * Initial revision
 **/

#ifndef load_volume_h
#define load_volume_h

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX_SLICES 512
#define MAX_BED_POS 32
#define MAX_SLICES 512
#define MAX_BED_POS 32
#include "matrix.h"

typedef struct _Tslice
{
    int matnum;
    float zloc;
} Tslice;

#if defined(__osf__) || defined(__cplusplus)
typedef (*qsort_func)(const void *, const void *);
#endif
#if defined(__cplusplus)
extern void display_message(const char*);
#else
#define display_message(s) 
#endif

#if defined(__osf__) || defined(__cplusplus)
static int compare_zloc(const Tslice *a, const Tslice *b) 
#else
static int compare_zloc(a,b)
Tslice *a, *b;
#endif
{
    if (a->zloc < b->zloc) return(-1);
    else if (a->zloc > b->zloc) return (1);
    else return (0);
}
#if defined(__osf__) || defined(__cplusplus)
static int slice_sort(Tslice *slices, int count)
{
	qsort(slices, count, sizeof(Tslice), (qsort_func)compare_zloc);
	return 1;
}
#else
static int slice_sort(slices,count)
Tslice *slices;
int count;
{
	qsort(slices, count, sizeof(Tslice), compare_zloc);
	return 1;
}
#endif

#if defined(__osf__) || defined(__cplusplus)
static int load_slices(MatrixFile *matrix_file, MatrixData *volume,
Tslice *slice, int nslices, int cubic)
#else
static int load_slices(matrix_file,volume,slice,nslices, cubic)
MatrixFile *matrix_file;
MatrixData *volume;
Tslice *slice;
int nslices, cubic;
#endif
{
	int i, j, k, sz;
	MatrixData *s1, *s2;
	float fval;
	short *vp, *p1, *p2;
	int npixels, nvoxels;
	char cbufr[256];
	float  zsep,zloc, w1, w2, scalef = volume->scale_factor;


	zsep = matrix_file->mhptr->plane_separation;
	slice_sort( slice, nslices);
	if (cubic) sz = (int)(1+(0.5+slice[nslices-1].zloc/volume->pixel_size));
	else sz = nslices;
	volume->zdim = sz;
	npixels = volume->xdim*volume->ydim;
	nvoxels = npixels*volume->zdim;
	volume->data_ptr = (caddr_t)calloc(nvoxels,sizeof(short));
	vp = (short*)volume->data_ptr;
						/* set position to center */
	if (!vp)
	{
		sprintf( cbufr, "malloc failure for %d voxels...volume too large",
		 nvoxels);
		display_message(cbufr);
		return 0;
	}
	if (!cubic) {
		for (i=0; i<nslices; i++)
    	{
			s1 = matrix_read( matrix_file, slice[i].matnum, UnknownMatDataType);
			p1 = (short int*) s1->data_ptr;
			w1 = s1->scale_factor/scalef;
			for (k=0; k<npixels; k++)
				*vp++ = (short)(w1*(*p1++));
			free_matrix_data( s1);
    	}
		return 1;
	}
	j = 1;
	s1 = matrix_read( matrix_file, slice[i].matnum, UnknownMatDataType);
	s2 = matrix_read( matrix_file, slice[1].matnum, UnknownMatDataType);
	for (i=0; i<sz; i++)
	{
		zloc = i*volume->pixel_size;
		sprintf( cbufr, "Computing slice %d...(%0.2f cm)", i+1,zloc);
		display_message(cbufr);
		while (zloc > slice[j].zloc)
		{
			free_matrix_data( s1);
			s1 = s2;
			if (j < nslices-1)
				s2 = matrix_read(matrix_file, slice[++j].matnum,
					 UnknownMatDataType);
			else {		/*	plane overflow */
				slice[++j].zloc = slice[j-1].zloc+zsep;
				s2 = NULL;
				break;
			}
		}
		if (!s2) break;
		w2 = (zloc-slice[j-1].zloc)/(slice[j].zloc-slice[j-1].zloc);
		w1 = 1.0 - w2;
		w1 = w1*s1->scale_factor;
		w2 = w2*s2->scale_factor;
		p1 = (short int*) s1->data_ptr;
		p2 = (short int*) s2->data_ptr;
		for (k=0; k<npixels; k++) {
			fval = w1*(*p1++)+w2*(*p2++);
			*vp++ = (short)(fval/scalef);
		}
	}
	free_matrix_data( s1);
	if (s2 && s2 != s1) free_matrix_data(s2);
	return 1;
}

#if defined(__osf__) || defined(__cplusplus)
static int load_v_slices(MatrixFile *matrix_file, MatrixData *volume,
Tslice *slice) 
#else
static int load_v_slices(matrix_file, volume, slice) 
MatrixFile *matrix_file;
MatrixData *volume;
Tslice *slice; 
#endif
{
	MatrixData *v_slices;
	short *s1_data, *s2_data, *s_p1, *s_p2;
	u_char *b1_data, *b2_data, *b_p1, *b_p2;
	u_char *b_vp;
	float fval;
	short *vp;
	float zloc, w1, w2, zsep,scalef;
	uint i, j, k, sz;
	int npixels, nvoxels, nslices;
	char cbufr[256];

	zsep = matrix_file->mhptr->plane_separation;
	v_slices = matrix_read( matrix_file, slice[0].matnum, UnknownMatDataType);
	nslices = v_slices->zdim;
	if (volume->mat_type == Short3dSinogram)  {
		volume->scale_factor = v_slices->scale_factor;
		volume->data_max = v_slices->data_max;
	}
	scalef = volume->scale_factor;
	for (j=1; j<nslices; j++) slice[j].zloc = slice[0].zloc+zsep*j;
	slice_sort( slice, nslices);
	sz = volume->zdim = (int)(1+(0.5+slice[nslices-1].zloc/volume->pixel_size));
						/* set position to center */
	npixels = volume->xdim*volume->ydim;
	nvoxels = npixels*sz;
	if (volume->data_type == ByteData) 
		volume->data_ptr = (caddr_t)calloc(nvoxels,1);
	else volume->data_ptr = (caddr_t)calloc(nvoxels,sizeof(short));
	if (!volume->data_ptr)
	{
		sprintf( cbufr, "malloc failure for %d voxels...volume too large",
		 nvoxels);
		display_message(cbufr);
		return 0;
	}
	if (volume->data_type == ByteData) b_vp = (u_char*)volume->data_ptr;
	else vp = (short*)volume->data_ptr;
	j = 1;
	if (volume->data_type == ByteData) {
		b1_data = (u_char*)v_slices->data_ptr;
		b2_data = (u_char*)v_slices->data_ptr+npixels;
	} else {
		s1_data = (short*)v_slices->data_ptr;
		s2_data = (short*)v_slices->data_ptr+npixels;
	}
	for (i=0; i<sz; i++)
	{
		zloc = i*volume->pixel_size;
		sprintf( cbufr, "Computing slice %d... (%g)", i+1,zloc);
		display_message(cbufr);
		while (zloc > slice[j].zloc)
		{
			if (volume->data_type == ByteData) b1_data = b2_data;
			else s1_data = s2_data;
			if (j < nslices-1) {
				j++;
				if (volume->data_type == ByteData)
					b2_data = (u_char*)v_slices->data_ptr+npixels*j;
				else s2_data = (short*)v_slices->data_ptr+npixels*j;
			} else { 	/*	plane overflow */
				slice[++j].zloc = slice[j-1].zloc+zsep;
				b2_data = NULL; s2_data = NULL;
				break;
			}
		}
		if (!b2_data && !s2_data) break;		/* plane overflow */
		w2 = (zloc-slice[j-1].zloc)/(slice[j].zloc-slice[j-1].zloc);
		w1 = 1.0 - w2;
		w1 *= scalef;
		w2 *= scalef;
		if (volume->data_type == ByteData) {
			b_p1 = b1_data; b_p2 = b2_data;
			for (k=0; k<npixels; k++) {
				fval = w1*(*b_p1++)+w2*(*b_p2++);
				*b_vp++ = (u_char)(fval/scalef);
			}
		} else {
			s_p1 = s1_data; s_p2 = s2_data;
			for (k=0; k<npixels; k++) {
				fval = w1*(*s_p1++)+w2*(*s_p2++);
				*vp++ = (short)(fval/scalef);
			}
		}
	}
	free_matrix_data( v_slices);
	return 1;
}

#if  defined(__cplusplus)
static MatrixData *load_volume(MatrixFile *matrix_file,int frame, int cubic)
#else
static MatrixData *load_volume(matrix_file,frame, cubic)
MatrixFile *matrix_file;
int frame;
#endif
{
	int i=0, ret=0;
	MatrixData *mat;
	int matnum;
	float zsep,maxval;
	Main_header *mh;
	Image_subheader *imh = NULL;
	int nmats, plane, bed, nslices=0;
	float bed_pos[MAX_BED_POS];
	MatDirNode *entry;
	struct Matval matval;
	Tslice slice[MAX_SLICES];
	MatrixData *volume;

	volume = (MatrixData*)calloc(1,sizeof(MatrixData));
	mh = matrix_file->mhptr;
	volume->mat_type = (DataSetType)mh->file_type;
	if (volume->mat_type != Short3dSinogram) 
		imh = (Image_subheader*)calloc(1,sizeof(Image_subheader));
	memset(bed_pos,0,MAX_BED_POS*sizeof(float));

/* BED OFFSETS CODING ERROR IN ECAT FILES, position 1 not filled */
	for (i=2; i<mh->num_bed_pos; i++)
		bed_pos[i] = mh->bed_offset[i-2];
	if (mh->num_bed_pos>2) bed_pos[1] = bed_pos[2]/2;

	zsep = mh->plane_separation;
	nmats = matrix_file->dirlist->nmats;
	entry = matrix_file->dirlist->first;
	maxval = 0.0;
	for (i=0; i<nmats; i++,entry=entry->next)
	{
		matnum = entry->matnum;
		mat_numdoc( matnum, &matval);
		plane = matval.plane;
		bed = matval.bed;
		if (matval.frame != frame) continue;
		mat = matrix_read( matrix_file, matnum, MAT_SUB_HEADER);
		volume->pixel_size = mat->pixel_size;
		volume->xdim = mat->xdim;
		volume->ydim = mat->ydim;
		if (imh) memcpy(imh,mat->shptr,sizeof(Image_subheader));
		slice[nslices].matnum = matnum;
		slice[nslices].zloc = bed_pos[bed]+(plane-1)*zsep;
		if (mat->data_max > maxval) maxval = mat->data_max;
		volume->data_type =  mat->data_type;
		nslices++;
		free_matrix_data(mat);
	}
	if (nslices == 0)
	{
		fprintf( stderr, "ERROR...No slices selected\n");
		free_matrix_data(volume);
		return 0;
	}
	if (volume->data_type == ByteData) volume->scale_factor = maxval/255;
	else volume->scale_factor = maxval/32767;
	volume->data_max = maxval;
	if (imh) volume->shptr = (caddr_t)imh;
	if (nslices > 1) ret = load_slices(matrix_file,volume,slice,nslices, cubic);
	else {
		if (cubic) ret = load_v_slices(matrix_file,volume,slice);
		else {
			free_matrix_data(volume);
			return matrix_read(matrix_file, slice[0].matnum,UnknownMatDataType);
		}
	}
	if (!ret) {
		free_matrix_data(volume);
		return 0;
	}
	volume->y_size = volume->pixel_size;
	if (cubic) volume->z_size = volume->pixel_size;
	else volume->z_size = zsep;
	if (imh) {
		imh->num_dimensions = 3;
		imh->z_dimension = volume->zdim;
		imh->y_pixel_size = volume->y_size;
		imh->z_pixel_size = volume->z_size;
	}
	volume->data_size = volume->xdim*volume->ydim*volume->zdim;
	if (volume->data_type != ByteData) volume->data_size *= sizeof(short);
	volume->matnum = slice[0].matnum;
	return volume;
}
#endif
