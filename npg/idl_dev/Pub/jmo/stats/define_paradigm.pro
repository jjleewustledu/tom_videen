;*************************
pro define_paradigm,fi,stc
;*************************

; Define stimulus time course.

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr
if(hdr.array_type eq !STITCH_ARRAY) then $
    tdim = stc(hd.mother).tdim $
else $
    tdim = hdr.tdim
if(tdim le 2) then begin
    stat=widget_message('*** Paradigm can only be defined for time-course data.  ***',/ERROR)
    return
endif
paradgim = fltarr(tdim+1)
paradigm = fi.paradigm(hd.mother,0:tdim)
value = strarr(5)
label = strarr(5)
value(0) = string('3')
value(1) = string('9')
value(2) = string('18')
value(3) = string('18')
value(4) = string('')
label(0) = string("Number of frames to skip: ")
label(1) = string("Initial rest period: ")
label(2) = string("Activation period: ")
label(3) = string("Rest period: ")
label(4) = string("Encoded paradigm")
vals = get_str(5,label,value)

skip = fix(vals(0))
rest1 = fix(vals(1))
act = fix(vals(2))
rest = fix(vals(3))
mri_paradigm_format = vals(4)

if(strlen(mri_paradigm_format) eq 0) then begin
    if(skip gt 0) then $
        paradigm(0:skip-1) = 0
    if(rest1 gt 0) then $
        paradigm(skip:skip+rest1-1) = -1
    ncycles = tdim/(act+rest) + 1
    j = skip+rest1
    for i=0,ncycles-1 do begin
        if(j+act lt tdim) then begin
            paradigm(j:j+act-1) = 1
            if(j+act+rest le tdim) then $
                paradigm(j+act:j+act+rest-1) = -1 $
            else $
                paradigm(j+act:tdim-1) = -1
        endif else begin
    	    if(j lt tdim) then $
                paradigm(j:tdim) = 1
        endelse
        j = j + act + rest
    endfor
endif else begin
    paradigm = convert_paradigm(mri_paradigm_format,tdim)
    if(paradigm(0) lt 0) then begin
        stat=widget_message('Invalid paradigm code.',/ERROR)
        fi.paradigm_loaded(hd.mother) = !FALSE
    endif
endelse
	
if(hdr.array_type eq !STITCH_ARRAY) then begin
    for i=0,stc(hd.mother).n-1 do begin
        fi.paradigm(hd.mother,i*tdim:(i+1)*tdim-1) = paradigm(0:tdim-1)
    endfor
endif else begin
    fi.paradigm(hd.mother,0:tdim) = paradigm
endelse

fi.paradigm_code(hd.mother) = encode_paradigm(fi.paradigm(hd.mother,0:hdr.tdim),hdr.tdim)
fi.paradigm_loaded(hd.mother) = !TRUE

return
end
