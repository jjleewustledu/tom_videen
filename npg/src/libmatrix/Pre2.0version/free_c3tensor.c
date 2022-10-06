/*	$Id: free_c3tensor.c,v 1.5 1999/12/13 18:06:58 tom Exp $
	$Log: free_c3tensor.c,v $
 * Revision 1.5  1999/12/13  18:06:58  tom
 * tov revision
 *
 * Revision 1.4  1995/07/28  15:09:44  tom
 * *** empty log message ***
 *
 * Revision 1.3  1995/07/28  14:53:07  tom
 * *** empty log message ***
 *
 * Revision 1.2  1995/07/28  14:46:01  tom
 * debugged
 *
 * Revision 1.1  1995/07/24  20:53:14  tom
 * Initial revision
 * */
/*	========================================================================
 *	Module:			free_c3tensor.c
 *	Date:			28-Jul-95
 *	Author:			Tom Videen
 *	Description:	Free memory allocated to a 3D tensor of 8-bit chars
 *					with range t[nzl..nzh][nyl..nyy][nxl..nxh] allocated by c3tensor.
 *	========================================================================
 */
#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/free_c3tensor.c,v 1.5 1999/12/13 18:06:58 tom Exp $";
#endif

#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

void            free_c3tensor (t, nzl, nzh, nyl, nyy, nxl, nxh)
	char         ***t;
	long            nzl, nzh, nyl, nyy, nxl, nxh;

{
	free ((FREE_ARG) (t[nzl][nyl] + nxl - NR_END));
	free ((FREE_ARG) (t[nzl] + nyl - NR_END));
	free ((FREE_ARG) (t + nzl - NR_END));
}
