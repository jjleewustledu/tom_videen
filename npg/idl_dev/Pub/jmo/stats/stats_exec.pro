;**********************
pro stats_exec_event,ev
;**********************

common stats_comm,wd,fi,pr,dsp,roi,fl,st,stc,cstm,bltimg,dsp_image,help

common profiles,x1,x2,y1,y2,x1m1,x2m1,y1m1,y2m1,row,col,profdat

common linear_fit,fit_name,slope,intcpt,trend

common color_table_index,table_indx

common volume_data,slicer_image

type = tag_names(ev,/STRUCTURE)

on_error,2

if(wd.draw gt 0) then begin
    widget_control,GET_VALUE=i,wd.draw
    if(dsp.image_index ne i) then begin
        wset,dsp.image_index
        return
    endif 
endif

error_status=0
catch,error_status
if(error_status ne 0) then begin
    stat=widget_message('*** Procedure aborted due to an error.  ***',/ERROR)
    pr.prof_on = !PROF_NO 
    wset,dsp.image_index
    return
endif

if(ev.id eq wd.draw) then begin
    if(ev.type eq !RELEASE) then return ; Only process button presses.
    if(n_elements(dsp.xdim) eq 0) then return ;No images loaded.
    if(ev.type ne 0) then return
    if(pr.prof_on ne !PROF_NO) then begin
	if(ev.press eq 2) then begin
	    pr.prof_on = !PROF_NO 
    	    pr.roiprof = !FALSE
	endif else begin
	    widget_control,wd.error,SET_VALUE=string('Exit profile mode with center button.')
	    if(pr.prof_on eq !PROF_REG) then $
	        ev.id = wd.roiprof $
	    else $
	        ev.id = wd.oprof
	endelse
    endif
endif


if(ev.id eq wd.repeat_action) then begin
    ev.id = st.last_evid 
endif

if((ev.id ne wd.draw) and (ev.id ne wd.nxtvol) and (ev.id ne wd.nextpln) and (ev.id ne wd.lastpln)) then $
    st.last_evid = ev.id

case ev.id of

wd.exit: widget_control,wd.leader,/DESTROY

wd.draw: begin
    ev.x = ev.x + 1
    ev.y = ev.y + 1
    if(ev.x lt 0) then ev.x = 0
    if(ev.x gt !DRAW_SIZEX-1) then ev.x = !DRAW_SIZEX-1
    if(ev.y lt 0) then ev.y = 0
    if(ev.y gt !DRAW_SIZEY-1) then ev.y = !DRAW_SIZEY-1
    if(ev.type eq 0) then begin
	x = ev.x/dsp.zoom
        y = (!DRAW_SIZEY - ev.y - 1)/dsp.zoom
        if(x gt dsp.xdim-1) then x = dsp.xdim - 1
        if(y gt dsp.ydim-1) then y = dsp.ydim - 1
        pln = dsp.zdim*(dsp.frame-1) + dsp.plane-1
        val = strcompress(string('(',x,',',y,'): ',dsp_image(x,y)),/REMOVE_ALL)
        widget_control,wd.error,SET_VALUE=val
	st.error = !ERROR
    endif
end

;*********** File I/O Functions.  *************

wd.ldmri_analyze: begin
    stat = load_data(fi,st,dsp,wd,stc,!MRI_ANALYZE,help)
    if(stat eq !OK) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.ldmri_many_4dfp: begin
    stat = !OK
    while (stat eq !OK) do stat = load_data(fi,st,dsp,wd,stc,!MRI_4DFP,help)
    if(stat eq !OK) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.ldmri_4dfp: begin
    stat = load_data(fi,st,dsp,wd,stc,!MRI_4DFP,help)
    if(stat eq !OK) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.ldraw: begin
    stat = load_data(fi,st,dsp,wd,stc,!RAW,help)
    if(stat eq !OK) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.ld_t88: begin
    stat = load_data(fi,st,dsp,wd,stc,!T88,help)
    if(stat eq !OK) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.stitch: begin
    stat = load_data(fi,st,dsp,wd,stc,!STITCH,help)
    if(stat ne !ERROR) then begin
        dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
	refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    endif
end

wd.delete: begin
    widget_control,/HOURGLASS
    delete_image,wd,fi,dsp
    if(fi.n gt 0) then begin
        update_image,fi,dsp,wd,stc
    endif
    widget_control,HOURGLASS=0
end

wd.wrtgif: begin
;   Write current or custom image to a GIF file.
    stat = write_data(fi,st,dsp,wd,stc,!GIF,help,cstm,pr)
end

wd.wrtraw: begin
;   Write current image to a RAW file.
    stat = write_data(fi,st,dsp,wd,stc,!RAW,help,cstm,pr)
end

wd.printany: begin
    print_window,dsp,cstm,pr
end

wd.files: begin
    fi.n = ev.index
    if(fi.n ge fi.nfiles) then return
    fi.name = fi.names(fi.n)
    fi.path = fi.paths(fi.n)
    fi.tail = fi.tails(fi.n)
    dsp.zoom = fi.zoom(fi.n)
    update_image,fi,dsp,wd,stc
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    widget_control,wd.sld_pln,SET_SLIDER_MAX=dsp.zdim
    widget_control,wd.sld_frm,SET_SLIDER_MAX=dsp.tdim
end

;*********** Display Functions.  **************

wd.loadct: begin
    xloadct_jmo,NCOLORS=!LEN_COLORTAB,GROUP=ev.top,FILE=!COLOR_TABLE,/SILENT
end

wd.cine: begin
    if(help.enable eq !TRUE) then begin
        value='Displays difference between each frame and the first frame in cine mode.'+'Images are smoothed with a 3x3 boxcar filter.'
        widget_control,help.id,SET_VALUE=value
    endif
    widget_control,/HOURGLASS
    frame = dsp.frame
    image1 = get_image(dsp.plane-1,fi,stc)
    x = fltarr(512,512)
    for i=2,dsp.tdim do begin
 	dsp.frame = i
        image2 = get_image(dsp.zdim*(dsp.frame-1)+dsp.plane-1,fi,stc)
 	diff = smooth(image2 - image1,3) 
	image2 = smooth(x,7)
        refresh_display,fi,dsp,stc,wd,IMAGE=diff
        widget_control,wd.error,SET_VALUE=string(dsp.frame)
    endfor
    dsp.frame = frame
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    widget_control,HOURGLASS=0
end

wd.nextpln: begin
    dsp.plane = dsp.plane + 1
    if(dsp.plane gt dsp.zdim) then dsp.plane = dsp.zdim
    widget_control,wd.sld_pln,SET_VALUE=dsp.plane
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.lastpln: begin
    dsp.plane = dsp.plane - 1
    if(dsp.plane lt 1) then dsp.plane = 1
    widget_control,wd.sld_pln,SET_VALUE=dsp.plane
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.nextfrm: begin
    dsp.frame = dsp.frame + 1
    if(dsp.frame gt dsp.tdim) then dsp.frame = dsp.tdim
    widget_control,wd.sld_frm,SET_VALUE=dsp.frame
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.lastfrm: begin
    dsp.frame = dsp.frame - 1
    if(dsp.frame lt 1) then dsp.frame = 1
    widget_control,wd.sld_frm,SET_VALUE=dsp.frame
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.sld_pln: begin
    widget_control,wd.sld_pln,GET_VALUE=i
    dsp.plane = i
    if(dsp.plane lt 1) then dsp.plane = 1
    if(dsp.plane gt dsp.zdim) then dsp.plane = dsp.zdim
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.sld_frm: begin
    widget_control,wd.sld_frm,GET_VALUE=i
    dsp.frame = i
    if(dsp.frame lt 1) then dsp.frame = 1
    if(dsp.frame gt dsp.tdim) then dsp.frame = dsp.tdim
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.zoomin: begin
    fi.zoom(fi.n) = 2*dsp.zoom
    dsp.zoom = fi.zoom(fi.n)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end
 
wd.zoomout: begin
    fi.zoom(fi.n) = .5*dsp.zoom
    dsp.zoom = fi.zoom(fi.n)
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.local_scl: begin
    if(help.enable eq !TRUE) then begin
        value='Each image is scaled to its own maximum.'
        widget_control,help.id,SET_VALUE=value
    endif
    dsp.scale = !LOCAL
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    widget_control,wd.local_scl,SENSITIVE=0
    widget_control,wd.global_scl,/SENSITIVE
    widget_control,wd.fixed_scl,/SENSITIVE
    widget_control,wd.fixed_glbl_scl,/SENSITIVE
end

wd.global_scl: begin
    if(help.enable eq !TRUE) then begin
        value='Images are scaled to the maximum over all displayed images.'
        widget_control,help.id,SET_VALUE=value
    endif
    dsp.scale = !GLOBAL
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    widget_control,wd.local_scl,/SENSITIVE
    widget_control,wd.global_scl,SENSITIVE=0
    widget_control,wd.fixed_scl,/SENSITIVE
    widget_control,wd.fixed_glbl_scl,/SENSITIVE
end

wd.fixed_scl: begin
    if(help.enable eq !TRUE) then begin
        value='Images are scaled to (max - min)/NUMBER_OF_COLORS where max and min are specified by the user.  Overflows and underflows are set to 0 and NUMBER_OF_COLORS respectively'
        widget_control,help.id,SET_VALUE=value
    endif
    dsp.scale = !FIXED
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
    max = max(dsp_image)
    min = min(dsp_image)
    if(abs(max) gt abs(min)) then begin
	dsp.fixed_min = -abs(max)
        dsp.fixed_max =  abs(max)
    endif else begin
	dsp.fixed_min = -abs(min)
        dsp.fixed_max =  abs(min)
    endelse
    value = strarr(2)
    label = strarr(2)
    value(0) = string(dsp.fixed_max)
    value(1) = string(dsp.fixed_min)
    label(0) = string("Maximum: ")
    label(1) = string("Minimum: ")
    minmax = get_str(2,label,value)
    dsp.fixed_max = float(minmax(0))
    dsp.fixed_min = float(minmax(1))
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    widget_control,wd.local_scl,/SENSITIVE
    widget_control,wd.global_scl,/SENSITIVE
    widget_control,wd.fixed_scl,/SENSITIVE
    widget_control,wd.fixed_glbl_scl,/SENSITIVE
end

wd.fixed_glbl_scl: begin
    if(help.enable eq !TRUE) then begin
        value='Images are scaled to (global_max - global_min)/NUMBER_OF_COLORS where max and min are specified by the user.  Overflows and underflows are set to 0 and NUMBER_OF_COLORS respectively'
        widget_control,help.id,SET_VALUE=value
    endif
    dsp.scale = !FIXED
    max = 0.
    min = 1e20
    for pln=0,dsp.zdim-1 do begin
        img = get_image(dsp.zdim*(dsp.frame-1) + pln,fi,stc)
        max1 = max(img)
  	if(max1 gt max) then max = max1
        min1 = min(img)
  	if(min1 lt max) then min = min1
    end
    label = strarr(2)
    label(0) = string('Symmetric limits')
    label(1) = string('Use global min and max')
    title = string('Set symmetric limits (min = -max)?')
    lcminmax = get_boolean(title,label)
    if(lcminmax eq !TRUE) then begin
        if(abs(max) gt abs(min)) then begin
    	    dsp.fixed_min = -abs(max)
            dsp.fixed_max =  abs(max)
        endif else begin
	    dsp.fixed_min = -abs(min)
            dsp.fixed_max =  abs(min)
        endelse
    endif
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
    widget_control,wd.local_scl,/SENSITIVE
    widget_control,wd.global_scl,/SENSITIVE
    widget_control,wd.fixed_scl,SENSITIVE=0
    widget_control,wd.fixed_glbl_scl,/SENSITIVE
end

wd.build_2x2: begin
    cstm.cur_window = cstm.nwindows
    cstm.nwindows = cstm.nwindows + 1
    title = string(cstm.cur_window,FORMAT='("Custom window ",i3)')
    custbase = widget_base(TITLE=title,/COLUMN)
    wd.build_draw = widget_draw(custbase,xsize=!CUSTOM_XSZ, $
    ysize=!CUSTOM_XSZ,RETAIN=2,KILL_NOTIFY=string('custom_dead'))
    widget_control,custbase,/REALIZE
    widget_control,GET_VALUE=custom_id,wd.build_draw
    cstm.id(cstm.cur_window) = custom_id
    cstm.draw(cstm.cur_window) = wd.build_draw
    cstm.base(cstm.cur_window) = custbase
    cstm.init = !TRUE
    handle_value,fi.hdr_handle(fi.n),hdr
    bltimg = fltarr(hdr.xdim,hdr.ydim,!MAX_BLTIMG)
    cstm.nblt = 0
    wset,dsp.image_index
    widget_control,wd.build_addimg,/SENSITIVE
    widget_control,wd.build_annotate,/SENSITIVE
end

wd.build_custom: begin
    value = strarr(2)
    label = strarr(2)
    value(0) = string('512')
    value(1) = string('512')
    label(0) = string("X dimension: ")
    label(1) = string("Y dimension: ")
    sizes = get_str(2,label,value)
    xsize = fix(sizes(0)) + 8
    ysize = fix(sizes(1)) + 8
    cstm.cur_window = cstm.nwindows
    cstm.nwindows = cstm.nwindows + 1
    title = string(cstm.cur_window,FORMAT='("Custom window ",i3)')
    custbase = widget_base(TITLE=title,/COLUMN)
    wd.build_draw = widget_draw(custbase,xsize=xsize, $
        ysize=ysize,RETAIN=2,KILL_NOTIFY=string('custom_dead'))
    widget_control,custbase,/REALIZE
    widget_control,GET_VALUE=custom_id,wd.build_draw
    cstm.id(cstm.cur_window) = custom_id
    cstm.init = !TRUE
    handle_value,fi.hdr_handle(fi.n),hdr
    bltimg = fltarr(hdr.xdim,hdr.ydim,!MAX_BLTIMG)
    cstm.nblt = 0
    wset,dsp.image_index
    widget_control,wd.build_addimg,/SENSITIVE
    widget_control,wd.build_annotate,/SENSITIVE
end

wd.build_addimg: begin
    if(cstm.init ne !TRUE) then begin
        stat=widget_message('*** Custom display not initialized.  ***',/ERROR)
        return
    endif
    if  (n_elements(dsp_image(*,0,0)) gt n_elements(bltimg(*,0,0))) or $
        (n_elements(dsp_image(0,*,0)) gt n_elements(bltimg(0,*,0))) then begin
        stat=widget_message(string('Invalid image dimensions.'),/ERROR)
        error = !TRUE
        return
    endif
    bxdim = n_elements(bltimg(*,0,cstm.nblt))
    bydim = n_elements(bltimg(0,*,cstm.nblt))
    nxdim = n_elements(dsp_image(*,0))
    nydim = n_elements(dsp_image(0,*))
    if(bxdim lt nxdim) then begin
        stat=widget_message(string('*** Custom images must have same dimensions. ***'),/ERROR)
        error = !TRUE
        return
    endif
    if(bydim lt nydim) then begin
        stat=widget_message(string('*** Custom images must have same dimensions. ***'),/ERROR)
        error = !TRUE
        return
    endif
    xpad = (bxdim - nxdim)/2
    ypad = (bydim - nydim)/2
    bltimg(xpad:bxdim-xpad-1,ypad:bydim-ypad-1,cstm.nblt) = dsp_image(*,*)
    cstm.nblt = cstm.nblt + 1
    wset,cstm.id(cstm.cur_window)
    case dsp.scale of
        !GLOBAL: display_glob,bltimg,min,max,ZOOM=dsp.zoom,NPANES=cstm.nblt
        !LOCAL: display,bltimg,min,max,ZOOM=dsp.zoom,NPANES=cstm.nblt
        !FIXED: begin
	    scale = !LEN_COLORTAB/float(dsp.fixed_max - dsp.fixed_min)
            display,bltimg,min,max,ZOOM=dsp.zoom,SCL=scale,FIXED_MIN=dsp.fixed_min,NPANES=cstm.nblt
	    min = dsp.fixed_min
	    max = dsp.fixed_max
	    end
	else: stat=widget_message('Illegal display mode.',/ERROR)
    endcase
    if(dsp.color_scale eq !TRUE) then $
        showcolors,'Custom',wd,dsp
    cstm.minmax(0) = min
    cstm.minmax(1) = max
;    if(dsp.color_scale eq !TRUE) then $
;	showcolors,'Custom',wd,dsp
    widget_control,wd.build_addimg,/SENSITIVE
    widget_control,wd.build_annotate,/SENSITIVE
    wset,dsp.image_index
end

wd.build_annotate: begin
    wset,cstm.id(cstm.cur_window)
    annotate,WINDOW=cstm.id(cstm.cur_window)
    wset,dsp.image_index
end

wd.select_custom: begin
    if(help.enable eq !TRUE) then begin
        value='Selects window that will be stored with the "Write GIF image" button.'
        widget_control,help.id,SET_VALUE=value
    endif
    labels = strarr(cstm.nwindows)
    for i=0,cstm.nwindows-1 do $
	labels(i) = strcompress(string(i),/REMOVE_ALL)
    index = get_button(labels)
    cstm.cur_window = index
end

wd.overlay_actmap: begin
    overlay_actmap,fi,dsp,wd,st,stc,help
    update_image,fi,dsp,wd,stc
    dsp_image = get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc)
end

wd.color_scale: begin
        colbase = widget_base(title='Colors',/COLUMN)
        wd.color_scale_widget = widget_draw(colbase,xsize=!COLSCL_WIN_XSZ, $
ysize=!COLSCL_WIN_YSZ,RETAIN=2,KILL_NOTIFY=string('colscl_dead'))
        widget_control,wd.color_scale_widget,/REALIZE
        dsp.color_scale = !TRUE
        showcolors,'Main',wd,dsp
end
    
wd.display_all: begin
    display_all,dsp,fi,help
end

wd.view3d: begin
    img = fltarr(dsp.xdim,dsp.ydim,dsp.zdim)
    for z=0,dsp.zdim-1 do $
        img(*,*,z) = get_image(dsp.zdim*(dsp.frame-1) + z,fi,stc)
    slicer_image = img
    slicer,/DETACHED
end

wd.print_all: begin
    if(help.enable eq !TRUE) then begin
        value=string("Prints all images in current frame to ",!PRINTER)
        widget_control,help.id,SET_VALUE=value
    endif
    print_all,dsp,fi
end
 
wd.refresh: begin
    erase
    refresh_display,fi,dsp,stc,wd,IMAGE=dsp_image
end

wd.help_info: begin
    help_base = widget_base(/COLUMN)
    help.id = widget_text(help_base,KILL_NOTIFY=string('help_dead'),/WRAP, $
    VALUE=string("Watch this space for deceptive and misleading comments.  "), $
	XSIZE=70,YSIZE=8,/SCROLL)
    widget_control,help_base,/REALIZE
    xmanager,'stats_exec',help_base
    help.enable = !TRUE
end

wd.help: begin
    text = strarr(1)
    text(0)="Send all questions to /dev/null@npg, or equivalently, to systems@npg."
    i = widget_message(text,/ERROR)
end

;********** Processing functions. ***************

wd.plnstats: begin
    print_stats,dsp_image,dsp,wd,fi,stc
end

wd.sinecorr: begin
    sinecorr,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end

wd.meanvar: begin
    meanvar,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end

wd.linfit: begin
    linfit,fi.n,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end

wd.kolmo_smrnov: begin
    ks,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end

wd.wilcoxon: begin
    wilcoxon,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end
    
wd.diffimg: begin
    diffimg,fi,pr,dsp,wd,stc,help
    update_image,fi,dsp,wd,stc
end
    
wd.actmap: begin
    actmap,fi,help
end

wd.define_stimulus: begin
    define_paradigm,fi,stc
end

wd.edit_stimulus: begin
    edit_paradigm,fi,dsp
end

wd.show_stimulus: begin
    plot_stimulus,fi,dsp
end

wd.save_stimulus: begin
    handle_value,fi.hdr_handle(fi.n),hd
    handle_value,fi.hdr_handle(hd.mother),hdr
    get_lun,lu
    tail = fi.tails(hd.mother)
    dot = rstrpos(tail,'.')
    if(dot gt 0) then $
        stem = strmid(tail,0,dot) $
    else $
	stem = tail
    tail = strcompress(string(stem,'.4dfp.ifh'),/REMOVE_ALL)
    filnam = fi.paths(hd.mother) + '/' + tail
    openu,lu,filnam,/APPEND
    printf,lu,'paradigm format := ',fi.paradigm_code(hd.mother)
    close,lu
    free_lun,lu
end

wd.covariance: begin
    if(pr.x ge 0) then $
        get_cov,fi,pr,dsp,wd,stc,help,pr.x,pr.y,dsp.plane-1
end

wd.smoothness: begin
    get_smoothness,fi,pr,dsp,wd,stc,help,pr.x,pr.y,dsp.plane-1
end

wd.time_corr: begin
    get_time_corr,fi,pr,dsp,wd,stc,help
end

wd.mult_comp: begin
    mult_comp,fi,wd,dsp,help
end

;wd.test1: begin 
;    image = wilcoxon_noise(fi,pr,dsp,wd,stc,help)
;end
;
;wd.test: begin
;end

wd.smobox: begin
    smobox,fi,dsp,wd
end

;********** Profile functions. ***************

wd.hprof: begin
    pr.roiprof = !FALSE
    pr.horzprof = !TRUE
    prof = get_profile(row,fi,pr,dsp,wd,st,stc,help) ; Interactively determine profile to plot.
    pr.prof_on = !PROF_HV
    profile,prof,row,pr,dsp,fi.tail,profdat
    widget_control,wd.profsave,/SENSITIVE
end

wd.vprof: begin
    pr.horzprof = !FALSE
    pr.roiprof = !FALSE
    prof = get_profile(row,fi,pr,dsp,wd,st,stc,help) ; Interactively determine profile to plot.
    pr.prof_on = !PROF_HV
    profile,prof,row,pr,dsp,fi.tail,profdat
    widget_control,wd.profsave,/SENSITIVE
end

wd.zprof: begin
    if(dsp.type eq !OVERLAY_ARRAY) then begin
      	stat=widget_message('Time profiles dont work with overlay images. Sorry.',/ERROR)
       	return
    endif
    pr.prof_on = !PROF_NO
    pr.roiprof = !FALSE
    widget_control,wd.draw,EVENT_PRO='get_time_prof'
    if(pr.mode eq !DRAW_DONE) then begin
        widget_control,wd.draw,GET_UVALUE=prof
    endif
end

wd.roiprof: begin
    pr.prof_on = !PROF_REG
    pr.roiprof = !TRUE
    widget_control,wd.draw,EVENT_PRO='get_time_prof'
    if(pr.mode eq !DRAW_DONE) then begin
        widget_control,wd.draw,GET_UVALUE=prof
    endif
end

wd.oprof: begin
;   Determine profile to plot.
    case pr.prof_on of
	!PROF_NO: return
	!PROF_HV: begin
	    prof = get_profile(row,fi,pr,dsp,wd,st,stc,help) 
	    name = strcompress(string(fi.tail,"_r",row),/REMOVE_ALL)
            overplot,prof,row,name,profdat,pr,dsp
	end
	!PROF_T: begin
            widget_control,wd.draw,EVENT_PRO='get_time_prof'
            if(pr.mode eq !DRAW_DONE) then begin
                widget_control,wd.draw,GET_UVALUE=prof
            endif
  	end
    endcase
end

wd.profsave: begin
    if(help.enable eq !TRUE) then begin
        value='Save profiles to disk in an ASCII file.'
        widget_control,help.id,SET_VALUE=value
    endif
    plotfile = pickfile(/read,FILTER='*.plt',PATH=fi.path, $
                                        GET_PATH=fi.path,file="profile.plt")
   if(plotfile eq '') then return
   handle_value,pr.profcmds(pr.cur_window),prd
   openw,luprof,plotfile(0),/GET_LUN
   x = fltarr(prd.nprof+1)
   for i=0,n_elements(prd.profdat(*,0))-1 do begin
       for j=0,prd.nprof do x(j) = prd.profdat(i,j)
       printf,luprof,x,FORMAT='(10g)'
   endfor
   close,luprof
   free_lun,luprof
end

wd.profwidth: begin
    if(help.enable eq !TRUE) then begin
        value='Profile will be computed by integrating over a line perpendicular to the profile for the width specified here.'
        widget_control,help.id,SET_VALUE=value
    endif
    labels = strarr(13)
    labels(0) = string('Width: 1')
    labels(1) = string('Width: 3')
    labels(2) = string('Width: 5')
    labels(3) = string('Width: 7')
    labels(4) = string('Width: 9')
    labels(5) = string('Width: 11')
    labels(6) = string('Width: 13')
    labels(7) = string('Width: 15')
    labels(8) = string('Width: 21')
    labels(9) = string('Width: 31')
    labels(10) = string('Width: 41')
    labels(11) = string('Width: 51')
    labels(12) = string('Width: 61')
    index = get_button(labels)
    if(index lt 8) then $
        pr.width = 2*index + 1 $
    else $
	pr.width = 10*(index-6) + 1
end

wd.profhght: begin
    if(help.enable eq !TRUE) then begin
        value='Profile will be computed by integrating over a line perpendicular to the profile for the height specified here.'
        widget_control,help.id,SET_VALUE=value
    endif
    labels = strarr(13)
    labels(0) = string('Height: 1')
    labels(1) = string('Height: 3')
    labels(2) = string('Height: 5')
    labels(3) = string('Height: 7')
    labels(4) = string('Height: 9')
    labels(5) = string('Height: 11')
    labels(6) = string('Height: 13')
    labels(7) = string('Height: 15')
    labels(8) = string('Width: 21')
    labels(9) = string('Width: 31')
    labels(10) = string('Width: 41')
    labels(11) = string('Width: 51')
    labels(12) = string('Width: 61')
    index = get_button(labels)
    if(index lt 8) then $
        pr.height = 2*index + 1 $
    else $
	pr.height = 10*(index-6) + 1
end

wd.t_box_size: begin
    if(help.enable eq !TRUE) then begin
        value='Time profile will be computed by either by integrating over the specified box at each frame or by summing pixels above a specified threshold.'
        widget_control,help.id,SET_VALUE=value
    endif
    labels = strarr(13)
    labels(0) = string('Height: 1x1')
    labels(1) = string('Height: 3x3')
    labels(2) = string('Height: 5x5')
    labels(3) = string('Height: 7x7')
    labels(4) = string('Height: 9x9')
    labels(5) = string('Height: 11x11')
    labels(6) = string('Height: 13x13')
    labels(7) = string('Height: 15x15')
    labels(8) = string('Width: 21x21')
    labels(9) = string('Width: 31x31')
    labels(10) = string('Width: 41x41')
    labels(11) = string('Width: 51x51')
    labels(12) = string('Width: 61x61')
    index = get_button(labels)
    if(index lt 8) then begin
        pr.height = 2*index + 1 
        pr.width = 2*index + 1
    endif else begin
	pr.height = 10*(index-6) + 1
	pr.width = 10*(index-6) + 1
    endelse
end

wd.prof_fft: begin
    prof_fft,pr,dsp,help
end

;wd.gaussfit: begin
;    if(help.enable eq !TRUE) then begin
;        value='Fit a Gaussian function to the most recently plotted profile.'
;        widget_control,help.id,SET_VALUE=value
;    endif
;    len = n_elements(profdat(*,0))
;    data = fltarr(2*len) ; Zero pad for fitting routine.
;    x = fltarr(2*len)
;    x(len/2:len+len/2-1) = profdat(*,0)
;    data(len/2:len+len/2-1) = profdat(*,pr.nprof)
;    fit = gaussfit1(profdat(*,0),profdat(*,1),params)
;    mean = params(1) - (2*len - 1)/2
;    label = strcompress(string(params(0),mean,params(2), $
;            FORMAT='(f10.0,"exp(-.5(x - ",f8.3,")**2/",f8.3,"**2)")'))
;    symtype = pr.symtype
;;    fitplot = fit(len/2:len+len/2-1)
;    pr.symtype = !LINE
;    overplot,fit,0,label,profdat,pr,dsp
;    pr.symtype = symtype
;    print,'Mean: ',mean,' Variance: ',params(2),' Scale: ',params(0)
;end

wd.print_prof: begin
    if(help.enable eq !TRUE) then begin
        value='Print most recently plotted profile to the default line printer.'
        widget_control,help.id,SET_VALUE=value
    endif
    print_prof,pr,fi
    widget_control,wd.error,SET_VALUE=string('Plot printed to ',!PRINTER)
end
 
wd.select_prof: begin
    if(help.enable eq !TRUE) then begin
        value='Select a profile.  This can be used to plot additonal profiles.'
        widget_control,help.id,SET_VALUE=value
    endif
    n = 0
    for i=0,pr.nwindows-1 do begin
        if(pr.id(i) gt 0) then n = n + 1
    end
    labels = strarr(n)
    j = 0
    for i=0,pr.nwindows-1 do begin
        if(pr.id(i) ge 0) then begin
	    labels(j) = strcompress(string(i),/REMOVE_ALL)
	    j = j + 1
	endif
    end
    index = get_button(labels)
    pr.cur_window = index
end

wd.kill_all_prof: begin
    n = 0
    for i=0,pr.nwindows-1 do begin
	handle_free,pr.profcmds(i)
        if(pr.base(i) gt 0) then begin
            widget_control,pr.base(i),/DESTROY
            pr.id(i) = -1
            pr.draw(i) = -1
            pr.base(i) = -1
        endif
    end
    pr.nwindows = 0
end

wd.profsym: begin
    if(help.enable eq !TRUE) then begin
        value='Profiles plot using symbols only.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.symtype = !SYMBOL
    widget_control,wd.profsym,SENSITIVE=0
    widget_control,wd.profline,/SENSITIVE
    widget_control,wd.profsymline,/SENSITIVE
end
 
wd.profline: begin
    if(help.enable eq !TRUE) then begin
        value='Profiles plot using lines only.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.symtype = !LINE
    widget_control,wd.profline,SENSITIVE=0
    widget_control,wd.profsym,/SENSITIVE
    widget_control,wd.profsymline,/SENSITIVE
end
 
wd.profsymline: begin
    if(help.enable eq !TRUE) then begin
        value='Profiles plot using symbols and lines.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.symtype = !SYMLINE
    widget_control,wd.profline,/SENSITIVE
    widget_control,wd.profsym,/SENSITIVE
    widget_control,wd.profsymline,SENSITIVE=0
end

wd.t_prof_plain: begin
    if(help.enable eq !TRUE) then begin
        value='Integrate over specified box at each frame.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.time_type = !PLAIN_TPROF
    widget_control,wd.t_prof_max_pix,/SENSITIVE
    widget_control,wd.t_prof_plain,SENSITIVE=0
    widget_control,wd.t_gang,/SENSITIVE
    widget_control,wd.phisto,/SENSITIVE
end

wd.t_gang: begin
    pr.time_type = !GANG_TPROF
    widget_control,wd.t_prof_max_pix,/SENSITIVE
    widget_control,wd.t_prof_plain,/SENSITIVE
    widget_control,wd.gang_dim,/SENSITIVE
    widget_control,wd.gang_subdim,/SENSITIVE
    widget_control,wd.phisto,/SENSITIVE
    widget_control,wd.t_gang,SENSITIVE=0
end

wd.phisto: begin
    pr.time_type = !HISTO_TPROF
    widget_control,wd.t_prof_max_pix,/SENSITIVE
    widget_control,wd.t_prof_plain,/SENSITIVE
    widget_control,wd.gang_dim,/SENSITIVE
    widget_control,wd.gang_subdim,/SENSITIVE
    widget_control,wd.t_gang,/SENSITIVE
    widget_control,wd.phisto,SENSITIVE=0
end

wd.gang_dim: begin
    labels = strarr(2)
    labels(0) = string('3x3')
    labels(1) = string('5x5')
    index = get_button(labels,TITLE='Enter profile array size')
    pr.gang_dim = (index+1)*2 + 1
end

wd.gang_subdim: begin
    labels = strarr(5)
    labels(0) = string('1')
    labels(1) = string('2')
    labels(2) = string('3')
    labels(3) = string('4')
    labels(4) = string('5')
    index = get_button(labels,TITLE='Number of voxels to average')
    pr.gang_subdim = index + 1
end

wd.t_prof_max_pix: begin
    if(help.enable eq !TRUE) then begin
        value='Time profiles are computed by summing over voxels inside the box with values above the threshold specified by the "Set Threshold" button.'+'  The threshold is computed as the specified fractional threshold times the maximum voxel within the box.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.time_type = !THRESH_TPROF
    widget_control,wd.t_prof_max_pix,SENSITIVE=0
    widget_control,wd.t_prof_plain,/SENSITIVE
    widget_control,wd.t_gang,/SENSITIVE
end

wd.thresh_pct: begin
    if(help.enable eq !TRUE) then begin
        value='Set the threshold used by the time profile command in the "Sum over Thresholded Region" mode.  The threshold is expressed as a percent of the maximum value in the region.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.time_thresh_type = !PERCENT
    value = '50'
    label = 'Threshold as percent of maximum.'
    thresh = get_str(1,label,value)
    pr.time_thresh = float(thresh(0))/100.
end

wd.thresh_sig: begin
    if(help.enable eq !TRUE) then begin
        value='Set the threshold used by the time profile command in the "Sum over Thresholded Region" mode.' + '  The threshold should be the minimum log-significance-probability.' + ' The absolute value of the significance probability will be tested against this threshold.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.time_thresh_type = !SIGNIFICANCE
    value = '5'
    label = 'Threshold as log-probability'
    thresh = get_str(1,label,value)
    pr.time_thresh = float(thresh(0))
end

wd.subtract_trend: begin
    if(help.enable eq !TRUE) then begin
        value='Subtract linear trend from time profile before plotting.'
        widget_control,help.id,SET_VALUE=value
    endif
    handle_value,fi.hdr_handle(fi.n),hdr
    if(trend.slope_handle(hdr.mother) eq 0) then $
        linfit,hdr.mother,fi,pr,dsp,wd,stc,help
    pr.subtract_trend = !TRUE
    widget_control,wd.no_trend,/SENSITIVE
    widget_control,wd.subtract_trend,SENSITIVE=0
end

wd.no_trend: begin
    if(help.enable eq !TRUE) then begin
        value='Dont subtract linear trend from time profile before plotting.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.subtract_trend = !FALSE
    widget_control,wd.no_trend,SENSITIVE=0
    widget_control,wd.subtract_trend,/SENSITIVE
end

wd.prof_wht: pr.color = !WHITE_INDEX
wd.prof_ylw: pr.color = !YELLOW_INDEX
wd.prof_red: pr.color = !RED_INDEX
wd.prof_grn: pr.color = !GREEN_INDEX
wd.prof_blu: pr.color = !BLUE_INDEX

wd.prbox_smooth0: begin
    if(help.enable eq !TRUE) then begin
        value='Dont smooth time profiles.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.boxcar_width = 0
end

wd.prbox_smooth3: begin
    if(help.enable eq !TRUE) then begin
        value='Smooth time profiles with a 3 point boxcar filter.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.boxcar_width = 3
end

wd.prbox_smooth5: begin
    if(help.enable eq !TRUE) then begin
        value='Smooth time profiles with a 5 point boxcar filter.'
        widget_control,help.id,SET_VALUE=value
    endif
    pr.boxcar_width = 5
end

wd.user1: begin
    user1,fi,pr,dsp,wd,stc,help
end

wd.user2: begin
    user2,fi,pr,dsp,wd,stc,help
end

wd.user3: begin
    user3,fi,pr,dsp,wd,stc,help
end

wd.user4: begin
    user4,fi,pr,dsp,wd,stc,help
end

wd.user5: begin
    user5,fi,pr,dsp,wd,stc,help
end

wd.user6: begin
    user6,fi,pr,dsp,wd,stc,help
end

wd.user7: begin
    user7,fi,pr,dsp,wd,stc,help
end

wd.user8: begin
    user8,fi,pr,dsp,wd,stc,help
end

wd.user9: begin
    user9,fi,pr,dsp,wd,stc,help
end

wd.user10: begin
    user10,fi,pr,dsp,wd,stc,help
end

else: print,'*** Unknown event encountered in stats_exec_event. ***

endcase

return
end


;*************
pro stats_exec
;*************

common stats_comm

common linear_fit

device,pseudo_color=8  ; Use pseudo-color on ultra creators.

@stats.h
@stats_init.h

loadct,!GRAY_SCALE,NCOLORS=!NUM_COLORS
fi.color_max = 100
fi.color_gamma = 1.
set_colors ;Setup plotting colors and color table length.

fi.tophand = handle_create()
pr.tophand = handle_create()
set_plot,'X'
imgtype = !CTI
dsp.img_minmax(1) = 255
fl.custom = !FALSE
cmd = string('pwd')
spawn,cmd,str
fi.path = str(0)
wd.oprof = -12345	; Nonsense widget value.
pr.prof_on = !PROF_NO
fit_name = 'null'
path = getenv('HOME')
fi.printfile = path + '/idl.ps'

wd.leader        = widget_base(title='fMRI Statistics',/COLUMN)
menrow      = widget_base(wd.leader,/ROW)
;drawrow     = widget_base(wd.leader,/ROW)
butrow      = widget_base(wd.leader,/ROW)
filerow     = widget_base(wd.leader,/COLUMN)

mload       = widget_button(menrow,value='Files',MENU=3)
mproc       = widget_button(menrow,value='Process',MENU=3)
mdisp       = widget_button(menrow,value='Display',MENU=3)
mprof       = widget_button(menrow,value='Profiles',MENU=3)
wd.refresh  = widget_button(menrow,value='Refresh')
mhelp       = widget_button(menrow,value='Help',MENU=3)
wd.help_info= widget_button(mhelp,value='Display help window')
wd.help     = widget_button(mhelp,value='Support')
;mroi       = widget_button(menrow,value='ROI',MENU=3)
;wd.repeat_action = widget_button(menrow,value='Repeat')

;wd.mldcti  = widget_button(mload,value='Load CTI',MENU=3)
mldmri      = widget_button(mload,value='Load MRI',MENU=3)
;wd.ldmri_analyze = widget_button(mldmri,value='Load Analyze x-y-t')
wd.ldmri_4dfp = widget_button(mldmri,value='Load 4dfp')
wd.ldmri_many_4dfp = widget_button(mldmri,value='Load several 4dfp')
wd.stitch    = widget_button(mldmri,value='Concatenate 4dfps')
wd.ld_t88   = widget_button(mload,value='Load T88')
wd.ldraw    = widget_button(mload,value='Load Unformatted')
wd.delete   = widget_button(mload,value='Delete image')
wd.wrtgif   = widget_button(mload,value='Write GIF image')
wd.wrtraw   = widget_button(mload,value='Write 4dfp')
wd.printany = widget_button(mload,value='Print any window')
wd.exit     = widget_button(mload,value='Exit')

wd.hprof    = widget_button(mprof,value='Horzontal Profile')
wd.vprof    = widget_button(mprof,value='Vertical Profile')
wd.zprof    = widget_button(mprof,value='Time Profile')
wd.roiprof  = widget_button(mprof,value='Profile region')
profsize    = widget_button(mprof,value='Size',MENU=3)
wd.profwidth= widget_button(profsize,value='Width')
wd.profhght = widget_button(profsize,value='Height')
wd.t_box_size = widget_button(profsize,value='Square')
wd.profsave = widget_button(mprof,value='Save Profiles')
wd.prof_fft   = widget_button(mprof,value='Profile FFT')
;wd.gaussfit  = widget_button(mprof,value='Fit Gaussian')
wd.select_prof = widget_button(mprof,value='Select profile')
;wd.annotate_prof = widget_button(mprof,value='Annotate profile')
wd.kill_all_prof = widget_button(mprof,value='Kill all profiles')
wd.print_prof = widget_button(mprof,value='Print profile')
wsymbol     = widget_button(mprof,value='Format',MENU=3)
wd.profsym   = widget_button(wsymbol,value='Symbols Only')
wd.profline  = widget_button(wsymbol,value='Lines Only')
wd.profsymline = widget_button(wsymbol,value='Lines and Symbols')
wpr_tproftyp  = widget_button(mprof,value='Time profile mode',MENU=3)
wd.t_prof_plain  = widget_button(wpr_tproftyp,value='Simple sum')
widget_control,wd.t_prof_plain,SENSITIVE=0
wd.t_prof_max_pix= widget_button(wpr_tproftyp,value='Sum over thresholded region')
threshold = widget_button(wpr_tproftyp,value='Set threshold',MENU=3)
wd.thresh_pct = widget_button(threshold,value='Percent of Maximum')
wd.thresh_sig = widget_button(threshold,value='Significance')
wd.subtract_trend = widget_button(wpr_tproftyp,value='Subtract trend')
wd.no_trend = widget_button(wpr_tproftyp,value=string("Don't subtract trend"))
widget_control,wd.no_trend,SENSITIVE=0
wd.t_gang  = widget_button(wpr_tproftyp,value='Ganged array')
wd.phisto   = widget_button(wpr_tproftyp,value='Histogram')
wd.gang_dim  = widget_button(wpr_tproftyp,value='Ganged Dimension ')
wd.gang_subdim  = widget_button(wpr_tproftyp,value='Ganged Pixel Size')
widget_control,wd.gang_dim,SENSITIVE=0
widget_control,wd.gang_subdim,SENSITIVE=0
wpr_color     = widget_button(mprof,value='Color',MENU=3)
wd.prof_wht   = widget_button(wpr_color,value='White')
wd.prof_ylw   = widget_button(wpr_color,value='Yellow')
wd.prof_red   = widget_button(wpr_color,value='Red')
wd.prof_grn   = widget_button(wpr_color,value='Green')
wd.prof_blu   = widget_button(wpr_color,value='Blue')
wprbox_smooth = widget_button(mprof,value='Boxcar smooth',MENU=3)
wd.prbox_smooth0 = widget_button(wprbox_smooth,value='None')
wd.prbox_smooth3 = widget_button(wprbox_smooth,value='Width: 3')
wd.prbox_smooth5 = widget_button(wprbox_smooth,value='Width: 5')

wd.plnstats = widget_button(mproc,value='Image stats')
wd.sinecorr = widget_button(mproc,value='Harmonics')
;wd.meanvar = widget_button(mproc,value='Mean and Variance')
wd.linfit = widget_button(mproc,value='Fit linear trend')
wd.kolmo_smrnov = widget_button(mproc,value='Compute K-S Statistic')
wd.wilcoxon = widget_button(mproc,value='Compute Wilcoxon Statistic')
wd.diffimg  = widget_button(mproc,value='Difference Images')
wd.actmap  = widget_button(mproc,value='Run actmapf_4dfp')
;wd.covariance  = widget_button(mproc,value='Estimate covariance matrix')
;wd.smoothness  = widget_button(mproc,value='Estimate smoothness')
;wd.time_corr  = widget_button(mproc,value='Estimate time correlation')
wd.mult_comp  = widget_button(mproc,value='Correct for multiple comparisons')
mstim      = widget_button(mproc,value='Stimulus time-course',MENU=3)
wd.define_stimulus  = widget_button(mstim,value='Define')
wd.edit_stimulus  = widget_button(mstim,value='Edit')
wd.show_stimulus  = widget_button(mstim,value='Display')
wd.save_stimulus  = widget_button(mstim,value='Save to ifh header')
msmoth      = widget_button(mproc,value='Smooth',MENU=3)
wd.smobox   = widget_button(msmoth,value='Boxcar filter')
wd.overlay_actmap= widget_button(mproc,value='Overlay activation map')
;wd.test     = widget_button(mproc,value='Test')
;;;wd.test1     = widget_button(mproc,value='Test 1')
user_buttons,mproc,wd

wd.loadct   = widget_button(mdisp,value='Color Tables')
wd.cine     = widget_button(mdisp,value='Cine')
wd.zoomin   = widget_button(mdisp,value='Zoom in')
wd.zoomout  = widget_button(mdisp,value='Zoom out')
mscale      = widget_button(mdisp,value='Scaling',MENU=3)
wd.display_all= widget_button(mdisp,value='Display all planes')
wd.view3d = widget_button(mdisp,value='Display in 3D')
wd.print_all= widget_button(mdisp,value='Print all planes')
wd.color_scale= widget_button(mdisp,value='Display color scale')
wd.local_scl= widget_button(mscale,value='Local')
wd.global_scl= widget_button(mscale,value='Global')
widget_control,wd.global_scl,SENSITIVE=0
wd.fixed_scl= widget_button(mscale,value='Fixed')
wd.fixed_glbl_scl= widget_button(mscale,value='Fixed global')
mcustom= widget_button(mdisp,value='Custom Display',MENU=3)
wd.build_2x2= widget_button(mcustom,value='Build 2x2')
wd.build_custom= widget_button(mcustom,value='Build custom size')
wd.build_addimg= widget_button(mcustom,value='Add image')
wd.build_annotate= widget_button(mcustom,value='Annotate display')
wd.select_custom= widget_button(mcustom,value='Select custom display')
;wd.mroitype = widget_button(mroi,value='Define ROI',MENU=3)

;wd.draw = widget_draw(drawrow,X_SCROLL_SIZE=!DRAW_SCROLL, $
;        Y_SCROLL_SIZE=!DRAW_SCROLL,/BUTTON_EVENTS,COLORS=!LEN_COLORTAB, $
;	XSIZE=!DRAW_SIZE,YSIZE=!DRAW_SIZE,RETAIN=2)
;widget_control,GET_VALUE=i,wd.draw
;dsp.image_index = i
;wset,dsp.image_index

wd.nextpln   = widget_button(butrow,value='Next Plane')
wd.lastpln    = widget_button(butrow,value='Prev Plane')
wd.nextfrm   = widget_button(butrow,value='Next Frame')
wd.lastfrm    = widget_button(butrow,value='Prev Frame')

wd.sld_pln = widget_slider(filerow,Title='Plane Number',SCROLL=1,FRAME=5)
wd.sld_frm = widget_slider(filerow,Title='Frame Number',SCROLL=1,FRAME=5)
wd.error   = widget_text(filerow,VALUE=' ')
wd.files   = widget_list(filerow,VALUE=fi.list,SCR_YSIZE=200,SCR_XSIZE=420)

widget_control,wd.leader,/REALIZE

xmanager,'stats_exec',wd.leader

end
