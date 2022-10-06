/*  $Log: put_sa7.c,v $
 * Revision 1.3  2003/02/14  21:03:44  tom
 * fix storage_order, span
 *
 * Revision 1.2  2000/09/28  14:44:29  tom
 * calloc
 *
 * Revision 1.1  2000/01/20  21:29:59  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		put_sa7
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write attencor subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/put_sa7.c,v 1.3 2003/02/14 21:03:44 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             put_sa7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	short           sbuf[NUMREC_ASHDR * LENREC_S];

	union temp_long {
		float           f;
		short           s[2];
	}               conv;

	Attn_subheader7 *atn;
	int             matnum, begrec, endrec;
	int             i, j;
	int             pln1 = 1;
	int             ALEN = 24183;

	if (file->m7->atn == NULL) {
		(void) fprintf (stderr, "ERROR [put_sa7]: Version 7 subheader undefined for %s\n", file->name);
		return (ERROR);
	}
	atn = file->m7->atn;

	/* Shorts  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte ((char *) sbuf, NUMREC_ASHDR * LENREC);
		j = 1;
	} else
		j = 0;

	sbuf[0] = atn->data_type;
	sbuf[1] = atn->num_dimensions;
	sbuf[2] = atn->attenuation_type;
	sbuf[3] = atn->num_r_elements;
	sbuf[4] = atn->num_angles;
	sbuf[5] = atn->num_z_elements;
	sbuf[6] = atn->ring_difference;
	sbuf[35] = atn->num_additional_atten_coeff;
	sbuf[54] = atn->storage_order;
	sbuf[55] = atn->span;
	for (i = 0; i < 64; i++)
		sbuf[56 + i] = atn->z_elements[i];

	/* Longs and Floats	 */

	conv.f = atn->x_resolution;
	sbuf[7] = conv.s[0 + j];
	sbuf[8] = conv.s[1 + j];

	conv.f = atn->y_resolution;
	sbuf[9] = conv.s[0 + j];
	sbuf[10] = conv.s[1 + j];

	conv.f = atn->z_resolution;
	sbuf[11] = conv.s[0 + j];
	sbuf[12] = conv.s[1 + j];

	conv.f = atn->w_resolution;
	sbuf[13] = conv.s[0 + j];
	sbuf[14] = conv.s[1 + j];

	conv.f = atn->scale_factor;
	sbuf[15] = conv.s[0 + j];
	sbuf[16] = conv.s[1 + j];

	conv.f = atn->x_offset;
	sbuf[17] = conv.s[0 + j];
	sbuf[18] = conv.s[1 + j];

	conv.f = atn->y_offset;
	sbuf[19] = conv.s[0 + j];
	sbuf[20] = conv.s[1 + j];

	conv.f = atn->x_radius;
	sbuf[21] = conv.s[0 + j];
	sbuf[22] = conv.s[1 + j];

	conv.f = atn->y_radius;
	sbuf[23] = conv.s[0 + j];
	sbuf[24] = conv.s[1 + j];

	conv.f = atn->tilt_angle;
	sbuf[25] = conv.s[0 + j];
	sbuf[26] = conv.s[1 + j];

	conv.f = atn->attenuation_coeff;
	sbuf[27] = conv.s[0 + j];
	sbuf[28] = conv.s[1 + j];

	conv.f = atn->attenuation_min;
	sbuf[29] = conv.s[0 + j];
	sbuf[30] = conv.s[1 + j];

	conv.f = atn->attenuation_max;
	sbuf[31] = conv.s[0 + j];
	sbuf[32] = conv.s[1 + j];

	conv.f = atn->skull_thickness;
	sbuf[33] = conv.s[0 + j];
	sbuf[34] = conv.s[1 + j];

	for (i = 0; i < 8; i++)
		conv.f = atn->additional_atten_coeff[i];
	sbuf[36 + i] = conv.s[0 + j];
	sbuf[37 + i] = conv.s[1 - j];

	conv.f = atn->edge_finding_threshold;
	sbuf[52] = conv.s[0 + j];
	sbuf[53] = conv.s[1 - j];

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, ALEN)) < 0) {
			(void) fprintf (stderr, "ERROR [put_sa7]: Could not entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	if (wrtrec (file->fp, (char *) sbuf, begrec, NUMREC_ASHDR) == ERROR) {
		(void) fprintf (stderr, "ERROR [put_sa7]: writing subheader\n");
		return (ERROR);
	} else
		return (OK);
}
