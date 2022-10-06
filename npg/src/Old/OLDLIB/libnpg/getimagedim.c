/*	$Id: getimagedim.c,v 1.17 1995/11/10 23:29:58 tom Exp tom $
	$Log: getimagedim.c,v $
 * Revision 1.17  1995/11/10  23:29:58  tom
 * *** empty log message ***
 * */
/* ===============================================================================
 *	Module:			getimagedim.c
 *	Date:			04/29/93
 *	Author:			Tom Yang
 *	Description:	Get the following descriptors of a PET image file:
 *	Output parameters:
 *		 scanner
 *		 width
 *		 height
 *		 num_slices
 *		 voxel_width
 *		 voxel_height
 *		 voxel_depth
 *	Return:			SUCCEED  or FAIL
 *	History:
 *		13-Nov-95	TOV modification
 * ===============================================================================
 */
#include <petutil.h>
#include <ifh.h>
#include <imglist.h>
#include <matrix7.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/getimagedim.c,v 1.17 1995/11/10 23:29:58 tom Exp tom $";

PUBLIC int      getimagedim (image_file, matval, scanner, width, height, num_slices, voxel_width,
			                     voxel_height, voxel_depth)
	char           *image_file;		 /* filename */
	struct Matval   matval;			 /* CTI structure */
	int            *scanner;		 /* defined in petutil.h */
	short          *width;			 /* number of pixels */
	short          *height;			 /* number of pixels */
	short          *num_slices;		 /* number of slices */
	float          *voxel_width;	 /* cm */
	float          *voxel_height;	 /* cm */
	float          *voxel_depth;	 /* cm */
{
	BOOLEAN         encoded_flag;
	IFH             ifhdr;
	MatrixFile     *fptr;
	char           *extension;
	char            ifhdr_file[MAXLINE];
	char            imgroot[MAXLINE];
	char           *lines[IFH_MAXLEN];
	int             fd;
	int             i;
	int             nlines;

	fd = open (image_file, O_RDONLY, 0);
	if (fd == -1) {
		fprintf (stderr, "ERROR [getimagedim]: cannot open %s\n", image_file);
		return (FAIL);
	}
	*scanner = getimage_type (fd, image_file, &encoded_flag, num_slices);
	close (fd);
	if (*scanner == ERROR_SCANNER) {
		fprintf (stderr, "ERROR [getimagedim]: illegal scanner type in %s\n", image_file);
		return (FAIL);
	}

/*
 *	Read ECAT headers (961 or 953)
 */

	if (*scanner == ECAT953_SCANNER || *scanner == ECAT961_SCANNER) {
		fptr = OpenEcat (image_file, num_slices, width, height,
				 voxel_width, voxel_depth, matval);
		if (fptr == (MatrixFile *) NULL) {
			fprintf (stderr, "ERROR [getimagedim]: cannot open %s as an ECAT file\n", image_file);
			return (FAIL);
		}
		matrix_close (fptr);
		if (*voxel_depth == 0.0) *voxel_depth = DEFAULT_ECAT_SEP;
		*voxel_height = *voxel_width;

/*
 *	Read Interfile headers
 */

	} else if (*scanner == INTERFILE) {
		memset (ifhdr_file, 0, MAXLINE);
		extension = strrchr (image_file, '.');
		if (extension != NULL)
			strncpy (ifhdr_file, image_file, strlen (image_file) - strlen (extension));
		else
			strcpy (ifhdr_file, image_file);
		strcpy (imgroot, ifhdr_file);
		strcat (ifhdr_file, IFH_EXT);

		if (readifhdr (ifhdr_file, FALSE, IFH_MAXLEN, &nlines, lines) == FAIL
			    || getifhdr (ifhdr_file, FALSE, nlines, lines, &ifhdr) == FAIL) {
			strcpy (ifhdr_file, imgroot);
			strcat (ifhdr_file, HDR_EXT);
			if  (hdr2ifh (ifhdr_file, TRUE, &ifhdr) == FAIL) return FAIL;
		}
		*width = ifhdr.matrix_size[0];
		*height = ifhdr.matrix_size[1];
		*num_slices = ifhdr.matrix_size[2];
		*voxel_width = ifhdr.scaling_factor[0] / MMS_PER_CM;
		*voxel_height = ifhdr.scaling_factor[1] / MMS_PER_CM;
		*voxel_depth = ifhdr.scaling_factor[2] / MMS_PER_CM;

/*
 *	Read PETT VI headers
 */

	} else {
		*width = ENCODED_SLICE_WIDTH;
		*height = ENCODED_SLICE_HEIGHT;
		if (*num_slices == PETT6_7SLICES) {
			*voxel_width = PETT6_PIXEL_SIZE;
			*voxel_depth = PETT6_SEPARATION;
		} else if (*num_slices == PETT6_14) {
			*voxel_width = PETT6_PIXEL_SIZE;
			*voxel_depth = PETT6_SEPARATION / 2;
		} else {
			*voxel_width = SMG_PIXEL_SIZE;
			*voxel_depth = SMG_SEPARATION;
		}
		*voxel_height = *voxel_width;
	}
	return SUCCEED;
}
