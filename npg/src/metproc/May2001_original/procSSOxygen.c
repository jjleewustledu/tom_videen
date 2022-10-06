/* $Log$ */
/*	========================================================================
 *	Module:			procSSOxygen.c
 *	Date:			May 2001
 *	Author:			Tom Videen
 *	Description:	process pet counts for CBF, CBV, OEF, CMRO2 or counts
 *					using steady-state model for oxygen
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
 *		Oxygen Extraction, CMRO2, or (float) ERROR.
 *
 *	Reference and Equations in:
 *		Lammertsma & Jones (1983) JCBFM, 3:416-424
 *		equations C.7, 13, and 14
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/RCS/procOxygen.c,v 1.2 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <imgutil.h>

/*	-----------------------
 *	Function procSSOxygen
 *	-----------------------
 */
float           procSSOxygen (oxygencnts, flowcnts, volcnts, process,
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
	float           brainden = 1.05; /* density of brain (g/ml) */
	float           pc = 1.0;		 /* partition coefficient for water (ml blood/ml brain) */
	float           b2p_water;		 /* blood/plasma counts during H2O	 */
	float           b2p_oxygen;		 /* blood/plasma counts during O2	 */
	float           plasma_conc;	 /* plasma concentration of O15 after O2	 */
	float           ep;				 /* uncorrected oxygen extraction	 */
	float           e;				 /* oxygen extraction	 */
	float           cmro;			 /* oxygen metabolism	 */
	float           a0;				 /* oxygen counts	*/
	float           kd;				 /* decay constant	*/
	float           scantime;		 /* scantime (min)	*/
	float           k, x;			 /* temporary components in computation */
	float           cbf, cbv;		 /* computed flow and volume */
	int             localproc;		 /* type of processing */

/*	Check process	*/

	if (process != PROC_OEF && process != PROC_CMRO2)
		return ((float) ERROR);

/*	Process flow */

	if (readhdr (flowname, hdr, recfp) == ERROR)
		return ((float) ERROR);
	if (hdr->ntype != 2)
		return ((float) ERROR);
	if (hdr->ho_blood_to_plasma == 0)
		return ((float) ERROR);
	else
		b2p_water = hdr->ho_blood_to_plasma;
	localproc = PROC_CBF;
	cbf = procPET (flowcnts, localproc, flowname, hdr, recfp);
	if (cbf == (float) ERROR) {
		fprintf (stderr, "Error processing %s\n", flowname);
		return ((float) ERROR);
	}

/*	Process volume */

	localproc = PROC_CBV;
	cbv = procPET (volcnts, localproc, volname, hdr, recfp);
	if (cbv == (float) ERROR) {
		fprintf (stderr, "Error processing %s\n", volname);
		return ((float) ERROR);
	}

/*   Read HDR file for oxygen image	*/

	if (readhdr (oxygenname, hdr, recfp) == ERROR)
		return ((float) ERROR);
	if (hdr->ntype != 1)
		return ((float) ERROR);

/*	Oxygen Extraction
 *	kd = decay_constant (1/sec)
 *	pc = partition coefficient for water = 1.0 (ml blood/ml brain)
 *	scantime = min
 *	oxygencnts = ECATcounts/pixel/scantime
 *	pettcnv1 = decay-corrected conversion factor (wellcounts/ml/sec) / (ECATcounts/pixel/min)
 *	a0 = decay-corrected (wellcounts/ml)
 *	plasma_conc = wellcounts/ml
 */

	cbf = brainden * cbf / 60 / 100;		/* converts (ml/100g/min) -> (ml/ml/sec)	*/
	cbv = brainden * cbv / 100;				/* converts (ml/100g) -> (ml/ml)	*/
	b2p_oxygen = hdr->oo_blood_to_plasma;
	plasma_conc = hdr->oo_plasma_conc;
	kd = hdr->tau;
	scantime = hdr->scantime / 60.;

	a0 = oxygencnts * hdr->pettcnv1 / scantime;
	k = kd/cbf + 1/pc;
	ep = (a0*k/plasma_conc - b2p_water) / (b2p_oxygen - b2p_water);
	x = (cbf + kd) / (cbf/cbv + kd);
	if (x == 1)
		e = 0;
	else
		e = (ep - x) / (1 - x);

/*	Rate of Oxygen Utilization (CMRO2)	*/

	if (process == PROC_CMRO2) {
		cmro = cbf * hdr->oxycont1 * e;
		return (cmro);
	} else
		return (e);
}
