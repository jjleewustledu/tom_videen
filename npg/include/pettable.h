/*	$Log: pettable.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.4  2003/09/16  16:55:22  tom
 * *** empty log message ***
 *
 * Revision 1.3  1996/04/04  14:09:33  tom
 * *** empty log message ***
 *
 * Revision 1.2  1996/03/07  15:18:10  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:44:02  tom
 * Initial revision
 *
	$Id: pettable.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* ===============================================================================
 *	Module:			pettable.h
 *	Date:			Jan 1993
 *	Author:			Tom Yang
 *	Description:	Define filter names.
 *	History:
 *		Dec-95	add FILTERG, FILTERF (PETT VI filters)
 * ===============================================================================
 */
#ifndef PETTABLE_INCLUDED
#define PETTABLE_INCLUDED

/*@unused@*/
#ifndef lint
static char rcsid_pettable [] = "$Header: /usr/local/npg/include/RCS/pettable.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#define RAMP_INDEX				1		/* index to filterTable below */
#define GAUSS_INDEX				7
#define BUTTERWORTH_INDEX		8

#define GAUSSIAN_FILTER_CODE    100		/* old code adds this to filter_code */
#define BUTTERWORTH_FILTER_CODE 200

static char *filterTable [] = {"None        ",
			"Ramp        ",
			"Butterworth ",
			"Hanning     ",
			"Hamming     ",
			"Parzen      ",
			"Shepp       ",
			"3D Gaussian ",
			"3D Butterwor"};

#endif /* PETTABLE_INCLUDED */
