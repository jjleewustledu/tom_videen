/*$Id: getimage_type.c,v 1.17 1995/10/30 22:39:01 tom Exp $*/
/*$Log: getimage_type.c,v $
 * Revision 1.17  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.16  1995/10/13  17:15:53  ty7777
 * Use matrix7.h.
 *
 * Revision 1.15  1995/09/28  18:32:26  ty7777
 * Same as the last one.
 *
 * Revision 1.14  1995/06/14  20:09:27  ty7777
 * Added calling for isecat ().
 *
 * Revision 1.13  1995/06/13  19:19:42  ty7777
 * Same as the last one.
 *
 * Revision 1.12  1995/06/13  17:19:50  ty7777
 * Added processing for analyze header files.
 *
 * Revision 1.11  1995/06/01  18:46:56  ty7777
 * Added getifhdr ().
 *
 * Revision 1.10  1995/05/19  16:32:41  ty7777
 * Added MRI processing.
 *
 * Revision 1.9  1995/05/19  14:31:21  ty7777
 * Use IFH_EXT.
 *
 * Revision 1.8  1995/05/18  21:47:16  ty7777
 * Added MRI processing.
 *
 * Revision 1.7  1994/03/03  20:13:25  ty7777
 * Replace id with header so that HP ident can recongnise.
 *
 * Revision 1.6  1993/10/25  21:16:30  ty7777
 * Added initialization for matval.
 *
 * Revision 1.5  1993/09/29  16:43:47  ty7777
 * Added getfile_type.
 *
 * Revision 1.4  1993/09/22  18:54:02  ty7777
 * Added getfiule_type () and delted matval initializations.
 *
 * Revision 1.3  1993/08/06  16:28:13  ty7777
 * Changed error message.
 *
 * Revision 1.2  1993/08/06  16:21:04  ty7777
 * Same as the last one.
 *
 * Revision 1.1  1993/03/03  20:34:55  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid[]="@(#)getimage_type.c 10/09/92 Copyright Neural Pet Group, Washington University 1992";
#endif
static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getimage_type.c,v 1.17 1995/10/30 22:39:01 tom Exp $";

/*___________________________________________________________________________________
 *
 *	File Name:	getimage_type.c
 *	Function Name:	getimage_type
 *	Arguments:	fd:		File descriptor, assigned before calling.	
 *			image_name:	Image name.
 *			encoded_flag:	True: encoded image; False: decoded image;
 *					returned. 
 *			num_slices:	Number of slices, returned.
 *	Return Value:	int:		Image type.			
 *					PETT VI: 6;	Super PETT I: 7;
 *					ECAT: 10;	Super PETT II: 8;
 *					Error: -1.
 *	Description:	Read an image file and determine its image type.
 *	Called by:	GETIMG () in getimg.f of libpetutil.a.
 *	Calling:	OpenEcat () in OpenEcat.c of libpetutil.a	
 *	Author:		Tom Yang
 *	History:	Created by Tom Yang on 04/15/1992.
 *
___________________________________________________________________________________*/

#include <archive.h>
#include <matrix7.h>
#include <ifh.h>
#include <imglist.h>

PUBLIC int getimage_type (fd, image_name, encoded_flag, num_slices)
int		fd;
char		*image_name;
BOOLEAN		*encoded_flag;
short		*num_slices;
{
	IFH		ifhdr;
	char		*char_header;		/* character header */
	char		*extension;
	char		fileform [ENCODED_FILE_FORM_LEN + 1];			
	char		ifhdr_file [MAXLINE];
	char		hdrfile [MAXLINE];
	char		*lines [IFH_MAXLEN];
	char		super_pet1_fileform [ENCODED_FILE_FORM_LEN + 1];			
	int		file_type;
	int		nlines;
	int		scan_type;
	short		*int_header;		/* integer header */
	static char	*function	= "getimage_type";

	/* 
	 * Create the interfile header file.
	 */
	memset (ifhdr_file, 0, MAXLINE); 
	extension	= strrchr (image_name, '.');
	if (extension != NULL)
		strncpy (ifhdr_file, image_name, strlen (image_name)  - strlen (extension));
	else
		strcpy (ifhdr_file, image_name);

	strcpy (hdrfile, ifhdr_file);
	strcat (ifhdr_file, IFH_EXT);
	strcat (hdrfile, HDR_EXT);

	/* 
	 * Allocate memory for image header. 
	 */
	char_header = (char *) pkg_malloc (HEADER_SIZE, function, "char_header");

	/* 
	 * Read first record and check if file is encoded. 
	 */
	lseek (fd, 0L, 0);
	if (read (fd, char_header, HEADER_SIZE) < HEADER_SIZE) 
	{
		pkg_message (PKG_ERROR, function, "fread", 
			"char_header: %s.\n", "Reading error");
		return	ERROR_SCANNER;
	}

	int_header	= (short *) char_header; 
	strncpy (fileform, char_header, ENCODED_FILE_FORM_LEN);
	strncpy (super_pet1_fileform, char_header + 
		SUPER_PETT_I_NAME_POS, ENCODED_FILE_FORM_LEN);

	/*
	 * Get the image type.
	 */
	*encoded_flag	= FALSE;

	if (strncmp (fileform, "ENCODED   ", ENCODED_FILE_FORM_LEN) == 0)
	{
		*encoded_flag	= TRUE;
		*num_slices	= int_header [ENCODED_NUMSLICES_POS];
		scan_type	= int_header [ENCODED_PETTNUM_POS]; 
	}
	else if (strncmp (fileform, "SUPER PETT", 10) == 0)
	{
		*num_slices	= int_header [SPETT_II_NUM_SLICES_POS];
		scan_type	= SUPER_PETT_II_SCANNER;
	}
	else if (strncmp (fileform, "PC2048-15B", 10)	== 0 || 
		strncmp (fileform, "ECAT-953", 8)	== 0 ||
		strncmp (fileform, "PETT VI ", 8)	== 0)
	{
		*num_slices	= int_header [SPETT_II_NUM_SLICES_POS];
		scan_type	= SUPER_PETT_II_OTHERS;
	}
	else if (strncmp (super_pet1_fileform, "SUPER PETT", 10) == 0)
	{
		*num_slices	= int_header [SPETT_I_NUM_SLICES_POS];
		scan_type	=  SUPER_PETT_I_SCANNER;
	}
	else if (int_header [PETT6_POS] == PETT6_NUMBER) 
	{
		if (int_header [PETT6_NUM_SLICES_POS] != 0)
		{
			*num_slices	= int_header [PETT6_NUM_SLICES_POS];
		}
		else
		{
			*num_slices	= PETT6_7SLICES;
		}

		scan_type	=  PETT6_SCANNER;	
	}
	else if (((readifhdr (ifhdr_file, FALSE, IFH_MAXLEN, &nlines, lines) == SUCCEED
	&& getifhdr (ifhdr_file, FALSE, nlines, lines, &ifhdr) == SUCCEED)
	|| hdr2ifh (hdrfile, FALSE, &ifhdr) != FAIL)
	&& strcmp (ifhdr.interfile, "INTERFILE") == 0)
	{
			scan_type	= INTERFILE;
			*num_slices	= ifhdr.matrix_size [2];
	}
	else if (isecat (image_name, num_slices) == TRUE)
	/* 
 	 * Check if image is in ECAT format.
 	 */
	{
		scan_type	= ECAT_SCANNER;
	}
	else
	{
		fprintf (stderr, "Image format in file %s is not known.\n", image_name);

		scan_type	=  ERROR_SCANNER;
	}

	free (char_header);

	return	scan_type;
}

/*
 * Check if a file is binary or ascii.
 */

PUBLIC int getfile_type (filename)
	char	*filename;
{
	FILE	*fp;
	int	ftype;
	int	c;

	if ((fp = fopen (filename, "r" )) == NULL) 
	{
		return INVALID;
	}

	while ((c = getc (fp)) != EOF && isascii (c));
	fclose (fp);

	return c == EOF? ASCII: BINARY;
}
