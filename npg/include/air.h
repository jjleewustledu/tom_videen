/*  ========================================================================
 *  Module:         AIR.h
 *  Date:           8/5/93
 *  Author:         Roger Woods
 *  Description:    Structure of air file from Automated Image Registration
 *	History:
 *		Abbreviated for use by TOV (11-Oct-95)
 *		Modified for AIR version 2.0 by TOV (15-Aug-96) using Woods' AIR.h (1/23/96)
 *  ========================================================================
 */
#ifndef lint
static char     rcsid_air[] = "$Header: /usr/local/npg/include/RCS/air.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

/* AIR.h  -- abbreviated for matrix extraction */

struct keyinfo {
	int             bits;
	int             xdim;
	int             ydim;
	int             zdim;
	double          xsize;
	double          ysize;
	double          zsize;
};

struct air1 {						/* AIR 1.0 .air files */
	double          e[4][3];
	char            stdfile[128];	/* standard or fixed image */
	struct keyinfo  std;
	char            resfile[128];	/* reslice or aligned image */
	struct keyinfo  res;
	char            comment[128];
	unsigned long int stdhash;
	unsigned long int reshash;
	char            reserved[120];
};

struct air2 {						/* AIR 2.0 .air files */
	double          e[4][4];
	char            stdfile[128];
	struct keyinfo  std;
	char            resfile[128];
	struct keyinfo  res;
	char            comment[128];
	unsigned long int stdhash;
	unsigned long int reshash;
	unsigned short  stdvolume;		 /* Not used in this version of AIR */
	unsigned short  resvolume;		 /* Not used in this version of AIR */
	char            reserved[116];
};

