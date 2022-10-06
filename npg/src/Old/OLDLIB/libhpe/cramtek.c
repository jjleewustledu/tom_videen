/*$Id: cramtek.c,v 1.21 1994/06/16 21:02:44 ty7777 Exp $*/
/*$Log: cramtek.c,v $
 * Revision 1.21  1994/06/16  21:02:44  ty7777
 * Added libpath for assign_colors () and mapcolor ().
 *
 * Revision 1.20  1994/06/14  19:54:22  ty7777
 * Added the default colormap for PC display.
 *
 * Revision 1.19  1994/06/09  20:15:59  ty7777
 * Deleted debugging messages.
 *
 * Revision 1.18  1994/06/09  19:07:32  ty7777
 * Added processing for display image in the PC environment.
 *
 * Revision 1.17  1994/06/06  19:03:13  ty7777
 * Use pointers to pass integer values.
 *
 * Revision 1.16  1994/06/06  16:49:34  ty7777
 * Introduces x, y, width, and height for ramini ().
 *
 * Revision 1.15  1994/01/21  18:09:41  ty7777
 * Changed the window name for the display window.
 * ./
 *
 * Revision 1.14  1994/01/06  21:19:08  ty7777
 * Added ButtonReleaseMask for capturing events.
 *
 * Revision 1.13  1993/11/24  20:26:52  ty7777
 * Changed xramini ().
 *
 * Revision 1.12  1993/11/24  18:46:52  ty7777
 * Fixed error in ramexit.
 *
 * Revision 1.11  1993/11/24  18:08:57  ty7777
 * Changed ramexit.
 *
 * Revision 1.10  1993/11/24  17:15:52  ty7777
 * Added exec_command to xramexit.
 **/

/*____________________________________________________________________________
 *
 *	File Name:	cramtek.c
 *
 *	Description:	This file contains a list of C functions to be used with
 *			FORTRAN mains for displaying images.
 *
 *	Authors:	Tom Yang and Todd Gamble.
 *
 *	History:	Created by Tom Yang on 01/25/1991.
 *
____________________________________________________________________________*/

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/XWDFile.h>
#define  XK_LATIN1
#include <X11/keysymdef.h>
#include <sys/types.h>
#include <values.h>

#define lowbit(x) ((x) & (~(x) + 1))
#define BEEP_VOLUME 0

extern char    *malloc ();
unsigned        Image_Size ();

static char     rcsid[] = "$Header: /usr/local/src/libpe/RCS/cramtek.c,v 1.21 1994/06/16 21:02:44 ty7777 Exp $";

#define RAMFONT1		"12x24romankana"
#define RAMFONT2		"-adobe-times-bold-r-normal--14-140-75-75-p-77-iso8859-1"
#define RAMTEXTHT1		24
#define RAMTEXTHT2		14
#define OVERLAY_MASK		0xFFFF
#define RAPTUREPLANE		1024
#define REGPLANE		2048
#define FIXPLANE		4095
#define TEMPPLANE		4096
#define GXOVERLAY		GXxor
#define GXSTANDARD		GXcopy
#define MAXLINE			256
#define TOTAL_COLOR_CELLS       256
#define NUM_PET_COLORS		20
#define NUM_REL_COLORS		20
#define NUM_COLORS		NUM_REL_COLORS + NUM_PET_COLORS
#define PET_COLOR_PLANES	12
#define REL_COLOR_PLANES	8
#define REL_MINIMUM		320
#define REL_ZERO		10
#define PCPET_COLOR_START	20
#define PET_COLOR_START		TOTAL_COLOR_CELLS - NUM_PET_COLORS
#define REL_COLOR_START		PET_COLOR_START - NUM_REL_COLORS

static Display *display;
static int      screen;
static GC       gc;
static Window   ramtek;
static u_long   border;
static u_long   background;
static XSetWindowAttributes ramtekA;
static XFontStruct *font_struct1;
static XFontStruct *font_struct2;
static XEvent   event;
static Colormap AppColormap;
static u_long   pc_colors[NUM_COLORS];

#ifdef HP
void            assign_colors ();
void            mapcolor ();
void            xramini (xstart, ystart, width, height, tbl_number, pcflag, libpath)
#else
void            assign_colors_ ();
void            mapcolor_ ();
void            xramini_ (xstart, ystart, width, height, tbl_number, pcflag, libpath)
#endif
	int            *xstart;
	int            *ystart;
	int            *width;
	int            *height;
	int            *tbl_number;
	int            *pcflag;			 /* 1: Displayed on PC */
	char           *libpath;
{
	XSizeHints      myhint;
	int             err;

	display = XOpenDisplay (0);
	if (display == NULL) {
		fprintf (stderr, "Can't create window on %s\n", getenv ("DISPLAY"));
		exit (1);
	}
	screen = DefaultScreen (display);
	border = BlackPixel (display, screen);
	background = BlackPixel (display, screen);

	/* default program-specified window position and size */
	myhint.x = *xstart;
	myhint.y = *ystart;
	myhint.width = *width;
	myhint.height = *height;
	myhint.flags = PPosition | PSize;

	ramtek = XCreateSimpleWindow (display, RootWindow (display, screen),
				      *xstart, *ystart, *width, *height, border, background);

	XSetStandardProperties (display, ramtek, "Washington University Neuroscience PET Group",
				"Ramtek", None, None, 0, &myhint);

	ramtekA.backing_store = WhenMapped;
	ramtekA.background_pixel = background;
	XChangeWindowAttributes (display, ramtek, CWBackingStore | CWBackPixel, &ramtekA);

	/*
	 * Input event selection
	 */
	XSelectInput (display, ramtek, ButtonPressMask | PointerMotionHintMask
		      | KeyPressMask | ButtonMotionMask | ButtonReleaseMask);

	XMapRaised (display, ramtek);

	gc = XCreateGC (display, ramtek, 0, 0);

	font_struct1 = XLoadQueryFont (display, RAMFONT1);
	font_struct2 = XLoadQueryFont (display, RAMFONT2);

	if (*tbl_number != -1 && *tbl_number != 40 && *tbl_number != 41
	    && *tbl_number != 42 && *tbl_number != 46)
		*tbl_number = -1;
#ifdef HP
	if (*pcflag)
		assign_colors (tbl_number, &err, libpath);
	else
		mapcolor (tbl_number, &err, libpath);
#else
	if (*pcflag)
		assign_colors_ (tbl_number, &err, libpath);
	else
		mapcolor_ (tbl_number, &err, libpath);
#endif
}

#ifdef HP
void            xramrst ()
#else
void            xramrst_ ()
#endif
{
	XWindowAttributes attribs;		 /* window attributes */

	XSetFunction (display, gc, GXSTANDARD);
	XSetForeground (display, gc, BlackPixel (display, screen));
	XGetWindowAttributes (display, ramtek, &attribs);
	XFillRectangle (display, ramtek, gc, 0, 0, attribs.width, attribs.height);
	XFlush (display);
}

#ifdef HP
void            xramexit (sys_command, exec_command)
#else
void            xramexit_ (sys_command, exec_command)
#endif
	char           *sys_command;
	int            *exec_command;
{
	int             stat;

	XFreeFont (display, font_struct1);
	XFreeFont (display, font_struct2);
	XFreeGC (display, gc);
	XDestroyWindow (display, ramtek);
	XCloseDisplay (display);

	if (*exec_command == 1) {
		stat = system (sys_command);
	}
	exit (0);
}

/*_______________________________________________________________________________
	Called by:	RAMWRITE in framtex.f
	Author:		Tom Yang
	Date:		09/25/1991
_______________________________________________________________________________*/
#ifdef HP
void            xramwrite (p_x, p_y, p_orientation, p_planes, p_text, p_fcode)
#else
void            xramwrite_ (p_x, p_y, p_orientation, p_planes, p_text, p_fcode)
#endif
	int            *p_x, *p_y;		 /* location to start string */
	int            *p_orientation;		 /* horizonatal=0 or vertical=4 */
	int            *p_planes;		 /* color to write */
	char           *p_text;			 /* text to write, terminated by "~" */
	short          *p_fcode;		 /* 1: font1; 2: font2 */
{
	int             x, y, cx, cy, orientation, planes;
	unsigned int    cwidth, cheight;
	char           *text;
	short           fcode;
	int             i;
	int             length;
	char           *c;
	int             foreground;
	XFontStruct    *font_struct;
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	XCharStruct     overall;
	short           font_height;

	x = *p_x;
	y = *p_y;
	orientation = *p_orientation;
	planes = *p_planes;
	text = (char *) p_text;
	fcode = *p_fcode;

	length = strcspn (text, "~");
	if (length == 0)
		return;

	if (fcode == 1) {
		font_struct = font_struct1;
		font_height = RAMTEXTHT1;
	} else {
		font_struct = font_struct2;
		font_height = RAMTEXTHT2;
	}

	XSetFont (display, gc, font_struct->fid);
	XTextExtents (font_struct, text, length, &direction_hint, &font_ascent,
		      &font_descent, &overall);

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = WhitePixel (display, screen);
		break;
	default:
		fprintf (stderr, "\nxramwrite:unsupported planes parameter: %d\n", planes);
		exit (1);
	}


	cx = x + overall.lbearing;
	cy = y;
	y += overall.ascent;

	switch (orientation) {
	case 0:
		if (planes == FIXPLANE) {
			cwidth = overall.rbearing - overall.lbearing;
			cheight = overall.ascent + overall.descent;
			XClearArea (display, ramtek, cx, cy, cwidth, cheight, False);
		}
		XSetForeground (display, gc, foreground);
		XDrawString (display, ramtek, gc, x, y, text, length);
		break;
	case 4:
		if (planes == FIXPLANE) {
			cwidth = (overall.rbearing - overall.lbearing) / length;
			cheight = length * font_height;
			XClearArea (display, ramtek, cx, cy, cwidth, cheight, False);
		}
		XSetForeground (display, gc, foreground);
		for (i = 0; i < length; i++) {
			XDrawString (display, ramtek, gc, x, y + (font_height * i), text + i, 1);
		}
		break;
	default:
		fprintf (stderr, "xramwrite: unsupported orientation %d\n", orientation);
		exit (1);
	}
	XFlush (display);
}

/*_______________________________________________________________________________
	Called by:	RAMCLEAR in framtek.f
	Author:		Tom Yang
	Date:		09/25/1991
_______________________________________________________________________________*/
#ifdef HP
void            xramclear (p_x, p_y, p_orientation, p_text, p_length, p_fcode)
#else
void            xramclear_ (p_x, p_y, p_orientation, p_text, p_length, p_fcode)
#endif
	int            *p_x, *p_y;		 /* location to start string */
	int            *p_orientation;		 /* horizonatal=0 or vertical=4 */
	char            p_text[];		 /* text to write */
	int            *p_length;
	short          *p_fcode;
{
	int             x, y, orientation;
	unsigned int    width, height;
	char           *text;
	char           *real_text;
	int             length;
	short           fcode;
	XFontStruct    *font_struct;
	int             direction_hint;
	int             font_ascent;
	int             font_descent;
	XCharStruct     overall;
	short           font_height;

	x = *p_x;
	y = *p_y;
	orientation = *p_orientation;
	text = (char *) p_text;
	fcode = *p_fcode;

	length = *p_length + 1;
	if (fcode == 1) {
		font_struct = font_struct1;
		font_height = RAMTEXTHT1;
	} else {
		font_struct = font_struct2;
		font_height = RAMTEXTHT2;
	}
	XSetFont (display, gc, font_struct->fid);
	real_text = (char *) malloc (length);
	strncpy (real_text, text, length);
	XTextExtents (font_struct, real_text, length, &direction_hint, &font_ascent,
		      &font_descent, &overall);

	switch (orientation) {
	case 0:
		x += overall.lbearing;
		width = overall.rbearing - overall.lbearing;
		height = overall.ascent + overall.descent;
		break;
	case 4:
		x += overall.lbearing;
		width = (overall.rbearing - overall.lbearing) / length;
		height = length * font_height;
		break;
	default:
		fprintf (stderr, "xramwrite: unsupported orientation %d\n", orientation);
		exit (1);
	}
	XClearArea (display, ramtek, x, y, width, height, False);
	free (real_text);

	XFlush (display);
}

/*_______________________________________________________________________________
	Name:		clrect_
	Description:	Clear a rectangle area on screen. The unit is pixel.
	Author:		Tom Yang
	Date:		09/25/1991
_______________________________________________________________________________*/
#ifdef HP
void            clrect (p_x, p_y, p_width, p_height)
#else
void            clrect_ (p_x, p_y, p_width, p_height)
#endif
	int            *p_x, *p_y;
	int            *p_width, *p_height;
{
	int             x, y;
	int             width, height;

	x = *p_x;
	y = *p_y;
	width = *p_width;
	height = *p_height;

	XClearArea (display, ramtek, x, y, width, height, False);
}

/*_______________________________________________________________________________
	Called by:	c2colabs.f
	Author:		Tom Yang
	Date:		09/24/1991
_______________________________________________________________________________*/
#ifdef HP
void            mapcolor (tbl_number, err, table_path)
#else
void            mapcolor_ (tbl_number, err, table_path)
#endif
	int            *tbl_number;
	int            *err;
	char           *table_path;
{
	Colormap        cmap;
	Colormap        dcmap;
	FILE           *tblfile;
	Visual         *visual;
	XColor          temp;
	char            red, green, blue;
	char            tblext[5];
	char            tblname[MAXLINE];
	int             i, j;
	int             length;
	int             tblnum;

	length = strcspn (table_path, " ");
	memset (tblname, 0, sizeof (tblname));
	strncpy (tblname, table_path, length);
	strcat (tblname, "/colortbl.");

	tblnum = *tbl_number;
	*err = 0;
	if (tblnum == -1)
		strcat (tblname, "def");
	else {
		fitoa (tblnum, tblext);
		strcat (tblname, tblext);
	}

	visual = XDefaultVisual (display, screen);
	dcmap = XDefaultColormap (display, screen);

	cmap = XCreateColormap (display, ramtek, visual, AllocAll);
	temp.flags = DoRed | DoGreen | DoBlue;

	for (i = 0; i < REL_COLOR_START; i++) {
		temp.pixel = i;
		XQueryColor (display, dcmap, &temp);
		XStoreColor (display, cmap, &temp);
	}

	tblfile = fopen (tblname, "r");
	if (tblfile == NULL) {
		*err = -1;
		perror ("fopen");
		return;
	}
	for (i = 0; i < NUM_COLORS; i++) {
		fscanf (tblfile, "%x", &j);
		blue = j & 0x00F;
		green = (j & 0x0F0) >> 4;
		red = (j & 0xF00) >> 8;

		temp.blue = (int) (((float) blue / 0xF) * 0xFFFF);
		temp.green = (int) (((float) green / 0xF) * 0xFFFF);
		temp.red = (int) (((float) red / 0xF) * 0xFFFF);
		temp.pixel = REL_COLOR_START + i;

		XStoreColor (display, cmap, &temp);
	}
	fclose (tblfile);

	XSetWindowColormap (display, ramtek, cmap);
	AppColormap = cmap;
}

/*_______________________________________________________________________________
	Called by:	c2colabs.f
	Author:		Tom Yang
	Date:		09/24/1991
_______________________________________________________________________________*/
#ifdef HP
void            assign_colors (tbl_number, err, table_path)
#else
void            assign_colors_ (tbl_number, err, table_path)
#endif
	int            *tbl_number;
	int            *err;
	char           *table_path;
{
	Colormap        cmap;
	Colormap        dcmap;
	FILE           *tblfile;
	XColor          temp;
	char           *lpath;
	char            red, green, blue;
	char            tblext[5];
	char            tblname[MAXLINE];
	int             i, j;
	int             length;
	int             tblnum;

	length = strcspn (table_path, " ");
	memset (tblname, 0, sizeof (tblname));
	strncpy (tblname, table_path, length);
	strcat (tblname, "/colortbl.");

	tblnum = *tbl_number;
	*err = 0;
	if (tblnum == -1)
		strcat (tblname, "def");
	else {
		fitoa (tblnum, tblext);
		strcat (tblname, tblext);
	}

	dcmap = XDefaultColormap (display, screen);
	tblfile = fopen (tblname, "r");
	if (tblfile == NULL) {
		*err = -1;
		perror ("fopen");
		return;
	}
	for (i = 0; i < NUM_COLORS; i++) {
		fscanf (tblfile, "%x", &j);
		blue = j & 0x00F;
		green = (j & 0x0F0) >> 4;
		red = (j & 0xF00) >> 8;

		temp.blue = (int) (((float) blue / 0xF) * 0xFFFF);
		temp.green = (int) (((float) green / 0xF) * 0xFFFF);
		temp.red = (int) (((float) red / 0xF) * 0xFFFF);

		if (XAllocColor (display, dcmap, &temp) == 0) {
			fprintf (stderr, "Error in allocating color (%x, %x, %x).\n",
				 blue, green, red);
		}
		pc_colors[i] = temp.pixel;
	}
	fclose (tblfile);

	AppColormap = dcmap;
}

/*_______________________________________________________________________________
	Called by:	RAMIMG in framtek.f
	Author:		Tom Yang
	Date:		09/24/1991
_______________________________________________________________________________*/
#ifdef HP
xramimg (image, p_ydim, p_xdim, p_yst, p_xst, p_scan, type, pcflag, absscale_flag)
#else
xramimg_ (image, p_ydim, p_xdim, p_yst, p_xst, p_scan, type, pcflag, absscale_flag)
#endif
	short          *image;
	int            *p_xdim, *p_ydim;
	int            *p_xst, *p_yst;
	int            *p_scan, *type;
	int            *pcflag;			 /* 1: Displayed on PC */
	char           *absscale_flag;
{
	int             ydim, xdim, yst, xst, scan;
	int             i, j, err, depth, format, bitmap_pad;
	int             ctbl;
	XImage         *imageptr;
	u_char         *imagebuf;
	char            color_index;
	short           raw_pixel_value;

	ydim = *p_ydim;
	xdim = *p_xdim;
	yst = *p_yst;
	xst = *p_xst;
	scan = *p_scan;

	imagebuf = (u_char *) malloc (xdim * ydim);

	for (i = 0; i < xdim; i++) {
		for (j = 0; j < ydim; j++) {
			raw_pixel_value = *(image + (ydim * i) + j);
			if (*absscale_flag) {
				color_index = raw_pixel_value / PET_COLOR_PLANES;
				if (color_index >= NUM_PET_COLORS)
					color_index = NUM_PET_COLORS - 1;
				else if (color_index < 0)
					color_index = 0;
				if (*pcflag) {
					color_index = pc_colors[color_index
								+ PCPET_COLOR_START];
				} else {
					color_index += PET_COLOR_START;
				}
			} else {
				color_index = (raw_pixel_value - REL_MINIMUM)
					/ REL_COLOR_PLANES;
				if (color_index >= NUM_REL_COLORS)
					color_index = NUM_REL_COLORS - 1;
				else if (color_index < 0)
					color_index = REL_ZERO;
				if (*pcflag) {
					color_index = pc_colors[color_index];
				} else {
					color_index += REL_COLOR_START;
				}
			}
			*(imagebuf + (xdim * j) + i) = color_index;
		}
	}

	depth = DefaultDepth (display, screen);
	format = ZPixmap;
	bitmap_pad = 8;

	imageptr = XCreateImage (display, DefaultVisual (display, screen),
				 depth, format, 0, imagebuf, ydim, xdim, bitmap_pad, ydim);
	XSetFunction (display, gc, GXSTANDARD);
	XPutImage (display, ramtek, gc, imageptr, 0, 0, yst, xst, ydim, xdim);

	XDestroyImage (imageptr);
}

#ifdef HP
void            xramcir (p_cx, p_cy, p_radius, p_planes)
#else
void            xramcir_ (p_cx, p_cy, p_radius, p_planes)
#endif
	long           *p_cx, *p_cy;		 /* center of circle */
	long           *p_radius;		 /* radius of circle */
	long           *p_planes;		 /* color */
{
	int             cx, cy, radius, planes;
	int             foreground;

	cx = *p_cx;
	cy = *p_cy;
	radius = *p_radius;
	planes = *p_planes;

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = WhitePixel (display, screen);
		break;
	default:
		fprintf (stderr, "\nxramlin: unsupported planes parameter: %d\n", planes);
		exit (1);
	}

	XSetForeground (display, gc, foreground);
	XSetLineAttributes (display, gc, 1, LineSolid, CapButt, JoinMiter);
	XDrawArc (display, ramtek, gc, cx - radius, cy - radius, cx + radius, cy + radius);
	XFlush (display);
}

#ifdef HP
void            xramlin (p_x1, p_y1, p_x2, p_y2, p_planes)
#else
void            xramlin_ (p_x1, p_y1, p_x2, p_y2, p_planes)
#endif
	long           *p_x1, *p_y1;		 /* first point on line */
	long           *p_x2, *p_y2;		 /* last point on line */
	long           *p_planes;		 /* color */
{
	int             x1, y1, x2, y2, planes;
	int             foreground;

	x1 = *p_x1;
	y1 = *p_y1;
	x2 = *p_x2;
	y2 = *p_y2;
	planes = *p_planes;

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = WhitePixel (display, screen);
		break;
	default:
		fprintf (stderr, "\nxramlin: unsupported planes parameter: %d\n", planes);
		exit (1);
	}

	XSetForeground (display, gc, foreground);
	XSetLineAttributes (display, gc, 1, LineSolid, CapButt, JoinMiter);
	XDrawLine (display, ramtek, gc, x1, y1, x2, y2);
	XFlush (display);
}

#ifdef HP
void            xrampoint (p_x, p_y, p_planes)
#else
void            xrampoint_ (p_x, p_y, p_planes)
#endif
	long           *p_x, *p_y;		 /* point to draw */
	long           *p_planes;		 /* color */
{
	int             x, y, planes;
	int             foreground;

	x = *p_x;
	y = *p_y;
	planes = *p_planes;

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = WhitePixel (display, screen);
		break;
	default:
		fprintf (stderr, "\nxrampoint: unsupported planes parameter: %d\n", planes);
		exit (1);
	}

	XSetForeground (display, gc, foreground);
	XDrawPoint (display, ramtek, gc, x, y);
	XFlush (display);
}

#ifdef HP
void            xramerase (p_x1, p_y1, p_x2, p_y2, p_planes)
#else
void            xramerase_ (p_x1, p_y1, p_x2, p_y2, p_planes)
#endif
	long           *p_x1, *p_y1;		 /* first point on line */
	long           *p_x2, *p_y2;		 /* last point on line */
	long           *p_planes;		 /* color */
{
	int             x1, y1, x2, y2, planes;
	int             foreground;

	x1 = *p_x1;
	y1 = *p_y1;
	x2 = *p_x2;
	y2 = *p_y2;
	planes = *p_planes;

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = BlackPixel (display, screen);
		break;
	default:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = BlackPixel (display, screen);
		break;
	}

	XSetForeground (display, gc, foreground);
	XFillRectangle (display, ramtek, gc, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
	XFlush (display);
}

#ifdef HP
void            xrambers (xstart, ystart, width, height, p_planes)
#else
void            xrambers_ (xstart, ystart, width, height, p_planes)
#endif
	int            *xstart;
	int            *ystart;
	int            *width;
	int            *height;
	long           *p_planes;		 /* color */
{
	int             planes;
	int             foreground;

	planes = *p_planes;

	switch (planes) {
	case REGPLANE:
	case TEMPPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = OVERLAY_MASK;
		break;
	case RAPTUREPLANE:
		XSetFunction (display, gc, GXOVERLAY);
		foreground = WhitePixel (display, screen);
		break;
	case FIXPLANE:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = BlackPixel (display, screen);
		break;
	default:
		XSetFunction (display, gc, GXSTANDARD);
		foreground = BlackPixel (display, screen);
		break;
	}

	XSetForeground (display, gc, foreground);
	XFillRectangle (display, ramtek, gc, *xstart, *ystart, *width, *height);
	XFlush (display);
}

#ifdef HP
xgetpad (x, y, flag)
#else
xgetpad_ (x, y, flag)
#endif
	int            *x;
	int            *y;
	int             flag[4];
{
	Window          child;
	Window          root;
	int             i;
	int             root_x;
	int             root_y;
	int             win_x;
	int             win_y;
	u_int           keys_buttons;
	u_int           test;

	for (i = 0; i < 4; i++)
		flag[i] = 0;

	/*
	 * Read the next event
	 */
	XNextEvent (display, &event);

	switch (event.type) {
	case ButtonPress:
		*x = event.xbutton.x;
		*y = event.xbutton.y;
		flag[0] = 1;
		flag[3] = 1;
		break;
	case MotionNotify:
		XQueryPointer (display, ramtek, &root, &child, &root_x, &root_y,
			       &win_x, &win_y, &keys_buttons);
		*x = win_x;
		*y = win_y;
		test = keys_buttons & Button1Mask;
		if ((keys_buttons == test) && keys_buttons != 0) {
			flag[1] = 1;
			flag[3] = 1;
		}
		break;
	case ButtonRelease:
		*x = event.xbutton.x;
		*y = event.xbutton.y;
		flag[2] = 1;
		break;
	}
}

#ifdef HP
void            xwdchck (flag)
#else
void            xwdchck_ (flag)
#endif
	char           *flag;
{
	int             i;
	char            c;

	/* Read the next event */
	XNextEvent (display, &event);

	switch (event.type) {
	case KeyPress:
		i = XLookupString (&event.xkey, &c, 1, NULL, NULL);
		if ((i == 1) && ((c == 's') || (c == 'S'))) {
			*flag = True;
		} else {
			*flag = False;
		}
	}
}

/*________________________________________________________________________
 * windump_: dump a window to a file which must already be open for
 *              writting.
 ________________________________________________________________________*/

#ifdef HP
void            windump (region_flag, rx, ry, rwidth, rheight, dump_file_name, length)
#else
void            windump_ (region_flag, rx, ry, rwidth, rheight, dump_file_name, length)
#endif
	Bool           *region_flag;
	int            *rx, *ry;
	unsigned int   *rwidth, *rheight;
	char           *dump_file_name;
	int            *length;
{
	FILE           *out;
	int             format = ZPixmap;
	Bool            nobdrs = True;
	Bool            standard_out = True;
	Bool            debug = False;
	long            add_pixel_value = 0;
	unsigned long   swaptest = 1;
	XColor         *colors;
	unsigned        buffer_size;
	int             win_name_size;
	int             header_size;
	int             ncolors, i;
	char           *win_name;
	Bool            got_win_name;
	XWindowAttributes win_info;
	XImage         *image;
	int             absx, absy, x, y;
	unsigned        width, height;
	int             dwidth, dheight;
	int             bw;
	Window          dummywin;
	XWDFileHeader   header;
	char            dump_name[40];

	strncpy (dump_name, dump_file_name, *length);
	dump_name[*length] = '\0';

	/*
	 * Inform the user not to alter the screen.
	 */
	Beep (display);

	if ((out = fopen (dump_name, "w")) == NULL) {
		printf ("Cannot open file %s\n", dump_name);
		return;
	}

	/*
	 * Get the parameters of the window being dumped.
	 */
	if (debug)
		outl ("xwd: Getting target window information.\n");
	if (!XGetWindowAttributes (display, ramtek, &win_info))
		Error ("Can't get target window attributes.");

	/*
	 * Handle any frame window.
	 */
	if (!XTranslateCoordinates (display, ramtek, RootWindow (display, screen), 0, 0,
				    &absx, &absy, &dummywin)) {
		fprintf (stderr,
			 "unable to translate window coordinates (%d,%d)\n",
			 absx, absy);
		exit (1);
	}
	win_info.x = absx;
	win_info.y = absy;
	width = win_info.width;
	height = win_info.height;
	bw = 0;

	if (!nobdrs) {
		bw = win_info.border_width;
		absx -= bw;
		absy -= bw;
		width += (2 * bw);
		height += (2 * bw);
	}
	dwidth = DisplayWidth (display, screen);
	dheight = DisplayHeight (display, screen);

	/*
	 * Clip to window.
	 */
	if (absx < 0) {
		width += absx;
		absx = 0;
	}
	if (absy < 0) {
		height += absy;
		absy = 0;
	}
	if (absx + width > dwidth)
		width = dwidth - absx;
	if (absy + height > dheight)
		height = dheight - absy;

	XFetchName (display, ramtek, &win_name);
	if (!win_name || !win_name[0]) {
		win_name = "xwdump";
		got_win_name = False;
	} else {
		got_win_name = True;
	}

	/*
	 * sizeof(char) is included for the null string terminator.
	 */
	win_name_size = strlen (win_name) + sizeof (char);

	/*
	 * Snarf the pixmap with XGetImage.
	 */

	if (!(*region_flag)) {
		x = absx - win_info.x;
		y = absy - win_info.y;
	} else {
		x = *rx;
		y = *ry;
		width = *rwidth;
		height = *rheight;
	}

	image = XGetImage (display, ramtek, x, y, width, height, AllPlanes, format);
	if (!image) {
		fprintf (stderr, "unable to get image at %dx%d+%d+%d\n",
			 width, height, x, y);
		exit (1);
	}
	if (add_pixel_value != 0)
		XAddPixel (image, add_pixel_value);

	/*
	 * Determine the pixmap size.
	 */
	buffer_size = Image_Size (image);

	if (debug)
		outl ("xwd: Getting Colors.\n");

	ncolors = getcolors_ (display, &win_info, &colors);

	/*
	 * Inform the user that the image has been retrieved.
	 */
	XBell (display, BEEP_VOLUME);
	XBell (display, BEEP_VOLUME);
	XFlush (display);

	/*
	 * Calculate header size.
	 */
	if (debug)
		outl ("xwd: Calculating header size.\n");
	header_size = sizeof (header) + win_name_size;

	/*
	 * Write out header information.
	 */
	if (debug)
		outl ("xwd: Constructing and dumping file header.\n");
	header.header_size = (CARD32) header_size;
	header.file_version = (CARD32) XWD_FILE_VERSION;
	header.pixmap_format = (CARD32) format;
	header.pixmap_depth = (CARD32) image->depth;
	header.pixmap_width = (CARD32) image->width;
	header.pixmap_height = (CARD32) image->height;
	header.xoffset = (CARD32) image->xoffset;
	header.byte_order = (CARD32) image->byte_order;
	header.bitmap_unit = (CARD32) image->bitmap_unit;
	header.bitmap_bit_order = (CARD32) image->bitmap_bit_order;
	header.bitmap_pad = (CARD32) image->bitmap_pad;
	header.bits_per_pixel = (CARD32) image->bits_per_pixel;
	header.bytes_per_line = (CARD32) image->bytes_per_line;
	header.visual_class = (CARD32) win_info.visual->class;
	header.red_mask = (CARD32) win_info.visual->red_mask;
	header.green_mask = (CARD32) win_info.visual->green_mask;
	header.blue_mask = (CARD32) win_info.visual->blue_mask;
	header.bits_per_rgb = (CARD32) win_info.visual->bits_per_rgb;
	header.colormap_entries = (CARD32) win_info.visual->map_entries;
	header.ncolors = ncolors;
	header.window_width = (CARD32) win_info.width;
	header.window_height = (CARD32) win_info.height;
	header.window_x = absx;
	header.window_y = absy;
	header.window_bdrwidth = (CARD32) win_info.border_width;

	if (*(char *) &swaptest) {
		_swaplong ((char *) &header, sizeof (header));
		for (i = 0; i < ncolors; i++) {
			_swaplong ((char *) &colors[i].pixel, sizeof (long));
			_swapshort ((char *) &colors[i].red, 3 * sizeof (short));
		}
	}
	(void) fwrite ((char *) &header, sizeof (header), 1, out);
	(void) fwrite (win_name, win_name_size, 1, out);

	/*
	 * Write out the color maps, if any
	 */

	if (debug)
		outl ("xwd: Dumping %d colors.\n", ncolors);
	(void) fwrite ((char *) colors, sizeof (XColor), ncolors, out);

	/*
	 * Write out the buffer.
	 */
	if (debug)
		outl ("xwd: Dumping pixmap.  bufsize=%d\n", buffer_size);

	/*
	 * This copying of the bit stream (data) to a file is to be replaced by an Xlib call which
	 * hasn't been written yet.  It is not clear what other functions of xwd will be taken over
	 * by this (as yet) non-existant X function.
	 */
	(void) fwrite (image->data, (int) buffer_size, 1, out);

	fclose (out);

	/*
	 * free the color buffer.
	 */

	if (debug && ncolors > 0)
		outl ("xwd: Freeing colors.\n");
	if (ncolors > 0)
		free (colors);

	/*
	 * Free window name string.
	 */
	if (debug)
		outl ("xwd: Freeing window name string.\n");
	if (got_win_name)
		XFree (win_name);

	/*
	 * Free image
	 */
	XDestroyImage (image);
}


/*
 * Get the XColors of all pixels in image - returns # of colors
 */
#ifdef HP
int             getcolors (display, win_info, colors)
#else
int             getcolors_ (display, win_info, colors)
#endif
	Display        *display;
	XWindowAttributes *win_info;
	XColor        **colors;
{
	int             i, ncolors;
	unsigned long   red, green, blue, red1, green1, blue1;

	if (!win_info->colormap)
		return (0);

	if (win_info->visual->class == TrueColor)
		return (0);			 /* colormap is not needed */

	ncolors = win_info->visual->map_entries;
	if (!(*colors = (XColor *) malloc (sizeof (XColor) * ncolors)))
		Error ("Out of memory!");

	if (win_info->visual->class == DirectColor) {

		/*
		 * Pixel red, green, blue, red1, green1, blue1;
		 */

		red = green = blue = 0;
		red1 = lowbit (win_info->visual->red_mask);
		green1 = lowbit (win_info->visual->green_mask);
		blue1 = lowbit (win_info->visual->blue_mask);
		for (i = 0; i < ncolors; i++) {
			(*colors)[i].pixel = red | green | blue;
			(*colors)[i].pad = 0;

			red += red1;
			if (red > win_info->visual->red_mask)
				red = 0;

			green += green1;
			if (green > win_info->visual->green_mask)
				green = 0;

			blue += blue1;
			if (blue > win_info->visual->blue_mask)
				blue = 0;
		}
	} else {
		for (i = 0; i < ncolors; i++) {
			(*colors)[i].pixel = i;
			(*colors)[i].pad = 0;
		}
	}

	XQueryColors (display, AppColormap, *colors, ncolors);

	return (ncolors);
}

/*__________________________________________________________________________
 * outl: a debugging routine.  Flushes stdout then prints a message on stderr
 *       and flushes stderr.  Used to print messages when past certain points
 *       in code so we can tell where we are.  Outl may be invoked like
 *       printf with up to 7 arguments.
___________________________________________________________________________*/
outl (msg, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
	char           *msg;
	char           *arg0, *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
{
	fflush (stdout);
	fprintf (stderr, msg, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
	fprintf (stderr, "\n");
	fflush (stderr);
}

/*
 * Beep: Routine to beep the display.
 */
Beep (display)
	Display        *display;
{
	XBell (display, -50);
}

unsigned        Image_Size (image)
	XImage         *image;
{
	if (image->format != ZPixmap)
		return (image->bytes_per_line * image->height * image->depth);

	return ((unsigned) image->bytes_per_line * image->height);
}

Error (string)
	char           *string;
{
	fprintf (stderr, "xwud: Error => %s\n", string);
	exit (1);
}

_swapshort (bp, n)
	register char  *bp;
	register unsigned n;
{
	register char   c;
	register char  *ep = bp + n;

	while (bp < ep) {
		c = *bp;
		*bp = *(bp + 1);
		bp++;
		*bp++ = c;
	}
}

_swaplong (bp, n)
	register char  *bp;
	register unsigned n;
{
	register char   c;
	register char  *ep = bp + n;
	register char  *sp;

	while (bp < ep) {
		sp = bp + 3;
		c = *sp;
		*sp = *bp;
		*bp++ = c;
		sp = bp + 1;
		c = *sp;
		*sp = *bp;
		*bp++ = c;
		bp += 2;
	}
}
