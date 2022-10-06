/*  $Id$
    $Log$
 */
/* =============================================================================
 *	Function:		getssh7
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Read scan subheader.
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

Scan3D_subheader7 *getssh7 (fd, plane, frame, gate, data, bed)
	FILE           *fd;			 /* File designator for image or scan file. */
	int             plane, frame, gate, data, bed;

{
	Scan3D_subheader7 *sh;

	union temporary {
		float           f[NUMREC_SSHDR * LENREC_S / 2];
		long            l[NUMREC_SSHDR * LENREC_S / 2];
		short           s[NUMREC_SSHDR * LENREC_S];
		char            c[NUMREC_SSHDR * LENREC];
	}               tmp;

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               tmpl;

	int             i;
	int             lenrd, matnum, begrec, endrec;
	short          *hdr;

	sh = (Scan3D_subheader7 *) malloc (sizeof (Scan3D_subheader7));
	if (sh == NULL) {
		fprintf (stderr, "ERROR [getssh7]: Cannot malloc sh\n");
		return (NULL);
	}
	matnum = numcod7 (plane, frame, gate, data, bed);
	if ((begrec = getmatpos (fd, matnum, &endrec)) == ERROR) {
		fprintf (stderr, "ERROR [getssh7]: Matrix #%x not found\n", matnum);
		return (NULL);
	}
	lenrd = NUMREC_SSHDR;
	if (rdrec (fd, (char *) tmp.s, begrec, lenrd) != 0)
		return (NULL);
	hdr = (short *) malloc (LENREC);
	if (hdr == NULL) {
		fprintf (stderr, "ERROR [getssh7]: Cannot malloc hdr\n");
		free (sh);
		return (NULL);
	}
	for (i = 0; i < LENREC_S; i++)
		hdr[i] = tmp.s[i];
	sh->whole_header = (void *) hdr;

	sh->data_type = tmp.s[0];
	sh->num_dimensions = tmp.s[1];
	sh->num_r_elements = tmp.s[2];
	sh->num_angles = tmp.s[3];
	sh->ring_difference = tmp.s[69];
	for (i = 0; i < sh->ring_difference; i++)
		sh->num_z_elements[i] = tmp.s[5 + i];

	sh->x_resolution = tmp.f[36];
	sh->y_resolution = tmp.f[37];
	sh->z_resolution = tmp.f[38];

	tmpl.s[0] = tmp.s[92];
	tmpl.s[1] = tmp.s[93];
	sh->scale_factor = tmpl.f;

	sh->scan_min = tmp.s[94];
	sh->scan_max = tmp.s[95];

	sh->prompts = tmp.l[48];
	sh->delayed = tmp.l[49];
	sh->multiples = tmp.l[50];
	sh->net_trues = tmp.l[51];

	sh->frame_start_time = tmp.l[55];
	sh->frame_duration = tmp.l[56];

	tmpl.s[0] = tmp.s[178];
	tmpl.s[1] = tmp.s[179];
	sh->loss_correction_fctr = tmp.f[57];

	return (sh);
}
