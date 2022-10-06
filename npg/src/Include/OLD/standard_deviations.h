/*$Id: standard_deviations.h,v 1.1 1995/10/30 18:25:09 tom Exp $*/
/*$Log: standard_deviations.h,v $
 * Revision 1.1  1995/10/30  18:25:09  tom
 * Initial revision
 *
 * Revision 1.2  1994/03/03  21:01:34  ty7777
 * Replace id with header.
 *
 * Revision 1.1  1993/11/11  17:54:12  ty7777
 * Initial revision
 **/

#ifndef STANDARD_DEVIATIONS_INCLUDED
#define STANDARD_DEVIATIONS_INCLUDED

/*_______________________________________________________________________
	Header File:	standard_deviations.h

	Description:	Filter table used by imageweld.

	Author:		Tom Yang.

	Date:		November 11, 1993.
_______________________________________________________________________*/

static char rcsid_standard_deviations [] = "$Header: /home/npggw/tom/inc/RCS/standard_deviations.h,v 1.1 1995/10/30 18:25:09 tom Exp $";

/*
 * Filter Table
 */

int weight2d [] = {1,
		   2,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   4,
		   3,
		   2,
		   1};

float standard_deviations3d [] = {0.1652,
				  0.1186,
				  0.0978,
				  0.0912,
				  0.0678,
				  0.0591,
				  0.0652,
				  0.0621,
				  0.0737,
				  0.0678,
				  0.0511,
				  0.0558,
				  0.0497,
				  0.0553,
				  0.0498,
				  0.0591,
				  0.0649,
				  0.0542,
				  0.0572,
				  0.0603,
				  0.0490,
				  0.0558,
				  0.0593,
				  0.0564,
				  0.0638,
				  0.0595,
				  0.0684,
				  0.0747,
				  0.0925,
				  0.1170,
				  0.1602};

#endif /* STANDARD_DEVIATIONS__INCLUDED */
