/*	$Log: matrixlib.h,v $
 * Revision 1.1  1996/03/05  22:43:45  tom
 * Initial revision
 *
	$Id: matrixlib.h,v 1.1 1996/03/05 22:43:45 tom Exp $
 */
/*  ========================================================================
 *  Module:         matrixlib.h
 *  Date:           20-Jul-95
 *  Author:         Tom Videen
 *  Description:    Function declarations for libmatrix.a
 *  ========================================================================
 */
#ifndef matrixlib_dot_h_defined
#define matrixlib_dot_h_defined

#ifndef	lint
static char     matrixlib_rcsid[] = "$Header: /usr/local/npg/include/RCS/matrixlib.h,v 1.1 1996/03/05 22:43:45 tom Exp $";
#endif

void            dmatrix_add ();
void            dmatrix_sub ();
void            dmatrix_mult ();
void            dmatrix_print ();
void            matrix_add ();
void            matrix_sub ();
void            matrix_mult ();
void            matrix_print ();
float         **matrix_trans ();
float			vector_dist ();
double			dvector_dist ();
char			***c3tensor();
short			***s3tensor();
void			free_c3tensor();
void			free_s3tensor();

#endif
