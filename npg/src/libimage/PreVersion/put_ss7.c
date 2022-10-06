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
 *  Date:           Dec-98
 *  Author:         Tom Videen (from JMO)
 *	Description:	Write scan subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/put_ss7.c,v 1.5 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             put_ss7 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temporary {
		float           f[NUMREC_SSHDR * LENREC_L];
		long            l[NUMREC_SSHDR * LENREC_L];
		short           s[NUMREC_SSHDR * LENREC_S];
		char            c[NUMREC_SSHDR * LENREC];
	}               tmp;

	Scan3D_subheader7 *s3d;
	int             len, matnum, begrec, endrec;
	int             i, pln1 = 1;
	int             VLEN = 12093;

	if (file->m7->s3d == NULL) {
		(void) fprintf (stderr, "ERROR [put_ss7]: Version 7 subheader undefined for %s\n", file->name);
		return (ERROR);
	}
	s3d = file->m7->s3d;
	tmp.s[0] = s3d->data_type;
	tmp.s[1] = s3d->num_dimensions;
	tmp.s[2] = s3d->num_r_elements;
	tmp.s[3] = s3d->num_angles;
	tmp.s[4] = s3d->corrections_applied;
	for (i = 0; i < 64; i++)
		tmp.s[5 + i] = s3d->num_z_elements[i];
	tmp.s[69] = s3d->ring_difference;
	tmp.s[70] = s3d->storage_order;
	tmp.s[71] = s3d->axial_compression;
	tmp.f[36] = s3d->x_resolution;
	tmp.f[37] = s3d->v_resolution;
	tmp.f[38] = s3d->z_resolution;
	tmp.f[39] = s3d->w_resolution;
	for (i = 0; i < 6; i++)
		tmp.s[80 + i] = s3d->reserved_gating[i];
	tmp.l[43] = s3d->gate_duration;
	tmp.l[44] = s3d->r_wave_offset;
	tmp.l[45] = s3d->num_accepted_beats;
	tmp.f[46] = s3d->scale_factor;
	tmp.s[94] = s3d->scan_min;
	tmp.s[95] = s3d->scan_max;
	tmp.l[48] = s3d->prompts;
	tmp.l[49] = s3d->delayed;
	tmp.l[50] = s3d->multiples;
	tmp.l[51] = s3d->net_trues;
	tmp.f[52] = s3d->tot_avg_cor;
	tmp.f[53] = s3d->tot_avg_uncor;
	tmp.l[54] = s3d->total_coin_rate;
	tmp.l[55] = s3d->frame_start_time;
	tmp.l[56] = s3d->frame_duration;
	tmp.f[57] = s3d->deadtime_correction_fctr;
	for (i = 0; i < 90; i++)
		tmp.s[116 + i] = s3d->reserved_cti[i];
	for (i = 0; i < 50; i++)
		tmp.s[206 + i] = s3d->reserved_cti[i];
	for (i = 0; i < 128; i++)
		tmp.f[128 + i] = s3d->uncor_singles[i];

	matnum = numcode7 (indx->frm, pln1, indx->gate, indx->data, indx->bed);
	len = NUMREC_SSHDR;
	if ((begrec = getmatpos (file, matnum, &endrec)) == ERROR) {
		if ((begrec = create_dir7 (file, matnum, VLEN)) < 0) {
			(void) fprintf (stderr, "ERROR [put_ss7]: Could not entry for matrix #%x\n", matnum);
			return (ERROR);
		}
	}
	if (wrtrec (file->fp, (char *) tmp.s, begrec, len) == ERROR)
		return (ERROR);
	else
		return (OK);
}
