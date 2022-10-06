/*$Id: fproj3d.h,v 1.2 1995/01/30 20:03:34 ty7777 Exp $*/
/*$Log: fproj3d.h,v $
 * Revision 1.2  1995/01/30  20:03:34  ty7777
 * Added program version.
 **/

/* @(#)fproj3d.h	1.2 7/10/91 */

#ifndef fproj3d_def

static char rcsid_fproj3d [] = "$Header: /home/npggw/tom/src/3drecon/RCS/fproj3d.h,v 1.2 1995/01/30 20:03:34 ty7777 Exp $";

typedef struct vol3d_info
	{
	  unsigned int xdim, ydim, zdim;
	  float voxel_size;
	}
*Vol3d_info;

typedef struct view2d_info
	{
	  unsigned int xdim, ydim;
	  float x_pixel_size, y_pixel_size;
	}
*View2d_info;

typedef struct stack3d_info
	{
	  unsigned int xdim, ydim, zdim;
	  float xy_size, z_size;
	}
*Stack3d_info;

#define fproj3d_def
#endif
