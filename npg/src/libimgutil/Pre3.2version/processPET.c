/* $Log: processPET.c,v $
 * Revision 2.5  1999/12/13  17:04:17  tom
 * tov revision
 *
 * Revision 2.4  1996/11/22  16:03:04  tom
 * add oxygen processing of single images when ntype > 10
 *
 * Revision 2.3  1996/10/30  22:12:23  tom
 * do NOT call readhdr for tissue activity
 *
 * Revision 2.2  1996/09/03  19:22:32  tom
 * change passed parameters
 *
 * Revision 2.1  1996/04/29  14:23:51  tom
 * *** empty log message ***
 *
 * Revision 2.0  1996/04/24  23:07:41  tom
 * new library release
 *
 * Revision 1.1  1996/03/12  23:00:34  tom
 * Initial revision
 *
 */
/*	========================================================================
 *	Module:			processPET.c
 *	Date:			28-Apr-93
 *	Author:			Tom Videen
 *	Description:	process an image for CBF, CBV, or counts
 *	Input:
 *		pet	-   unprocessed PET image
 *		hdr	- HEADER_DATA structure filled in
 *	Output:
 *		pet  -   processed, scaled PET image
 *
 *	Returns:
 *		OK or ERROR
 *	Note:
 *		The hdr file is read in this routine for CBF or CBV but must be passed
 *		in for tissue activity since for tissue activity the factor depends upon
 *		decay and frame duration which must be computed from the file subheader.
 *		(see voistat)
 *
 *	Algorithm:
 *	1)  If (process = 0) then only scale:
 *		pet = scaler * pet
 *	2)  If (process = PROC_CBV) and (hdr.ntype = 3) then process as blood volume:
 *		pet = scaler * hdr.bvfactor1 * pet
 *	3)  If (process = PROC_CBF)  and (hdr.ntype = 2 or 4) then process as blood flow:
 *		pet = scaler * pet * ((hdr.aflow1 * pet) + hdr.bflow1))
 *
 *	Reference:
 *		Videen et al (1987) JCBFM 7:513-516.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libimgutil/Pre3.2version/RCS/processPET.c,v 2.5 1999/12/13 17:04:17 tom Exp $";
#endif

#include <stdio.h>
#include <imgutil.h>

/*	-----------------------
 *	Function processPET
 *	-----------------------
 */
int             processPET (pet, dim, scaler, process, fname, recfp, hdr)
	float        ***pet;			 /* PET input image */
	long           *dim;			 /* dimensions of PET-array */
	float           scaler;			 /* normalization factor */
	int             process;		 /* type of processing */
	char           *fname;			 /* file name */
	FILE           *recfp;			 /* rec file pointer */
	HEADER_DATA    *hdr;			 /* hdr structure */
{
	long            i, j, k;
	float           x;			 /* temp for pet */
	float           sx;			 /* combined scaler */

/*
 *	Counts
 */
	if (process == PROC_NONE) {
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					pet[k][j][i] = scaler * pet[k][j][i];
				}
			}
		}
		return (OK);
	}
/*
 *	Cerebral Blood Volume
 */
	if (process == PROC_CBV) {
		if (readhdr (fname, hdr, recfp) == ERROR)
			return (ERROR);
		if (hdr->ntype != 3)
			return (ERROR);
		sx = scaler * hdr->bvfactor1;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					pet[k][j][i] = sx * pet[k][j][i];
				}
			}
		}
		return (OK);
	}
/*
 *	Cerebral Blood Flow
 */
	if (process == PROC_CBF) {
		if (readhdr (fname, hdr, recfp) == ERROR)
			return (ERROR);
		if (hdr->ntype != 2 && hdr->ntype != 4)
			return (ERROR);
		sx = scaler;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					x = pet[k][j][i];
					pet[k][j][i] = sx * x * ((hdr->aflow1 * x) + hdr->bflow1);
				}
			}
		}
		return (OK);
	}
/*
 *	Decay-Corrected Tissue Activity time frame duration
 *	Units will be: (well counts/ml)
 */
	if (process == PROC_DECAYCOR) {
		sx = scaler * hdr->decay_factor1;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					x = pet[k][j][i];
					pet[k][j][i] = sx * x;
				}
			}
		}
		return (OK);
	}
/*
 *	Non-Decay-Corrected Tissue Activity time frame duration
 *	Units will be: (well counts/ml)
 */
	if (process == PROC_UNCORRECTED) {
		sx = scaler * hdr->pettcnv1;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					x = pet[k][j][i];
					pet[k][j][i] = sx * x;
				}
			}
		}
		return (OK);
	}
/*
 *	Oxygen processing using single image (ntype >= 11)
 */
	if (process == PROC_OEF || process == PROC_CMRO2) {
		if (readhdr (fname, hdr, recfp) == ERROR)
			return (ERROR);
		if (hdr->ntype < 11)
			return (ERROR);
		sx = scaler * hdr->aoxygen1;
		for (k = 1; k <= dim[2]; ++k) {
			for (j = 1; j <= dim[1]; ++j) {
				for (i = 1; i <= dim[0]; ++i) {
					pet[k][j][i] = sx * pet[k][j][i];
				}
			}
		}
		return (OK);
	}
/*
 *	Not processed
 */
	return (ERROR);
}
