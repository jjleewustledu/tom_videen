/*$Id: getmagf.c,v 1.8 1996/05/03 21:45:19 ty7777 Exp $*/
/*$Log: getmagf.c,v $
 * Revision 1.8  1996/05/03  21:45:19  ty7777
 * Rearranged for 961 data.
 *
 * Revision 1.7  1995/06/27  19:11:08  ty7777
 * Updated to take any file names for HR data.
 *
 * Revision 1.6  1995/06/27  16:43:50  ty7777
 * Added processing for HR961 data.
 *
 * Revision 1.5  1995/02/13  22:02:55  ty7777
 * Use the patient id as the criterion for checking scans after Oct., 1994.
 *
 * Revision 1.4  1995/02/08  18:21:04  ty7777
 * Added check on patient id.
 *
 * Revision 1.3  1994/12/22  20:09:11  ty7777
 * Updated to regenerate PETT6 .sxr files.
 *
 * Revision 1.2  1994/11/02  15:33:25  ty7777
 * Get the new Magnmification factor.
 *
 * Revision 1.1  1993/01/26  20:55:25  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)getmagf.c	11/03/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/xray/RCS/getmagf.c,v 1.8 1996/05/03 21:45:19 ty7777 Exp $";

/*___________________________________________________________________________
  File:		getmagf.c

  Function:	void GetMagnificationFactor ()

  Arguments:	

  Description:	This function get the magnification factor based on the patient 
		id number.

  Called by:	

  Calling:	

  History:	Created by Tom Yang on 11/03/92. 
___________________________________________________________________________*/

#include "xray.h"

PUBLIC int GetMagnificationFactor (sxr_root, acquired_after_oct94, hrscanner, 
	scanner_type, check_delta_z, incorrect_delta_z, sxr_output)
	char		*sxr_root;
	BOOLEAN		acquired_after_oct94;
	BOOLEAN		hrscanner;
	int		scanner_type;
	BOOLEAN		*check_delta_z;
	BOOLEAN		*incorrect_delta_z;
	SXR_OUTPUT	*sxr_output;
{
	char		*string_patient_id;
	int		patient_id;

	if (hrscanner == FALSE)
	{
		string_patient_id	= strpbrk (sxr_root, "0123456789");
		if (string_patient_id == NULL)
			return FAIL;
		patient_id		= atoi (string_patient_id);
	}

	if (scanner_type != ECAT953_SCANNER && scanner_type != ECAT961_SCANNER)
	{
		if (patient_id < OLDDEST_HIGH)
			sxr_output->magnification_factor	= OLDDEST_MAG_FACTOR;	
		else if (patient_id < OLD_HIGH)
			sxr_output->magnification_factor	= OLD_MAG_FACTOR;	
		else 
		{
			sxr_output->magnification_factor	= NEW_MAG_FACTOR;	
			if (patient_id <= INCORRECT_DELTAZ_LOW)
				*check_delta_z		= TRUE;
			else
				*incorrect_delta_z	= TRUE;
		}
	}
	else
	{
		if (hrscanner)
		{
			sxr_output->magnification_factor	= ECAT961_MAG_FACTOR;
		}
		else
		{
			if (acquired_after_oct94)
				sxr_output->magnification_factor	= ECAT_MAG_FACTOR_AFTER_OCT94;
			else
				sxr_output->magnification_factor	= ECAT_MAG_FACTOR;
		}
	}

	return SUCCEED;
}
