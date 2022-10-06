/*$Id: get_zorigin.c,v 1.3 1995/10/31 14:54:25 tom Exp $*/
/*$Log: get_zorigin.c,v $
 * Revision 1.3  1995/10/31  14:54:25  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.2  1995/10/13  16:11:57  ty7777
 * Use matrix7.h
 *
 * Revision 1.1  1995/10/10  15:33:27  ty7777
 * Initial revision
 **/

/*____________________________________________________________________________
 File Name:	get_zorigin.c

 Function Name:	void get_zorigin (char *imgname, int fd, int *zorigin, int *err_value)

 Arguments:	imgname,	image file name. Array size should be applocated
				to 256 in the calling program.
		fd,		image file descriptor, get it before calling
				by using the FORTRAN call getfd () or the C
				call open ().
		zorigin,	zorigin of image, returned.
		err_value,	TRUE (= 1): if unable to open the disk file, altered 
				upon return.

 Return Value:	void.

 Description:	This function is written for FORTRAN functions with a FORTRAN
 		main, though it can be also called by C functions. It gets the 
		zorigin for ECAT 953b images. The zorigin is a slice number which 
		is used in transformations between the image space and the 
		streotactic space.

 Called by:

 Calling:	pet_matrixx_open () in pet_matrixx.c of libpetutil.a.

 Author:	Tom Yang. 

 History:	Created by Tom Yang on 04/24/1994.
____________________________________________________________________________*/

#include <petutil.h>
#include <matrix7.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/get_zorigin.c,v 1.3 1995/10/31 14:54:25 tom Exp $";

void get_zorigin (imgname, fd, zorigin, err_value)
char	*imgname;
int	fd;
int	*zorigin;
int	*err_value;
{
	MatrixFile	*pet_matrix_open ();

	COMMENT_INFO	comment_info_data;
	MatrixFile	*matfptr;
	int		image_type;
	short		xdim;
	short		ydim;

	lseek (fd, 0L, 0);
	matfptr	= pet_matrix_open (fd, imgname, MAT_READ_ONLY, MAT_UNKNOWN_FTYPE);
	if (matfptr == NULL)
	{
		*err_value	= TRUE;
		fprintf (stderr, "Error: cannot open ECAT image file %s\n", imgname);
		return;
	}

	comment_info (matfptr->mhptr->study_description, &comment_info_data);
	*zorigin	= comment_info_data.zorigin;

	matrix_close (matfptr);

	*err_value	= FALSE;
}
