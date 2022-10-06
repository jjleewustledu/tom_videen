/*	$Log: matrixlib.h,v $
 * Revision 2.1  2004/08/31  14:39:24  tom
 * double tensor
 *
 * Revision 2.2  2004/02/12  22:12:27  tom
 * Feb 2004
 *
 * Revision 2.0  2004/01/02  21:40:12  tom
 * splint cleaning
 *
 * Revision 1.5  1999/02/19  16:16:22  tom
 * *** empty log message ***
 *
 * Revision 1.3  1999/02/19  16:15:36  tom
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/22  20:09:57  tom
 * add smatrix, free_smatrix
 *
 * Revision 1.1  1996/03/05  22:43:45  tom
 * Initial revision
 *
	$Id: matrixlib.h,v 2.1 2004/08/31 14:39:24 tom Exp $
 */
/*  ========================================================================
 *  Module:         matrixlib.h
 *  Date:           Aug 2004
 *  Author:         Tom Videen
 *  Description:    Function declarations for libmatrix.a
 *  ========================================================================
 */
#ifndef matrixlib_dot_h_defined
#define matrixlib_dot_h_defined

#ifndef	lint
static char     matrixlib_rcsid[] = "$Header: /usr/local/npg/include/RCS/matrixlib.h,v 2.1 2004/08/31 14:39:24 tom Exp $";
#endif

void            dmatrix_add();
void            dmatrix_mult();
void            dmatrix_print();
double			dvector_dist();
void            matrix_add();
void            matrix_mult();
void            matrix_print();
void            matrix_sub();
float         **matrix_trans();
float			vector_dist();
void			vector_mult();
char		 ***c3tensor();
short		 ***s3tensor();
double		 ***d3tensor();
short		  **smatrix();
void			free_c3tensor();
void			free_s3tensor();
void			free_d3tensor();
void			free_smatrix();

#endif
