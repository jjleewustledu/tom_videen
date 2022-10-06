/*	$Id: filterimg.c,v 2.1 1999/12/13 17:04:17 tom Exp $
	$Log: filterimg.c,v $
 * Revision 2.1  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			filterimg.c
 *	Date:			Jul-93
 *	Author:			Tom Yang
 *	Description:	Call a filter program to filter an image.
 * ===============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/filterimg.c,v 2.1 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

int			filterimg (filename, filter_type, cutoff, order, filtered_name)
	char           *filename;
	int             filter_type;
	float           cutoff;
	int             order;
	char           *filtered_name;
{
	char            cbuffer[MAXFNAME];
	char            command[MAXPATH];

	if (filter_type == FILTER_GAUSS)
		strcpy (command, "gauss ");
	else if (filter_type == FILTER_BUTTERWORTH)
		strcpy (command, "butterworth ");

	printf ("%s\n", filename);
	strcat (command, filename);
	strcat (command, " ");
	printf ("%s\n", filtered_name);
	strcat (command, filtered_name);
	strcat (command, " ");

	sprintf (cbuffer, " %f", cutoff);
	strcat (command, cbuffer);

	if (filter_type == FILTER_BUTTERWORTH) {
		sprintf (cbuffer, " %d", order);
		strcat (command, cbuffer);
	}
	printf ("%s\n", command);
	if (system (command) != 0) {
		fprintf (stderr, "ERROR [filterimg]: Could not filter %s\n", filename);
		return (ERROR);
	}
	return (OK);
}
