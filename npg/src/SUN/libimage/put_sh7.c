/*  $Id: put_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $
    $Log: put_sh7.c,v $
    Revision 1.2  1999/12/13 17:45:50  tom
    tov revision

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
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/put_sh7.c,v 1.2 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             put_sh7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temporary {
		float           f[NUMREC_ISHDR * LENREC_S / 2];
		long            l[NUMREC_ISHDR * LENREC_S / 2];
		short           s[NUMREC_ISHDR * LENREC_S];
		char            c[NUMREC_ISHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		long            l;
		short           s[2];
	}               tmpl;

	Image_subheader7 *sh7;
	DataSetType     dtype;
	int             len, matnum, begrec, endrec;
	int             i, pln1 = 1;

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
	tmp.s[0] = sh7->data_type;
	tmp.s[1] = sh7->num_dimensions;
	tmp.s[2] = sh7->x_dimension;
	tmp.s[3] = sh7->y_dimension;
	tmp.s[4] = sh7->z_dimension;

	tmpl.f = sh7->x_offset;
	tmp.s[5] = tmpl.s[0];
	tmp.s[6] = tmpl.s[1];

	tmpl.f = sh7->y_offset;
	tmp.s[7] = tmpl.s[0];
	tmp.s[8] = tmpl.s[1];

	tmpl.f = sh7->z_offset;
	tmp.s[9] = tmpl.s[0];
	tmp.s[10] = tmpl.s[1];

	tmpl.f = sh7->recon_zoom;
	tmp.s[11] = tmpl.s[0];
	tmp.s[12] = tmpl.s[1];

	tmpl.f = sh7->scale_factor;
	tmp.s[13] = tmpl.s[0];
	tmp.s[14] = tmpl.s[1];

	tmp.s[15] = sh7->image_min;
	tmp.s[16] = sh7->image_max;

	tmpl.f = sh7->x_pixel_size;
	tmp.s[17] = tmpl.s[0];
	tmp.s[18] = tmpl.s[1];

	tmpl.f = sh7->y_pixel_size;
	tmp.s[19] = tmpl.s[0];
	tmp.s[20] = tmpl.s[1];

	tmpl.f = sh7->z_pixel_size;
	tmp.s[21] = tmpl.s[0];
	tmp.s[22] = tmpl.s[1];

	tmpl.l = sh7->frame_duration;
	tmp.s[23] = tmpl.s[0];
	tmp.s[24] = tmpl.s[1];

	tmpl.l = sh7->frame_start_time;
	tmp.s[25] = tmpl.s[0];
	tmp.s[26] = tmpl.s[1];

	tmp.s[27] = sh7->filter_code;

	tmpl.f = sh7->x_resolution;
	tmp.s[28] = tmpl.s[0];
	tmp.s[29] = tmpl.s[1];

	tmpl.f = sh7->y_resolution;
	tmp.s[30] = tmpl.s[0];
	tmp.s[31] = tmpl.s[1];

	tmpl.f = sh7->z_resolution;
	tmp.s[32] = tmpl.s[0];
	tmp.s[33] = tmpl.s[1];

	tmpl.f = sh7->num_r_elements;
	tmp.s[34] = tmpl.s[0];
	tmp.s[35] = tmpl.s[1];

	tmpl.f = sh7->num_angles;
	tmp.s[36] = tmpl.s[0];
	tmp.s[37] = tmpl.s[1];

	tmpl.f = sh7->z_rotation_angle;
	tmp.s[38] = tmpl.s[0];
	tmp.s[39] = tmpl.s[1];

	tmpl.f = sh7->decay_corr_fctr;
	tmp.s[40] = tmpl.s[0];
	tmp.s[41] = tmpl.s[1];

	tmpl.l = sh7->processing_code;
	tmp.s[42] = tmpl.s[0];
	tmp.s[43] = tmpl.s[1];

	tmpl.l = sh7->gate_duration;
	tmp.s[44] = tmpl.s[0];
	tmp.s[45] = tmpl.s[1];

	tmpl.l = sh7->r_wave_offset;
	tmp.s[46] = tmpl.s[0];
	tmp.s[47] = tmpl.s[1];

	tmpl.l = sh7->num_accepted_beats;
	tmp.s[48] = tmpl.s[0];
	tmp.s[49] = tmpl.s[1];

	tmpl.f = sh7->filter_cutoff_frequency;
	tmp.s[50] = tmpl.s[0];
	tmp.s[51] = tmpl.s[1];

	tmpl.f = sh7->filter_resolution;
	tmp.s[52] = tmpl.s[0];
	tmp.s[53] = tmpl.s[1];

	tmpl.f = sh7->filter_ramp_slope;
	tmp.s[54] = tmpl.s[0];
	tmp.s[55] = tmpl.s[1];

	tmp.s[56] = sh7->filter_order;

	tmpl.f = sh7->filter_scatter_fraction;
	tmp.s[57] = tmpl.s[0];
	tmp.s[58] = tmpl.s[1];

	tmpl.f = sh7->filter_scatter_slope;
	tmp.s[59] = tmpl.s[0];
	tmp.s[60] = tmpl.s[1];

	for (i = 0; i < 40; i++)
		tmp.c[122 + i] = sh7->annotation[i];

	tmpl.f = sh7->da_x_rotation_angle;
	tmp.s[81] = tmpl.s[0];
	tmp.s[82] = tmpl.s[1];

	tmpl.f = sh7->da_y_rotation_angle;
	tmp.s[83] = tmpl.s[0];
	tmp.s[84] = tmpl.s[1];

	tmpl.f = sh7->da_z_rotation_angle;
	tmp.s[85] = tmpl.s[0];
	tmp.s[86] = tmpl.s[1];

	tmpl.f = sh7->da_x_translation;
	tmp.s[87] = tmpl.s[0];
	tmp.s[88] = tmpl.s[1];

	tmpl.f = sh7->da_y_translation;
	tmp.s[89] = tmpl.s[0];
	tmp.s[90] = tmpl.s[1];

	tmpl.f = sh7->da_z_translation;
	tmp.s[91] = tmpl.s[0];
	tmp.s[92] = tmpl.s[1];

	tmpl.f = sh7->da_x_scale_factor;
	tmp.s[93] = tmpl.s[0];
	tmp.s[94] = tmpl.s[1];

	tmpl.f = sh7->da_y_scale_factor;
	tmp.s[95] = tmpl.s[0];
	tmp.s[96] = tmpl.s[1];

	tmpl.f = sh7->da_z_scale_factor;
	tmp.s[97] = tmpl.s[0];
	tmp.s[98] = tmpl.s[1];

	tmpl.f = sh7->rfilter_cutoff;
	tmp.s[99] = tmpl.s[0];
	tmp.s[100] = tmpl.s[1];

	tmpl.f = sh7->rfilter_resolution;
	tmp.s[101] = tmpl.s[0];
	tmp.s[102] = tmpl.s[1];

	tmp.s[103] = sh7->rfilter_code;
	tmp.s[104] = sh7->rfilter_order;

	tmpl.f = sh7->zfilter_cutoff;
	tmp.s[105] = tmpl.s[0];
	tmp.s[106] = tmpl.s[1];

	tmpl.f = sh7->zfilter_resolution;
	tmp.s[107] = tmpl.s[0];
	tmp.s[108] = tmpl.s[1];

	tmp.s[109] = sh7->zfilter_code;
	tmp.s[110] = sh7->zfilter_order;

	for (i = 0; i < 145; i++)
		tmp.s[111 + i] = sh7->reserved[i];

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	len = NUMREC_ISHDR;
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, len)) < 0) {
			(void) fprintf (stderr, "ERROR [put_sh7]: Could not entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	if (wrtrec (file->fp, (char *) tmp.s, begrec, len) == ERROR)
		return (ERROR);
	else
		return (OK);
}
