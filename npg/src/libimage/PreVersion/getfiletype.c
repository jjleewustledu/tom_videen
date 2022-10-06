/*  $Id: getfiletype.c,v 1.10 2002/12/17 21:55:15 tom Exp $
    $Log: getfiletype.c,v $
 * Revision 1.10  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
    Revision 1.9  2002/04/05 16:40:36  tom
    check "Advance" for Wake Forest Univ converted 6.5 file

 * Revision 1.8  2000/07/18  15:40:31  tom
 * add Interfile compatibility
 *
 * Revision 1.7  2000/02/18  21:38:39  tom
 * extend range of data_type to check
 *
 * Revision 1.6  1999/12/13  17:45:50  tom
 * tov revision
 *
 * Revision 1.5  1998/09/18  13:38:08  tom
 * add system 962
 *
 * Revision 1.4  1998/08/27  21:09:10  tom
 * permit system == 1 and sw_version == 65 for MATRIX_REV6
 *
 * Revision 1.3  1996/10/16  15:49:01  tom
 * system = 6
 *
 * Revision 1.2  1996/09/26  21:48:27  tom
 * permit system == 0
 *
 * Revision 1.1  1996/04/19  19:33:20  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Function:       getfiletype
 *  Date:           Mar-96
 *  Author:         Tom Videen
 *  Description:    Return type of file (types defined in imagio.h)
 *		1 = MATRIX_REV6
 *		2 = MATRIX_REV7
 *		3 = INTERFILE
 *		4 = PETT6
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/getfiletype.c,v 1.10 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <image.h>

#define TESTLEN 28

int             getfiletype (file)
	ImageFile      *file;
{
	union conv {
		short           s[TESTLEN];
		char            c[2 * TESTLEN];
	}               test;

	FILE           *fp;
	char           *filename;
	int             filetype;
	short           system, data_type, sw_version;
	char            hdrfil[MAXFNAME], *ptr;

	filename = file->name;
	if ((fp = fopen (filename, "r")) == NULL) {
		(void) fprintf (stderr, "ERROR [getfiletype]: Could not open %s\n", filename);
		return (ERROR);
	}
	if (fread (test.s, sizeof (short), TESTLEN, fp) != TESTLEN) {
		(void) fprintf (stderr, "ERROR [getfiletype]: Could not read %s\n", filename);
		return (ERROR);
	}
	(void) fclose (fp);
	test.c[TESTLEN - 1] = '\0';

	data_type = (((test.s[25] & 0xff) << 8) | ((test.s[25] & 0xff00) >> 8));
	if (data_type > 1 && data_type < 8) {
		system = (((test.s[26] & 0xff) << 8) | ((test.s[26] & 0xff00) >> 8));
		sw_version = (((test.s[24] & 0xff) << 8) | ((test.s[24] & 0xff00) >> 8));
	} else {
		system = test.s[26];
		sw_version = test.s[24];
	}

	if (strncmp (test.c, "INTERFILE", 9) == 0) /* Interfile header */
		filetype = INTERFILE;
	else if (strncmp (test.c, "MATRIX7", 7) == 0) /* Matrix file version 7. */
		filetype = MATRIX_REV7;
	else if (((system == 921) || (system == 951) || (system == 953) || (system == 961) ||
		      (system == 962) || (system == 6) || (system == 1) || (system == 0)) &&
		 ((sw_version == 64) || (sw_version == 65) || (sw_version == 6)))
		filetype = MATRIX_REV6;
	else if (strncmp (&(test.c[28]), "Advance", 7) == 0)	/* Wake Forest file */
		filetype = MATRIX_REV6;
	else if (strncmp (test.c, "**", 2) == 0)
		filetype = PETT6;
	else {
		(void) strcpy (hdrfil, filename);
		ptr = strrchr (hdrfil, '.');
		if (ptr != NULL)
			ptr = NULL;
		(void) strcat (hdrfil, ".ifh");
		if ((fp = fopen (hdrfil, "r")) == NULL)
			filetype = ERROR;
		else {
			if (fread (test.s, sizeof (short), TESTLEN, fp) != TESTLEN) {
				filetype = ERROR;
				(void) fclose (fp);
			} else {
				if (strncmp (test.c, "INTERFILE", 9) == 0)	/* Interfile file. */
					filetype = INTERFILE;
				else
					filetype = ERROR;
			}
		}
	}
	return (filetype);
}
