/* $Id$
 * $Log$ */
/* ===============================================================================
 *  Module:         getimage_type.c
 *  Date:           04/15/92
 *  Author:         Tom Yang
 *  Description:    Determin type of image.
 *	Returns:		encoded_flag	-	true if encoded image
 *					num_slices		-	number of slices
 *  Return Value:   codes image type:
 *                  6	-	PETT VI
 *                  7	-	Super PETT I
 *                  8	-	Super PETT II
 *                  10	-	ECAT (953 or 961)
 *                  -1	-	Error
 *  History:
 *      13-Nov-95   Modified TOV
 *
 * ===============================================================================
 */
#include <archive.h>
#include <matrix7.h>
#include <ifh.h>
#include <imglist.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/getimage_type.c,v 1.19 1995/11/10 16:28:25 tom Exp tom $";

PUBLIC int      getimage_type (fd, image_name, encoded_flag, num_slices)
	int             fd;					/* file descriptor; assigned upon entry */
	char           *image_name;			/* filename */
	BOOLEAN        *encoded_flag;		/* true if encoded image */
	short          *num_slices;			/* number of slices */
{
	IFH             ifhdr;
	char           *char_header;		 /* character header */
	char           *extension;
	char            fileform[ENCODED_FILE_FORM_LEN + 1];
	char            ifhdr_file[MAXLINE];
	char            hdrfile[MAXLINE];
	char           *lines[IFH_MAXLEN];
	char            super_pet1_fileform[ENCODED_FILE_FORM_LEN + 1];
	int             ecat_image_type;
	int             file_type;
	int             nlines;
	int             scan_type;
	short          *int_header;		 /* integer header */
	static char    *function = "getimage_type";

/*	Create the interfile header file	*/

	memset (ifhdr_file, 0, MAXLINE);
	extension = strrchr (image_name, '.');
	if (extension != NULL)
		strncpy (ifhdr_file, image_name, strlen (image_name) - strlen (extension));
	else
		strcpy (ifhdr_file, image_name);

	strcpy (hdrfile, ifhdr_file);
	strcat (ifhdr_file, IFH_EXT);
	strcat (hdrfile, HDR_EXT);

/*	Allocate memory for image header	*/

	char_header = (char *) pkg_malloc (HEADER_SIZE, function, "char_header");

/*	Read first record and check if file is encoded	*/

	lseek (fd, 0L, 0);
	if (read (fd, char_header, HEADER_SIZE) < HEADER_SIZE) {
		pkg_message (PKG_ERROR, function, "fread",
			     "char_header: %s.\n", "Reading error");
		return ERROR_SCANNER;
	}
	int_header = (short *) char_header;
	strncpy (fileform, char_header, ENCODED_FILE_FORM_LEN);
	strncpy (super_pet1_fileform, char_header +
		 SUPER_PETT_I_NAME_POS, ENCODED_FILE_FORM_LEN);

/*	Get the image type */

	*encoded_flag = FALSE;

	if (strncmp (fileform, "ENCODED   ", ENCODED_FILE_FORM_LEN) == 0) {
		*encoded_flag = TRUE;
		*num_slices = int_header[ENCODED_NUMSLICES_POS];
		scan_type = int_header[ENCODED_PETTNUM_POS];
	} else if (strncmp (fileform, "SUPER PETT", 10) == 0) {
		*num_slices = int_header[SPETT_II_NUM_SLICES_POS];
		scan_type = SUPER_PETT_II_SCANNER;
	} else if (strncmp (fileform, "PC2048-15B", 10) == 0 ||
		   strncmp (fileform, "ECAT-953", 8) == 0 ||
		   strncmp (fileform, "PETT VI ", 8) == 0) {
		*num_slices = int_header[SPETT_II_NUM_SLICES_POS];
		scan_type = SUPER_PETT_II_OTHERS;
	} else if (strncmp (super_pet1_fileform, "SUPER PETT", 10) == 0) {
		*num_slices = int_header[SPETT_I_NUM_SLICES_POS];
		scan_type = SUPER_PETT_I_SCANNER;
	} else if (int_header[PETT6_POS] == PETT6_NUMBER) {
		if (int_header[PETT6_NUM_SLICES_POS] != 0) {
			*num_slices = int_header[PETT6_NUM_SLICES_POS];
		} else {
			*num_slices = PETT6_7SLICES;
		}
		scan_type = PETT6_SCANNER;

	} else if (((readifhdr (ifhdr_file, FALSE, IFH_MAXLEN, &nlines, lines) == SUCCEED
		     && getifhdr (ifhdr_file, FALSE, nlines, lines, &ifhdr) == SUCCEED)
		    || hdr2ifh (hdrfile, FALSE, &ifhdr) != FAIL)
		   && strcmp (ifhdr.interfile, "INTERFILE") == 0) {
		scan_type = INTERFILE;
		*num_slices = ifhdr.matrix_size[2];

/* Check if image is in ECAT format	*/

	} else if (isecat (image_name, &ecat_image_type, num_slices) == TRUE)
		scan_type = ecat_image_type;
	else {
		fprintf (stderr, "Image format in file %s is not known.\n", image_name);
		scan_type = ERROR_SCANNER;
	}
	free (char_header);
	return scan_type;
}

/* ===============================================================================
 *  Module:         getfile_type.c
 *  Date:           04/21/92
 *  Author:         Tom Yang
 *  Description:    Determine if file is ASCII or BINARY
 *	Notes:			This function does not appear to be used anywhere else.
 * ===============================================================================
 */

PUBLIC int      getfile_type (filename)
	char           *filename;
{
	FILE           *fp;
	int             ftype;
	int             c;

	if ((fp = fopen (filename, "r")) == NULL) {
		return INVALID;
	}
	while ((c = getc (fp)) != EOF && isascii (c));
	fclose (fp);
	return c == EOF ? ASCII : BINARY;
}
