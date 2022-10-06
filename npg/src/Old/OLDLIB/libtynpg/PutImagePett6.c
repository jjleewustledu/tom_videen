/*$Id: PutImagePett6.c,v 1.6 1994/11/09 17:24:56 ty7777 Exp $*/
/*$Log: PutImagePett6.c,v $
 * Revision 1.6  1994/11/09  17:24:56  ty7777
 * Added the function prototype.
 *
 * Revision 1.5  1994/03/03  19:57:33  ty7777
 * Replace id with header.
 *
 * Revision 1.4  1993/10/22  21:36:06  ty7777
 * Changed pett6_header data type from PETT6_HEADER to short integer array
 * so Avi can use it.
 **/

/*___________________________________________________________________________________
 	File Name:	PutImagePett6.c

	Function:	void PutImagePett6 (short *pet_slice_image, short slice_index, 
				short *header, char *pet_image_outname)

	Arguments:	pet_slice_data:		This is an array of short integers 
						storing the pixel values of a pet slice. 
						The user of the function ia responsible
						for allocating memory and assigning
						values for the array before calling
						this function. The dimension of the
						array is 
						PETT6_SLICE_WIDTH x PETT6_SLICE_HEIGHT.
			slice_index:		Slice number of a pet image, start at 1.
			header:			pett6 header. It is a short integer 
						array. The application programmer 
						should allocate and fill the structure 
						before calling the function.
			pet_image_outname:	File name for storing the processed
			pet slice.

	Return Value:	void

	Description:	Given an array of pixels corresponding to a slice from a pet
			image with index "slice_index", this function writes the pet 
			image data and a PETT6 header to the proper place in the out-
			put image file "pet_image_outname". The header is setup in 
			such a way that the fields corresponding to those in the 
			structure "pet_slice_header" are filled while all the other
			fields are set to zero. If the slice_index is the first slice,
			we check whether the user specified output file exists. For all
			other slices, we check the size of the output image and make 
			sure that before we append the current slice to the output
			file, there are already (slice_index -1) slices in the output
			image file.

	Calling:	ieee2ps

	Author:		Tom Yang

	Date:		03/24/1992
___________________________________________________________________________________*/

#include <stdio.h>
#include <unistd.h>
#include <petutil/petutil.h>

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/PutImagePett6.c,v 1.6 1994/11/09 17:24:56 ty7777 Exp $";

#define	PERMS	0755		/* protection mode: rwx for user; rx for both group and others. */

void PutImagePett6 (pet_slice_image, slice_index, header, pet_image_outname)
	short	*pet_slice_image;
	short	slice_index;
	short	*header;				
	char	*pet_image_outname;
{
	PETT6HDR	*pett6_header;				/* PETT6 header structure */
	char		pett6_garbage [PETT6_GARBAGE_SIZE];	/* garbage at end of pixel data */
	int		i;					/* counter */
	FILE		*fp;					/* file pointer for output image file */

	pett6_header	= (PETT6HDR *) header;

	/*
	 * Check whether the specified output file exists before any
	 * writing.
	 */
	if (slice_index == 1)
	{
		if ((fp = fopen (pet_image_outname, "r")) != NULL)
		{
			fprintf (stderr, "Output file %s is not empty when writing started\n", 
					pet_image_outname);
			fclose (fp);
			exit (0);
		}
		fclose (fp);
	}

	/*
	 * Open or create if necessary the output file for writing.
	 */
	fp	= fopen (pet_image_outname, "a+b");

	/*
	 * Check whether we have the right size.
	 */
	if (slice_index > 1)
	{
		fseek (fp, 0, SEEK_END);
		if (ftell (fp) != (slice_index - 1) * PETT6_RECS_PER_SLICE * RECORD_SIZE)
		{
			fprintf (stderr, "Output file %s does not have the right size.\n", 
					pet_image_outname);
			fprintf (stderr, "slice index = %d\n", slice_index);
			return;
		}
	}

	/*
	 * Write the slice header.
	 */
	i	= fwrite (pett6_header, HEADER_SIZE, 1, fp);
	if (i != 1)
		printf ("Error: Only %d bytes header are written.\n", i);

	/*
	 * Write the slice pixel data.
	 */
	i	= fwrite (pet_slice_image, PETT6_SLICE_SIZE, 1, fp);
	if (i != 1)
		printf ("Error: Only %d bytes data are written.\n", i);

	/*
	 * Fill the tail with trash.
	 */
	i	= fwrite (pett6_garbage, PETT6_GARBAGE_SIZE, 1, fp);
	if (i != 1)
		printf ("Error: Only %d bytes data are written.\n", i);

	/*
	 * Free memory and close output file.
	 */
	fclose (fp);
}
