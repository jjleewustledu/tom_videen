;**************************************************************************
function get_image,index,fi,stc,FILNUM=filnum,SUBTRACT_TREND=subtract_trend
;**************************************************************************

common linear_fit,fit_name,slope,intcpt,trend

if keyword_set(filnum) then begin
    filnum = filnum - 1
endif else begin
    filnum = fi.n
endelse

if keyword_set(subtract_trend) then begin
    subtrnd = !TRUE
endif else begin
    subtrnd = !FALSE
endelse
handle_value,fi.hdr_handle(filnum),hdr

if(hdr.array_type eq !STITCH_ARRAY) then begin
    t_total = index/hdr.zdim 		;Overall time index.
    i_file = t_total/stc(filnum).tdim	;Index to stc array.
    t = t_total - i_file*stc(filnum).tdim
    z = index - t_total*hdr.zdim
    i1 = index - i_file*stc(filnum).tdim*hdr.zdim
    handle_value,stc(filnum).hdr_handle(i_file),hdr
    get_lun,lun
    openr,lun,stc(filnum).filnam(i_file)
    img = assoc(lun,fltarr(hdr.xdim,hdr.ydim,/NOZERO),0)
    image = hdr.scl*img(*,*,i1)
    close,lun
    free_lun,lun
    if((subtrnd eq !TRUE) and (trend.slope_handle(filnum) ne 0)) then begin
	handle_value,trend.slope_handle(filnum),slope
	handle_value,trend.intcpt_handle(filnum),intcpt
  	t_rest1 = trend.t_rest1(filnum,i_file)
	if(t lt t_rest1) then $
            image = image - (t_rest1*slope(*,*,z,i_file) + intcpt(*,*,z,i_file)) $
	else $
            image = image - (t_total*slope(*,*,z,i_file) + intcpt(*,*,z,i_file))
    endif
endif else begin
    handle_value,fi.data_handle(filnum),img
    if((subtrnd eq !TRUE) and (trend.slope_handle(filnum) ne 0)) then begin
        t = index/hdr.zdim
        z = index - t*hdr.zdim
	handle_value,trend.slope_handle(filnum),slope
	handle_value,trend.intcpt_handle(filnum),intcpt
  	t_rest1 = trend.t_rest1(filnum)
	if(t lt t_rest1) then $
            image = img(*,*,index) - (t_rest1*slope(*,*,z) + intcpt(*,*,z)) $
	else $
            image = img(*,*,index) - (t*slope(*,*,z) + intcpt(*,*,z))
    endif else begin
        image = img(*,*,index)
    endelse
endelse

return,image
end
