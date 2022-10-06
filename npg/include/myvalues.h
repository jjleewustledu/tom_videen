/*  $Log: myvalues.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.3  1999/12/17  17:03:34  tom
 * BOOLEAN change to int
 *
 * Revision 1.2  1996/04/04  14:09:18  tom
 * *** empty log message ***
 *
 * Revision 1.1  1996/03/05  22:43:47  tom
 * Initial revision
 *
    $Id: myvalues.h,v 2.0 2004/02/12 20:30:50 tom Exp $
 */
/* =============================================================================
 *  Module:         myvalues.h
 *  Date:           Dec 1995
 *  Author:         Tom Videen
 * =============================================================================
 */
#ifndef myvalues_dot_h_defined
#define myvalues_dot_h_defined

/*@unused@*/
#ifndef	lint
static char     myvalues_rcsid[] = "$Header: /usr/local/npg/include/RCS/myvalues.h,v 2.0 2004/02/12 20:30:50 tom Exp $";
#endif

typedef int	BOOLEAN;

#define TRUE		1
#define FALSE		0
#define OK			0
#define ERROR		-1

#define MAXTOKEN 	32
#define MAXFNAME	128
#define MAXLINE		512
#define MAXPATH		1024
#define MAXGRPS 	160

#endif
