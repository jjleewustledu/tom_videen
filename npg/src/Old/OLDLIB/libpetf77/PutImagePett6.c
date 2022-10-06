/*$Id: PutImagePett6.c,v 1.2 1995/10/31 14:54:18 tom Exp $*/
/*$Log: PutImagePett6.c,v $
 * Revision 1.2  1995/10/31  14:54:18  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/10/10  15:30:03  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________________
 *
 *	File Name:	PutImagePett6.c
 *	Function Name:	PutImagePett6
 *	Arguments:	pet_slice_image:	This is an array of short integers 
 *						storing the pixel values of a pet slice. 
 *						The user of the function ia responsible
 *						for allocating memory and assigning
 *						values for the array before calling
 *						this function.
 *			slice_index:		Slice number of a pet image.
 *			pett6_header:		A PETI6HDR structure. See the header
 *						file "petimage.h" for details. The 
 *						application programmer should allocate
 *						and fill the structure before calling
 *						the function.
 *			pet_image_outname:	File name for storing the processed
 *						pet slice.
 *	Return Value:	void
 *	Description:	Given an array of pixels corresponding to a slice from a pet
 *			image with index "slice_index", this function writes the pet 
 *			image data and a PETT6 header to the proper place in the out-
 *			put image file "pet_image_outname". The header is setup in 
 *			such a way that the fields corresponding to those in the 
 *			structure "pet_slice_header" are filled while all the other
 *			fields are set to zero. If the slice_index is the first slice,
 *			we check whether the user specified output file exists. For all
 *			other slices, we check the size of the output image and make 
 *			sure that before we append the current slice to the output
 *			file, there are already (slice_index -1) slices in the output
 *			image file.
 *	Function Used:	ieee2ps
 *	Author:		Tom Yang
 *	Date:		03/24/1992
 *
___________________________________________________________________________________*/

#include <stdio.h>
#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/PutImagePett6.c,v 1.2 1995/10/31 14:54:18 tom Exp $";

#define	PERMS	0755		/* protection mode: rwx for user; rx for both
				   group and others. */

void PutImagePett6 (pet_slice_image, slice_index, pett6_header, pet_image_outname)
short		*pet_slice_image;
short		slice_index;
PETT6HDR	*pett6_header;				/* PETT6 header structure */
char		*pet_image_outname;
{
	char		pett6_garbage [PETT6_GARBAGE_SIZE];	/* garbage at end of pixel data */
	int		i;					/* counter */
	FILE		*existfp;				/* file pointer for checking 
								   existence of output image 
								   file */ 
	FILE		*outfp;					/* file pointer for output 
								   image file */

	/*
	 * Check whether the specified output file exists before any
	 * writing.
	 */
	if (slice_index == 1)
	{
		if ((existfp = fopen (pet_image_outname, "r")) != NULL)
		{
			fprintf (stderr, "Output file %s is not empty when writing started\n", 
					pet_image_outname);
			fclose (existfp);
			exit (0);
		}
		fclose (existfp);
	}

	/*
	 * Open or create if necessary the output file for writing.
	 */
	outfp	= fopen (pet_image_outname, "a+b");

	/*
	 * Check whether we have the right size.
	 */
	if (slice_index > 1)
	{
		if (ftell (outfp) != (slice_index - 1) * PETT6_RECS_PER_SLICE * RECORD_SIZE)
		{
			fprintf (stderr, "Output file %s does not have the right size\n", 
					pet_image_outname);
			fprintf (stderr, "slice index = %d\n", slice_index);
			exit (1);
		}
	}

	/*
	 * Write the slice header.
	 */
	i	= fwrite (pett6_header, HEADER_SIZE, 1, outfp);
	if (i != 1)
		printf ("Error: Only %d bytes header are written.\n", i);

	/*
	 * Write the slice pixel data.
	 */
	i	= fwrite (pet_slice_image, PETT6_SLICE_SIZE, 1, outfp);
	if (i != 1)
		printf ("Error: Only %d bytes data are written.\n", i);

	/*
	 * Fill the tail with trash.
	 */
	i	= fwrite (pett6_garbage, PETT6_GARBAGE_SIZE, 1, outfp);
	if (i != 1)
		printf ("Error: Only %d bytes data are written.\n", i);

	/*
	 * Free memory and close output file.
	 */
	fclose (outfp);
}
