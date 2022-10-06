/*	$Id: getpath.c,v 3.0 2003/08/21 15:16:40 tom Exp $
	$Log: getpath.c,v $
 * Revision 3.0  2003/08/21  15:16:40  tom
 * 21Aug2003
 *
 * Revision 2.4  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.3  1999/10/12  13:15:56  tom
 * new arc path (single disk)
 *
 * Revision 2.2  1999/05/07  18:49:56  tom
 * remove Warning
 *
 * Revision 2.1  1996/05/16  18:58:05  tom
 * change warning if file not found
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 * */
/* ===============================================================================
 *	Module:			getpath.c
 *	Date:			Feb-96
 *	Author:			Tom Videen
 *	Description:	Get full path to data file 
 * ===============================================================================
 */
#ifndef lint
static char		rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/getpath.c,v 3.0 2003/08/21 15:16:40 tom Exp $";
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imgutil.h>

char           *getpath (fname, dtype, prm)
	char            *fname;			/* short filename */
	char            *dtype;			/* data type (used by archive) */
	PARAMETER_DATA  *prm;			/* parameter list */
{
    FILE           *fp;
	char            *fullname;
	int              i;

/*	Check datapath */

	fullname = (char *) malloc (MAXPATH);
	if (strlen (prm->datapath) > 0) {
		(void) strcpy (fullname, prm->datapath);
		(void) strcat (fullname, "/");
		(void) strcat (fullname, fname);
    	fp = fopen (fullname, "r");
		if (fp != (FILE *) NULL) {
			(void) fclose (fp);
			fprintf (stdout, "Using: %s\n", fullname);
			return (fullname);
		}
	}

/*	Check local directory */

	(void) strcpy (fullname, fname);
    fp = fopen (fullname, "r");
	if (fp != (FILE *) NULL) {
		(void) fclose (fp);
		fprintf (stdout, "Using: %s\n", fullname);
		return (fullname);
	}

/*	Check tmp */

	(void) strcpy (fullname, "/tmp/");
	(void) strcat (fullname, fname);
   	fp = fopen (fullname, "r");
	if (fp != (FILE *) NULL) {
		(void) fclose (fp);
		fprintf (stdout, "Using: %s\n", fullname);
		return (fullname);
	}

/*	Check archive */

	(void) strcpy (fullname, "/data/arc/");
	(void) strcat (fullname,prm->category);
	(void) strcat (fullname,"/");
	i = strspn (fname+1, "0123456789");
	(void) strncat (fullname,fname,i+1);
	(void) strcat (fullname,"/");
	(void) strcat (fullname,dtype);
	(void) strcat (fullname,"/");
	(void) strcat (fullname,fname);
   	fp = fopen (fullname, "r");
	if (fp != (FILE *) NULL) {
		(void) fclose (fp);
		fprintf (stdout, "Using: %s\n", fullname);
		return (fullname);
	}

/*	Could not find file */

	(void) strcpy (fullname, "");
	return (fullname);
}
