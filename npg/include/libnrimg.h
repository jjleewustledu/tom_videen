/*  $Log: libnrimg.h,v $
 * Revision 2.0  2004/02/12  20:30:50  tom
 * sync Feb 2004
 *
 * Revision 1.15  2004/01/08  15:36:50  tom
 * *** empty log message ***
 *
 * Revision 1.14  2002/11/27  18:24:25  tom
 * *** empty log message ***
 *
 * Revision 1.13  1999/12/17  17:03:22  tom
 * subtensors
 *
 * Revision 1.12  1999/09/24  16:37:08  tom
 * add s3tensor, free_s3tensor
 *
 * Revision 1.11  1999/09/24  16:25:12  tom
 * remove s3tensor, free_s3tensor (move to libmatrix.h)
 *
 * Revision 1.10  1999/08/13  20:19:57  tom
 * add s3tensor
 *
 * Revision 1.8  1999/01/12  15:29:29  tom
 * add s3tensor, free_s3tensor
 *
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
    $Id: libnrimg.h,v 2.0 2004/02/12 20:30:50 tom Exp tom $
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

/*@unused@*/
#ifndef	lint
static char     libnrimg_rcsid[] = "$Header: /usr/local/npg/include/RCS/libnrimg.h,v 2.0 2004/02/12 20:30:50 tom Exp tom $";
#endif

void            masktensor ();
void            maskobjtensor ();
void            maskalltensor ();
void            maskmatrix ();
void            maskrangetensor ();
void            threshtensor ();
float        ***padtensor ();
float        ***rdtensor ();
float        ***rdtensor1 ();
float        ***readtensor ();
float        ***readtensor_decay ();
float        ***readtensor_decaycor ();
void            subtensors ();
void            thintensor ();
float        ***trimtensor ();
int             writetensor ();
int             wrtensor ();
short		 ***s3tensor ();
void			free_s3tensor ();

#endif	/* libnrimg_dot_h_defined */
