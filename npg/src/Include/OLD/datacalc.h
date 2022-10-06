/*     @(#)datacalc.h	2.1   2/24/92    */

/*   datacalc header file   */


#ifndef datacalc_dot_h
#define datacalc_dot_h

/*   standard system and sunview header files   */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

/*   CTI header files   */

#include "matrixx_extra.h"

/*  math macro's  */

#define sup(x, y) ((x) <= (y)) ? (y) : (x)
#define inf(x, y) ((x) <= (y)) ? (x) : (y)

/*  value used for overflow  */

double  overflow_val() ;
#define ROUND_NUM overflow_val()

/*  basic arithmetic and transformations  */

#define	MAT_ADD_MAT 0
#define MAT_SUB_MAT 1
#define MAT_MUL_MAT 2
#define MAT_DIV_MAT 3
#define MAT_POW_MAT 4
#define MAT_ADD_SCALAR 5
#define MAT_SUB_SCALAR 6
#define MAT_MUL_SCALAR 7
#define MAT_DIV_SCALAR 8
#define MAT_POW_SCALAR 9
#define SCALAR_SUB_MAT 10
#define SCALAR_DIV_MAT 11
#define MAT_SIN 12
#define MAT_ASIN 13
#define MAT_COS 14
#define MAT_ACOS 15
#define MAT_TAN 16
#define MAT_ATAN 17
#define MAT_EXP 18
#define MAT_LOG 19
#define MAT_EXP10 20
#define MAT_LOG10 21
#define MAT_ABS 22
#define SCALAR_ADD_SCALAR 23
#define SCALAR_SUB_SCALAR 24
#define SCALAR_MUL_SCALAR 25
#define SCALAR_DIV_SCALAR 26
#define SCALAR_POW_SCALAR 27
#define SCALAR_SIN 28
#define SCALAR_COS 29
#define SCALAR_EXP 30
#define SCALAR_LOG 31
#define SCALAR_10EXP 32
#define SCALAR_10LOG 33

/*  mathematical function declarations  */

extern void 	mat_add(),
		mat_sub(),
	  	mat_mult(),
		mat_div(),
		mat_abs(),
		mat_recip(),
		mat_sin(),
		mat_asin(),
		mat_cos(),
		mat_acos(),
	  	mat_tan(),
		mat_atan(),
		mat_exp(),
		mat_log(),
		mat_exp10(),
	    	mat_log10(),
		mat_pow(),
		mat_chs() ;

extern double	mat_qs(),
		mat_max(),
		mat_min() ;

extern void	scalar_mat_add(),
		scalar_mat_mult(),
		scalar_mat_div(),
		scalar_mat_pow() ;

extern double	s_add(),
		s_sub(),
		s_mult(),
		s_div(),
		s_abs(),
		s_recip(),
		s_sin(),
		s_asin(),
		s_cos(),
		s_acos(),
		s_tan(),
		s_atan(),
		s_exp(),
		s_log(),
		s_exp10(),
		s_log10(),
		s_pow(),
		s_chs() ;

int		matrix_abs(),
		matrix_acos(),
		matrix_add_matrix(),
		matrix_add_scalar(),
		matrix_asin(),
		matrix_atan(),
		matrix_cos(),
		matrix_div_matrix(),
		matrix_div_scalar(),
		matrix_exp(),
		matrix_exp10(),
		matrix_log(),
		matrix_log10(),
		matrix_mul_matrix(),
		matrix_mul_scalar(),
		matrix_pow_matrix(),
		matrix_pow_scalar(),
		matrix_sin(),
		matrix_sub_matrix(),
		matrix_sub_scalar(),
		matrix_tan() ;

/*  Matrix_List definition and functions  */

#define Mlist_NULL (Mlist*)0

typedef struct Matrix_List { int  nmats  ;
	                     int  frames ;
			     int  planes ;
	 		     int  gates  ;
			     int  beds   ;
	                     int  *list  ; }  Mlist ;

Mlist	*make_list(),
	*create_Mlist() ;

int    	*parse_matnum(),
	parse_list(),
	numcomp(),
	extract_num(),
	match_list(),
	match_files() ;

void    init_Mlist(),
	free_Mlist(),
	print_Mlist(),
	make_matlist(),
	make_num_list() ;

/*   Matrix file definitions and declarations   */

#define IMG MAT_IMAGE_DATA
#define SCN MAT_SCAN_DATA
#define ATN MAT_ATTN_DATA
#define NRM MAT_NORM_DATA
#define MATFILE_NULL (MatrixFile *)0
MatrixFile *make_new_file() ;

/*   error handling definitions   */

typedef struct DC_error { char   msg[256] ;
			  int	 errn ;
			} dc_err ;

void   set_dclib_err() ;
void   dclib_error() ;
#define DCLIB_FILE_TYPE_MISMATCH_ER 1
#define DCLIB_MAKE_MATLIST_ER 2
#define DCLIB_MAKE_NEW_FILE_ER 3
#define DCLIB_MEM_ALLOC_ER 4
#define DCLIB_FILE_OPEN_ER 5 
#define DCLIB_LIST_SYNTAX_ER 6
#define DCLIB_MATNUM_LIST_ER 7
#define DCLIB_DATACALC_DIR_ER 8
#define DCLIB_TMPNAME_ER 9
#define DCLIB_MAIN_HEADER_INIT_ER 10
#define DCLIB_DATA_READ_ER 11
#define DCLIB_DATA_DIM_MISMATCH_ER 12
#define DCLIB_CAL_UNITS_MISMATCH_ER 13
#define DCLIB_NOT_SCAN_OR_IMAGE_ER 14
#define DCLIB_DATA_WRITE_ER 15
#define DCLIB_NO_WEIGHT_FILE_ER 16
#define DCLIB_TOO_FEW_WEIGHTS_ER 17
#define DCLIB_MUST_BE_IMG_ER 18
#define DCLIB_UNKNOWN_FILE_TYPE_ER 19
#define DCLIB_NBHD_SIZE_ER 20
#define DCLIB_NO_MATS_FOUND_ER 21
#define DCLIB_DECAY_ER 22
#define DCLIB_DATACALC_OVERWRITE_ER 23

/*   neighborhood filters   */

#define NBHD_MEAN 0
#define NBHD_MEDIAN 1
#define NBHD_MIN 2
#define NBHD_MAX 3
#define NBHD_MAXDIF 4

int	nbhd_ops(),
	is_even(),
	compardbl(),
	matrix_nbhd_mean(),
	matrix_nbhd_median(),
	matrix_nbhd_min(),
	matrix_nbhd_max(),
	matrix_nbhd_max_dif() ;

double	nbhd_mean(),
	nbhd_median(),
	nbhd_op() ;

void	get_nbhd() ;

/*   operators   */

#define SOBEL 0
#define GRADIENT 1
#define LAPLACE 2
#define THRESH 3
#define MONO 4

int	matrix_gradient(),
	matrix_sobel(),
	matrix_laplace(),
	matrix_filters(),
	matrix_threshold(),
	matrix_fft(),
	mat_fft(),
	fft2d(),
	fft1d(),
	pad(),
	sobel(),
	gradient(),
	laplace(),
	thresh() ;

void	fft() ;

/*   structure defs for data summing   */

typedef struct SumList { int   nframes, *frames ;
			 int   nplanes, *planes ;
	                 int   ngates,  *gates ;
			 int   nbeds,	*beds ;
		       } Sum_List ;

Sum_List   *make_sum_list() ;
void   	   free_Sum_List() ;

/*   data summation and manipulation  */

int	matrix_frame_sum(),
	matrix_single_frame_add(),
	matrix_plane_sum(),
	matrix_single_plane_add(),
	matrix_decay(),
	mat_decay(),
	mat_make_subset(),
	single_frame_add(),
	single_plane_add(),
	frame_sum(),
	plane_sum(),
	matrix_make_subset() ;


/* 
 * Global Functions
 */
extern	MatrixData	*CreateMatrixData ();
extern	MatrixFile	*OpenEcat ();
extern	Image_subheader *matrixx_init_image_subheader ();
extern	MatrixFile	*pet_matrixx_open ();

		      
#endif datacalc_dot_h
