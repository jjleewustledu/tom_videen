;widget ids
wd = {	Widget_IDs,	$
	mldcti:0,	$
	wldimg:0,	$
	wldt88:0,	$
	wldscn:0,	$
	wldatn:0,	$
	wldnrm:0,	$
	wldv:0,	$
	wldS:0,	$
	wldN:0,	$
	wlda:0,	$
	mldraw:0,	$  ; Load raw data.
	ldraw_int:0,	$  ; Load long integer raw data.
	ldraw_sht:0,	$  ; Load short integer raw data.
	ldraw_flt:0,	$  ; Load floating raw data.
	ldraw_byt:0,	$  ; Load byte raw data.
	ldifh:0,	$  ; Load interfile.
	ldmri:0,	$  ; Load mri file (short integer flipped about x).
	wrtraw_flt:0,	$  ; Write floating raw data.
	mwrite:0,	$  ; Write data
	mshow:0,	$  ; Show info
	mldelse:0,	$  ; Load other
	wldmonte:0,	$
	wldmonte_all:0,	$
	wldmonte_scat:0, $
	wdispglob:0,	$
	wdisploc:0,	$
	mscale:0,	$
	colscl_draw:0,	$
	custom_draw:0,	$
	custom_id:0,	$
	msmooth:0,	$
	wsmo121:0,	$
	wsmobox:0,	$
	wsmobox5:0,	$
	wsmobox7:0,	$
	smogauss:0,	$
	wscatstat:0,	$
	rotate:0,	$
	transpose:0,	$
	wrdtiff:0,	$
	wstats:0,	$  ; Calculate image stats.
	wvstats:0,	$  ; Calculate volume stats.
	gaussfit:0,	$
	zprof:0,	$
	fftprof:0,	$
	profwidth:0,	$
	profwidth1:0,	$
	profwidth2:0,	$
	profwidth4:0,	$
	profwidth8:0,	$
	profwidth16:0,	$
	profwidthx:0,	$
	msymbol:0,	$ ; Base symbol button.
	profsym:0,	$ ; Select profiles with symbols.
	profline:0,	$ ; Select profiles with lines.
	profsymline:0,	$ ; Select profiles with lines and symbols.
	profsum:0,	$ ; Select profiles with lines and symbols.
	getint1:0,	$ ; Widget to get integer values.
	getint2:0,	$
	getint3:0,	$
	getintok:0,	$ ; OK button for get_str widget.
  	get_true:0,	$ ; True for get_true_false widget.
  	get_false:0,	$ ; False for get_true_false widget.
	scatcor:0,	$
	imgdiv:0,	$
	roicpy:0,	$ ; Copy roi to all planes.
	roiplnplt:0,	$ ; Plot roi values vs. plane.
	roiplnoplt:0,	$ ; Plot roi values vs. plane.
	nrmplt:0,	$ ; Plot norm values vs. plane.
	nrmoplt:0,	$ ; Overplot norm values vs. plane.
	roicolor:0,	$ 
	roiwht:0,	$
	roiylw:0,	$
	roired:0,	$
	roigrn:0,	$
	roiblu:0,	$
	mroitype:0,	$ ; Type menu button.
	roiirreg:0,	$ ; Draw irregular region.
	roisq:0,	$ ; Draw square region.
	roicrc:0,	$ ; Draw square region.
        roimov:0,	$ ; Move region of interest.
        roipcpy:0,	$ ; Copy regions from another plane
	build3x2:0,	$ ; Build 3x2 matrix of images.
	build2x3:0,	$ ; Build 2x3 matrix of images.
	buildcustom:0,	$ ; Build custom matrix of images.
        calc:0,		$ ; Invoke image calculator.
        scatmc:0,	$ ; Calculate statistics for Monte Carlo error.
        denorm:0,	$ ; Denormalize a sinogram file.
        mhisto:0,	$ ; Plot histograms
        histo_all:0,	$ ; Plot histogram of volume.
        histo_pln:0,	$ ; Plot histogram of image plane.
        wxwdtif:0,	$
        ldfilter:0,	$ ; Define filter for reading images.
        threshset:0,	$ ; Threshold over range and set to value.
        axialavg:0,	$ ; Average image over planes.
	chngname:0,	$ ; Change name in list.
	nxtvol:0,	$ ; Apply next volume number.
	repeat_action:0,$ ; Repeat last action.
	mhdrdmp:0,	$ ; Dump main header.
	sbhdrdmp:0,	$ ; Dump subheader.
	pln2sino:0,	$ ; Convert from plane view to sinogram.
	mstats:0,	$ ; Convert from plane view to sinogram.
	stats_by_pln:0,	$ ; Convert from plane view to sinogram.
	dispsep:0,	$ ; Display image on a separate window (with ROIs).
	flip:0,		$ ; Flip image about vertical axis.
	prof_gif:0,	$ ; Write profile to GIF file.
	tumor:0,	$ ; Insert tumor
        wxwdgif:0	$
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
	symtype:!LINE,	$
	roicolor:!GREEN_INDEX, $
	profwidth:1,	$
	ldfilter:''	$ ; Filter used for reading images.
	}
