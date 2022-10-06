/*$Id: matpkg.h,v 1.2 1995/01/30 20:04:42 ty7777 Exp $*/
/*$Log: matpkg.h,v $
 * Revision 1.2  1995/01/30  20:04:42  ty7777
 * Added program version.
 **/

/* @(#)matpkg.h	1.2 3/15/91 */
#ifndef matpkg_h_defined
#define matpkg_h_defined
#include <math.h>

static char rcsid_matpkg [] = "$Header: /home/npggw/tom/src/3drecon/RCS/matpkg.h,v 1.2 1995/01/30 20:04:42 ty7777 Exp $";

typedef struct matrix
	{
	  int ncols, nrows;
	  float *data;
	}
*Matrix;

typedef struct vol3d
	{
	  int xdim, ydim, zdim;
	  float voxel_size;
	  float *data;
	}
*Vol3d;

typedef struct stack3d
	{
	  int xdim, ydim, zdim;
	  float xy_size, z_size;
	  float *data;
	}
*Stack3d;

typedef struct view2d
	{
	  int xdim, ydim;
	  float x_pixel_size, y_pixel_size;
	  float *data;
	}
*View2d;

Matrix matrix();
Vol3d make3d_volume();
Stack3d make3d_stack();
#endif matpkg_h_defined
