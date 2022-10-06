;***************************************
pro get_time_corr,fi,pr,dsp,wd,stc,help
;***************************************

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

nx = fltarr(10)
m = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
s = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr = fltarr(hdr.xdim,hdr.ydim,hdr.zdim,10)
lagimg = fltarr(hdr.xdim,hdr.ydim,10)
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

widget_control,/HOURGLASS

; First compute variance image over rest periods.
for z=0,hdr.zdim-1 do begin
     widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
     N = 0
     jt = 0
     il = 0
for i=0,9 do nx(i) = 0
     for t1=0,hdr.tdim-1 do begin
        if(paradigm(t1) lt 0) then begin
            jimg = get_image(t1*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1,/SUBTRACT_TREND)
;jimg = reform(normaln_jmo(4096,seed),64,64)
	    img = gauss_smoth(jimg,fwhm)
	    m(*,*,z) = m(*,*,z) + img
	    s(*,*,z) = s(*,*,z) + img^2
	    lagmax = jt < 9
	    for lag=0,lagmax-1 do begin
	        jl = il - lag - 1
		if(jl lt 0) then jl = jl + 10
		corr(*,*,z,lag) = corr(*,*,z,lag) + img*lagimg(*,*,jl)
		nx(lag) = nx(lag) + 1
	    endfor
	    lagimg(*,*,il) = img
	    il = (il + 1) mod 10
    	    N = N + 1
	    jt = jt + 1
        endif
    endfor
endfor

msq = m^2/N
s =  s/(N-1) - msq/(N-1)
mask = s eq 0.

for il=0,9 do begin
    N1 = N - il - 1
    corr(*,*,*,il) =  corr(*,*,*,il)/(N1-1) - N1*msq/N/(N1-1)
    corr(*,*,*,il) = (1-mask)*corr(*,*,*,il)/(s + mask)
endfor
corr(*,*,*,9) = 0
corr = reform(corr,hdr.xdim,hdr.ydim,10*hdr.zdim)

name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_corr")'),/REMOVE_ALL)
paradigm(0) = -5
put_image,corr,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,10,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,-1,paradigm

;name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_var")'),/REMOVE_ALL)
;put_image,s,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,1,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,-1,paradigm

widget_control,HOURGLASS=0

return
end
