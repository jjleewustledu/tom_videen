/* $Log: processOxygen.c,v $
 * Revision 2.12  2001/06/01  15:28:47  tom
 * use f,v for cbf,cbv
 *
 * Revision 2.11  2001/05/31  19:33:21  tom
 * add steady-state model
 *
 * Revision 2.10  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.9  1999/06/30  16:57:15  tom
 * add PROC_ODIV
 *
 * Revision 2.8  1996/11/21  17:47:12  tom
 * oef cannot be greater than 1.1
 * cmro2 cannot be greater than 100.
 *
 * Revision 2.7  1996/11/05  18:48:00  tom
 * remove .001 from sx scaler
 *
 * Revision 2.6  1996/09/30  22:40:30  tom
 * set range on CMRO2 to <= 100
 *
 * Revision 2.5  1996/09/17  17:28:49  tom
 * allocate hdr externally
 *
 * Revision 2.4  1996/09/03  19:22:15  tom
 * move readhdr to processPET.c
 *
 * Revision 2.3  1996/06/21  16:54:27  tom
 * set OEF limits at -100 and 1100
 *
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
 *		hdr         -   pointer to HEADER_DATA structure
 *
 *	Output:
 *		pet         -   processed, scaled PET image
 *		hdr			-	HEADER_DATA structure filled in
 *		writes hdr files used to file recfp
 *
 *	Returns:
 *		OK or ERROR
 *
 *	Algorithm:
 *	1)  If (process != PROC_CMRO2 AND process != PROC_OEF AND process != PROC_ODIV) then return ERROR.
 *	2)  Read hdr and process the flow image.
 *	3)  Read hdr and process the volume image.
 *	4)  Read hdr file associated with oxygen image.
 *	5)  If (hdr->ntype != 1) then return ERROR.
 *	6)  If (process = PROC_OEF) then process as oxygen extraction:
 *	7)  If (process = PROC_CBF")  and (hdr->ntype = 2 or 4) then process as blood flow:
 *	8)	All values are scaled up by 1000.
 *	9)	OEF cannot be > 1.1; CMRO2 cannot be > 100.0
 *
 *	Reference for Mintun bolus method and Equations in:
 *		Videen et al (1987) JCBFM 7:513-516.
 *	Reference for steady-state method and Equations in:
 *		Peter Herscovitch notes, Feb 1984
 *
 *	History:
 *		05-Jan-93	Set scaler to 1 for processing cbf and cbv images (TOV)
 *		14-Jan-94	OEF must be between 0 and 1.1 (0 - 1100) (TOV)
 *		Apr-96		TOV add module to read hdrfiles.
 *		May 2001	TOV add steady-state model for ntype=7
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/processOxygen.c,v 2.12 2001/06/01 15:28:47 tom Exp $";
#endif

#include <stdio.h>
#include <imgutil.h>

/*	-----------------------
 *	Function processOxygen
 *	-----------------------
 */
int             processOxygen (oxygenimage, flowimage, volimage, dim, scaler, process,
			                       oxygenname, flowname, volname, recfp, hdr)
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
	HEADER_DATA    *hdr;			 /* hdr structure */
{
	long            i, j, k;
	float           sx;				 /* combined scaler */
	float           slvratio = 0.85; /* ratio of small vessel/large vessel hematocrit */
	float           volfactr = 0.835;/* (postcap vol + 0.5*cap vol)/total blood vol */
	float           brainden = 1.05; /* density of brain (g/ml) */
	float           pc = 1.0;		 /* partition coef for water (ml blood/ml brain) */
	float           kd;				 /* decay constant  */
	float           watercont;		 /* O15 water content */
	float           cbf, cbv;		 /* computed flow and volume */
	float           f, v;			 /* flow in ml/ml/sec and volume in ml/ml */
	float           oo;				 /* oxygen image array element */
	float           c1, c2, c3;		 /* constants */
	float           a0, a1, a2, a3, a4, a5;	 /* temp values in computation */
	int             localproc;		 /* type of processing */

/*	Check process type	*/

	if (process != PROC_OEF && process != PROC_CMRO2 && process != PROC_ODIV)
		return (ERROR);

/*	Process flow image	*/

	localproc = PROC_CBF;
	sx = 1;
	if (processPET (flowimage, dim, sx, localproc, flowname, recfp, hdr) != OK) {
		fprintf (stderr, "Error processing %s\n", flowname);
		return (ERROR);
	}
/*	Process volume image */

	localproc = PROC_CBV;
	sx = 1;
	if (processPET (volimage, dim, sx, localproc, volname, recfp, hdr) != OK) {
		fprintf (stderr, "Error processing %s\n", volname);
		return (ERROR);
	}
	if (readhdr (oxygenname, hdr, recfp) == ERROR)
		return (ERROR);

/*	Mintun bolus oxygen method	*/

	if (hdr->ntype == 1) {
		c1 = slvratio * hdr->o2cnts1 / 100.0;
		c2 = volfactr * c1;
		c3 = hdr->pettcnv1 / brainden;
		sx = scaler;

		if (process == PROC_OEF) {
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						oo = oxygenimage[k][j][i];
						if (oo == 0.)
							oxygenimage[k][j][i] = 0;
						else {
							cbf = flowimage[k][j][i];
							cbv = volimage[k][j][i];
							a1 = c3 * oo;
							a2 = cbf * (hdr->awater1 * cbf + hdr->bwater1);
							a3 = c1 * cbv;
							a4 = cbf * (hdr->aoxygen1 * cbf + hdr->boxygen1);
							a5 = c2 * cbv;
							if (a4 == a5)
								oxygenimage[k][j][i] = 0;
							else
								oxygenimage[k][j][i] = sx * (a1 - a2 - a3) / (a4 - a5);
							if (oxygenimage[k][j][i] > 1.1)
								oxygenimage[k][j][i] = 1.1;
							else if (oxygenimage[k][j][i] < -1)
								oxygenimage[k][j][i] = -1;
						}
					}
				}
			}
			return (OK);

		} else if (process == PROC_CMRO2) {
			sx = scaler;
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						oo = oxygenimage[k][j][i];
						if (oo == 0.)
							oxygenimage[k][j][i] = 0;
						else {
							cbf = flowimage[k][j][i];
							cbv = volimage[k][j][i];
							a1 = c3 * oo;
							a2 = cbf * (hdr->awater1 * cbf + hdr->bwater1);
							a3 = c1 * cbv;
							a4 = cbf * (hdr->aoxygen1 * cbf + hdr->boxygen1);
							a5 = c2 * cbv;
							if (a4 == a5)
								oxygenimage[k][j][i] = 0;
							else
								oxygenimage[k][j][i] = sx * cbf * hdr->oxycont1 * (a1 - a2 - a3) / (a4 - a5);
							if (oxygenimage[k][j][i] > 100.)
								oxygenimage[k][j][i] = 100.;
							else if (oxygenimage[k][j][i] < -100.)
								oxygenimage[k][j][i] = -100.;
						}
					}
				}
			}
			return (OK);

		} else if (process == PROC_ODIV) {	/* Oxygen Divisor (for noise analysis) */
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						cbf = flowimage[k][j][i];
						cbv = volimage[k][j][i];
						a4 = cbf * (hdr->aoxygen1 * cbf + hdr->boxygen1);
						a5 = c2 * cbv;
						oxygenimage[k][j][i] = a4 - a5;
					}
				}
			}
			return (OK);
		}

/*	Steady-State method	*/

	} else if (hdr->ntype == 7) {
		kd = hdr->tau;
		watercont = hdr->oxycont1 * hdr->aoxygen1;
		c1 = hdr->pettcnv1 / hdr->scantime;
		c2 = slvratio * hdr->oxycont1;
		c3 = slvratio * volfactr * hdr->oxycont1;
		sx = scaler;

		if (process == PROC_OEF) {
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						oo = oxygenimage[k][j][i];
						if (oo == 0.)
							oxygenimage[k][j][i] = 0;
						else {
							f = flowimage[k][j][i] * brainden / (60*100);
							v = volimage[k][j][i] * brainden / 100;
							a0 = f / (kd + f / pc);
							a1 = oo * c1;
							a2 = a0 * watercont;
							a3 = v * c2;
							a4 = a0 * hdr->oxycont1;
							a5 = v * c3;
							if (a4 == a5)
								oxygenimage[k][j][i] = 0;
							else
								oxygenimage[k][j][i] = sx * (a1 - a2 - a3) / (a4 - a5);
							if (oxygenimage[k][j][i] > 1.1)
								oxygenimage[k][j][i] = 1.1;
							else if (oxygenimage[k][j][i] < -1)
								oxygenimage[k][j][i] = -1;
						}
					}
				}
			}
			return (OK);

		} else if (process == PROC_CMRO2) {
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						oo = oxygenimage[k][j][i];
						if (oo == 0.)
							oxygenimage[k][j][i] = 0;
						else {
							f = flowimage[k][j][i] * brainden / (60*100);
							v = volimage[k][j][i] * brainden / 100;
							a0 = f / (kd + f / pc);
							a1 = oo * c1;
							a2 = a0 * watercont;
							a3 = v * c2;
							a4 = a0 * hdr->oxycont1;
							a5 = v * c3;
							if (a4 == a5)
								oxygenimage[k][j][i] = 0;
							else
								oxygenimage[k][j][i] = sx * cbf * hdr->oxycont1 * (a1 - a2 - a3) / (a4 - a5);
							if (oxygenimage[k][j][i] > 100.)
								oxygenimage[k][j][i] = 100.;
							else if (oxygenimage[k][j][i] < -100.)
								oxygenimage[k][j][i] = -100.;
						}
					}
				}
			}
			return (OK);

		} else if (process == PROC_ODIV) {	/* Oxygen Divisor (for noise analysis) */
			for (k = 1; k <= dim[2]; ++k) {
				for (j = 1; j <= dim[1]; ++j) {
					for (i = 1; i <= dim[0]; ++i) {
						f = flowimage[k][j][i] * brainden / (60*100);
						v = volimage[k][j][i] * brainden / 100;
						a0 = f / (kd + f / pc);
						a4 = a0 * hdr->oxycont1;
						a5 = v * c3;
						oxygenimage[k][j][i] = a4 - a5;
					}
				}
			}
			return (OK);
		}
	} 
	return (ERROR);
}
