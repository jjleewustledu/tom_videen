/*  $Log: libnrimg.h,v $
 * Revision 1.7  1996/05/23  21:44:34  tom
 * add thintensor
 *
 * Revision 1.6  1996/04/04  14:08:35  tom
 * *** empty log message ***
 *
 * Revision 1.5  1996/03/08  23:33:04  tom
 * add writetensor6
 *
 * Revision 1.4  1996/03/08  17:58:04  tom
 * add freetensor7
 *
 * Revision 1.3  1996/03/08  17:53:54  tom
 * add readtensor7
 *
 * Revision 1.2  1996/03/06  15:18:22  tom
 * add writetensor7
 *
 * Revision 1.1  1996/03/05  22:43:39  tom
 * Initial revision
 *
    $Id: libnrimg.h,v 1.7 1996/05/23 21:44:34 tom Exp $
 */
/* =============================================================================
 *  Module:         libnrimg.h
 *  Date:           Dec-95
 *  Author:         Tom Videen
 *  Description:    Prototypes for routines in libnrimg (image i/o extensions to Num Recipes).
 *	Functions in:	libnrimg.a
 * =============================================================================
 */
#ifndef libnrimg_dot_h_defined
#define libnrimg_dot_h_defined

#ifndef	lint
static char     libnrimg_rcsid[] = "$Header: /usr/local/npg/include/RCS/libnrimg.h,v 1.7 1996/05/23 21:44:34 tom Exp $";
#endif

void            masktensor ();
float        ***padtensor ();
float        ***rdtensor ();
float        ***readtensor ();
void            subtensor ();
void            thintensor ();
float        ***trimtensor ();
int             writetensor ();
int             wrtensor ();

#endif	/* libnrimg_dot_h_defined */
