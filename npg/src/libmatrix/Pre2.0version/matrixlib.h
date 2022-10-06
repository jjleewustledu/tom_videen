/*	$Log: matrixlib.h,v $
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
	$Id: matrixlib.h,v 1.5 1999/02/19 16:16:22 tom Exp $
 */
/*  ========================================================================
 *  Module:         matrixlib.h
 *  Date:           Feb-99
 *  Author:         Tom Videen
 *  Description:    Function declarations for libmatrix.a
 *  ========================================================================
 */
#ifndef matrixlib_dot_h_defined
#define matrixlib_dot_h_defined

#ifndef	lint
static char     matrixlib_rcsid[] = "$Header: /home/npggw/tom/src/libmatrix/Pre2.0version/RCS/matrixlib.h,v 1.5 1999/02/19 16:16:22 tom Exp $";
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
short		  **smatrix();
void			free_c3tensor();
void			free_s3tensor();
void			free_smatrix();

#endif
