/*  $Id: voistat.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 *	$Log: voistat.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.3  1996/04/26  15:07:37  tom
 * mean and sd indexed by [frame][voi]
 *
 * Revision 1.2  1996/04/26  13:32:28  tom
 * change VOI_FILETYPE to 30
 *
 * Revision 1.1  1996/04/25  16:09:10  tom
 * Initial revision
 *
 */
/* =============================================================================
 *  Module:         voimean.h
 *  Date:           Apr-96
 *  Author:         Tom Videen
 *  Description:    Routines for voimean
 *	History:
 *		Modified version of voimean.h by Tom Yang
 * =============================================================================
 */
#ifndef voimean_dot_h_defined
#define voimean_dot_h_defined

#ifndef	lint
static char     voimean_rcsid[] = "$Header: /usr/local/npg/include/RCS/voistat.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

#ifndef myvalues_dot_h_defined
#include <myvalues.h>
#endif

#define	MEAN_UNDEFINED		-99999.000
#define VOI_FILETYPE		30

typedef struct {
	char	*fname;
	int		 nvoi;
	int		 nframe;
	float   *time;		/* starttime for [frame] */
	float   *duration;	/* frame duration for [frame] */
	float  **mean;		/* mean for [frame][voi] */
	float  **sd;		/* SD for [frame][voi] */
} RGTDATA;	/* structure used for rgt data. */ 

void	average ();
int		readvoi ();
void	rgtmean ();
int		writergt ();

#endif 

