;***************************
pro mult_comp,fi,wd,dsp,help
;***************************

; Compute mask that is one for valid activations.

; Parameters:
; sigprob: log significance probabilities.
; thresh_max: Maximum region size.
; min_reg_size: Minimum allowable region size.
; connect: !EDGE: Edge and face connectivity.
; 		!FACE: Face connectivity only.

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Corrects for multiple comparisons using an algorithm based on the size of a connected region.'+' Regions consist of voxels connected either through edges and faces or only through faces.'+'  The minimum region size option can be used to reject all activated regions below a specified size.'+'  A minimum threshold can be specified that will be active when it is lower than the programmed threshold.')
endif

label = strarr(2)
label(0) = string('Edge and face')
label(1) = string('Face-only')
title = string('Connect regions with Face-only or Edge-and-Face connectivity?')
conn = get_boolean(title,label)
if(conn eq !TRUE) then $
    connect = !EDGE $
else $
    connect = !FACE

value = strarr(2)
label = strarr(2)
value(0) = string('2')
value(1) = string('1')
label(0) = string("Minimum Region Size: ")
label(1) = string("Minimum threshold (log prob): ")
strs = get_str(2,label,value)
min_reg_size = fix(strs(0))
scl = alog(10)
thresh_print = float(strs(1))
thresh_max = exp(-scl*float(strs(1)))

labels = strarr(fi.n+1)
labels(0:fi.n) = fi.tails(0:fi.n)
idx = get_button(labels,TITLE='Enter log-probability (logp) file.')

handle_value,fi.hdr_handle(idx),hdr
length = hdr.xdim*hdr.ydim*hdr.zdim

logprob = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
for z=0,hdr.zdim-1 do $
    logprob(*,*,z) = get_image(z,fi,stc,FILNUM=idx+1)

widget_control,/HOURGLASS

if(hdr.ydim ne hdr.xdim) then begin
    pad = (hdr.ydim - hdr.xdim)/2
    actmask = fltarr(hdr.ydim,hdr.ydim,hdr.zdim)
    sigprob = fltarr(hdr.ydim,hdr.ydim,hdr.zdim)
    sigprob(pad:pad+hdr.xdim-1,*,*) = exp(-scl*abs(logprob))
endif else begin
    actmask = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
    sigprob = exp(-scl*abs(logprob))
endelse

;if(connect eq 1) then conn = 'EDGE' else conn = 'FACE'
;print,thresh_max,min_reg_size,conn, $
;    FORMAT='("thresh_max; ",f," min_reg_size: ",i2," Connectivity: ",a)'

passtype = bytarr(6)
passtype(2) = 1
passtype(3) = 1
passtype(4) = 0
passtype(5) = 1
num_sig_reg = call_external(!SHARE_LIB,'_mult_comp',sigprob,actmask, $
	min_reg_size,connect,thresh_max,hdr.zdim,VALUE=passtype)

if(hdr.ydim ne hdr.xdim) then begin
    x = actmask(pad:pad+hdr.xdim-1,*,*)
    logprob = logprob*actmask(pad:pad+hdr.xdim-1,*,*)
endif else begin
    logprob = logprob*actmask
endelse

widget_control,HOURGLASS=0

name = strcompress(string(labels(idx),min_reg_size,thresh_print, $
			FORMAT='(a,"_mcomp_n",i2,"_t",f4.1,".")'),/REMOVE_ALL)
put_image,logprob,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hdr.mother,fi.paradigm(hdr.mother,*)
fi.color_scale(fi.n) = !DIFF_SCALE


return
end
