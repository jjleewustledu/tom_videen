/*$Id: updateifhdr.c,v 1.4 1995/11/10 16:29:33 tom Exp $*/
/*$Log: updateifhdr.c,v $
 * Revision 1.4  1995/11/10  16:29:33  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.3  1995/07/24  20:33:35  ty7777
 * Same as the last one.
 *
 * Revision 1.2  1995/07/21  20:37:06  ty7777
 * Added processing for atlas origin.
 *
 * Revision 1.1  1995/06/01  16:54:49  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		updateifhdr.c

  Function:	int updateifhdr (IFH ifhdr, BOOLEAN print_message, int nlines, char *lines [])

  Arguments:	ifhdr,		a structure defined in ifhdr.h, passed in.
		print_message,	TRUE: print error message if there is an error;
				FALSE: no error message will be printed, passed in.
		nlines,		number of lines from the .ifh file, passed in.
		lines,		contain all the information from the .ifh file arranged
				line by line, changed upon return.

  Description:	This function updates the array of lines from the information given in
		a structure.

  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang, May 31, 1995.

  History:	Created by Tom Yang on 05/30/95.
___________________________________________________________________________*/

#include <stdio.h>
#include <petutil.h>
#include <ifh.h>

static char     rcsid[] = "$Header: /home/npggw/tom/src/libnpg/RCS/updateifhdr.c,v 1.4 1995/11/10 16:29:33 tom Exp $";

PUBLIC int      updateifhdr (ifhdr, print_message, nlines, lines)
	IFH            *ifhdr;
	BOOLEAN         print_message;
	int             nlines;
	char           *lines[];
{
	char            assign[MAXLINE];
	char            cbuffer[MAXLINE];
	char            label[MAXLINE];
	char            original_label[MAXLINE];
	char            original_parameter[MAXLINE];
	char            parameter[MAXLINE];
	char            parameter_string[MAXLINE];
	char           *value;
	int             i;
	int             index;

	/*
	 * Parse the data.
	 */
	for (index = 0; index < nlines; index++) {
		if (strncmp (lines[index], "#", 1) == 0)
			continue;

		if ((value = strstr (lines[index], ":=")) != NULL) {
			memset (label, 0, MAXLINE);
			memset (parameter_string, 0, MAXLINE);
			strncpy (label, lines[index], strlen (lines[index]) - strlen (value) + 2);
			strcat (label, " ");

			if (strstr (label, "INTERFILE") != NULL) {
				continue;
			}
			if (sscanf (value, "%s %s", assign, parameter) != 2)
				continue;

			strcpy (lines[index], label);
			Lowered (label, strlen (label));

			if (strstr (label, "version of keys") != NULL) {
				strcat (lines[index], ifhdr->version_of_keys);

				continue;
			}
			if (strstr (label, "image modality") != NULL) {
				strcat (lines[index], ifhdr->image_modality);

				continue;
			}
			if (strstr (label, "originating system") != NULL) {
				strcat (lines[index], ifhdr->originating_system);

				continue;
			}
			if (strstr (label, "conversion program") != NULL) {
				strcat (lines[index], ifhdr->conversion_program);

				continue;
			}
			if (strstr (label, "program version") != NULL) {
				strcat (lines[index], ifhdr->program_version);

				continue;
			}
			if (strstr (label, "program date") != NULL) {
				strcat (lines[index], ifhdr->program_date);

				continue;
			}
			if (strstr (label, "program author") != NULL) {
				strcat (lines[index], ifhdr->program_author);

				continue;
			}
			if (strstr (label, "original institution") != NULL) {
				strcat (lines[index], ifhdr->original_institution);

				continue;
			}
			if (strstr (label, "name of data file") != NULL) {
				strcat (lines[index], ifhdr->name_of_data_file);

				continue;
			}
			if (strstr (label, "subject id") != NULL) {
				strcat (lines[index], ifhdr->subject_id);

				continue;
			}
			if (strstr (label, "study date") != NULL) {
				strcat (lines[index], ifhdr->study_date);

				continue;
			}
			if (strstr (label, "number format") != NULL) {
				strcat (lines[index], ifhdr->number_format);

				continue;
			}
			if (strstr (label, "number of bytes per pixel") != NULL) {
				sprintf (cbuffer, "%d", ifhdr->number_of_bytes_per_pixel);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "number of dimensions") != NULL) {
				sprintf (cbuffer, "%d", ifhdr->number_of_dimensions);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "matrix size [1]") != NULL) {
				sprintf (cbuffer, "%d", ifhdr->matrix_size[0]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "matrix size [2]") != NULL) {
				sprintf (cbuffer, "%d", ifhdr->matrix_size[1]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "matrix size [3]") != NULL) {
				sprintf (cbuffer, "%d", ifhdr->matrix_size[2]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "scaling factor (mm/pixel) [1]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->scaling_factor[0]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "scaling factor (mm/pixel) [2]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->scaling_factor[1]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "scaling factor (mm/pixel) [3]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->scaling_factor[2]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "matrix initial element [1]") != NULL) {
				strcat (lines[index], ifhdr->matrix_initial_element[0]);

				continue;
			}
			if (strstr (label, "matrix initial element [2]") != NULL) {
				strcat (lines[index], ifhdr->matrix_initial_element[1]);

				continue;
			}
			if (strstr (label, "matrix initial element [3]") != NULL) {
				strcat (lines[index], ifhdr->matrix_initial_element[2]);

				continue;
			}
			if (strstr (label, "atlas origin [1]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->atlas_origin[0]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "atlas origin [2]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->atlas_origin[1]);
				strcat (lines[index], cbuffer);

				continue;
			}
			if (strstr (label, "atlas origin [3]") != NULL) {
				sprintf (cbuffer, "%f", ifhdr->atlas_origin[2]);
				strcat (lines[index], cbuffer);

				continue;
			}
		}
	}

	return SUCCEED;
}
