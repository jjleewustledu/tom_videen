/*$Id: xwd.c,v 1.3 1994/09/06 22:05:33 ty7777 Exp tom $*/
/*$Log: xwd.c,v $
 * Revision 1.3  1994/09/06  22:05:33  ty7777
 * Modified the border width.
 *
 * Revision 1.2  1994/09/02  19:02:18  ty7777
 * Updated to incorporated new algorithm for setting the default reference slice.
 *
 * Revision 1.3  1994/08/30  14:38:01  ty7777
 * Delected declaration for malloc ().
 **/
/*____________________________________________________________________________
	xwd.h

	Copyright:
		1987 Massachusetts Institute of Technology.

	Description:
		Functions for window dump.

	Author:
		Tony Della Fera, DEC
		17-Jun-85

	Modifications:
		Modified GetXColors so that the application defined
		``view25dColormap'' colormap is used to query the colors
		in a rectangular region.
		3/12/92 by Tom Yang.

	Pending:

	Called by:

_____________________________________________________________________________*/

#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XWDFile.h>
#define  XK_LATIN1
#include <X11/keysymdef.h>

#include <TY/xray.h>

#define lowbit(x) ((x) & (~(x) + 1))
#define FEEP_VOLUME 0

unsigned        Image_Size ();

/*________________________________________________________________________
 * WindowDump: dump a window to a file which must already be open for
 *              writting.
 ________________________________________________________________________*/

PUBLIC void     WindowDump (dpy, window, region_flag, rx, ry, rwidth, rheight, dump_file_name)
	Display        *dpy;
	Window          window;
	BOOLEAN         region_flag;
	int             rx, ry;
	unsigned        rwidth, rheight;
	char           *dump_file_name;
{
	FILE           *out;
	int             screen;
	int             format = ZPixmap;
	BOOLEAN         nobdrs = TRUE;
	Bool            standard_out = True;
	BOOLEAN         debug = FALSE;
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

	/*
	 * Inform the user not to alter the screen.
	 */
	Beep (dpy);

	if ((out = fopen (dump_file_name, "w")) == NULL) {
		printf ("Cannot open file %s\n", dump_file_name);
		return;
	}
	screen = DefaultScreen (dpy);

	/*
	 * Get the parameters of the window being dumped.
	 */
	if (debug)
		outl ("xwd: Getting target window information.\n");
	if (!XGetWindowAttributes (dpy, window, &win_info))
		Error ("Can't get target window attributes.");

	/*
	 * Handle any frame window
	 */
	if (!XTranslateCoordinates (dpy, window, RootWindow (dpy, screen), 0, 0,
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
	dwidth = DisplayWidth (dpy, screen);
	dheight = DisplayHeight (dpy, screen);

	/*
	 * clip to window
	 */
	if (absx < 0)
		width += absx, absx = 0;
	if (absy < 0)
		height += absy, absy = 0;
	if (absx + width > dwidth)
		width = dwidth - absx;
	if (absy + height > dheight)
		height = dheight - absy;

	XFetchName (dpy, window, &win_name);
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

	if (!region_flag) {
		x = absx - win_info.x;
		y = absy - win_info.y;

		/*
		 * Get rid of the white border.
		 */
		width -= 37;
	} else {
		x = rx;
		y = ry;
		width = rwidth;
		height = rheight;
	}
	image = XGetImage (dpy, window, x, y, width, height, AllPlanes, format);
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

	ncolors = GetXColors (dpy, &win_info, &colors);

	/*
	 * Inform the user that the image has been retrieved.
	 */
	XBell (dpy, FEEP_VOLUME);
	XBell (dpy, FEEP_VOLUME);
	XFlush (dpy);

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


/*_____________________________________________________________
  Get the XColors of all pixels in image - returns # of colors.
 ____________________________________________________________*/
PUBLIC int      GetXColors (dpy, win_info, colors)
	Display        *dpy;
	XWindowAttributes *win_info;
	XColor        **colors;
{
	int             i, ncolors;
	int             screen;
	Colormap        dcmap;

	screen = DefaultScreen (dpy);
	dcmap = XDefaultColormap (dpy, screen);

	if (!win_info->colormap)
		return (0);

	if (win_info->visual->class == TrueColor)
		return (0);			 /* colormap is not needed */

	ncolors = win_info->visual->map_entries;
	if (!(*colors = (XColor *) malloc (sizeof (XColor) * ncolors)))
		Error ("Out of memory!");

	if (win_info->visual->class == DirectColor) {
		Pixel           red, green, blue, red1, green1, blue1;

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

	XQueryColors (dpy, dcmap, *colors, ncolors);

	return (ncolors);
}

PUBLIC void     DoPseudo (dpy, colormap, ncolors, colors, in_image, out_image)
	Display        *dpy;
	Colormap       *colormap;
	int             ncolors;
	XColor         *colors;
	register XImage *in_image, *out_image;
{
	register int    i, x, y;
	register XColor *color;
	Pixel           pixel;

	for (i = 0; i < ncolors; i++) {
		colors[i].flags = 0;
	}

	for (y = 0; y < in_image->height; y++) {
		for (x = 0; x < in_image->width; x++) {
			color = &colors[XGetPixel (in_image, x, y)];
			if (!color->flags) {
				color->flags = DoRed | DoGreen | DoBlue;
				if (!XAllocColor (dpy, *colormap, color)) {
					*colormap
						= XCopyColormapAndFree (dpy, *colormap);
					XAllocColor (dpy, *colormap, color);
				}
			}
			XPutPixel (out_image, x, y, color->pixel);
		}
	}
}

Do_Direct (dpy, header, colormap, ncolors, colors, in_image, out_image)
	Display        *dpy;
	XWDFileHeader  *header;
	Colormap       *colormap;
	int             ncolors;
	XColor         *colors;
	XImage         *in_image, *out_image;
{
	register int    x, y;
	XColor          color;
	int             direct = 0;
	unsigned long   rmask, gmask, bmask;
	int             rshift = 0, gshift = 0, bshift = 0;
	int             i;
	unsigned long   pix;
	unsigned long  *pixels = (unsigned long *) NULL;

	rmask = header->red_mask;
	while (!(rmask & 1)) {
		rmask >>= 1;
		rshift++;
	}

	gmask = header->green_mask;
	while (!(gmask & 1)) {
		gmask >>= 1;
		gshift++;
	}

	bmask = header->blue_mask;
	while (!(bmask & 1)) {
		bmask >>= 1;
		bshift++;
	}

	if ((ncolors > 0) && (header->visual_class == DirectColor))
		direct = 1;
	if (in_image->depth <= 12) {
		pix = 1 << in_image->depth;
		pixels = (unsigned long *) malloc (sizeof (unsigned long) * pix);
		for (i = 0; i < pix; i++)
			pixels[i] = ~0L;
	}
	color.flags = DoRed | DoGreen | DoBlue;
	for (y = 0; y < in_image->height; y++) {
		for (x = 0; x < in_image->width; x++) {
			pix = XGetPixel (in_image, x, y);
			if (!pixels || ((color.pixel = pixels[pix]) == ~0L)) {
				color.red = (pix >> rshift) & rmask;
				color.green = (pix >> gshift) & gmask;
				color.blue = (pix >> bshift) & bmask;

				if (direct) {
					color.red = colors[color.red].red;
					color.green = colors[color.green].green;
					color.blue = colors[color.blue].blue;
				} else {
					color.red = ((unsigned long) color.red
						     * 65535) / rmask;
					color.green = ((unsigned long) color.green
						       * 65535) / gmask;
					color.blue = ((unsigned long) color.blue
						      * 65535) / bmask;
				}

				if (!XAllocColor (dpy, *colormap, &color)) {
					*colormap = XCopyColormapAndFree (dpy, *colormap);
					XAllocColor (dpy, *colormap, &color);
				}
				if (pixels)
					pixels[pix] = color.pixel;
			}
			XPutPixel (out_image, x, y, color.pixel);
		}
	}
}

Extract_Plane (in_image, out_image, plane)
	register XImage *in_image, *out_image;
	int             plane;
{
	register int    x, y;

	for (y = 0; y < in_image->height; y++)
		for (x = 0; x < in_image->width; x++)
			XPutPixel (out_image, x, y,
				   (XGetPixel (in_image, x, y) >> plane) & 1);
}

Latin1Upper (str)
	unsigned char  *str;
{
	unsigned char   c;

	for (; c = *str; str++) {
		if ((c >= XK_a) && (c <= XK_z))
			*str = c - (XK_a - XK_A);
		else if ((c >= XK_agrave) && (c <= XK_odiaeresis))
			*str = c - (XK_agrave - XK_Agrave);
		else if ((c >= XK_oslash) && (c <= XK_thorn))
			*str = c - (XK_oslash - XK_Ooblique);
	}
}

int             EffectiveSize (vinfo)
	XVisualInfo    *vinfo;
{
	if ((vinfo->class == DirectColor) || (vinfo->class == TrueColor))
		return (vinfo->red_mask | vinfo->green_mask | vinfo->blue_mask) + 1;
	else
		return vinfo->colormap_size;
}

VisualRank (class)
	int             class;
{
	switch (class) {
	case PseudoColor:
		return 5;
	case DirectColor:
		return 4;
	case TrueColor:
		return 3;
	case StaticColor:
		return 2;
	case GrayScale:
		return 1;
	case StaticGray:
		return 0;
	}
}

int             IsGray (dpy, stdmap)
	Display        *dpy;
	XStandardColormap *stdmap;
{
	XColor          color;

	color.pixel = stdmap->base_pixel + (stdmap->red_max * stdmap->red_mult);
	XQueryColor (dpy, stdmap->colormap, &color);
	return (color.green || color.blue);
}

Do_StdGray (dpy, stdmap, ncolors, colors, in_image, out_image)
	Display        *dpy;
	XStandardColormap *stdmap;
	int             ncolors;
	XColor         *colors;
	register XImage *in_image, *out_image;
{
	register int    i, x, y;
	register XColor *color;
	unsigned        lim;

	lim = stdmap->red_max + 1;
	for (i = 0, color = colors; i < ncolors; i++, color++)
		color->pixel = stdmap->base_pixel +
			(((((30L * color->red +
			     59L * color->green +
			     11L * color->blue) / 100)
			   * lim) >> 16) * stdmap->red_mult);

	for (y = 0; y < in_image->height; y++) {
		for (x = 0; x < in_image->width; x++) {
			XPutPixel (out_image, x, y,
				   colors[XGetPixel (in_image, x, y)].pixel);
		}
	}
}

Do_StdCol (dpy, stdmap, ncolors, colors, in_image, out_image)
	Display        *dpy;
	XStandardColormap *stdmap;
	int             ncolors;
	XColor         *colors;
	register XImage *in_image, *out_image;
{
	register int    i, x, y;
	register XColor *color;
	unsigned        limr, limg, limb;

	limr = stdmap->red_max + 1;
	limg = stdmap->green_max + 1;
	limb = stdmap->blue_max + 1;

	for (i = 0, color = colors; i < ncolors; i++, color++)
		color->pixel = stdmap->base_pixel +
			(((color->red * limr) >> 16) * stdmap->red_mult) +
			(((color->green * limg) >> 16) * stdmap->green_mult) +
			(((color->blue * limb) >> 16) * stdmap->blue_mult);

	for (y = 0; y < in_image->height; y++) {
		for (x = 0; x < in_image->width; x++) {
			XPutPixel (out_image, x, y,
				   colors[XGetPixel (in_image, x, y)].pixel);
		}
	}
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

/*_________________________________________________________________
 * Beep: Routine to beep the display.
 ________________________________________________________________*/
Beep (dpy)
	Display        *dpy;
{
	XBell (dpy, 50);
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
