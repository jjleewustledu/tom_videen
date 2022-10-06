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
 *  Date:           Jan-00
 *  Author:         Tom Videen
 *	Description:	Write attencor subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/put_sa7.c,v 1.3 2003/02/14 21:03:44 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             put_sa7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temporary {
		float           f[NUMREC_ASHDR * LENREC_L];
		short           s[NUMREC_ASHDR * LENREC_S];
	}               tmp;

    union temp_long {
        float           f;
        short           s[2];
    }               tmpl;

	Attn_subheader7 *atn;
	int             len, matnum, begrec, endrec;
	int             i, pln1 = 1;
	int             ALEN = 24183;

	if (file->m7->atn == NULL) {
		(void) fprintf (stderr, "ERROR [put_sa7]: Version 7 subheader undefined for %s\n", file->name);
		return (ERROR);
	}
	atn = file->m7->atn;
	tmp.s[0] = atn->data_type;
	tmp.s[1] = atn->num_dimensions;
	tmp.s[2] = atn->attenuation_type;
	tmp.s[3] = atn->num_r_elements;
	tmp.s[4] = atn->num_angles;
	tmp.s[5] = atn->num_z_elements;
	tmp.s[6] = atn->ring_difference;

	tmpl.f =atn->x_resolution;
    tmp.s[7] = tmpl.s[0];
    tmp.s[8] = tmpl.s[1];

	tmpl.f =atn->y_resolution;
    tmp.s[9] = tmpl.s[0];
    tmp.s[10] = tmpl.s[1];

	tmpl.f =atn->z_resolution;
    tmp.s[11] = tmpl.s[0];
    tmp.s[12] = tmpl.s[1];

	tmpl.f =atn->w_resolution;
    tmp.s[13] = tmpl.s[0];
    tmp.s[14] = tmpl.s[1];

	tmpl.f =atn->scale_factor;
    tmp.s[15] = tmpl.s[0];
    tmp.s[16] = tmpl.s[1];

	tmpl.f =atn->x_offset;
    tmp.s[17] = tmpl.s[0];
    tmp.s[18] = tmpl.s[1];

	tmpl.f =atn->y_offset;
    tmp.s[19] = tmpl.s[0];
    tmp.s[20] = tmpl.s[1];

	tmpl.f =atn->x_radius;
    tmp.s[21] = tmpl.s[0];
    tmp.s[22] = tmpl.s[1];

	tmpl.f =atn->y_radius;
    tmp.s[23] = tmpl.s[0];
    tmp.s[24] = tmpl.s[1];

	tmpl.f =atn->tilt_angle;
    tmp.s[25] = tmpl.s[0];
    tmp.s[26] = tmpl.s[1];

	tmpl.f =atn->attenuation_coeff;
    tmp.s[27] = tmpl.s[0];
    tmp.s[28] = tmpl.s[1];

	tmpl.f =atn->attenuation_min;
    tmp.s[29] = tmpl.s[0];
    tmp.s[30] = tmpl.s[1];

	tmpl.f =atn->attenuation_max;
    tmp.s[31] = tmpl.s[0];
    tmp.s[32] = tmpl.s[1];

	tmpl.f =atn->skull_thickness;
    tmp.s[33] = tmpl.s[0];
    tmp.s[34] = tmpl.s[1];

	tmp.s[35] = atn->num_additional_atten_coeff;
	for (i = 0; i < 8; i++) 
		tmp.f[18 + i] = atn->additional_atten_coeff[i];
	tmp.f[26] = atn->edge_finding_threshold;
	tmp.s[54] = atn->storage_order;
	tmp.s[55] = atn->span;
	for (i = 0; i < 64; i++)
		tmp.s[56 + i] = atn->z_elements[i];

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	len = NUMREC_ASHDR;
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, ALEN)) < 0) {
			(void) fprintf (stderr, "ERROR [put_sa7]: Could not entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	if (wrtrec (file->fp, (char *) tmp.s, begrec, len) == ERROR)
		return (ERROR);
	else
		return (OK);
}
