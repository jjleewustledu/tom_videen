/*  $Id: get_mh.c,v 1.5 2002/03/13 21:56:41 tom Exp $
    $Log: get_mh.c,v $
 * Revision 1.5  2002/03/13  21:56:41  tom
 * lint cleaning
 *
 * Revision 1.4  2000/09/27  19:03:28  tom
 * *** empty log message ***
 *
 * Revision 1.3  2000/07/18  15:40:28  tom
 * add Interfile compatibility
 *
 * Revision 1.2  2000/03/13  18:23:14  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *	Function:		get_mh
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Get a generic mainheader from an image file.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/get_mh.c,v 1.5 2002/03/13 21:56:41 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             get_mh (file)
	ImageFile      *file;
{
	Index		   *indx;
	int             fformat;

	fformat = getfiletype (file);

	if (fformat == MATRIX_REV7) {		 /* Matrix file version 7 */
		if ((get_mh7 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_mh]: Cannot read MainHeader from %s\n", file->name);
			return (ERROR);
		}
		if ((use_mh7 (file)) == ERROR) 
			return (ERROR);
	} else if (fformat == MATRIX_REV6) {	 /* Matrix file version 6 */
		if ((get_mh6 (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_mh]: Cannot read MainHeader from %s\n", file->name);
			return (ERROR);
		}
		if ((use_mh6 (file)) == ERROR)
			return (ERROR);
	} else if (fformat == PETT6) {		 /* PETT VI */
		indx = (Index *) calloc (1, sizeof (Index));
		indx->frm = 1;
		indx->pln = 1;
		indx->gate = 1;
		indx->data = 0;
		indx->bed = 0;
		if ((get_hp6 (file, indx)) == ERROR) {
			free (indx);
			(void) fprintf (stderr, "ERROR [get_mh]: Cannot read Header from %s\n", file->name);
			return (ERROR);
		}
		if ((use_mhp6 (file)) == ERROR)
			return (ERROR);
		free (indx);
	} else if (fformat == INTERFILE) {	 /* Interfile file. */
		if ((get_ifh (file)) == ERROR) {
			(void) fprintf (stderr, "ERROR [get_mh]: Cannot read MainHeader from %s\n", file->name);
			return (ERROR);
		}
		if ((use_mhif (file)) == ERROR)
			return (ERROR);
	} else {
		(void) fprintf (stderr, "ERROR [get_mh]: Unsupported file format %d in %s\n", fformat, file->name);
		return (ERROR);
	}
	return (OK);
}
