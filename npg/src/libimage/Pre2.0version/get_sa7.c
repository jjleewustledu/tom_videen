/*  $Log: get_sa7.c,v $
 * Revision 1.5  2003/07/01  19:14:19  tom
 * *** empty log message ***
 *
 * Revision 1.4  2003/02/14  21:03:44  tom
 * fix storage_order, span
 *
 * Revision 1.3  2000/09/28  18:17:35  tom
 * order always 1
 *
 * Revision 1.2  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		get_sa7
 *  Date:           Mar 2003
 *  Author:         Tom Videen (from JMO)
 *	Description:	Get Attenuation Correction Subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/Temp/RCS/get_sa7.c,v 1.5 2003/07/01 19:14:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             get_sa7 (file, indx)
	ImageFile      *file;
	Index          *indx;

{
	Attn_subheader7 *atn;

	short           sbuf[NUMREC_ASHDR * LENREC_S];

	union temp_long {
		float           f;
		short           s[2];
	}               conv;

	int             matnum, begrec, endrec;
	int             i, j, pln1 = 1;
	long            nbytes = NUMREC_ISHDR * LENREC;

	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [get_sa7]: no mainheader for %s\n", file->name);
		return (ERROR);
	}
	if (file->m7->atn == NULL) {
		if ((make_sa7 (file)) == ERROR) {
			fprintf (stderr, "ERROR [get_sa7]: calloc memory for atn\n");
			return (ERROR);
		}
	}
	atn = file->m7->atn;

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		fprintf (stderr, "ERROR [get_sa7]: Matrix #%x not found\n", matnum);
		return (ERROR);
	}
	if (rdrec (file->fp, (char *) sbuf, begrec, NUMREC_ASHDR) != 0) {
		fprintf (stderr, "ERROR [get_sa7]: reading %s\n", file->name);
		return (ERROR);
	}
	/* Shorts  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte ((char *) sbuf, nbytes);
		j = 1;
	} else
		j = 0;

	atn->data_type = sbuf[0];
	atn->num_dimensions = sbuf[1];
	atn->attenuation_type = sbuf[2];
	atn->num_r_elements = sbuf[3];
	atn->num_angles = sbuf[4];
	atn->num_z_elements = sbuf[5];
	atn->ring_difference = sbuf[6];
	atn->num_additional_atten_coeff = sbuf[35];
	atn->storage_order = sbuf[54];
	atn->span = sbuf[55];
	for (i = 0; i < 64; i++)
		atn->z_elements[i] = sbuf[56 + i];

	/* Floats */

	conv.s[0 + j] = sbuf[7];
	conv.s[1 - j] = sbuf[8];
	atn->x_resolution = conv.f;

	conv.s[0 + j] = sbuf[9];
	conv.s[1 - j] = sbuf[10];
	atn->y_resolution = conv.f;

	conv.s[0 + j] = sbuf[11];
	conv.s[1 - j] = sbuf[12];
	atn->z_resolution = conv.f;

	conv.s[0 + j] = sbuf[13];
	conv.s[1 - j] = sbuf[14];
	atn->w_resolution = conv.f;

	conv.s[0 + j] = sbuf[15];
	conv.s[1 - j] = sbuf[16];
	atn->scale_factor = conv.f;

	conv.s[0 + j] = sbuf[17];
	conv.s[1 - j] = sbuf[18];
	atn->x_offset = conv.f;

	conv.s[0 + j] = sbuf[19];
	conv.s[1 - j] = sbuf[20];
	atn->y_offset = conv.f;

	conv.s[0 + j] = sbuf[21];
	conv.s[1 - j] = sbuf[22];
	atn->x_radius = conv.f;

	conv.s[0 + j] = sbuf[23];
	conv.s[1 - j] = sbuf[24];
	atn->y_radius = conv.f;

	conv.s[0 + j] = sbuf[25];
	conv.s[1 - j] = sbuf[26];
	atn->tilt_angle = conv.f;

	conv.s[0 + j] = sbuf[27];
	conv.s[1 - j] = sbuf[28];
	atn->attenuation_coeff = conv.f;

	conv.s[0 + j] = sbuf[29];
	conv.s[1 - j] = sbuf[30];
	atn->attenuation_min = conv.f;

	conv.s[0 + j] = sbuf[31];
	conv.s[1 - j] = sbuf[32];
	atn->attenuation_max = conv.f;

	conv.s[0 + j] = sbuf[33];
	conv.s[1 - j] = sbuf[34];
	atn->skull_thickness = conv.f;

	for (i = 0; i < 8; i++) {
		conv.s[0 + j] = sbuf[36 + i];
		conv.s[1 - j] = sbuf[37 + i];
		atn->additional_atten_coeff[i] = conv.f;
	}

	conv.s[0 + j] = sbuf[52];
	conv.s[1 - j] = sbuf[53];
	atn->edge_finding_threshold = conv.f;

	return (OK);
}
