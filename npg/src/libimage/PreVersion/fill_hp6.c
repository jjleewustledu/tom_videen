/*  $Id: fill_hp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $
 *  $Log: fill_hp6.c,v $
 * Revision 1.3  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.2  1996/10/21  22:27:04  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		fill_hp6
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Fill PETT VI header from MainHeader structure
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/fill_hp6.c,v 1.3 1999/12/13 17:45:50 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <libimage.h>

int             fill_hp6 (file, indx)
	ImageFile      *file;
	Index          *indx;
{
	MainHeader     *mh;
	SubHeader      *sh;
	PETT6HDR       *p6h;
	float			decay;
	int             i;

	if (file->mh == NULL) {
		(void) fprintf (stderr, "ERROR [fill_hp6]: Mainheader is not defined\n");
		return (ERROR);
	}
	if (file->p6 == NULL) {
		if (make_hp6 (file) == ERROR) {
			(void) fprintf (stderr, "ERROR [fill_hp6]: Cannot malloc PETT6HDR\n");
			return (ERROR);
		}
	}
	mh = file->mh;
	sh = file->sh;
	p6h = file->p6;

	p6h->file_flag[0] = '*';
	p6h->file_flag[1] = '*';
	for (i = 0; i < 6; i++)
		p6h->run_name[i] = mh->radiopharmaceutical[i];
	p6h->rotation = 0;
	p6h->wobble_rate = 1;
	p6h->nwobble_pts = 0;
	p6h->scan_time = (short) (sh->duration / 1000);
	if (p6h->scan_time == 0)
		p6h->scan_time = 1;
	for (i = 0; i < 8; i++)
		p6h->subject_id[i] = mh->patnum[i];
	for (i = 0; i < 6; i++)
		p6h->date[i] = '0';
	if (strcmp (mh->acquisition, "high") == 0)
		p6h->resolution = 1;
	else
		p6h->resolution = 0;
	p6h->isotope_code = 1;
	p6h->slice_number = (short) indx->pln;

	if (strcmp (mh->isotope, "O-15") == 0)
		decay = 0.005668;
	else if (strcmp (mh->isotope, "N-13") == 0)
		decay = 0.001160;
	else if (strcmp (mh->isotope, "C-11") == 0)
		decay = 0.0005668;
	else if (strcmp (mh->isotope, "Ga-68") == 0)
		decay = 0.0001691;
	else if (strcmp (mh->isotope, "Ge-68") == 0)
		decay = 0.0001691;
	else if (strcmp (mh->isotope, "F-18") == 0)
		decay = 0.00010529;
	else
		decay = 0.0;
	p6h->decay_constant = ieee2pe (&decay);
	p6h->image_orientation = 0;
	p6h->time_per_frame = 0;
	p6h->total_counts = 0;
	for (i = 0; i < 10; i++)
		p6h->compound[i] = ' ';
	for (i = 0; i < 2; i++)
		p6h->unused_1[i] = 0;
	p6h->number_of_slices = mh->nplane;
	for (i = 0; i < 5; i++)
		p6h->unused_2[i] = 0;
	p6h->file_type = 3000;
	p6h->ecat_orientation = 1;
	p6h->rotation_increment = 0;
	for (i = 0; i < 12; i++)
		p6h->filter[i] = ' ';
	for (i = 0; i < 2; i++)
		p6h->unused_4[i] = 0;
	p6h->un_norm_average = 0;
	p6h->norm_average = 0;
	p6h->total_raw_events = 0;
	p6h->total_random_events = 0;
	p6h->raw_random_events = 0;
	for (i = 0; i < 13; i++)
		p6h->unused_5[i] = 0;
	p6h->norm_run_time = 0;
	for (i = 0; i < 8; i++)
		p6h->unused_6[i] = 0;
	for (i = 0; i < 16; i++)
		p6h->metabolic_data[i] = 0;
	for (i = 0; i < 26; i++)
		p6h->unused_7[i] = 0;
	p6h->pett_number = 6;
	p6h->model_number = 0;
	for (i = 0; i < 12; i++)
		p6h->pett_location[i] = ' ';
	return (OK);
}
