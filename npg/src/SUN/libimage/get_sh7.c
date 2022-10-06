/*  $Id: get_sh7.c,v 1.2 2000/09/27 19:03:28 tom Exp $
    $Log: get_sh7.c,v $
    Revision 1.2  2000/09/27 19:03:28  tom
    *** empty log message ***

 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_sh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get Subheader of Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_sh7.c,v 1.2 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_sh7 (file, indx)
	ImageFile      *file;
	Index          *indx;

{
	Image_subheader7 *sh7;
	DataSetType     dtype;

	union temporary {
		float           f[NUMREC_ISHDR * LENREC_S / 2];
		long            l[NUMREC_ISHDR * LENREC_S / 2];
		short           s[NUMREC_ISHDR * LENREC_S];
		char            c[NUMREC_ISHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               tmpl;

	int             lenrd, matnum, begrec, endrec;
	int             i;
	int             pln1 = 1;

	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [get_sh7]: no mainheader for %s\n", file->name);
		return (ERROR);
	}
	dtype = (DataSetType) file->m7->mh->file_type;
	if (dtype != PetVolume) {
		fprintf (stderr, "ERROR [get_sh7]: file type %d not supported for %s\n", dtype, file->name);
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
	lenrd = NUMREC_ISHDR;
	if (rdrec (file->fp, (char *) tmp.s, begrec, lenrd) != 0)
		return (ERROR);

	sh7->data_type = tmp.s[0];
	sh7->num_dimensions = tmp.s[1];
	sh7->x_dimension = tmp.s[2];
	sh7->y_dimension = tmp.s[3];
	sh7->z_dimension = tmp.s[4];

	tmpl.s[0] = tmp.s[5];
	tmpl.s[1] = tmp.s[6];
	sh7->x_offset = tmpl.f;

	tmpl.s[0] = tmp.s[7];
	tmpl.s[1] = tmp.s[8];
	sh7->y_offset = tmpl.f;

	tmpl.s[0] = tmp.s[9];
	tmpl.s[1] = tmp.s[10];
	sh7->z_offset = tmpl.f;

	tmpl.s[0] = tmp.s[11];
	tmpl.s[1] = tmp.s[12];
	sh7->recon_zoom = tmpl.f;

	tmpl.s[0] = tmp.s[13];
	tmpl.s[1] = tmp.s[14];
	sh7->scale_factor = tmpl.f;

	sh7->image_min = tmp.s[15];
	sh7->image_max = tmp.s[16];

	tmpl.s[0] = tmp.s[17];
	tmpl.s[1] = tmp.s[18];
	sh7->x_pixel_size = tmpl.f;

	tmpl.s[0] = tmp.s[19];
	tmpl.s[1] = tmp.s[20];
	sh7->y_pixel_size = tmpl.f;

	tmpl.s[0] = tmp.s[21];
	tmpl.s[1] = tmp.s[22];
	sh7->z_pixel_size = tmpl.f;

	tmpl.s[0] = tmp.s[23];
	tmpl.s[1] = tmp.s[24];
	sh7->frame_duration = tmpl.l;

	tmpl.s[0] = tmp.s[25];
	tmpl.s[1] = tmp.s[26];
	sh7->frame_start_time = tmpl.l;

	sh7->filter_code = tmp.s[27];

	tmpl.s[0] = tmp.s[28];
	tmpl.s[1] = tmp.s[29];
	sh7->x_resolution = tmpl.f;

	tmpl.s[0] = tmp.s[30];
	tmpl.s[1] = tmp.s[31];
	sh7->y_resolution = tmpl.f;

	tmpl.s[0] = tmp.s[32];
	tmpl.s[1] = tmp.s[33];
	sh7->z_resolution = tmpl.f;

	tmpl.s[0] = tmp.s[34];
	tmpl.s[1] = tmp.s[35];
	sh7->num_r_elements = tmpl.f;

	tmpl.s[0] = tmp.s[36];
	tmpl.s[1] = tmp.s[37];
	sh7->num_angles = tmpl.f;

	tmpl.s[0] = tmp.s[38];
	tmpl.s[1] = tmp.s[39];
	sh7->z_rotation_angle = tmpl.f;

	tmpl.s[0] = tmp.s[40];
	tmpl.s[1] = tmp.s[41];
	sh7->decay_corr_fctr = tmpl.f;

	tmpl.s[0] = tmp.s[42];
	tmpl.s[1] = tmp.s[43];
	sh7->processing_code = tmpl.l;

	tmpl.s[0] = tmp.s[44];
	tmpl.s[1] = tmp.s[45];
	sh7->gate_duration = tmpl.l;

	tmpl.s[0] = tmp.s[46];
	tmpl.s[1] = tmp.s[47];
	sh7->r_wave_offset = tmpl.l;

	tmpl.s[0] = tmp.s[48];
	tmpl.s[1] = tmp.s[49];
	sh7->num_accepted_beats = tmpl.l;

	tmpl.s[0] = tmp.s[50];
	tmpl.s[1] = tmp.s[51];
	sh7->filter_cutoff_frequency = tmpl.f;

	tmpl.s[0] = tmp.s[52];
	tmpl.s[1] = tmp.s[53];
	sh7->filter_resolution = tmpl.f;

	tmpl.s[0] = tmp.s[54];
	tmpl.s[1] = tmp.s[55];
	sh7->filter_ramp_slope = tmpl.f;

	sh7->filter_order = tmp.s[56];

	tmpl.s[0] = tmp.s[57];
	tmpl.s[1] = tmp.s[58];
	sh7->filter_scatter_fraction = tmpl.f;

	tmpl.s[0] = tmp.s[59];
	tmpl.s[1] = tmp.s[60];
	sh7->filter_scatter_slope = tmpl.f;

	for (i = 0; i < 40; i++)
		sh7->annotation[i] = tmp.c[122 + i];

	tmpl.s[0] = tmp.s[81];
	tmpl.s[1] = tmp.s[82];
	sh7->da_x_rotation_angle = tmpl.f;

	tmpl.s[0] = tmp.s[83];
	tmpl.s[1] = tmp.s[84];
	sh7->da_y_rotation_angle = tmpl.f;

	tmpl.s[0] = tmp.s[85];
	tmpl.s[1] = tmp.s[86];
	sh7->da_z_rotation_angle = tmpl.f;

	tmpl.s[0] = tmp.s[87];
	tmpl.s[1] = tmp.s[88];
	sh7->da_x_translation = tmpl.f;

	tmpl.s[0] = tmp.s[89];
	tmpl.s[1] = tmp.s[90];
	sh7->da_y_translation = tmpl.f;

	tmpl.s[0] = tmp.s[91];
	tmpl.s[1] = tmp.s[92];
	sh7->da_z_translation = tmpl.f;

	tmpl.s[0] = tmp.s[93];
	tmpl.s[1] = tmp.s[94];
	sh7->da_x_scale_factor = tmpl.f;

	tmpl.s[0] = tmp.s[95];
	tmpl.s[1] = tmp.s[96];
	sh7->da_y_scale_factor = tmpl.f;

	tmpl.s[0] = tmp.s[97];
	tmpl.s[1] = tmp.s[98];
	sh7->da_z_scale_factor = tmpl.f;

	tmpl.s[0] = tmp.s[99];
	tmpl.s[1] = tmp.s[100];
	sh7->rfilter_cutoff = tmpl.f;

	tmpl.s[0] = tmp.s[101];
	tmpl.s[1] = tmp.s[102];
	sh7->rfilter_resolution = tmpl.f;

	sh7->rfilter_code = tmp.s[103];
	sh7->rfilter_order = tmp.s[104];

	tmpl.s[0] = tmp.s[105];
	tmpl.s[1] = tmp.s[106];
	sh7->zfilter_cutoff = tmpl.f;

	tmpl.s[0] = tmp.s[107];
	tmpl.s[1] = tmp.s[108];
	sh7->zfilter_resolution = tmpl.f;

	sh7->zfilter_code = tmp.s[109];
	sh7->zfilter_order = tmp.s[110];

	for (i = 0; i < 145; i++)
		sh7->reserved[i] = tmp.s[111 + i];
	return (OK);
}
