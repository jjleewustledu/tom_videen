/*  $Id: get_ifh.c,v 1.9 2002/12/17 21:55:15 tom Exp $
 *  $Log: get_ifh.c,v $
 * Revision 1.9  2002/12/17  21:55:15  tom
 * fix pointer/cast errors found with gcc
 *
 *  Revision 1.8  2002/03/28 16:01:08  tom
 *  read study_date
 *
 * Revision 1.7  2001/11/21  17:01:28  tom
 * remove CR from strings
 *
 * Revision 1.6  2000/12/07  22:24:17  tom
 * fill additional ifh parameters
 *
 * Revision 1.5  2000/10/12  15:24:56  tom
 * *** empty log message ***
 *
 * Revision 1.4  2000/09/28  14:44:29  tom
 * calloc
 *
 * Revision 1.3  2000/07/18  20:08:52  tom
 * *** empty log message ***
 *
 * Revision 1.2  2000/07/18  17:01:59  tom
 * initialize ifh dimensions
 *
 * Revision 1.1  2000/07/18  15:40:27  tom
 * Initial revision
 * */
/* =============================================================================
 *	Function:		get_ifh
 *  Date:           Jul 2002
 *  Author:         Tom Videen
 *	Description:	Read Interfile header and store in IFheader.
 * =============================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimage/PreVersion/RCS/get_ifh.c,v 1.9 2002/12/17 21:55:15 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <image.h>
#include <interfile.h>

#define MAXSTR 256

int             get_ifh (file)
	ImageFile      *file;
{
	MainHeader_if  *ifh;
	char            str[MAXSTR];
	char            hdrfil[MAXSTR];
	char           *ptr, *ptr2;
	float           fval;
	int             ival;

	if (file->ih == NULL) {
		file->ih = (IFheader *) calloc (1, sizeof (IFheader));
		if (file->ih == NULL) {
			(void) fprintf (stderr, "ERROR [get_ifh]: Could not calloc memory for Interfile header\n");
			return (ERROR);
		}
		file->ih->mh = NULL;
	}
	if (file->ih->mh == NULL) {
		file->ih->mh = (MainHeader_if *) calloc (1, sizeof (MainHeader_if));
		if (file->ih->mh == NULL) {
			(void) fprintf (stderr, "ERROR [get_ifh]: Could not calloc memory for Interfile header\n");
			return (ERROR);
		}
		file->ih->mh->modality = NULL;
		file->ih->mh->sw_name = NULL;
		file->ih->mh->orig_system = NULL;
		file->ih->mh->institution = NULL;
		file->ih->mh->data_file = NULL;
		file->ih->mh->subject_id = NULL;
		file->ih->mh->study_date = NULL;
		file->ih->mh->dim1 = 1;
		file->ih->mh->dim2 = 1;
		file->ih->mh->dim3 = 1;
		file->ih->mh->dim4 = 1;
		(void) strcpy (file->ih->mh->initial_1, "");
		(void) strcpy (file->ih->mh->initial_2, "");
		(void) strcpy (file->ih->mh->initial_3, "");
	}
	ifh = (MainHeader_if *) file->ih->mh;
	if (file->fp != NULL)
		if (ftell (file->fp) >= 0)
			(void) fclose (file->fp);

	file->fp = fopen (file->name, file->mode);
	if (file->fp == NULL) {
		(void) fprintf (stderr, "ERROR [get_ifh]: Could not open Interfile header %s\n", file->name);
		return (ERROR);
	}
	if (fgets (str, sizeof (str), file->fp) == NULL) {
		(void) fprintf (stderr, "ERROR [get_ifh]: reading %s\n", file->name);
		return (ERROR);
	}
	ptr = strstr (str, "INTERFILE");
	if (ptr == NULL) {
		(void) strcpy (hdrfil, file->name);
		ptr = strrchr (hdrfil, '.');
		if (ptr != NULL)
			ptr = NULL;
		(void) strcat (hdrfil, ".ifh");
		(void) fclose (file->fp);
		file->fp = fopen (hdrfil, file->mode);
		if (file->fp == NULL) {
			(void) fprintf (stderr, "ERROR [get_ifh]: %s is not type Interfile\n", file->name);
			return (ERROR);
		}
		(void) fprintf (stdout, "Reading %s\n", hdrfil);
		if (fgets (str, sizeof (str), file->fp) == NULL) {
			(void) fprintf (stderr, "ERROR [get_ifh]: reading %s\n", hdrfil);
			return (ERROR);
		}
		ptr = strstr (str, "INTERFILE");
		if (ptr == NULL) {
			(void) fprintf (stderr, "ERROR [get_ifh]: %s is not type Interfile\n", hdrfil);
			(void) fclose (file->fp);
			return (ERROR);
		} else
			(void) strcpy (file->name, hdrfil);
	}
	while (fgets (str, sizeof (str), file->fp) != NULL) {
		if (strncmp (str, "END OF INTERFILE", 16) == 0) {
			(void) fclose (file->fp);
			return (OK);
		}
		ptr = strstr (str, ":=");
		if (ptr != NULL) {
			ptr += 3;
			ptr2 =  strrchr(ptr,'\n');
			if (ptr2 != NULL)
				ptr2 = NULL;
			if (strstr (str, "conversion program") != NULL) {
				if (ifh->sw_name == NULL)
					ifh->sw_name = calloc (MAXSTR, sizeof (char));
				(void) sscanf (ptr, "%s", ifh->sw_name);
				continue;
			}
			if (strstr (str, "program version") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->sw_version = ival;
				continue;
			}
			if (strstr (str, "name of data file") != NULL) {
				if (ifh->data_file == NULL)
					ifh->data_file = calloc (MAXSTR, sizeof (char));
				(void) sscanf (ptr, "%s", ifh->data_file);
				continue;
			}
			if (strstr (str, "number format") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (strstr (ptr, "integer") != NULL) {
					ifh->number_format = INT_DATA;
					ifh->nbytes = 2;
				} else if (strstr (ptr, "short") != NULL) {
					ifh->number_format = SHORT_DATA;
					ifh->nbytes = 2;
				} else if (strstr (ptr, "float") != NULL) {
					ifh->number_format = FLOAT_DATA;
					ifh->nbytes = 4;
				} else if (strstr (ptr, "byte") != NULL) {
					ifh->number_format = BYTE_DATA;
					ifh->nbytes = 1;
				} else {
					ifh->number_format = 0;
				}
				continue;
			}
			if (strstr (str, "number of bytes per pixel") != NULL) {
				continue;
			}
			if (strstr (str, "number of dimensions") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->ndim = ival;
				continue;
			}
			if (strstr (str, "matrix size [1]") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->dim1 = ival;
				continue;
			}
			if (strstr (str, "matrix size [2]") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->dim2 = ival;
				continue;
			}
			if (strstr (str, "matrix size [3]") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->dim3 = ival;
				continue;
			}
			if (strstr (str, "matrix size [4]") != NULL) {
				(void) sscanf (ptr, "%d", &ival);
				ifh->dim4 = ival;
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [1]") != NULL) {
				(void) sscanf (ptr, "%f", &fval);
				ifh->voxel_size_1 = fval;
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [2]") != NULL) {
				(void) sscanf (ptr, "%f", &fval);
				ifh->voxel_size_2 = fval;
				continue;
			}
			if (strstr (str, "scaling factor (mm/pixel) [3]") != NULL) {
				(void) sscanf (ptr, "%f", &fval);
				ifh->voxel_size_3 = fval;
				continue;
			}
			if (strstr (str, "matrix initial element [1]") != NULL) {
				while (*ptr == ' ')
					ptr++;
				(void) strcpy (ifh->initial_1, ptr);
				continue;
			}
			if (strstr (str, "matrix initial element [2]") != NULL) {
				while (*ptr == ' ')
					ptr++;
				(void) strcpy (ifh->initial_2, ptr);
				continue;
			}
			if (strstr (str, "matrix initial element [3]") != NULL) {
				while (*ptr == ' ')
					ptr++;
				(void) strcpy (ifh->initial_3, ptr);
				continue;
			}
			if (strstr (str, "image modality") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (ifh->modality == NULL)
					ifh->modality = calloc (MAXSTR, sizeof (char));
				(void) strcpy (ifh->modality, ptr);
				continue;
			}
			if (strstr (str, "originating system") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (ifh->orig_system == NULL)
					ifh->orig_system = calloc (MAXSTR, sizeof (char));
				(void) strcpy (ifh->orig_system, ptr);
				continue;
			}
			if (strstr (str, "original institution") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (ifh->institution == NULL)
					ifh->institution = calloc (MAXSTR, sizeof (char));
				(void) strcpy (ifh->institution, ptr);
				continue;
			}
			if (strstr (str, "subject ID") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (ifh->subject_id == NULL)
					ifh->subject_id = calloc (MAXSTR, sizeof (char));
				(void) strcpy (ifh->subject_id, ptr);
				continue;
			}
			if (strstr (str, "study date") != NULL) {
				while (*ptr == ' ')
					ptr++;
				if (ifh->study_date == NULL)
					ifh->study_date = calloc (MAXSTR, sizeof (char));
				(void) strcpy (ifh->study_date, ptr);
				continue;
			}
		}
	}

	(void) fclose (file->fp);
	return (OK);
}
