/*$Id: readparm.c,v 1.49 1996/08/01 19:40:26 yang Exp $*/
/*$Log: readparm.c,v $
 * Revision 1.49  1996/08/01  19:40:26  yang
 * Added 961.
 *
 * Revision 1.48  1996/04/03  17:59:24  ty7777
 * Save before making any further changes.
 *
 * Revision 1.47  1995/12/14  20:08:38  ty7777
 * Use NEWimglist.h
 *
 * Revision 1.46  1995/12/06  15:44:08  ty7777
 * Added scanner type 961.
 *
 * Revision 1.45  1995/04/18  15:44:37  ty7777
 * Added processing for normalization and scatter correction parameters.
 *
 * Revision 1.44  1995/04/17  15:22:36  ty7777
 * Added voi_sd.
 *
 * Revision 1.43  1994/10/03  17:03:18  ty7777
 * Deleted extension default.
 *
 * Revision 1.42  1994/10/03  16:42:45  ty7777
 * Initializes every fields of the structure PARAMETER_DATA.
 *
 * Revision 1.41  1994/09/20  22:04:40  ty7777
 * Added the default value for voi_norm.
 *
 * Revision 1.40  1994/06/20  17:46:01  ty7777
 * Uses 999999.00 for halflife for backward compatibability.
 *
 * Revision 1.39  1994/05/11  14:49:29  ty7777
 * Allowing upper case letters for datapath.
 *
 * Revision 1.38  1994/04/13  15:22:08  ty7777
 * Added wellcounts processing.
 *
 * Revision 1.37  1994/04/07  15:28:37  ty7777
 * Use Lowered ().
 *
 * Revision 1.36  1994/04/05  19:13:39  ty7777
 * Fixed the voi_norm problem.
 *
 * Revision 1.35  1994/04/04  21:57:55  ty7777
 * Added voi_norm processing.
 *
 * Revision 1.34  1994/03/25  22:40:09  ty7777
 * Added processing for imglist_mode.
 *
 * Revision 1.33  1994/03/22  16:18:32  ty7777
 * Added values.h where MAXFLOAT is defined.
 *
 * Revision 1.32  1994/03/22  16:08:00  ty7777
 * Use MAXFLOAT as the default halflife.
 *
 * Revision 1.31  1994/03/22  16:06:21  ty7777
 * Added processing for tissue activity and halflife.
 *
 * Revision 1.30  1994/03/11  17:14:53  ty7777
 * Added the default for category.
 *
 * Revision 1.29  1994/03/04  19:20:49  ty7777
 * Assign the default maskimg.
 *
 * Revision 1.28  1994/03/04  16:31:38  ty7777
 * Release memory for lowered.
 *
 * Revision 1.27  1994/03/03  20:41:28  ty7777
 * Replace id with header.
 *
 * Revision 1.26  1994/03/03  16:14:18  ty7777
 * Added processing for processing, rgt_format, regfile.
 *
 * Revision 1.25  1994/02/23  21:31:17  ty7777
 * Added processing for voi.
 *
 * Revision 1.24  1994/02/22  21:10:10  ty7777
 * Fixed voi processing bug.
 *
 * Revision 1.23  1994/02/22  20:46:51  ty7777
 * Added processing for reading voi parameters.
 *
 * Revision 1.22  1994/02/02  21:20:41  ty7777
 * Added processing for atlfile.
 *
 * Revision 1.21  1993/11/15  22:17:43  ty7777
 * Same as the last one.
 *
 * Revision 1.20  1993/11/15  21:49:50  ty7777
 * Added processing for 12.
 * recfilter = 12.
 *
 * Revision 1.19  1993/11/15  21:23:04  ty7777
 * Added li to maximum_like.
 *
 * Revision 1.18  1993/11/03  17:00:33  ty7777
 * Same as the last version.
 *
 * Revision 1.17  1993/11/02  21:18:23  ty7777
 * Processing for suffix.
 *
 * Revision 1.16  1993/10/28  16:43:40  ty7777
 * Added literal list mode.
 *
 * Revision 1.15  1993/10/22  15:21:57  ty7777
 * Added error checking for reconstruction, datatype, etc.
 *
 * Revision 1.14  1993/09/07  20:34:48  ty7777
 * Correcting the contant STD_IMAGE_MODE.
 *
 * Revision 1.13  1993/09/07  20:27:50  ty7777
 * Added weld_lstmode.
 *
 * Revision 1.12  1993/09/03  17:01:16  ty7777
 * Added f-tracer and v-tracer.
 *
 * Revision 1.11  1993/08/12  14:54:55  ty7777
 * Add reading rms_align.
 *
 * Revision 1.10  1993/08/09  20:30:46  ty7777
 * Reading extension.
 *
 * Revision 1.9  1993/08/06  19:54:10  ty7777
 * Check acquisition parameter.
 *
 * Revision 1.8  1993/08/06  19:48:03  ty7777
 * Check parameter for mskimg.
 *
 * Revision 1.7  1993/08/06  19:35:39  ty7777
 * Change parmaeters for mskimg.
 *
 * Revision 1.6  1993/07/30  16:54:07  ty7777
 * Added parameter "slices".
 *
 * Revision 1.5  1993/07/28  19:24:32  ty7777
 * Adding rcsid.
 *
 * Revision 1.4  1993/07/23  20:00:23  ty7777
 * change strchr to strncmp for checking comment lines.
 *
 * Revision 1.3  1993/07/23  15:38:37  ty7777
 * Adding imgmean related parameters.
 *
 * Revision 1.2  1993/07/21  19:20:44  ty7777
 * Changing finfo to parameter_data.
 *
 * Revision 1.1  1993/07/06  18:13:49  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		readparm.c

  Function:	int readparm (char *parameter_file, PARAMETER_DATA *parameter_data)

  Arguments:	parameter_file,	file containing image parameters, passed in.
		parameter_data,	a structure defined in /usr/local/include/petutil/imglist.h.
				returned.

  Description:	This function reads the image parameter file and puts the information
		it contains in a structure.

  Return:	SUCCEED for successful; FAIL for error.

  Reminder:	Every real number should contain at least a decimal point. 
		The sxr file extension should start with a letter.

  Creator:	Tom Yang
  History:	Created by Tom Yang on 05/18/93. 
___________________________________________________________________________*/

#include <stdio.h>
#include <string.h>
#include <values.h>
#include <petutil/imglist.h>
/*
#include <petutil/NEWimglist.h>
*/

#define LOW_RESOLUTION	0
#define HIGH_RESOLUTION	1
#define STATIONARY	2
#define WOBBLE		3
#define THREE_DIM	4
#define FBOLD		0
#define FB		1
#define ML		2
#define FBDGP		3
#define FBP3D		4
#define FBP3DJMON	5
#define FBP3DJMOS	6
#define DEFAULT_CUTOFF	0.5
#define DEFAULT_ORDER	5

#define	SCANNER_953B	"953"
#define	SCANNER_961	"961"

char *acquisition_string []	= {"low", 
				   "high", 
				   "sta", 
				   "wob",
				   "3d"};

char *reconstruction_string []	= {"fbold", 
				   "fb",
				   "ml",
				   "fbdgp",
				   "3dbp",
				   "3dbpjmon",
				   "3dbpjmos"};

static char rcsid [] = "$Header: /home/petsun4/yang/src/libnpg/RCS/readparm.c,v 1.49 1996/08/01 19:40:26 yang Exp $";

PRIVATE void error_message ();

PRIVATE void error_message (keyword, parameter, file)
	char	*keyword;
	char	*parameter;
	char	*file;
{
	fprintf (stderr, 
	"Error: wrong parameter (%s) for keyword (%s) in parameter file (%s).\n",
	parameter, keyword, file);
}

PUBLIC int readparm (parameter_file, parameter_data) 
	char		*parameter_file;
	PARAMETER_DATA	*parameter_data;
{
	FILE	*fp;
	FILE	*pathfp;
	char	label [MAXLINE];
	char	line [MAXLINE];
	char	original_parameter [MAXLINE];
	char	parameter [MAXLINE];
	char	parameter2 [MAXLINE];
	char	parameter3 [MAXLINE];
	float	x;
	float	y;
	float	z;
	int	cat;
	int	first_slice;
	int	last_slice;
	int	index;
	int	type;

	fp	= fopen (parameter_file, "r");
	if (fp == (FILE *) NULL)
	{
		fprintf (stderr, "Error in opening parameter file %s\n", parameter_file);
		return FAIL;
	}

	/*
	 * Initialization
	 */
	memset (parameter_data, 0, sizeof (PARAMETER_DATA)); 

/*
	parameter_data->category		= INDIVIDUAL;
*/
	parameter_data->recfilter_type		= FILTER_NONE;
	parameter_data->recfilter_cutoff	= DEFAULT_CUTOFF;
	parameter_data->recfilter_order		= DEFAULT_ORDER;
	parameter_data->mskfilter_type		= FILTER_NONE;
	parameter_data->mskfilter_cutoff	= DEFAULT_CUTOFF;
	parameter_data->mskfilter_order		= DEFAULT_ORDER;
	parameter_data->imgfilter_type		= FILTER_NONE;
	parameter_data->imgfilter_cutoff	= DEFAULT_CUTOFF;
	parameter_data->imgfilter_order		= DEFAULT_ORDER;
	parameter_data->imglist_mode		= STDIMAGE_MODE;
	parameter_data->first_slice		= 0;
	parameter_data->last_slice		= 0;
	parameter_data->voi_shape		= VOI_UNDEFINED;
	parameter_data->voi_norm		= 0.0;
	parameter_data->voi_sd			= FALSE;
	parameter_data->voi_xdim		= 0;
	parameter_data->voi_ydim		= 0;
	parameter_data->voi_zdim		= 0;
	parameter_data->processing		= PROCESSING_NONE;
	parameter_data->rgt_format		= REGIONCOL;
	parameter_data->image_frames		= 1;
	parameter_data->halflife		= MAXFLOAT;

	while (fgets (line, MAXLINE, fp) != NULL)
	{
		if (sscanf (line, "%s %s", label, parameter) == 2)
		{
			if (strncmp (label, "#", 1) == 0)
				continue;

			strcpy (original_parameter, parameter);
			Lowered (label, strlen (label));
			Lowered (parameter, strlen (parameter));

			if (strncmp (label, "datapath:", 9) == 0)
			{
				strcpy (parameter2, original_parameter);
				strcat (parameter2, "/.");
				pathfp	= fopen (parameter2, "r");
				if (pathfp != (FILE *) NULL)
					strcpy (parameter_data->datapath, original_parameter);
				else
				{
					fprintf (stderr, "datapath error. Path %s does not exist\n", 
					parameter);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "suffix:", 7) == 0 && strlen (original_parameter) > 0)
			{
				strcpy (parameter_data->suffix, original_parameter);

				continue;
			}

			if (strncmp (label, "extension:", 10) == 0 
			&& strlen (original_parameter) > 0)
			{
				if (strlen (original_parameter) > 0 
				&& strncmp (parameter, ".", 1) != 0)
				{
					strcpy (parameter_data->extension, ".");
					strcat (parameter_data->extension, original_parameter);
				}
				else
					strcpy (parameter_data->extension, original_parameter);

				continue;
			}

			if (strncmp (label, "slices:", 8) == 0)
			{
				if (sscanf (parameter, "%dto%d", &first_slice, &last_slice) == 2)
				{
					if (first_slice <= last_slice)
					{
						parameter_data->first_slice	= first_slice;
						parameter_data->last_slice	= last_slice;
					}
					else
					{
						parameter_data->first_slice	= last_slice;
						parameter_data->last_slice	= first_slice;
					}
				}

				continue;
			}

			if (strncmp (label, "species:", 8) == 0)
			{
				if (strncmp (parameter, "p", 1) == 0 ||
				strncmp (parameter, "m", 1) == 0)
					strcpy (parameter_data->species, parameter);
				
				continue;
			}

			if (strncmp (label, "tracer:", 7) == 0)
			{
				/*
				if (strncmp (parameter, "ho", 2) == 0 ||
				strncmp (parameter, "co", 2) == 0 ||
				strncmp (parameter, "oo", 2) == 0 ||
				strncmp (parameter, "bu", 2) == 0)
				*/
				strcpy (parameter_data->tracer, original_parameter);

				continue;
			}

			if (strncmp (label, "f-tracer:", 9) == 0)
			{
				strcpy (parameter_data->ftracer, original_parameter);

				continue;
			}

			if (strncmp (label, "v-tracer:", 9) == 0)
			{
				strcpy (parameter_data->vtracer, original_parameter);

				continue;
			}

			if (strncmp (label, "category:", 9) == 0)
			{
/*
				if (strncmp (parameter, "individual", 10) == 0)
					parameter_data->category	= INDIVIDUAL;
				else if (strncmp (parameter, "composite", 9) == 0)
					parameter_data->category	= COMPOSITE;
				else if (strncmp (parameter, "phantom", 7) == 0)
					parameter_data->category	= PHANTOM;
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}
*/

				continue;
			}

			if (strncmp (label, "datatype:", 9) == 0)
			{
/*
				if (strncmp (parameter, "common_data", 11) == 0)
					parameter_data->datatype	= COMMON_DATA;
				else if (strncmp (parameter, "pet_data", 8) == 0)
					parameter_data->datatype	= PET_DATA;
				else if (strncmp (parameter, "pet_image", 9) == 0)
					parameter_data->datatype	= PET_IMAGE;
				else if (strncmp (parameter, "mri_data", 8) == 0)
					parameter_data->datatype	= MRI_DATA;
				else if (strncmp (parameter, "mri_image", 9) == 0)
					parameter_data->datatype	= MRI_IMAGE;
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}
*/

				continue;
			}

			if (strncmp (label, "scanner:", 8) == 0)
			{
				if (strncmp (parameter, "pett6", 5) == 0 ||
				strncmp (parameter, SCANNER_953B, 3) == 0 ||
				strncmp (parameter, SCANNER_961, 3) == 0)
					strcpy (parameter_data->scanner, parameter);
				else if (strncmp (parameter, "ecat", 4) == 0)
					strcpy (parameter_data->scanner, SCANNER_953B);
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "acquisition:", 12) == 0)
			{
				if (strncmp (parameter, "low_resolution", 14) == 0)
					strcpy (parameter_data->acquisition, 
						acquisition_string [LOW_RESOLUTION]);
				else if (strncmp (parameter, "high_resolution", 15) == 0)
					strcpy (parameter_data->acquisition, 
						acquisition_string [HIGH_RESOLUTION]);
				else if (strncmp (parameter, "stationary", 10) == 0)
					strcpy (parameter_data->acquisition, 
						acquisition_string [STATIONARY]);
				else if (strncmp (parameter, "wobble", 6) == 0)
					strcpy (parameter_data->acquisition, 
						acquisition_string [WOBBLE]);
				else if (strncmp (parameter, "3d", 2) == 0)
					strcpy (parameter_data->acquisition, 
						acquisition_string [THREE_DIM]);
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "reconstruction:", 15) == 0)
			{
				if (strncmp (parameter, "filtered_backprojection_old", 27) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FBOLD]);
				else if (strncmp (parameter, "filtered_backprojection", 23) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FB]);
				else if (strncmp (parameter, "maximum_likelihood", 18) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [ML]);
				else if (strncmp (parameter, 
					"filtered_backprojection_Dave_Politte", 36) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FBDGP]);
				else if (strncmp (parameter, "3d_filtered_backprojection", 26) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FBP3D]);
				else if (strncmp (parameter, "3d_filtered_backprojection_jmo_norm", 35) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FBP3DJMON]);
				else if (strncmp (parameter, "3d_filtered_backprojection_jmo_scat", 35) == 0)
          				strcpy (parameter_data->reconstruction, 
						reconstruction_string [FBP3DJMOS]);
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "recfilter:", 10) == 0)
			{
				if (strncmp (parameter, "filterg", 7) == 0 ||
				strncmp (parameter, "filterf", 7) == 0 ||
				strncmp (parameter, "ramp", 4) == 0 ||
				strncmp (parameter, "12", 2) == 0)
					strcpy (parameter_data->recfilter, parameter);

				if ((strncmp (parameter, "gauss", 5) == 0 ||
				strncmp (parameter, "butterworth", 11) == 0) &&
				sscanf (line, "%s %s %s", label, parameter, parameter2) == 3)
				{
					strcpy (parameter_data->recfilter, parameter);

					if (strncmp (parameter, "gauss", 5) == 0)
						parameter_data->recfilter_type	= FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter, 
					parameter2, parameter3) == 4)
					{
						parameter_data->recfilter_type	= FILTER_BUTTERWORTH;
						if (DetermineType (parameter3) == DATA_TYPE_INTEGER)
							parameter_data->recfilter_order	= atoi (parameter3);
						else
						{
							fprintf (stderr, "recfilter_order error\n");
							return FAIL;
						}
					}

					if (DetermineType (parameter2) == DATA_TYPE_REAL ||
					DetermineType (parameter2) == DATA_TYPE_INTEGER)
						parameter_data->recfilter_cutoff	= atof (parameter2);
					else
					{
						fprintf (stderr, "Error: recfilter_cutoff (%s)\n", 
							parameter2);
						return FAIL;
					}
				}

				continue;
			}

			if (strncmp (label, "mskimg:", 7) == 0)
			{
				if (strncmp (parameter, "control_image", 13) == 0)
				{
					parameter_data->mskimg_type	= MASK_CONTROL;
				}
				else if (strncmp (parameter, "specified", 9) == 0)
				{
					parameter_data->mskimg_type	= MASK_SPECIFIED;
					if (sscanf (line, "%s %s %s", label, parameter, parameter2) == 3)
					{
						strcpy (parameter_data->mskimg, parameter2);
					}
				}
				else
				{
					error_message (label, parameter, parameter_file);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "mskpct:", 7) == 0)
			{
				if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					parameter_data->mskpct	= atoi (parameter);
				else
				{
					fprintf (stderr, "mskpct parameter error\n");
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "mskfilter:", 10) == 0)
			{
				if ((strncmp (parameter, "gauss", 5) == 0 ||
				strncmp (parameter, "butterworth", 11) == 0) &&
				sscanf (line, "%s %s %s", label, parameter, parameter2) == 3)
				{
					if (strncmp (parameter, "gauss", 5) == 0)
						parameter_data->mskfilter_type	= FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter, 
					parameter2, parameter3) == 4)
					{
						parameter_data->mskfilter_type	= FILTER_BUTTERWORTH;
						if (DetermineType (parameter3) == DATA_TYPE_INTEGER)
							parameter_data->mskfilter_order	= atoi (parameter3);
						else
						{
							fprintf (stderr, "mskfilter_order error\n");
							return FAIL;
						}
					}

					if (DetermineType (parameter2) == DATA_TYPE_REAL ||
					DetermineType (parameter2) == DATA_TYPE_INTEGER)
						parameter_data->mskfilter_cutoff	= atof (parameter2);
					else
					{
						fprintf (stderr, "mskfilter_cutoff error\n");
						return FAIL;
					}
				}

				continue;
			}

			if (strncmp (label, "imgfilter:", 10) == 0)
			{
				if ((strncmp (parameter, "gauss", 5) == 0 ||
				strncmp (parameter, "butterworth", 11) == 0) &&
				sscanf (line, "%s %s %s", label, parameter, parameter2) == 3)
				{
					if (strncmp (parameter, "gauss", 5) == 0)
						parameter_data->imgfilter_type	= FILTER_GAUSS;
					else if (sscanf (line, "%s %s %s %s", label, parameter, 
					parameter2, parameter3) == 4)
					{
						parameter_data->imgfilter_type	= FILTER_BUTTERWORTH;
						if (DetermineType (parameter3) == DATA_TYPE_INTEGER)
							parameter_data->imgfilter_order	= atoi (parameter3);
						else
						{
							fprintf (stderr, "imgfilter_order error\n");
							return FAIL;
						}
					}

					if (DetermineType (parameter2) == DATA_TYPE_REAL ||
					DetermineType (parameter2) == DATA_TYPE_INTEGER)
						parameter_data->imgfilter_cutoff	= atof (parameter2);
					else
					{
						fprintf (stderr, "imgfilter_cutoff error\n");
						return FAIL;
					}
				}

				continue;
			}

			if (strncmp (label, "rms_align:", 10) == 0)
			{
				if ((strncmp (parameter, "n", 1) == 0 ||
				strncmp (parameter, "a", 1) == 0))
				{
					strncpy (parameter_data->rms_align, parameter, 1);
				}

				continue;
			}

			if (strncmp (label, "imglist_mode:", 13) == 0
			|| strncmp (label, "imgmode:", 8) == 0
			|| strncmp (label, "weld_lstmode:", 13) == 0)
			{
				if (strncmp (parameter, "standard", 8) == 0)
				{
					parameter_data->imglist_mode	= STDIMAGE_MODE;
				}
				else if (strncmp (parameter, "diff", 4) == 0)
				{
					parameter_data->imglist_mode	= DIFIMAGE_MODE;
				}
				else if (strncmp (parameter, "prewelded", 9) == 0)
				{
					parameter_data->imglist_mode	= PREWELDED_IMAGE_MODE;
				}
				else if (strncmp (parameter, "multitracer", 11) == 0)
				{
					parameter_data->imglist_mode	= MULTITRACER_IMAGE_MODE;
				}
				else if (strncmp (parameter, "welded", 6) == 0)
				{
					parameter_data->imglist_mode	= WELDED_IMAGE_MODE;
				}
				else if (strncmp (parameter, "literal", 7) == 0)
				{
					parameter_data->imglist_mode	= LITERAL_IMAGE_MODE;
				}

				continue;
			}

			if (strncmp (label, "atlfile:", 8) == 0)
			{
				strcpy (parameter_data->atlfile, original_parameter);

				continue;
			}

			if (strncmp (label, "voi_norm:", 9) == 0)
			{
				if (DetermineType (parameter) == DATA_TYPE_REAL ||
				DetermineType (parameter) == DATA_TYPE_INTEGER)
					parameter_data->voi_norm	= atof (parameter);
				else
				{
					fprintf (stderr, "Error: voi_norm (%s)\n", parameter);
					return FAIL;
				}

				continue;
			}

			if (strncmp (label, "voi_sd:", 7) == 0)
			{
				if (strncmp (parameter, "yes", 3) == 0)
					parameter_data->voi_sd	= TRUE;;

				continue;
			}

			if (strncmp (label, "voi:", 4) == 0)
			{
				if (sscanf (line, "%s %s %f %f %f", label, parameter, &x, &y, &z) == 5
				&& strncmp (parameter, "cube", 4) == 0)
				{
					parameter_data->voi_shape	= VOI_CUBE;
					parameter_data->voi_xdim	= x;
					parameter_data->voi_ydim	= y;
					parameter_data->voi_zdim	= z;
				}
				else if (sscanf (line, "%s %s %f", label, parameter, &x) == 3
				&& strncmp (parameter, "sphere", 6) == 0)
				{
					parameter_data->voi_shape	= VOI_SPHERE;
					parameter_data->voi_xdim	= x;
					parameter_data->voi_ydim	= x;
					parameter_data->voi_zdim	= x;
				}

				continue;
			}

			if (strncmp (label, "processing:", 11) == 0)
			{
				if (strncmp (parameter, "none", 4) == 0)
					parameter_data->processing	= PROCESSING_NONE;
				else if (strncmp (parameter, "cbf", 3) == 0)
					parameter_data->processing	= PROCESSING_CBF;
				else if (strncmp (parameter, "cbv", 3) == 0)
					parameter_data->processing	= PROCESSING_CBV;
				else if (strncmp (parameter, "tissue_activity", 15) == 0)
					parameter_data->processing	= PROCESSING_TISACT;
				else if (strncmp (parameter, "wellcounts", 10) == 0)
					parameter_data->processing	= PROCESSING_WELLCOUNTS;
				else if (strncmp (parameter, "oef", 3) == 0)
					parameter_data->processing	= PROCESSING_OEF;
				else if (strncmp (parameter, "cmro2", 5) == 0)
					parameter_data->processing	= PROCESSING_CMRO2;

				continue;
			}

			if (strncmp (label, "halflife:", 9) == 0)
			{
				parameter_data->halflife	= atof (parameter);
				if (parameter_data->halflife == 999999.00)
					parameter_data->halflife	= NONDECAY_HALFLIFE;

				continue;
			}

			if (strncmp (label, "rgt_format:", 11) == 0)
			{
				if (strncmp (parameter, "regioncol", 9) == 0)
					parameter_data->rgt_format	= REGIONCOL;
				else if (strncmp (parameter, "regionrow", 9) == 0)
					parameter_data->rgt_format	= REGIONROW;

				continue;
			}

			if (strncmp (label, "frames:", 7) == 0)
			{
				if (strncmp (parameter, "all", 3) == 0)
					parameter_data->image_frames	= ALL_FRAMES;
				else if (DetermineType (parameter) == DATA_TYPE_INTEGER)
					parameter_data->image_frames	= atoi (parameter);

				continue;
			}

			if (strncmp (label, "regfile:", 8) == 0)
			{
				strcpy (parameter_data->regfile, parameter);
			}
		}
	}

	/*
	 * Check the parameters that are necessary.
	 */
	if (strlen (parameter_data->extension) == 0)
	{
		if (strncmp (parameter_data->scanner, SCANNER_953B, 3) == 0)
			strcpy (parameter_data->extension, IMAGE_EXT);
	}

	fclose (fp);

	return SUCCEED;
}
