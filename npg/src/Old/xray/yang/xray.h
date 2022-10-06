/*$Id: xray.h,v 1.27 1996/07/10 14:55:30 yang Exp $*/
/*$Log: xray.h,v $
 * Revision 1.27  1996/07/10  14:55:30  yang
 * Modified ECAT961_INC.
 *
 * Revision 1.26  1996/05/03  21:44:29  ty7777
 * Updated to rearrange 961 PET data.
 *
 * Revision 1.25  1995/11/27  20:46:02  ty7777
 * Added constant for 953B offset set on October 19, 1995.
 *
 * Revision 1.24  1995/06/27  16:42:07  ty7777
 * Added the HR961 offset.
 *
 * Revision 1.23  1995/02/08  18:15:56  ty7777
 * Added FIRST_SCAN_AFTER_OCT94.
 *
 * Revision 1.22  1994/12/22  20:10:03  ty7777
 * Updated to regenerate PETT6 .sxr files.
 *
 * Revision 1.21  1994/11/02  15:31:31  ty7777
 * After making new ecat offset work.
 *
 * Revision 1.20  1994/11/01  20:39:18  ty7777
 * Added ECAT_MAG_FACTOR_AFTER_OCT.
 *
 * Revision 1.19  1994/09/20  16:01:40  ty7777
 * Updated for peakslice processing.
 *
 * Revision 1.18  1994/09/02  21:28:45  ty7777
 * Added ACPC_FONT.
 *
 * Revision 1.17  1994/09/02  19:00:48  ty7777
 * Update to incorporated new algorithm for setting the reference slice.
 *
 * Revision 1.16  1994/09/01  21:04:37  ty7777
 * Updated for new algorithm.
 *
 * Revision 1.15  1993/12/20  21:27:28  ty7777
 * Added recompute to GRAPHICS_DATA.
 *
 * Revision 1.14  1993/10/06  19:01:37  ty7777
 * more ENCODED_NUM_SLICES to petutil.h.
 *
 * Revision 1.13  1993/09/14  16:18:05  ty7777
 * Some constants are moved to petutil.h.
 *
 * Revision 1.12  1993/09/10  14:48:56  ty7777
 * Move TYPE to XRAY_TYPE in /usr/local/include/petutil/petutil.h.
 *
 * Revision 1.11  1993/09/09  20:22:43  ty7777
 * Moved DAY, MONTH, YEAR to /usr/local/include/petutil/petutil.h.
 *
 * Revision 1.10  1993/05/06  14:55:52  ty7777
 * Before making print work.
 *
 * Revision 1.9  1993/04/28  21:15:56  ty7777
 * Move TRANSMISSION_ECAT, EMISSION_ECAT, etc to /usr/local/include/petutil/petutil.h.
 *
 * Revision 1.8  1993/04/21  16:35:21  ty7777
 * Moved SXR_OUTPUT to petutil.h.
 *
 * Revision 1.7  1993/02/19  16:50:57  ty7777
 * Before adding film position, scan position, and offset.
 *
 * Revision 1.6  1993/02/17  20:41:46  ty7777
 * Before changing xray slices lengths computation order.
 *
 * Revision 1.5  1993/02/15  20:35:36  ty7777
 * Before adding lines for xray slice lines.
 *
 * Revision 1.4  1993/02/09  18:23:32  ty7777
 * Release checking in.
 *
 * Revision 1.3  1993/01/27  22:07:40  ty7777
 * Adding trim, encoded, etc to xray.h.
 *
 * Revision 1.2  1993/01/27  20:44:52  ty7777
 * After adding PET image width and skull X ray width.
 *
 * Revision 1.1  1993/01/26  21:01:10  ty7777
 * Initial revision
 **/

/*____________________________________________________________________________	
	xray.h

	Description:
		Header file for xray. Includes description of X ray data, constants,
		and global variables. Also includes necessary X and application 
		headers. 

	Author:
		Tom Yang 
		11/16/92

	History:
		Created by TY 11/16/92.
_____________________________________________________________________________*/	

#ifndef _XRAY_H_INCLUDED
#define _XRAY_H_INCLUDED

#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/SelectioB.h>
#include <Xm/FileSB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MessageB.h>
#include <Xm/CascadeB.h>
#include <Xm/Command.h>
#include <X11/cursorfont.h>
#include <petutil/winutil.h>


/* 
 * constants 
 */
#define	ACPC_PERCENTAGE			0.21	/* factor defining AC-PC line from Zmax */ 
#define	ECAT_PIXEL_SIZE			0.260777	/* ECAT image pixel size, cm / pixel */ 
#define	PETT6_SCALE			4.0	/* magnification factor for PETT6 display */ 
#define	ECAT_SCALE			2.0	/* magnification factor for ECAT display */ 
#define	ECAT961_SCALE			1.0	/* mag. factor for ECAT 961 display */ 
#define	PETT6_XSTART			120	/* PETT6 display x start */ 
#define	PETT6_YSTART			300	/* PETT6 display y start */ 
#define	ECAT_XSTART			80	/* ECAT display x start */ 
#define	ECAT_YSTART			100	/* ECAT display y start */ 
#define	ECAT961_XSTART			60	/* ECAT 961 display x start */ 
#define	ECAT961_YSTART			75	/* ECAT 961 display y start */ 
#define	PETT6_XINC			180	/* PETT6 display x increment */ 
#define	PETT6_YINC			200	/* PETT6 display y increment */ 
#define	ECAT_XINC			160	/* ECAT display x increment */ 
#define	ECAT_YINC			200	/* ECAT display y increment */ 
#define	ECAT961_XINC			120	/* ECAT 961 display x increment */ 
#define	ECAT961_YINC			120	/* ECAT 961 display y increment */ 
#define	OLDDEST_HIGH			987	/* maximum number for olddest group */ 
#define	OLDDEST_MAG_FACTOR		0.93	/* patient id < 987 */ 
#define	OLD_HIGH			1575	/* maximum number for old group */ 
#define	OLD_MAG_FACTOR			0.94 	/* patient id < 1575 && id >= 987 or old skull X ray */ 
#define	NEW_MAG_FACTOR			0.945 	/* mag. factor with patient id >= 1575 or new sxr */ 
#define	INCORRECT_DELTAZ_LOW		1611	/* incorrect_delta_z for patient id > this number */ 
#define	ECAT_MAG_FACTOR			0.96	/* ECAT magnification factor */ 
#define	ECAT_MAG_FACTOR_AFTER_OCT94	0.9325	/* ECAT magnification factor after Oct., 1994 */ 
#define	ECAT961_MAG_FACTOR		0.923	/* ECAT magnification factor for the 961 scanner */ 
#define	FIRST_SCAN_AFTER_OCT94		2490	/* first scan id after Oct., 1994 */ 
#define	FIRST_SCAN_AFTER_OCT95		2678	/* first scan id after October 19, 1995 */ 
#define	RASPIXEL_SIZE			0.0254	/* raster image pixel size, cm / pixel */ 
#define	PETT6_NUM_SLICES		7	/* PETT6 number of slices */ 
#define CROSSHAIR_SIZE			30	/* in pixels */
#define DISPLAY_MARGIN_WIDTH		100
#define DISPLAY_MARGIN_HEIGHT		100
#define DRAW_AREA_WIDTH			1600	/* dimension of screen */
#define DRAW_AREA_HEIGHT		1200	
#define MAIN_WINDOW_WIDTH		1152	/* dimension of showed screen */
#define MAIN_WINDOW_HEIGHT		870	/* dimension of showed screen */
#define CX_IMAGE			64.5	
#define CY_IMAGE			64.5	
#define BOUNDING_LINE_LENGTH		50
#define PET_LABEL_FONT	"-adobe-times-bold-r-normal--12-120-75-75-p-67-iso8859-1" /* default font for drawing text */
#define ACPC_FONT			"12x24romankana"
#define BITS_PER_HEX			4	
#define PS_ROW_LEN			80	
#define MESSAGE_NUM			7		
#define NUM_BOUNDARY_SLICES		3	/* number of boundary slices not used as a reference slice */


/* 
 * application defined structures 
 */
typedef struct 
{
	POINT2D	glabella;
	POINT2D	inion;
	POINT2D	slice_lpoint;
	POINT2D	slice_rpoint;
	POINT2D	left_ear_marker;
	POINT2D	right_ear_marker;
} DEFINING_POINTS;

typedef struct 
{
	POINT2D	glabella;
	POINT2D	inion;
	POINT2D	slice_anterior;
	POINT2D	slice_posterior;
	POINT2D	acpc_anterior;
	POINT2D	acpc_posterior;
	POINT2D	acpc_center;
	POINT2D	zvertex;
	POINT2D	vertex_zmax;
	POINT2D	ref_normalref;
	POINT2D	*slices_ap;
} DRAWING_POINTS;

struct rasterfile 
{
	long	ras_magic;	/* A 32-bit magic number identifies the file type */
#define	RAS_MAGIC	0x59a66a95	
	long	ras_width;	/* image width */
	long	ras_height;	/* image height */
	long	ras_depth;	/* bits for a pixel */
	long	ras_length;	/* image size in bytes */
	long	ras_type;	/* raster image type */
#define RT_OLD		0	/* OLd raw pixrect image in 68000 byte order */
#define RT_STANDARD	1	/* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED	2	/* Run-length compression of bytes */
#define RT_FORMAT_RGB	3	/* XRGB or RGB instead of XBGR or BGR */
#define RT_FORMAT_TIFF	4	/* tiff <-> standard rasterfile */
#define RT_FORMAT_IFF	5	/* iff (TAAC format) <-> standard rasterfile */
#define RT_EXPERIMENTAL 0xffff	/* Reserved for testing */
	long	ras_maptype;
#define RMT_NONE	0
#define RMT_EQUAL_RGB	1
#define RMT_RAW		2
	long	ras_maplength;
};

typedef struct 
{
	Widget		*frameParameters;
	Widget		*labelXRayFile;
	Widget		*textMagnificationFactor;
	Widget		*textFilmPosition;
	Widget		*textScanPosition;
	Widget		*textOffset;
	Widget		*labelPetFile;
	Widget		*textPixelSize;
	Widget		*textSliceSpacing;
	Widget		*textReferenceSlice;
	Widget		*textNumberOfSlices;
	Widget		*textThreshold;
	SXR_OUTPUT	*sxr_output;
	char		*petfile;
	char		petimagetype;		
	char		trim;		
	int		pettnum;	
	BOOLEAN		encoded;
	float		ecat_offset;
	llist		petlist;
} PARAMETER_MENU;			/* structure for parameter menu */

typedef struct 
{
	Display		*display;
	Window		window;
	GC		gc;
} DRAW_DATA;

typedef enum 
{
	NoState,
	GlabellaState,
	InionState,
	LeftRefPointState,
	RightRefPointState,
	LeftEarMarkerState,
	RightEarMarkerState,
} PointerStatus;

typedef struct 
{
	DRAW_DATA	*draw_data;	
	Cursor		cursor;
	Pixel		xray_color;
	int		cols;
	int		rows;
	short int	*xray_tracing;
	Widget		message;
	XmString	*xm_message;
} XRAY_DATA;

typedef struct 
{
	DRAW_DATA	*draw_data;
	GC		xorgc;
	BOOLEAN		button_down_first;
	int		start_x;
	int		start_y;
	int		last_x;
	int		last_y;
	DEFINING_POINTS	*defining_points;
	DRAWING_POINTS	*drawing_points;
	int		cols;
	int		rows;
	short int	*xray_tracing;
	SXR_OUTPUT	*sxr_output;
	Widget		message;
	Widget		recompute;
	XmString	*xm_message;
	Pixel		xray_color;
	Pixel		marker_color;
	Pixel		pet_color;
} GRAPHICS_DATA;

typedef struct 
{
	char		node;
	PARAMETER_MENU	*parameter_menu;
	GRAPHICS_DATA	*graphics_data;
} PARAMETER_DATA;			/* structure for parameter data */

typedef struct 
{
	DRAW_DATA	*draw_data;
	Widget		dialog;
} DATA_FILE_SELECTION;

typedef struct 
{
	DRAW_DATA	*draw_data;
	Cursor		cursor;
	DRAWING_POINTS	*drawing_points;
	int		cols;
	int		rows;
	short int	*xray_tracing;
	char		*postscript_file;
} PRINT_DATA;

typedef struct 
{
	char		*sxr_name;
	char		*petfile;
	char		*sxr_root;
	BOOLEAN		incorrect_delta_z;
	SXR_OUTPUT	*sxr_output;
} SAVESXR_DATA;

typedef struct 
{
	DRAW_DATA	*draw_data;
	Pixel		pet_color;
	Pixel		peak_color;
	Pixel		xray_color;
	int		pettnum;	/* PET number */
	SXR_OUTPUT	*sxr_output;
	llist		petlist;
	Widget		*dialog;
	Widget		*text_peak_slice;
} DISPLAY_PET_DATA;

typedef struct 
{
	char			node;
	PARAMETER_MENU		*parameter_menu;
	DISPLAY_PET_DATA	*display_pet_data;
} PEAK_SLICE_CHANGED_DATA;

typedef struct 
{
	short int	*xray_tracing;
	float		*rlslicedim;		/* Right-Left dimension of slices (cm) in Xray*/
	float		*rlpetdim;		/* Right-Left dimension of PET slices (pixel) */
	float		*rlcenter;		/* center of the right-left axis of PET slices (pixel no.) */
	float		*vertical_petdim;	/* Top-Bottom dimension of PET slices (pixel) */
	XmString	*xm_message;
	llist		petlist;
} QUIT_DATA;


/* 
 * function prototypes 
 */
PUBLIC GC	CreateXorGC ();
PUBLIC void	DoPseudo ();
PUBLIC int	GetXColors ();
PUBLIC int	GetMagnificationFactor ();
PUBLIC void	GetRotationAngles ();
PUBLIC void	GetXRayParameters ();
PUBLIC void	PetColormap ();
PUBLIC void	ReadRaster ();
PUBLIC int	SetParameters ();
PUBLIC void	WindowDump ();
PUBLIC void	interp ();


/* 
 * global variables 
 */
PUBLIC PointerStatus	cPointerState;

#endif /* _XRAY_H_INCLUDED */

