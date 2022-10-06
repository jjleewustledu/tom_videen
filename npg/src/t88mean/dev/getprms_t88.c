/* $Log: getprms_t88.c,v $
 * Revision 2.0  1996/05/10  21:58:47  tom
 * new libimage
 * indexing from 1 on images and atlas
 *
 * Revision 1.85  1995/11/10  18:48:00  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.84  1995/08/31  21:59:04  tom
 * add atlas
 *
 * Revision 1.83  1995/01/19  19:17:33  tom
 * smg_divisor = none
 *
 * Revision 1.82  1994/10/03  16:17:13  tom
 * standard format with indent
 *
 * Revision 1.81  1994/09/14  14:41:33  tom
 * add z0 and zmax
 *
 * Revision 1.80  1994/05/18  15:53:30  tom
 * changed $Id: getprms_t88.c,v 2.0 1996/05/10 21:58:47 tom Exp tom $ to $Header: /home/npggw/tom/src/t88mean/RCS/getprms_t88.c,v 2.0 1996/05/10 21:58:47 tom Exp tom $
 * */
/*	===============================================================================
 *	Module:			getprms_t88.c
 *	Date:			10-Aug-93
 *	Author:			Tom Videen
 *	Description:	Read program-specific parameters from the parameter file.
 *
 *	Input:
 *		prmfile	-	name of parameter file
 *
 *	Output (parameters assigned values):
 *	Required:
 *		smg_norm
 *		smg_mode
 *		smg_divisor
 *		smg_included
 *		processing
 *	Optional:
 *		smg_air
 *		smg_mask
 *		smg_sdimage
 *		smg_nimage
 *		smg_symmetry
 *		smg_mskpct
 *		smg_compute
 *		pixel_size
 *		plane_separation
 *		first_slice
 *		last_slice
 *		zorigin
 *		zmax
 *
 *	History:
 *		19-Aug-93	added smg_symmetry and smg_mskpct
 *		24-Aug-93	added smg_scaletopet
 *		27-Aug-93	added pixel_size and plane_separation
 *		12-Apr-94	additional option in smg_include
 *		28-Apr-94	added first_slice and last_slice
 *		10-May-94	eliminated smg_scaletopet
 *		13-Sep-94	added zorigin and zmax
 *		27-Sep-94	added smg_compute
 *		23-Aug-95	add atlas_name
 *	===============================================================================
 */
#include <stdio.h>
#include <stdlib.h>					/* atof, atoi */
#include <string.h>
#include <imgutil.h>
#include <myvalues.h>				/* ERROR */

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/t88mean/RCS/getprms_t88.c,v 2.0 1996/05/10 21:58:47 tom Exp tom $";
#endif

int        getprms_t88 (prmfile, smg_norm, smg_divisor, smg_included, processing, smg_mode, smg_mask,
                smg_sdimage, smg_nimage, smg_symmetry, smg_mskpct, smg_compute, pixel_size,
	            plane_separation, first_slice, last_slice, zorigin, zmax, atlas_name, smg_air)
	char           *prmfile;
	float          *smg_norm;
	char           *smg_divisor;
	char           *smg_included;
	int            *processing;
	char           *smg_mode;
	char           *smg_air;
	char           *smg_mask;
	char           *smg_compute;
	char           *smg_sdimage;
	char           *smg_nimage;
	char           *smg_symmetry;
	float          *smg_mskpct;
	float          *pixel_size;
	float          *plane_separation;
	int            *first_slice;
	int            *last_slice;
	int            *zorigin;
	int            *zmax;
	char           *atlas_name;

{
	FILE           *fp;
	char            line[MAXLINE];
	char            label[MAXLINE];
	char            parameter[MAXLINE];
	int             slice1, slice2;

	*smg_norm = 0;
	*smg_mskpct = 0;
	*pixel_size = 0;
	*plane_separation = 0;
	*first_slice = 1;
	*last_slice = 1;
	*zorigin = 0;
	*zmax = 0;
	*processing = -1;
	strcpy (smg_divisor, "NONE");
	strcpy (smg_included, "NONE");
	strcpy (smg_mode, "NONE");
	strcpy (smg_air, "no");
	strcpy (smg_mask, "no");
	strcpy (smg_compute, "yes");
	strcpy (smg_sdimage, "no");
	strcpy (smg_nimage, "no");
	strcpy (smg_symmetry, "no");
	strcpy (atlas_name, "t88");

	fp = fopen (prmfile, "r");
	if (fp == (FILE *) NULL) {
		printf ("Error opening parameter file %s\n", prmfile);
		return (ERROR);
	}
/*
 *	Read each line of parameter file.
 *	If the line has 2 strings, compare the label with the list of eligible keys:
 *	and if the key is found, set the appropriate parameter.
 */

	while (fgets (line, MAXLINE, fp) != NULL) {
		if (sscanf (line, "%s %s", label, parameter) == 2) {

/*
 *	Get smg_norm
 */

			if (strcmp (label, "smg_norm:") == 0) {
				if (stringtype (parameter) == TYPE_REAL ||
				    stringtype (parameter) == TYPE_INTEGER)
					*smg_norm = atof (parameter);
				else {
					printf ("Error reading smg_norm from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get smg_mode
 */
			} else if (strcmp (label, "smg_mode:") == 0) {
				if (strcmp (parameter, "single") == 0 ||
				    strcmp (parameter, "paired") == 0 ||
				    strcmp (parameter, "diff") == 0)
					strcpy (smg_mode, parameter);
				else {
					printf ("Error reading smg_mode from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get smg_divisor
 */
			} else if (strcmp (label, "smg_divisor:") == 0) {
				if (strcmp (parameter, "global") == 0 ||
				    strcmp (parameter, "local") == 0 ||
				    strcmp (parameter, "none") == 0)
					strcpy (smg_divisor, parameter);
				else {
					printf ("Error reading smg_divisor from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get smg_included
 */
			} else if (strcmp (label, "smg_included:") == 0) {
				if (strcmp (parameter, "all") == 0 ||
				    strcmp (parameter, "sampled") == 0 ||
				    strncmp (parameter, "n=", 2) == 0)
					strcpy (smg_included, parameter);
				else {
					printf ("Error reading smg_included from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get processing
 */
			} else if (strcmp (label, "processing:") == 0) {
                if (strncmp (parameter, "none", 4) == 0)
                    *processing = PROC_NONE;
                else if (strncmp (parameter, "cbf", 3) == 0)
                    *processing = PROC_CBF;
                else if (strncmp (parameter, "cbv", 3) == 0)
                    *processing = PROC_CBV;
                else if (strncmp (parameter, "tissue_activity", 15) == 0)
                    *processing = PROC_DECAYCOR;
                else if (strncmp (parameter, "decay-corrected", 15) == 0)
                    *processing = PROC_DECAYCOR;
                else if (strncmp (parameter, "wellcounts", 10) == 0)
                    *processing = PROC_UNCORRECTED;
                else if (strncmp (parameter, "uncorrected", 11) == 0)
                    *processing = PROC_UNCORRECTED;
                else if (strncmp (parameter, "oef", 3) == 0)
                    *processing = PROC_OEF;
                else if (strncmp (parameter, "cmro2", 5) == 0)
                    *processing = PROC_CMRO2;
				else {
					printf ("Error reading processing from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get smg_air
 */
			} else if (strcmp (label, "smg_air:") == 0) {
				strcpy (smg_air, parameter);
/*
 *	Get smg_mask
 */
			} else if (strcmp (label, "smg_mask:") == 0) {
				strcpy (smg_mask, parameter);
/*
 *	Get smg_compute
 */
			} else if (strcmp (label, "smg_compute:") == 0) {
				strcpy (smg_compute, parameter);
/*
 *	Get smg_sdimage
 */
			} else if (strcmp (label, "smg_sdimage:") == 0) {
				strcpy (smg_sdimage, parameter);
/*
 *	Get smg_nimage
 */
			} else if (strcmp (label, "smg_nimage:") == 0) {
				strcpy (smg_nimage, parameter);
/*
 *	Get smg_symmetry
 */
			} else if (strcmp (label, "smg_symmetry:") == 0) {
				strcpy (smg_symmetry, parameter);
/*
 *	Get pixel_size
 */
			} else if (strcmp (label, "pixel_size:") == 0) {
				if (stringtype (parameter) == TYPE_REAL ||
				    stringtype (parameter) == TYPE_INTEGER)
					*pixel_size = atof (parameter);
				else {
					printf ("Error reading pixel_size from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get plane_separation
 */
			} else if (strcmp (label, "plane_separation:") == 0) {
				if (stringtype (parameter) == TYPE_REAL ||
				    stringtype (parameter) == TYPE_INTEGER)
					*plane_separation = atof (parameter);
				else {
					printf ("Error reading plane_separation from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get first_slice and last_slice
 */
			} else if (strcmp (label, "slices:") == 0) {
				if (sscanf (parameter, "%dto%d", &slice1, &slice2) == 2) {
					if (slice1 <= slice2) {
						*first_slice = slice1;
						*last_slice = slice2;
					} else {
						*first_slice = slice2;
						*last_slice = slice1;
					}
				}
/*
/*
 *	Get zorigin
 */
			} else if (strcmp (label, "z0:") == 0) {
				if (stringtype (parameter) == TYPE_INTEGER)
					*zorigin = atoi (parameter);
				else {
					printf ("Error reading z0 from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get zmax
 */
			} else if (strcmp (label, "zmax:") == 0) {
				if (stringtype (parameter) == TYPE_INTEGER)
					*zmax = atoi (parameter);
				else {
					printf ("Error reading zmax from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get smg_mskpct
 */
			} else if (strcmp (label, "smg_mskpct:") == 0) {
				if (stringtype (parameter) == TYPE_REAL ||
				    stringtype (parameter) == TYPE_INTEGER)
					*smg_mskpct = atof (parameter);
				else {
					printf ("Error reading smg_mskpct from %s\n", prmfile);
					return (ERROR);
				}
/*
 *	Get atlas_name
 */
			} else if (strcmp (label, "atlas:") == 0) {
				if (strcmp (parameter, "none") == 0 ||
				    strcmp (parameter, "t88") == 0)
					strcpy (atlas_name, parameter);
				else {
					printf ("Error reading atlas_name from %s\n", prmfile);
					return (ERROR);
				}
			}
		}
	}
/*
 *	Check that all required parameters have been assigned new values before exiting.
 */

	if (strcmp (smg_compute, "yes") == 0) {
		if (*smg_norm == 0) {
			printf ("Error:  smg_norm not found in %s\n", prmfile);
			return (ERROR);
		}
		if (strcmp (smg_divisor, "NONE") == 0) {
			printf ("Error:  smg_divisor not found in %s\n", prmfile);
			return (ERROR);
		}
		if (strcmp (smg_included, "NONE") == 0) {
			printf ("Error:  smg_included not found in %s\n", prmfile);
			return (ERROR);
		}
		if (strcmp (smg_mode, "NONE") == 0) {
			printf ("Error:  smg_mode not found in %s\n", prmfile);
			return (ERROR);
		}
		if (*processing < 0) {
			printf ("Error:  processing not found in %s\n", prmfile);
			return (ERROR);
		}
	}
	return (OK);
}
