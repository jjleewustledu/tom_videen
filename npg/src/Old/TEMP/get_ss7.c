/*  $Log: get_ss7.c,v $
 * Revision 1.4  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/16  21:49:53  tom
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/15  14:52:22  tom
 * debugged version
 *
 * Revision 1.1  1998/12/10  18:41:44  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		get_ss7
 *  Date:           Mar 2003
 *  Author:         Tom Videen (from JMO)
 *	Description:	Get 3D Sinogram Subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/get_ss7.c,v 1.4 2000/09/27 19:03:28 tom Exp tom $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>
#include <endian.h>

int             get_ss7 (file, indx)
	ImageFile      *file;
	Index          *indx;

{
	Scan3D_subheader7 *s3d;
	short           sbuf[NUMREC_SSHDR * LENREC_S];

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               conv;

	int             matnum, begrec, endrec;
	int             i, j, pln1 = 1;

	if (file->m7 == NULL) {
		fprintf (stderr, "ERROR [get_ss7]: no mainheader for %s\n", file->name);
		return (ERROR);
	}
	if (file->m7->s3d == NULL) {
		if ((make_ss7 (file)) == ERROR) {
			fprintf (stderr, "ERROR [get_ss7]: Could not calloc memory for s3d\n");
			return (ERROR);
		}
	}
	s3d = file->m7->s3d;

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		fprintf (stderr, "ERROR [get_ss7]: Matrix #%x not found\n", matnum);
		return (ERROR);
	}
	if (rdrec (file->fp, (char *) sbuf, begrec, NUMREC_SSHDR) != 0)
		return (ERROR);

	/* Shorts  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte ((char *) sbuf, NUMREC_SSHDR * LENREC);
		j = 1;
	} else
		j = 0;

	s3d->data_type = sbuf[0];
	s3d->num_dimensions = sbuf[1];
	s3d->num_r_elements = sbuf[2];
	s3d->num_angles = sbuf[3];
	s3d->corrections_applied = sbuf[4];
	for (i = 0; i < 64; i++)
		s3d->num_z_elements[i] = sbuf[5 + i];
	s3d->ring_difference = sbuf[69];
	s3d->storage_order = sbuf[70];
	s3d->axial_compression = sbuf[71];
	for (i = 0; i < 6; i++)
		s3d->reserved_gating[i] = sbuf[80 + i];
	s3d->scan_min = sbuf[94];
	s3d->scan_max = sbuf[95];
	for (i = 0; i < 90; i++)
		s3d->reserved_cti[i] = sbuf[116 + i];
	for (i = 0; i < 50; i++)
		s3d->reserved_cti[i] = sbuf[206 + i];

	/* Longs and Floats */

	conv.s[0 + j] = sbuf[72];
	conv.s[1 - j] = sbuf[73];
	s3d->x_resolution = conv.f;

	conv.s[0 + j] = sbuf[74];
	conv.s[1 - j] = sbuf[75];
	s3d->v_resolution = conv.f;

	conv.s[0 + j] = sbuf[76];
	conv.s[1 - j] = sbuf[77];
	s3d->z_resolution = conv.f;

	conv.s[0 + j] = sbuf[78];
	conv.s[1 - j] = sbuf[79];
	s3d->w_resolution = conv.f;

	conv.s[0 + j] = sbuf[86];
	conv.s[1 - j] = sbuf[87];
	s3d->gate_duration = conv.l;

	conv.s[0 + j] = sbuf[88];
	conv.s[1 - j] = sbuf[89];
	s3d->r_wave_offset = conv.l;

	conv.s[0 + j] = sbuf[90];
	conv.s[1 - j] = sbuf[91];
	s3d->num_accepted_beats = conv.l;

	conv.s[0 + j] = sbuf[92];
	conv.s[1 - j] = sbuf[93];
	s3d->scale_factor = conv.f;

	conv.s[0 + j] = sbuf[96];
	conv.s[1 - j] = sbuf[97];
	s3d->prompts = conv.l;

	conv.s[0 + j] = sbuf[98];
	conv.s[1 - j] = sbuf[99];
	s3d->delayed = conv.l;

	conv.s[0 + j] = sbuf[100];
	conv.s[1 - j] = sbuf[101];
	s3d->multiples = conv.l;

	conv.s[0 + j] = sbuf[102];
	conv.s[1 - j] = sbuf[103];
	s3d->net_trues = conv.l;

	conv.s[0 + j] = sbuf[104];
	conv.s[1 - j] = sbuf[105];
	s3d->tot_avg_cor = conv.f;

	conv.s[0 + j] = sbuf[106];
	conv.s[1 - j] = sbuf[107];
	s3d->tot_avg_uncor = conv.f;

	conv.s[0 + j] = sbuf[108];
	conv.s[1 - j] = sbuf[109];
	s3d->total_coin_rate = conv.l;

	conv.s[0 + j] = sbuf[110];
	conv.s[1 - j] = sbuf[111];
	s3d->frame_start_time = conv.l;

	conv.s[0 + j] = sbuf[112];
	conv.s[1 - j] = sbuf[113];
	s3d->frame_duration = conv.l;

	conv.s[0 + j] = sbuf[114];
	conv.s[1 - j] = sbuf[115];
	s3d->deadtime_correction_fctr = conv.f;

	for (i = 0; i < 128; i++) {
		conv.s[0 + j] = sbuf[256 + i];
		conv.s[1 - j] = sbuf[257 + i];
		s3d->uncor_singles[i] = conv.f;
	}

	return (OK);
}
