/*$Id: GetImagePett6.c,v 1.9 1995/11/10 16:27:34 tom Exp $*/
/*$Log: GetImagePett6.c,v $
 * Revision 1.9  1995/11/10  16:27:34  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.8  1994/11/08  16:34:05  ty7777
 * Edited the comments.
 *
 * Revision 1.7  1993/11/03  17:51:51  ty7777
 * Added rcsid.
 *
 * Revision 1.6  1993/11/03  17:49:19  ty7777
 * Added rcsid.
 **/

/*___________________________________________________________________________________
	File Name:	GetImagePett6.c

	Function:	int GetImagePett6 (int fd, short slice_number, BOOLEAN encoded_flag, 
			char *header, short *slice_data)

	Arguments:	fd:		File descriptor for a PETT6 image file. Get this 
					descriptor before calling with the open () UNIX 
					system call. Close the descriptor after it is no 
					longer used.
			slice_number:	slice number, a short integer between 1 and 
					the maximum number of slices.
			encoded_flag:	True:	encoded PETT6 image; 
					False:	decoded PETT6 image.
			header:		Pointer to a short integer array. Allocate 
					memory before calling.
			slice_data:	Pointer to a short integer array. Allocate 
					memory before calling.
			Return Value:	int
					SUCCEED:	success;
					FAIL:		fail.
	Description:	Read a slice of PETT6 image and fill the fields of a header.

	Author:		Tom Yang

	Date:		04/15/1992
___________________________________________________________________________________*/

#include <stdio.h>
#include <unistd.h>
#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/GetImagePett6.c,v 1.9 1995/11/10 16:27:34 tom Exp $";

int GetImagePett6 (fd, slice_number, encoded_flag, header, slice_data)
int		fd;
short		slice_number;
BOOLEAN		encoded_flag;
char		*header;
short		*slice_data;
{
	ENCODEDHDR	*encoded_header;
	PETT6HDR	*pett6_header;
	char		*char_header;
	char		*databuf;
	int		data_address;
	int		header_address;
	int		i;			/* counter */
	int		j;			/* counter */
	long		N;			/* counter */
	long		size_bytes;
	static char	*function	= "GetImagePett6";
	short		*databuf16;
	short		tempx;		
	short		tempy;		

	if (encoded_flag)
	{
		/* 
		 * Allocate buffer for encoded Super Pett image header. 
		 */
		char_header	= (char *) pkg_malloc (HEADER_SIZE, function, "char_header");

		/* 
		 * Read first record to get record array.
		 */
		lseek (fd, 0L, 0);
		if (read (fd, char_header, HEADER_SIZE) < HEADER_SIZE) 
		{
			fprintf (stderr, "Header reading error in %s\n", function);
			return	FAIL;
		}

		encoded_header	= (ENCODEDHDR *) char_header; 

		header_address	= slice_number;	
		data_address	= encoded_header->rec [slice_number - 1];	

		if (slice_number < encoded_header->nslcs)
			size_bytes = (encoded_header->rec [slice_number] 
					- encoded_header->rec [slice_number - 1]) * RECORD_SIZE;
		else
			size_bytes = (encoded_header->nrecs 
					- encoded_header->rec [slice_number - 1]) * RECORD_SIZE;

		free (char_header);
	}
	else
	{
		header_address	= (slice_number - 1) * PETT6_RECS_PER_SLICE;
		size_bytes	= PETT6_SLICE_SIZE; 
	}

	databuf	= (char *) calloc (size_bytes, sizeof (char));
	if (databuf == (char *) NULL)
	{
		pkg_message (PKG_ERROR, function, "calloc",
		"Error: Can't allocate databuf.\n");
	}

	lseek (fd, header_address * RECORD_SIZE, 0);

	if (read (fd, header, HEADER_SIZE) < HEADER_SIZE) 
	{
		fprintf (stderr, "Cann't to header in %s\n", function);
		return	FAIL;
	}

	if (encoded_flag)
		lseek (fd, data_address * RECORD_SIZE, 0);

	if (read (fd, databuf, size_bytes) < size_bytes) 
	{
		/*
		fprintf (stderr, "Data reading error in %s\n", function);
		*/
		return	FAIL;
	}	
	databuf16	= (short *) databuf;

	/* 
	 * Decode encoded PETT6 image or 
	 * copy decoded PETT6 image to standard array. 
	 */
	if (encoded_flag)
	{
		N = 0;
		for (j = 0; j < PETT6_SLICE_HEIGHT; j ++)
		{
			tempx = databuf16 [N];
			N++;
			tempy = databuf16 [N];
			N++;
			if (tempy != 0 && tempx <= tempy)
			{
				for (i = tempx - 1; i < tempy; i++) 
				{
					slice_data [(j + HALF_DHEIGHT) * ENCODED_SLICE_WIDTH 
						+ HALF_DWIDTH + i]
						= databuf16 [N];
					N++;
				}
			}
		}
	}
	else
	{
		for (j = 0; j < PETT6_SLICE_HEIGHT; j++)
		{
			for (i = 0; i < PETT6_SLICE_HEIGHT; i++)
			{
				slice_data [(j + HALF_DHEIGHT) * ENCODED_SLICE_WIDTH 
						+ HALF_DWIDTH + i]
					= databuf16 [j * PETT6_SLICE_WIDTH + i];
			}
		}
	}

	free (databuf);

	pett6_header	= (PETT6HDR *) header;
	if (!encoded_flag && pett6_header->file_type == ECAT_FILE_TYPE && 
	pett6_header->ecat_orientation != 1)
	{
		reflection (ENCODED_SLICE_WIDTH,
		ENCODED_SLICE_HEIGHT, slice_data);
	}
	pett6_header->ecat_orientation	= 1;

	return SUCCEED;
}
