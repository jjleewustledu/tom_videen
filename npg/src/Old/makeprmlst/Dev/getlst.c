/*$Id: getlst.c,v 1.1 1994/10/07 14:39:59 ty7777 Exp $*/
/*$Log: getlst.c,v $
 * Revision 1.1  1994/10/07  14:39:59  ty7777
 * Initial revision
 **/

/*_________________________________________________________________________________
File Name:      getlst.c

Date:           05-Oct-94

Author:         Tom Yang

Description:    Get species, tracer, subject_id, and scan_id from the image name.

History:	05-Oct-94	Created by Tom Yang

Function:	PUBLIC int getlst (char *imgfile, PARAMETER_DATA *parameter_data,
			IMAGE_LIST_DATA *image_list)

Arguments:	imgfile		image file name.
		parameter_data:	structure containing parameter data, modified
				upon return.
		image_list:	structure containing a image list, modified
				upon return.

Return:		SUCCEED = 0, FAIL = -1.

Calling:	None

Called by:	main in makeprmlst.c.

Algorithm:	1) Parse the imgfile string to get species and subject_id.

		2) Parse the rest of the string to get tracer and scan_id.
_________________________________________________________________________________*/

#include "makeprmlst.h"

static char     rcsid[] = "$Header: /home/npggw/tom/src/makeprmlst/tmp/RCS/getlst.c,v 1.1 1994/10/07 14:39:59 ty7777 Exp $";

PUBLIC int      getlst (imgfile, parameter_data, image_list)
	char           *imgfile;
	PARAMETER_DATA *parameter_data;
	IMAGE_LIST_DATA *image_list;
{
	char            c;
	char            sbuffer[MAXLINE];
	int             i;
	int             ibuffer;
	int             num_scanned;

	num_scanned = sscanf (imgfile, "%c %d %s", &c, &ibuffer, sbuffer);
	if (num_scanned == 3) {
		parameter_data->species[0] = c;
		sprintf (image_list->subject_id, "%d", ibuffer);

		for (i = 0; i < MAXLINE && !(isdigit (sbuffer[i])); i++) {
			;
		}

		ibuffer = i;
		for (i = ibuffer; i < MAXLINE && isdigit (sbuffer[i])
		     && sbuffer[i] != '.'; i++) {
			;
		}
		strncpy (parameter_data->tracer, sbuffer, ibuffer);
		strncpy (image_list->scanset1_id, sbuffer + ibuffer, i - ibuffer);
	} else {
		fprintf (stderr, "Error: from parsing %s.\n", imgfile);
		return FAIL;
	}

	return SUCCEED;
}
