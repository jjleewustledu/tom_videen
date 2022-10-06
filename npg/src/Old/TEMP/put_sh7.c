/*  $Id: put_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp tom $
    $Log: put_sh7.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/put_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             put_sh7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temporary {
		short           s[NUMREC_ISHDR * LENREC_S];
		char            c[NUMREC_ISHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		long            l;
		short           s[2];
	}               conv;

	Image_subheader7 *sh7;
	DataSetType     dtype;
	int             matnum, begrec, endrec;
	int             i, j;
	int             pln1 = 1;

	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		(void) fprintf (stderr, "ERROR [put_sh7]: Unsupported data type %d for %s\n", dtype, file->name);
		return (ERROR);
	}
	if (file->m7->sh == NULL) {
		(void) fprintf (stderr, "ERROR [put_sh7]: Version 7 subheader undefined for %s\n", file->name);
		return (ERROR);
	}
	sh7 = file->m7->sh;

	for (i = 0; i < NUMREC_ISHDR * LENREC_S; i++)
		tmp.s[i] = 0;

	/* CONSTCOND */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		j = 1;
	} else
		j = 0;

	/* Shorts	 */
	/* starts with byte 1025 or rec 3 */
	tmp.s[0] = sh7->data_type;
	tmp.s[1] = sh7->num_dimensions;
	tmp.s[2] = sh7->x_dimension;
	tmp.s[3] = sh7->y_dimension;
	tmp.s[4] = sh7->z_dimension;
	tmp.s[15] = sh7->image_min;
	tmp.s[16] = sh7->image_max;
	tmp.s[27] = sh7->filter_code;
	tmp.s[56] = sh7->filter_order;
	tmp.s[109] = sh7->zfilter_code;
	tmp.s[110] = sh7->zfilter_order;
	for (i = 0; i < 145; i++)
		tmp.s[111 + i] = sh7->reserved[i];

	/* Longs and Floats	 */

	conv.f = sh7->x_offset;
	tmp.s[5] = conv.s[0 + j];
	tmp.s[6] = conv.s[1 - j];

	conv.f = sh7->y_offset;
	tmp.s[7] = conv.s[0 + j];
	tmp.s[8] = conv.s[1 - j];

	conv.f = sh7->z_offset;
	tmp.s[9] = conv.s[0 + j];
	tmp.s[10] = conv.s[1 - j];

	conv.f = sh7->recon_zoom;
	tmp.s[11] = conv.s[0 + j];
	tmp.s[12] = conv.s[1 - j];

	conv.f = sh7->scale_factor;
	tmp.s[13] = conv.s[0 + j];
	tmp.s[14] = conv.s[1 - j];

	conv.f = sh7->x_pixel_size;
	tmp.s[17] = conv.s[0 + j];
	tmp.s[18] = conv.s[1 - j];

	conv.f = sh7->y_pixel_size;
	tmp.s[19] = conv.s[0 + j];
	tmp.s[20] = conv.s[1 - j];

	conv.f = sh7->z_pixel_size;
	tmp.s[21] = conv.s[0 + j];
	tmp.s[22] = conv.s[1 - j];

	conv.l = sh7->frame_duration;
	tmp.s[23] = conv.s[0 + j];
	tmp.s[24] = conv.s[1 - j];

	conv.l = sh7->frame_start_time;
	tmp.s[25] = conv.s[0 + j];
	tmp.s[26] = conv.s[1 - j];

	conv.f = sh7->x_resolution;
	tmp.s[28] = conv.s[0 + j];
	tmp.s[29] = conv.s[1 - j];

	conv.f = sh7->y_resolution;
	tmp.s[30] = conv.s[0 + j];
	tmp.s[31] = conv.s[1 - j];

	conv.f = sh7->z_resolution;
	tmp.s[32] = conv.s[0 + j];
	tmp.s[33] = conv.s[1 - j];

	conv.f = sh7->num_r_elements;
	tmp.s[34] = conv.s[0 + j];
	tmp.s[35] = conv.s[1 - j];

	conv.f = sh7->num_angles;
	tmp.s[36] = conv.s[0 + j];
	tmp.s[37] = conv.s[1 - j];

	conv.f = sh7->z_rotation_angle;
	tmp.s[38] = conv.s[0 + j];
	tmp.s[39] = conv.s[1 - j];

	conv.f = sh7->decay_corr_fctr;
	tmp.s[40] = conv.s[0 + j];
	tmp.s[41] = conv.s[1 - j];

	conv.l = sh7->processing_code;
	tmp.s[42] = conv.s[0 + j];
	tmp.s[43] = conv.s[1 - j];

	conv.l = sh7->gate_duration;
	tmp.s[44] = conv.s[0 + j];
	tmp.s[45] = conv.s[1 - j];

	conv.l = sh7->r_wave_offset;
	tmp.s[46] = conv.s[0 + j];
	tmp.s[47] = conv.s[1 - j];

	conv.l = sh7->num_accepted_beats;
	tmp.s[48] = conv.s[0 + j];
	tmp.s[49] = conv.s[1 - j];

	conv.f = sh7->filter_cutoff_frequency;
	tmp.s[50] = conv.s[0 + j];
	tmp.s[51] = conv.s[1 - j];

	conv.f = sh7->filter_resolution;
	tmp.s[52] = conv.s[0 + j];
	tmp.s[53] = conv.s[1 - j];

	conv.f = sh7->filter_ramp_slope;
	tmp.s[54] = conv.s[0 + j];
	tmp.s[55] = conv.s[1 - j];

	conv.f = sh7->filter_scatter_fraction;
	tmp.s[57] = conv.s[0 + j];
	tmp.s[58] = conv.s[1 - j];

	conv.f = sh7->filter_scatter_slope;
	tmp.s[59] = conv.s[0 + j];
	tmp.s[60] = conv.s[1 - j];

	conv.f = sh7->da_x_rotation_angle;
	tmp.s[81] = conv.s[0 + j];
	tmp.s[82] = conv.s[1 - j];

	conv.f = sh7->da_y_rotation_angle;
	tmp.s[83] = conv.s[0 + j];
	tmp.s[84] = conv.s[1 - j];

	conv.f = sh7->da_z_rotation_angle;
	tmp.s[85] = conv.s[0 + j];
	tmp.s[86] = conv.s[1 - j];

	conv.f = sh7->da_x_translation;
	tmp.s[87] = conv.s[0 + j];
	tmp.s[88] = conv.s[1 - j];

	conv.f = sh7->da_y_translation;
	tmp.s[89] = conv.s[0 + j];
	tmp.s[90] = conv.s[1 - j];

	conv.f = sh7->da_z_translation;
	tmp.s[91] = conv.s[0 + j];
	tmp.s[92] = conv.s[1 - j];

	conv.f = sh7->da_x_scale_factor;
	tmp.s[93] = conv.s[0 + j];
	tmp.s[94] = conv.s[1 - j];

	conv.f = sh7->da_y_scale_factor;
	tmp.s[95] = conv.s[0 + j];
	tmp.s[96] = conv.s[1 - j];

	conv.f = sh7->da_z_scale_factor;
	tmp.s[97] = conv.s[0 + j];
	tmp.s[98] = conv.s[1 - j];

	conv.f = sh7->rfilter_cutoff;
	tmp.s[99] = conv.s[0 + j];
	tmp.s[100] = conv.s[1 - j];

	conv.f = sh7->rfilter_resolution;
	tmp.s[101] = conv.s[0 + j];
	tmp.s[102] = conv.s[1 - j];

	tmp.s[103] = sh7->rfilter_code;
	tmp.s[104] = sh7->rfilter_order;

	conv.f = sh7->zfilter_cutoff;
	tmp.s[105] = conv.s[0 + j];
	tmp.s[106] = conv.s[1 - j];

	conv.f = sh7->zfilter_resolution;
	tmp.s[107] = conv.s[0 + j];
	tmp.s[108] = conv.s[1 - j];

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, NUMREC_ISHDR)) < 0) {
			(void) fprintf (stderr, "ERROR [put_sh7]: Could not create entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	/* CONSTCOND */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		swap_byte (tmp.c, NUMREC_ISHDR * LENREC);

	/* Strings	 */
	for (i = 0; i < 40; i++)
		tmp.c[122 + i] = sh7->annotation[i];

	if (wrtrec (file->fp, tmp.c, begrec, NUMREC_ISHDR) == ERROR) {
		(void) fprintf (stderr, "ERROR [put_sh7]: writing subheader\n");
		return (ERROR);
	} else
		return (OK);
}
