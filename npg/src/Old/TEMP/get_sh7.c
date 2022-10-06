/*  $Id: get_sh7.c,v 1.2 2000/09/27 19:03:28 tom Exp tom $
    $Log: get_sh7.c,v $
 * Revision 1.2  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_sh7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Get Subheader of Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/get_sh7.c,v 1.2 2000/09/27 19:03:28 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             get_sh7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	Image_subheader7 *sh7;
	DataSetType     dtype;

	union temporary {
		short           s[NUMREC_ISHDR * LENREC_S];
		char            c[NUMREC_ISHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               conv;

	int             matnum, begrec, endrec;
	int             i, j;
	int             pln1 = 1;

	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [get_sh7]: no mainheader for %s\n", file->name);
		return (ERROR);
	}
	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		fprintf (stderr, "ERROR [get_sh7]: file_type %d not supported for %s\n", dtype, file->name);
		return (ERROR);
	}
	if (file->m7->sh == NULL) {
		if ((make_sh7 (file)) == ERROR) {
			fprintf (stderr, "ERROR [get_sh7]: Could not calloc memory for sh7\n");
			return (ERROR);
		}
	}
	sh7 = file->m7->sh;

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		fprintf (stderr, "ERROR [get_sh7]: Matrix #%x not found\n", matnum);
		return (ERROR);
	}
	if (rdrec (file->fp, tmp.c, begrec, NUMREC_ISHDR) != 0)
		return (ERROR);

	for (i = 0; i < 40; i++)
		sh7->annotation[i] = tmp.c[122 + i];

	/* Shorts  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte (tmp.c, NUMREC_ISHDR * LENREC);
		j = 1;
	} else
		j = 0;

	sh7->data_type = tmp.s[0];
	sh7->num_dimensions = tmp.s[1];
	sh7->x_dimension = tmp.s[2];
	sh7->y_dimension = tmp.s[3];
	sh7->z_dimension = tmp.s[4];
	sh7->image_min = tmp.s[15];
	sh7->image_max = tmp.s[16];
	sh7->filter_code = tmp.s[27];
	sh7->filter_order = tmp.s[56];
	sh7->rfilter_code = tmp.s[103];
	sh7->rfilter_order = tmp.s[104];
	sh7->zfilter_code = tmp.s[109];
	sh7->zfilter_order = tmp.s[110];
	for (i = 0; i < 145; i++)
		sh7->reserved[i] = tmp.s[111 + i];

	/* Longs and Floats */

	conv.s[0 + j] = tmp.s[5];
	conv.s[1 - j] = tmp.s[6];
	sh7->x_offset = conv.f;

	conv.s[0 + j] = tmp.s[7];
	conv.s[1 - j] = tmp.s[8];
	sh7->y_offset = conv.f;

	conv.s[0 + j] = tmp.s[9];
	conv.s[1 - j] = tmp.s[10];
	sh7->z_offset = conv.f;

	conv.s[0 + j] = tmp.s[11];
	conv.s[1 - j] = tmp.s[12];
	sh7->recon_zoom = conv.f;

	conv.s[0 + j] = tmp.s[13];
	conv.s[1 - j] = tmp.s[14];
	sh7->scale_factor = conv.f;

	conv.s[0 + j] = tmp.s[17];
	conv.s[1 - j] = tmp.s[18];
	sh7->x_pixel_size = conv.f;

	conv.s[0 + j] = tmp.s[19];
	conv.s[1 - j] = tmp.s[20];
	sh7->y_pixel_size = conv.f;

	conv.s[0 + j] = tmp.s[21];
	conv.s[1 - j] = tmp.s[22];
	sh7->z_pixel_size = conv.f;

	conv.s[0 + j] = tmp.s[23];
	conv.s[1 - j] = tmp.s[24];
	sh7->frame_duration = conv.l;

	conv.s[0 + j] = tmp.s[25];
	conv.s[1 - j] = tmp.s[26];
	sh7->frame_start_time = conv.l;

	conv.s[0 + j] = tmp.s[28];
	conv.s[1 - j] = tmp.s[29];
	sh7->x_resolution = conv.f;

	conv.s[0 + j] = tmp.s[30];
	conv.s[1 - j] = tmp.s[31];
	sh7->y_resolution = conv.f;

	conv.s[0 + j] = tmp.s[32];
	conv.s[1 - j] = tmp.s[33];
	sh7->z_resolution = conv.f;

	conv.s[0 + j] = tmp.s[34];
	conv.s[1 - j] = tmp.s[35];
	sh7->num_r_elements = conv.f;

	conv.s[0 + j] = tmp.s[36];
	conv.s[1 - j] = tmp.s[37];
	sh7->num_angles = conv.f;

	conv.s[0 + j] = tmp.s[38];
	conv.s[1 - j] = tmp.s[39];
	sh7->z_rotation_angle = conv.f;

	conv.s[0 + j] = tmp.s[40];
	conv.s[1 - j] = tmp.s[41];
	sh7->decay_corr_fctr = conv.f;

	conv.s[0 + j] = tmp.s[42];
	conv.s[1 - j] = tmp.s[43];
	sh7->processing_code = conv.l;

	conv.s[0 + j] = tmp.s[44];
	conv.s[1 - j] = tmp.s[45];
	sh7->gate_duration = conv.l;

	conv.s[0 + j] = tmp.s[46];
	conv.s[1 - j] = tmp.s[47];
	sh7->r_wave_offset = conv.l;

	conv.s[0 + j] = tmp.s[48];
	conv.s[1 - j] = tmp.s[49];
	sh7->num_accepted_beats = conv.l;

	conv.s[0 + j] = tmp.s[50];
	conv.s[1 - j] = tmp.s[51];
	sh7->filter_cutoff_frequency = conv.f;

	conv.s[0 + j] = tmp.s[52];
	conv.s[1 - j] = tmp.s[53];
	sh7->filter_resolution = conv.f;

	conv.s[0 + j] = tmp.s[54];
	conv.s[1 - j] = tmp.s[55];
	sh7->filter_ramp_slope = conv.f;

	conv.s[0 + j] = tmp.s[57];
	conv.s[1 - j] = tmp.s[58];
	sh7->filter_scatter_fraction = conv.f;

	conv.s[0 + j] = tmp.s[59];
	conv.s[1 - j] = tmp.s[60];
	sh7->filter_scatter_slope = conv.f;

	conv.s[0 + j] = tmp.s[81];
	conv.s[1 - j] = tmp.s[82];
	sh7->da_x_rotation_angle = conv.f;

	conv.s[0 + j] = tmp.s[83];
	conv.s[1 - j] = tmp.s[84];
	sh7->da_y_rotation_angle = conv.f;

	conv.s[0 + j] = tmp.s[85];
	conv.s[1 - j] = tmp.s[86];
	sh7->da_z_rotation_angle = conv.f;

	conv.s[0 + j] = tmp.s[87];
	conv.s[1 - j] = tmp.s[88];
	sh7->da_x_translation = conv.f;

	conv.s[0 + j] = tmp.s[89];
	conv.s[1 - j] = tmp.s[90];
	sh7->da_y_translation = conv.f;

	conv.s[0 + j] = tmp.s[91];
	conv.s[1 - j] = tmp.s[92];
	sh7->da_z_translation = conv.f;

	conv.s[0 + j] = tmp.s[93];
	conv.s[1 - j] = tmp.s[94];
	sh7->da_x_scale_factor = conv.f;

	conv.s[0 + j] = tmp.s[95];
	conv.s[1 - j] = tmp.s[96];
	sh7->da_y_scale_factor = conv.f;

	conv.s[0 + j] = tmp.s[97];
	conv.s[1 - j] = tmp.s[98];
	sh7->da_z_scale_factor = conv.f;

	conv.s[0 + j] = tmp.s[99];
	conv.s[1 - j] = tmp.s[100];
	sh7->rfilter_cutoff = conv.f;

	conv.s[0 + j] = tmp.s[101];
	conv.s[1 - j] = tmp.s[102];
	sh7->rfilter_resolution = conv.f;

	conv.s[0 + j] = tmp.s[105];
	conv.s[1 - j] = tmp.s[106];
	sh7->zfilter_cutoff = conv.f;

	conv.s[0 + j] = tmp.s[107];
	conv.s[1 - j] = tmp.s[108];
	sh7->zfilter_resolution = conv.f;
	return (OK);
}
