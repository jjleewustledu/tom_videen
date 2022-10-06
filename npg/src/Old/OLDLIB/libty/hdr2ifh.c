/*$Id: hdr2ifh.c,v 1.5 1995/11/10 16:28:34 tom Exp $*/
/*$Log: hdr2ifh.c,v $
 * Revision 1.5  1995/11/10  16:28:34  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.4  1995/09/05  17:16:30  ty7777
 * Use public defined ERROR_BOUND.
 *
 * Revision 1.3  1995/06/14  20:10:22  ty7777
 * Working version.
 *
 * Revision 1.2  1995/06/13  17:20:48  ty7777
 * Added interfile.
 *
 * Revision 1.1  1995/06/12  21:48:47  ty7777
 * Initial revision
 **/

#include <petutil.h>
#include <ifh.h>
#include <AIR.h>

#define	DEFAULT_XSIZE	1.0 
#define	DEFAULT_YSIZE	1.0 
#define	DEFAULT_ZSIZE	1.25

/*____________________________________________________________________________	
	File:		hdr2ifh.c	

	Function:	int hdr2ifh (char *analyze_header, BOOLEAN print_message, IFH *ifhdr)

			analyze_header:	analyze header file, passed in.
			print_message:	TRUE:	printing error message;
					FALSE:	supress error message.
			ifhdr:		IFH structure, returned.

	Description:	Read an annlyze header and converts it to an .ifh
			header.

	Return:		FAIL (-1):			the analyze .hdr file does not exist.
			MODE_USE_ANALYZE_HEADER (1):	voxel size is obtained from the analyze
							header.
			MODE_DEFAULT_VOXEL_SIZE (2):	use the default voxel size.

	Author:		Tom (Tongzeng) Yang
			(06/08/1993)
_____________________________________________________________________________*/

PUBLIC int hdr2ifh (analyze_header, print_message, ifhdr)
	char	*analyze_header;
	BOOLEAN	print_message;
	IFH	*ifhdr;
{
	float		read_header2 ();

	FILE		*fp_hdr;
	FILE		*fp_img;
	int		dummy [8];
	int		return_value;
	float		fbuffer;
	struct key_info	dimensions;

/*
 * Read the analyze header.
 */
	if ((fp_hdr = fopen (analyze_header, "rb")) == NULL)
	{
		if (print_message)
			fprintf (stderr, "Error: cannot open file %s for input\n", analyze_header);

		return FAIL;
	}

	fp_hdr = NULL;
	fp_img = NULL;
	fbuffer	= read_header2 (analyze_header, &fp_hdr, &fp_img, &dimensions, -1, dummy);
	if (fp_hdr == NULL || pkg_abs (fbuffer) < ERROR_BOUND) 
	{
		if (print_message)
			fprintf (stderr, "Error: in reading analyze header file %s.\n", analyze_header);

		return FAIL;
	}
	fclose (fp_hdr);

/*
 * Create the interfile HDR.
 */
	ifhdr->matrix_size [0]		= dimensions.x_dim;
	ifhdr->matrix_size [1]		= dimensions.y_dim;
	ifhdr->matrix_size [2]		= dimensions.z_dim;
	if (pkg_abs (dimensions.x_size) < ERROR_BOUND
	&& pkg_abs (dimensions.y_size) < ERROR_BOUND
	&& pkg_abs (dimensions.z_size) < ERROR_BOUND)
	{
		ifhdr->scaling_factor [0]	= DEFAULT_XSIZE;
		ifhdr->scaling_factor [1]	= DEFAULT_YSIZE;
		ifhdr->scaling_factor [2]	= DEFAULT_ZSIZE;
		return_value			= MODE_DEFAULT_VOXEL_SIZE;
	}
	else
	{
		ifhdr->scaling_factor [0]	= dimensions.x_size;
		ifhdr->scaling_factor [1]	= dimensions.y_size;
		ifhdr->scaling_factor [2]	= dimensions.z_size;
		return_value			= MODE_USE_ANALYZE_HEADER;
	}

	ifhdr->number_of_dimensions		= 3;
	ifhdr->number_of_bytes_per_pixel	= 2;
	strcpy (ifhdr->interfile, "INTERFILE");
	strcpy (ifhdr->number_format, "signed integer");
	strcpy (ifhdr->matrix_initial_element [0], "right");
	strcpy (ifhdr->matrix_initial_element [1], "posterior");
	strcpy (ifhdr->matrix_initial_element [2], "inferior");

	return return_value;
}
