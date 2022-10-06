/*	$Id: filterimg.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: filterimg.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
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
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/filterimg.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
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
		(void) strcpy (command, "gauss ");
	else if (filter_type == FILTER_BUTTERWORTH)
		(void) strcpy (command, "butterworth ");

	fprintf (stdout, "%s\n", filename);
	(void) strcat (command, filename);
	(void) strcat (command, " ");
	fprintf (stdout, "%s\n", filtered_name);
	(void) strcat (command, filtered_name);
	(void) strcat (command, " ");

	(void) sprintf (cbuffer, " %f", cutoff);
	(void) strcat (command, cbuffer);

	if (filter_type == FILTER_BUTTERWORTH) {
		(void) sprintf (cbuffer, " %d", order);
		(void) strcat (command, cbuffer);
	}
	fprintf (stdout, "%s\n", command);
	if (system (command) != 0) {
		fprintf (stderr, "ERROR [filterimg]: Could not filter %s\n", filename);
		return (ERROR);
	}
	return (OK);
}
