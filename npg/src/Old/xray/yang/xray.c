
/*$Id: xray.c,v 1.12 1995/11/27 20:40:49 ty7777 Exp $*/
/*$Log: xray.c,v $
 * Revision 1.12  1995/11/27  20:40:49  ty7777
 * Updated for new 953B offset.
 *
 * Revision 1.11  1995/06/27  16:44:24  ty7777
 * Added processing for HR961 data.
 *
 * Revision 1.10  1995/02/13  22:03:31  ty7777
 * Added sxr_root to Setparameters ().
 *
 * Revision 1.9  1994/12/22  20:09:42  ty7777
 * Updated to regenerate PETT6 .sxr files.
 *
 * Revision 1.8  1994/11/02  15:36:21  ty7777
 * Added processing for new magnification factor and new ecat offset.
 *
 * Revision 1.7  1994/09/28  16:42:28  ty7777
 * Initialization for xscale.
 *
 * Revision 1.6  1994/09/20  16:00:19  ty7777
 * Updated for processing peakslice.
 *
 * Revision 1.5  1994/09/02  19:01:25  ty7777
 * Updated to incorporated new algorithm for setting the default reference slice.
 *
 * Revision 1.4  1994/08/30  20:15:18  ty7777
 * Updated to X11R5.
 *
 * Revision 1.3  1993/12/20  21:26:21  ty7777
 * Added recompute to GRAPHICS_DATA.
 *
 * Revision 1.2  1993/11/02  15:59:08  ty7777
 * Added makere_color.
 *
 * Revision 1.1  1993/10/07  16:26:37  ty7777
 * Initial revision
 **/

#ifndef lint
static char sccsid [] = "@(#)xray.c	11/16/92  Copyright Neural Pet Group, Washington University 1992";
#endif

static char rcsid [] = "$Header: /home/petsun4/ty7777/src/xray/RCS/xray.c,v 1.12 1995/11/27 20:40:49 ty7777 Exp $";

#include "xray.h"

#define MAX_STRINGS  64
#define MAX_ARGS     64

#include "xray.widgets"
#include "xray.callbacks"

char *messages []	= {"       Message:", 
			   "       Message: Select Glabella", 
			   "       Message: Select Inion", 
			   "       Message: Select Left Reference Slice Defining Point", 
			   "       Message: Select Right Reference Slice Defining Point", 
			   "       Message: Select Left Ear Marker", 
			   "       Message: Select Right Ear Marker"};

char list [] = "*;*;*;*"; /* any frame plane gate bed */

PointerStatus	cPointerState	= NoState;

main (argc, argv)
	int	argc;
	char	**argv;
{
	Arg			arglist [MAX_ARGS];
	BOOLEAN			acquired_after_oct94;
	BOOLEAN			assigned_threshold;
	BOOLEAN			check_delta_z;
	BOOLEAN			encoded;
	BOOLEAN			hrscanner;
	BOOLEAN			incorrect_delta_z;
	Colormap		cmap;                  /* resource id of color map */
	Cursor			define_cursor;
	DEFINING_POINTS		defining_points;
	DISPLAY_PET_DATA	display_pet_data;
	Display			*display;	/* X display */
	DRAW_DATA		draw_data;
	DRAWING_POINTS		drawing_points;
	GC			gcCanvas;       /* gc for canvas */
	GC			xorgc;      	/* gc for inverting */
	DATA_FILE_SELECTION	selectFileData;
	GRAPHICS_DATA		graphics_data;
	PARAMETER_MENU		parameter_menu;
	PARAMETER_DATA		initialize_parameter_data;
	PARAMETER_DATA		recompute_parameter_data;
	PARAMETER_DATA		cancel_parameter_data;
	PEAK_SLICE_CHANGED_DATA	recompute_peak_slice_data;
	PEAK_SLICE_CHANGED_DATA	cancel_peak_slice_data;
	PRINT_DATA		print_data;
	QUIT_DATA		quit_data;
	SAVESXR_DATA		savesxr_data;
	SAVESXR_DATA		savesxr_data_check_no;
	SXR_OUTPUT		sxr_output;
	Window			window;         /* window to display in */
	Window			root_window;         /* window to display in */
	XColor			cursor_bcolor;		/* RGB spec; contains pixel on return */
	XColor			cursor_fcolor;		/* RGB spec; contains pixel on return */
	XColor			marker_color;		/* RGB spec; contains pixel on return */
	XColor			pet_color;		/* RGB spec; contains pixel on return */
	XColor			xray_color;		/* RGB spec; contains pixel on return */
	XmString		xm_messages [MESSAGE_NUM];
	XmString		xm_buffers [MAX_ARGS];
	XRAY_DATA		xray_data;
	XSetWindowAttributes	wattr;		/* window attribute buffer */
	char			*ras_name;
	char			peaktype;
	char			*petfile;
	char			petimagetype;
	char			postscript_file [MAXLINE];
	char			*sxr_name;
	char			*sxr_root;
	char			szBuffer [MAXLINE];
	char			threshold;
	char			trim;
	int			cols, rows;
	int			i;
	int			j;
	int			n;
	int			imgfd;
	int			pettnum;	
	int			screen;		/* X screen */
	int			size;
	short int		num_slices;
	short int		temp;
	short int		*xray_tracing;
	llist			petlist;

	sxr_name		= argv [1];
	ras_name		= argv [2];
	petfile			= argv [3];
	threshold		= atoi (argv [4]);
	peaktype		= atoi (argv [5]);
	trim			= atoi (argv [6]);
	sxr_root		= argv [7];
	assigned_threshold	= atoi (argv [8]);

	/*
	 * Initializations.
	 */
	petlist	= llinit ();

	for (i = 0; i < XtNumber (messages); i++)
	{
		xm_messages [i]	= XmStringCreate (messages [i], XmSTRING_DEFAULT_CHARSET);	
	}

	check_delta_z		= FALSE;
	incorrect_delta_z	= FALSE;

	/*
	 * Read in X ray tracing.
	 */
	temp	= 1;
	ReadRaster (ras_name, &cols, &rows, &temp, TRUE);

	size		= cols * rows;
	xray_tracing	= (short *) malloc (size * sizeof (short));

	ReadRaster (ras_name, &cols, &rows, xray_tracing, FALSE);

	/*
	 * Get image number and determine if image is emission or transmission.
	 */
	imgfd	= open (petfile, O_RDONLY, 0);
	if (imgfd == -1)
	{
		fprintf (stderr, "Can't open %s\n", petfile); 
		exit (FAIL);
	}

	pettnum	= getimage_type (imgfd, petfile, &encoded, &num_slices);

	if (pettnum == ERROR_SCANNER)
	{
		fprintf (stderr, "Unrecongnized scanner type\n"); 
		exit (FAIL);
	}

	close (imgfd);

	/*
	 * Get magnification factor based on patient ID.
	 */
	sxr_output.film_position	= 0.0;
	sxr_output.number_slices	= num_slices;
	sxr_output.peaktype		= peaktype;
	sxr_output.threshold		= threshold;
	sxr_output.peakslice		= PEAK_SLICE_UNDEFINED;
	sxr_output.xscale		= 0.0;
	acquired_after_oct94		= FALSE;
	hrscanner			= FALSE;

	/*
	 * Assign parameters that are constant.
	 */
	if (SetParameters (petfile, sxr_root, pettnum, encoded, &num_slices, &sxr_output,
	&petimagetype, &trim, &acquired_after_oct94, &hrscanner, assigned_threshold) == FAIL)
	{
		fprintf (stderr, "SetParameters Error.\n");
		exit (FAIL);
	}

	if (GetMagnificationFactor (sxr_root, acquired_after_oct94, 
	hrscanner, pettnum, &check_delta_z, &incorrect_delta_z, &sxr_output) == FAIL)
	{
		fprintf (stderr, "Error: in GetMagnificationFactor.\n");
		exit (FAIL);
	}

	/*
	 * Memory allocation.
	 */
	sxr_output.rlslicedim		= (float *) malloc (num_slices * sizeof (float));
	sxr_output.rlpetdim		= (float *) malloc (num_slices * sizeof (float));
	sxr_output.rlcenter		= (float *) malloc (num_slices * sizeof (float));
	sxr_output.vertical_petdim	= (float *) malloc (num_slices * sizeof (float));
	drawing_points.slices_ap	= (POINT2D *) malloc (num_slices * 2 * sizeof (POINT2D));

#include "xray.code"

	/*
	 * Fill the menu structrue.
	 */
	parameter_menu.frameParameters		= &frameParameters;
	parameter_menu.labelXRayFile		= &labelXRayFile;
	parameter_menu.textMagnificationFactor	= &textMagnificationFactor;
	parameter_menu.textFilmPosition		= &textFilmPosition;
	parameter_menu.textScanPosition		= &textScanPosition;
	parameter_menu.textOffset		= &textOffset;
	parameter_menu.labelPetFile		= &labelPetFile;
	parameter_menu.textPixelSize		= &textPixelSize;
	parameter_menu.textSliceSpacing		= &textSliceSpacing;
	parameter_menu.textReferenceSlice	= &textReferenceSlice;
	parameter_menu.textNumberOfSlices	= &textNumberOfSlices;
	parameter_menu.textThreshold		= &textThreshold;
	parameter_menu.petfile			= petfile;
	parameter_menu.petimagetype		= petimagetype;
	parameter_menu.trim			= trim;
	parameter_menu.pettnum			= pettnum;
	parameter_menu.encoded			= encoded;
	parameter_menu.ecat_offset		= sxr_output.scan_position;
	parameter_menu.sxr_output		= &sxr_output;
	parameter_menu.petlist			= petlist;

	XtAddCallback (cascadeParameter, XmNactivateCallback, ShowParmMenuCallback, &parameter_menu);

	XtRealizeWidget (appShell);

	/* 
	 * Get the current display and screen. 
	 */
	display		= XtDisplay (appShell);
	screen		= DefaultScreen (display);
	define_cursor	= XCreateFontCursor (display, XC_crosshair);

        /* 
	 * Get the drawing area window. 
	 */
	root_window	= XtWindow (appShell);
	window		= XtWindow (mainWindow_ww);

	/* 
	 * Set backing store on window. 
	 */
	wattr.backing_store	= WhenMapped;
	XChangeWindowAttributes (display, window, CWBackingStore, &wattr);

	/* 
	 * Set the graphics context. 
	 */
	gcCanvas	= XCreateGC (display, window, 0, 0);
	xorgc		= CreateXorGC (mainWindow_ww);

	/* 
	 * Define foreground color. 
	 */
	cmap			= DefaultColormap (display, DefaultScreen (display));

	xray_color.red		= 65535;
	xray_color.green	= 65535;
	xray_color.blue		= 65535;
	if (XAllocColor (display, cmap, &xray_color) == 0)
	{
		fprintf (stderr, "Cann't allocate color\n");
	} 
	XSetForeground (display, gcCanvas, xray_color.pixel);

	marker_color.red	= 65535;
	marker_color.green	= 0;
	marker_color.blue	= 0;
	if (XAllocColor (display, cmap, &marker_color) == 0)
	{
		fprintf (stderr, "Cann't allocate color\n");
	} 

	pet_color.red	= 0;
	pet_color.green	= 0;
	pet_color.blue	= 65535;
	if (XAllocColor (display, cmap, &pet_color) == 0)
	{
		fprintf (stderr, "Cann't allocate color\n");
	} 

	cursor_fcolor.red	= 0;
	cursor_fcolor.green	= 65535;
	cursor_fcolor.blue	= 0;
	if (XAllocColor (display, cmap, &cursor_fcolor) == 0)
	{
		fprintf (stderr, "Cann't allocate color\n");
	} 

	cursor_bcolor.red	= 0;
	cursor_bcolor.green	= 0;
	cursor_bcolor.blue	= 0;
	if (XAllocColor (display, cmap, &cursor_bcolor) == 0)
	{
		fprintf (stderr, "Cann't allocate color\n");
	} 

	XRecolorCursor (display, define_cursor, &cursor_fcolor, &cursor_bcolor);

	draw_data.display	= display;
	draw_data.window	= window;
	draw_data.gc		= gcCanvas;

	xray_data.draw_data	= &draw_data;
	xray_data.cursor	= define_cursor;
	xray_data.xray_color	= xray_color.pixel;
	xray_data.cols		= cols;
	xray_data.rows		= rows;
	xray_data.xray_tracing	= xray_tracing;
	xray_data.message	= cascadeMessage;
	xray_data.xm_message	= xm_messages;
	XtAddCallback (cascadeXRay, XmNactivateCallback, DisplayXRayCallback, &xray_data);

	/*
	strcpy (postscript_file, sxr_root);
	strcat (postscript_file, ".ps");
	print_data.draw_data		= &draw_data;
	print_data.cursor		= define_cursor;
	print_data.drawing_points	= &drawing_points;
	print_data.cols			= cols;
	print_data.rows			= rows;
	print_data.xray_tracing		= xray_tracing;
	print_data.postscript_file	= postscript_file;
	XtAddCallback (cascadeSaveWinDump, XmNactivateCallback, PrintCallback, &print_data);
	*/

	XtAddCallback (XmFileSelectionBoxGetChild (winDumpFileSelection, 
			XmDIALOG_HELP_BUTTON), XmNactivateCallback, 
			CancelCallback, winDumpFileSelection);

	selectFileData.draw_data	= &draw_data;
	selectFileData.dialog		= winDumpFileSelection;
	XtAddCallback (XmFileSelectionBoxGetChild (winDumpFileSelection, 
			XmDIALOG_CANCEL_BUTTON), XmNactivateCallback, 
			SaveWinDumpCallback, &selectFileData);

	display_pet_data.draw_data		= &draw_data;
	display_pet_data.pet_color		= pet_color.pixel;
	display_pet_data.peak_color		= marker_color.pixel;
	display_pet_data.xray_color		= xray_color.pixel;
	display_pet_data.pettnum		= pettnum;
	display_pet_data.sxr_output		= &sxr_output;
	display_pet_data.petlist		= petlist;
	display_pet_data.dialog			= &framePeakSlice;
	display_pet_data.text_peak_slice	= &textPeakSlice;
	XtAddCallback (cascadePet, XmNactivateCallback, DisplayPetCallback, &display_pet_data);

	savesxr_data.sxr_name		= sxr_name;
	savesxr_data.petfile		= petfile;
	savesxr_data.sxr_root		= sxr_root;
	savesxr_data.incorrect_delta_z	= incorrect_delta_z;
	savesxr_data.sxr_output		= &sxr_output;
	if (!check_delta_z)
	{
		XtAddCallback (cascadeSave, XmNactivateCallback, SaveSXRCallback, &savesxr_data);
	}
	else
	{
		XtAddCallback (cascadeSave, XmNactivateCallback, ShowMenuCallback, checkDeltazQuestion);

		savesxr_data.incorrect_delta_z	= TRUE;
		XtAddCallback (XmMessageBoxGetChild (checkDeltazQuestion, XmDIALOG_OK_BUTTON),
		XmNactivateCallback, SaveSXRCallback, &savesxr_data);

		savesxr_data_check_no.sxr_name		= sxr_name;
		savesxr_data_check_no.petfile		= petfile;
		savesxr_data_check_no.sxr_root		= sxr_root;
		savesxr_data_check_no.incorrect_delta_z	= FALSE;
		savesxr_data_check_no.sxr_output	= &sxr_output;
		XtAddCallback (XmMessageBoxGetChild (checkDeltazQuestion, XmDIALOG_CANCEL_BUTTON),
		XmNactivateCallback, SaveSXRCallback, &savesxr_data_check_no);
	}

	quit_data.xray_tracing		= xray_tracing;
	quit_data.rlslicedim		= sxr_output.rlslicedim;
	quit_data.rlpetdim		= sxr_output.rlpetdim;
	quit_data.rlcenter		= sxr_output.rlcenter;
	quit_data.vertical_petdim	= sxr_output.vertical_petdim;
	quit_data.xm_message		= xm_messages;
	quit_data.petlist		= petlist;
	XtAddCallback (cascadeQuit, XmNactivateCallback, QuitCallback, &quit_data);

	/* 
	 * Add event handlers for mouse button events. 
	 */
	graphics_data.draw_data		= &draw_data;
	graphics_data.xorgc		= xorgc;
	graphics_data.defining_points	= &defining_points;
	graphics_data.drawing_points	= &drawing_points;
	graphics_data.cols		= cols;
	graphics_data.rows		= rows;
	graphics_data.xray_tracing	= xray_tracing;
	graphics_data.sxr_output	= &sxr_output;
	graphics_data.message		= cascadeMessage;
	graphics_data.recompute		= pushParametersRecompute;
	graphics_data.xm_message	= xm_messages;
	graphics_data.xray_color	= xray_color.pixel;
	graphics_data.marker_color	= marker_color.pixel;
	graphics_data.pet_color		= pet_color.pixel;

	initialize_parameter_data.node		= SELECT;
	initialize_parameter_data.parameter_menu	= &parameter_menu;
	initialize_parameter_data.graphics_data	= &graphics_data;
	XtAddCallback (pushParametersInitialize, XmNactivateCallback, 
			SelectParametersCallback, &initialize_parameter_data);

	recompute_parameter_data.node		= REDRAW;
	recompute_parameter_data.parameter_menu	= &parameter_menu;
	recompute_parameter_data.graphics_data	= &graphics_data;
	XtAddCallback (pushParametersRecompute, XmNactivateCallback, 
			SelectParametersCallback, &recompute_parameter_data);

	cancel_parameter_data.node		= CANCEL;
	cancel_parameter_data.parameter_menu	= &parameter_menu;
	cancel_parameter_data.graphics_data	= &graphics_data;
	XtAddCallback (pushParametersCancel, XmNactivateCallback, 
			SelectParametersCallback, &cancel_parameter_data);

	recompute_peak_slice_data.node			= REDRAW;
	recompute_peak_slice_data.parameter_menu	= &parameter_menu;
	recompute_peak_slice_data.display_pet_data	= &display_pet_data;
	XtAddCallback (pushPeakSliceRecompute, XmNactivateCallback, 
			PeakSliceChangedCallback, &recompute_peak_slice_data);

	cancel_peak_slice_data.node		= CANCEL;
	cancel_peak_slice_data.parameter_menu	= &parameter_menu;
	cancel_peak_slice_data.display_pet_data	= &display_pet_data;
	XtAddCallback (pushPeakSliceCancel, XmNactivateCallback, 
			PeakSliceChangedCallback, &cancel_peak_slice_data);

	XtAddEventHandler (mainWindow_ww, ButtonPressMask, FALSE, 
				ButtonDownEventHandler, &graphics_data);
	XtAddEventHandler (mainWindow_ww, ButtonMotionMask, FALSE, 
				ButtonMoveEventHandler, &graphics_data);
	XtAddEventHandler (mainWindow_ww, ButtonReleaseMask, FALSE, 
				ButtonReleaseEventHandler, &graphics_data);

	/* 
	 * Grab the mouse pointer on mouse events in drawing screen. 
	 */
	XGrabButton (XtDisplay (mainWindow_ww), AnyButton, AnyModifier, window, TRUE,
		ButtonPressMask | ButtonMotionMask | ButtonReleaseMask, GrabModeAsync, 
		GrabModeAsync, window, XCreateFontCursor (XtDisplay (mainWindow_ww), XC_fleur));
	XtMainLoop ();
}

