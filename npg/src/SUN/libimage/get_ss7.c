/*  $Log: get_ss7.c,v $
 *  Revision 1.4  2000/09/27 19:03:28  tom
 *  *** empty log message ***
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
 *  Date:           Dec-98
 *  Author:         Tom Videen (from JMO)
 *	Description:	Get 3D Sinogram Subheader for Version 7 file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_ss7.c,v 1.4 2000/09/27 19:03:28 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_ss7 (file, indx)
	ImageFile      *file;
	Index          *indx;

{
	Scan3D_subheader7 *s3d;

	union temporary {
		float           f[NUMREC_SSHDR * LENREC_L];
		long            l[NUMREC_SSHDR * LENREC_L];
		short           s[NUMREC_SSHDR * LENREC_S];
		char            c[NUMREC_SSHDR * LENREC];
	}               tmp;

	int             lenrd, matnum, begrec, endrec;
	int             i, pln1 = 1;

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
	lenrd = NUMREC_SSHDR;
	if (rdrec (file->fp, (char *) tmp.s, begrec, lenrd) != 0)
		return (ERROR);

	s3d->data_type = tmp.s[0];
	s3d->num_dimensions = tmp.s[1];
	s3d->num_r_elements = tmp.s[2];
	s3d->num_angles = tmp.s[3];
	s3d->corrections_applied = tmp.s[4];
	for (i = 0; i < 64; i++)
		s3d->num_z_elements[i] = tmp.s[5 + i];
	s3d->ring_difference = tmp.s[69];
	s3d->storage_order = tmp.s[70];
	s3d->axial_compression = tmp.s[71];
	s3d->x_resolution = tmp.f[36];
	s3d->v_resolution = tmp.f[37];
	s3d->z_resolution = tmp.f[38];
	s3d->w_resolution = tmp.f[39];
	for (i = 0; i < 6; i++)
		s3d->reserved_gating[i] = tmp.s[80 + i];
	s3d->gate_duration = tmp.l[43];
	s3d->r_wave_offset = tmp.l[44];
	s3d->num_accepted_beats = tmp.l[45];
	s3d->scale_factor = tmp.f[46];
	s3d->scan_min = tmp.s[94];
	s3d->scan_max = tmp.s[95];
	s3d->prompts = tmp.l[48];
	s3d->delayed = tmp.l[49];
	s3d->multiples = tmp.l[50];
	s3d->net_trues = tmp.l[51];
	s3d->tot_avg_cor = tmp.f[52];
	s3d->tot_avg_uncor = tmp.f[53];
	s3d->total_coin_rate = tmp.l[54];
	s3d->frame_start_time = tmp.l[55];
	s3d->frame_duration = tmp.l[56];
	s3d->deadtime_correction_fctr = tmp.f[57];
	for (i = 0; i < 90; i++)
		s3d->reserved_cti[i] = tmp.s[116 + i];
	for (i = 0; i < 50; i++)
		s3d->reserved_cti[i] = tmp.s[206 + i];
	for (i = 0; i < 128; i++)
		s3d->uncor_singles[i] = tmp.f[128 + i];

	return (OK);
}
