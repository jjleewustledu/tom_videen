;***********************************************************************
pro put_image,image,fi,wd,dsp,file,xdim,ydim,zdim,tdim,num_type,dxdy,dz,scl, $
	type,mother,paradigm
;***********************************************************************

;Store image into data structures.

; fi: File data structure.
; filnam: Complete file name.
; xdim: Horizontal dimension.
; ydim: Vertical dimension.
; zdim: Depth dimension.
; tdim: Time dimension.
; num_type: Number type, can be !FLOAT or !SHORT
; dxdy: Pixel size.
; dz: Slice thickness
; scl: Scale factor to be applied to image.

pos = strpos(file,".4d")
if(pos gt 0) then $
    filnam = strmid(file,0,pos) $
else $
    filnam = file

; Increment pointers to lists. 
fi.n = fi.nfiles
fi.nfiles = fi.nfiles + 1

; Store info into structures.
fi.names(fi.n) = filnam
fi.name = filnam
if(mother lt 0) then $
    mother = fi.n
xx = {name:'',xdim:xdim(0),ydim:ydim(0),zdim:zdim(0),tdim:tdim(0), $
		type:num_type,dxdy:dxdy,dz:dz,scl:1.,array_type:type, $
		mother:mother}
fi.hdr_handle(fi.n) = handle_create(fi.tophand,VALUE=xx)

if(type ne !STITCH_ARRAY) then begin
    pos = rstrpos(filnam,'/')
    len = strlen(filnam)
    if(pos ge 0) then $
        tail = strmid(filnam,pos+1,len-pos-1) $
    else $
        tail = filnam
    fi.paths(fi.n) = fi.path
    fi.tails(fi.n) = tail
    fi.list(fi.n) = tail
    fi.tail = tail
    fi.data_handle(fi.n) = handle_create(fi.tophand,VALUE=image)
endif else begin
    pos = rstrpos(filnam,'/')
    len = strlen(filnam)
    if(pos ge 0) then $
        tail = strmid(filnam,pos+1,len-pos-1) $
    else $
        tail = filnam
    fi.tails(fi.n) = tail
    fi.list(fi.n) = tail
    fi.tail = tail
endelse

if(paradigm(0) gt -2) then begin
    fi.paradigm(fi.n,0:tdim-1) = paradigm(0:tdim-1)
    fi.paradigm_loaded(fi.n) = !TRUE
endif else begin
    fi.paradigm_loaded(fi.n) = !FALSE
endelse

if(type ne !OVERLAY_ARRAY) then $
    fi.zoom(fi.n) = 256/float(xdim(0)) $
else $
    fi.zoom(fi.n) = 1
dsp.zoom = fi.zoom(fi.n)
dsp.xdim = xdim(0)
dsp.ydim = ydim(0)
dsp.zdim = zdim(0)
dsp.tdim = tdim(0)
dsp.type = type
dsp.sizex = dsp.zoom*dsp.xdim
dsp.sizey = dsp.zoom*dsp.ydim
if(dsp.plane ge dsp.zdim) then dsp.plane = dsp.zdim
if(dsp.plane eq        0) then dsp.plane = 2
widget_control,wd.sld_pln,SET_SLIDER_MIN=1,SET_SLIDER_MAX=dsp.zdim
widget_control,wd.sld_pln,SET_VALUE=dsp.plane
if(dsp.frame ge dsp.tdim) then dsp.frame = dsp.tdim
if(dsp.frame eq        0) then dsp.frame = 1
widget_control,wd.sld_frm,SET_SLIDER_MIN=1,SET_SLIDER_MAX=dsp.tdim
widget_control,wd.sld_frm,SET_VALUE=dsp.frame

widget_control,wd.files,SET_VALUE=fi.list
widget_control,wd.files,SET_LIST_SELECT=fi.n
widget_control,wd.sld_pln,SET_SLIDER_MIN=1,SET_SLIDER_MAX=dsp.zdim
 
return
end
