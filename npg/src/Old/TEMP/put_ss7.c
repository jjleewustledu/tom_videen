/*  $Log: put_ss7.c,v $
 * Revision 1.5  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1998/12/16  21:50:00  tom
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/15  14:52:22  tom
 * debugged version
 *
 * Revision 1.1  1998/12/10  18:41:53  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		put_ss7
 *  Date:           Mar 2003
 *  Author:         Tom Videen (from JMO)
 *	Description:	Write scan subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/put_ss7.c,v 1.5 2000/09/27 19:03:28 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             put_ss7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               conv;

	Scan3D_subheader7 *s3d;

	short           sbuf[NUMREC_SSHDR * LENREC_S];
	int             matnum, begrec, endrec;
	int             i, j;
	int             pln1 = 1;
	int             VLEN = 12093;

	if (file->m7->s3d == NULL) {
		(void) fprintf (stderr, "ERROR [put_ss7]: Version 7 subheader undefined for %s\n", file->name);
		return (ERROR);
	}
	s3d = file->m7->s3d;

	/* Shorts  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte ((char *) sbuf, NUMREC_SSHDR * LENREC);
		j = 1;
	} else
		j = 0;

	sbuf[0] = s3d->data_type;
	sbuf[1] = s3d->num_dimensions;
	sbuf[2] = s3d->num_r_elements;
	sbuf[3] = s3d->num_angles;
	sbuf[4] = s3d->corrections_applied;
	for (i = 0; i < 64; i++)
		sbuf[5 + i] = s3d->num_z_elements[i];
	sbuf[69] = s3d->ring_difference;
	sbuf[70] = s3d->storage_order;
	sbuf[71] = s3d->axial_compression;
	for (i = 0; i < 6; i++)
		sbuf[80 + i] = s3d->reserved_gating[i];
	sbuf[94] = s3d->scan_min;
	sbuf[95] = s3d->scan_max;
	for (i = 0; i < 90; i++)
		sbuf[116 + i] = s3d->reserved_cti[i];
	for (i = 0; i < 50; i++)
		sbuf[206 + i] = s3d->reserved_cti[i];

	/* Longs and Floats	 */

	conv.f = s3d->x_resolution;
	sbuf[72] = conv.s[0 + j];
	sbuf[73] = conv.s[1 + j];

	conv.f = s3d->v_resolution;
	sbuf[74] = conv.s[0 + j];
	sbuf[75] = conv.s[1 + j];

	conv.f = s3d->z_resolution;
	sbuf[76] = conv.s[0 + j];
	sbuf[77] = conv.s[1 + j];

	conv.f = s3d->w_resolution;
	sbuf[78] = conv.s[0 + j];
	sbuf[79] = conv.s[1 + j];

	conv.l = s3d->gate_duration;
	sbuf[86] = conv.s[0 + j];
	sbuf[87] = conv.s[1 + j];

	conv.l = s3d->r_wave_offset;
	sbuf[88] = conv.s[0 + j];
	sbuf[89] = conv.s[1 + j];

	conv.l = s3d->num_accepted_beats;
	sbuf[90] = conv.s[0 + j];
	sbuf[91] = conv.s[1 + j];

	conv.f = s3d->scale_factor;
	sbuf[92] = conv.s[0 + j];
	sbuf[93] = conv.s[1 + j];

	conv.l = s3d->prompts;
	sbuf[96] = conv.s[0 + j];
	sbuf[97] = conv.s[1 + j];

	conv.l = s3d->delayed;
	sbuf[98] = conv.s[0 + j];
	sbuf[99] = conv.s[1 + j];

	conv.l = s3d->multiples;
	sbuf[100] = conv.s[0 + j];
	sbuf[101] = conv.s[1 + j];

	conv.l = s3d->net_trues;
	sbuf[102] = conv.s[0 + j];
	sbuf[103] = conv.s[1 + j];

	conv.f = s3d->tot_avg_cor;
	sbuf[104] = conv.s[0 + j];
	sbuf[105] = conv.s[1 + j];

	conv.f = s3d->tot_avg_uncor;
	sbuf[106] = conv.s[0 + j];
	sbuf[107] = conv.s[1 + j];

	conv.l = s3d->total_coin_rate;
	sbuf[108] = conv.s[0 + j];
	sbuf[109] = conv.s[1 + j];

	conv.l = s3d->frame_start_time;
	sbuf[110] = conv.s[0 + j];
	sbuf[111] = conv.s[1 + j];

	conv.l = s3d->frame_duration;
	sbuf[112] = conv.s[0 + j];
	sbuf[113] = conv.s[1 + j];

	conv.f = s3d->deadtime_correction_fctr;
	sbuf[114] = conv.s[0 + j];
	sbuf[115] = conv.s[1 + j];

	for (i = 0; i < 128; i++) {
		conv.f = s3d->uncor_singles[i];
		sbuf[256 + i] = conv.s[0 + j];
		sbuf[257 + i] = conv.s[1 + j];
	}

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, VLEN)) < 0) {
			(void) fprintf (stderr, "ERROR [put_ss7]: Could not entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	if (wrtrec (file->fp, (char *) sbuf, begrec, NUMREC_SSHDR) == ERROR)
		return (ERROR);
	else
		return (OK);
}
