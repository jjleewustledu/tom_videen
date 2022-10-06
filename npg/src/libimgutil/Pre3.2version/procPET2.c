/* $Log: procPET2.c,v $
 * Revision 1.2  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 1.1  1999/07/14  21:13:51  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			procPET2.c
 *	Date:			May-98
 *	Author:			Tom Videen
 *	Description:	process PET counts to CBF, CBV, or counts
 *					Do not read hdr if flag = 0
 *	Input:
 *		pet	- mean counts
 *		hdr	- HEADER_DATA structure filled in
 *		flag - read hdr if > 0
 *	Returns:
 *		processed counts or (float) ERROR.
 *	Note:
 *		The hdr file is read in this routine for CBF or CBV but must be passed
 *		in for tissue activity since for tissue activity the factor depends upon
 *		decay and frame duration which must be computed from the file subheader.
 *		(see voistat)
 *
 *	Algorithm:
 *	1)  If (process = 0) then return pet;
 *	2)  If (process = PROC_CBV) and (hdr.ntype = 3) then process as blood volume:
 *		pet = hdr.bvfactor1 * pet
 *	3)  If (process = PROC_CBF)  and (hdr.ntype = 2 or 4) then process as blood flow:
 *		pet = pet * ((hdr.aflow1 * pet) + hdr.bflow1))
 *
 *	Reference:
 *		Videen et al (1987) JCBFM 7:513-516.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/procPET2.c,v 1.2 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <imgutil.h>

/*	-----------------------
 *	Function procPET2
 *	-----------------------
 */
float           procPET2 (pet, process, fname, hdr, recfp, flag)
	float           pet;			 /* mean counts */
	int             process;		 /* type of processing */
	char           *fname;			 /* file name */
	HEADER_DATA    *hdr;			 /* hdr structure */
	FILE           *recfp;			 /* rec file pointer */
	int				flag;
{

/*
 *	Counts
 */
	if (process == PROC_NONE)
		return (pet);

/*
 *	Cerebral Blood Volume
 */
	if (process == PROC_CBV) {
		if (flag > 0) {
			if (readhdr (fname, hdr, recfp) == ERROR)
				return ((float) ERROR);
		}
		if (hdr->ntype != 3)
			return ((float) ERROR);
		return (pet * hdr->bvfactor1);
	}
/*
 *	Cerebral Blood Flow
 */
	if (process == PROC_CBF) {
		if (flag > 0) {
			if (readhdr (fname, hdr, recfp) == ERROR)
				return ((float) ERROR);
		}
		if (hdr->ntype != 2 && hdr->ntype != 4)
			return ((float) ERROR);
		return (pet * ((hdr->aflow1 * pet) + hdr->bflow1));
	}
/*
 *	Decay-Corrected Tissue Activity time frame duration
 *	Units will be: (well counts/ml)
 */
	if (process == PROC_DECAYCOR)
		return (pet * hdr->decay_factor1);

/*
 *	Non-Decay-Corrected Tissue Activity time frame duration
 *	Units will be: (well counts/ml)
 */
	if (process == PROC_UNCORRECTED)
		return (pet * hdr->pettcnv1);

/*
 *	Oxygen processing using single image (ntype >= 11)
 */
	if (process == PROC_OEF || process == PROC_CMRO2) {
		if (flag > 0) {
			if (readhdr (fname, hdr, recfp) == ERROR)
				return ((float) ERROR);
		}
		if (hdr->ntype < 11)
			return ((float) ERROR);
		return (pet * hdr->aoxygen1);
	}
/*
 *	Not processed
 */
	return ((float) ERROR);
}
