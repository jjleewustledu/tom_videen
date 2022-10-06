/*$Id: getifhdr.c,v 1.5 1995/10/30 22:39:01 tom Exp $*/
/*$Log: getifhdr.c,v $
 * Revision 1.5  1995/10/30  22:39:01  tom
 * change include to remove "petutil"
 *
 * Revision 1.4  1995/07/24  18:40:38  ty7777
 * Added initialization for atlas origin.
 *
 * Revision 1.3  1995/07/24  18:24:04  ty7777
 * Added atlas origin.
 *
 * Revision 1.2  1995/07/21  20:36:47  ty7777
 * Added atlas origin.
 *
 * Revision 1.1  1995/06/01  16:17:03  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		getifhdr.c

  Function:	int getifhdr (char *ifhdrfile, BOOLEAN print_message, int nlines, 
		char *lines [], IFH *ifhdr)

  Arguments:	ifhdrfile,	interfile header file. Used only for locating error
				messages.
		print_message,	TRUE: print error message if there is an error;
				FALSE: no error message will be printed, passed in.
		nlines,		number of lines from the .ifh file, passed in.
		lines,		contain all the information from the .ifh file arranged
				line by line, passed in.
		ifhdr,		a structure defined in ifhdr.h, returned.

  Description:	This function extracts information from the interfile and puts the 
		information in a structure.

  Return:	SUCCEED for successful; FAIL for error.

  Author:	Tom Yang, May 30, 1995.

  History:	Created by Tom Yang on 05/30/95. 
___________________________________________________________________________*/

#include <stdio.h>
#include <petutil.h>
#include <ifh.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libhp/RCS/getifhdr.c,v 1.5 1995/10/30 22:39:01 tom Exp $";

PRIVATE void error_message ();
PRIVATE int true_length ();

PRIVATE void error_message (keyword, parameter, file)
	char	*keyword;
	char	*parameter;
	char	*file;
{
	fprintf (stderr, 
	"Error: wrong parameter (%s) for keyword (%s) in parameter file (%s).\n",
	parameter, keyword, file);
}

PRIVATE int true_length (string)
	char	*string;
{
	int	i;
	int	length;

	length	= strlen (string);

	for (i = length - 1; i >= 0; i--)
	{
		if (isgraph (string [i]))
			break;
	}

	return i + 1;
}

PUBLIC int getifhdr (ifhdrfile, print_message, nlines, lines, ifhdr) 
	char	*ifhdrfile;
	BOOLEAN	print_message;
	int	nlines;
	char	*lines [];
	IFH	*ifhdr;
{
	char	assign [MAXLINE];
	char	*cbuffer;
	char	label [MAXLINE];
	char	original_label [MAXLINE];
	char	original_parameter [MAXLINE];
	char	parameter [MAXLINE];
	char	parameter_string [MAXLINE];
	char	*value;
	int	i;
	int	index;
	int	length;

	/*
	 * Initialization
	 */
	memset (ifhdr, 0, sizeof (IFH)); 

	ifhdr->atlas_origin [0]	= DEFAULT_XORIGIN;
	ifhdr->atlas_origin [1]	= DEFAULT_YORIGIN;
	ifhdr->atlas_origin [2]	= DEFAULT_ZORIGIN;

	/*
	 * Parse the data.
	 */
	for (index = 0; index < nlines; index++)
	{
		if (strncmp (lines [index], "#", 1) == 0)
			continue;

		if ((value = strstr (lines [index], ":=")) != NULL)
		{
			memset (label, 0, MAXLINE); 
			memset (parameter_string, 0, MAXLINE); 
			strncpy (label, lines [index], strlen (lines [index]) - strlen (value));
			length	= true_length (label);
			for (i = length; i < MAXLINE; i++)
				label [i]	= '\0';

			if (strstr (label, "INTERFILE") != NULL)
			{
				strcpy (ifhdr->interfile, label);

				continue;
			}

			if (sscanf (value, "%s %s", assign, parameter) != 2)
				continue;

			cbuffer	= strstr (value, parameter);
			length	= true_length (cbuffer);
			strncpy (parameter_string, cbuffer, length); 
			for (i = length; i < MAXLINE; i++)
				parameter_string [i]	= '\0';

			strcpy (original_label, label);
			strcpy (original_parameter, parameter);
			Lowered (label, strlen (label));
			Lowered (parameter, strlen (parameter));

			if (strstr (label, "version of keys") != NULL)
			{
				strcpy (ifhdr->version_of_keys, original_parameter);

				continue;
			}

			if (strstr (label, "image modality") != NULL)
			{
				strcpy (ifhdr->image_modality, original_parameter);

				continue;
			}

			if (strstr (label, "originating system") != NULL)
			{
				strcpy (ifhdr->originating_system, original_parameter);

				continue;
			}

			if (strstr (label, "conversion program") != NULL)
			{
				strcpy (ifhdr->conversion_program, original_parameter);

				continue;
			}

			if (strstr (label, "program version") != NULL)
			{
				strcpy (ifhdr->program_version, parameter);

				continue;
			}

			if (strstr (label, "program date") != NULL)
			{
				strcpy (ifhdr->program_date, original_parameter);

				continue;
			}

			if (strstr (label, "program author") != NULL)
			{
				strcpy (ifhdr->program_author, original_parameter);

				continue;
			}

			if (strstr (label, "original institution") != NULL)
			{
				strcpy (ifhdr->original_institution, parameter_string);

				continue;
			}

			if (strstr (label, "name of data file") != NULL)
			{
				strcpy (ifhdr->name_of_data_file, original_parameter);

				continue;
			}

			if (strstr (label, "subject id") != NULL)
			{
				strcpy (ifhdr->subject_id, original_parameter);

				continue;
			}

			if (strstr (label, "study date") != NULL)
			{
				strcpy (ifhdr->study_date, original_parameter);

				continue;
			}

			if (strstr (label, "number format") != NULL)
			{
				strcpy (ifhdr->number_format, parameter_string);

				continue;
			}

			if (strstr (label, "number of bytes per pixel") != NULL)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					ifhdr->number_of_bytes_per_pixel	= atoi (parameter);
				else
				{
					if (print_message)
						error_message ("number of bytes per pixel", 
						original_parameter, ifhdrfile);
					return FAIL;
				}

				continue;
			}

			if (strstr (label, "number of dimensions") != NULL)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					ifhdr->number_of_dimensions	= atoi (parameter);
				else
				{
					if (print_message)
						error_message ("number of dimensions", 
						original_parameter, ifhdrfile);
					return FAIL;
				}

				continue;
			}

			if (strstr (label, "matrix size [1]") != NULL)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					ifhdr->matrix_size [0]	= atoi (parameter);
				else
				{
					if (print_message)
						error_message ("matrix size [1]", 
						original_parameter, ifhdrfile);
					return FAIL;
				}

				continue;
			}

			if (strstr (label, "matrix size [2]") != NULL)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					ifhdr->matrix_size [1]	= atoi (parameter);
				else
				{
					if (print_message)
						error_message ("matrix size [2]", 
						original_parameter, ifhdrfile);
					return FAIL;
				}

				continue;
			}

			if (strstr (label, "matrix size [3]") != NULL)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					ifhdr->matrix_size [2]	= atoi (parameter);
				else
				{
					if (print_message)
						error_message ("matrix size [3]", 
						original_parameter, ifhdrfile);
					return FAIL;
				}

				continue;
			}

			if (strstr (label, "scaling factor (mm/pixel) [1]") != NULL)
			{
				ifhdr->scaling_factor [0]	= atof (parameter);

				continue;
			}

			if (strstr (label, "scaling factor (mm/pixel) [2]") != NULL)
			{
				ifhdr->scaling_factor [1]	= atof (parameter);

				continue;
			}

			if (strstr (label, "scaling factor (mm/pixel) [3]") != NULL)
			{
				ifhdr->scaling_factor [2]	= atof (parameter);

				continue;
			}

			if (strstr (label, "matrix initial element [1]") != NULL)
			{
				strcpy (ifhdr->matrix_initial_element [0], original_parameter);

				continue;
			}

			if (strstr (label, "matrix initial element [2]") != NULL)
			{
				strcpy (ifhdr->matrix_initial_element [1], original_parameter);

				continue;
			}

			if (strstr (label, "matrix initial element [3]") != NULL)
			{
				strcpy (ifhdr->matrix_initial_element [2], original_parameter);

				continue;
			}

			if (strstr (label, "atlas origin [1]") != NULL)
			{
				ifhdr->atlas_origin [0]	= atof (parameter);

				continue;
			}

			if (strstr (label, "atlas origin [2]") != NULL)
			{
				ifhdr->atlas_origin [1]	= atof (parameter);

				continue;
			}

			if (strstr (label, "atlas origin [3]") != NULL)
			{
				ifhdr->atlas_origin [2]	= atof (parameter);

				continue;
			}

		}
	}

	return SUCCEED;
}
