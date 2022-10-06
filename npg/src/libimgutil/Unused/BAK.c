/* $Log: processOxygen.c,v $
 * Revision 2.1  1996/06/13  21:58:26  tom
 * eliminate some erroneous scalers left over from spideram
 * add scalefactor of 1000
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 *
 */
/*	========================================================================
 *	Module:			processOxygen.c
 *	Date:			22-Sep-93
 *	Author:			Tom Videen
 *	Description:	process an image for CBF, CBV, OEF, CMRO2 or counts and scale the result
 *
 *	Input:
 *		oxygenimage -   oxygen (oo) PET input image
 *		flowimage   -   flow (ho) PET input image
 *		volimage    -   vol (co) PET input image
 *		dim	      	-   dimensions of PET image
 *		scaler      -   normalization factor
 *		process     -   type of processing
 *		oxygenname  -   oxygen image file name
 *		flowname    -   flow image file name
 *		volname     -   vol image file name
 *		recfp       -   rec file pointer
 *
 *	Output:
 *		pet         -   processed, scaled PET image
 *		writes hdr files used to rec file
 *
 *	Returns:
 *		OK or ERROR
 *
 *	Algorithm:
 *	1)  If (process != PROC_CMRO2 AND process != PROC_OEF) then return ERROR.
 *	2)  Read hdr and process the flow image.
 *	3)  Read hdr and process the volume image.
 *	4)  Read hdr file associated with oxygen image.
 *	5)  If (hdr.ntype != 1) then return ERROR.
 *	6)  If (process = PROC_OEF) then process as oxygen extraction:
 *	7)  If (process = PROC_CBF")  and (hdr.ntype = 2 or 4) then process as blood flow:
 *	8)	All values are scaled up by 1000.
 *
 *	Reference and Equations in:
 *		Videen et al (1987) JCBFM 7:513-516.
 *
 *	History:
 *		05-Jan-93	Set scaler to 1 for processing cbf and cbv images (TOV)
 *		14-Jan-94	OEF must be between 0 and 1 (0 - 1000) (TOV)
 *		Apr-96		TOV add module to read hdrfiles.
 *	========================================================================
 */

#ifndef lint
static char     rcsid[] = "$Header: /home/petsun19/tom/src/libimgutil/RCS/processOxygen.c,v 2.1 1996/06/13 21:58:26 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <imgutil.h>
#include <image.h>

/*	-----------------------
 *	Function processOxygen
 *	-----------------------
 */
int             processOxygen (oxygenimage, flowimage, volimage, dim, scaler, process,
			                       oxygenname, flowname, volname, recfp)
	float        ***oxygenimage;	 /* oxygen (oo) PET input image */
	float        ***flowimage;		 /* flow (ho) PET input image */
	float        ***volimage;		 /* vol (co) PET input image */
	long            dim[3];			 /* dimensions of PET-array */
	float           scaler;			 /* normalization factor */
	int             process;		 /* type of processing */
	char           *oxygenname;		 /* oxygen image file name */
	char           *flowname;		 /* flow image file name */
	char           *volname;		 /* vol image file name */
	FILE           *recfp;			 /* rec file pointer */
{
	HEADER_DATA     hdr;			 /* hdr structure */
	long            i, j, k;
	float           sx;			 	 /* combined scaler */
	float           slvratio = 0.85; /* ratio of cerebral small vessel/large vessel hematocrit */
	float           volfactr = 0.835;/* (postcapillary vol + 0.5*capillary vol)/total blood vol */
	float           brainden = 1.05; /* density of brain (g/ml) */
	float           x1, x2, x3;		 /* image array elements from image arrays */
	float           c1;				 /* slvratio * integral of blood O2 counts/100.0 */
	float           c2;				 /* volfactr * c1 */
	float           c3;				 /* pettconv(n) / brainden */
	float           a1, a2, a3, a4, a5;	 /* temporary components in computation */
	int             localproc;		 /* type of processing */

/*
 *	Check process
 */

	if (process != PROC_OEF && process != PROC_CMRO2)
		return (ERROR);

/*
 *	Process flow image
 */

	localproc = PROC_CBF;
	if (readhdr (flowname, &hdr, recfp) == ERROR)
		return (ERROR);
	if (processPET (flowimage, dim, 1.0, localproc, &hdr) > 0) {
		fprintf (stderr, "Error processing %s\n", flowname);
		return (ERROR);
	}
/*
 *	Process volume image
 */

	localproc = PROC_CBV;
	if (readhdr (volname, &hdr, recfp) == ERROR)
		return (ERROR);
	if (processPET (volimage, dim, 1.0, localproc, &hdr) > 0) {
		fprintf (stderr, "Error processing %s\n", volname);
		return (ERROR);
	}
/*
 *  Read HDR file for oxygen image
 */

	if (readhdr (oxygenname, &hdr, recfp) == ERROR)
		return (ERROR);
	if (hdr.ntype != 1)
		return (ERROR);

/*
 *	Constants for oxygen processing
 */

	c1 = slvratio * hdr.o2cnts1 / 100.0;
	c2 = volfactr * c1;
	c3 = hdr.pettcnv1 / brainden;

/*
 *	Oxygen Extraction
 */
	if (process == PROC_OEF) {
		sx = scaler/.001;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					x1 = oxygenimage[k][j][i];
					if (x1 == 0.)
						oxygenimage[k][j][i] = 0;
					else {
						x2 = flowimage[k][j][i];
						x3 = volimage[k][j][i];
						a1 = c3 * x1;
						a2 = x2 * (hdr.awater1 * x2 + hdr.bwater1);
						a3 = c1 * x3;
						a4 = x2 * (hdr.aoxygen1 * x2 + hdr.boxygen1);
						a5 = c2 * x3;
						if (a4 == a5)
							oxygenimage[k][j][i] = 0;
						else
							oxygenimage[k][j][i] = sx * (a1 - a2 - a3) / (a4 - a5);
						if (oxygenimage[k][j][i] > 1000.)
							oxygenimage[k][j][i] = 1000.;
						else if (oxygenimage[k][j][i] < 0.)
							oxygenimage[k][j][i] = 0.;
					}
				}
			}
		}
		return (OK);
	}
/*
 *	Rate of Oxygen Utilization (CMRO2)
 */

	if (process == PROC_CMRO2) {
		sx = scaler/.001;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					x1 = oxygenimage[k][j][i];
					if (x1 == 0.)
						oxygenimage[k][j][i] = 0;
					else {
						x2 = flowimage[k][j][i];
						x3 = volimage[k][j][i];
						a1 = c3 * x1;
						a2 = x2 * (hdr.awater1 * x2 + hdr.bwater1);
						a3 = c1 * x3;
						a4 = x2 * (hdr.aoxygen1 * x2 + hdr.boxygen1);
						a5 = c2 * x3;
						if (a4 == a5)
							oxygenimage[k][j][i] = 0;
						else
							oxygenimage[k][j][i] = sx * x2 * hdr.oxycont1 * (a1 - a2 - a3) / (a4 - a5);
					}
				}
			}
		}
		return (OK);
	}
	return (OK);
}
