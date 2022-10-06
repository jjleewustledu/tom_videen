/* $Log$ */
 * ===============================================================================
 *  Module: getprm.c
 *  Date:   Aug-96
 *  Author: Tom Videen
Algorithm:	1) Get the basic header information which contains scanner,
		acquisition, reconstruction, and recfilter among others.

		2) Obtain the image attributes (scanner, acquisition,
		reconstruction, and recfilter) from the the basic header.

		3) For ECAT 953B images, get imgfilter.

		4) Copy the image attributes to prm.
 * ===============================================================================
 */
static char     rcsid[] = "$Header: /home/npggw/tom/src/makeprmlst/tmp/RCS/getprm.c,v 1.2 1996/08/02 16:32:24 yang Exp $";

#include <myvalues.h>
#include "makeprmlst.h"
#include <petutil/matrix7.h>

int      getprm (imgfile, prm)
	char           *imgfile;
	PARAMETER_DATA *prm;
{
    ImageFile      *file;
    Index           indx;


	MatrixData     *matrix_read ();

	COMMENT_INFO    comment_info_data;
	Image_subheader *image_subheader;
	MatrixData     *matrixx_data;
	MatrixFile     *fptr;
	float           voxel_depth;
	float           voxel_width;
	int             image_type;
	int             matnum;
	short           height;
	short           num_slices;
	short           width;
	struct ATTR_TYPE attr;
	struct BASIC_HEADER basic_header;
	struct Matval   matval;

	if (read_hdr (imgfile, &basic_header) == PET_IMAGE) {
		image_type = basic_header.Scanner;

		if (image_type == PETT6_SCANNER || image_type == SUPER_PETT_I_SCANNER ||
		    image_type == SUPER_PETT_II_SCANNER) {
			if (pett6_attr (&basic_header, &attr) == INVALID)
				return (ERROR);
		} else if (image_type == ECAT953_SCANNER || image_type == ECAT961_SCANNER) {
			if (ecat_hdr_attr (image_type, &basic_header, &attr) == INVALID)
				return (ERROR);

			matval.frame = 1;
			matval.plane = 1;
			matval.gate = 1;
			matval.data = 0;
			matval.bed = 0;

			fptr = OpenEcat (imgfile, &num_slices, &width, &height,
					 &voxel_width, &voxel_depth, matval);
			if (fptr == (MatrixFile *) NULL) {
				fprintf (stderr, "Cannnot open image %s.\n", imgfile);
				return;
			}
			if (image_type == ECAT961_SCANNER)
				strcpy (prm->extension, ".v");

			comment_info (fptr->mhptr->study_description, &comment_info_data);

			matnum = mat_numcod (matval.frame, matval.plane, matval.gate,
					     matval.data, matval.bed);
			matrixx_data = matrix_read (fptr, matnum);
			if (matrixx_data == NULL) {
				fprintf (stderr, "matrixx_read error in getrealimg.\n");
				return (ERROR);
			}
			image_subheader = (Image_subheader *) matrixx_data->shptr;

			if (comment_info_data.gauss3d_filter) {
				prm->imgfilter_type = FILTER_GAUSS;
				prm->imgfilter_cutoff
					= image_subheader->filter_ramp_slope;
			} else if (comment_info_data.butterworth3d_filter) {
				prm->imgfilter_type = FILTER_BUTTERWORTH;
				prm->imgfilter_cutoff
					= image_subheader->filter_ramp_slope;
				prm->imgfilter_order
					= image_subheader->filter_order;
			} else
				prm->imgfilter_type = FILTER_NONE;

			free_matrix_data (matrixx_data);
			matrix_close (fptr);
		} else
			return (ERROR);

		strcpy (prm->scanner, attr.scanner);
		strcpy (prm->acquisition, attr.acq);
		strcpy (prm->reconstruction, attr.recon);
		strcpy (prm->recfilter, attr.fltr);
	} else
		return (ERROR);

	return (OK);
}
