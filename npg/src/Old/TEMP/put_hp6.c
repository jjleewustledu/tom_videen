/*  $Id: put_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp tom $
 *  $Log: put_hp6.c,v $
 * Revision 1.2  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		put_hp6
 *  Date:           Mar 2003
 *  Author:         Tom Videen
 *	Description:	Write PETT VI slice header.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/RCS/put_hp6.c,v 1.2 1999/12/13 17:45:50 tom Exp tom $";
#endif

#include <stdio.h>
#include <image.h>
#include <endian.h>

int             put_hp6 (file)
	ImageFile      *file;
{
	union temp_short {
		short           s[RECLEN/2];
		char            c[RECLEN];
	}               tmp;

	union temp_long {
		int             l;
		short           s[2];
	}               conv;

	PETT6HDR       *p6;
	int             i, j;

	if (file->p6 == NULL) {
		fprintf (stderr, "ERROR [put_hp6]: no PETT 6 header\n");
		return (ERROR);
	}
	p6 = file->p6;

	for (i = 0; i < RECLEN/2; i++)
		tmp.s[i] = 0;

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
		j = 1;
	else
		j = 0;

	/* Shorts and Longs  */

	tmp.s[4] = p6->rotation;
	tmp.s[5] = p6->wobble_rate;
	tmp.s[6] = p6->nwobble_pts;
	tmp.s[7] = p6->scan_time;

	tmp.s[15] = p6->resolution;
	tmp.s[17] = p6->isotope_code;
	tmp.s[18] = p6->slice_number;

	conv.l = p6->decay_constant;
	tmp.s[18] = conv.s[0 + j];
	tmp.s[19] = conv.s[1 - j];

	tmp.s[20] = p6->image_orientation;
	tmp.s[21] = p6->time_per_frame;

	conv.l = p6->total_counts;
	tmp.s[22] = conv.s[0 + j];
	tmp.s[23] = conv.s[1 - j];

	for (i = 0; i < 2; i++)
		tmp.s[29 + i] = p6->unused_1[i];
	tmp.s[31] = p6->number_of_slices;
	for (i = 0; i < 5; i++)
		tmp.s[32 + i] = p6->unused_2[i];

	tmp.s[37] = p6->file_type;
	tmp.s[38] = p6->ecat_orientation;
	tmp.s[39] = p6->rotation_increment;

	for (i = 0; i < 2; i++)
		tmp.s[46 + i] = p6->unused_4[i];
	tmp.s[48] = p6->un_norm_average;
	tmp.s[49] = p6->norm_average;

	conv.l = p6->total_raw_events;
	tmp.s[50] = conv.s[0 + j];
	tmp.s[51] = conv.s[1 - j];

	conv.l = p6->total_random_events;
	tmp.s[52] = conv.s[0 + j];
	tmp.s[53] = conv.s[1 - j];

	conv.l = p6->raw_random_events;
	tmp.s[54] = conv.s[0 + j];
	tmp.s[55] = conv.s[1 - j];

	for (i = 0; i < 13; i++)
		tmp.s[56 + i] = p6->unused_5[i];
	tmp.s[69] = p6->norm_run_time;
	for (i = 0; i < 8; i++)
		tmp.s[70 + i] = p6->unused_6[i];
	for (i = 0; i < 16; i++)
		tmp.s[78 + i] = p6->metabolic_data[i];
	for (i = 0; i < 26; i++)
		tmp.s[94 + i] = p6->unused_7[i];
	tmp.s[120] = p6->pett_number;
	tmp.s[121] = p6->model_number;

	/* CONSTCOND  */
	if (__BYTE_ORDER == __LITTLE_ENDIAN) 
		swap_byte (tmp.c, RECLEN);

	/* Strings	 */

	for (i = 0; i < 2; i++)
		tmp.c[i] = p6->file_flag[i];
	for (i = 0; i < 6; i++)
		tmp.c[2 + i] = p6->run_name[i];
	for (i = 0; i < 8; i++)
		tmp.c[16 + i] = p6->subject_id[i];
	for (i = 0; i < 6; i++)
		tmp.c[24 + i] = p6->date[i];
	for (i = 0; i < 10; i++)
		tmp.c[48 + i] = p6->compound[i];
	for (i = 0; i < 12; i++)
		tmp.c[80 + i] = p6->filter[i];
	for (i = 0; i < 12; i++)
		tmp.c[244 + i] = p6->pett_location[i];

	if (fwrite (tmp.c, 1, RECLEN, file->fp) != RECLEN) {
		(void) fprintf (stderr, "ERROR [put_hp6]: writing PETT VI header to %s\n", file->name);
		return (ERROR);
	}
	return (OK);
}
