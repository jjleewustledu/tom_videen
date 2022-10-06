/* $Log: procOxygen.c,v $
 * Revision 1.4  2001/06/01  15:28:47  tom
 * use f,v for cbf,cbv
 *
 * Revision 1.3  2001/05/31  19:33:21  tom
 * add steady-state model
 *
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1998/05/27  15:38:08  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			procOxygen.c
 *	Date:			May-98
 *	Author:			Tom Videen
 *	Description:	process pet counts for CBF, CBV, OEF, CMRO2 or counts
 *
 *	Input:
 *		oxygencnts -   oxygen (oo) PET counts
 *		flowcnts   -   flow (ho) PET counts
 *		volcnts    -   vol (co) PET counts
 *		process    -   type of processing
 *		oxygenname -   oxygen image file name
 *		flowname   -   flow image file name
 *		volname    -   vol image file name
 *		hdr        -   pointer to HEADER_DATA structure
 *	Returns:
 *		processed counts or (float) ERROR.
 *
 *	Reference for Mintun bolus method and Equations in:
 *		Videen et al (1987) JCBFM 7:513-516.
 *	Reference for steady-state method and Equations in:
 *		Peter Herscovitch notes, Feb 1984
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutilOLD/RCS/procOxygen.c,v 1.4 2001/06/01 15:28:47 tom Exp $";
#endif

#include <stdio.h>
#include <imgutil.h>

/*	-----------------------
 *	Function procOxygen
 *	-----------------------
 */
float           procOxygen (oxygencnts, flowcnts, volcnts, process,
			                    oxygenname, flowname, volname, hdr, recfp)
	float           oxygencnts;		 /* oxygen (oo) PET input counts */
	float           flowcnts;		 /* flow (ho) PET input counts */
	float           volcnts;		 /* vol (co) PET input counts */
	int             process;		 /* type of processing */
	char           *oxygenname;		 /* oxygen image file name */
	char           *flowname;		 /* flow image file name */
	char           *volname;		 /* vol image file name */
	HEADER_DATA    *hdr;			 /* hdr structure */
	FILE           *recfp;			 /* rec file pointer */
{
	float           slvratio = 0.85; /* ratio small vessel/large vessel hematocrit */
	float           volfactr = 0.835;/* (postcap vol + 0.5*cap vol)/total blood vol */
	float           brainden = 1.05; /* density of brain (g/ml) */
	float           pc = 1.0;		 /* partition coef for water (ml blood/ml brain) */
	float           kd;				 /* decay constant  */
	float           watercont;		 /* O15 water content */
	float           cbf, cbv;		 /* computed flow and volume */
	float           f, v;		 	 /* flow in ml/ml/sec and volume in ml/ml */
	float           c1, c2, c3;		 /* constants */
	float           a1, a2, a3, a4, a5;	 /* temp values in computation */
	float           e;				 /* oxygen extraction    */
	float           cmro;			 /* oxygen metabolism    */
	int             localproc;		 /* type of processing */

/*	Check process	*/

	if (process != PROC_OEF && process != PROC_CMRO2)
		return ((float) ERROR);

/*	Process flow	*/

	localproc = PROC_CBF;
	cbf = procPET (flowcnts, localproc, flowname, hdr, recfp);
	if (cbf == (float) ERROR) {
		fprintf (stderr, "Error processing %s\n", flowname);
		return ((float) ERROR);
	}

/*	Process volume	*/

	localproc = PROC_CBV;
	cbv = procPET (volcnts, localproc, volname, hdr, recfp);
	if (cbv == (float) ERROR) {
		fprintf (stderr, "Error processing %s\n", volname);
		return ((float) ERROR);
	}

/*  Read HDR file for oxygen image	*/

	if (readhdr (oxygenname, hdr, recfp) == ERROR)
		return ((float) ERROR);

/*  Mintun bolus oxygen method  */

	if (hdr->ntype == 1) {
		if (oxygencnts == 0.)
			e = 0;
		else {
			c1 = slvratio * hdr->o2cnts1 / 100.0;
			c2 = volfactr * c1;
			c3 = hdr->pettcnv1 / brainden;
			a1 = c3 * oxygencnts;
			a2 = cbf * (hdr->awater1 * cbf + hdr->bwater1);
			a3 = c1 * cbv;
			a4 = cbf * (hdr->aoxygen1 * cbf + hdr->boxygen1);
			a5 = c2 * cbv;
			if (a4 == a5)
				e = 0;
			else
				e = (a1 - a2 - a3) / (a4 - a5);
		}
		if (process == PROC_CMRO2) {
			cmro = cbf * hdr->oxycont1 * e;
			return (cmro);
		} else
			return (e);

	} else if (hdr->ntype == 7) {
		if (oxygencnts == 0.)
			e = 0;
		else {
			f = brainden * cbf / (60 * 100);	/* (ml/100g/min) -> (ml/ml/sec) */
			v = brainden * cbv / 100;			/* (ml/100g) -> (ml/ml) */
			kd = hdr->tau;						/* 1/sec */
			c1 = f / (kd + f / pc);				/* unitless */
			watercont = hdr->oxycont1 * hdr->aoxygen1;	/* ml/ml */

			a1 = oxygencnts * hdr->pettcnv1 / hdr->scantime;
			a2 = c1 * watercont;
			a3 = v * slvratio * hdr->oxycont1;
			a4 = c1 * hdr->oxycont1;
			a5 = v * slvratio * volfactr * hdr->oxycont1;
			if (a4 == a5)
				e = 0;
			else
				e = (a1 - a2 - a3) / (a4 - a5);
		}
		if (process == PROC_CMRO2) {
			cmro = cbf * hdr->oxycont1 * e;
			return (cmro);
		} else
			return (e);

	} else
		return ((float) ERROR);
}
