/*$Id: GetImageSuperPett.c,v 1.4 1995/10/30 22:39:01 tom Exp $*/
/*$Log: GetImageSuperPett.c,v $
 * Revision 1.4  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.3  1994/11/08  17:17:10  ty7777
 * Modified the comments.
 *
 * Revision 1.2  1993/11/03  17:53:45  ty7777
 * Added rcsid.
 **/

/*___________________________________________________________________________________
	File Name:	GetImageSuperPett.c
	 
	Function Name:	int GetImageSuperPett (int fd, short slice_number, BOOLEAN 
				encoded_flag, char *header, short *slice_data)

	Arguments:	fd:		File descriptor for a Super PETT I or Super 
					PETT II image file. Get this descriptor before 
					calling with the open () UNIX system call. 
					Close the descriptor after it is no longer 
					used.
			slice_number:	slice number, a short integer between 1 and 
					the maximum number of slices.
			encoded_flag:	True:	encoded Super PETT I or II image; 
					False:	decoded Super PETT I or II image.
			header:		Pointer to a short integer array. Allocate 
					memory before calling.
			slice_data:	Pointer to a short integer array. Allocate 
					memory before calling.
	Return Value:	int
			SUCCEED:	success;
			FAIL:		fail.
	Description:	Read a slice of Super PETT I or Super PETT II image and fill 
			the fields of a header.
	Author:		Tom Yang
	Date:		04/16/1992
___________________________________________________________________________________*/

#include <stdio.h>
#include <unistd.h>
#include <petutil.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/GetImageSuperPett.c,v 1.4 1995/10/30 22:39:01 tom Exp $";

int GetImageSuperPett (fd, slice_number, encoded_flag, header, slice_data)
int		fd;
short		slice_number;
BOOLEAN		encoded_flag;
char		*header;
short		*slice_data;
{
	static char	*function	= "GetImageSuperPett";
	char		*char_header;
	ENCODEDHDR	*encoded_header;
	int		header_address;
	int		data_address;
	long		size_bytes;
	char		*databuf;
	short		*databuf16;
	long		N;			/* counter */
	short		tempx;		
	short		tempy;		
	int		i;			/* counter */
	int		j;			/* counter */

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
		header_address	= (slice_number - 1) * ENCODED_RECS_PER_SLICE;
		size_bytes	= ENCODED_SLICE_DIM * sizeof (short); 
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

	if (read (fd, databuf, size_bytes) != size_bytes) 
	{
		fprintf (stderr, "Data reading error in %s\n", function);
		return	FAIL;
	}	
	databuf16	= (short *) databuf;

	/* 
	 * Decode encoded Super PETT image or 
	 * copy decoded Super PETT image to standard array. 
	 */
	if (encoded_flag)
	{
		N = 0;
		for (j = 0; j < ENCODED_SLICE_HEIGHT; j ++)
		{
			tempx = databuf16 [N];
			N++;
			tempy = databuf16 [N];
			N++;
			if (tempy != 0 && tempx <= tempy)
			{
				for (i = tempx - 1; i < tempy; i++) 
				{
					slice_data [j * ENCODED_SLICE_WIDTH + i]
						= databuf16 [N];
					N++;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < ENCODED_SLICE_DIM - 1; i++)
		{
			slice_data [i]	= databuf16 [i];
		}
	}

	free (databuf);

	return SUCCEED;
}
