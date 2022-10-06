/*	$Id: getmap.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: getmap.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1997/03/19  22:32:51  tom
 * Initial revision
 *
 * Revision 2.1  1996/09/10  20:00:55  tom
 * *** empty log message ***
 *
 * Revision 2.0  1996/08/14  20:13:10  tom
 * new libs
 *
 * Revision 1.1  1995/10/12  21:19:02  tom
 * Initial revision
 * */
/*	===============================================================================
 *	Module:			getmap.c
 *	Date:			12-Oct-95
 *	Author:			Tom Videen
 *	Description:	Read matrix transformation from a map file.
 *	===============================================================================
 */
#include <stdio.h>
#include <image.h>
#include <nrutil.h>

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/getmap.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif

int             getmap (fname, map)
	char           *fname;
	float         **map;

{
	FILE           *fp;
	char            line[MAXLINE];
	int				i;

	fp = fopen (fname, "r");
	if (fp == (FILE *) NULL) {
		fprintf (stderr, "ERROR [getmap]: opening matrix transform file %s\n", fname);
		return (ERROR);
	}
	for (i = 1; i <= 2; i++) {
		if (fgets (line, MAXLINE, fp) == NULL) {
			fprintf (stderr, "ERROR [getmap]: reading matrix transform file\n");
			return (ERROR);
		}
	}
	for (i = 1; i <= 4; i++) {
		if (fgets (line, MAXLINE, fp) != NULL) {
			if (sscanf (line, "%f %f %f %f", &map[i][1], &map[i][2], &map[i][3],
				    &map[i][4]) != 4) {
				fprintf (stderr, "ERROR [getmap]: reading matrix transform file\n");
				return (ERROR);
			}
		} else {
			fprintf (stderr, "ERROR [getmap]: reading matrix transform file\n");
			return (ERROR);
		}
	}
	(void) fclose (fp);
	return (OK);
}
