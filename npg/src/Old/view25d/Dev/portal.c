/*$Id: portal.c,v 2.38 1995/10/26 19:06:34 ty7777 Exp tom $*/
/*$Log: portal.c,v $
 * Revision 2.38  1995/10/26  19:06:34  ty7777
 * Display labels.
 *
 * Revision 2.37  1995/10/26  19:02:28  ty7777
 * Fixed the coronal and sagittal offset bug.
 *
 * Revision 2.36  1995/09/08  21:40:44  ty7777
 * Updated to introduce atlas 2.
 *
 * Revision 2.35  1995/09/07  17:04:13  ty7777
 * Deleted error message.
 *
 * Revision 2.34  1995/09/07  16:55:05  ty7777
 * Fixed the crash in RedrawPortals.
 *
 * Revision 2.33  1995/08/31  20:18:38  ty7777
 * Before watch cursor works.
 *
 * Revision 2.32  1995/08/29  16:46:37  ty7777
 * Added mutual tracking.
 *
 * Revision 2.31  1995/08/29  15:52:58  ty7777
 * After z0 works correctly.
 *
 * Revision 2.30  1995/08/28  21:09:23  ty7777
 * After color bar works.
 *
 * Revision 2.29  1995/08/28  15:38:46  ty7777
 * Before adding mutual image1 and image2 point display.
 *
 * Revision 2.28  1995/08/22  16:51:58  ty7777
 * Before doing anything else.
 *
 * Revision 2.27  1995/08/02  16:49:48  ty7777
 * Tracking works.
 *
 * Revision 2.26  1995/07/31  15:10:38  ty7777
 * Before work on tracking.
 *
 * Revision 2.25  1995/07/27  19:55:14  ty7777
 * Draw atlas only for image1 when there is overlay.
 *
 * Revision 2.24  1995/07/26  20:41:17  ty7777
 * Made image overlay work.
 *
 * Revision 2.23  1995/07/25  16:12:08  ty7777
 * Fixed the offset problem for Image 2.
 *
 * Revision 2.22  1995/07/24  20:28:17  ty7777
 * The mask for interfile worked.
 *
 * Revision 2.21  1995/07/14  20:53:34  ty7777
 * Before further changes.
 *
 * Revision 2.20  1995/07/07  17:12:30  ty7777
 * Two data sets works correctly.
 *
 * Revision 2.19  1995/06/30  15:22:12  ty7777
 *  Before making further changes, tow independent color bars worked.
 *
 * Revision 2.18  1995/06/14  20:12:10  ty7777
 * Added processing for analyze .hdr file.
 *
 * Revision 2.17  1995/06/13  20:12:47  ty7777
 * Added processing for reading .hdr files.
 *
 * Revision 2.16  1995/06/12  19:30:55  ty7777
 * Before adding processing for .hdr file.
 *
 * Revision 2.15  1995/06/07  14:36:03  ty7777
 * Relieve the checking for 75 slices.
 *
 * Revision 2.14  1995/06/05  19:50:57  ty7777
 * Before changing the constraint on the number of slices to display.
 *
 * Revision 2.13  1995/06/02  14:23:13  ty7777
 * Added InputInterfileData ().
 *
 * Revision 2.12  1995/01/18  20:11:16  ty7777
 * Added black border for PosNeg_on_White.tbl.
 *
 * Revision 2.11  1995/01/17  20:07:49  ty7777
 * Deleted things that are related to data_stes.
 *
 * Revision 2.10  1994/12/30  21:20:58  ty7777
 * Added processing for big fonts.
 *
 * Revision 2.9  1994/12/01  20:09:48  ty7777
 * Get rid of the path in atl files.
 *
 * Revision 2.8  1994/12/01  16:58:44  ty7777
 * Pass library_path from the main program.
 *
 * Revision 2.7  1994/11/30  20:00:08  ty7777
 * Same as the last version.
 *
 * Revision 2.6  1994/11/01  17:36:48  ty7777
 * Change the mask file if necessary when reading defaults file.
 *
 * Revision 2.5  1994/10/28  16:17:43  ty7777
 * Use winutil.h
 *
 * Revision 2.4  1994/10/27  20:02:04  ty7777
 * Added checking for zorigin.
 *
 * Revision 2.3  1994/10/26  16:15:35  ty7777
 * Added winutil.h
 *
 * Revision 2.2  1994/10/19  17:17:38  ty7777
 * Change IF to if.
 *
 * Revision 2.1  1994/10/19  17:01:16  ty7777
 * The pattern *pattern* works.
 *
 * Revision 1.99  1994/10/17  19:53:07  ty7777
 * Before file patterns work.
 *
 * Revision 1.98  1994/10/14  18:07:42  ty7777
 * Allow maximum distance for single slices.
 *
 * Revision 1.97  1994/10/13  18:54:48  ty7777
 * Uses pett6_n99 as the default t88 mask.
 *
 * Revision 1.96  1994/08/25  16:40:41  ty7777
 * Changed the documentation.
 *
 * Revision 1.95  1994/08/25  16:39:00  ty7777
 * Same as the last one.
 * X11R5 version.
 *
 * Revision 1.94  1994/08/17  21:19:36  ty7777
 * Delected redundant code for getting the zorigin.
 *
 * Revision 1.93  1994/08/17  20:26:19  ty7777
 * Added processing for X, Y, Z values when getting defaults.
 *
 * Revision 1.92  1994/08/17  19:46:01  ty7777
 * Display the bottom slices of the PET iamge.
 *
 * Revision 1.91  1994/08/17  16:16:29  ty7777
 * Apply the zorigin only to the transverse slices.
 *
 * Revision 1.90  1994/08/16  15:45:44  ty7777
 * Added error checking for the first and last slice.
 *
 * Revision 1.89  1994/08/12  16:59:19  ty7777
 * Use the index in BuildQueue ().
 *
 * Revision 1.88  1994/08/11  21:21:13  ty7777
 * Changed the error message.
 *
 * Revision 1.87  1994/06/23  17:12:24  ty7777
 * Delected the debugging messages.
 *
 * Revision 1.86  1994/06/23  16:57:11  ty7777
 * Added checking for the first slice and last slice in GetDefaultParameters ().
 *
 * Revision 1.85  1994/06/02  20:55:16  ty7777
 * Match the PET images correctly with the atlas slices in the right
 * handed coordinate system.
 *
 * Revision 1.84  1994/05/19  14:11:53  ty7777
 * Changed from (z, x, y) to (x, y, z).
 *
 * Revision 1.83  1994/05/09  21:59:17  ty7777
 * Update for mask 75 slice images and read zorigin.
 *
 * Revision 1.82  1994/05/09  16:43:31  ty7777
 * Changed the processing for mask to use zorigin.
 *
 * Revision 1.81  1994/05/09  16:07:53  ty7777
 * Added color table Color43.tbl.
 *
 * Revision 1.80  1994/05/06  19:40:45  ty7777
 * Added processing for zorigin display for coronal and sagittal sections.
 *
 * Revision 1.79  1994/04/18  17:55:03  ty7777
 * Added processing for zorigin.
 *
 * Revision 1.78  1994/04/14  20:38:12  ty7777
 * Uses slice2z () and z2slice ().
 *
 * Revision 1.77  1994/04/13  15:09:59  ty7777
 * Changed the color table names.
 *
 * Revision 1.76  1994/04/12  20:46:15  ty7777
 * Added frames for color bar.
 *
 * Revision 1.75  1994/04/12  20:08:59  ty7777
 * Adding frames to black and white color bar.
 *
 * Revision 1.74  1994/04/12  15:27:41  ty7777
 * Lowered () is changed.
 *
 * Revision 1.73  1994/04/07  16:52:09  ty7777
 * Final checking in.
 *
 * Revision 1.72  1994/02/17  17:27:44  ty7777
 * Fixed show points error for FACING_LEFT slices.
 *
 * Revision 1.71  1994/02/11  22:23:15  ty7777
 * Fixed region display problem.
 *
 * Revision 1.70  1994/01/24  20:08:25  ty7777
 * Added copyright information.
 *
 * Revision 1.69  1994/01/12  21:37:49  ty7777
 * Fix GetLegendDim bug.
 *
 * Revision 1.68  1994/01/11  22:24:18  ty7777
 * Added processing for legend.
 *
 * Revision 1.67  1994/01/11  17:55:38  ty7777
 * Make the Show Coordinates right for Talairach atlases.
 *
 * Revision 1.66  1994/01/11  16:15:52  ty7777
 * Changed the HD6 Brain and T88 Brain to original and hd6tot88.
 *
 * Revision 1.65  1993/12/16  15:51:27  ty7777
 * Modified DrawLabel. Added GetLabelDim.
 *
 * Revision 1.64  1993/12/14  20:00:07  ty7777
 * Added processing for AF3D display in UpdatePortals.
 *
 * Revision 1.63  1993/12/13  20:21:48  ty7777
 * Deleted old DrawLabel.
 *
 * Revision 1.62  1993/12/13  20:20:22  ty7777
 * Modified DrawLabel.
 *
 * Revision 1.61  1993/12/13  19:43:50  ty7777
 * Deleted debugging lines.
 *
 * Revision 1.60  1993/12/13  15:19:59  ty7777
 * After adding view25d.
 *
 * Revision 1.59  1993/12/08  21:02:40  ty7777
 * Fixed problem for reading default symbol type and symbol color.
 *
 * Revision 1.58  1993/11/02  16:52:32  ty7777
 * Changed ErrorMinMax.
 *
 * Revision 1.57  1993/11/01  21:57:27  ty7777
 * Fixed error checking for Fran.
 *
 * Revision 1.56  1993/10/28  15:44:36  ty7777
 * Fixed bug for displaying sagittal slices facing on left with atlas trim on.
 *
 * Revision 1.55  1993/10/22  20:05:04  ty7777
 * Change RegularMinMax.
 *
 * Revision 1.54  1993/09/27  19:00:52  ty7777
 * Added gray color button for atlas color.
 *
 * Revision 1.53  1993/09/23  20:40:58  ty7777
 * View and Log Coordinates File.
 *
 * Revision 1.52  1993/07/26  22:01:59  ty7777
 * Modified mormula for HD6 to T88 conversion.
 *
 * Revision 1.51  1993/07/13  15:06:51  ty7777
 * After adding show point location.
 *
 * Revision 1.50  1993/07/06  16:49:49  ty7777
 * Before working on getting points.
 *
 * Revision 1.49  1993/06/03  19:38:56  ty7777
 * Make coronal and saggital reginos display work.
 *
 * Revision 1.48  1993/06/03  14:33:17  ty7777
 * After coronal and saggital regions show.
 *
 * Revision 1.47  1993/06/02  17:17:14  ty7777
 * After multiple regions display worked.
 *
 * Revision 1.46  1993/05/17  17:17:50  ty7777
 * Move CreateXorGC () to xor.c in libhp.a.
 *
 * Revision 1.45  1993/05/17  16:23:48  ty7777
 * Fixed any scale problem.
 *
 * Revision 1.44  1993/05/07  21:55:53  ty7777
 * Processing for pet mask image.
 *
 * Revision 1.43  1993/05/07  16:56:25  ty7777
 * Before testing number of slices.
 *
 * Revision 1.42  1993/04/05  17:32:58  ty7777
 * After add AF3D points works.
 *
 * Revision 1.41  1993/02/23  18:15:15  ty7777
 * Before fixing ECAT image bug.
 *
 * Revision 1.40  1993/01/28  15:42:56  ty7777
 * Before changing libcti.a
 *
 * Revision 1.39  1993/01/13  19:32:49  ty7777
 * Modified "atlas trim" to make it work correctly.
 *
 * Revision 1.38  1993/01/11  15:51:00  ty7777
 * Add atlas trim.
 *
 * Revision 1.37  1992/12/31  21:56:51  ty7777
 * Before Randy's change.
 *
 * Revision 1.36  1992/12/07  20:34:28  ty7777
 * Adding a message box when the user gives an ascii af3d file.
 *
 * Revision 1.35  1992/12/07  15:51:57  ty7777
 * Same as the last version.
 *
 * Revision 1.34  1992/12/03  20:25:12  ty7777
 * Before testing libcti.a library.
 *
 * Revision 1.33  1992/11/20  16:35:26  ty7777
 * Working and Updated version.
 *
 * Revision 1.31  1992/10/30  21:51:25  ty7777
 * portal.c checking in.
 *
 * Revision 1.30  1992/09/28  16:47:24  ty7777
 * After Add "xlabel" button.
 *
 * Revision 1.29  1992/09/23  19:30:26  ty7777
 * Same as the last version.
 *
 * Revision 1.28  1992/08/26  21:28:42  ty7777
 * Testing RCS checking in.
 **/
/*$Header: /home/npggw/tom/src/view25d/RCS/portal.c,v 2.38 1995/10/26 19:06:34 ty7777 Exp tom $*/

#ifndef lint
static char     sccsid[] = "@(#)portal.c	10/22/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char     rcsid[] = "$Header: /home/npggw/tom/src/view25d/RCS/portal.c,v 2.38 1995/10/26 19:06:34 ty7777 Exp tom $";

/*_________________________________________________________________________________
	File Name:
		portal.c

	Description:
		Support functions for program ``view25d''. File includes
		manipulation functions for Portals, and AtlasSlices. Also,
		the heart of the slice display functions and data structure
		update routines reside here.
	Authors:
		Tom Yang and M. Todd Gamble.
		02/01/91

	Modifications:
		Created a private function Transformation to transform
		regions of PET images.
		Tom Yang (03/18/1991)

		Rewrite public function GetAtlasSliceData to read in pet
		images and region files.
		Tom Yang (03/20/1991)

        	Rewrite private functions MakePortal and UpdatePortals
		so we can display pet images and regions.
		Tom Yang (03/21/1991)

		Created a public function ClearPortals to clear the portals
		drawn on screen.
		Tom Yang (05/01/1991)

		Created private function ResizeSlice and ResizePortals for
		resizing images and displaying negative PET images.
		Tom Yang (05/06/1991)

		Created a public function DrawPortal to draw portals on
		screen.
		Tom Yang (05/07/1991)

	Pending:
		All comments which include the string "FIX" have information
		on current limitation, bugs, or noted problems.

______________________________________________________________________________*/

#include <time.h>
#include <math.h>
#include "portal.h"
#include <unistd.h>
#include <petutil/matrix7.h>
#include <petutil/winutil.h>
#include <petutil/ifh.h>
#include <petutil/ANALYZE.h>
#include <petutil/AIR.h>
#include <Xm/Label.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>
#include <Xm/SelectioB.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#define	SMOOTH_LENGTH	10



PRIVATE void    BuildPortals ();
PRIVATE SCALE_HEADER *BuildQueue ();
PRIVATE void    ClearRegionSets ();
PRIVATE int     DisplayAF3DPoints ();
PRIVATE int     DrawGammaZData ();
PRIVATE int     DrawAsciiGammaZData ();
PRIVATE void    DrawOutlinedRegion ();
PRIVATE void    DrawPortal ();
PRIVATE void    FormatScreen ();
PRIVATE int     GammaZPointCmp ();
PRIVATE float   GetCoordinate ();
PRIVATE SCALE_HEADER *GetCurrentHeader ();
PRIVATE short   GetDesiredFirst ();
PRIVATE void    GetFileStatus ();
PUBLIC GC       GetXorGC ();
PRIVATE void    GetPetCenter ();
PRIVATE void    GetPetDimension ();
PRIVATE int     InputInterfileData ();
PRIVATE void    InputImageData ();
PRIVATE int     InputRegionData ();
PRIVATE int     ReadRegion ();
PRIVATE GammaZData *MakeAsciiGammaZData ();
PRIVATE Portal *MakePortal ();
PRIVATE void    ResizePortals ();
PRIVATE SliceData *ResizeSlice ();



/*______________________________________________________________________________
	Procedure:
		SetSelectData
	Parameters:
		w:		Widget, widget where callback was triggered.
		status_flag:	BOOLEAN, either on or off.
	Description:
		Set the toggle button to the specified state.
		Author: Tom Yang
		(05/21/91)
_____________________________________________________________________________*/

PUBLIC void     SetSelectData (w, status_flag)
	Widget          w;
	BOOLEAN         status_flag;
{
	if (status_flag != XmToggleButtonGetState (w)) {
		XmToggleButtonSetState (w, status_flag, FALSE);
	}
}



/*_____________________________________________________________________________
	Function:	GammaZPointCmp
	Arguments:	a: pointer to GammaZPoint
			b: pointer to GammaZPoint
	Description:	Compare two gammaz data points by their z value.
	Return Value:	-1 if z value of a less than z value of b
 			 0 if z value of a equals z value of b
			 1 if z value of a greater than z value of b
_____________________________________________________________________________*/

PRIVATE int     GammaZPointCmp (a, b)
	GammaZPoint    *a, *b;
{
	if (a->z < b->z)
		return (-1);
	if (a->z > b->z)
		return (1);
	return (0);
}




/*______________________________________________________________________________
	Procedure:
		GetCoordinate
	Parameters:
		point:		FLOAT_POINT3D.
		atlas_orient:	atlas orientation.
	Description:
		Get the coordinate of a 3d floating point in the current active
		atlas orientation.
		Author: Tom Yang
		(07/17/92)
_____________________________________________________________________________*/

PRIVATE float   GetCoordinate (point, atlas_orient)
	FLOAT_POINT3D   point;
	char            atlas_orient;
{
	if (atlas_orient == TRANSVERSE)
		return point.z;
	else if (atlas_orient == CORONAL)
		return point.y;
	else
		return point.x;
}



/*___________________________________________________________________________
	Function:	DrawGammaZData

	Arguments:	fd: Unix file descriptor, file to read from
			c: character read to test condition of file

	Description:	Allocate, read, and display a set of gammaz data
			consisting of the header of point information from
			a Sybase application.

	Author:		Tom Yang
			Created January 12, 1995.

	Return Value:	SUCCEED: successful; FAIL: failed.
____________________________________________________________________________*/

PRIVATE int     DrawGammaZData (display, window, gc, fd, c, file_index, symbol_type,
	                    pixel_color, symbol_size, outline_width, atlas_orient, current_header,
				                draw_portal_data)
	Display        *display;
	Window          window;
	GC              gc;
	int             fd;
	char            c;
	int             file_index;
	char            symbol_type;
	Pixel           pixel_color;
	unsigned short  symbol_size;
	int             outline_width;
	char            atlas_orient;
	SCALE_HEADER   *current_header;
	DRAW_PORTAL_DATA *draw_portal_data;
{
	Arg             arglist[1];
	FLOAT_POINT3D   point;
	RawGammaZPoint *rawpoint;		 /* raw point read from input file */
	char           *description;		 /* pointer to AF3D description */
	char           *path;			 /* pointer to path of image file */
	char            s[2];			 /* temp var to use test char and */
	double          coordinate;
	int             af3d_id;		 /* AF3D id */
	int             i;			 /* counter */
	int             num_points;		 /* number of points in dataset */
	int             slice_xyz;
	int             sx;
	int             sy;
	int             sz;
	int             tempx;
	int             tempy;
	long            bytes;
	long            current_bytes;
	short           desc_len;		 /* length of AF3D description */
	short           path_len;		 /* length of path string */
	short           x;
	short           y;
	static char    *function = "DrawGammaZData";	/* read path len var from file */

	/*
	 * Keep the test character and read the rest of the path_len var.
	 */
	s[0] = c;
	pkg_read (fd, &s[1], 1, function, "s[1]");
	memcpy ((char *) &path_len, s, 2);

	current_bytes = lseek (fd, 0L, 1);
	bytes = lseek (fd, 0L, 2);
	if (path_len <= 0 || path_len > bytes - sizeof (RawGammaZPoint)) {
		XtManageChild (errorMessageBox);
		return FAIL;
	}
	lseek (fd, current_bytes, 0);

	/*
	 * Allocate memory for the path and read it.
	 */
	path = (char *) pkg_malloc (path_len, function, "path");
	pkg_read (fd, path, path_len, function, "path");
	free (path);

	/*
	 * Get the length of the description string.
	 */
	pkg_read (fd, &desc_len, sizeof (desc_len), function, "desc_len");

	/*
	 * Allocate memory for the description and read it.
	 */
	description = (char *) pkg_malloc (desc_len, function, "description");
	pkg_read (fd, description, desc_len, function, "description");
	free (description);

	/*
	 * Read the af3d id.
	 */
	pkg_read (fd, &af3d_id, sizeof (af3d_id), function, "af3d_id");

	/*
	 * Read the number of points.
	 */
	pkg_read (fd, &num_points, sizeof (num_points), function, "num_points");

	/*
	 * Allocate a buffer for point reading.
	 */
	rawpoint = (RawGammaZPoint *) pkg_malloc (sizeof (RawGammaZPoint),
						  function, "rawpoint");
	x = draw_portal_data->portal->x;
	y = draw_portal_data->portal->y;
	slice_xyz = draw_portal_data->portal->slice->slice_xyz;
	if (num_points > 0) {
		XSetForeground (display, gc, pixel_color);
		XSetLineAttributes (display, gc, pkg_min (symbol_size / 2,
						   outline_width), LineSolid, CapButt, JoinMiter);
	}

	/*
	 * Read the gammaz points.
	 */
	for (i = 0; i < num_points; i++) {

		/*
		 * Read a raw point.
		 */
		pkg_read (fd, rawpoint, sizeof (RawGammaZPoint), function, "rawpoint");

		/*
		 * Dsiplay the point if it inside the current slice.
		 */
		point.x = -rawpoint->x;
		point.y = rawpoint->y;
		point.z = rawpoint->z;

		coordinate = GetCoordinate (point, atlas_orient);
		if (*draw_portal_data->all_planes ||
		    ((coordinate >= slice_xyz - draw_portal_data->spacing) &&
		     (coordinate < slice_xyz + draw_portal_data->spacing))) {
			draw_portal_data->symbol_displayed[file_index] = TRUE;

			/*
			 * Reverse coordination orientation and scale.
			 */
			sx = ROUND ((point.x * 1.0) / current_header->pixel_size);
			sy = ROUND ((point.y * -1.0) / current_header->pixel_size);
			sz = ROUND ((point.z * -1.0) / current_header->pixel_size);
			if (atlas_orient == TRANSVERSE) {
				tempx = sx;
				tempy = sy;
			} else if (atlas_orient == CORONAL) {
				tempx = sx;
				tempy = sz;
			} else if (atlas_orient == FACING_LEFT) {
				tempx = sy;
				tempy = sz;
			} else {
				tempx = -sy;
				tempy = sz;
			}

			tempx += x + current_header->centerx - symbol_size / 2;
			tempy += y + current_header->centery - symbol_size / 2;

			DrawSymbol (display, window, gc, tempx, tempy,
				    symbol_size, symbol_size, symbol_type);
		}
	}

	/*
	 * Free the raw point buffer and return to the default line width.
	 */
	free (rawpoint);
	if (num_points > 0)
		XSetLineAttributes (display, gc, 1, LineSolid, CapButt, JoinMiter);

	return SUCCEED;
}



/*___________________________________________________________________________
	Function:	DrawAsciiGammaZData

	Arguments:

	Description:	Allocate, read, and display a set of gammaz data
			created from by an ascii editor.

	Author:		Tom Yang
			Created January 12, 1995.

	Return Value:	SUCCEED: successful; FAIL: failed.
____________________________________________________________________________*/

PRIVATE int     DrawAsciiGammaZData (display, window, gc, filename, file_index, symbol_type,
	                    pixel_color, symbol_size, outline_width, atlas_orient, current_header,
				                     draw_portal_data)
	Display        *display;
	Window          window;
	GC              gc;
	char           *filename;
	int             file_index;
	char            symbol_type;
	Pixel           pixel_color;
	unsigned short  symbol_size;
	int             outline_width;
	char            atlas_orient;
	SCALE_HEADER   *current_header;
	DRAW_PORTAL_DATA *draw_portal_data;
{
	FILE           *fp;
	FLOAT_POINT3D   point;
	char            line[MAXLINE];
	double          coordinate;
	float           x;
	float           y;
	float           z;
	int             slice_xyz;
	int             sx;
	int             sy;
	int             sz;
	int             tempx;
	int             tempy;
	short           xstart;
	short           ystart;
	static char    *function = "DrawAsciGammaZData";

	fp = fopen (filename, "r");
	if (fp == (FILE *) NULL) {
		pkg_message (PKG_ERROR, "view25d", function, "fopen",
			     "Can't open %s\n", filename);
		return FAIL;
	}
	xstart = draw_portal_data->portal->x;
	ystart = draw_portal_data->portal->y;
	slice_xyz = draw_portal_data->portal->slice->slice_xyz;
	XSetForeground (display, gc, pixel_color);
	XSetLineAttributes (display, gc, pkg_min (symbol_size / 2,
						  outline_width), LineSolid, CapButt, JoinMiter);

	while (fgets (line, MAXLINE, fp) != NULL) {
		if (sscanf (line, "%f %f %f", &x, &y, &z) == 3) {
			point.x = x;
			point.y = y;
			point.z = z;

			coordinate = GetCoordinate (point, atlas_orient);
			if (*draw_portal_data->all_planes ||
			    ((coordinate >= slice_xyz - draw_portal_data->spacing) &&
			     (coordinate < slice_xyz + draw_portal_data->spacing))) {
				draw_portal_data->symbol_displayed[file_index] = TRUE;

				/*
				 * Reverse coordination orientation and scale.
				 */
				sx = ROUND ((point.x * 1.0) / current_header->pixel_size);
				sy = ROUND ((point.y * -1.0) / current_header->pixel_size);
				sz = ROUND ((point.z * -1.0) / current_header->pixel_size);
				if (atlas_orient == TRANSVERSE) {
					tempx = sx;
					tempy = sy;
				} else if (atlas_orient == CORONAL) {
					tempx = sx;
					tempy = sz;
				} else if (atlas_orient == FACING_LEFT) {
					tempx = sy;
					tempy = sz;
				} else {
					tempx = -sy;
					tempy = sz;
				}

				tempx += xstart + current_header->centerx - symbol_size / 2;
				tempy += ystart + current_header->centery - symbol_size / 2;

				DrawSymbol (display, window, gc, tempx, tempy,
					    symbol_size, symbol_size, symbol_type);
			}
		}
	}

	fclose (fp);

	return SUCCEED;
}



/*_______________________________________________________________________________
	Function:	void ReadAtlasHeader (char *atlas_filename, char *library_path)

	Argument:	atlas_filename: pointer to atlas image filename.

	Description:	Read atlas header and set the rAtlasHeader structure
			of type ATLAS_HEADER.

	Return Value:	none.

	Author:		Tom Yang, March, 1992.

			(02/03/1992)
_______________________________________________________________________________*/
PUBLIC void     ReadAtlasHeader (atlas_filename, library_path)
	char           *atlas_filename;
	char           *library_path;
{
	ATLAS_HEADER   *atlas_header = &rAtlasHeader;
	BOOLEAN        *talairach_flag = &bTalairachFlag;
	FILE           *atlasfp;		 /* file pointer to atlas header */
	char            atlasz[MAXLINE];	 /* string to hold xyz value */
	char            label[MAXLINE];		 /* text label in header file */
	char            szBuffer[MAXLINE];	 /* buffer string */
	int             i;
	int             index;
	int             pet_slice_num;
	int             z_value;

	/*
	 * Open the header file.
	 */
	atlasfp = fopen (atlas_filename, "r");
	if (atlasfp == (FILE *) NULL) {
		pkg_message (PKG_ERROR, "view25d", "ReadAtlasHeader", "fopen",
			     "Can't open %s\n", atlas_filename);
		exit (FAIL);
	}

	/*
	 * Read header information.
	 */
	if (fgets (label, MAXLINE, atlasfp) == NULL) {
		pkg_message (PKG_ERROR, "view25d", "ReadAtlasHeader", "fgets",
			     "File %s is empty.\n", atlas_filename);
		return;
	}

	/*
	 * Read Header for Scale 0.25.
	 */
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_025_header.width));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_025_header.height));
	fscanf (atlasfp, "%s %f", label, &(atlas_header->scale_025_header.pixel_size));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_025_header.centerx));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_025_header.centery));
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_025_header.image_name, library_path);
	strcat (atlas_header->scale_025_header.image_name, szBuffer);
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_025_header.mask_name, library_path);
	strcat (atlas_header->scale_025_header.mask_name, szBuffer);

	fscanf (atlasfp, "%s %s", label, szBuffer);

	/*
	 * Read Header for Scale 0.50.
	 */
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_050_header.width));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_050_header.height));
	fscanf (atlasfp, "%s %f", label, &(atlas_header->scale_050_header.pixel_size));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_050_header.centerx));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_050_header.centery));
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_050_header.image_name, library_path);
	strcat (atlas_header->scale_050_header.image_name, szBuffer);
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_050_header.mask_name, library_path);
	strcat (atlas_header->scale_050_header.mask_name, szBuffer);

	fscanf (atlasfp, "%s %s", label, szBuffer);

	/*
	 * Read Header for Scale 1.00.
	 */
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_100_header.width));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_100_header.height));
	fscanf (atlasfp, "%s %f", label, &(atlas_header->scale_100_header.pixel_size));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_100_header.centerx));
	fscanf (atlasfp, "%s %d", label, &(atlas_header->scale_100_header.centery));
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_100_header.image_name, library_path);
	strcat (atlas_header->scale_100_header.image_name, szBuffer);
	fscanf (atlasfp, "%s %s", label, szBuffer);
	strcpy (atlas_header->scale_100_header.mask_name, library_path);
	strcat (atlas_header->scale_100_header.mask_name, szBuffer);

	fscanf (atlasfp, "%s %s %s", label, szBuffer, atlasz);

	/*
	 * Read Index Table.
	 */
	i = 0;
	while (fscanf (atlasfp, "%d %d %d", &pet_slice_num, &index, &z_value) != EOF) {
		atlas_header->index_table[i++] = index;
	}
	atlas_header->scale_025_header.any_scale_flag = FALSE;
	atlas_header->scale_050_header.any_scale_flag = FALSE;
	atlas_header->scale_100_header.any_scale_flag = FALSE;

	if (strstr (atlas_filename, "Talairach") == NULL)
		*talairach_flag = FALSE;
	else
		*talairach_flag = TRUE;

	fclose (atlasfp);
}



/*_______________________________________________________________________________
	Function:	GetCurrentHeader
	Arguments:
	Description:	Get the active header corresponding to the current scale.

	Return Value:	pointer to structure SCALE_HEADER..
	Author:		Tom (Tongzeng) Yang
			(02/03/1992)
_______________________________________________________________________________*/

PRIVATE SCALE_HEADER * GetCurrentHeader (current_mag, atlas_header)
	float          *current_mag;
	ATLAS_HEADER   *atlas_header;
{
	SCALE_HEADER   *current_header;

	if (pkg_abs (*current_mag - 0.25) < ERROR_BOUND)
		current_header = &atlas_header->scale_025_header;
	else if (pkg_abs (*current_mag - 0.50) < ERROR_BOUND)
		current_header = &atlas_header->scale_050_header;
	else if (pkg_abs (*current_mag - 1.00) < ERROR_BOUND)
		current_header = &atlas_header->scale_100_header;
	else {
		current_header = &rAnyScaleHeader;
		current_header->width
			= ROUND (atlas_header->scale_100_header.width * *current_mag);
		current_header->height
			= ROUND (atlas_header->scale_100_header.height * *current_mag);
		current_header->pixel_size
			= atlas_header->scale_100_header.pixel_size / *current_mag;
		current_header->centerx = (current_header->width - 1) / 2;
		current_header->centery = (current_header->height - 1) / 2;
		current_header->any_scale_flag = TRUE;
		strcpy (current_header->image_name,
			atlas_header->scale_100_header.image_name);
		current_header->any_atlas_width
			= atlas_header->scale_100_header.width;
		current_header->any_atlas_height
			= atlas_header->scale_100_header.height;
	}

	return current_header;
}



/*_______________________________________________________________________________
	Function:	BuildQueue
	Arguments:
			atlas_list1:	linked list for atlas slices in image 1.
			atlas_list2:	linked list for atlas slices in image 2.
			first_slice:	first slice specified by user.
			last_slice:	last slice specified by user.
			spacing:	spacing specified by user.
	Description:	Build linked list for atlas slices.

	Return Value:	none.
	Author:		Tom (Tongzeng) Yang
			(02/03/1992)
_______________________________________________________________________________*/

PRIVATE SCALE_HEADER * BuildQueue (first_slice, last_slice, spacing, atlas_list1, atlas_list2, current_mag)
	short           first_slice;
	short           last_slice;
	short           spacing;
	llist           atlas_list1;
	llist           atlas_list2;
	float          *current_mag;
{
	ATLAS_HEADER   *atlas_header = &rAtlasHeader;
	SCALE_HEADER   *current_header;
	SliceData      *slice1;
	SliceData      *slice2;
	char            atlas_orient = cAtlasOrientation;
	char           *image1_filename = szImage1FileName;
	float           voxel_depth;
	float           voxel_height;
	float           voxel_width;
	int             default_max_index_num;
	int             desired_slice;
	int             image_type;
	int             index_image;
	int             max_index_num;
	int             min_index_num;
	short           height;
	short           num_slices;
	short           width;
	struct Matval   matval;

	/*
	 * Set comparison function for llinsert.
	 */
	llcmp = AtlasSliceCmp;

	/*
	 * Build the atlas list.
	 */
	desired_slice = first_slice;
	current_header = GetCurrentHeader (current_mag, atlas_header);

	if (atlas_orient == TRANSVERSE) {
		default_max_index_num = MAX_TRANS_ATLAS_NUM - 1;
	} else if (atlas_orient == CORONAL) {
		default_max_index_num = MAX_ATLAS_NUM - 1;
	} else {
		default_max_index_num = MAX_ATLAS_NUM - 1;
	}

	if (strlen (image1_filename) > 0) {
		matval.frame = 1;
		matval.plane = 1;
		matval.gate = 1;
		matval.data = 0;
		matval.bed = 0;

		if (getimagedim (image1_filename, matval, &image_type, &width, &height,
				 &num_slices, &voxel_width, &voxel_height, &voxel_depth) == FAIL)
			return;

		if (atlas_orient == TRANSVERSE) {
			max_index_num = num_slices - 1;
		} else if (atlas_orient == CORONAL) {
			max_index_num = height - 1;
		} else {
			max_index_num = width - 1;
		}
	} else {
		max_index_num = default_max_index_num;
	}
	min_index_num = 0;

	while (desired_slice <= last_slice) {

		/*
		 * Allocate memory for a slice.
		 */
		slice1 = (SliceData *) pkg_malloc (sizeof (SliceData),
						   "BuildQueue", "slice1");
		slice2 = (SliceData *) pkg_malloc (sizeof (SliceData),
						   "BuildQueue", "slice2");

		slice1->slice_xyz = GetSliceCoord (desired_slice, atlas_orient, zorigin_image1);
		slice2->slice_xyz = slice1->slice_xyz;
		if (atlas_orient == TRANSVERSE) {
			index_image = desired_slice - 1 + (DEFAULT_ZORIGIN - zorigin_image1);

			if ((index_image < min_index_num && desired_slice >= 1)
			|| (index_image > max_index_num && desired_slice <= MAX_TRANS_ATLAS_NUM)) {
				index_image = max_index_num;
			}
		} else {
			index_image = desired_slice - 1;
		}

		if (index_image < min_index_num || index_image > max_index_num) {
			fprintf (stderr,
				 "The image index %d is outside the range [%d, %d].\n",
				 index_image, min_index_num, max_index_num);
			exit (0);
		}
		if (index_image < default_max_index_num) {
			slice1->atlas_index = atlas_header->index_table[index_image];
		} else {
			slice1->atlas_index = -1;
		}
		slice2->atlas_index = slice1->atlas_index;

		slice1->pet_exist = FALSE;
		slice2->pet_exist = FALSE;
		llinsert (atlas_list1, slice1);
		llinsert (atlas_list2, slice2);
		desired_slice += abs (spacing);
	}

	return current_header;
}



/*_______________________________________________________________________________
	Function:	InputImageData
	Arguments:	imgfile: pointer to pet image file
	Description:	Read pet images into atlas list.

	Return Value:	void.
	Author:		Tom Yang
			(06/12/1991)
_______________________________________________________________________________*/

PRIVATE void    InputImageData (current_header, imgfile, atlas_list, first_slice, last_slice,
                   spacing, image_zorigin, apply_mask, auto_min_image_value, auto_max_image_value)
	SCALE_HEADER   *current_header;
	char           *imgfile;
	llist           atlas_list;
	int             first_slice;
	int             last_slice;
	int             spacing;
	int             image_zorigin;
	BOOLEAN         apply_mask;
	short          *auto_min_image_value;
	short          *auto_max_image_value;
{
	BOOLEAN         encoded;
	BOOLEAN         err_value = SUCCEED;
	BOOLEAN         mask_encoded;
	BOOLEAN         minmax_changed = FALSE;
	BOOLEAN         pettvi_14 = FALSE;
	BOOLEAN         sagittal_flag;
	COMMENT_INFO    comment_info_data;
	HorizontalSlice *horizontal_slice;
	MatrixFile     *fptr;
	SliceData      *slice;			 /* slice pointer to new slice */
	XPoint          ptBottomRight;
	XPoint          ptUpperLeft;
	char            atlas_orient = cAtlasOrientation;
	char           *image_mask_file_name = szImageMaskFileName;
	float           voxel_depth;
	float           voxel_width;
	int             i;			 /* counter */
	int             imgfd;
	int             j;			 /* counter */
	int             mskfd;
	int             mask_pett_num;
	int             mask_zorigin;
	int             pett_num;
	int             rows;
	int             slice_height;
	int             slice_width;
	int             tempx;
	int             tempy;
	int             zorigin_offset;
	llist           slice3d_list;
	long            slice_dim;		 /* dimension of the slice image */
	short           global_max = -MAXSHORT;
	short           global_min = MAXSHORT;
	short          *header;
	short           height;
	short           mask_num_slices;
	short          *mask_slice_image;
	short           mask_slice_index;
	short           num_slices;
	short          *pet_slice_image;
	short           slice_index;
	short           temp_pet_value;
	short           width;
	static char    *function = "InputImageData";
	struct Matval   matval;

	matval.frame = 1;
	matval.plane = 1;
	matval.gate = 1;
	matval.data = 0;
	matval.bed = 0;

	/*
	 * Find the encoding, number of slices, the pett number.
	 */
	header = (short *) calloc (HEADER_SIZE / sizeof (short), sizeof (short));
	if (header == (short *) NULL) {
		pkg_message (PKG_ERROR, "view25d", function, "calloc",
			     "Error: Can't allocate header.\n");
	}
	imgfd = open (imgfile, O_RDONLY, 0);
	if (imgfd == -1) {
		fprintf (stderr, "Can't open image file %s\n", imgfile);
		return;
	}
	pett_num = getimage_type (imgfd, imgfile, &encoded, &num_slices);

	if (pett_num == ERROR_SCANNER)
		return;
	close (imgfd);

	if ((pett_num == PETT6_SCANNER) && (num_slices <= PETT6_14))
		pettvi_14 = TRUE;

	if (pett_num == INTERFILE) {

		InputInterfileData (current_header, imgfile, atlas_list,
				    first_slice, last_slice, spacing, apply_mask,
				    image_mask_file_name, auto_min_image_value,
				    auto_max_image_value);
		return;
	}

	/*
	 * Find the encoding, number of slices, the pett number, and the zorigin for the mask file
	 * if it exists.
	 */
	if (apply_mask) {
		mskfd = open (image_mask_file_name, O_RDONLY, 0);
		if (mskfd == -1) {
			fprintf (stderr, "Can't open mask file%s\n", image_mask_file_name);
			return;
		}
		mask_pett_num = getimage_type (mskfd, image_mask_file_name,
					       &mask_encoded, &mask_num_slices);

		if (mask_pett_num == ERROR_SCANNER)
			return;
		close (mskfd);

		mask_zorigin = DEFAULT_ZORIGIN;
		if (mask_pett_num == ECAT_SCANNER) {
			fptr = OpenEcat (image_mask_file_name, &mask_num_slices,
					 &width, &height, &voxel_width, &voxel_depth, matval);
			if (fptr == (MatrixFile *) NULL) {
				fprintf (stderr, "Cannnot open mask image %s.\n", image_mask_file_name);
				return;
			}
			comment_info (fptr->mhptr->study_description, &comment_info_data);

			mask_zorigin = comment_info_data.zorigin;
			matrix_close (fptr);

			if (mask_zorigin == 0) {
				fprintf (stderr,
				     "Error: There is no z0 information in your mask image %s.\n",
					 image_mask_file_name);
				fprintf (stderr,
					 "       You can use the program setz0 to add the z0 slice to your image.\n");

				exit (FAIL);
			}
		}
	}

	/*
	 * Read the image and mask it if it is necessory.
	 */
	slice3d_list = llinit ();
	slice_dim = ENCODED_SLICE_DIM;
	zorigin_offset = mask_zorigin - image_zorigin;
	for (slice_index = 1; slice_index <= num_slices; slice_index++) {
		horizontal_slice = (HorizontalSlice *) pkg_malloc (sizeof (HorizontalSlice),
								   function, "horizontal_slice");
		horizontal_slice->slice_index = slice_index;
		horizontal_slice->slice_data = (short *) calloc (slice_dim, sizeof (short));
		imgfd = open (imgfile, O_RDONLY, 0);
		if (imgfd == -1)
			fprintf (stderr, "Can't open %d\n", imgfile);
		getimage (horizontal_slice->slice_data, header, imgfd, imgfile, slice_index,
			  encoded, num_slices, pett_num, &err_value);
		close (imgfd);

		if (apply_mask) {
			mskfd = open (image_mask_file_name, O_RDONLY, 0);
			if (mskfd == -1) {
				fprintf (stderr, "Can't open mask file %s\n", image_mask_file_name);
				return;
			}
			mask_slice_image = (short *) calloc (slice_dim, sizeof (short));
			mask_slice_index = slice_index + zorigin_offset;

			if (mask_slice_index <= mask_num_slices && mask_slice_index >= 1) {
				getimage (mask_slice_image, header, mskfd, image_mask_file_name,
					  mask_slice_index, mask_encoded, mask_num_slices,
					  mask_pett_num, &err_value);
				for (i = 0; i < slice_dim; i++) {
					if (mask_slice_image[i] <= 0)
						horizontal_slice->slice_data[i] = 0;
				}
			}
			close (mskfd);
			free (mask_slice_image);
		}
		lladd (slice3d_list, horizontal_slice);
	}

	if (llsize (atlas_list) > 0) {

		/*
		 * The coordinate system is right handed. The slice number and the x value either
		 * increase or decrase at the same time.
		 */
		if (atlas_orient == FACING_LEFT || atlas_orient == FACING_RIGHT) {
			sagittal_flag = TRUE;
			llhead (atlas_list);
		} else {
			sagittal_flag = FALSE;
			lltail (atlas_list);
		}
		slice_index = first_slice;

		GetPetDimension (atlas_orient, num_slices, &slice_width, &slice_height);
		slice_dim = slice_width * slice_height;

		do {

			/*
			 * Allocate and Zero the image array.
			 */
			pet_slice_image = (short *) calloc (slice_dim, sizeof (short));
			llhead (slice3d_list);
			slice = llretrieve (atlas_list);
			if (atlas_orient == TRANSVERSE) {
				if (slice_index > num_slices) {
					slice->pet_exist = FALSE;
					free (pet_slice_image);
					slice_index += abs (spacing);
					continue;
				}
				for (i = 1; i < slice_index; i++) {
					llnext (slice3d_list);
				}
				horizontal_slice = (HorizontalSlice *) llretrieve (slice3d_list);
				for (i = 0; i < slice_dim; i++) {
					pet_slice_image[i] = horizontal_slice->slice_data[i];
				}
			} else if (atlas_orient == CORONAL) {
				for (j = 0; j < slice_height; j++) {
					horizontal_slice
						= (HorizontalSlice *) llretrieve (slice3d_list);
					rows = j * slice_width;
					for (i = 0; i < slice_width; i++) {
						pet_slice_image[rows + i]
							= horizontal_slice->slice_data[(slice_index - 1)
								       * ENCODED_SLICE_WIDTH + i];
					}
					llnext (slice3d_list);
				}
			} else if (atlas_orient == FACING_LEFT) {
				for (j = 0; j < slice_height; j++) {
					horizontal_slice
						= (HorizontalSlice *) llretrieve (slice3d_list);
					rows = j * slice_width;
					for (i = 0; i < slice_width; i++) {
						pet_slice_image[rows + i]
							= horizontal_slice->slice_data[i
							 * ENCODED_SLICE_WIDTH + slice_index - 1];
					}
					llnext (slice3d_list);
				}
			} else {
				for (j = 0; j < slice_height; j++) {
					horizontal_slice
						= (HorizontalSlice *) llretrieve (slice3d_list);
					rows = j * slice_width;
					for (i = 0; i < slice_width; i++) {
						pet_slice_image[rows + (slice_width - 1 - i)]
							= horizontal_slice->slice_data[i
							 * ENCODED_SLICE_WIDTH + slice_index - 1];
					}
					llnext (slice3d_list);
				}
			}

			if (slice->pet_exist)
				XtFree (slice->pet_input);
			for (i = 0; i < slice_dim &&
			     pet_slice_image[i] == PET_BACK_VALUE; i++);
			if (i >= slice_dim) {
				slice->pet_exist = FALSE;
				free (pet_slice_image);
				slice_index += abs (spacing);
				continue;
			} else
				slice->pet_exist = TRUE;

			i /= slice_width;
			if (i >= slice_height) {
				i = slice_height - 1;
			}
			ptUpperLeft.y = i;

			for (i = slice_dim - 1; i >= 0 &&
			     pet_slice_image[i] == PET_BACK_VALUE; i--);
			i /= slice_width;
			if (i < 0)
				i = 0;
			ptBottomRight.y = i;

			tempx = slice_width - 1;
			tempy = 0;
			for (j = ptUpperLeft.y; j <= ptBottomRight.y; j++) {
				rows = j * slice_width;
				for (i = 0; i < slice_width &&
				     pet_slice_image[rows + i] == PET_BACK_VALUE; i++);
				if (i < tempx)
					tempx = i;

				for (i = slice_width - 1; i >= 0 &&
				     pet_slice_image[rows + i] == PET_BACK_VALUE; i--);
				if (i > tempy)
					tempy = i;
			}
			ptUpperLeft.x = tempx;
			ptBottomRight.x = tempy;

			slice->pet_xstart = ptUpperLeft.x;
			slice->pet_ystart = ptUpperLeft.y;
			slice->pet_width = (ptBottomRight.x - ptUpperLeft.x + 1);
			slice->pet_height = (ptBottomRight.y - ptUpperLeft.y + 1);
			if (slice->pet_exist && pettvi_14) {
				slice->rgn_pet_pixelwidth
					= (PETT6_PIXEL_SIZE * MMS_PER_CM)
					/ current_header->pixel_size;
				slice->rgn_pet_pixelheight
					= (PETT6_PIXEL_SIZE * MMS_PER_CM)
					/ current_header->pixel_size;
			} else {
				slice->rgn_pet_pixelwidth
					= RGN_PIXEL_WIDTH / current_header->pixel_size;
				slice->rgn_pet_pixelheight
					= RGN_PIXEL_HEIGHT / current_header->pixel_size;
			}

			if (atlas_orient != TRANSVERSE) {
				slice->pet_ystart += DEFAULT_ZORIGIN - image_zorigin;
			}
			slice->pet_input = (short *) pkg_malloc (slice->pet_width
							     * slice->pet_height * sizeof (short),
								 function, "slice->pet_input");

			tempx = 0;
			for (j = ptUpperLeft.y; j <= ptBottomRight.y; j++) {
				rows = j * slice_width;
				for (i = ptUpperLeft.x; i <= ptBottomRight.x; i++) {
					temp_pet_value = pet_slice_image[rows + i];
					slice->pet_input[tempx++] = temp_pet_value;

					if (temp_pet_value > global_max)
						global_max = temp_pet_value;
					if (temp_pet_value < global_min)
						global_min = temp_pet_value;
				}
			}
			minmax_changed = TRUE;
			free (pet_slice_image);
			slice_index += abs (spacing);
		}
		while (((!sagittal_flag && llprevious (atlas_list) != FAIL)
			|| (sagittal_flag && llnext (atlas_list) != FAIL))
		       && slice_index <= last_slice);

		if (minmax_changed) {
			*auto_min_image_value = global_min;
			*auto_max_image_value = global_max;
		}
	}
	free (header);

	/*
	 * Clear slice3d_list data.
	 */
	llhead (slice3d_list);
	while (llsize (slice3d_list) > 0) {
		horizontal_slice = (HorizontalSlice *) llretrieve (slice3d_list);
		XtFree (horizontal_slice->slice_data);
		XtFree (horizontal_slice);

		lldelete (slice3d_list);
	}
}



/*_______________________________________________________________________________
	Function:	InputInterfileData
	Arguments:	imgfile: pointer to pet image file
	Description:	Read pet images into atlas list.

	Return Value:	SUCCEED: successful; FAIL: fail.
	Author:		Tom Yang
			(05/19/1995)
_______________________________________________________________________________*/

PRIVATE int     InputInterfileData (current_header, imgfile, atlas_list, first_slice, last_slice,
	                 spacing, apply_mask, mskfile, auto_min_image_value, auto_max_image_value)
	SCALE_HEADER   *current_header;
	char           *imgfile;
	llist           atlas_list;
	int             first_slice;
	int             last_slice;
	BOOLEAN         apply_mask;
	char           *mskfile;
	int             spacing;
	short          *auto_min_image_value;
	short          *auto_max_image_value;
{
	BOOLEAN         minmax_changed = FALSE;
	BOOLEAN         sagittal_flag;
	IFH             ifhdr;
	IFH             ifhdr_mask;
	SliceData      *slice;			 /* slice pointer to new slice */
	XPoint          ptBottomRight;
	XPoint          ptUpperLeft;
	char            atlas_orient = cAtlasOrientation;
	char           *extension;
	char            ifhdr_file[MAXLINE];
	char            ifhdr_mskfile[MAXLINE];
	char            ifmskfile[MAXLINE];
	char            imgroot[MAXLINE];
	char           *lines[IFH_MAXLEN];
	float          *mask_slice_data;
	float           rgn_mask_pixelheight;
	float           rgn_mask_pixelwidth;
	float           rgn_pet_pixelheight;
	float           rgn_pet_pixelwidth;
	float          *slice_data;
	int             i;			 /* counter */
	int             j;			 /* counter */
	int             mask_slice_height;
	int             mask_slice_width;
	int             nlines;
	int             offset;
	int             rows;
	int             slice_height;
	int             slice_width;
	int             tempx;
	int             tempy;
	long            mask_slice_dim;		 /* dimension of the slice image */
	long            slice_dim;		 /* dimension of the slice image */
	short           global_max = -MAXSHORT;
	short           global_min = MAXSHORT;
	short           height;
	short           mask_index;
	short           num_slices;
	short          *pet_slice_image;
	short           slice_index;
	short           temp_pet_value;
	short           width;
	static char    *function = "InputInterfileData";

	/*
	 * Create the interfile header file.
	 */
	memset (imgroot, 0, MAXLINE);
	extension = strrchr (imgfile, '.');
	if (extension != NULL)
		strncpy (imgroot, imgfile, strlen (imgfile) - strlen (extension));
	else
		strcpy (imgroot, imgfile);
	strcpy (ifhdr_file, imgroot);
	strcat (ifhdr_file, IFH_EXT);

	if (readifhdr (ifhdr_file, FALSE, IFH_MAXLEN, &nlines, lines) == FAIL
	    || getifhdr (ifhdr_file, FALSE, nlines, lines, &ifhdr) == FAIL) {
		strcpy (ifhdr_file, imgroot);
		strcat (ifhdr_file, HDR_EXT);

		if (hdr2ifh (ifhdr_file, &ifhdr) == FAIL)
			return;
	}
	if (atlas_orient == TRANSVERSE) {
		i = 0;
		j = 1;
	} else if (atlas_orient == CORONAL) {
		i = 0;
		j = 2;
	} else if (atlas_orient == FACING_LEFT || atlas_orient == FACING_RIGHT) {
		i = 1;
		j = 2;
	}
	slice_dim = ifhdr.matrix_size[i] * ifhdr.matrix_size[j];
	slice_width = ifhdr.matrix_size[i];
	slice_height = ifhdr.matrix_size[j];
	rgn_pet_pixelwidth = ifhdr.scaling_factor[i] / current_header->pixel_size;
	rgn_pet_pixelheight = ifhdr.scaling_factor[j] / current_header->pixel_size;

	if (apply_mask) {
		memset (imgroot, 0, MAXLINE);
		extension = strrchr (mskfile, '.');
		if (extension != NULL)
			strncpy (imgroot, mskfile, strlen (mskfile) - strlen (extension));
		else
			strcpy (imgroot, mskfile);
		strcat (imgroot, "_ifimg");
		strcpy (ifhdr_mskfile, imgroot);
		strcat (ifhdr_mskfile, IFH_EXT);

		strcpy (ifmskfile, imgroot);
		strcat (ifmskfile, ".mask");

		if (readifhdr (ifhdr_mskfile, FALSE, IFH_MAXLEN, &nlines, lines) == FAIL
		    || getifhdr (ifhdr_mskfile, FALSE, nlines, lines, &ifhdr_mask) == FAIL) {
			strcpy (ifhdr_mskfile, imgroot);
			strcat (ifhdr_mskfile, HDR_EXT);

			if (hdr2ifh (ifhdr_mskfile, &ifhdr_mask) == FAIL)
				return;
		}
		mask_slice_dim = ifhdr_mask.matrix_size[i] * ifhdr_mask.matrix_size[j];
		mask_slice_width = ifhdr_mask.matrix_size[i];
		mask_slice_height = ifhdr_mask.matrix_size[j];
		if (pkg_abs (ifhdr.scaling_factor[i] - ifhdr_mask.scaling_factor[i]) >= ERROR_BOUND
		    || pkg_abs (ifhdr.scaling_factor[j] - ifhdr_mask.scaling_factor[j]) >= ERROR_BOUND) {
			fprintf (stderr, "Error: the image %s and the mask %s have different voxel size\n",
				 imgfile, ifmskfile);
			return;
		}
	}

	/*
	 * Read the image.
	 */
	if (llsize (atlas_list) > 0) {

		/*
		 * The coordinate system is right handed. The slice number and the x value either
		 * increase or decrase at the same time.
		 */
		if (atlas_orient == FACING_LEFT || atlas_orient == FACING_RIGHT) {
			sagittal_flag = TRUE;
			llhead (atlas_list);
		} else {
			sagittal_flag = FALSE;
			lltail (atlas_list);
		}
		slice_index = first_slice;

		do {
			slice = llretrieve (atlas_list);

			/*
			 * Allocate and Zero the image array.
			 */
			slice_data = (float *) calloc (slice_dim, sizeof (float));
			if (readifimg (imgfile, atlas_orient, slice_index, slice_dim,
				       ifhdr, slice_data) == FAIL) {
				slice->pet_exist = FALSE;
				free (slice_data);
				slice_index += abs (spacing);
				continue;
			}
			pet_slice_image = (short *) calloc (slice_dim, sizeof (short));
			for (i = 0; i < slice_dim; i++) {
				pet_slice_image[i] = ROUND (slice_data[i]);
			}

			free (slice_data);

			if (apply_mask) {
				mask_slice_data = (float *) calloc (mask_slice_dim, sizeof (float));
				offset = ROUND (ifhdr_mask.atlas_origin[2] - ifhdr.atlas_origin[2]);

				if (atlas_orient == TRANSVERSE) {
					mask_index = slice_index + offset;
				} else
					mask_index = slice_index;
				if (readifimg (ifmskfile, atlas_orient, mask_index, mask_slice_dim,
					       ifhdr_mask, mask_slice_data) == FAIL) {
					free (mask_slice_data);
					slice_index += abs (spacing);
					continue;
				}
				if (atlas_orient == TRANSVERSE) {
					for (i = 0; i < slice_dim; i++) {
						if (pkg_abs (mask_slice_data[i]) < ERROR_BOUND)
							pet_slice_image[i] = 0;
					}
				} else {
					for (j = 0; j < slice_height; j++) {
						rows = j * slice_width;
						for (i = 0; i < slice_width; i++) {
							if (pkg_abs (mask_slice_data[(j + offset)
								* slice_width + i]) < ERROR_BOUND)
								pet_slice_image[rows + i] = 0;
						}
					}
				}

				free (mask_slice_data);
			}
			if (slice->pet_exist)
				XtFree (slice->pet_input);
			for (i = 0; i < slice_dim &&
			     pet_slice_image[i] == PET_BACK_VALUE; i++);
			if (i >= slice_dim) {
				slice->pet_exist = FALSE;
				free (pet_slice_image);
				slice_index += abs (spacing);
				continue;
			} else
				slice->pet_exist = TRUE;

			i /= slice_width;
			if (i >= slice_height) {
				i = slice_height - 1;
			}
			ptUpperLeft.y = i;

			for (i = slice_dim - 1; i >= 0 &&
			     pet_slice_image[i] == PET_BACK_VALUE; i--);
			i /= slice_width;
			if (i < 0)
				i = 0;
			ptBottomRight.y = i;

			tempx = slice_width - 1;
			tempy = 0;
			for (j = ptUpperLeft.y; j <= ptBottomRight.y; j++) {
				rows = j * slice_width;
				for (i = 0; i < slice_width &&
				     pet_slice_image[rows + i] == PET_BACK_VALUE; i++);
				if (i < tempx)
					tempx = i;

				for (i = slice_width - 1; i >= 0 &&
				     pet_slice_image[rows + i] == PET_BACK_VALUE; i--);
				if (i > tempy)
					tempy = i;
			}
			ptUpperLeft.x = tempx;
			ptBottomRight.x = tempy;

			slice->pet_xstart = ptUpperLeft.x;
			slice->pet_ystart = ptUpperLeft.y;
			slice->pet_width = (ptBottomRight.x - ptUpperLeft.x + 1);
			slice->pet_height = (ptBottomRight.y - ptUpperLeft.y + 1);
			slice->rgn_pet_pixelwidth = rgn_pet_pixelwidth;
			slice->rgn_pet_pixelheight = rgn_pet_pixelheight;

			slice->pet_input = (short *) pkg_malloc (slice->pet_width
							     * slice->pet_height * sizeof (short),
								 function, "slice->pet_input");

			tempx = 0;
			for (j = ptUpperLeft.y; j <= ptBottomRight.y; j++) {
				rows = j * slice_width;
				for (i = ptUpperLeft.x; i <= ptBottomRight.x; i++) {
					temp_pet_value = pet_slice_image[rows + i];
					slice->pet_input[tempx++] = temp_pet_value;

					if (temp_pet_value > global_max)
						global_max = temp_pet_value;
					if (temp_pet_value < global_min)
						global_min = temp_pet_value;
				}
			}
			minmax_changed = TRUE;
			free (pet_slice_image);
			slice_index += abs (spacing);
		}
		while (((!sagittal_flag && llprevious (atlas_list) != FAIL)
			|| (sagittal_flag && llnext (atlas_list) != FAIL))
		       && slice_index <= last_slice);

		if (minmax_changed) {
			*auto_min_image_value = global_min;
			*auto_max_image_value = global_max;
		}
	}
}



/*_______________________________________________________________________________
	Function:	InputRegionData
	Arguments:
	Description:	Read region data set into region_sets list.

	Return Value:	SUCCEED: successful; FAIL: failed.
	Author:		Tom Yang
			(05/22/1992)
_______________________________________________________________________________*/

PRIVATE int     InputRegionData (filenames, display_region, filling_type, region_color, region_sets)
	char            filenames[][MAXLINE];
	BOOLEAN        *display_region;
	char           *filling_type;
	Pixel          *region_color;
	llist           region_sets;
{
	int             fd;
	int             i;
	BOOLEAN         talairach_region_flag = bTalairachRegionFlag;

	/*
	 * Clear old data sets.
	 */
	ClearRegionSets (region_sets);

	for (i = 0; i < MAX_REGIONSET_NUM; i++) {
		if (*(display_region + i)) {
			if (strlen (filenames[i]) == 0 ||
			    (fd = open (filenames[i], O_RDONLY, 0)) == -1) {
				continue;
			}
			close (fd);

			/*
			 * Read data from file.
			 */
			if (ReadRegion (filenames[i], filling_type[i], region_color[i],
					region_sets, talairach_region_flag) == FAIL) {
				fprintf (stderr, "Error in ReadRegion for file %s\n", filenames[i]);
			}
		}
	}
}



/*_______________________________________________________________________________
	Function:	ReadRegion
	Arguments:	rgn_filename:	region filename.
			filling_type:	region filling type.
			region_color:	region color.
			region_sets:	region linked list.
	Description:	Read region slices into atlas list.

	Return Value:	int.
			SUCCEED: successful; FAIL: failed.
	Author:		Tom Yang
			(06/10/1991)
_______________________________________________________________________________*/

PRIVATE int     ReadRegion (rgn_filename, filling_type, region_color, region_sets, talairach_region_flag)
	char           *rgn_filename;
	char            filling_type;
	Pixel           region_color;
	llist           region_sets;
	BOOLEAN         talairach_region_flag;
{
	FILE           *rgnfp;			 /* input file pointer */
	RegionListData *region_data;
	char            filename[MAXLINE];
	char            filetype[MAXLINE];
	char            line[MAXLINE];		 /* line buffer */
	double          x, y, z;
	int             first_rgn_slice;
	int             i;			 /* counter */
	int             j;			 /* counter */
	int             last_rgn_slice;
	int             num_slices;
	int             pixel_num;
	int             slice_index;
	int             tempx, tempy;
	static char    *function = "ReadRegion";

	/*
	 * Open the region file.
	 */
	rgnfp = fopen (rgn_filename, "r");
	if (rgnfp == (FILE *) NULL) {
		fprintf (stderr, "Error in opening region file %s\n", rgn_filename);
		return FAIL;
	}

	/*
	 * Ignore the first HEADER lines in the region file.
	 */
	if (fgets (line, MAXLINE, rgnfp) == NULL) {
		fprintf (stderr, "Error: region file %s is empty.\n", rgn_filename);
		return FAIL;
	}
	if (sscanf (line, "%s %d %s", filetype, &i, filename) == 3 &&
	    strncmp (filetype, "filetype:", 9) == 0 && (i == 1 || i == 2)) {
		fgets (line, MAXLINE, rgnfp);
	}
	fscanf (rgnfp, "%d %d %f %f %f %d %d",
		&num_slices, &tempy, &x, &y, &z, &i, &slice_index);
	fscanf (rgnfp, "%d %d %d %d %d %d", &i, &slice_index,
		&first_rgn_slice, &last_rgn_slice, &tempx, &tempy);

	j = 0;
	while (fscanf (rgnfp, "%d %d", &slice_index, &pixel_num) != EOF && j < num_slices) {

		/*
		 * Allocate the region set header.
		 */
		region_data = (RegionListData *) pkg_malloc (sizeof (RegionListData),
							     function, "region_data");
		region_data->region_input = (XPoint *) pkg_malloc (pixel_num * sizeof (XPoint),
							   function, "region_data->region_input");
		for (i = 0; i < pixel_num; i++) {

			/*
			 * Convert pixels in image space to HD6 atlas space Units are in mm.
			 */
			fscanf (rgnfp, "%2x %2x", &tempx, &tempy);
			region_data->region_input[i].x = (tempx - REGION_SLICE_CX)
				* RGN_PIXEL_WIDTH;
			region_data->region_input[i].y = (REGION_SLICE_CY - tempy)
				* RGN_PIXEL_HEIGHT;
		}

		region_data->filling_type = filling_type;
		region_data->region_color = region_color;
		region_data->z = (int) slice2z (slice_index, zorigin_image1);
		region_data->region_pixel_num = pixel_num;

		if (talairach_region_flag) {
			region_data->z = ROUND (1.07 * region_data->z);

			for (i = 0; i < pixel_num; i++) {
				region_data->region_input[i].x
					= ROUND (0.900 * region_data->region_input[i].x);
				region_data->region_input[i].y
					= ROUND (1.06 * (region_data->region_input[i].y - 14.0));
			}
		}

		/*
		 * Add data set to global list.
		 */
		lladd (region_sets, region_data);

		j++;
	}

	fclose (rgnfp);
}



/*_______________________________________________________________________________
	Function:	DisplayAF3DPoints
	Arguments:	filenames:	AF3D file names, up to 10 sets.
	Description:	Read and display AF3D points for a specific slice.

	Return Value:	SUCCEED: successful; FAIL: failed.
	Author:		Tom Yang
			Created January 12, 1995.
_______________________________________________________________________________*/

PRIVATE int     DisplayAF3DPoints (display, window, gc, filenames, atlas_orient,
	                     display_symbol, symbol_type, pixel_color, symbol_size, outline_width,
				                   current_header, draw_portal_data)
	Display        *display;
	Window          window;
	GC              gc;
	char            filenames[][MAXLINE];
	char            atlas_orient;
	BOOLEAN        *display_symbol;
	char           *symbol_type;
	Pixel          *pixel_color;
	unsigned short *symbol_size;
	int            *outline_width;
	SCALE_HEADER   *current_header;
	DRAW_PORTAL_DATA *draw_portal_data;
{
	char            c;			 /* test char */
	int             fd;
	int             file_type;
	int             i;

	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		if (*(display_symbol + i)) {
			if (strlen (filenames[i]) == 0 ||
			    (fd = open (filenames[i], O_RDONLY, 0)) == -1) {
				continue;
			}

			/*
			 * Read either in Binary or Ascii format.
			 */
			file_type = getfile_type (filenames[i]);
			if (file_type == BINARY) {
				if (read (fd, &c, 1) == 1)
					DrawGammaZData (display, window, gc,
							fd,
							c,
							i,
							*(symbol_type + i),
							*(pixel_color + i),
							*(symbol_size + i),
							*outline_width,
							atlas_orient,
							current_header,
							draw_portal_data);
				close (fd);
			} else if (file_type == ASCII) {
				close (fd);
				DrawAsciiGammaZData (display, window, gc,
						     filenames[i],
						     i,
						     *(symbol_type + i),
						     *(pixel_color + i),
						     *(symbol_size + i),
						     *outline_width,
						     atlas_orient,
						     current_header,
						     draw_portal_data);
			}
		}
	}
	return SUCCEED;
}



/*________________________________________________________________________________
	Function:	ResizeSlice
	Arguments:	slice: pointer to slice.
			minimum: minimum pixel value of the pet image slice.
			minimum: maximum pixel value of the pet image slice.
	Description:
			Resize and process a slice.

	Return Value:	Pointer to new slice.

	Author:		Tom (Tongzeng) Yang
			(05/03/1991)
________________________________________________________________________________*/
PRIVATE SliceData * ResizeSlice (current_header, slice, minimum, maximum, pet_cx, pet_cy,
				 image1_flag, auto_image_value_flag, image_value_flag)
	SCALE_HEADER   *current_header;
	SliceData      *slice;			 /* slice pointer to new slice */
	short          *minimum;
	short          *maximum;
	float           pet_cx;
	float           pet_cy;
	BOOLEAN         image1_flag;
	BOOLEAN         auto_image_value_flag;
	char            image_value_flag;
{
	static char    *function = "ResizeSlice";
	long            slice_size;		 /* size of the slice image */
	short          *databuf;		 /* buffer for interpolated pet image */
	char           *pet_coord_databuf;	 /* temp buffer for coord. pet image */
	int             i, j;			 /* loop control integer */
	int             tempx, tempy;
	int             xoffset, yoffset;	 /* Offset for pet image */
	SliceData      *portalSlice;
	int             temp_coord_num;
	int             temp_back_num;
	int             rows;

	/*
	 * Allocate memory for portal slice.
	 */
	portalSlice = (SliceData *) pkg_malloc (sizeof (SliceData), function, "portalSlice");

	portalSlice->slice_xyz = slice->slice_xyz;
	portalSlice->atlas_index = slice->atlas_index;
	portalSlice->pet_exist = slice->pet_exist;
	portalSlice->rgn_pet_pixelwidth = slice->rgn_pet_pixelwidth;
	portalSlice->rgn_pet_pixelheight = slice->rgn_pet_pixelheight;

	/*
	 * Memory allocation for pet image.
	 */
	if (portalSlice->pet_exist) {
		portalSlice->pet_width = ROUND (slice->pet_width * slice->rgn_pet_pixelwidth);
		portalSlice->pet_height = ROUND (slice->pet_height * slice->rgn_pet_pixelheight);
		slice_size = portalSlice->pet_width * portalSlice->pet_height;
		databuf = (short *) pkg_malloc (slice_size * sizeof (short),
						function, "databuf");
		pet_coord_databuf = (char *) pkg_malloc (slice_size,
							 function, "pet_coord_databuf");

		/*
		 * Compute pet image offset.
		 */
		Transformation (portalSlice->rgn_pet_pixelwidth,
				portalSlice->rgn_pet_pixelheight,
				pet_cx,
				pet_cy,
				slice->pet_xstart,
				slice->pet_ystart,
				current_header->centerx,
				current_header->centery,
				&xoffset,
				&yoffset);

		/*
		 * The pixel has different size.
		 */
		xoffset -= ROUND (portalSlice->rgn_pet_pixelwidth / 2);
		yoffset -= ROUND (portalSlice->rgn_pet_pixelheight / 2);
		portalSlice->pet_xstart = xoffset;
		portalSlice->pet_ystart = yoffset;

		/*
		 * Resize and convert pet image.
		 */
		interp (slice->pet_input, slice->pet_width, slice->pet_height,
			databuf, portalSlice->pet_width, portalSlice->pet_height);

		Convert16to8 (databuf, portalSlice->pet_width, portalSlice->pet_height,
			      pet_coord_databuf, minimum, maximum,
			      auto_image_value_flag, image_value_flag);
		XtFree (databuf);

		temp_coord_num = 0;
		for (j = 0; j < portalSlice->pet_height; j++) {
			rows = j * portalSlice->pet_width;

			for (i = 0; i < portalSlice->pet_width &&
			     pet_coord_databuf[rows + i] == PET_BACK_VALUE; i++);
			tempx = i;

			for (i = portalSlice->pet_width - 1; i >= 0 &&
			     pet_coord_databuf[rows + i] == PET_BACK_VALUE; i--);
			tempy = i;

			if (tempy >= tempx) {
				temp_coord_num += tempy - tempx + 1;
			}
		}
		portalSlice->pet_point_num = temp_coord_num;

		portalSlice->pet_point_pixel = (unsigned char *) pkg_malloc (portalSlice->pet_point_num,
							function, "portalSlice->pet_point_pixel");
		portalSlice->pet_point_coord
			= (XPoint *) pkg_malloc (portalSlice->pet_point_num * sizeof (XPoint),
						 function, "portalSlice->pet_point_coord");

		/*
		 * Compute PET pixels and coordinates.
		 */
		temp_coord_num = 0;
		for (j = 0; j < portalSlice->pet_height; j++) {
			rows = j * portalSlice->pet_width;

			for (i = 0; i < portalSlice->pet_width &&
			     pet_coord_databuf[rows + i] == PET_BACK_VALUE; i++);
			tempx = i;

			for (i = portalSlice->pet_width - 1; i >= 0 &&
			     pet_coord_databuf[rows + i] == PET_BACK_VALUE; i--);
			tempy = i;

			for (i = tempx; i <= tempy; i++) {
				if (image1_flag)
					portalSlice->pet_point_pixel[temp_coord_num]
						= pet_coord_databuf[rows + i] + PET_COLOR_START;
				else
					portalSlice->pet_point_pixel[temp_coord_num]
						= pet_coord_databuf[rows + i] + PET_COLOR_START - 20;
				portalSlice->pet_point_coord[temp_coord_num].x = i + xoffset;
				portalSlice->pet_point_coord[temp_coord_num].y = j + yoffset;
				temp_coord_num++;
			}
		}
		XtFree (pet_coord_databuf);
	}
	return (portalSlice);
}



/*________________________________________________________________________________
	Function:	MakePortal
	Arguments:	display: pointer to X Display
			window: Window to which this portal will be drawn.
			slice: pointer SliceData to display in window
			x,y: int representing upper left coord. of window
	Description: 	Allocate, create, and display portal with atlas slice.

	Return Value:	A pointer to the new Portal.
_________________________________________________________________________________*/

PRIVATE Portal * MakePortal (slice)
	SliceData      *slice;
{
	Portal         *p;			 /* pointer to Portal */

	/*
	 * Allocate mem for the Portal header.
	 */
	p = (Portal *) pkg_malloc (sizeof (Portal), "MakePortal", "p");

	/*
	 * Assign portal header.
	 */
	p->slice = slice;

	/*
	 * Return the new portal.
	 */
	return (p);
}



/*_________________________________________________________________________________
	Function:	UpdatePortals
	Arguments:	display: pointer to X Display
			portal_list: linked list of Portals for image 1 or 2.
	Description: 	Redraw slice images and point data in slices.
	Return Value:	None.
_________________________________________________________________________________*/

PUBLIC void     UpdatePortals (display, window, gc, current_header, portal_list,
	                     atlas_orient, display_atlas_flag, display_image_flag, xstart, ystart,
	                auto_image_value_flag, image_value_flag, colorbar_xstart, colorbar_ystart,
                 colorbar_width, colorbar_length, hori_colorbar, colorbar_onoff, color_table_name,
		                  color_bar_exist, color_bar_min, color_bar_middle, color_bar_max,
			                       image1_flag, image_zorigin, overlay)
	Display        *display;
	Window          window;			 /* window to display to */
	GC              gc;			 /* graphics context */
	SCALE_HEADER   *current_header;
	llist           portal_list;
	char            atlas_orient;
	BOOLEAN        *display_atlas_flag;
	BOOLEAN        *display_image_flag;
	short           xstart;			 /* portal x start */
	short           ystart;			 /* portal y start */
	BOOLEAN         auto_image_value_flag;
	BOOLEAN         image_value_flag;
	short           colorbar_xstart;
	short           colorbar_ystart;
	short           colorbar_width;
	short           colorbar_length;
	BOOLEAN         hori_colorbar;
	BOOLEAN         colorbar_onoff;
	char           *color_table_name;
	BOOLEAN        *color_bar_exist;
	XRectangle     *color_bar_min;
	XRectangle     *color_bar_middle;
	XRectangle     *color_bar_max;
	BOOLEAN         image1_flag;
	int             image_zorigin;
	BOOLEAN         overlay;
{
	BOOLEAN         low2high;
	BOOLEAN         pet_exist = FALSE;
	BOOLEAN         slice_num_flag;
	DRAW_PORTAL_DATA draw_portal_data;
	FILE           *atlasfp;
	FILE           *maskfp;
	Portal         *portal;			 /* current portal */
	SliceData      *slice;			 /* current slice */
	XCharStruct     overall;
	XFontStruct    *font_struct;
	char            stringbuf[MAXLINE];	 /* buffer for label */
	float           slice_scale = dSliceMagnification;
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	int             i;			 /* counter */
	int             j;			 /* counter */
	int             k;			 /* counter */
	int             mask_bottom;
	int             mask_top;
	int             rows_index;
	int             temp_bottom;
	int             temp_top;
	short          *atlas_slice;
	short          *abs_mask_slice;
	short           bottom;
	short          *databuf;		 /* buffer for interpolated atlas image */
	short           label_xorigin;
	short           label_yorigin;
	short           left;
	short           list_size;
	short          *mask_slice;
	short           minimum;
	short           maximum;
	short           right;
	short           top;
	short           xinc;			 /* portal x increment in millimeter */
	short           yinc;			 /* portal y increment in millimeter */
	short           xinc_mm;		 /* portal x increment in millimeter */
	short           yinc_mm;		 /* portal y increment in millimeter */
	short           xlabel_mm;
	short           ylabel_mm;
	short           xoffset;		 /* portal x offset */
	short           yoffset;		 /* portal y offset */
	short           xrow_mm;		 /* row x increment in millimeter */
	short           yrow_mm;		 /* row y increment in millimeter */
	unsigned int    any_slice_size;
	unsigned int    mask_size;
	unsigned int    slice_size;

	list_size = llsize (portal_list);
	if (list_size > 0) {
		draw_portal_data.display_atlas_flag = display_atlas_flag;
		draw_portal_data.display_pet_flag = display_image_flag;
		draw_portal_data.display_region_flag = bDisplayRegion;
		draw_portal_data.display_label_flag = &bDisplayLabel;
		if (list_size == 1)
			draw_portal_data.spacing = dMaximumDistance;
		else
			draw_portal_data.spacing = pkg_min (nSpacing, dMaximumDistance);
		draw_portal_data.atlas_trim = &bAtlasTrim;
		draw_portal_data.all_planes = &bAllPlanes;
		draw_portal_data.symbol_displayed = bAF3DSymbolDisplayed;
		for (i = 0; i < MAX_AF3DSET_NUM; i++) {
			draw_portal_data.symbol_displayed[i] = FALSE;
		}

		xoffset = xstart;
		yoffset = ystart;
		xinc_mm = nxOffsetMillimeter;
		yinc_mm = nyOffsetMillimeter;
		xrow_mm = nxRowRowMillimeter;
		yrow_mm = nyRowRowMillimeter;
		xlabel_mm = nxLabelStart;
		ylabel_mm = nyLabelStart;
		left = nxFormatedScreen;
		top = nyFormatedScreen;
		right = left + nFormatedScreenWidth;
		bottom = top + nFormatedScreenHeight;
		low2high = bLow2High;
		font_struct = FontStruct;
		slice_num_flag = bSliceNumberFlag;

		/*
		 * Move to head of portal list.
		 */
		if (low2high)
			llhead (portal_list);
		else
			lltail (portal_list);

		if (!current_header->any_scale_flag) {
			maskfp = fopen (current_header->mask_name, "rb");
			if (maskfp == (FILE *) NULL) {
				fprintf (stderr, "UpdatePortals: Can't open %s\n",
					 current_header->mask_name);
				exit (-1);
			}
		}
		mask_size = sizeof (short) * 2 * current_header->height;

		if (*draw_portal_data.display_atlas_flag || current_header->any_scale_flag) {
			atlasfp = fopen (current_header->image_name, "rb");
			if (atlasfp == (FILE *) NULL) {
				fprintf (stderr, "UpdatePortals: Can't open %s\n",
					 current_header->image_name);
				exit (-1);
			}
			slice_size = sizeof (short) * current_header->width
				* current_header->height;
			if (current_header->any_scale_flag) {
				any_slice_size = slice_size;
				slice_size = sizeof (short) * current_header->any_atlas_width
					* current_header->any_atlas_height;
			}
		}

		/*
		 * For each portal.
		 */
		k = 0;
		while (k < list_size) {

			/*
			 * Get a portal.
			 */
			portal = (Portal *) llretrieve (portal_list);
			slice = portal->slice;
			xinc = ROUND (xinc_mm / current_header->pixel_size);
			yinc = ROUND (yinc_mm / current_header->pixel_size);
			pet_exist = pet_exist || slice->pet_exist;

			if (list_size > 1)
				FormatScreen (current_header->pixel_size, xinc, yinc, xrow_mm,
					      yrow_mm, &xoffset, &yoffset, &xstart,
					      &ystart, left, top, right, bottom);

			portal->x = xoffset - current_header->centerx;
			portal->y = yoffset - current_header->centery;

			if (slice_num_flag) {
				sprintf (stringbuf, "%d",
				  GetSliceNumber (slice->slice_xyz, atlas_orient, image_zorigin));
			} else {
				if (atlas_orient == TRANSVERSE)
					sprintf (stringbuf, "Z = %d", slice->slice_xyz);
				else if (atlas_orient == CORONAL)
					sprintf (stringbuf, "Y = %d", slice->slice_xyz);
				else
					sprintf (stringbuf, "X = %d", slice->slice_xyz);
			}

			XTextExtents (font_struct, stringbuf, strlen (stringbuf),
				      &direction_hint, &font_ascent, &font_descent, &overall);
			label_xorigin = xoffset + ROUND (xlabel_mm
							 / current_header->pixel_size);
			label_yorigin = yoffset + ROUND (ylabel_mm
							 / current_header->pixel_size);
			slice->xlabel_start = label_xorigin + overall.lbearing;
			slice->ylabel_start = label_yorigin - overall.ascent;
			slice->label_width = overall.rbearing - overall.lbearing;
			slice->label_height = overall.ascent + overall.descent;

			if (slice->atlas_index >= 0 && (*draw_portal_data.display_atlas_flag
							|| current_header->any_scale_flag)) {
				atlas_slice = (short *) malloc (slice_size);

				if (fseek (atlasfp, slice->atlas_index
					   * slice_size, SEEK_SET) != 0) {
					fprintf (stderr,
						 "UpdatePortals (portal.c): Seeking Error\n");
					exit (-1);
				}
				if (fread (atlas_slice, slice_size, 1, atlasfp) != 1) {
					fprintf (stderr, "%d fread error\n");
					perror ("view25d");
					exit (-1);
				}
				if (!current_header->any_scale_flag) {
					draw_portal_data.atlas_slice = atlas_slice;
				} else {
					databuf = (short *) malloc (any_slice_size);
					interp (atlas_slice,
						current_header->any_atlas_width,
						current_header->any_atlas_height,
						databuf,
						current_header->width,
						current_header->height);
					draw_portal_data.atlas_slice = databuf;
					XtFree (atlas_slice);
				}
			}
			mask_slice = (short *) malloc (mask_size);
			if (!current_header->any_scale_flag && slice->atlas_index >= 0) {
				if (fseek (maskfp, slice->atlas_index
					   * mask_size, SEEK_SET) != 0) {
					fprintf (stderr,
						 "UpdatePortals (portal.c): Seeking Error\n");
					exit (-1);
				}
				if (fread (mask_slice, mask_size, 1, maskfp) != 1) {
					fprintf (stderr, "%d fread error\n");
					perror ("view25d");
					exit (-1);
				}
			} else if (slice->atlas_index >= 0) {
				for (j = 0; j < current_header->height; j++) {
					rows_index = j * current_header->width;
					for (i = 0; i < current_header->width &&
					     draw_portal_data.atlas_slice[rows_index + i]
					     < ATLAS_THRESHOLD; i++);
					mask_slice[2 * j] = i;

					for (i = current_header->width - 1; i >= 0 &&
					     draw_portal_data.atlas_slice[rows_index + i]
					     < ATLAS_THRESHOLD; i--);
					mask_slice[2 * j + 1] = i;
				}
			}

			/*
			 * Fix broken outline if there is any.
			 */
			if (current_header->any_scale_flag && slice_scale <= 0.2 && slice->atlas_index >= 0) {
				abs_mask_slice = (short *) malloc (mask_size);

				for (j = 1; j < current_header->height; j++) {
					rows_index = 2 * j;
					abs_mask_slice[rows_index - 2] = abs (mask_slice[rows_index]
								    - mask_slice[rows_index - 2]);
					abs_mask_slice[rows_index - 1] = abs (mask_slice[rows_index + 1]
								    - mask_slice[rows_index - 1]);
				}

				for (j = 0; j < current_header->height
				     && mask_slice[2 * j] > mask_slice[2 * j + 1]; j++);
				mask_top = j;

				for (j = current_header->height - 1; j >= 0 &&
				     mask_slice[2 * j] > mask_slice[2 * j + 1]; j--);
				mask_bottom = j;

				for (j = mask_top + 1; j < mask_bottom; j++) {
					rows_index = 2 * (j - 1);
					if (abs (mask_slice[rows_index + 2] - mask_slice[rows_index])
					    >= SMOOTH_LENGTH) {
						mask_slice[rows_index + 2]
							= pkg_min (mask_slice[rows_index],
								   mask_slice[rows_index + 2]);
						draw_portal_data.atlas_slice[j * current_header->width
								+ mask_slice[rows_index + 2]] = 1;
					}
					if (abs (mask_slice[rows_index + 3] - mask_slice[rows_index + 1])
					    >= SMOOTH_LENGTH) {
						mask_slice[rows_index + 3]
							= pkg_max (mask_slice[rows_index + 1],
								   mask_slice[rows_index + 3]);
						draw_portal_data.atlas_slice[j * current_header->width
								+ mask_slice[rows_index + 3]] = 1;
					}
				}
				free (abs_mask_slice);
			}
			draw_portal_data.mask_slice = mask_slice;

			draw_portal_data.portal = portal;
			draw_portal_data.zlabel = stringbuf;
			draw_portal_data.label_xorigin = &label_xorigin;
			draw_portal_data.label_yorigin = &label_yorigin;

			DrawPortal (display, window, gc, current_header,
				    &draw_portal_data, atlas_orient, image1_flag, overlay);

			portal->xold = portal->x;
			portal->yold = portal->y;
			if (atlas_orient == FACING_RIGHT) {
				portal->xold += current_header->centerx
					+ (current_header->centerx - current_header->width + 1);
			}
			if (slice->atlas_index >= 0)
				XtFree (mask_slice);

			if (slice->atlas_index >= 0 && (*draw_portal_data.display_atlas_flag
							|| current_header->any_scale_flag))
				XtFree (draw_portal_data.atlas_slice);

			xoffset += xinc;
			yoffset += yinc;
			k++;
			if (low2high)
				llnext (portal_list);
			else
				llprevious (portal_list);
		}

		if (!current_header->any_scale_flag)
			fclose (maskfp);

		if (*draw_portal_data.display_atlas_flag)
			fclose (atlasfp);

		if (*draw_portal_data.display_pet_flag && pet_exist && colorbar_onoff) {
			DrawColorBar (display, window, gc, colorbar_xstart, colorbar_ystart,
				      colorbar_length, colorbar_width, hori_colorbar,
				      image_value_flag, color_table_name, image1_flag);

			XSetForeground (display, gc, LABEL_CELL);
			GetImageMinMax (image1_flag, auto_image_value_flag, &minimum, &maximum);
			DrawColorBarText (display, window, gc, &minimum, &maximum, font_struct,
					  hori_colorbar, colorbar_xstart, colorbar_ystart,
				    colorbar_length, colorbar_width, FALSE, auto_image_value_flag,
					  image_value_flag, color_bar_exist, color_bar_min,
					  color_bar_middle, color_bar_max);
		}
	}
}



/*________________________________________________________________________
	Function:	DrawPortal
	Arguments:	display: pointer to X Display
			portal: Portal to be drawn on screen
	Description: 	Redraw portal on screen.
	Return Value:	None.

	Author:		Tom Yang
			(05/07/1991)
________________________________________________________________________*/

PRIVATE void    DrawPortal (display, window, gc, current_header, draw_portal_data, atlas_orient,
			                    image1_flag, overlay)
	Display        *display;
	Window          window;
	GC              gc;
	SCALE_HEADER   *current_header;
	DRAW_PORTAL_DATA *draw_portal_data;
	char            atlas_orient;
	BOOLEAN         image1_flag;
	BOOLEAN         overlay;
{
	FLOAT_POINT3D  *region_input;
	FLOAT_POINT3D   point;
	GammaZData     *data_set;
	GammaZPoint    *gammaz_point;		 /* point to plot */
	Pixel           BackgroundPixel;
	Pixel           ColorPixel;
	RegionListData *region_set;
	SliceData      *slice;			 /* current slice */
	XPoint         *region_draw;
	double          coordinate;
	int             i;			 /* counter */
	int             j;			 /* counter */
	int             region_input_num;
	int             rgn_pet_pixelheight;
	int             rgn_pet_pixelwidth;
	int             rows;
	int             tempx;
	int             tempy;
	int             sx;
	int             sy;
	int             sz;
	short           x;
	short           y;

	/*
	 * Get header variables.
	 */
	slice = draw_portal_data->portal->slice;
	rgn_pet_pixelwidth = ROUND (slice->rgn_pet_pixelwidth);
	rgn_pet_pixelheight = ROUND (slice->rgn_pet_pixelheight);
	x = draw_portal_data->portal->x;
	y = draw_portal_data->portal->y;

	/*
	 * Draw mask
	 */

	/*
	 * XSetForeground (display, gc, BACKGROUND_CELL); if (slice->atlas_index >= 0 && (!overlay
	 * || !image1_flag)) { if (atlas_orient != FACING_RIGHT) { for (j = 0; j <
	 * current_header->height; j++) { tempx	= draw_portal_data->mask_slice [2 * j]; tempy	=
	 * draw_portal_data->mask_slice [2 * j + 1];
	 * 
	 * if (tempx <= tempy) XDrawLine (display, window, gc, x + tempx, y + j, x + tempy, y + j); }
	 * } else { for (j = 0; j < current_header->height; j++) { tempx	=
	 * draw_portal_data->mask_slice [2 * j]; tempy	= draw_portal_data->mask_slice [2 * j + 1];
	 * 
	 * tempx	= current_header->centerx + (current_header->centerx - tempy); tempy	=
	 * current_header->centerx + (current_header->centerx - tempx);
	 * 
	 * if (tempx <= tempy) XDrawLine (display, window, gc, x + tempx, y + j, x + tempy, y + j); }
	 * } }
	 */

	/*
	 * Put the PET image on the screen.
	 */
	if (*draw_portal_data->display_pet_flag && slice->pet_exist) {
		if (*draw_portal_data->atlas_trim) {
			i = 0;
			while (i < slice->pet_point_num) {
				sx = slice->pet_point_coord[i].x;
				sy = slice->pet_point_coord[i].y;

				if (sy >= 0 && sy < current_header->height) {
					tempx = draw_portal_data->mask_slice[2 * sy];
					tempy = draw_portal_data->mask_slice[2 * sy + 1];

					if (atlas_orient == FACING_RIGHT) {
						tempx = current_header->centerx
							+ (current_header->centerx - tempy);
						tempy = current_header->centerx
							+ (current_header->centerx - tempx);
					}
					if (sx >= tempx && sx <= tempy) {
						XSetForeground (display, gc, slice->pet_point_pixel[i]);
						XDrawPoint (display, window, gc, x + sx, y + sy);
					}
				}
				i++;
			}
		} else {
			i = 0;
			BackgroundPixel = PET_BACK_VALUE + PET_COLOR_START;
			while (i < slice->pet_point_num) {
				ColorPixel = slice->pet_point_pixel[i];

				XSetForeground (display, gc, ColorPixel);
				do {
					if (ColorPixel != BackgroundPixel)
						XDrawPoint (display, window, gc,
							    x + slice->pet_point_coord[i].x,
							    y + slice->pet_point_coord[i].y);
					i++;
				} while (i < slice->pet_point_num &&
					 slice->pet_point_pixel[i] == ColorPixel);
			}
		}
	}

	/*
	 * Draw atlas.
	 */
	if (*draw_portal_data->display_atlas_flag && slice->atlas_index >= 0
	    && (!overlay || overlay && image1_flag)) {
		if (image1_flag)
			XSetForeground (display, gc, ATLAS1_CELL);
		else
			XSetForeground (display, gc, ATLAS2_CELL);
		if (atlas_orient != FACING_RIGHT) {
			for (i = 0; i < current_header->height; i++) {
				for (j = 0; j < current_header->width; j++) {
					if (draw_portal_data->atlas_slice
					    [i * current_header->width + j] > 0) {
						XDrawPoint (display, window, gc, j + x, i + y);
					}
				}
			}
		} else {
			for (i = 0; i < current_header->height; i++) {
				for (j = 0; j < current_header->width; j++) {
					if (draw_portal_data->atlas_slice
					    [i * current_header->width + j] > 0) {
						XDrawPoint (display, window, gc,
							    x + current_header->centerx
							    + (current_header->centerx - j),
							    y + i);
					}
				}
			}
		}
	}

	/*
	 * Draw region.
	 */
	if (llsize (region_sets) > 0
	    && (!overlay || overlay && image1_flag)) {
		llhead (region_sets);
		do {
			region_set = (RegionListData *) llretrieve (region_sets);
			region_input = (FLOAT_POINT3D *) pkg_malloc (region_set->region_pixel_num
					  * sizeof (FLOAT_POINT3D), "DrawPortal", "region_input");
			j = 0;
			point.z = region_set->z;
			for (i = 0; i < region_set->region_pixel_num; i++) {
				point.x = region_set->region_input[i].x;
				point.y = region_set->region_input[i].y;
				coordinate = GetCoordinate (point, atlas_orient);
				if (coordinate >= slice->slice_xyz - 1 &&
				    coordinate < slice->slice_xyz + 1) {
					region_input[j].x = point.x;
					region_input[j].y = point.y;
					region_input[j].z = point.z;
					j++;
				}
			}
			region_input_num = j;

			if (region_input_num > 0) {
				region_draw = (XPoint *) pkg_malloc (region_input_num
						  * sizeof (XPoint), "DrawPortal", "region_draw");
				for (i = 0; i < region_input_num; i++) {

					/*
					 * Reverse coordination orientation and scale.
					 */
					sx = ROUND ((region_input[i].x * 1.0)
						    / current_header->pixel_size);
					sy = ROUND (region_input[i].y * -1.0
						    / current_header->pixel_size);
					sz = ROUND (region_input[i].z * -1.0
						    / current_header->pixel_size);

					if (atlas_orient == TRANSVERSE) {
						tempx = sx;
						tempy = sy;
					} else if (atlas_orient == CORONAL) {
						tempx = sx;
						tempy = sz;
					} else if (atlas_orient == FACING_LEFT) {
						tempx = sy;
						tempy = sz;
					} else {
						tempx = -sy;
						tempy = sz;
					}
					region_draw[i].x = x + tempx + current_header->centerx
						- rgn_pet_pixelwidth / 2;
					region_draw[i].y = y + tempy + current_header->centery
						- rgn_pet_pixelheight / 2;
				}

				XSetForeground (display, gc, region_set->region_color);

				if (region_set->filling_type == SOLID_RECT_INDEX) {
					for (i = 0; i < region_input_num; i++) {
						XFillRectangle (display, window, gc,
							region_draw[i].x - rgn_pet_pixelwidth / 2,
						       region_draw[i].y - rgn_pet_pixelheight / 2,
							 rgn_pet_pixelwidth, rgn_pet_pixelheight);
					}
				} else if (region_set->filling_type < SOLID_RECT_INDEX) {
					XSetLineAttributes (display, gc, 5, LineSolid,
							    CapButt, JoinMiter);
					DrawOutlinedRegion (display, window, gc, rgn_pet_pixelwidth,
							    region_input_num, region_draw);
					XSetLineAttributes (display, gc, 1, LineSolid, CapButt, JoinMiter);
				} else {
					for (i = 0; i < region_input_num; i++) {
						XDrawRectangle (display, window, gc,
							region_draw[i].x - rgn_pet_pixelwidth / 2,
						       region_draw[i].y - rgn_pet_pixelheight / 2,
							 rgn_pet_pixelwidth, rgn_pet_pixelheight);
					}
				}
				free (region_draw);
			}
			free (region_input);
		} while (llnext (region_sets) != FAIL);
	}

	/*
	 * Draw AF3D points.
	 */
	if (!overlay || overlay && image1_flag)
		DisplayAF3DPoints (display, window, gc, szAF3DFileNames, atlas_orient,
			 bDisplaySymbol, cSymbolType, pixelAF3DColor, nSymbolSize, &nOutlineWidth,
				   current_header, draw_portal_data);

	/*
	 * Compute the z-coord label dimension and put the label on image.
	 */
	if (*draw_portal_data->display_label_flag) {
		XSetForeground (display, gc, LABEL_CELL);
		XDrawString (display, window, gc, *draw_portal_data->label_xorigin,
			     *draw_portal_data->label_yorigin, draw_portal_data->zlabel,
			     strlen (draw_portal_data->zlabel));
	}
}



/*________________________________________________________________________________
	Function:	ResizePortals
	Arguments:	display: pointer to X Display
			portal_list: linked list of Portals on screen
	Description: 	Resize portals.
	Return Value:	None.

	Author:		Tom (Tongzeng) Yang
			05/06/91.
__________________________________________________________________________________*/

PRIVATE void    ResizePortals (current_header, portal_list, atlas_list, minimum, maximum,
	                     pet_cx, pet_cy, image1_flag, auto_image_value_flag, image_value_flag)
	SCALE_HEADER   *current_header;
	llist           portal_list;
	llist           atlas_list;
	short          *minimum;
	short          *maximum;
	float           pet_cx;
	float           pet_cy;
	BOOLEAN         image1_flag;
	BOOLEAN         auto_image_value_flag;
	char            image_value_flag;
{
	Portal         *portal;			 /* current portal */
	SliceData      *slice;			 /* current slice */
	int             size;			 /* size of portal list */

	size = llsize (atlas_list);
	if (size > 0) {
		llhead (atlas_list);

		do {

			/*
			 * Move to head of portal list.
			 */
			llhead (portal_list);

			/*
			 * Get slice and portal.
			 */
			portal = (Portal *) llretrieve (portal_list);
			slice = portal->slice;

			/*
			 * Free memory that will change for portal->slice.
			 */
			if (slice->pet_exist) {
				XtFree (slice->pet_point_pixel);
				XtFree (slice->pet_point_coord);
			}

			/*
			 * Resize slices.
			 */
			slice = (SliceData *) llretrieve (atlas_list);
			portal->slice = ResizeSlice (current_header, slice, minimum, maximum,
					       pet_cx, pet_cy, image1_flag, auto_image_value_flag,
						     image_value_flag);

			/*
			 * Make new portal.
			 */
			portal = MakePortal (portal->slice);
			lldelete (portal_list);
			lladdtail (portal_list, portal);
		} while (llnext (atlas_list) != FAIL);
	}
}



/*_________________________________________________________________________________
	Function:
		Swap
	Arguments:
		int *: first integet pointer
		int *: second integet pointer
	Description:
		Swap two integer pointers.

	Author:
		Tom (Tongzeng) Yang
		(05/14/1991)
________________________________________________________________________________*/

PUBLIC void     Swap (pnFirst, pnSecond)
	int            *pnFirst;
	int            *pnSecond;
{
	int             temp;			 /* temp int buffer */

	temp = *pnFirst;
	*pnFirst = *pnSecond;
	*pnSecond = temp;
}



/*________________________________________________________________________________
	Function:
		SwapShort
	Arguments:
		short *: first integet pointer
		short *: second integet pointer
	Description:
		Swap two short integer pointers.

	Author:
		Tom (Tongzeng) Yang
		(11/14/1991)
________________________________________________________________________________*/

PUBLIC void     SwapShort (pnFirst, pnSecond)
	short          *pnFirst;
	short          *pnSecond;
{
	short           temp;			 /* temp int buffer */

	temp = *pnFirst;
	*pnFirst = *pnSecond;
	*pnSecond = temp;
}



/*____________________________________________________________________________
	Procedure:	ClearData.
	Arguments:	portal_list:		portal linked list.
			atlas_list1:		atlas linked list.
	Description:	Clear portal data from memory. Remove all points from
			screen and data structures.
	Author:		Tom Yang
			(06/05/1991)
____________________________________________________________________________*/

PUBLIC void     ClearData (portal_list, atlas_list)
	llist           atlas_list;
	llist           portal_list;
{
	Portal         *portal;
	SliceData      *slice;

	/*
	 * Clear portal_list data.
	 */
	llhead (portal_list);			 /* step through portal list */
	while (llsize (portal_list) > 0) {
		portal = (Portal *) llretrieve (portal_list);
		slice = portal->slice;

		if (slice->pet_exist) {
			XtFree (slice->pet_point_pixel);
			XtFree (slice->pet_point_coord);
		}
		XtFree (slice);
		XtFree (portal);

		lldelete (portal_list);
	}

	/*
	 * Clear memory corresponding to points in slice.
	 */
	llhead (atlas_list);			 /* step through the atlas list */
	while (llsize (atlas_list) > 0) {

		/*
		 * Get a slice.
		 */
		slice = (SliceData *) llretrieve (atlas_list);

		if (slice->pet_exist)
			XtFree (slice->pet_input);

		XtFree (slice);
		lldelete (atlas_list);
	}
}



/*____________________________________________________________________________
	Procedure:	ClearRegionSets.
	Arguments:	region_sets:	linked list for region data.
	Description:	Clear region sets linked list from memory.
	Author:		Tom Yang
			(05/22/1993)
____________________________________________________________________________*/

PRIVATE void    ClearRegionSets (region_sets)
	llist           region_sets;
{
	RegionListData *region_set;

	llhead (region_sets);
	while (llsize (region_sets) > 0) {	 /* step through data sets */

		/*
		 * Get a region set.
		 */
		region_set = (RegionListData *) llretrieve (region_sets);

		/*
		 * Free memory for region_input.
		 */
		XtFree (region_set->region_input);

		lldelete (region_sets);		 /* delete region set from list */
		XtFree (region_set);		 /* free memory for region set header */
	}
}



/*____________________________________________________________________________
	Procedure:	GetFileStatus.
			atlas_list:	lineked atlas list for image1.
			first_slice:	first slice id.
			last_slice:	last slice id.
			spacing:	spacing.
			input_node:	input node returned.
			image_zorigin:	zorigin of image.
	Description:	Determine if file is new, updated, or same.
	Author:		Tom (Tongzeng) Yang
			(06/26/1991)
_____________________________________________________________________________*/

PRIVATE void    GetFileStatus (atlas_list, first_slice, last_slice, spacing, image_zorigin, input_node)
	llist           atlas_list;
	short           first_slice;
	short          *last_slice;
	short           spacing;
	int             image_zorigin;
	char           *input_node;
{
	int             size;
	SliceData      *slice;
	int             desired_slice;
	char            atlas_orient = cAtlasOrientation;

	size = llsize (atlas_list);
	if (size > 0) {
		if (size == 1 + (*last_slice - first_slice) / spacing) {
			llhead (atlas_list);
			slice = llretrieve (atlas_list);
			for (desired_slice = first_slice; desired_slice <= *last_slice;
			     desired_slice += spacing);
			if (desired_slice > *last_slice)
				desired_slice -= spacing;
			*last_slice = desired_slice;
			if (slice->slice_xyz != GetSliceCoord (*last_slice, atlas_orient, image_zorigin)) {
				*input_node = INPUT_UPDATE;
			} else {
				lltail (atlas_list);
				slice = llretrieve (atlas_list);
				if (slice->slice_xyz != GetSliceCoord (first_slice,
								   atlas_orient, image_zorigin)) {
					*input_node = INPUT_UPDATE;
				} else {
					*input_node = INPUT_SAME;
				}
			}
		} else {
			*input_node = INPUT_UPDATE;
		}
	} else
		*input_node = INPUT_NEW;
}



/*__________________________________________________________________________
	Function:
		AtlasSliceCmp
	Arguments:
		s1: atlas slice structure.
		s2: atlas slice structure.
	Description:
		Function for llcmp assignment which compares two atlas slices
		by their z coord. Returns:

			-1: is z of s1 < z of s2
			 0: is z of s1 = z of s2
			 1: is z of s1 > z of s2
__________________________________________________________________________*/

PUBLIC int      AtlasSliceCmp (s1, s2)
	SliceData      *s1;
	SliceData      *s2;
{
	if (s1->slice_xyz < s2->slice_xyz)
		return (-1);
	if (s1->slice_xyz > s2->slice_xyz)
		return (1);
	return (0);
}



/*______________________________________________________________________________________
	Function:
		FormatScreen
	Arguments:
		xincrement:	increment in x direction.
		yincrement:	increment in y direction.
		slice:		current slice.
		xoffset:	offset in x direction.
		yoffset:	offset in y direction.
		xstart:		starting point in x direction.
		ystart:		starting point in y direction.
	Description:
		Compute the offsets of the next slice for making a portal.
	Author:		Tom (Tongzeng) Yang
			(06/26/1991)
______________________________________________________________________________________*/

PRIVATE void    FormatScreen (pixel_size, xincrement, yincrement, xrow_mm, yrow_mm,
		                       xoffset, yoffset, xstart, ystart, left, top, right, bottom)
	float           pixel_size;
	short           xincrement;
	short           yincrement;
	short           xrow_mm;
	short           yrow_mm;
	short          *xoffset;
	short          *yoffset;
	short          *xstart;
	short          *ystart;
	short           left;
	short           top;
	short           right;
	short           bottom;
{
	if (*xoffset + xincrement > right || *xoffset + xincrement < left ||
	    *yoffset + yincrement > bottom || *yoffset + yincrement < top) {
		*xstart += ROUND (xrow_mm / pixel_size);
		*ystart += ROUND (yrow_mm / pixel_size);
		*xoffset = *xstart;
		*yoffset = *ystart;
	}
}



/*_____________________________________________________________________________
	Function:
		RedrawPortals
	Arguments:
	Description:
		Redraw slices.
	Author:	Tom (Tongzeng) Yang
		(07/05/1991)
_____________________________________________________________________________*/

PUBLIC void     RedrawPortals (draw_data, atlas_list1, portal_list1, atlas_list2, portal_list2)
	DRAW_DATA      *draw_data;
	llist           atlas_list1;
	llist           portal_list1;
	llist           atlas_list2;
	llist           portal_list2;
{
	BOOLEAN        *atlas_changed = &bAtlasChanged;
	BOOLEAN        *mag_changed = &bMagChanged;
	BOOLEAN         overlay;
	BOOLEAN        *z0_changed = &bImage1ZoriginChanged;
	Display        *display = draw_data->display;
	GC              gc = draw_data->gc;
	Portal         *portal;
	SCALE_HEADER   *current_header;
	SliceData      *portal_slice;
	SliceData      *slice;
	Window          window = draw_data->window;
	char            atlas_orient = cAtlasOrientation;
	char            input_node = INPUT_NEW;
	float          *current_mag = &dSliceMagnification;
	float           pet_cx;
	float           pet_cy;
	int             image2_slice_offset;
	short           first_slice = nFirstSlice;
	short           last_slice = nLastSlice;
	short           maximum;
	short           minimum;
	short           spacing = nSpacing;

	if (bClearSliceArea)
		XClearArea (display, window, 0, 0, DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, FALSE);

	if (*mag_changed || *atlas_changed || *z0_changed) {
		input_node = INPUT_UPDATE;
		*mag_changed = FALSE;
		*atlas_changed = FALSE;
		*z0_changed = FALSE;
	} else
		GetFileStatus (atlas_list1, first_slice, &last_slice, spacing, zorigin_image1, &input_node);

	first_slice = GetDesiredFirst (first_slice, last_slice, spacing);

	if (input_node != INPUT_SAME) {
		if (input_node == INPUT_UPDATE) {
			ClearData (portal_list1, atlas_list1);
			ClearData (portal_list2, atlas_list2);
		}
		rCurrentScaleHeader = BuildQueue (first_slice, last_slice, spacing,
						  atlas_list1, atlas_list2, current_mag);
	}
	current_header = rCurrentScaleHeader;

	if (bImage1Selected) {
		InputImageData (current_header, szImage1FileName, atlas_list1, first_slice,
			   last_slice, spacing, zorigin_image1, bApplyMask1, &nAutoMinImage1Value,
				&nAutoMaxImage1Value);
	}
	if (bImage2Selected) {
		if (atlas_orient == TRANSVERSE)
			image2_slice_offset = zorigin_image2 - zorigin_image1;
		else
			image2_slice_offset = 0;
		InputImageData (current_header, szImage2FileName, atlas_list2,
			      first_slice + image2_slice_offset, last_slice + image2_slice_offset,
				spacing, zorigin_image2, bApplyMask2, &nAutoMinImage2Value,
				&nAutoMaxImage2Value);
	}
	if (InputRegionData (szRegionFileNames, bDisplayRegion, cRegionType,
			     pixelRegionColor, region_sets) == FAIL) {
		fprintf (stderr, "Error: in InputRegionData ()\n");
		return;
	}
	GetPetCenter (atlas_orient, &pet_cx, &pet_cy);

	if (nxImage1Start == nxImage2Start && nyImage1Start == nyImage2Start)
		overlay = TRUE;
	else
		overlay = FALSE;

	/*
	 * Process data for image 2
	 */
	if (bDisplayImage2 && bImage2Selected && strlen (szImage2FileName) > 0) {
		GetImageMinMax (FALSE, bAutoImage2ValueFlag, &minimum, &maximum);

		if (input_node != INPUT_SAME || llsize (portal_list2) == 0) {
			BuildPortals (current_header, atlas_list2, portal_list2,
				      &minimum, &maximum, pet_cx, pet_cy, FALSE,
				      bAutoImage2ValueFlag, cImage2ValueFlag);
		} else if (input_node == INPUT_SAME) {
			ResizePortals (current_header, portal_list2, atlas_list2,
				       &minimum, &maximum, pet_cx, pet_cy, FALSE,
				       bAutoImage2ValueFlag, cImage2ValueFlag);
		}
		UpdatePortals (display, window, gc, current_header, portal_list2,
			       atlas_orient, &bDisplayAtlas2, &bDisplayImage2, nxImage2Start,
		     nyImage2Start, bAutoImage2ValueFlag, cImage2ValueFlag, nxImage2ColorBarStart,
			       nyImage2ColorBarStart, nImage2ColorBarWidth, nImage2ColorBarLength,
			  bImage2HorizontalColorBar, bImage2ColorBarOnOff, szImage2ColorTableName,
			 &bImage2ColorBarExist, &rectImage2ColorBarMin, &rectImage2ColorBarMiddle,
			       &rectImage2ColorBarMax, FALSE, zorigin_image2, overlay);

		nxImage2OldColorBar = nxImage2ColorBarStart;
		nyImage2OldColorBar = nyImage2ColorBarStart;
		nImage2OldColorBarLength = nImage2ColorBarLength;
		nImage2OldColorBarWidth = nImage2ColorBarWidth;
		bImage2OldHoriColorBar = bImage2HorizontalColorBar;
	}

	/*
	 * Process data for image 1
	 */
	if (bDisplayImage1 && bImage1Selected && strlen (szImage1FileName) > 0) {
		GetImageMinMax (TRUE, bAutoImage1ValueFlag, &minimum, &maximum);

		if (input_node != INPUT_SAME || llsize (portal_list1) == 0)
			BuildPortals (current_header, atlas_list1, portal_list1,
				      &minimum, &maximum, pet_cx, pet_cy, TRUE,
				      bAutoImage1ValueFlag, cImage1ValueFlag);
		else if (input_node == INPUT_SAME)
			ResizePortals (current_header, portal_list1, atlas_list1,
				       &minimum, &maximum, pet_cx, pet_cy, TRUE,
				       bAutoImage1ValueFlag, cImage1ValueFlag);

		UpdatePortals (display, window, gc, current_header, portal_list1,
			       atlas_orient, &bDisplayAtlas1, &bDisplayImage1, nxImage1Start,
		     nyImage1Start, bAutoImage1ValueFlag, cImage1ValueFlag, nxImage1ColorBarStart,
			       nyImage1ColorBarStart, nImage1ColorBarWidth, nImage1ColorBarLength,
			  bImage1HorizontalColorBar, bImage1ColorBarOnOff, szImage1ColorTableName,
			 &bImage1ColorBarExist, &rectImage1ColorBarMin, &rectImage1ColorBarMiddle,
			       &rectImage1ColorBarMax, TRUE, zorigin_image1, overlay);

		nxImage1OldColorBar = nxImage1ColorBarStart;
		nyImage1OldColorBar = nyImage1ColorBarStart;
		nImage1OldColorBarLength = nImage1ColorBarLength;
		nImage1OldColorBarWidth = nImage1ColorBarWidth;
		bImage1OldHoriColorBar = bImage1HorizontalColorBar;
	}
}



/*______________________________________________________________________________________
	Function:
		BuildPortals
	Arguments:
	Description:
		Build portal slices.
	Author:	Tom (Tongzeng) Yang
		(07/05/1991)
______________________________________________________________________________________*/

PRIVATE void    BuildPortals (current_header, atlas_list, portal_list, minimum, maximum,
	                     pet_cx, pet_cy, image1_flag, auto_image_value_flag, image_value_flag)
	SCALE_HEADER   *current_header;
	llist           atlas_list;
	llist           portal_list;
	short          *minimum;
	short          *maximum;
	float           pet_cx;
	float           pet_cy;
	BOOLEAN         image1_flag;
	BOOLEAN         auto_image_value_flag;
	char            image_value_flag;
{
	SliceData      *slice;
	SliceData      *portal_slice;
	Portal         *portal;

	if (llsize (atlas_list) > 0) {
		llhead (atlas_list);

		do {
			slice = (SliceData *) llretrieve (atlas_list);
			portal_slice = ResizeSlice (current_header, slice, minimum, maximum,
					       pet_cx, pet_cy, image1_flag, auto_image_value_flag,
						    image_value_flag);
			portal = MakePortal (portal_slice);
			lladd (portal_list, portal);
		} while (llnext (atlas_list) != FAIL);
	}
}



/*______________________________________________________________________________________
	Function:
		DrawColorBar
	Arguments:
	Description:
		Draw a color bar on screen.
	Author:	Tom (Tongzeng) Yang
		(07/08/1991)
______________________________________________________________________________________*/

PUBLIC void     DrawColorBar (display, window, gc, x, y, length, width,
		                   horizontal_flag, pet_value_flag, color_table_name, image1_flag)
	Display        *display;
	Window          window;
	GC              gc;
	short           x;
	short           y;
	short           length;
	short           width;
	BOOLEAN         horizontal_flag;
	char            pet_value_flag;
	char           *color_table_name;
	BOOLEAN         image1_flag;
{
	BOOLEAN         direction_flag;
	BOOLEAN         show_border;
	Pixel           color_pixel;
	char           *extension;
	char           *short_color_table_name;
	int             border_height;
	int             border_width;
	int             screen;
	short           h;
	char            i;
	short           increment;
	short           w;
	short           xstart;
	short           ystart;
	unsigned long   border_foreground;

	extension = strrchr (color_table_name, '/');
	if (extension != NULL && strncmp (extension, "/", 1) == 0)
		short_color_table_name = extension + 1;
	else
		short_color_table_name = color_table_name;

	if (strncmp (short_color_table_name, "Black_on_White.tbl", 18) == 0
	    || strncmp (short_color_table_name, "White_on_Black.tbl", 18) == 0
	    || strncmp (short_color_table_name, "PosNeg_on_White.tbl", 19) == 0
	    || strncmp (short_color_table_name, "Color43.tbl", 11) == 0) {
		show_border = TRUE;
	} else
		show_border = FALSE;

	increment = length / NCOLORS;

	if (horizontal_flag) {
		w = increment;
		h = width;
		border_width = length;
		border_height = width;
	} else {
		w = width;
		h = increment;
		border_width = width;
		border_height = length;
	}

	if (!show_border) {
		XSetForeground (display, gc, BACKGROUND_CELL);
		XDrawRectangle (display, window, gc, x, y, border_width, border_height);
	}
	xstart = x;
	ystart = y;
	direction_flag = ((pet_value_flag != PETVALUE_NEGATIVE && horizontal_flag)
			  || (pet_value_flag == PETVALUE_NEGATIVE && !horizontal_flag));
	if (image1_flag)
		color_pixel = (direction_flag) ? PET_COLOR_START :
			PET_COLOR_START + NCOLORS - 1;
	else
		color_pixel = (direction_flag) ? PET_COLOR_START - 20 :
			PET_COLOR_START - 1;

	for (i = 0; i < NCOLORS; i++) {
		XSetForeground (display, gc, color_pixel);
		XFillRectangle (display, window, gc, xstart, ystart, w, h);

		if (horizontal_flag)
			xstart += increment;
		else
			ystart += increment;

		if (direction_flag)
			color_pixel++;
		else
			color_pixel--;
	}

	if (show_border) {
		screen = DefaultScreen (display);
		border_foreground = BlackPixel (display, screen);
		XSetForeground (display, gc, border_foreground);
		XDrawRectangle (display, window, gc, x, y, border_width, border_height);
	}
}



/*______________________________________________________________________________________
	Function:
		GetDesiredFirst
	Arguments:
	Description:
		Get the desired first slice number in a list configuration.
	Author:	Tom (Tongzeng) Yang
		(07/11/1991)
______________________________________________________________________________________*/

PRIVATE short   GetDesiredFirst (first_slice, last_slice, spacing)
	short           first_slice;
	short           last_slice;
	short           spacing;
{
	short           desired_slice;

	if (spacing < 0) {
		for (desired_slice = last_slice; desired_slice > first_slice;
		     desired_slice += spacing);
		if (desired_slice < first_slice)
			desired_slice += -spacing;
	} else {
		desired_slice = first_slice;
	}

	return desired_slice;
}



/*______________________________________________________________________________________
	Function:
		DrawColorBarText
	Arguments:
	Description:
		Draw or Clear Color Bar Text.
	Author:	Tom (Tongzeng) Yang
		(07/15/1991)
______________________________________________________________________________________*/

PUBLIC void     DrawColorBarText (display, window, gc, min_image_value, max_image_value,
                    font_struct, hori_colorbar, colorbar_xstart, colorbar_ystart, colorbar_length,
	                      colorbar_width, clear_flag, auto_image_value_flag, image_value_flag,
		                  color_bar_exist, color_bar_min, color_bar_middle, color_bar_max)
	Display        *display;
	Window          window;
	GC              gc;
	short          *min_image_value;
	short          *max_image_value;
	XFontStruct    *font_struct;
	BOOLEAN         hori_colorbar;
	short           colorbar_xstart;
	short           colorbar_ystart;
	short           colorbar_length;
	short           colorbar_width;
	BOOLEAN         clear_flag;
	BOOLEAN         auto_image_value_flag;
	BOOLEAN         image_value_flag;
	BOOLEAN        *color_bar_exist;
	XRectangle     *color_bar_min;
	XRectangle     *color_bar_middle;
	XRectangle     *color_bar_max;
{
	XCharStruct     overall;
	char            stringbuf[MAXLINE];	 /* buffer for label */
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	short           xstart;
	short           ystart;
	short           width;
	short           height;

	RegularMinMax (auto_image_value_flag, image_value_flag, min_image_value, max_image_value);

	sprintf (stringbuf, "%d", *min_image_value);
	XTextExtents (font_struct, stringbuf, strlen (stringbuf), &direction_hint,
		      &font_ascent, &font_descent, &overall);

	xstart = colorbar_xstart;
	ystart = colorbar_ystart;
	width = overall.rbearing - overall.lbearing;
	height = overall.ascent + overall.descent;

	if (hori_colorbar) {
		ystart += colorbar_width;
	} else {
		xstart += colorbar_width;
		ystart += colorbar_length - overall.ascent;
	}

	if (clear_flag && *color_bar_exist) {
		XClearArea (display, window, color_bar_min->x, color_bar_min->y,
			    color_bar_min->width, color_bar_min->height, FALSE);
	} else {
		XDrawString (display, window, gc, xstart - overall.lbearing,
			     ystart + overall.ascent, stringbuf, strlen (stringbuf));
	}
	color_bar_min->x = xstart;
	color_bar_min->y = ystart;
	color_bar_min->width = width;
	color_bar_min->height = height;

	sprintf (stringbuf, "%d", (*min_image_value + *max_image_value) / 2);
	XTextExtents (font_struct, stringbuf, strlen (stringbuf), &direction_hint,
		      &font_ascent, &font_descent, &overall);
	width = overall.rbearing - overall.lbearing;
	height = overall.ascent + overall.descent;
	if (hori_colorbar) {
		xstart = colorbar_xstart + colorbar_length / 2 - width / 2;
		if (!clear_flag)
			xstart -= overall.lbearing;
	} else {
		ystart = colorbar_ystart + colorbar_length / 2 - height / 2;
	}

	if (clear_flag && *color_bar_exist) {
		XClearArea (display, window, color_bar_middle->x, color_bar_middle->y,
			    color_bar_middle->width, color_bar_middle->height, FALSE);
	} else {
		XDrawString (display, window, gc, xstart - overall.lbearing,
			     ystart + overall.ascent, stringbuf, strlen (stringbuf));
	}
	color_bar_middle->x = xstart;
	color_bar_middle->y = ystart;
	color_bar_middle->width = width;
	color_bar_middle->height = height;

	sprintf (stringbuf, "%d", *max_image_value);
	XTextExtents (font_struct, stringbuf, strlen (stringbuf), &direction_hint,
		      &font_ascent, &font_descent, &overall);
	width = overall.rbearing - overall.lbearing;
	height = overall.ascent + overall.descent;
	if (hori_colorbar) {
		xstart = colorbar_xstart + colorbar_length - width;
		if (!clear_flag)
			xstart -= overall.lbearing;
	} else {
		ystart = colorbar_ystart;
	}

	if (clear_flag) {
		XClearArea (display, window, color_bar_max->x, color_bar_max->y,
			    color_bar_max->width, color_bar_max->height, FALSE);
	} else {
		XDrawString (display, window, gc, xstart - overall.lbearing,
			     ystart + overall.ascent, stringbuf, strlen (stringbuf));
	}
	color_bar_max->x = xstart;
	color_bar_max->y = ystart;
	color_bar_max->width = width;
	color_bar_max->height = height;

	if (!(*color_bar_exist))
		*color_bar_exist = TRUE;
}



/*____________________________________________________________________
	Function::
		DrawOutlinedRegion
	Parameters:
		display:	X Display pointer.
		window:		X window to draw into.
		gc:		graphics context.
		pixel_width:	pixel width.
		num_pixels:	number of pixels to be drawn for region.
		pixel_points:	pixel coordinates.
	Description:
		Draw outlined region.
	Author:	Tom (Tongzeng) Yang
		(08/29/1991)
____________________________________________________________________*/

PRIVATE void    DrawOutlinedRegion (display, window, gc, pixel_width, num_pixels, pixel_points)
	Display        *display;
	Window          window;
	GC              gc;
	unsigned short  pixel_width;
	int             num_pixels;
	XPoint         *pixel_points;
{
	static char    *function = "DrawOutlinedRegion";
	int             i;
	int             j;
	short           xmin = MAXSHORT;
	short           xmax = -MAXSHORT;
	short           ymin = MAXSHORT;
	short           ymax = -MAXSHORT;
	char           *character_matrix;
	unsigned short  width;
	unsigned short  height;

	for (i = 0; i < num_pixels; i++) {
		xmin = pkg_min (xmin, pixel_points[i].x);
		xmax = pkg_max (xmax, pixel_points[i].x);
		ymin = pkg_min (ymin, pixel_points[i].y);
		ymax = pkg_max (ymax, pixel_points[i].y);
	}

	width = xmax - xmin + 1;
	height = ymax - ymin + 1;
	character_matrix = (char *) pkg_malloc (width * height, function, "character_matrix");

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			character_matrix[j * width + i] = FALSE;
		}
	}

	for (i = 0; i < num_pixels; i++) {
		j = (pixel_points[i].y - ymin) * width + (pixel_points[i].x - xmin);
		character_matrix[j] = TRUE;
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (character_matrix[j * width + i]) {
				if (i == 0 || !character_matrix[j * width + (i - pixel_width)])
					XDrawLine (display, window, gc,
						   xmin + i - pixel_width / 2,
						   ymin + j - pixel_width / 2,
						   xmin + i - pixel_width / 2,
						   ymin + j + pixel_width / 2);
				if (i == width - 1 || !character_matrix[j * width + (i + pixel_width)])
					XDrawLine (display, window, gc,
						   xmin + i + pixel_width / 2,
						   ymin + j - pixel_width / 2,
						   xmin + i + pixel_width / 2,
						   ymin + j + pixel_width / 2);
				if (j == 0 || !character_matrix[(j - pixel_width) * width + i])
					XDrawLine (display, window, gc,
						   xmin + i - pixel_width / 2,
						   ymin + j - pixel_width / 2,
						   xmin + i + pixel_width / 2,
						   ymin + j - pixel_width / 2);
				if (j == height - 1 || !character_matrix[(j + pixel_width) * width + i])
					XDrawLine (display, window, gc,
						   xmin + i - pixel_width / 2,
						   ymin + j - pixel_width / 2,
						   xmin + i + pixel_width / 2,
						   ymin + j - pixel_width / 2);
			}
		}
	}

	XtFree (character_matrix);
}

PUBLIC void     DrawRectangle (display, window, gc, x, y, x2, y2)
	Display        *display;
	Window          window;
	GC              gc;
	int             x, y, x2, y2;
{
	CheckPoints (&x, &y, &x2, &y2);
	XDrawRectangle (display, window, gc, x, y, x2 - x, y2 - y);
}

PUBLIC void     CheckPoints (x, y, x2, y2)
	int            *x, *y, *x2, *y2;
{
	if (*x2 < *x)
		Swap (x2, x);
	if (*y2 < *y)
		Swap (y2, y);
}

PUBLIC void     GetLegendDim (display, files, nfiles, legend_font,
			                      legend_width, legend_height, font_height)
	Display        *display;
	char            files[][MAXLINE];
	int             nfiles;
	char           *legend_font;
	short          *legend_width;
	short          *legend_height;
	short          *font_height;
{
	BOOLEAN        *symbol_displayed = bAF3DSymbolDisplayed;
	XFontStruct    *font_struct;
	char           *extension;
	char           *short_name;
	int             actual_list_size;
	int             i;
	short           height;
	short           string_width;
	short           width;

	font_struct = XLoadQueryFont (display, legend_font);
	if (font_struct == 0) {
		fprintf (stderr, "Can Not Find Font %s\n", legend_font);
		return;
	}
	*font_height = font_struct->ascent + font_struct->descent;

	width = 0;
	actual_list_size = 0;
	for (i = 0; i < nfiles; i++) {
		if (strlen (files[i]) > 0 && symbol_displayed[i]) {
			actual_list_size++;
			extension = strrchr (files[i], '/');
			if (extension != NULL && strncmp (extension, "/", 1) == 0)
				short_name = extension + 1;
			else
				short_name = files[i];
			string_width = XTextWidth (font_struct, short_name, strlen (short_name));
			if (string_width > width)
				width = string_width;
		}
	}

	XFreeFont (display, font_struct);

	*legend_width = width + *font_height * 2 + *font_height / 2;
	*legend_height = *font_height * (actual_list_size - 1) * 1.2 + *font_height;
}

PUBLIC void     DrawLegend (display, window, gc, xstart, ystart, files, nfiles, legend_font,
			                    text_color, symbol_color, symbol_type)
	Display        *display;
	Window          window;
	GC              gc;
	int             xstart;
	int             ystart;
	char            files[][MAXLINE];
	int             nfiles;
	char           *legend_font;
	Pixel           text_color;
	Pixel          *symbol_color;
	char           *symbol_type;
{
	BOOLEAN        *symbol_displayed = bAF3DSymbolDisplayed;
	XFontStruct    *font_struct;
	char           *extension;
	char           *short_name;
	int             i;
	int             x;
	int             y;
	short           font_height;

	font_struct = XLoadQueryFont (display, legend_font);
	if (font_struct == 0) {
		fprintf (stderr, "Can Not Find Font %s\n", legend_font);
		return;
	}
	XSetFont (display, gc, font_struct->fid);

	font_height = font_struct->ascent + font_struct->descent;
	x = xstart + font_height * 2;
	y = ystart + font_height / 4;

	for (i = 0; i < nfiles; i++) {
		if (strlen (files[i]) > 0 && symbol_displayed[i]) {
			XSetForeground (display, gc, *(symbol_color + i));
			DrawSymbol (display, window, gc, xstart - font_height / 2,
			   y - 3 * font_height / 4, font_height, font_height, *(symbol_type + i));

			extension = strrchr (files[i], '/');
			if (extension != NULL && strncmp (extension, "/", 1) == 0)
				short_name = extension + 1;
			else
				short_name = files[i];
			XSetForeground (display, gc, text_color);
			XDrawString (display, window, gc, x, y, short_name, strlen (short_name));
			y += 1.2 * font_height;
		}
	}

	XFreeFont (display, font_struct);
}

PUBLIC void     GetLabelDim (font_struct, text_string, line_spacing, text_orient, label_width, label_height)
	XFontStruct    *font_struct;
	char           *text_string;
	int             line_spacing;
	char            text_orient;
	short          *label_width;
	short          *label_height;
{
	XCharStruct     overall;
	char           *pure_string;
	char           *string_buffer;
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	int             font_height;
	int             font_width;
	int             i, j, k, m;
	int             string_width;
	int             tabspaces;
	int             tabspaces_per_line;
	int             text_length;
	short           num_lines;
	short           width;
	short           height;
	static char    *function = "GetLabelDim";

	text_length = strlen (text_string);
	font_struct = FontStruct;
	font_height = font_struct->ascent + font_struct->descent;

	string_buffer = (char *) pkg_malloc (text_length, function, "string_buffer");
	pure_string = (char *) pkg_malloc (text_length, function, "pure_string");

	strncpy (string_buffer, "\0", text_length);
	strncpy (pure_string, "\0", text_length);

	XTextExtents (font_struct, "x", 1, &direction_hint, &font_ascent, &font_descent, &overall);
	font_width = overall.rbearing - overall.lbearing;

	i = 0;
	width = 0;
	height = 0;
	num_lines = 0;

	while (i < text_length) {
		j = 0;
		k = 0;
		m = 0;
		tabspaces_per_line = 0;
		while (i < text_length && text_string[i] != '\n') {
			if (text_string[i] != '\t') {
				pure_string[k++] = text_string[i];
				m++;
			} else {
				tabspaces_per_line += (m / 8 + 1) * 8 - m;
				m += tabspaces_per_line;
			}
			string_buffer[j++] = text_string[i];

			i++;
		}

		string_buffer[j++] = text_string[i];
		pure_string[j++] = text_string[i];
		i++;

		switch (text_orient) {
		case ORIENT_HORI:
			if (strlen (string_buffer) > 0) {
				string_width = XTextWidth (font_struct, pure_string,
							   strlen (pure_string))
					+ tabspaces_per_line * font_width;
				if (string_width > width)
					width = string_width;
			}
			num_lines++;
			break;
		case ORIENT_VERT:
			string_width = font_height * strlen (string_buffer);
			if (height > string_width)
				height = string_width;

			num_lines++;
			break;
		}

		strncpy (string_buffer, "\0", text_length);
		strncpy (pure_string, "\0", text_length);
	}
	XtFree (string_buffer);
	XtFree (pure_string);

	if (text_orient == ORIENT_HORI) {
		*label_width = width;
		*label_height = ((num_lines - 1) * font_height * line_spacing)
			/ LINE_SPACING_FACTOR + font_height;
	} else if (text_orient == ORIENT_VERT) {
		*label_width = ((num_lines - 1) * font_width * line_spacing)
			/ LINE_SPACING_FACTOR + font_width;
		*label_height = height;
	}
}

PUBLIC void     DrawLabel (display, window, gc, x, y, x2, y2, color_pixel, font_struct,
			                   text_string, alignment_flag, line_spacing, text_orient)
	Display        *display;
	Window          window;
	GC              gc;
	int             x, y, x2, y2;
	Pixel           color_pixel;
	XFontStruct    *font_struct;
	char           *text_string;
	char            alignment_flag;
	int             line_spacing;
	char            text_orient;
{
	char           *pure_string;
	char           *string_buffer;
	int             font_height;
	int             font_width;
	int             i, j, k, m;
	int             tabspaces;
	int             tabspaces_per_line;
	int             string_width;
	int             text_length;
	int             xinitial;
	int             yinitial;
	int             xstart;
	int             ystart;
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	XCharStruct     overall;
	static char    *function = "DrawLabel";

	text_length = strlen (text_string);
	font_struct = FontStruct;
	font_height = font_struct->ascent + font_struct->descent;

	string_buffer = (char *) pkg_malloc (text_length, function, "string_buffer");
	pure_string = (char *) pkg_malloc (text_length, function, "pure_string");

	strncpy (string_buffer, "\0", text_length);
	strncpy (pure_string, "\0", text_length);
	XSetFont (display, gc, font_struct->fid);
	XSetForeground (display, gc, color_pixel);

	XTextExtents (font_struct, "x", 1, &direction_hint, &font_ascent, &font_descent, &overall);
	font_width = overall.rbearing - overall.lbearing;

	i = 0;
	xstart = x;
	ystart = y;
	while (i < text_length) {
		j = 0;
		k = 0;
		m = 0;
		tabspaces_per_line = 0;
		while (i < text_length && text_string[i] != '\n') {
			if (text_string[i] != '\t') {
				pure_string[k++] = text_string[i];
				m++;
			} else {
				tabspaces_per_line += (m / 8 + 1) * 8 - m;
				m += tabspaces_per_line;
			}
			string_buffer[j++] = text_string[i];

			i++;
		}

		string_buffer[j++] = text_string[i];
		pure_string[j++] = text_string[i];
		i++;

		switch (text_orient) {
		case ORIENT_HORI:
			if (strlen (string_buffer) > 0) {
				string_width = XTextWidth (font_struct, pure_string,
							   strlen (pure_string))
					+ tabspaces_per_line * font_width;

				xstart = x;

				/*
				 * Only xstart is changed.
				 */
				GetTextStart (text_orient, alignment_flag, string_width,
					      font_height, &xstart, &ystart);

				xinitial = xstart;

				strncpy (pure_string, "\0", text_length);
				j = 0;
				k = 0;
				m = 0;
				while (j <= strlen (string_buffer)) {
					if (j < strlen (string_buffer) && string_buffer[j] != '\t') {
						pure_string[k++] = string_buffer[j];
						m++;
					} else {
						XDrawString (display, window, gc, xstart, ystart,
							     pure_string, k);

						m = (m / 8 + 1) * 8;
						xstart = xinitial + m * font_width;
						strncpy (pure_string, "\0", text_length);
						k = 0;
					}

					j++;
				}
			}
			ystart += (font_height * line_spacing) / LINE_SPACING_FACTOR;
			break;
		case ORIENT_VERT:
			ystart = y;
			string_width = font_height * strlen (string_buffer);

			/*
			 * Only ystart is changed.
			 */
			GetTextStart (text_orient, alignment_flag, font_width,
				      string_width, &xstart, &ystart);

			for (k = 0; k < strlen (string_buffer); k++) {
				XDrawString (display, window, gc,
					     xstart, ystart + (font_height * k),
					     string_buffer + k, 1);
			}

			xstart += (font_width * line_spacing) / LINE_SPACING_FACTOR;
			break;
		}

		strncpy (string_buffer, "\0", text_length);
		strncpy (pure_string, "\0", text_length);
	}

	XtFree (string_buffer);
	XtFree (pure_string);
}

PUBLIC void     GetImageMinMax (image1_flag, auto_flag, minimum, maximum)
	BOOLEAN         image1_flag;
	BOOLEAN         auto_flag;
	short          *minimum;
	short          *maximum;
{
	if (image1_flag) {
		if (auto_flag) {
			*minimum = nAutoMinImage1Value;
			*maximum = nAutoMaxImage1Value;
		} else {
			*minimum = nMinImage1Value;
			*maximum = nMaxImage1Value;
		}
	} else {
		if (auto_flag) {
			*minimum = nAutoMinImage2Value;
			*maximum = nAutoMaxImage2Value;
		} else {
			*minimum = nMinImage2Value;
			*maximum = nMaxImage2Value;
		}
	}
}

PUBLIC void     ErrorCheckMinMax (image_value_flag, minimum, maximum)
	BOOLEAN         image_value_flag;
	short          *minimum;
	short          *maximum;
{
	if (*minimum > *maximum)
		SwapShort (minimum, maximum);
	else if (*minimum == *maximum)
		(*maximum)++;

	if ((image_value_flag == PETVALUE_POSITIVE && *maximum <= 0)
	    || (image_value_flag == PETVALUE_NEGATIVE && *minimum >= 0)) {
		*maximum *= -1;
		*minimum *= -1;
		SwapShort (minimum, maximum);
	}
}

PUBLIC void     RegularMinMax (auto_image_value_flag, image_value_flag, minimum, maximum)
	BOOLEAN         auto_image_value_flag;
	BOOLEAN         image_value_flag;
	short          *minimum;
	short          *maximum;
{
	if (image_value_flag == PETVALUE_POSITIVE && (auto_image_value_flag || *minimum < 0)) {
		*minimum = 0;
	} else if (image_value_flag == PETVALUE_NEGATIVE && (auto_image_value_flag || *maximum > 0)) {
		*maximum = 0;
	} else if (image_value_flag == PETVALUE_DUAL) {
		*minimum = -pkg_max (abs (*minimum), abs (*maximum));
		*maximum = -*minimum;
	}
}



/*___________________________________________________________________________________
	Function:
		GetActiveWidget
	Arguments:
	Description:
		Get the current select widget from a group of widgets.

	Author:
		Tom (Tongzeng) Yang
		(12/05/1991)
_____________________________________________________________________________________*/

PUBLIC Widget GetActiveWidget (widget_set, num_widget)
	Widget         *widget_set;
	int             num_widget;
{
	int             index;

	index = GetActiveIndex (widget_set, num_widget);
	return *(widget_set + index);
}



/*____________________________________________________________________________________
	Function:
		GetActiveIndex
	Arguments:
	Description:
		Get the index of the current selected widget from a group of widgets.

	Author:
		Tom (Tongzeng) Yang
		(12/09/1991)
_____________________________________________________________________________________*/

PUBLIC int      GetActiveIndex (widget_set, num_widget)
	Widget         *widget_set;
	int             num_widget;
{
	int             i;
	int             index;

	for (i = 0; i < num_widget; i++) {
		if (XmToggleButtonGetState (widget_set[i])) {
			index = i;
			break;
		}
	}

	return index;
}



/*________________________________________________________________________________________________
	Function:
		GetNameFromWidget
	Arguments:
	Description:
		Get the color name form the current selected widget.

	Author:
		Tom (Tongzeng) Yang
		(12/05/1991)
_______________________________________________________________________________________________*/

PUBLIC void     GetNameFromWidget (widget_set, num_widget, mode, name)
	Widget         *widget_set;
	int             num_widget;
	char            mode;
	char           *name;
{
	int             index;

	index = GetActiveIndex (widget_set, num_widget);
	switch (mode) {
	case TOGGLE_COLOR:
		strcpy (name, nonImageColorList[index]);
		break;
	case TOGGLE_ALIGN:
		strcpy (name, alignmentList[index]);
		break;
	case TOGGLE_SLANT:
		strcpy (name, slantList[index]);
		break;
	case TOGGLE_WEIGHT:
		strcpy (name, weightList[index]);
		break;
	case TOGGLE_ORIENT:
		strcpy (name, orientList[index]);
		break;
	case TOGGLE_SCALE:
		strcpy (name, scaleList[index]);
		break;
	case TOGGLE_ATLAS_ORIENT:
		strcpy (name, atlasOrientList[index]);
		break;
	}
}



/*________________________________________________________________________________________________
	Function:
		GetIndexFromName
	Arguments:
	Description:
		Get the index from the name of a widget.

	Author:
		Tom (Tongzeng) Yang
		(12/09/1991)
_______________________________________________________________________________________________*/

PUBLIC int      GetIndexFromName (name, mode)
	char           *name;
	char            mode;
{
	char            lower_name[MAXLINE];
	int             index;
	float           mag_value;
	char          **symbol_names = symbolNames;

	strcpy (lower_name, name);
	Lowered (lower_name, strlen (lower_name));

	switch (mode) {
	case TOGGLE_COLOR:
		if (strncmp (lower_name, "black", 5) == 0)
			index = BLACK;
		else if (strncmp (lower_name, "blue", 4) == 0)
			index = BLUE;
		else if (strncmp (lower_name, "cyan", 4) == 0)
			index = CYAN;
		else if (strncmp (lower_name, "green", 5) == 0)
			index = GREEN;
		else if (strncmp (lower_name, "yellow", 6) == 0)
			index = YELLOW;
		else if (strncmp (lower_name, "magenta", 7) == 0)
			index = MAGENTA;
		else if (strncmp (lower_name, "red", 3) == 0)
			index = RED;
		else if (strncmp (lower_name, "white", 5) == 0)
			index = WHITE;
		else
			index = GRAY;
		break;
	case TOGGLE_ALIGN:
		if (strncmp (lower_name, "left", 4) == 0)
			index = ALIGNMENT_LEFT;
		else if (strncmp (lower_name, "center", 5) == 0)
			index = ALIGNMENT_CENTER;
		else
			index = ALIGNMENT_RIGHT;
		break;
	case TOGGLE_SLANT:
		if (strncmp (lower_name, "roman", 5) == 0)
			index = SLANT_ROMAN;
		else if (strncmp (lower_name, "italic", 6) == 0)
			index = SLANT_ITALIC;
		else
			index = SLANT_OBLIQUE;
		break;
	case TOGGLE_WEIGHT:
		if (strncmp (lower_name, "medium", 6) == 0)
			index = WEIGHT_MEDIUM;
		else
			index = WEIGHT_BOLD;
		break;
	case TOGGLE_ORIENT:
		if (strncmp (lower_name, "horizontal", 10) == 0)
			index = ORIENT_HORI;
		else
			index = ORIENT_VERT;
		break;
	case TOGGLE_MAG:
		mag_value = atof (name);
		for (index = 0; index < MAG_NUM - 1; index++) {
			if (mag_value == magnify_table[index])
				break;
		}
		break;
	case TOGGLE_SCALE:
		if (strncmp (lower_name, "positive", 8) == 0)
			index = PETVALUE_POSITIVE;
		else if (strncmp (lower_name, "negative", 8) == 0)
			index = PETVALUE_NEGATIVE;
		else
			index = PETVALUE_DUAL;
		break;
	case TOGGLE_ATLAS_ORIENT:
		if (strncmp (lower_name, "transverse", 10) == 0)
			index = TRANSVERSE;
		else if (strncmp (lower_name, "coronal", 7) == 0)
			index = CORONAL;
		else if (strncmp (lower_name, "facingleft", 10) == 0)
			index = FACING_LEFT;
		else
			index = FACING_RIGHT;
		break;
	case TOGGLE_SYMBOL_SHAPE:
		for (index = 0; index < NUM_SYMBOL_NAMES; index++) {
			if (strcmp (lower_name, symbol_names[index]) == 0)
				break;
		}
		break;
	}

	free (lower_name);

	return index;
}



/*______________________________________________________________________________________
	Function:
		GetWidgetFormName
	Arguments:
	Description:
		Get the current select widget from a group of widgets.

	Author:
		Tom (Tongzeng) Yang
		(12/05/1991)
_____________________________________________________________________________________*/

PUBLIC Widget GetWidgetFromName (widget_set, name, mode, index)
	Widget         *widget_set;
	char           *name;
	char            mode;
	int            *index;
{
	*index = GetIndexFromName (name, mode);

	return *(widget_set + *index);
}



/*_________________________________________________________________________________
	Function:
		GetFontName
	Arguments:
	Description:
		Get the font name from the settings in the Font menu.

	Author:
		Tom (Tongzeng) Yang
		(12/09/1991)
_________________________________________________________________________________*/
PUBLIC char    *GetFontName (font_list, font_name, weight_set,
			                     slant_set, size_list, size_name)
	Widget         *font_list;
	Widget         *font_name;
	Widget         *weight_set;
	Widget         *slant_set;
	Widget         *size_list;
	Widget         *size_name;
{
	char           *buffer;
	char           *font_string;
	char           *lower_name;
	char            size_string[MAXLINE];
	int             i;
	int             point_size;

	font_string = (char *) pkg_malloc (MAXLINE, "GetFontName", "font_string");

	strcpy (font_string, "-*-");
	buffer = XmTextGetString (*font_name);
	nFontNameIndex = XmListItemPos (*font_list,
					XmStringCreate (buffer, XmSTRING_DEFAULT_CHARSET));
	strcat (font_string, buffer);
	XtFree (buffer);

	if (XmToggleButtonGetState (weight_set[0]))
		strcat (font_string, "-medium");
	else
		strcat (font_string, "-bold");

	if (XmToggleButtonGetState (slant_set[0]))
		strcat (font_string, "-r");
	else if (XmToggleButtonGetState (slant_set[1]))
		strcat (font_string, "-i");
	else
		strcat (font_string, "-o");
	strcat (font_string, "-*-*-");

	buffer = XmTextGetString (*size_name);
	nFontSizeIndex = XmListItemPos (*size_list,
					XmStringCreate (buffer, XmSTRING_DEFAULT_CHARSET));
	strcat (font_string, buffer);
	point_size = PIXEL_POINT_FACTOR * atoi (buffer);
	sprintf (size_string, "-%d", point_size);
	strcat (font_string, size_string);
	XtFree (buffer);

	if (strstr (font_string, "symbol") == NULL)
		strcat (font_string, "-75-75-*-*-iso8859-1");
	else
		strcat (font_string, "-75-75-*-*-*-*");

	return font_string;
}

PUBLIC void     GetDefaultParameters (display, root_window, gc, file_name, default_parameters,
				                      xmsize_symbol)
	Display        *display;
	Window          root_window;
	GC              gc;
	char           *file_name;
	PARAMETERS_DATA *default_parameters;
	XmString        xmsize_symbol[];
{
	Arg             arglist[2];
	BOOLEAN        *auto_image1_value_flag = &bAutoImage1ValueFlag;
	BOOLEAN        *auto_image2_value_flag = &bAutoImage2ValueFlag;
	BOOLEAN         bool_buffer;
	BOOLEAN        *color_bar_orient_image1 = &bImage1HorizontalColorBar;
	BOOLEAN        *color_bar_orient_image2 = &bImage2HorizontalColorBar;
	BOOLEAN         filled;
	BOOLEAN        *image1_colorbar_onoff = &bImage1ColorBarOnOff;
	BOOLEAN        *image2_colorbar_onoff = &bImage2ColorBarOnOff;
	BOOLEAN        *image1_value_flag = &cImage1ValueFlag;
	BOOLEAN        *image2_value_flag = &cImage2ValueFlag;
	BOOLEAN        *region_onoff = bDisplayRegion;
	BOOLEAN        *slice_number_flag = &bSliceNumberFlag;
	BOOLEAN        *symbol_onoff = bDisplaySymbol;
	COLOR_BAR_MENU *color_bar_menu = default_parameters->color_bar_menu;
	DATA_TYPE_MENU *data_type_menu = default_parameters->data_type_menu;
	FILE           *file;
	FONT_MENU      *font_menu = default_parameters->font_menu;
	FORMAT_SCREEN_MENU *format_screen_menu = default_parameters->format_screen_menu;
	MAG_SCALE_MENU *mag_scale_menu = default_parameters->mag_scale_menu;
	Pixel           background;
	Pixel           foreground;
	Pixmap          pixmap_symbol;
	REGION_MENU    *region_menu = default_parameters->region_menu;
	SYMBOL_MENU    *symbol_menu = default_parameters->symbol_menu;
	Screen         *screen;
	Widget          widget_buffer;
	XFontStruct    *font_struct;
	XmString        item;
	char           *atlas_orient = &cAtlasOrientation;
	char           *color_table_image1 = szImage1ColorTableName;
	char           *color_table_image2 = szImage2ColorTableName;
	char           *complete_atlas_name = szAtlasFileName;
	char           *dynamic_symbol_color = cDynamicSymbolColor;
	char           *dynamic_symbol_type = cDynamicSymbolType;
	char           *extension;
	char           *font_name;
	char            label[MAXLINE];
	char            line[MAXLINE];
	char            lowered_value[MAXLINE];
	char            mask_name[MAXLINE];
	char           *region_alter_set = &cActiveRegionSet;
	char           *region_color = cRegionColor;
	char           *region_type = cRegionType;
	char           *short_atlas_name;
	char           *symbol_alter_set = &cActiveAF3DSet;
	char           *symbol_color = cSymbolColor;
	char          **symbol_names = symbolNames;
	char           *symbol_type = cSymbolType;
	char            value[MAXLINE];
	float          *slice_scale = &dSliceMagnification;
	float           new_scale = *slice_scale;
	int             i;
	int             n;
	short          *color_bar_length_image1 = &nImage1ColorBarLength;
	short          *color_bar_length_image2 = &nImage2ColorBarLength;
	short          *color_bar_width_image1 = &nImage1ColorBarWidth;
	short          *color_bar_width_image2 = &nImage2ColorBarWidth;
	short          *first_slice = &nFirstSlice;
	short          *last_slice = &nLastSlice;
	short          *max_value_image1 = &nMaxImage1Value;
	short          *max_value_image2 = &nMaxImage2Value;
	short           max_slice_num;
	short          *min_value_image1 = &nMinImage1Value;
	short          *min_value_image2 = &nMinImage2Value;
	short           min_slice_num;
	short          *xcolor_bar_image1 = &nxImage1ColorBarStart;
	short          *xcolor_bar_image2 = &nxImage2ColorBarStart;
	short          *ycolor_bar_image1 = &nyImage1ColorBarStart;
	short          *ycolor_bar_image2 = &nyImage2ColorBarStart;
	unsigned short *symbol_size = nSymbolSize;

	file = fopen (file_name, "r");
	if (file == NULL) {
		fprintf (stderr, "Error: Cannot Open file %s\n", file_name);
		return;
	}
	while (fgets (line, MAXLINE, file) != NULL) {
		if (sscanf (line, "%s %s", label, value) == 2) {
			strcpy (lowered_value, value);
			Lowered (lowered_value, strlen (lowered_value));

			if (strcmp (label, "AtlasFileName:") == 0) {
				extension = strrchr (value, '/');
				if (extension != NULL && strncmp (extension, "/", 1) == 0)
					short_atlas_name = extension + 1;
				else
					short_atlas_name = value;

				strcpy (complete_atlas_name, default_parameters->library_path);
				strcat (complete_atlas_name, short_atlas_name);
				ReadAtlasHeader (complete_atlas_name, default_parameters->library_path);

				strcpy (mask_name, default_parameters->library_path);
				if (strstr (short_atlas_name, "Talairach") == NULL) {
					strcat (mask_name, "HD6.mask");
				} else {
					strcat (mask_name, "pett6_n99_t88.mask");
				}
				strcpy (szImageMaskFileName, mask_name);

				continue;
			}
			if (strcmp (label, "DisplayImage1AtlasFlag:") == 0
			    || strcmp (label, "DisplayAtlasFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->atlas1, bool_buffer);
				bDisplayAtlas1 = bool_buffer;

				continue;
			}
			if (strcmp (label, "DisplayImage2AtlasFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->atlas2, bool_buffer);
				bDisplayAtlas2 = bool_buffer;

				continue;
			}
			if (strcmp (label, "AtlasTrim:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->atlas_trim, bool_buffer);
				bAtlasTrim = bool_buffer;

				continue;
			}
			if (strcmp (label, "DisplayPetFlag:") == 0
			    || strcmp (label, "DisplayImage1Flag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->image1, bool_buffer);
				bDisplayImage1 = bool_buffer;

				continue;
			}
			if (strcmp (label, "ApplyMaskFlag:") == 0
			    || strcmp (label, "ApplyImage1MaskFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->mask1, bool_buffer);
				bApplyMask1 = bool_buffer;

				continue;
			}
			if (strcmp (label, "DisplayImage2Flag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->image2, bool_buffer);
				bDisplayImage2 = bool_buffer;

				continue;
			}
			if (strcmp (label, "ApplyImage2MaskFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->mask2, bool_buffer);
				bApplyMask2 = bool_buffer;

				continue;
			}
			if (strcmp (label, "DisplayLabelFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*data_type_menu->label, bool_buffer);
				bDisplayLabel = bool_buffer;

				continue;
			}
			if (strcmp (label, "SliceNumZValueFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				n = 0;
				XtSetArg (arglist[n], XmNlabelString,
					  XmStringCreate (bool_buffer ? "Slice Numbers" :
							  "Z Values", XmSTRING_DEFAULT_CHARSET));
				n++;
				XtSetValues (*data_type_menu->slice_num_flag, arglist, n);

				SetSelectData (*data_type_menu->slice_num_flag, bool_buffer);
				*slice_number_flag = bool_buffer;

				continue;
			}
			if (strcmp (label, "AtlasOrientation:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (data_type_menu->orientation_set,
							      ORIENTATION_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (data_type_menu->orientation_set,
							     value, TOGGLE_ATLAS_ORIENT, &n);
				*atlas_orient = (char) n;
				SetDefaultAtlas (*atlas_orient, default_parameters->library_path);

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "FirstSlice:") == 0) {
				*first_slice = atoi (value);
				if (!(*slice_number_flag)) {
					*first_slice = GetSliceNumber (*first_slice,
								   *atlas_orient, zorigin_image1);
				}
				XmTextSetString (*data_type_menu->first_slice, value);

				continue;
			}
			if (strcmp (label, "LastSlice:") == 0) {
				*last_slice = atoi (value);
				if (!(*slice_number_flag)) {
					*last_slice = GetSliceNumber (*last_slice,
								   *atlas_orient, zorigin_image1);
				}
				XmTextSetString (*data_type_menu->last_slice, value);

				continue;
			}
			if (strcmp (label, "Spacing:") == 0) {
				nSpacing = *slice_number_flag ? atoi (value) :
					atoi (value) / 2;
				XmTextSetString (*data_type_menu->spacing, value);


				continue;
			}
			if (strcmp (label, "MagnificationFactor:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (mag_scale_menu->mag_set,
							      MAG_NUM), arglist, n);

				widget_buffer = GetWidgetFromName (mag_scale_menu->mag_set,
								   value, TOGGLE_MAG, &n);
				new_scale = atof (value);
				if (pkg_abs (*slice_scale - new_scale) >= ERROR_BOUND) {
					*slice_scale = new_scale;
					bMagChanged = TRUE;
				}
				cMagnifyIndex = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				XmTextSetString (*mag_scale_menu->text_mag_any, value);

				continue;
			}
			if (strcmp (label, "Image1Scale:") == 0
			    || strcmp (label, "PetImageScale:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget
					     (mag_scale_menu->scale_set_image1,
					      SCALE_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (mag_scale_menu->scale_set_image1,
							     value, TOGGLE_SCALE, &n);
				*image1_value_flag = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "AutoImage1MinMaxFlag:") == 0
			    || strcmp (label, "AutoPetMinMaxFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*mag_scale_menu->auto_minmax_image1, bool_buffer);
				*auto_image1_value_flag = bool_buffer;

				continue;
			}
			if (strcmp (label, "Image1Minimum:") == 0
			    || strcmp (label, "PetMinimum:") == 0) {
				*min_value_image1 = atoi (value);
				if (!(*auto_image1_value_flag))
					XmTextSetString (*mag_scale_menu->min_text_image1, value);

				continue;
			}
			if (strcmp (label, "Image1Maximum:") == 0
			    || strcmp (label, "PetMaximum:") == 0) {
				*max_value_image1 = atoi (value);
				if (!(*auto_image1_value_flag))
					XmTextSetString (*mag_scale_menu->max_text_image1, value);

				continue;
			}
			if (strcmp (label, "Image2Scale:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget
					     (mag_scale_menu->scale_set_image2,
					      SCALE_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (mag_scale_menu->scale_set_image2,
							     value, TOGGLE_SCALE, &n);
				*image2_value_flag = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "AutoImage2MinMaxFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*mag_scale_menu->auto_minmax_image2, bool_buffer);
				*auto_image2_value_flag = bool_buffer;

				continue;
			}
			if (strcmp (label, "Image2Minimum:") == 0) {
				*min_value_image2 = atoi (value);
				if (!(*auto_image2_value_flag))
					XmTextSetString (*mag_scale_menu->min_text_image2, value);

				continue;
			}
			if (strcmp (label, "Image2Maximum:") == 0) {
				*max_value_image2 = atoi (value);
				if (!(*auto_image2_value_flag))
					XmTextSetString (*mag_scale_menu->max_text_image2, value);

				continue;
			}
			if (strcmp (label, "AlterRegionSet:") == 0) {
				*region_alter_set = atoi (value) - 1;

				SetSelectData (GetActiveWidget (region_menu->toggle_alter_set,
								MAX_REGIONSET_NUM), False);
				SetSelectData (region_menu->toggle_alter_set[*region_alter_set], True);

				continue;
			}
			if (strcmp (label, "DisplayRegionSet:") == 0) {
				for (i = 0; i < MAX_REGIONSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);
					Lowered (value, strlen (value));

					if (strcmp (value, "true") == 0)
						bool_buffer = TRUE;
					else
						bool_buffer = FALSE;

					*(region_onoff + i) = bool_buffer;
				}

				continue;
			}
			if (strcmp (label, "RegionColorSet:") == 0) {
				for (i = 0; i < MAX_REGIONSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);

					*(region_color + i)
						= GetIndexFromName (value, TOGGLE_COLOR);
				}

				continue;
			}
			if (strcmp (label, "RegionFillingTypeSet:") == 0) {
				for (i = 0; i < MAX_REGIONSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);

					*(region_type + i)
						= GetIndexFromName (value, TOGGLE_SYMBOL_SHAPE);
				}

				continue;
			}
			if (strcmp (label, "TalairachRegionFlag:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				n = 0;
				XtSetArg (arglist[n], XmNset, bool_buffer);
				n++;
				XtSetArg (arglist[n], XmNlabelString,
					  XmStringCreate (bool_buffer ? "hd6tot88" :
							  "original", XmSTRING_DEFAULT_CHARSET));
				n++;
				XtSetValues (*region_menu->toggle_talairach_region, arglist, n);

				bTalairachRegionFlag = bool_buffer;

				continue;
			}
			if (strcmp (label, "AlterSymbolSet:") == 0) {
				*symbol_alter_set = atoi (value) - 1;

				SetSelectData (GetActiveWidget (symbol_menu->toggle_alter_set,
								MAX_AF3DSET_NUM), False);
				SetSelectData (symbol_menu->toggle_alter_set[*symbol_alter_set], True);

				continue;
			}
			if (strcmp (label, "DisplaySymbolSet:") == 0) {
				for (i = 0; i < MAX_AF3DSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);
					Lowered (value, strlen (value));

					if (strcmp (value, "true") == 0)
						bool_buffer = TRUE;
					else
						bool_buffer = FALSE;

					*(symbol_onoff + i) = bool_buffer;
				}

				continue;
			}
			if (strcmp (label, "AF3DColorSet:") == 0) {
				for (i = 0; i < MAX_AF3DSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);

					*(symbol_color + i)
						= GetIndexFromName (value, TOGGLE_COLOR);
					*(dynamic_symbol_color + i)
						= *(symbol_color + i);
				}

				continue;
			}
			if (strcmp (label, "AF3DSymbolShapeSet:") == 0) {
				for (i = 0; i < MAX_AF3DSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);

					*(symbol_type + i)
						= GetIndexFromName (value, TOGGLE_SYMBOL_SHAPE);
					*(dynamic_symbol_type + i) = *(symbol_type + i);
				}

				continue;
			}
			if (strcmp (label, "AF3DSymbolSizeSet:") == 0) {
				for (i = 0; i < MAX_AF3DSET_NUM; i++) {
					fgets (line, MAXLINE, file);
					sscanf (line, "%s %s", label, value);

					*(symbol_size + i) = atoi (value);
				}
			}
			if (strcmp (label, "AF3DSymbolOutlineWidth:") == 0) {
				n = atoi (value);
				nOutlineWidth = n;
				XmScaleSetValue (*symbol_menu->outline_width, n);
			}
			if (strcmp (label, "AF3DSymMaximumDistance:") == 0) {
				dMaximumDistance = atof (value);
				XmTextSetString (*symbol_menu->text_maximum_distance, value);
			}
			if (strcmp (label, "xFormatedScreen:") == 0) {
				nxFormatedScreen = atoi (value);
				XmTextSetString (*format_screen_menu->format_x, value);

				continue;
			}
			if (strcmp (label, "yFormatedScreen:") == 0) {
				nyFormatedScreen = atoi (value);
				XmTextSetString (*format_screen_menu->format_y, value);

				continue;
			}
			if (strcmp (label, "FormatedScreenWidth:") == 0) {
				nFormatedScreenWidth = atoi (value);
				XmTextSetString (*format_screen_menu->format_width, value);

				continue;
			}
			if (strcmp (label, "FormatedScreenHeight:") == 0) {
				nFormatedScreenHeight = atoi (value);
				XmTextSetString (*format_screen_menu->format_height, value);

				continue;
			}
			if (strcmp (label, "xStartFormatedSlices:") == 0
			    || strcmp (label, "xImage1StartFormatedSlices:") == 0) {
				nxImage1Start = atoi (value);
				XmTextSetString (*format_screen_menu->xstart_text_image1, value);

				continue;
			}
			if (strcmp (label, "yStartFormatedSlices:") == 0
			    || strcmp (label, "yImage1StartFormatedSlices:") == 0) {
				nyImage1Start = atoi (value);
				XmTextSetString (*format_screen_menu->ystart_text_image1, value);

				continue;
			}
			if (strcmp (label, "xImage2StartFormatedSlices:") == 0) {
				nxImage2Start = atoi (value);
				XmTextSetString (*format_screen_menu->xstart_text_image2, value);

				continue;
			}
			if (strcmp (label, "yImage2StartFormatedSlices:") == 0) {
				nyImage2Start = atoi (value);
				XmTextSetString (*format_screen_menu->ystart_text_image2, value);

				continue;
			}
			if (strcmp (label, "xOffsetFormatedSlices:") == 0) {
				nxOffsetMillimeter = atoi (value);
				XmTextSetString (*format_screen_menu->xoffset_text, value);

				continue;
			}
			if (strcmp (label, "yOffsetFormatedSlices:") == 0) {
				nyOffsetMillimeter = atoi (value);
				XmTextSetString (*format_screen_menu->yoffset_text, value);

				continue;
			}
			if (strcmp (label, "xRowRowOffsetFormatedSlices:") == 0) {
				nxRowRowMillimeter = atoi (value);
				XmTextSetString (*format_screen_menu->rowxoffset_text, value);

				continue;
			}
			if (strcmp (label, "yRowRowOffsetFormatedSlices:") == 0) {
				nyRowRowMillimeter = atoi (value);
				XmTextSetString (*format_screen_menu->rowyoffset_text, value);

				continue;
			}
			if (strcmp (label, "xLabelStartFormatedSlices:") == 0) {
				nxLabelStart = atoi (value);
				XmTextSetString (*format_screen_menu->label_xstart, value);

				continue;
			}
			if (strcmp (label, "yLabelStartFormatedSlices:") == 0) {
				nyLabelStart = atoi (value);
				XmTextSetString (*format_screen_menu->label_ystart, value);

				continue;
			}
			if (strcmp (label, "ZLowToHighOrder:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*format_screen_menu->display_order, bool_buffer);
				bLow2High = bool_buffer;

				continue;
			}
			if (strcmp (label, "xImage1ColorBarStart:") == 0
			    || strcmp (label, "xColorBarStart:") == 0) {
				*xcolor_bar_image1 = atoi (value);
				XmTextSetString (*color_bar_menu->image1_xstart, value);

				continue;
			}
			if (strcmp (label, "yImage1ColorBarStart:") == 0
			    || strcmp (label, "yColorBarStart:") == 0) {
				*ycolor_bar_image1 = atoi (value);
				XmTextSetString (*color_bar_menu->image1_ystart, value);

				continue;
			}
			if (strcmp (label, "Image1ColorBarWidth:") == 0
			    || strcmp (label, "ColorBarWidth:") == 0) {
				*color_bar_width_image1 = atoi (value);
				XmTextSetString (*color_bar_menu->image1_height, value);

				continue;
			}
			if (strcmp (label, "Image1ColorBarLength:") == 0
			    || strcmp (label, "ColorBarLength:") == 0) {
				*color_bar_length_image1 = atoi (value);
				XmTextSetString (*color_bar_menu->image1_width, value);

				continue;
			}
			if (strcmp (label, "Image1ColorBarOrientation:") == 0
			    || strcmp (label, "ColorBarOrientation:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*color_bar_menu->image1_orientation, bool_buffer);
				*color_bar_orient_image1 = bool_buffer;

				continue;
			}
			if (strcmp (label, "Image1ColorBarOnOff:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*color_bar_menu->image1_onoff, bool_buffer);
				*image1_colorbar_onoff = bool_buffer;

				continue;
			}
			if (strcmp (label, "xImage2ColorBarStart:") == 0) {
				*xcolor_bar_image2 = atoi (value);
				XmTextSetString (*color_bar_menu->image2_xstart, value);

				continue;
			}
			if (strcmp (label, "yImage2ColorBarStart:") == 0) {
				*ycolor_bar_image2 = atoi (value);
				XmTextSetString (*color_bar_menu->image2_ystart, value);

				continue;
			}
			if (strcmp (label, "Image2ColorBarWidth:") == 0) {
				*color_bar_width_image2 = atoi (value);
				XmTextSetString (*color_bar_menu->image2_height, value);

				continue;
			}
			if (strcmp (label, "Image2ColorBarLength:") == 0) {
				*color_bar_length_image2 = atoi (value);
				XmTextSetString (*color_bar_menu->image2_width, value);

				continue;
			}
			if (strcmp (label, "Image2ColorBarOrientation:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*color_bar_menu->image2_orientation, bool_buffer);
				*color_bar_orient_image2 = bool_buffer;

				continue;
			}
			if (strcmp (label, "Image2ColorBarOnOff:") == 0) {
				if (strcmp (lowered_value, "true") == 0)
					bool_buffer = TRUE;
				else
					bool_buffer = FALSE;

				SetSelectData (*color_bar_menu->image2_onoff, bool_buffer);
				*image2_colorbar_onoff = bool_buffer;

				continue;
			}
			if (strcmp (label, "BackgroundColor:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (default_parameters->back_color_set,
							      MAX_NONIMAGE_COLOR), arglist, n);

				widget_buffer
					= GetWidgetFromName (default_parameters->back_color_set,
							     value, TOGGLE_COLOR, &n);

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "Atlas1Color:") == 0
			    || strcmp (label, "AtlasColor:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (default_parameters->atlas1_color_set,
							      MAX_NONIMAGE_COLOR + 1), arglist, n);

				widget_buffer
					= GetWidgetFromName (default_parameters->atlas1_color_set,
							     value, TOGGLE_COLOR, &n);

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "Atlas2Color:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (default_parameters->atlas2_color_set,
							      MAX_NONIMAGE_COLOR + 1), arglist, n);

				widget_buffer
					= GetWidgetFromName (default_parameters->atlas2_color_set,
							     value, TOGGLE_COLOR, &n);

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "Image1ColorTable:") == 0
			    || strcmp (label, "PetColorTable:") == 0) {
				item = XmStringCreate (value, XmSTRING_DEFAULT_CHARSET);
				widget_buffer
					= XmSelectionBoxGetChild (*default_parameters->image1_color_box,
								  XmDIALOG_LIST);
				XmListSelectItem (widget_buffer, item, True);
				XtFree (item);

				strcpy (color_table_image1, default_parameters->library_path);
				strcat (color_table_image1, value);

				continue;
			}
			if (strcmp (label, "Image2ColorTable:") == 0) {
				item = XmStringCreate (value, XmSTRING_DEFAULT_CHARSET);
				widget_buffer
					= XmSelectionBoxGetChild (*default_parameters->image2_color_box,
								  XmDIALOG_LIST);
				XmListSelectItem (widget_buffer, item, True);
				XtFree (item);

				strcpy (color_table_image2, default_parameters->library_path);
				strcat (color_table_image2, value);

				continue;
			}
			if (strcmp (label, "LabelColor:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (default_parameters->label_color_set,
							      MAX_NONIMAGE_COLOR), arglist, n);

				widget_buffer
					= GetWidgetFromName (default_parameters->label_color_set,
							     value, TOGGLE_COLOR, &n);

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "FontColor:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (font_menu->font_color_set,
							      MAX_NONIMAGE_COLOR), arglist, n);

				widget_buffer
					= GetWidgetFromName (font_menu->font_color_set,
							     value, TOGGLE_COLOR, &n);
				cTextColorIndex = n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "FontName:") == 0) {
				n = XmListItemPos (*font_menu->font_list,
						XmStringCreate (value, XmSTRING_DEFAULT_CHARSET));
				nFontNameIndex = n;

				XmListSelectPos (*font_menu->font_list, n, True);
				XmTextSetString (*font_menu->font_name, value);

				continue;
			}
			if (strcmp (label, "FontSize:") == 0) {
				n = XmListItemPos (*font_menu->size_list,
						XmStringCreate (value, XmSTRING_DEFAULT_CHARSET));
				nFontSizeIndex = n;

				XmListSelectPos (*font_menu->size_list, n, True);
				XmTextSetString (*font_menu->size_name, value);

				continue;
			}
			if (strcmp (label, "FontSlant:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (font_menu->slant_set,
							      SLANT_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (font_menu->slant_set,
							     value, TOGGLE_SLANT, &n);
				cFontSlantIndex = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "FontWeight:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (font_menu->weight_set,
							      WEIGHT_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (font_menu->weight_set,
							     value, TOGGLE_WEIGHT, &n);
				cFontWeightIndex = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "FontOrientation:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (font_menu->orient_set,
							      ORIENT_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (font_menu->orient_set,
							     value, TOGGLE_ORIENT, &n);
				cFontOrientIndex = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "TextInterLineSpace:") == 0) {
				XmScaleSetValue (*font_menu->line_spacing,
						 atoi (value));
				nLineSpacing = atoi (value);
				continue;
			}
			if (strcmp (label, "TextAlignment:") == 0) {
				n = 0;
				XtSetArg (arglist[n], XmNset, False);
				n++;
				XtSetValues (GetActiveWidget (font_menu->alignment_set,
							      ALIGNMENT_NUM), arglist, n);

				widget_buffer
					= GetWidgetFromName (font_menu->alignment_set,
							     value, TOGGLE_ALIGN, &n);
				cAlignment = (char) n;

				n = 0;
				XtSetArg (arglist[n], XmNset, True);
				n++;
				XtSetValues (widget_buffer, arglist, n);

				continue;
			}
			if (strcmp (label, "SaveFileFormat:") == 0) {
				n = 0;
				if (strcmp (value, "PostScript") == 0) {
					XtSetArg (arglist[n], XmNset, False);
					n++;
					XtSetArg (arglist[n], XmNlabelString,
						  XmStringCreate ("File Format: PostScript ",
								  XmSTRING_DEFAULT_CHARSET));
					n++;
					bWinDumpFile = FALSE;
				} else {
					XtSetArg (arglist[n], XmNset, True);
					n++;
					XtSetArg (arglist[n], XmNlabelString,
						  XmStringCreate ("File Format: Window Dump ",
								  XmSTRING_DEFAULT_CHARSET));
					n++;
					bWinDumpFile = TRUE;
				}
				XtSetValues (*default_parameters->save_format, arglist, n);

				continue;
			}
			if (strcmp (label, "SaveArea:") == 0) {
				n = 0;
				if (strcmp (value, "UserDefined") == 0) {
					XtSetArg (arglist[n], XmNset, False);
					n++;
					XtSetArg (arglist[n], XmNlabelString,
						  XmStringCreate ("Area Saved: User Defined ",
								  XmSTRING_DEFAULT_CHARSET));
					n++;
					bSaveWholeScreen = FALSE;
				} else {
					XtSetArg (arglist[n], XmNset, True);
					n++;
					XtSetArg (arglist[n], XmNlabelString,
						  XmStringCreate ("Area Saved: Entire Window",
								  XmSTRING_DEFAULT_CHARSET));
					n++;
					bSaveWholeScreen = TRUE;
				}
				XtSetValues (*default_parameters->save_area, arglist, n);
			}
		} else {
			fprintf (stderr, "Error: GetDefaultParameters %s\n", line);
		}
	}

	if (*slice_number_flag)
		strcpy (value, "Slice Numbers");
	else if (*atlas_orient == TRANSVERSE)
		strcpy (value, "Z Values");
	else if (*atlas_orient == CORONAL)
		strcpy (value, "Y Values");
	else
		strcpy (value, "X Values");

	n = 0;
	item = XmStringCreate (value, XmSTRING_DEFAULT_CHARSET);
	XtSetArg (arglist[n], XmNlabelString, item);
	n++;
	XtSetValues (*data_type_menu->slice_num_flag, arglist, n);
	XtFree (item);

	ErrorCheckMinMax (*image1_value_flag, min_value_image1, max_value_image1);
	ErrorCheckMinMax (*image2_value_flag, min_value_image2, max_value_image2);

	/*
	 * Check if the first and last slice are within the allowed range and change the boundary
	 * slices if necessary.
	 */
	if (*atlas_orient == TRANSVERSE) {
		max_slice_num = MAX_TRANS_ATLAS_NUM;
	} else {
		max_slice_num = MAX_ATLAS_NUM;
	}
	min_slice_num = 1;

	if (*first_slice < min_slice_num || *first_slice > max_slice_num) {
		fprintf (stderr,
			 "Your first slice %d is outside the allowed range of [%d, %d].\n",
			 *first_slice, min_slice_num, max_slice_num);

		if (*first_slice < min_slice_num)
			*first_slice = min_slice_num;
		else if (*first_slice > max_slice_num)
			*first_slice = max_slice_num;

		fprintf (stderr,
			 "The number %d is assigned as the new first slice.\n",
			 *first_slice);
	}
	if (*last_slice < min_slice_num || *last_slice > max_slice_num) {
		fprintf (stderr,
			 "Your last slice %d is outside the allowed range of [%d, %d].\n",
			 *last_slice, min_slice_num, max_slice_num);

		if (*last_slice < min_slice_num)
			*last_slice = min_slice_num;
		else if (*last_slice > max_slice_num)
			*last_slice = max_slice_num;

		fprintf (stderr,
			 "The number %d is assigned as the new last slice.\n",
			 *last_slice);
	}
	if (*first_slice > *last_slice)
		Swap (first_slice, last_slice);

	nxImage1OldColorBar = *xcolor_bar_image1;
	nyImage1OldColorBar = *ycolor_bar_image1;
	nImage1OldColorBarLength = *color_bar_length_image1;
	nImage1OldColorBarWidth = *color_bar_width_image1;
	bImage1OldHoriColorBar = *color_bar_orient_image1;

	nxImage2OldColorBar = *xcolor_bar_image2;
	nyImage2OldColorBar = *ycolor_bar_image2;
	nImage2OldColorBarLength = *color_bar_length_image2;
	nImage2OldColorBarWidth = *color_bar_width_image2;
	bImage2OldHoriColorBar = *color_bar_orient_image2;

	n = 0;
	XtSetArg (arglist[n], XtNbackground, &background);
	n++;
	XtGetValues (symbol_menu->toggle_symbol_set[0], arglist, n);

	screen = XtScreen (symbol_menu->label_symbol_set[0]);

	for (i = 0; i < MAX_REGIONSET_NUM; i++) {
		SetSelectData (region_menu->display_onoff_set[i], *(region_onoff + i));

		n = 0;
		XtSetArg (arglist[n], XtNforeground, &foreground);
		n++;
		XtGetValues (region_menu->toggle_color_set[*(region_color + i)], arglist, n);

		pixmap_symbol = XmGetPixmap (screen,
					     symbol_names[*(region_type + i)],
					     foreground, background);

		n = 0;
		XtSetArg (arglist[n], XmNlabelType, XmPIXMAP);
		n++;
		XtSetArg (arglist[n], XmNlabelPixmap, pixmap_symbol);
		n++;
		XtSetValues (region_menu->label_pattern_set[i], arglist, n);
	}

	SetSelectData (GetActiveWidget (region_menu->toggle_color_set, MAX_NONIMAGE_COLOR), False);
	SetSelectData (region_menu->toggle_color_set[*(region_color + *region_alter_set)], True);

	if (*(region_type + *region_alter_set) == SOLID_RECT_INDEX)
		i = REGION_FILLED;
	else if (*(region_type + *region_alter_set) < SOLID_RECT_INDEX)
		i = REGION_OUTLINE;
	else
		i = REGION_PIXEL;

	SetSelectData (GetActiveWidget (region_menu->toggle_pattern_set, REGION_STYLE_NUM), False);
	SetSelectData (region_menu->toggle_pattern_set[i], True);

	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		SetSelectData (symbol_menu->display_onoff_set[i], *(symbol_onoff + i));

		n = 0;
		XtSetArg (arglist[n], XtNforeground, &foreground);
		n++;
		XtGetValues (symbol_menu->toggle_color_set[*(symbol_color + i)], arglist, n);

		pixmap_symbol = XmGetPixmap (screen,
					     symbol_names[*(symbol_type + i)],
					     foreground, background);

		n = 0;
		XtSetArg (arglist[n], XmNlabelType, XmPIXMAP);
		n++;
		XtSetArg (arglist[n], XmNlabelPixmap, pixmap_symbol);
		n++;
		XtSetValues (symbol_menu->label_symbol_set[i], arglist, n);

		XtFree (xmsize_symbol[i]);
		itoa (*(symbol_size + i), value);
		xmsize_symbol[i] = XmStringCreate (value, XmSTRING_DEFAULT_CHARSET);

		n = 0;
		XtSetArg (arglist[n], XmNlabelString, *(xmsize_symbol + i));
		n++;
		XtSetValues (symbol_menu->label_symbol_size_set[i], arglist, n);
	}

	SetSelectData (GetActiveWidget (symbol_menu->toggle_color_set, MAX_NONIMAGE_COLOR), False);
	SetSelectData (symbol_menu->toggle_color_set[*(symbol_color + *symbol_alter_set)], True);

	if (*(symbol_type + *symbol_alter_set) >= MAX_SYMBOL_NUM) {
		i = *(symbol_type + *symbol_alter_set) - MAX_SYMBOL_NUM;
		filled = TRUE;
	} else {
		i = *(symbol_type + *symbol_alter_set);
		filled = FALSE;
	}

	SetSelectData (GetActiveWidget (symbol_menu->toggle_symbol_set, MAX_SYMBOL_NUM), False);
	SetSelectData (symbol_menu->toggle_symbol_set[i], True);

	SetSelectData (*symbol_menu->filled, filled);

	XmScaleSetValue (*symbol_menu->size_scale, *(symbol_size + *symbol_alter_set));

	SetColorParameters (display, root_window, DefaultScreen (display), default_parameters);

	font_name = GetFontName (*font_menu->font_list,
				 *font_menu->font_name,
				 font_menu->weight_set,
				 font_menu->slant_set,
				 *font_menu->size_list,
				 *font_menu->size_name);
	font_struct = XLoadQueryFont (display, font_name);
	if (font_struct == 0)
		fprintf (stderr, "Can Not Find Font %s\n", font_name);
	else {
		XSetFont (display, gc, font_struct->fid);
	}
	XtFree (font_name);
	FontStruct = font_struct;

	fclose (file);
}

PUBLIC void     SaveDefaultParameters (file_name, default_parameters)
	char           *file_name;
	PARAMETERS_DATA *default_parameters;
{
	BOOLEAN         bool_buffer;
	BOOLEAN        *slice_number_flag = &bSliceNumberFlag;
	BOOLEAN        *region_onoff = bDisplayRegion;
	BOOLEAN        *symbol_onoff = bDisplaySymbol;
	COLOR_BAR_MENU *color_bar_menu = default_parameters->color_bar_menu;
	DATA_TYPE_MENU *data_type_menu = default_parameters->data_type_menu;
	FILE           *file;
	FONT_MENU      *font_menu = default_parameters->font_menu;
	FORMAT_SCREEN_MENU *format_screen_menu = default_parameters->format_screen_menu;
	SYMBOL_MENU    *symbol_menu = default_parameters->symbol_menu;
	char            atlas_orient = cAtlasOrientation;
	char          **color_list = nonImageColorList;
	char           *complete_atlas_name = szAtlasFileName;
	char           *extension;
	char           *region_color = cRegionColor;
	char           *region_type = cRegionType;
	char           *short_atlas_name;
	char           *string;
	char           *symbol_color = cSymbolColor;
	char           *symbol_shape = cSymbolType;
	char          **symbol_names = symbolNames;
	char            value[MAXLINE];
	short           first_slice = nFirstSlice;
	int             i;
	int             int_buffer;
	short           last_slice = nLastSlice;
	short           spacing = nSpacing;
	unsigned short *symbol_size = nSymbolSize;

	file = fopen (file_name, "w");
	if (file == NULL) {
		fprintf (stderr, "Error: Cannot Open file %s\n", file_name);
		return;
	}
	extension = strrchr (complete_atlas_name, '/');
	if (extension != NULL && strncmp (extension, "/", 1) == 0)
		short_atlas_name = extension + 1;
	else
		short_atlas_name = complete_atlas_name;
	fprintf (file, "AtlasFileName:		%s\n", short_atlas_name);

	bool_buffer
		= XmToggleButtonGetState (*data_type_menu->atlas_trim);
	fprintf (file, "AtlasTrim:		%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->atlas1);
	fprintf (file, "DisplayImage1AtlasFlag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->image1);
	fprintf (file, "DisplayImage1Flag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->mask1);
	fprintf (file, "ApplyImage1MaskFlag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->atlas2);
	fprintf (file, "DisplayImage2AtlasFlag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->image2);
	fprintf (file, "DisplayImage2Flag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->mask2);
	fprintf (file, "ApplyImage2MaskFlag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->label);
	fprintf (file, "DisplayLabelFlag:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer = XmToggleButtonGetState (*data_type_menu->slice_num_flag);
	fprintf (file, "SliceNumZValueFlag:	%s\n", bool_buffer ? "True" : "False");

	GetNameFromWidget (data_type_menu->orientation_set,
			   ORIENTATION_NUM, TOGGLE_ATLAS_ORIENT, value);
	fprintf (file, "AtlasOrientation:	%s\n", value);

	if (!(*slice_number_flag)) {
		spacing *= 2;
		first_slice = GetSliceCoord (first_slice, atlas_orient, zorigin_image1);
		last_slice = GetSliceCoord (last_slice, atlas_orient, zorigin_image1);
	}
	fprintf (file, "FirstSlice:		%d\n", first_slice);
	fprintf (file, "LastSlice:		%d\n", last_slice);
	fprintf (file, "Spacing:		%d\n", spacing);

	int_buffer = GetActiveIndex (default_parameters->mag_scale_menu->mag_set,
				     MAG_NUM);
	if (int_buffer < MAG_NUM - 1) {
		sprintf (value, "%.3f", magnify_table[int_buffer]);
	} else {
		string = XmTextGetString (*default_parameters->mag_scale_menu->text_mag_any);
		strcpy (value, string);
		XtFree (string);
	}
	fprintf (file, "MagnificationFactor:	%s\n", value);

	GetNameFromWidget (default_parameters->mag_scale_menu->scale_set_image1,
			   SCALE_NUM, TOGGLE_SCALE, value);
	fprintf (file, "Image1Scale:		%s\n", value);

	bool_buffer
		= XmToggleButtonGetState (*default_parameters->mag_scale_menu->auto_minmax_image1);
	fprintf (file, "AutoImage1MinMaxFlag:	%s\n", bool_buffer ? "True" : "False");

	string = XmTextGetString (*default_parameters->mag_scale_menu->min_text_image1);
	fprintf (file, "Image1Minimum:		%s\n", string);
	XtFree (string);
	string = XmTextGetString (*default_parameters->mag_scale_menu->max_text_image1);
	fprintf (file, "Image1Maximum:		%s\n", string);
	XtFree (string);

	GetNameFromWidget (default_parameters->mag_scale_menu->scale_set_image2,
			   SCALE_NUM, TOGGLE_SCALE, value);
	fprintf (file, "Image2Scale:		%s\n", value);

	bool_buffer
		= XmToggleButtonGetState (*default_parameters->mag_scale_menu->auto_minmax_image2);
	fprintf (file, "AutoImage2MinMaxFlag:	%s\n", bool_buffer ? "True" : "False");

	string = XmTextGetString (*default_parameters->mag_scale_menu->min_text_image2);
	fprintf (file, "Image2Minimum:		%s\n", string);
	XtFree (string);
	string = XmTextGetString (*default_parameters->mag_scale_menu->max_text_image2);
	fprintf (file, "Image2Maximum:		%s\n", string);
	XtFree (string);

	fprintf (file, "AlterRegionSet:		%d\n", cActiveRegionSet + 1);
	fprintf (file, "DisplayRegionSet:	On\n");
	for (i = 0; i < MAX_REGIONSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, *(region_onoff + i) ? "True" : "False");
	}

	fprintf (file, "RegionColorSet:		Color\n");
	for (i = 0; i < MAX_REGIONSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, color_list[*(region_color + i)]);
	}

	fprintf (file, "RegionFillingTypeSet:	Shape\n");
	for (i = 0; i < MAX_REGIONSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, symbol_names[*(region_type + i)]);
	}

	bool_buffer = XmToggleButtonGetState (*default_parameters->region_menu->toggle_talairach_region);
	fprintf (file, "TalairachRegionFlag:	%s\n", bool_buffer ? "True" : "False");

	fprintf (file, "AlterSymbolSet:		%d\n", cActiveAF3DSet + 1);
	fprintf (file, "DisplaySymbolSet:	On\n");
	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, *(symbol_onoff + i) ? "True" : "False");
	}

	fprintf (file, "AF3DColorSet:		Color\n");
	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, color_list[*(symbol_color + i)]);
	}

	fprintf (file, "AF3DSymbolShapeSet:	Shape\n");
	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		fprintf (file, "	%d		%s\n",
			 i + 1, symbol_names[*(symbol_shape + i)]);
	}

	fprintf (file, "AF3DSymbolSizeSet:	Size\n");
	for (i = 0; i < MAX_AF3DSET_NUM; i++) {
		fprintf (file, "	%d		%d\n",
			 i + 1, *(symbol_size + i));
	}

	XmScaleGetValue (*symbol_menu->outline_width, &int_buffer);
	fprintf (file, "AF3DSymbolOutlineWidth:	%d\n", int_buffer);

	string = XmTextGetString (*symbol_menu->text_maximum_distance);
	fprintf (file, "AF3DSymMaximumDistance:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->format_x);
	fprintf (file, "xFormatedScreen:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->format_y);
	fprintf (file, "yFormatedScreen:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->format_width);
	fprintf (file, "FormatedScreenWidth:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->format_height);
	fprintf (file, "FormatedScreenHeight:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->xstart_text_image1);
	fprintf (file, "xImage1StartFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->ystart_text_image1);
	fprintf (file, "yImage1StartFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->xstart_text_image2);
	fprintf (file, "xImage2StartFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->ystart_text_image2);
	fprintf (file, "yImage2StartFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->xoffset_text);
	fprintf (file, "xOffsetFormatedSlices:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->yoffset_text);
	fprintf (file, "yOffsetFormatedSlices:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->rowxoffset_text);
	fprintf (file, "xRowRowOffsetFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->rowyoffset_text);
	fprintf (file, "yRowRowOffsetFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->label_xstart);
	fprintf (file, "xLabelStartFormatedSlices:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (*format_screen_menu->label_ystart);
	fprintf (file, "yLabelStartFormatedSlices:	%s\n", string);
	XtFree (string);

	bool_buffer
		= XmToggleButtonGetState (*format_screen_menu->display_order);
	fprintf (file, "ZLowToHighOrder:		%s\n", bool_buffer ? "True" : "False");

	string = XmTextGetString (*color_bar_menu->image1_xstart);
	fprintf (file, "xImage1ColorBarStart:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image1_ystart);
	fprintf (file, "yImage1ColorBarStart:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image1_width);
	fprintf (file, "Image1ColorBarLength:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image1_height);
	fprintf (file, "Image1ColorBarWidth:		%s\n", string);
	XtFree (string);

	bool_buffer
		= XmToggleButtonGetState (*color_bar_menu->image1_orientation);
	fprintf (file, "Image1ColorBarOrientation:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer
		= XmToggleButtonGetState (*color_bar_menu->image1_onoff);
	fprintf (file, "Image1ColorBarOnOff:		%s\n", bool_buffer ? "True" : "False");

	string = XmTextGetString (*color_bar_menu->image2_xstart);
	fprintf (file, "xImage2ColorBarStart:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image2_ystart);
	fprintf (file, "yImage2ColorBarStart:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image2_width);
	fprintf (file, "Image2ColorBarLength:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*color_bar_menu->image2_height);
	fprintf (file, "Image2ColorBarWidth:		%s\n", string);
	XtFree (string);

	bool_buffer
		= XmToggleButtonGetState (*color_bar_menu->image2_orientation);
	fprintf (file, "Image2ColorBarOrientation:	%s\n", bool_buffer ? "True" : "False");

	bool_buffer
		= XmToggleButtonGetState (*color_bar_menu->image2_onoff);
	fprintf (file, "Image2ColorBarOnOff:		%s\n", bool_buffer ? "True" : "False");

	GetNameFromWidget (default_parameters->back_color_set,
			   MAX_NONIMAGE_COLOR, TOGGLE_COLOR, value);
	fprintf (file, "BackgroundColor:	%s\n", value);

	GetNameFromWidget (default_parameters->atlas1_color_set,
			   MAX_NONIMAGE_COLOR + 1, TOGGLE_COLOR, value);
	fprintf (file, "Atlas1Color:		%s\n", value);

	GetNameFromWidget (default_parameters->atlas2_color_set,
			   MAX_NONIMAGE_COLOR + 1, TOGGLE_COLOR, value);
	fprintf (file, "Atlas2Color:		%s\n", value);

	string = XmTextGetString (XmSelectionBoxGetChild
				  (*default_parameters->image1_color_box, XmDIALOG_TEXT));
	fprintf (file, "Image1ColorTable:	%s\n", string);
	XtFree (string);

	string = XmTextGetString (XmSelectionBoxGetChild
				  (*default_parameters->image2_color_box, XmDIALOG_TEXT));
	fprintf (file, "Image2ColorTable:	%s\n", string);
	XtFree (string);

	GetNameFromWidget (default_parameters->label_color_set,
			   MAX_NONIMAGE_COLOR, TOGGLE_COLOR, value);
	fprintf (file, "LabelColor:		%s\n", value);

	GetNameFromWidget (font_menu->font_color_set,
			   MAX_NONIMAGE_COLOR, TOGGLE_COLOR, value);
	fprintf (file, "FontColor:		%s\n", value);

	string = XmTextGetString (*font_menu->font_name);
	fprintf (file, "FontName:		%s\n", string);
	XtFree (string);

	string = XmTextGetString (*font_menu->size_name);
	fprintf (file, "FontSize:		%s\n", string);
	XtFree (string);

	GetNameFromWidget (font_menu->slant_set,
			   SLANT_NUM, TOGGLE_SLANT, value);
	fprintf (file, "FontSlant:		%s\n", value);

	GetNameFromWidget (font_menu->weight_set,
			   WEIGHT_NUM, TOGGLE_WEIGHT, value);
	fprintf (file, "FontWeight:		%s\n", value);

	GetNameFromWidget (font_menu->orient_set,
			   ORIENT_NUM, TOGGLE_ORIENT, value);
	fprintf (file, "FontOrientation:	%s\n", value);

	XmScaleGetValue (*font_menu->line_spacing, &int_buffer);
	fprintf (file, "TextInterLineSpace:	%d\n", int_buffer);

	GetNameFromWidget (font_menu->alignment_set,
			   ALIGNMENT_NUM, TOGGLE_ALIGN, value);
	fprintf (file, "TextAlignment:		%s\n", value);

	if (XmToggleButtonGetState (*default_parameters->save_format))
		fprintf (file, "SaveFileFormat:		WindowDump\n");
	else
		fprintf (file, "SaveFileFormat:		PostScript\n");

	if (XmToggleButtonGetState (*default_parameters->save_area))
		fprintf (file, "SaveArea:		EntireWindow\n");
	else
		fprintf (file, "SaveArea:		UserDefined\n");

	fclose (file);
}



/*____________________________________________________________________________
	Procedure:	GetSliceCoord.
			slice_num:		slice number.
			atlas_orientation:	atlas orientation, transverse,
						coronal, or sagittal.
			image_zorigin,		zorigin of image.
	Description:	Get the slice coordinate (x, y, or z value) for a slice.
	Return:		slice coordinate.
	Author:		Tom (Tongzeng) Yang
			(05/21/1992)
_____________________________________________________________________________*/

PUBLIC int      GetSliceCoord (slice_num, atlas_orientation, image_zorigin)
	short           slice_num;
	char            atlas_orientation;
	int             image_zorigin;
{
	int             atlas_coord;

	if (atlas_orientation == TRANSVERSE) {
		atlas_coord = (int) slice2z (slice_num, image_zorigin);
	} else if (atlas_orientation == CORONAL) {
		atlas_coord = SliceY (slice_num);
	} else {
		atlas_coord = SliceX (slice_num);
	}

	return atlas_coord;
}



/*____________________________________________________________________________
	Procedure:	GetSliceNumber.
			slice_coord:		slice coordinate, x, y, or z.
			atlas_orientation:	atlas orientation, transverse,
						coronal, or sagittal.
			iamge_zorigin:		zorigin of image.
	Description:	Get the slice number for a slice.
	Return:		slice number.
	Author:		Tom (Tongzeng) Yang
			(05/22/1992)
_____________________________________________________________________________*/

PUBLIC int      GetSliceNumber (slice_coord, atlas_orientation, image_zorigin)
	short           slice_coord;
	char            atlas_orientation;
	int             image_zorigin;
{
	int             slice_number;

	if (atlas_orientation == TRANSVERSE) {
		slice_number = z2slice ((float) slice_coord, image_zorigin);
	} else if (atlas_orientation == CORONAL) {
		slice_number = YtoSlice (slice_coord);
	} else {
		slice_number = XtoSlice (slice_coord);
	}

	return slice_number;
}



/*____________________________________________________________________________
	Procedure:	GetPetDimension.
			atlas_orientation:	atlas orientation, transverse,
						coronal, or sagittal.
			num_slices:		total number of slices.
			slice_width:		pointer to slice width.
			slice_height:		pointer to slice height.
	Description:	Get the image dimension of a pet image.
	Return:		none.
	Author:		Tom (Tongzeng) Yang
			(05/28/1992)
_____________________________________________________________________________*/

PRIVATE void    GetPetDimension (atlas_orient, num_slices, slice_width, slice_height)
	char            atlas_orient;
	short           num_slices;
	int            *slice_width;
	int            *slice_height;
{
	if (atlas_orient == TRANSVERSE) {
		*slice_width = ENCODED_SLICE_WIDTH;
		*slice_height = ENCODED_SLICE_HEIGHT;
	} else if (atlas_orient == CORONAL) {
		*slice_width = ENCODED_SLICE_WIDTH;
		*slice_height = num_slices;
	} else {
		*slice_width = ENCODED_SLICE_HEIGHT;
		*slice_height = num_slices;
	}
}



/*____________________________________________________________________________
	Procedure:	GetPetCenter.
			atlas_orient:	atlas orientation, transverse,
					coronal, or sagittal.
			slice_cx:	pointer to slice center x.
			slice_cy:	pointer to slice center y.
	Description:	Get the image center of a pet image.
	Return:		none.
	Author:		Tom (Tongzeng) Yang
			(05/28/1992)
_____________________________________________________________________________*/

PRIVATE void    GetPetCenter (atlas_orient, slice_cx, slice_cy)
	char            atlas_orient;
	float          *slice_cx;
	float          *slice_cy;
{
	if (atlas_orient == TRANSVERSE) {
		*slice_cx = CX_IMAGE;
		*slice_cy = CY_IMAGE;
	} else if (atlas_orient == CORONAL) {
		*slice_cx = CX_IMAGE;
		*slice_cy = CZ_IMAGE;
	} else {
		*slice_cx = CY_IMAGE;
		*slice_cy = CZ_IMAGE;
	}
}



/*____________________________________________________________________________
	Procedure:	SetDefaultAtlas.
			atlas_orient:	atlas orientation, transverse,
					coronal, or sagittal.
	Description:	Set the default atlas image file as the current atlas image file.
	Return:		None.
	Author:		Tom (Tongzeng) Yang
			(06/02/1992)
_____________________________________________________________________________*/

PUBLIC void     SetDefaultAtlas (atlas_orient, library_path)
	char            atlas_orient;
	char           *library_path;
{
	char            szBuffer[MAXLINE];
	char            atlas_name[MAXLINE];
	char            mask_name[MAXLINE];

	strcpy (szBuffer, szAtlasFileName);
	strcpy (atlas_name, szBuffer);

	if (atlas_orient == TRANSVERSE && strstr (szBuffer, "transverse") == NULL) {
		strcpy (atlas_name, library_path);
		if (strstr (szBuffer, "Talairach") == NULL) {
			strcat (atlas_name, "HD6");
		} else {
			strcat (atlas_name, "Talairach");
		}
		strcat (atlas_name, "_transverse.atl");
	} else if (atlas_orient == CORONAL && strstr (szBuffer, "coronal") == NULL) {
		strcpy (atlas_name, library_path);
		strcat (atlas_name, "Talairach_coronal.atl");
	} else if ((atlas_orient == FACING_LEFT ||
		    atlas_orient == FACING_RIGHT)
		   && (strstr (szBuffer, "coronal") != NULL ||
		       strstr (szBuffer, "trans") != NULL)) {
		strcpy (atlas_name, library_path);
		strcat (atlas_name, "Talairach_sagittal.atl");
	}
	if (strcmp (szBuffer, atlas_name) != 0) {
		strcpy (szAtlasFileName, atlas_name);
		bAtlasChanged = TRUE;
		ReadAtlasHeader (atlas_name, library_path);

		strcpy (mask_name, library_path);
		if (strstr (atlas_name, "Talairach") == NULL) {
			strcat (mask_name, "HD6.mask");
		} else {
			strcat (mask_name, "pett6_n99_t88.mask");
		}
		strcpy (szImageMaskFileName, mask_name);
	}
}



/*____________________________________________________________________________
	Procedure:	ShowCoordinates
			atlas_orient:	atlas orientation, transverse,
					coronal, or sagittal.
	Description:	Show coordinates in the HD6 or Talairach 88 space.
	Return:		None.
	Author:		Tom (Tongzeng) Yang
			(06/08/1993)
_____________________________________________________________________________*/

PUBLIC void     ShowCoordinates (display, window, gc, x, y, current_header, portal_list1,
				                 portal_list2, atlas_orient, widget, save)
	Display        *display;
	Window          window;
	GC              gc;
	int             x;
	int             y;
	SCALE_HEADER   *current_header;
	llist           portal_list1;
	llist           portal_list2;
	char            atlas_orient;
	Widget          widget;
	BOOLEAN         save;
{
	Arg             arglist[1];
	FILE           *logfp;
	GC              mygc;
	Portal         *best_portal;
	Portal         *best_portal1;
	Portal         *best_portal2;
	Portal         *portal;
	SliceData      *slice;
	XmString        xmstring_buffer;
	char           *coordinates_file = szCoordinatesFileName;
	char           *image1_filename = szImage1FileName;
	char           *image2_filename = szImage2FileName;
	char            short_filename1[MAXLINE];
	char            short_filename2[MAXLINE];
	char           *string;
	char            szBuffer[MAXLINE];
	double          best_distance;
	double          best_distance1;
	double          best_distance2;
	double          distance;
	double          product;
	int             ax;
	int             ay;
	int             az;
	int             cx;
	int             cy;
	int             n;
	int             screen;
	int             tempx;
	int             tempy;
	int             xyz;
	llist           tracking_list;
	unsigned long   border_foreground;

	best_distance1 = (float) MAXSHORT;
	if (llsize (portal_list1) > 0) {
		llhead (portal_list1);
		do {
			portal = (Portal *) llretrieve (portal_list1);
			slice = portal->slice;
			if (slice->atlas_index >= 0) {
				cx = portal->xold + current_header->centerx;
				cy = portal->yold + current_header->centery;
				product = ((double) x - cx) * (x - cx)
					+ ((double) y - cy) * (y - cy);
				distance = sqrt (product);
				if (distance < best_distance1) {
					best_distance1 = distance;
					best_portal1 = portal;
				}
			}
		} while (llnext (portal_list1) != FAIL);
	}
	best_distance2 = (float) MAXSHORT;
	if (llsize (portal_list2) > 0) {
		llhead (portal_list2);
		do {
			portal = (Portal *) llretrieve (portal_list2);
			slice = portal->slice;
			if (slice->atlas_index >= 0) {
				cx = portal->xold + current_header->centerx;
				cy = portal->yold + current_header->centery;
				product = ((double) x - cx) * (x - cx)
					+ ((double) y - cy) * (y - cy);
				distance = sqrt (product);
				if (distance < best_distance2) {
					best_distance2 = distance;
					best_portal2 = portal;
				}
			}
		} while (llnext (portal_list2) != FAIL);
	}
	if (best_distance2 < best_distance1) {
		best_distance = best_distance2;
		best_portal = best_portal2;
		tracking_list = portal_list1;
	} else {
		best_distance = best_distance1;
		best_portal = best_portal1;
		tracking_list = portal_list2;
	}

	if (best_distance < (float) MAXSHORT) {
		tempx = best_portal->xold;
		if (atlas_orient == FACING_RIGHT) {
			tempx -= current_header->centerx
				+ (current_header->centerx - current_header->width + 1);
		}
		cx = tempx + current_header->centerx;
		cy = best_portal->yold + current_header->centery;
		slice = best_portal->slice;

		tempx = ROUND ((x - cx) * current_header->pixel_size * (-1.0));
		tempy = ROUND ((y - cy) * current_header->pixel_size * (-1.0));
		xyz = slice->slice_xyz;

		if (atlas_orient == TRANSVERSE) {
			ax = tempx;
			ay = tempy;
			az = xyz;
		} else if (atlas_orient == CORONAL) {
			ax = tempx;
			ay = xyz;
			az = tempy;
		} else if (atlas_orient == FACING_LEFT) {
			ax = xyz;
			ay = tempx;
			az = tempy;
		} else {
			ax = xyz;
			ay = -tempx;
			az = tempy;
		}
	} else {
		ax = x;
		ay = y;
		az = 0;
	}

	tempx = x - cx;
	tempy = y - cy;

	if (llsize (tracking_list) > 0) {
		llhead (tracking_list);
		do {
			portal = (Portal *) llretrieve (tracking_list);
			slice = portal->slice;
			if (slice->slice_xyz == xyz) {
				cx = portal->xold + current_header->centerx;
				cy = portal->yold + current_header->centery;
				tempx += cx;
				tempy += cy;

				/*
				 * XSetForeground (display, mygc, 0xffffffff); mygc	= GetXorGC
				 * (display, window); DrawSymbol (display, window, gc, tempx - 25,
				 * tempy - 25, 50, 50, FILLED_DIAMOND);
				 * 
				 * XSetForeground (display, mygc, 0);
				 */
				DrawSymbol (display, window, gc, tempx, tempy, 50, 50, CROSSHAIR);
				DrawSymbol (display, window, gc,
					    tempx - 25, tempy - 25, 50, 50, DIAMOND);

				/*
				 * XSetForeground (display, mygc, 0xffffffff);
				 */
			}
		} while (llnext (tracking_list) != FAIL);

		if (save) {
			DrawSymbol (display, window, gc, x, y, 50, 50, CROSSHAIR);
			DrawSymbol (display, window, gc, x - 25, y - 25, 50, 50, DIAMOND);
		}
	}
	if (atlas_orient == TRANSVERSE || atlas_orient == CORONAL) {
		ax *= -1;
	}

	/*
	 * If atlas is in Talairach 88 space, do transformation.
	 */

	/*
	 * if (bTalairachFlag) { hd6tot88 (&ax, &ay, &az); }
	 */

	/*
	 * Create string and Motif string.
	 */
	string = strrchr (image1_filename, '/');
	if (string != NULL)
		strcpy (short_filename1, string + 1);
	else if (strlen (image1_filename) > 0)
		strcpy (short_filename1, image1_filename);
	else
		strcpy (short_filename1, "NULL");

	string = strrchr (image2_filename, '/');
	if (string != NULL)
		strcpy (short_filename2, string + 1);
	else if (strlen (image2_filename) > 0)
		strcpy (short_filename2, image2_filename);
	else
		strcpy (short_filename2, "NULL");

	bzero (szBuffer, MAXLINE);
	sprintf (szBuffer, "(%d, %d, %d)     ", ax, ay, az);
	strcpy (szAtlasCoordinate, szBuffer);
	strcat (szBuffer, "          Image 1:  ");
	strcat (szBuffer, short_filename1);
	strcat (szBuffer, "    Image 2:  ");
	strcat (szBuffer, short_filename2);
	xmstring_buffer = XmStringCreate (szBuffer, XmSTRING_DEFAULT_CHARSET);

	/*
	 * Display Motif string on menu bar.
	 */
	n = 0;
	XtSetArg (arglist[n], XmNlabelString, xmstring_buffer);
	n++;
	XtSetValues (widget, arglist, n);
	free (xmstring_buffer);

	if (save && strlen (coordinates_file) > 0) {
		logfp = fopen (coordinates_file, "a");
		if (logfp == (FILE *) NULL) {
			fprintf (stderr, "Error in opening log file %s\n", coordinates_file);
			return;
		}
		fprintf (logfp, "%d      %d      %d\n", ax, ay, az);
		fprintf (logfp, "Atlas File:     %s\n", szAtlasFileName);
		if (strlen (image1_filename) > 0)
			fprintf (logfp, "Image 1: %s\n", image1_filename);
		if (strlen (image2_filename) > 0)
			fprintf (logfp, "Image 2: %s\n", image2_filename);
		fprintf (logfp, "\n");

		fclose (logfp);
	}
}


PUBLIC GC       GetXorGC (display, window)
	Display        *display;
	Window          window;
{
	GC              gc;
	int             screen;
	unsigned long   mask;

	screen = DefaultScreen (display);
	mask = BlackPixel (display, screen) ^ WhitePixel (display, screen);

	gc = XCreateGC (display, window, 0, 0);
	XSetForeground (display, gc, 0xffffffff);
	XSetBackground (display, gc, 0);
	XSetFunction (display, gc, GXxor);
	XSetPlaneMask (display, gc, mask);

	return gc;
}
