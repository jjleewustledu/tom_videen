/*  $Id: get_hp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $
 *  $Log: get_hp6.c,v $
 *  Revision 1.4  1999/12/13 17:45:50  tom
 *  tov revision
 *
 * Revision 1.3  1997/12/02  22:55:59  tom
 * add unistd.h
 *
 * Revision 1.2  1997/12/02  22:46:21  tom
 * modify for dynamic
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_hp6
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Read PETT VI slice header and store in PETT6HDR.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/get_hp6.c,v 1.4 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <image.h>
#include <endian.h>

int             get_hp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	union temp_short {
		short           s[LENREC_S];
		char            c[LENREC];
	}               tmp;

	union temp_long {
		float           f;
		int             l;
		short           s[2];
	}               conv;

	PETT6HDR       *p6;
	int             i, j;
	long            offset, slices, n;

	if (file->p6 == NULL) {
		if ((p6 = (PETT6HDR *) calloc (1, sizeof (PETT6HDR))) == NULL) {
			(void) fprintf (stderr, "ERROR [get_hp6]: Could not calloc memory for PETT VI header\n");
			return (ERROR);
		}
	} else
		p6 = file->p6;

	slices = p6->number_of_slices;
	if (slices == 0)
		slices = 7;
	n = (long) ((indx->frm) - 1) * slices;
	offset = (n + (long) indx->pln - 1) * PETT6_SLICE_BYTES;
	(void) fseek (file->fp, offset, SEEK_SET);

	if ((fread (tmp.c, 1, RECLEN, file->fp)) != RECLEN) {
		(void) fprintf (stderr, "ERROR [get_hp6]: reading PETT VI header from %s\n", file->name);
		return (ERROR);
	}

	/* Strings	 */
	for (i = 0; i < 2; i++)
		p6->file_flag[i] = tmp.c[i];
	for (i = 0; i < 6; i++)
		p6->run_name[i] = tmp.c[2 + i];
	for (i = 0; i < 8; i++)
		p6->subject_id[i] = tmp.c[16 + i];
	for (i = 0; i < 6; i++)
		p6->date[i] = tmp.c[24 + i];
	for (i = 0; i < 10; i++)
		p6->compound[i] = tmp.c[48 + i];
	for (i = 0; i < 12; i++)
		p6->filter[i] = tmp.c[80 + i];
	for (i = 0; i < 12; i++)
		p6->pett_location[i] = tmp.c[244 + i];

	/* Shorts and Longs  */
	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) {
		swap_byte (tmp.c, RECLEN);
		j = 1;
	} else
		j = 0;

	p6->rotation = tmp.s[4];
	p6->wobble_rate = tmp.s[5];
	p6->nwobble_pts = tmp.s[6];
	p6->scan_time = tmp.s[7];

	p6->resolution = tmp.s[15];
	p6->isotope_code = tmp.s[16];
	p6->slice_number = tmp.s[17];

	conv.s[0 + j] = tmp.s[18];
	conv.s[1 - j] = tmp.s[19];
	p6->decay_constant = conv.l;

	p6->image_orientation = tmp.s[20];
	p6->time_per_frame = tmp.s[21];

	conv.s[0 + j] = tmp.s[22];
	conv.s[1 - j] = tmp.s[23];
	p6->total_counts = conv.l;

	for (i = 0; i < 2; i++)
		p6->unused_1[i] = tmp.s[29 + i];
	p6->number_of_slices = tmp.s[31];
	for (i = 0; i < 5; i++)
		p6->unused_2[i] = tmp.s[32 + i];

	p6->file_type = tmp.s[37];
	p6->ecat_orientation = tmp.s[38];
	p6->rotation_increment = tmp.s[39];

	for (i = 0; i < 2; i++)
		p6->unused_4[i] = tmp.s[46 + i];
	p6->un_norm_average = tmp.s[48];
	p6->norm_average = tmp.s[49];

	conv.s[0 + j] = tmp.s[50];
	conv.s[1 - j] = tmp.s[51];
	p6->total_raw_events = conv.l;

	conv.s[0 + j] = tmp.s[52];
	conv.s[1 - j] = tmp.s[53];
	p6->total_random_events = conv.l;

	conv.s[0 + j] = tmp.s[54];
	conv.s[1 - j] = tmp.s[55];
	p6->raw_random_events = conv.l;

	for (i = 0; i < 13; i++)
		p6->unused_5[i] = tmp.s[56 + i];
	p6->norm_run_time = tmp.s[69];
	for (i = 0; i < 8; i++)
		p6->unused_6[i] = tmp.s[70 + i];
	for (i = 0; i < 16; i++)
		p6->metabolic_data[i] = tmp.s[78 + i];
	for (i = 0; i < 26; i++)
		p6->unused_7[i] = tmp.s[94 + i];
	p6->pett_number = tmp.s[120];
	p6->model_number = tmp.s[121];

	file->p6 = p6;
	return (OK);
}
