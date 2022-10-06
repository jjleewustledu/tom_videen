;**********************
function get_plnfrm,dim
;**********************

;Returns planes(2). planes(1) is minimum, planes(2) = maximum

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common getpln_comm,wpln_min,wpln_max,wok,plnfrm_sel,wfrm,maxfrm

maxpln = dim(2)
maxfrm = dim(3)
plnfrm_sel = intarr(3)
plnfrm_sel(*) = 0
getpln = widget_base(title='Planes',/COLUMN)

widget_control,wftyp,get_value=type 
if(type eq 'CTI File') then min = 1 else min = 0
max = maxpln+min-1
if(min eq max) then max = max - 1
wpln_min = widget_slider(getpln,Title='Starting Plane', $
					MINIMUM=min,MAXIMUM=max)
wpln_max = widget_slider(getpln,Title='Ending Plane', $
			MINIMUM=min,MAXIMUM=max,VALUE=max)
if(ftyp eq 'CTI') and (maxfrm gt 1) then $
    wfrm    = widget_slider(getpln,Title='Frame', $
			MINIMUM=1,MAXIMUM=maxfrm,VALUE=1)
wok = widget_button(getpln,value='OK',uvalue='done')

widget_control,getpln,/REALIZE
xmanager,'get_plnfrm',getpln,/MODAL

return,plnfrm_sel
end

;**********************
pro get_plnfrm_event,ev
;**********************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats


common getpln_comm,wpln_min,wpln_max,wok,plnfrm_sel,wfrm,maxfrm

if(ev.id eq wok) then  begin
    widget_control,wpln_min,GET_VALUE=min
    widget_control,wpln_max,GET_VALUE=max
    if(max lt 0) then max = 0
    plnfrm_sel(0) = min
    plnfrm_sel(1) = max
    if(ftyp eq 'CTI' and (maxfrm gt 1)) then begin
	widget_control,wfrm,GET_VALUE=frm
	plnfrm_sel(2) = frm
    endif else begin
	plnfrm_sel(2) = 1
    endelse
    widget_control,ev.top,/destroy
endif

end

;***********************
function get_profile,row
;***********************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

device, set_graphics=6             ;Set XOR mode
cursor,x,y,/WAIT,/DEV	;Get current position.
plots,[0,!DRAW_SIZE],[y,y],COLOR=!d.table_size-1,/DEV,/NOCLIP
widget_control,werror,SET_VALUE=string((!DRAW_SIZE -y)/zoom)
ym1 = y
print,'Depress rightmost button to plot profile.'
repeat begin
    cursor,x,y,/WAIT,/DEV	;Get current position.
    if((!DRAW_SIZE -y)/zoom ge ydim(0)-1) then $
	y = !DRAW_SIZE - (ydim(0) - 1)
    if(y ne ym1) then begin
        plots,[0,!DRAW_SIZE],[ym1,ym1],COLOR=!d.table_size-1,/DEV,/NOCLIP
        plots,[0,!DRAW_SIZE],[y,y],COLOR=!d.table_size-1,/DEV,/NOCLIP
        widget_control,werror,SET_VALUE=string((!DRAW_SIZE -y)/zoom)
	ym1 = y
    endif
endrep until !err eq 4
row = (!DRAW_SIZE -y)/zoom
prof = image(*,row,plane)
device,SET_GRAPHICS=3	;Resume normal graphics.

return,prof
end

;********************************
function roistats,verts,nvrts,pln
;********************************

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

stats = fltarr(3)
stats(*) = -1
if(nvrts gt 0) then begin
    pixels = polyfillv(verts(0,0,0:nvrts-1,0),verts(0,0,0:nvrts-1,1),$
							xdim(0),xdim(0))
endif else begin
    print,'Error in roistats, Plane: ',pln,' nvrts: ',nvrts
    return,stats
endelse

npix = n_elements(pixels)
row = pixels/xdim(0)
col = pixels - row*xdim(0)
sum = 0.
sumsq = 0
for i=0,npix-1 do begin
    sum = sum + image(col(i),row(i),pln)
    sumsq = sumsq + image(col(i),row(i),pln)^2
endfor

stats(0) = sum
stats(1) = sum/npix
var = (npix*sumsq - sum^2)/(npix*(npix-1))
stats(2) = sqrt(var)/abs(stats(1))

return,stats
end


;**********************
function get_roinum,x,y
;**********************

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol,$
	roinum,zoom

index = xdim(0)*(!DRAW_SIZE -  y)/zoom + x/zoom
for iroi=0,nroi(plane)-1 do begin
    pixels = polyfillv(roi(plane,iroi,0:nverts(plane,iroi)-1,0), $
	               roi(plane,iroi,0:nverts(plane,iroi)-1,1),xdim(0),xdim(0))
    for i = 0,n_elements(pixels)-1 do begin
	if(index eq pixels(i)) then return,iroi
    endfor
endfor

return,-1
end

;*******************
pro display_event,ev
;*******************

common disp_comm,wdisp_done

if(ev.id eq wdisp_done) then widget_control,ev.top,/destroy

end

;*******************
pro plot_event,ev
;*******************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

common plot_comm,wplot_done

display,image(*,*,plane),ZOOM=zoom
widget_control,woprof,SENSITIVE=0
if(ev.id eq wplot_done) then begin
    widget_control,wdone,/SENSITIVE
    widget_control,ev.top,/destroy
endif

end

;***************
pro roi_event,ev
;***************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

common disp_comm,wdisp_done

common plot_comm,wplot_done

common images,a,b,c,d,e,f,g,h,i,j,file_names,file_onlies,paths,nfiles

common volume_data,slicer_vol

type = tag_names(ev,/structure)

if(type eq 'WIDGET_BUTTON') then begin
    widget_control,ev.id,get_value=button

    if(ev.id eq wdone) then widget_control,ev.top,/destroy

    if(ev.id eq wprof) then begin 
	widget_control,wdone,SENSITIVE=0
	prof_on = !TRUE
	widget_control,wprof,SENSITIVE=0
	prof = get_profile(row)	; Interactively determine profile to plot.
	plotbase = widget_base(title='Profile',/COLUMN)
	wplot = widget_draw(plotbase,xsize=400,ysize=400,COLORS=2)
	wplot_done=widget_button(plotbase,VALUE='Exit',XSIZE=8)

	widget_control,plotbase,/REALIZE
	xmanager,'plot',plotbase
	widget_control,GET_VALUE=plot_index,wplot
	wset,plot_index
	xr=fltarr(2)
	xr(0) = 0
	xr(1) = xdim(0)
	subtitle = string(FORMAT='(a,": ",i3)',fileonly,row)
	plot,prof,BACKGROUND=!d.n_colors-1,COLOR=0,XRANGE=xr,FONT=0,XSTYLE=1, $
	     SUBTITLE=subtitle,XTITLE=string('Column number'),$
	     YTITLE=string('Image value')
	prof(*) = 0
	oplot,prof,COLOR=0
	wset,image_index

	widget_control,wprof,/SENSITIVE
	widget_control,woprof,/SENSITIVE
    endif

    if(ev.id eq woprof) then begin 
	prof = get_profile(row)	; Interactively determine profile to plot.
	wset,plot_index
	subtitle = string(FORMAT='(a,": ",i3)',fileonly,row)
	oplot,prof,psym=6,COLOR=0,SYMSIZE=.5
	wset,image_index
    endif

    if(ev.id eq wstats) then begin 
	min = min(image(*,*,plane))
	max = max(image(*,*,plane))
	sum = total(image(*,*,plane))
        print,filnam
	print,'Plane: ',plane,'  Minimum: ',min,'  Maximum: ',max,'  Sum: ',sum
    endif

    if(ev.id eq wsmooth) then begin 
        widget_control,/HOURGLASS
	x = total(image)
	image = smooth(image,3)
	y = total(image)
        widget_control,HOURGLASS=0
	display,image(*,*,plane),ZOOM=zoom
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
    endif
    endif
    
    if(button eq 'Refresh Image') then display,image(*,*,plane),ZOOM=zoom

;    if(ev.id eq wzoomin) then begin
;	zoom = 2*zoom
;	display,image(*,*,plane),ZOOM=zoom
;	for iroi=0,nroi(plane)-1 do begin
;	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
; 	endfor
;    endif

;    if(ev.id eq wzoomout) then begin
;	zoom = .5*zoom
;	display,image(*,*,plane),ZOOM=zoom
;	for iroi=0,nroi(plane)-1 do begin
;	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
; 	endfor
;    endif

    if(ev.id eq wdreg) then begin
;	Draw regions.
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
    endif

    if(ev.id eq wftyp) then begin
	; Select file type.
	widget_control,wftyp,get_value=type 
	if(type eq 'CTI File') then begin
	    widget_control,wftyp,SET_VALUE=string('Cube File')
	    ftyp = 'Cube'
	endif else begin
	    widget_control,wftyp,set_value=string('CTI File')
	    ftyp = 'CTI'
	endelse
    endif
    
    if(ev.id eq wsave) then begin
	; Save regions.
        cmd = string('set y = ',filnam,'; echo $y:r\.reg')
	spawn,cmd,roifile
	openw,lusave,roifile(0),/GET_LUN
	writeu,lusave,maxpln
	writeu,lusave,nroi
	writeu,lusave,nverts
	writeu,lusave,roi
	close,lusave
    endif

    if(ev.id eq wload) then begin
	; Load regions.
	roifile = pickfile(/read,FILTER='*.reg',PATH=regpath(0),/NOCONFIRM)
	if(roifile eq '') then return
        cmd = string('set y = ',filnam,'; echo $y:h')
	spawn,cmd,regpath
        cd,regpath(0)
	openu,luload,roifile(0),/GET_LUN
	readu,luload,maxpln
	readu,luload,nroi
	readu,luload,nverts
	readu,luload,roi
	close,luload
        display,image(*,*,plane),ZOOM=zoom
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
	widget_control,wvol,/SENSITIVE
	widget_control,wsave,/SENSITIVE
    endif

    if(ev.id eq wsreg) then begin
;	Sum regions.
	for pln=0,maxpln(0)-1 do begin
	    if(ftyp eq 'CTI') then jpln = pln + 1 else jpln = pln
 	    for iroi=0,nroi(pln)-1 do begin
	        stats = roistats(roi(pln,iroi,*,*),nverts(pln,iroi),pln)
		print,'File: ',filnam,'  Plane: ',jpln
		print,' ROI: ',iroi,' Vol: ',vol(pln,iroi),' Sum: ',stats(0), $
		' Mean: ',stats(1),' CV: ',stats(2)
	    endfor
	endfor
    endif

    if(ev.id eq wdef) then begin
;	Define region.
	roinum = nroi(plane)
        widget_control,wroinum,set_value=string('Region: ',roinum)
	verts =  defroi(!DRAW_SIZE-2,!DRAW_SIZE-2,/NOFILL)
	nverts(plane,roinum) = n_elements(verts(*,0))
	roi(plane,roinum,0:nverts(plane,roinum)-1,*) = verts(*,*)
	nc1 = !d.table_size-1
	roi(plane,roinum,*,1) = !DRAW_SIZE - roi(plane,roinum,*,1)
        roi(plane,roinum,*,*) = roi(plane,roinum,*,*)/zoom

        draw_roi,roi(plane,roinum,*,*),nverts(plane,roinum)
	nroi(plane) = nroi(plane) + 1
	widget_control,wvol,/SENSITIVE
	widget_control,wsave,/SENSITIVE
    endif
    
    if(ev.id eq wdel) then begin ;Delete Region
      case 1 of
	(nroi(plane) lt 1): $
	    widget_control,werror,set_value=string('No regions defined') ;
	(nroi(plane) eq 1): begin
	    nroi(plane) = 0
    	    display,image(*,*,plane),ZOOM=zoom
	    for iroi=0,nroi(plane)-1 do begin
	        draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	    endfor
 	end
	(nroi(plane) gt 1): begin
	    nroi(plane) = nroi(plane) - 1
	    roi(plane,roinum,*,*) = roi(plane,nroi(plane),*,*)
	    nverts(plane,roinum) =  nverts(plane,nroi(plane))
	    vol(plane,roinum) = vol(plane,nroi(plane))
    	    display,image(*,*,plane),ZOOM=zoom
	    for iroi=0,nroi(plane)-1 do begin
	        draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	    endfor
	end
      endcase
    endif

    if(ev.id eq wdelall) then begin ;Delete all regions.
	for pln=0,maxpln(0)-1 do begin
	    nroi(pln) = 0
	endfor
    	display,image(*,*,plane),ZOOM=zoom
	widget_control,wvol,SENSITIVE=0
	widget_control,wsave,SENSITIVE=0
    endif

    if(button eq 'Color Tables') then begin
	xloadct
    endif

    if(button eq 'Load Image') then begin
	load_image
    endif

    if(ev.id eq wslicer) then begin
	slicer_vol = image
	slicer
    endif

    if(ev.id eq wsurf) then begin
	xsurface,image(*,*,plane)
    endif

    if(ev.id eq wdispglob) then begin
	disp = widget_base(title=filnam,/COLUMN)
        zoom1 = 128/xdim(0)
	dx = xdim(0)*zoom1
 	dy = ydim(0)*zoom1
	ncol = 1024/dx
	nrow = maxpln(0)/ncol
	if(nrow*ncol ne maxpln(0)) then nrow = nrow + 1
	wimg = widget_draw(disp,xsize=ncol*dx+2,ysize=nrow*dy+2)
	wdisp_done=widget_button(disp,value='Exit',XSIZE=8)

	widget_control,disp,/REALIZE
	widget_control,GET_VALUE=index,wimg
	wset,index
        display_glob,image,ZOOM=zoom1
	xmanager,'display',disp
	widget_control,GET_VALUE=index,wdraw
	wset,index
     endif

    if(ev.id eq wdisplay) then begin
	disp = widget_base(title=filnam,/COLUMN)
        zoom1 = 128/xdim(0)
	dx = xdim(0)*zoom1
 	dy = ydim(0)*zoom1
	ncol = 1024/dx
	nrow = maxpln(0)/ncol
	if(nrow*ncol ne maxpln(0)) then nrow = nrow + 1
	wimg = widget_draw(disp,xsize=ncol*dx+2,ysize=nrow*dy+2)
	wdisp_done=widget_button(disp,value='Exit',XSIZE=8)

	widget_control,disp,/REALIZE
	widget_control,GET_VALUE=index,wimg
	wset,index
        display,image,ZOOM=zoom1
	xmanager,'display',disp
	widget_control,GET_VALUE=index,wdraw
	wset,index
    endif

endif else begin

    if(type eq 'WIDGET_SLIDER') then begin 
        widget_control,wsld_pln,get_value=plane 
	if(ftyp eq 'CTI') then pln = plane + 1 else pln = plane
        widget_control,wplane,set_value=string('Plane: ',pln) 
        if(ftyp eq 'CTI') then plane = plane - 1
	if(plane lt 0) then plane = 0
	if(xdim(0) eq ydim(0)) then begin
	    zoom = !DRAW_SIZE/xdim(0)
        endif else begin
	    if(xdim(0) ge ydim(0)) then zoom = !DRAW_SIZE/xdim(0) $
	    else zoom = !DRAW_SIZE/ydim(0)
	endelse
        display,image(*,*,plane),ZOOM=zoom
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
    endif

    if(ev.id eq wdraw) then begin 
        if(ev.type eq 0) then begin
	    roinum = get_roinum(ev.x,ev.y)
	    if(roinum ge 0) then begin
                widget_control,wroinum,set_value=string('Region: ',roinum) 
                widget_control,wvol,set_value=string(vol(plane,roinum))
	        widget_control,werror,set_value=string(' ') ; Clear error message.
	    endif else begin
	 	x = ev.x/zoom - 1
		y = (!DRAW_SIZE - ev.y)/zoom - 1
		if(x gt xdim(0)-1) then x = xdim(0) - 1
		if(y gt ydim(0)-1) then y = ydim(0) - 1
		val = string('(',x,',',y,'): ',image(x,y,plane))
	        widget_control,werror,SET_VALUE=val
	    endelse
	endif
    endif

    if(ev.id eq wvol) then begin 
print,plane,'roinum: ',roinum
        widget_control,ev.id,get_value=svol
	vol(plane,roinum) = fix(svol)
    endif

    if(ev.id eq wfiles) then begin
	get_image,file_names(ev.index)
	widget_control,wpln_file,set_value=string('File: ',file_onlies(ev.index))
	widget_control,wpln_path,set_value=string('File: ',paths(ev.index))
	display,image(*,*,plane),ZOOM=zoom
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
    endif
    
endelse


end


;***********************
pro draw_roi,verts,nvrts
;***********************

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

nc1 = !d.table_size-1
for n=0,nvrts-2 do begin
    plots, [verts(0,0,n,0),verts(0,0,n+1,0)]*zoom, $
           [!DRAW_SIZE-zoom*verts(0,0,n,1),!DRAW_SIZE-zoom*verts(0,0,n+1,1)],$
           /DEVICE,COLOR=nc1,/NOCLIP
endfor
plots, [verts(0,0,0,0),verts(0,0,nvrts-1,0)]*zoom, $
       [!DRAW_SIZE-zoom*verts(0,0,0,1),!DRAW_SIZE-zoom*verts(0,0,nvrts-1,1)],$
       /DEVICE,COLOR=nc1,/NOCLIP        ;Complete polygon

end


;******
pro roi
;******

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common images,a,b,c,d,e,f,g,h,i,j,file_names,file_onlies,paths,nfiles

path = '.'
regpath = '.'
file_names = strarr(!NUM_FILES)
file_onlies = strarr(!NUM_FILES)
paths = strarr(!NUM_FILES)
nfiles = 0
prof_on = !FALSE
ftyp = 'CTI'

base = widget_base(title='Regions of Interest',/row)
lcol = widget_base(base,/column)
mcol = widget_base(base,/column)
rcol = widget_base(base,/column)

;Define leftmost column of widgets.
wcolor    = widget_button(lcol,value='Color Tables')
wprof     = widget_button(lcol,value='Profile',uvalue='done')
woprof    = widget_button(lcol,value='Overplot',uvalue='done')
wdisp     = widget_button(lcol,value='Refresh Image')
wzoomin   = widget_button(lcol,value='Zoom In')
wzoomout  = widget_button(lcol,value='Zoom Out')
;wdisplay  = widget_button(lcol,value='Display Images')
;wdispglob = widget_button(lcol,value='Display Global')
wsmooth   = widget_button(lcol,value='Smooth Images')
wslicer   = widget_button(lcol,value='Slicer')
wsurf     = widget_button(lcol,value='Surface Plot')
wstats     = widget_button(lcol,value='Image Stats')

wdraw = widget_draw(mcol,X_SCROLL_SIZE=!DRAW_SIZE_TOT,XSIZE=300,YSIZE=300, $
	Y_SCROLL_SIZE=!DRAW_SIZE_TOT,/BUTTON_EVENTS,/SCROLL)
werror    = widget_text(mcol,VALUE=' ')
wpln_path = widget_text(mcol,VALUE='Path: .')
wpln_file = widget_text(mcol,VALUE='No file selected')
wsld_pln  = widget_slider(mcol,Title='Plane Number')
wfiles    = widget_list(mcol,VALUE=file_onlies,YSIZE=4)
wpick     = widget_button(mcol,value='Load Image')
wftyp     = widget_button(mcol,value='CTI File')

wplane    = widget_label(rcol,value='Plane: 0')
wroinum   = widget_label(rcol,value='Region: -1')
wvol      = cw_field(rcol,TITLE='Volume: ',VALUE='-1',/RETURN_EVENTS,XSIZE=4)

wdef      = widget_button(rcol,value='Define ROI')
wdel      = widget_button(rcol,value='Delete ROI')
wdelall   = widget_button(rcol,value='Delete All ROIs')
wdreg     = widget_button(rcol,value='Refresh ROIs')
wsreg     = widget_button(rcol,value='Sum ROIs')
wload     = widget_button(rcol,value='Load ROIs')
wsave     = widget_button(rcol,value='Save ROIs')
wdone     = widget_button(rcol,value='Exit')

widget_control,wdef,SENSITIVE=0
widget_control,wdel,SENSITIVE=0
widget_control,wdisp,SENSITIVE=0
widget_control,wdreg,SENSITIVE=0
widget_control,wsreg,SENSITIVE=0
widget_control,wvol,SENSITIVE=0
widget_control,wsld_pln,SENSITIVE=0
widget_control,wsave,SENSITIVE=0
widget_control,wdisplay,SENSITIVE=0
widget_control,wprof,SENSITIVE=0
widget_control,woprof,SENSITIVE=0
widget_control,wsmooth,SENSITIVE=0
widget_control,wslicer,SENSITIVE=0
widget_control,wdispglob,SENSITIVE=0
widget_control,wsurf,SENSITIVE=0
widget_control,wstats,SENSITIVE=0

widget_control,base,/realize

widget_control,GET_VALUE=image_index,wdraw
wset,image_index

xmanager,'roi',base

end

;*************
pro load_image
;*************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,VOL, $
	roinum,zoom

common images,a,b,c,d,e,f,g,h,i,j,file_names,file_onlies,paths,nfiles

if(ftyp eq 'Cube') then begin
;    File is in cube format.
    filnam = pickfile(/read,FILTER='*h0*.cub',PATH=path(0),/NOCONFIRM)
    if(filnam eq '') then return

    ; Get number of planes.
    cmd = string('cubparam ',filnam,' 0 zdim')
    spawn,cmd,nz
    zdim = fix(nz)

    cmd = string('cubparam ',filnam,' 0 zdim')
    spawn,cmd,smaxpln
    maxpln = fix(smaxpln)
    cmd = string('cubparam ',filnam,' 0 xdim')
    spawn,cmd,sxdim
    xdim = fix(sxdim)
    cmd = string('cubparam ',filnam,' 0 ydim')
    spawn,cmd,sydim
    ydim = fix(sydim)

    dim = intarr(4)
    dim(0) = xdim(0)
    dim(1) = ydim(0)
    dim(2) = zdim(0)
    dim(3) = 1
    plnfrm_sel = get_plnfrm(dim)
    maxpln = plnfrm_sel(1)
    minpln = plnfrm_sel(0)
    widget_control,/HOURGLASS
    rd_cub,filnam,image,MIN_PLANE=minpln(0),MAX_PLANE=maxpln(0)
    widget_control,HOURGLASS=0
endif else begin
;   Assume this is a CTI file.
    filnam = pickfile(/read,FILTER='*.img',PATH=path(0),/NOCONFIRM)
    if(filnam eq '') then return
    dim = get_cti_dim(filnam)
    plnfrm_sel = get_plnfrm(dim)
    maxpln = plnfrm_sel(1)
    minpln = plnfrm_sel(0)
    widget_control,/HOURGLASS
    rd_cti,filnam,image,xdim,ydim,zdim, $
	MIN_PLANE=minpln(0),MAX_PLANE=maxpln(0),FRAME=plnfrm_sel(2)
   widget_control,HOURGLASS=0
endelse

case nfiles of

0: begin
       a = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

1: begin
       b = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

2: begin
       c = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

3: begin
       d = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

4: begin
       e = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

5: begin
       f = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

6: begin
       g = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

7: begin
       h = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

8: begin
       i = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

9: begin
       j = {name:'',xdim:xdim,ydim:ydim,zmax:maxpln(0),zmin:minpln(0), $
					img:image(*,*,*)}
   end

else: print,'*** Maximum number of opened files exceeded. ***'

endcase

;Setup path for next time file is loaded. 
cmd = string('set y = ',filnam,'; echo $y:t')
spawn,cmd,fileonly
cmd = string('set y = ',filnam,'; echo $y:h')
spawn,cmd,path
cd,path(0)

file_names(nfiles) = filnam
file_onlies(nfiles) = fileonly
paths(nfiles) = path
nfiles = nfiles + 1
widget_control,wfiles,SET_VALUE=file_onlies

roi = intarr(maxpln(0)+1,10,200,2)
nverts = intarr(maxpln(0)+1,10)
nroi = intarr(256)
vol = intarr(maxpln(0)+1,10)
vol(*,*) = -1

widget_control,wpln_path,set_value=string('Path: ',path)
widget_control,wpln_file,set_value=string('File: ',fileonly)
if(plnfrm_sel(0) eq plnfrm_sel(1)) then plnfrm_sel(1) = plnfrm_sel(1) - 1
widget_control,wsld_pln,SET_SLIDER_MIN=plnfrm_sel(0),SET_SLIDER_MAX=plnfrm_sel(1)
widget_control,wsld_pln,SET_VALUE=plnfrm_sel(0)
if(ftyp eq 'CTI') then plane = plnfrm_sel(0) - 1 else plane = plnfrm_sel(0)

if(xdim(0) eq ydim(0)) then begin
    zoom = !DRAW_SIZE/xdim(0)
endif else begin
    if(xdim(0) gt ydim(0)) then zoom = !DRAW_SIZE/xdim(0) $
    else zoom = !DRAW_SIZE/ydim(0)
endelse
display,image(*,*,plane),ZOOM=zoom

; Enable buttons.
widget_control,wdef,/SENSITIVE
widget_control,wdel,/SENSITIVE
widget_control,wdelall,/SENSITIVE
widget_control,wdisp,/SENSITIVE
widget_control,wdreg,/SENSITIVE
widget_control,wsreg,/SENSITIVE
widget_control,wdisplay,/SENSITIVE
widget_control,wsld_pln,/SENSITIVE
widget_control,wprof,/SENSITIVE
widget_control,wsmooth,/SENSITIVE
widget_control,wslicer,/SENSITIVE
widget_control,wdispglob,/SENSITIVE
widget_control,wsurf,/SENSITIVE
widget_control,wstats,/SENSITIVE

return
end

;**********************
pro get_image,file_name
;**********************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdef,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wftyp,ftyp,wdelall,wdisplay,wdraw,wfiles,wprof,prof_on,image_index, $
       plot_index,woprof,lcol,wsmooth,wdone,wslicer,wdispglob,wsurf,fileonly, $
       wstats

common images,a,b,c,d,e,f,g,h,i,j,file_names,file_onlies,paths,nfiles

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
	roinum,zoom

ifile = -1
for i=0,nfiles-1 do begin
    if(file_name eq file_names(i)) then begin
	ifile = i
	i = !NUM_FILES
    endif
endfor
if(ifile lt 0) then return

filnam = file_name

case ifile of

0: begin
       xdim = a.xdim
       ydim = a.ydim
       minpln = a.zmin
       maxpln = a.zmax
       image = a.img(*,*,*)
   end

1: begin
       xdim = b.xdim
       ydim = b.ydim
       minpln = b.zmin
       maxpln = b.zmax
       image = b.img(*,*,*)
   end

2: begin
       xdim = c.xdim
       ydim = c.ydim
       minpln = c.zmin
       maxpln = c.zmax
       image = c.img(*,*,*)
   end

3: begin
       xdim = d.xdim
       ydim = d.ydim
       minpln = d.zmin
       maxpln = d.zmax
       image = d.img(*,*,*)
   end

4: begin
       xdim = e.xdim
       ydim = e.ydim
       minpln = e.zmin
       maxpln = e.zmax
       image = e.img(*,*,*)
   end

5: begin
       xdim = f.xdim
       ydim = f.ydim
       minpln = f.zmin
       maxpln = f.zmax
       image = f.img(*,*,*)
   end

6: begin
       xdim = g.xdim
       ydim = g.ydim
       minpln = g.zmin
       maxpln = g.zmax
       image = g.img(*,*,*)
   end

7: begin
       xdim = h.xdim
       ydim = h.ydim
       minpln = h.zmin
       maxpln = h.zmax
       image = h.img(*,*,*)
   end

8: begin
       xdim = i.xdim
       ydim = i.ydim
       minpln = i.zmin
       maxpln = i.zmax
       image = i.img(*,*,*)
   end

9: begin
       xdim = j.xdim
       ydim = j.ydim
       minpln = j.zmin
       maxpln = j.zmax
       image = j.img(*,*,*)
   end

else: print,'*** Maximum number of opened files exceeded. ***'

endcase

zoom = !DRAW_SIZE/ydim(0)

; Ensure plane number in range.
if(ftyp eq 'CTI') then pln = plane + 1 else pln = plane
if(pln gt maxpln(0)-1) then pln = maxpln(0)-1
if(pln lt minpln(0))   then pln = minpln(0)
if(ftyp eq 'CTI') then plane = pln - 1 

if(minpln eq maxpln) then mx = mx - 1 else mx = maxpln
widget_control,wsld_pln,SET_SLIDER_MIN=minpln,SET_SLIDER_MAX=mx
widget_control,wsld_pln,SET_VALUE=pln

return
end
