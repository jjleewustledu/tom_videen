;**************************************************
pro get_smoothness,fi,pr,dsp,wd,stc,help,x_pos,y_pos,z_pos
;**************************************************

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Estimate smoothness in equivalent width of a Gaussian filter using method of Forman et al., MRM 33:636-647, (1995).')
endif

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr

if(fi.paradigm_loaded(hd.mother) ne !TRUE) then begin
    stat=widget_message('*** No stimulus time-course defined.  ***',/ERROR)
    return
endif

m = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
s = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
sd = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
diff = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
paradigm = fltarr(hdr.tdim)
paradigm = fi.paradigm(hd.mother,*)
N = 0
Nt = 0
for t1=0,hdr.tdim-1 do begin
     if(paradigm(t1) lt 0) then Nt = Nt + 1
endfor
Nt = 2*(Nt/2)

seed = intarr(3)
value = strarr(1)
label = strarr(1)
value(0) = string('0')
label(0) = string("FWHM of Gaussian filter (pixels): ")
dims = get_str(1,label,value)
fwhm = float(dims(0))

; Compute linear trend if necessary.
if(strpos(fi.tails(hd.mother),fit_name) lt 0) then $
    linfit,hd.mother,fi,pr,dsp,wd,stc,help

;print,'patch in get_smoothness'
;slope(*,*,*) = 0.

widget_control,/HOURGLASS

; First compute variance image over rest periods.
for z=0,hdr.zdim-1 do begin
     widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
     N = 0
     jt = 1
     for t1=0,hdr.tdim-1 do begin
        if(paradigm(t1) lt 0) then begin
            jimg = get_image(t1*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1,/SUBTRACT_TREND)
;jimg = reform(normaln_jmo(4096,seed),64,64)
	    img = gauss_smoth(jimg,fwhm)
	    m(*,*,z) = m(*,*,z) + img
	    s(*,*,z) = s(*,*,z) + img^2
	    if(jt le Nt/2) then $
		diff(*,*,z) = diff(*,*,z) + img
	    if((jt gt Nt/2) and (jt le Nt)) then $
		diff(*,*,z) = diff(*,*,z) - img
    	    N = N + 1
	    jt = jt + 1
        endif
    endfor
endfor

sd = ((diff - shift(diff,0, 1,0))^2 + (diff - shift(diff, 1,0,0))^2 + $
      (diff - shift(diff,0,-1,0))^2 + (diff - shift(diff,-1,0,0))^2 + $
      (diff - shift(diff,1, 1,0))^2 + (diff - shift(diff, 1,-1,0))^2 + $
      (diff - shift(diff,-1,1,0))^2 + (diff - shift(diff,-1,-1,0))^2)/8
;      (diff - shift(diff,0,-1,0))^2 + (diff - shift(diff,-1,0,0))^2 )/3

;sd = ((diff * shift(diff,0, 1,0)) + (diff * shift(diff, 1,0,0)) + $
;      (diff * shift(diff,0,-1,0)) + (diff * shift(diff,-1,0,0)) + $
;      (diff * shift(diff,1, 1,0)) + (diff * shift(diff, 1,-1,0)) + $
;      (diff * shift(diff,-1,1,0)) + (diff * shift(diff,-1,-1,0)))/8
;      (diff * shift(diff,0,-1,0)) + (diff * shift(diff,-1,0,0)) )/3

print,total(m)
s =  (N*s - m*m)/(N*(N-1))
;s = s/(N-1)
;s1 = total(s)/hdr.xdim/hdr.ydim/hdr.zdim

mask = s eq 0
rho = (1 - mask)*sd/(Nt*s + mask)
;rho = 2*(1 - mask)*(1 - sd/(Nt*s + mask))
rho = (1 - mask)*sd/(Nt*s + (1-mask))
;rho = (1 - mask)*sd/(Nt*s1 + mask) ; Pooled variance
mask = rho lt 5
rho = rho*mask + 5*(1 - mask)
rho(0:fwhm,*,*) = 0
rho(*,0:fwhm,*) = 0
rho(hdr.xdim-fwhm-1:hdr.xdim-1,*,*) = 0
rho(*,hdr.ydim-fwhm-1:hdr.ydim-1,*) = 0

rho_bar = total(rho(fwhm+1:hdr.xdim-fwhm-2,fwhm+1:hdr.ydim-fwhm-2,*))/(hdr.xdim-4)/(hdr.ydim-4)/hdr.zdim
print,'Smoothness for fwhm of ',fwhm,' pixels is ',rho_bar

name = strcompress(string(fi.tails(hd.mother),fwhm,FORMAT='(a,"_rho_fwhm",f5.2)'),/REMOVE_ALL)
put_image,rho,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_diff")'),/REMOVE_ALL)
;put_image,diff,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

;mask = s gt 0
;s = 1./(mask*s + (1-mask)*max(s))
;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_1/var")'),/REMOVE_ALL)
;put_image,s,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_mask")'),/REMOVE_ALL)
;put_image,mask,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_sdelta")'),/REMOVE_ALL)
;put_image,sd,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,hd.mother,paradigm

widget_control,HOURGLASS=0

return
end
