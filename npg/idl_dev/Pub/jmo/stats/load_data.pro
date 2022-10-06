;************************************************
function load_data,fi,st,dsp,wd,stc,img_type,help
;************************************************

; Loads image into image structure.

if(fi.nfiles ge !NUM_FILES) then begin
    stat=widget_message('Too many images loaded.',/ERROR)
    st.error = !TRUE
    return,!ERROR
endif
scl = 1.

case img_type of

;!MRI_ANALYZE: begin
;    if(help.enable eq !TRUE) then begin
;        widget_control,help.id,SET_VALUE=string('Loads MRI data in Analyze format. Data set is assumed to consist of a single plane with many frames.')
;    endif
;    file = pickfile(/read,FILTER='*.img',PATH=fi.path(0), $
;                                GET_PATH=fi.path,/NOCONFIRM)
;    if(file eq '') then return,!ERROR
;    if(n_elements(file) gt 1) then $
;	filnam = file(0)+file(1) $
;    else $
;	filnam = file
;   Build header file name and read Analyze format header.
;    hdrfil=filnam
;    dot = rstrpos(hdrfil,'.')
;    strput,hdrfil,'hdr',dot+1
;    get_lun,lu
;    openr,lu,hdrfil(0)
;    x = intarr(174)
;    readu,lu,x
;    xdim = x(21)
;    ydim = x(22)
;    zdim = x(23)
;    tdim = x(24)
;    bits_per_pix = x(36)
;    if(zdim gt tdim) then begin
;        i = zdim
;        zdim = tdim
;        tdim = i
;    endif
;    close,lu
;    openr,lu,hdrfil(0)
;    y = fltarr(87)
;    readu,lu,y
;    close,lu
;    dxdy = y(20)
;    dz = y(22)
;    type = !ASSOC_ARRAY
;    
;    if(bits_per_pix eq  32) then $
;	num_type = !FLOAT $
;    else $
;        num_type = !SHORT
;
;;   Associate the data
;    get_lun,lun
;    fi.n = fi.nfiles
;    fi.lun(fi.n) = lun
;    openr,lun,filnam(0)
;    if(num_type eq !FLOAT) then begin
;        offset = 0
;        img = assoc(lun,fltarr(xdim,ydim,/NOZERO),offset)
;    endif else begin
;        offset = 0
;        img = assoc(lun,intarr(xdim,ydim,/NOZERO),offset)
;    endelse
;end

!T88: begin
    if(help.enable eq !TRUE) then begin
        widget_control,help.id,SET_VALUE=string('Loads a t88 stack of images.')
    endif
    filnam = pickfile(/read,FILTER='*.t88',PATH=fi.path(0), $
                                GET_PATH=fi.path,/NOCONFIRM)
    if(n_elements(img) eq 1) then begin
 	stat=widget_message('Invalid t88 file.',/ERROR)
        return,!ERROR
    endif
    paradigm = fltarr(2)
    widget_control,/HOURGLASS
    img = read_t88(filnam,xdim,ydim,zdim,tdim,dxdy,dz)
    widget_control,HOURGLASS=0
    if(n_elements(img) eq 1) then begin
 	stat=widget_message('Error reading t88 image.',/ERROR)
        return,!ERROR
    endif
    type = !FLOAT_ARRAY
    num_type = !FLOAT
    scl = 1.  
end

!MRI_4DFP: begin
    if(help.enable eq !TRUE) then begin
        widget_control,help.id,SET_VALUE=string('Loads a single 4DFP stack using associated variables.  The file should be on a local disk or all operations will be slowed.')
    endif
    file = pickfile(/read,FILTER='*.img',PATH=fi.path(0), $
                                GET_PATH=fi.path,/NOCONFIRM)
    if(file eq '') then return,!ERROR
    if(n_elements(file) gt 1) then $
	filnam = file(0)+file(1) $
    else $
	filnam = file
;   Build header file name and read Analyze format header.
    read_header,filnam,xdim,ydim,zdim,tdim,dxdy,dz,paradigm
    if(paradigm(0) lt 0) then $
        widget_control,wd.error,SET_VALUE=string('Header contains invalid paradigm code.')
    num_type = !FLOAT
    type = !ASSOC_ARRAY
    get_lun,lun
    fi.n = fi.nfiles
    fi.lun(fi.n) = lun
    openr,lun,filnam(0)
    if(num_type eq !FLOAT) then begin
        offset = 0
        img = assoc(lun,fltarr(xdim,ydim,/NOZERO),offset)
    endif else begin
        offset = 0
        img = assoc(lun,intarr(xdim,ydim,/NOZERO),offset)
    endelse
end

!STITCH: begin
    stat =load_stitch(fi,st,dsp,wd,stc,img_type,help,filnam,xdim,ydim,zdim,tdim,num_type,dxdy,dz, $
        scl,type,paradigm)
    if(stat eq !ERROR) then $
	return,!ERROR
end

!RAW: begin
    stat = load_raw(fi,st,dsp,wd,stc,img_type,help,filnam,xdim,ydim,zdim, $
	tdim,num_type,dxdy,dz,scl,type,paradigm,img)
    if(stat eq !ERROR) then $
	return,!ERROR
end

else: begin
 	stat=widget_message('Unknown file type.',/ERROR)
        st.error_msg = string('Unknown file type.')
        st.error = !TRUE
	return,!ERROR
end

endcase

mother = 'none'
put_image,img,fi,wd,dsp,filnam,xdim,ydim,zdim,tdim,num_type,dxdy,dz, $
	scl,type,fi.n,paradigm
fi.color_scale(fi.n) = !GRAY_SCALE
dsp.color_table_index = !GRAY_SCALE
loadct,!GRAY_SCALE,NCOLORS=!LEN_COLORTAB,FILE=!COLOR_TABLE,/SILENT

return,!OK
end
