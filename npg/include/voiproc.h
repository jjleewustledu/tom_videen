/*	$Log: voiproc.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.2  1999/10/12  13:35:48  tom
 * correct comment
 * */
/* =============================================================================
 *  Module:         voiproc.h
 *  Date:           Jan-97
 *  Author:         Tom Videen
 *  Description:    Routines for voiproc
 *	History:
 *		Modified version of voistat.h
 * =============================================================================
 */
#ifndef voiproc_dot_h_defined
#define voiproc_dot_h_defined

#ifndef	lint
static char     voiproc_rcsid[] = "$Header: /usr/local/npg/include/RCS/voiproc.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
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
	float   *midtime;	/* mid-time for [frame] */
	float   *duration;	/* frame duration for [frame] */
	float  **mean;		/* mean for [frame][voi] */
} RGTDATA;	/* structure used for rgt data. */ 

void	average ();
int		readvoi ();
void	rgtmean ();
int		writergt ();

#endif 

