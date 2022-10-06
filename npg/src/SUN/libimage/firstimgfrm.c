/*  $Log: firstimgfrm.c,v $
 *  Revision 1.5  2000/07/18 15:40:23  tom
 *  add Interfile compatibility
 *
 * Revision 1.4  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.3  1999/07/07  18:02:50  tom
 * read mh if it is null
 *
 * Revision 1.2  1997/03/10  17:17:29  tom
 * return neg of max if cannot find
 *
 * Revision 1.1  1997/01/24  18:17:56  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		firstimgfrm
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *	Description:	Return frame number for first image frame which exists
 *					or the negative of max frame number tested.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/tom/src/libimage/RCS/firstimgfrm.c,v 1.5 2000/07/18 15:40:23 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <libimage.h>

int             firstimgfrm (file)
	ImageFile      *file;
{
	Index           indx;
	int             fformat;
	int             max = 3;

	indx.pln = 1;
	indx.gate = 1;
	indx.data = 0;
	indx.bed = 0;

	if (file->mh == NULL) 
	    if (get_mh (file) == ERROR) return (ERROR);
	fformat = file->mh->file_format;
	if (fformat == MATRIX_REV7) {
		for (indx.frm = 1; indx.frm <= max; indx.frm++)
			if (existfrm7 (file, &indx) != ERROR)
				return (indx.frm);
	} else if (fformat == MATRIX_REV6) {
		for (indx.frm = 1; indx.frm <= max; indx.frm++)
			if (existfrm6 (file, &indx) != ERROR)
				return (indx.frm);
	} else if (fformat == PETT6) {
		for (indx.frm = 1; indx.frm <= max; indx.frm++)
			if (existp6 (file, &indx) != ERROR)
				return (indx.frm);
	} else if (fformat == INTERFILE) {	
		for (indx.frm = 1; indx.frm <= max; indx.frm++)
			if (existfif (file, &indx) != ERROR)
				return (indx.frm);
	}
	return (-max);
}
