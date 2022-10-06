

;widget ids
wd = {	Widget_IDs,	$
	repeat_action:0, $	; Repeat last action.
	exit:0,		$	; Exit script.
	leader:0,	$	; Lead widget.
	msmooth:0,	$	; Generate horizontal profile.
	color:0,	$	; Generate color table.
	mroitype:0,	$	; Define ROI.
	roinum:0,	$	; Current ROI number.
	vol:0,		$	; Current volume number
	nxtvol:0,	$	; Assign next volume number.
;Display
	draw:-1,		$	; Draw widget images displayed on.
	drawbase:-1,		$	; Draw widget images displayed on.
	loadct:0,	$	; Call color table widget.
	cine:0,		$	; Display in cine mode.
	zoomin:0,	$	; Increase zoom factor.
	zoomout:0,	$	; Decrease zoom factor.
	refresh:0,	$	; Refresh display.
	display_all:0,	$	; Display all planes.
	print_all:0,	$	; Print all planes.
	local_scl:!TRUE,$	; Scale each image separately.
	global_scl:!FALSE,$	; Same scale for entire volume.
	fixed_scl:!FALSE,$	; User specified scale for entire volume.
	fixed_glbl_scl:!FALSE,$	; Global scale used for entire volume.
	build_2x2:0,	$	; Build 2x2 display
	build_custom:0,	$	; Build custom display
	build_addimg:0,	$	; Add image to custom display.
	build_annotate:0,$	; Annotate custom display.
	build_draw:0,	$	; Draw widget for custom display.
	select_custom:0,$	; Select current custom window.
	color_scale:0,$		; Display color scale.
	color_scale_widget:0,$	; Widget number of color scale.
	overlay_actmap:0,$	; Overlay activation map on image.
	view3d:0,	$	; Invoke slicer.
;Status
	error:0,	$	; Error display.
;Profiles
	hprof:0,	$	; Generate horizontal profile.
	vprof:0,	$	; Generate vertical profile.
	zprof:0,	$ 	; Generate time or Z profile.
	roiprof:0,	$ 	; Generate region using time profile.
	oprof:0,	$	; Overlay profile on existing profiles.
	profsave:0,	$	; Save profiles to disk.
	phisto:0,	$	; Plot histogram of profile data.Plot.
	prof_fft:0,	$	; Plot histogram of profile data.Plot.
	profsym:0,	$	; Plot profiles with symbols only.
	profline:0,	$	; Plot profiles with lines only.
	profsymline:0,	$	; Plot profiles with lines and symbols.
	gaussfit:0,	$	; Fit Gaussian to profile.
	profwidth:0,	$	; Profile width.
	profhght:0,	$	; Profile height.
	print_prof:0,	$	; Print profiles.
	select_prof:0, $	; Print profiles.
	annotate_prof:0, $	; Print profiles.
	kill_all_prof:0, $	; Print profiles.
	prbox_smooth:0,$	; Width of boxcar for profile smooth.
	prbox_smooth0:0,$ 	; Boxcar width of 0.
	prbox_smooth3:0,$ 	; Boxcar width of 3.
	prbox_smooth5:0,$	; Boxcar of width of 5
	prof_wht:0,	$	; White profiles.
	prof_ylw:0,	$	; Yellow profiles.
	prof_red:0,	$	; Red profiles.
	prof_grn:0,	$	; Green profiles.
	prof_blu:0,	$	; Blue profiles.
	t_box_size:0,	$	; Box size for time profiles.
	t_gang:0,	$	; Ganged plot for time profile.
	gang_dim:0,	$	; Number of plots in gang.
	gang_subdim:0,	$	; Number of voxels to sum for each gang plot.
	t_prof_plain:0,	$	; Sum all pixels within designated box.
	t_prof_max_pix:0,$	; Sum pixels above threshold.
	thresh_pct:0,	$	; Threshold for time profile.
	thresh_sig:0,	$	; Threshold for time profile.
	subtract_trend:0,$	; Subtract trend from time profiles?
	no_trend:0,	$	; Don't subtract trend from time profiles?
;Process
	plnstats:0,	$	; Statistics of displayed image.
	sinecorr:0,	$	; Correlate with sine function.
	diffimg:0,	$	; Compute difference image.
	actmap:0,	$	; Spawn Avi's actmap program.
        smobox:0,	$	; Smooth with boxcar filter.
        meanvar:0,	$	; Compute mean and variance.
        linfit:0,	$	; Fit line to time course.
        kolmo_smrnov:0,	$	; Compute Kolmogorov-Smirnov statistic.
        wilcoxon:0,	$	; Compute two-tailed Wilcoxon statistic.
        define_stimulus:0,$	; Define stimulus time-course.
        edit_stimulus:0,$	; Edit stimulus time-course.
        show_stimulus:0,$	; Plot stimulus time-course.
        save_stimulus:0,$	; Save stimulus time-course in interfile hdr.
        covariance:0,$		; Compute covariance matrix for a single voxel.
        smoothness:0,$		; Smoothness estimate ala Forman et al.
        time_corr:0,$		; Estimate of time-correlation.
        mult_comp:0,$		; Correct for multiple comparisons.
;Files
	mldcti:0,	$	; Load CTI image.
	ldmri_analyze:0,$	; Load Analyze MRI data.
	ldmri_4dfp:0,	$	; Load 4D floating point MRI data file.
	ld_t88:0,	$	; Load 4D floating point MRI data file.
	ldmri_many_4dfp:0,$	; Load multiple 4D floating point MRI data files.
	ldraw:0,	$	; Load unformatted data.
	stitch:0,	$	; Stitch together multiple files.
	wrtgif:0,	$	; Write GIF file.
	wrtraw:0,	$	; Write Raw file.
	printany:0,	$	; Print any window
;Help
	help:0,		$	; Main help button.
	help_info:0,	$	; Display help window.
;Controls
	files:0,	$	; File list display.
	nextpln:0,	$	; Display next plane.
	lastpln:0,	$	; Display previous plane.
	nextfrm:0,	$	; Display next frame.
	lastfrm:0,	$	; Display previous frame.
	sld_pln:0, 	$	; Plane number slider.
	sld_frm:0, 	$	; Frame number slider.
	delete:0,	$	; Delete file.
	user1:0,	$
	user2:0,	$
	user3:0,	$
	user4:0,	$
	user5:0,	$
	user6:0,	$
	user7:0,	$
	user8:0,	$
	user9:0,	$
	user10:0,	$
	test:0,		$	; Test
	test1:0		$	; Test
	}

cstm = {Custom_image,	$	; Structure for custom images.
	xdim:0,		$	; Horizontal dimension.
	ydim:0,		$	; Vertical dimension.
	id:fltarr(!PROF_NWIN),$	; IDs of custom windows.
	draw:fltarr(!PROF_NWIN),$ ; widget ids of custom windows.
	base:fltarr(!PROF_NWIN),$ ; base ids of custom windows.
	nblt:0,		$	; Number of images in custom display.
	nbltmax:0,	$	; Maximum number of images in custom display.
	nwindows:0,	$	; Number of custom windows.
	cur_window:-1,	$	; Current window number.
	minmax:fltarr(2),$	; Array containing minimum and maximum.
	init:!FALSE	$	; !TRUE if initialized.
        }

help ={Help,		$
	enable:!FALSE,	$	; !TRUE: display help window.
	id:0		$	; Widget id of help window.
        }

fd = {File_data,	$	; Structure for each open file.
	file:'',	$	; File name.
	xdim:0,		$	; Horizontal dimension.
	ydim:0,		$	; Vertical dimension.
	zdim:0,		$	; Depth dimension.
	tdim:0,		$	; Time dimension.
	type:0,		$	; Number format, !FLOAT, !SHORT.
	dxdy:0.,	$	; Pixel size.
	dz:0.,		$	; Slice thickness.
	scl:1.,		$	; Scale factor for entire space.
	array_type:0 	$	; Type of array, real or associated I/O
	}

stc1 = {Stitch,		$	; File that stitches together multiple files.
	name:'',	$	; Name to be displayed in file window.
	n:0,		$	; Number of stitched files.
	tdim:0,		$	; Time dimension of each file.
	hdr_handle:lonarr(20),$	; Handles for headers.
	filnam:strarr(20)$	; Handles for data.
	}
stc = replicate(stc1,!NUM_FILES)

trend = { LinearTrend,	$
        t_rest1:intarr(!NUM_FILES,20), $; Number of starting rest files.
	slope_handle:lonarr(!NUM_FILES), $; Assoc slope files.
	intcpt_handle:lonarr(!NUM_FILES), $; Assoc intercept files.
	hdr_handle:lonarr(!NUM_FILES) $ Handles for image structs.
	}

fi = { Files,		$
	path:'',	$ 	; Current path.
	tail:'',	$ 	; Current file name less path.
	name:'',	$ 	; Current complete file name with path.
	paths:strarr(!NUM_FILES), $ ; Complete filenames loaded.
	names:strarr(!NUM_FILES), $ ; Complete filenames loaded.
	tails:strarr(!NUM_FILES), $ ; Filenames less path ($names:t)
	list:strarr(!NUM_FILES),$ ; Name of filename in displayed list.
	paradigm:fltarr(!NUM_FILES,2000),$ ;Stimulus time-course.
        paradigm_code:strarr(!NUM_FILES), $; ASCII encoded paradigm.
        paradigm_loaded:intarr(!NUM_FILES), $; !TRUE: paradigm loaded.
        zoom:intarr(!NUM_FILES), $; Zoom factor.
	nfiles:0,	$	; Number of files loaded.
	n:0,		$	; Current file being displayed.
	color_scale:intarr(!NUM_FILES),	$ ; Color scale for this file.
	color_min:intarr(!NUM_FILES),	$ ; Parameter set by xloadct
	color_max:intarr(!NUM_FILES),	$ ; Parameter set by xloadct
	color_gamma:fltarr(!NUM_FILES),	$ ; Parameter set by xloadct
	tophand:0,	$	; Top level handle.
	lun:lonarr(!NUM_FILES), $ ; Logical units for data files.
	data_handle:lonarr(!NUM_FILES), $; Assoc data files.
	hdr_handle:lonarr(!NUM_FILES), $ Handles for image structs.
	printfile:'idl.ps'$	; Temporary file for printing.
	}

pr = { Profiles,	$
	tophand:0,	$	; Top level handle.
	profcmds:lonarr(!MAX_PROFILES), $ ; Handles for profile commands
	prof_on:!FALSE,	$ ; !TRUE: A profile window is active.
	mode:!DRAW_OFF,	$ ; !DRAW_OFF, !DRAWING, !DRAW_DONE
	coord:intarr(2),$ ; Row,column used in profile mode.
	hprof:!FALSE,	$ ; Horizontal profile
	oprof: -12345,	$ ; Overplotted profile.
	roiprof:!FALSE, $ ; True: Compute sum of profile region only.
	horzprof:0,	$ ; In horizontal profile mode.
	width:1,	$ ; Width of profile in pixels.
	height:1,	$ ; Height of profile in pixels.
	symtype:!LINE,	$ ; Symbol type for profile.
	sym:intarr(20), $ ; Symbols used in plots.
	profcmd:strarr(20), $ ; List of commands for current profile.
	plotfiles:strarr(20), $ ; Data in current profile.
        plot_title:'',	$ ; Title of current profile.
	nprof:0,	$ ; Number of profiles on current plot.
	nwindows:0,	$ ; Number of profile windows.
	cur_window:-1,	$ ; Current profile window number.
	base:intarr(!PROF_NWIN), $ ; Current profile base widget.
	draw:intarr(!PROF_NWIN), $ ; Current profile draw widget.
	id:intarr(!PROF_NWIN), $ ; Current profile window number.
	boxcar_width:0,  $ ; Width of boxcar smoothing kernel.
	time_type:!PLAIN_TPROF,$ ; !PLAIN_TPROF: sum over region. 
;			           !THRESH_TPROF: sum over thresholded region.
	time_thresh:.5,	$ ; Threshold for time profiles.
	time_thresh_type:!SIGNIFICANCE,	$ ; Type of threshold, SIGNIFICANCE or PERCENT
	subtract_trend:!FALSE, $ ;!TRUE: Subtract linear trend.
	x:0,		$ ; x coordinate of profile.
	y:0,		$ ; y coordinate of profile.
	z:0,		$ ; z coordinate of profile.
	gang_dim:3,	$	; Number of plots in gang.
	gang_subdim:1,	$	; Number of voxels to sum for each gang plot.
	color:!GREEN_INDEX $ ; Color of profile
	}

dsp = { Display,	$
	sizex:256,	$	; Size of image display.
	sizey:256,	$	; Size of image display.
	scale:!GLOBAL,	$	; Type of scaling, global or local.
	table:!GLOBAL,	$	; Number of current color table.
	color_scale:!FALSE,$	; TRUE: Color scale is displayed.
	color_table_index:1,$	; Current color table index.
	image_index:0,	$	; Index used by wset for draw widget.
	id:intarr(500), $	; Window ids from "display all" cmd
	draw:intarr(500), $	; Draw ids from "display all" cmd
	base:intarr(500), $	; Base ids from "display all" cmd
	cur_window:0,	$	; Index to next id to be assigned.
	nwindows:0,	$	; Total number of custom windows.
	view:!TRANSVERSE, $	; Default view.
	img_minmax:fltarr(2), $
	min:0.,		$	; Current displayed minimum.
	max:0.,		$	; Current displayed maximum.
	fixed_min:0.,	$	; User specified minimum value.
	fixed_max:0.,	$	; User specified maximum value.
	xdim:0,		$ 	; Horizontal dimension of disp image.
	ydim:0,		$ 	; Vertical dimension of disp image.
	zdim:0,		$ 	; Depth dimension of disp image.
	tdim:0,		$ 	; Time dimension of disp image.
	type:0,		$ 	; Type of array, ASSOC_ARRAY, FLOAT_ARRAY etc.
	zoom:1,		$	; Zoom value for display window.
	plane:1,	$	; Current plane displayed.
	frame:1		$	; Current frame displayed.
	}

roi = { ROI,		$
	mode:!FALSE	$
	}
	

fl = { Flags,		$
	first:!TRUE,	$
	colscl:!NEVER,	$
	custom:!FALSE,	$
	plnplt:!FALSE	$
	}

st = { State,		$
	pln_sld_min:0,	$
	pln_sld_max:1,	$
	last_vol_num:-1,$
	last_evid:0,	$
	last_crc_diam:10,$ ; Last diameter of circular roi.
	last_sq_side:10,$ ; Last diameter of circular roi.
	roicolor:!GREEN_INDEX, $
	ldfilter:'',	$ ; Filter used for reading images.
	error:0,	$
	error_msg:''	$
	}
