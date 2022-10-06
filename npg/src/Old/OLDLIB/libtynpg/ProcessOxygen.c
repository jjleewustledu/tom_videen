/*$Id: ProcessOxygen.c,v 1.4 1995/12/14 19:59:46 ty7777 Exp $*/
/*$Log: ProcessOxygen.c,v $
 * Revision 1.4  1995/12/14  19:59:46  ty7777
 * Use NEWimglist.h.
 *
 * Revision 1.3  1995/10/13  17:05:02  ty7777
 * Use matrix7.h.
 *
 * Revision 1.2  1994/04/08  15:02:14  ty7777
 * Delected voimean.h.
 *
 * Revision 1.1  1994/04/08  14:43:23  ty7777
 * Initial revision
 **/

/*___________________________________________________________________________
  File:		ProcessOxygen.c

  Function:	PUBLIC int ProcessOxygen (float *oxygenimage, float *flowimage, 
		float *volumeimage, long dimension, int processing, 
		char *oxygen_imgfile, char *flow_imgfile, 
		char *volume_imgfile)

  Arguments:	oxygenimage,	a real array containing the 3d oxygen image data, 
				changed upon return.
		flowimage,	a real array containing the 3d blood flow image data, 
				changed upon return.
		volumeimage,	a real array containing the 3d blood volume image data, 
				changed upon return.
		dimension,	dimension of image.
		processing,	processing code.
		oxygen_imgfile,	oxygen image file name, used to get the oxygen 
				header file.
		flow_imgfile,	flow image file name, used to get the blood flow 
				header file.
		volume_imgfile,	flow image file name, used to get the blood volume 
				header file.

  Return:	SUCCEED: successful; FAIL: error.

  Description:	This function processes an image for OEF, CMRO2.

  Called by:	ProcessOxygen.c	

  Calling:	None.	

  History:	Created by Tom Yang on 03/23/94. 
___________________________________________________________________________*/

#include <petutil/NEWimglist.h>
#include <petutil/matrix7.h>
#include <values.h>
#include <math.h>

#define	SMALL_VESSEL_RATIO	0.85	/* radio of cerebral vessel to large vessel hematocrit */
#define	VOLUME_FACTOR		0.835	/* (postcapillary vol + 0.5 * capillary vol) / total blood vol */
#define	BRAIN_DENSITY		1.05	/* density of brain, unit: g / ml */

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/libnpg/RCS/ProcessOxygen.c,v 1.4 1995/12/14 19:59:46 ty7777 Exp $";

PUBLIC int ProcessOxygen (oxygenimage, flowimage, volumeimage, dimension, processing, 
	oxygen_imgfile, flow_imgfile, volume_imgfile)
	float	*oxygenimage;
	float	*flowimage;
	float	*volumeimage;
	long	dimension;
	int	processing;
	char	*oxygen_imgfile;
	char	*flow_imgfile;
	char	*volume_imgfile;
{
	HEADER_DATA	header_data;
	char		complete_hdrfile [MAXPATH];
	char		short_hdrfile [MAXLINE];
	float		a1;
	float		a2;
	float		a3;
	float		a4;
	float		a5;
	float		c1;
	float		c2;
	float		c3;
	float		value;
	float		x1;
	float		x2;
	float		x3;
	long		i;
	struct Matval	matval;

	/*
	 * No processing if not OEF or CMRO2.
	 */
	if (processing != PROCESSING_OEF
	&& processing != PROCESSING_CMRO2)
		return SUCCEED;

	/*
	 * No processing if not OEF or CMRO2.
	 */
	gethdrfile (oxygen_imgfile, short_hdrfile, complete_hdrfile);

	if (gethdr (short_hdrfile, &header_data) != SUCCEED)
	{
		if (gethdr (complete_hdrfile, &header_data) != SUCCEED)
		{
			fprintf (stderr, "Error: header file can not be found as %s or %s\n", 
				short_hdrfile, complete_hdrfile);

			return FAIL;
		}
	}

	/*
	 * Process the flow image.
	 */
	if (header_data.ntype == 1)
	{
		if (ProcessImage (flowimage, dimension, PROCESSING_CBF, 0.0, 
		flow_imgfile, matval) == FAIL)
		{
			fprintf (stderr, "Error in processing flow image %s\n", flow_imgfile);

			return FAIL;
		}

		/*
	 	 * Process the volume image.
	 	 */
		if (ProcessImage (volumeimage, dimension, PROCESSING_CBV, 0.0, 
		volume_imgfile, matval) == FAIL)
		{
			fprintf (stderr, "Error in processing volume image %s\n", volume_imgfile);

			return FAIL;
		}
	}

	/*
	 * Constants for oxygen processing.
	 */
	c1	= SMALL_VESSEL_RATIO * header_data.o2cnts1 / 100.0;
	c2	= VOLUME_FACTOR * c1;
	c3	= header_data.pettcnv1 / BRAIN_DENSITY;

	if (header_data.ntype == 1)
	{
		for (i = 0; i < dimension; i++)
		{
			x1	= oxygenimage [i];
			if (x1 != 0.0)
			{
				x2	= flowimage [i];
				x3	= volumeimage [i];
				a1	= c3 * x1;
				a2	= x2 * (header_data.awater1 * x2 + header_data.bwater1);
				a3	= c1 * x3;
				a4	= x2 * (header_data.aoxygen1 * x2 + header_data.boxygen1);
				a5	= c2 * x3;
				if (a4 == a5)
					oxygenimage [i]	= 0.0;
				else if (processing == PROCESSING_OEF)
					oxygenimage [i]	= (a1 - a2 - a3) / (a4 - a5);
				else if (processing == PROCESSING_CMRO2)
					oxygenimage [i]	= x2 * header_data.oxycont1 
							* (a1 - a2 - a3) / (a4 - a5);
			}
		}
	}
	else if ((processing == PROCESSING_OEF && header_data.ntype == 11) 
	|| (processing == PROCESSING_CMRO2 && header_data.ntype == 12)) 
	{
		for (i = 0; i < dimension; i++)
		{
			oxygenimage [i]	*= header_data.aoxygen1;
		}
	}
	else
	{
		fprintf (stderr,
		"Error: processing %d and data type %d in header file %s does not match.\n",
		processing, header_data.ntype, short_hdrfile);

		return FAIL;
	}


	return SUCCEED;
}
