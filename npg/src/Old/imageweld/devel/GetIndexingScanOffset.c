/*$Id: GetIndexingScanOffset.c,v 1.3 1993/10/19 20:25:35 ty7777 Exp $*/
/*$Log: GetIndexingScanOffset.c,v $
 * Revision 1.3  1993/10/19  20:25:35  ty7777
 * Added show_offset as a flag to display the offset on the screen.
 *
 * Revision 1.2  1993/10/15  20:51:06  ty7777
 * Before Interpolation.
 *
 * Revision 1.1  1993/09/08  16:35:37  ty7777
 * Initial revision
 **/

/*_________________________________________________________________
  File:		GetIndexingScanOffset.c

  Function:	int GetIndexingScanOffset (char *top_scan, char *bottom_scan,
		float *offset, BOOLEAN *top_bottom_flipped, BOOLEAN show_offset)

  Arguments:	top_scan,		top scan name.
		bottom_scan,		bottom scan name.
		offset,			offset of the horizontal bed position.
		top_bottom_flipped,	the top and the bottom scan image is
					flipped after runing this function.
		show_offset,		show the offset on screen.

  Return:	SUCCEED: succeesful; FAIL: error.

  Description:	This functions get the offset of indexing scans in
		mm.

  History:	Created by Tom Yang on 05/05/93. 
_________________________________________________________________*/

#include "imageweld.h"
#include <petutil/matrix.h>

PRIVATE int GetBedPosition ();

static char rcsid []= "$Id: GetIndexingScanOffset.c,v 1.3 1993/10/19 20:25:35 ty7777 Exp $";

PUBLIC int GetIndexingScanOffset (top_scan, bottom_scan, offset, top_bottom_flipped, show_offset)
	char	*top_scan;
	char	*bottom_scan;
	float	*offset;
	BOOLEAN	*top_bottom_flipped;
	BOOLEAN	show_offset;
{
	char	cbuffer [MAXLINE];
	float	bottom_scan_position;
	float	top_scan_position;

	if (GetBedPosition (top_scan, &top_scan_position, show_offset) == FAIL)
	{
		fprintf (stderr, "Error in GetBedPosition () for %s\n", top_scan);
		return FAIL;
	}

	if (GetBedPosition (bottom_scan, &bottom_scan_position, show_offset) == FAIL)
	{
		fprintf (stderr, "Error in GetBedPosition () for %s\n", bottom_scan);
		return FAIL;
	}

	if (top_scan_position <= bottom_scan_position)
	{
		*offset			= bottom_scan_position - top_scan_position;
		*top_bottom_flipped	= FALSE;
	}
	else
	{
		*offset	= top_scan_position - bottom_scan_position;
		strcpy (cbuffer, top_scan);
		strcpy (top_scan, bottom_scan);
		strcpy (bottom_scan, cbuffer);
		*top_bottom_flipped	= TRUE;
	}

	return SUCCEED;
}

PRIVATE int GetBedPosition (ecat_filename, scan_position, show_offset)
	char	*ecat_filename;
	float	*scan_position;
	BOOLEAN	show_offset;
{
	MatrixFile	*matrix_open ();

	Main_header	*main_header;
	MatrixFile	*matfptr;

	matfptr	= matrix_open (ecat_filename, "r");
	if (matfptr == NULL)
	{
		fprintf (stderr, "Error: matrix_open in opening %s ().\n", ecat_filename);
		return FAIL;
	}

	main_header	= matfptr->mhptr;

	/*
	 * Check if the ECAT file is a really an ECAT image.
	 */
	if (main_header->data_type != IMAGE_DATA)
	{
		fprintf (stderr, "Error: Data type is not compatible with image data type.\n");
		matrix_close (matfptr);
		return FAIL;
	}

	*scan_position		= main_header->init_bed_position;
	matrix_close (matfptr);

	/*
	 * Check if the scan_position is 0.
	 */
	if (*scan_position == 0.0)
	{
		fprintf (stdout, "The horizontal bed position in image file %s main header is 0.0.\n",
			ecat_filename);
		fprintf (stdout, "Please specify the horizontal bed position for file %s in mm.\n",
			ecat_filename);
		scanf ("%f", scan_position);
	}
	else if (show_offset)
	{
		fprintf (stdout, "The horizontal bed position in image file %s main header is %f.\n",
			ecat_filename, *scan_position);
		fprintf (stdout, "Please make sure it is correct.\n\n");
	}

	return SUCCEED;
}
