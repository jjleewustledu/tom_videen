/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		getssh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Write scan subheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimg/RCS/cti7.c,v 1.11 1995/12/15 21:40:17 tom Exp $";
#endif

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <libimg.h>

int             getssh7 (fd, sh, plane, frame, gate, data, bed)
	FILE           *fd;			 /* File designator for image or scan file. */
	Scan3D_subheader7 *sh;
	int             plane, frame, gate, data, bed;

{
	union temporary {
		float           f[NUMREC_SSHDR * LENREC_S / 2];
		long            l[NUMREC_SSHDR * LENREC_S / 2];
		short           s[NUMREC_SSHDR * LENREC_S];
		char            c[NUMREC_SSHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		long            l;
		short           s[2];
	}               tmpl;

	int             i, len, matnum, begrec, endrec;
	short          *hdr;

	for (i = 0; i < (NUMREC_SSHDR * LENREC_S); i++)
		tmp.s[i] = 0;
	hdr = (short *) sh->whole_header;
	for (i = 0; i < LEN_SSHDR; i++)
		tmp.s[i] = hdr[i];

	tmp.s[0] = sh->data_type;
	tmp.s[1] = sh->num_dimensions;
	tmp.s[2] = sh->num_r_elements;
	tmp.s[3] = sh->num_angles;
	tmp.s[69] = sh->ring_difference;
	for (i = 0; i < sh->ring_difference; i++)
		tmp.s[5 + i] = sh->num_z_elements[i];

	tmp.f[36] = sh->x_resolution;
	tmp.f[37] = sh->y_resolution;
	tmp.f[38] = sh->z_resolution;

	tmpl.f = sh->scale_factor;
	tmp.s[92] = tmpl.s[0];
	tmp.s[93] = tmpl.s[1];

	tmp.s[94] = sh->scan_min;
	tmp.s[95] = sh->scan_max;

	tmp.l[48] = sh->prompts;
	tmp.l[49] = sh->delayed;
	tmp.l[50] = sh->multiples;
	tmp.l[51] = sh->net_trues;

	tmp.l[55] = sh->frame_start_time;
	tmp.l[55] = sh->frame_duration;

	tmp.f[57] = sh->loss_correction_fctr;
	tmp.s[178] = tmpl.s[0];
	tmp.s[179] = tmpl.s[1];

	matnum = numcod7 (plane, frame, gate, data, bed);
	len = NUMREC_SSHDR;
	if ((begrec = getmatpos (fd, matnum, &endrec)) == ERROR) {
		if ((begrec = credirent (fd, matnum, len)) < 0) {
			fprintf (stderr, "ERROR [getssh7]: Could not entry for matrix #%x\n", matnum);
			return (NULL);
		}
	}
	if (wrtrec (fd, (char *) tmp.s, begrec, len) != 0)
		return (ERROR);
	else
		return (NULL);
}
