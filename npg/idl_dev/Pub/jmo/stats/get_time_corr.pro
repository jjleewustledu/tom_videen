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

m = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
m1 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
m2 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
m3 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
m4 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
m5 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
s = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
sd = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
diff = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr1 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr2 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr3 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr4 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
corr5 = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
x1 = fltarr(hdr.xdim,hdr.ydim)
x21 = fltarr(hdr.xdim,hdr.ydim)
x22 = fltarr(hdr.xdim,hdr.ydim)
x31 = fltarr(hdr.xdim,hdr.ydim)
x32 = fltarr(hdr.xdim,hdr.ydim)
x33 = fltarr(hdr.xdim,hdr.ydim)
x41 = fltarr(hdr.xdim,hdr.ydim)
x42 = fltarr(hdr.xdim,hdr.ydim)
x43 = fltarr(hdr.xdim,hdr.ydim)
x44 = fltarr(hdr.xdim,hdr.ydim)
x51 = fltarr(hdr.xdim,hdr.ydim)
x52 = fltarr(hdr.xdim,hdr.ydim)
x53 = fltarr(hdr.xdim,hdr.ydim)
x54 = fltarr(hdr.xdim,hdr.ydim)
x55 = fltarr(hdr.xdim,hdr.ydim)
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
     N1 = 0
     N2 = 0
     N3 = 0
     N4 = 0
     N5 = 0
     for t1=0,hdr.tdim-1 do begin
        if(paradigm(t1) lt 0) then begin
            jimg = get_image(t1*hdr.zdim+z,fi,stc,FILNUM=hd.mother+1,/SUBTRACT_TREND)
	    img = gauss_smoth(jimg,fwhm)
	    m(*,*,z) = m(*,*,z) + img
	    s(*,*,z) = s(*,*,z) + img^2
	    if((jt mod 2) eq 0) then begin
		x1 = img
	    endif else begin
		corr1(*,*,z) = corr1(*,*,z) + x1*img
	        m1(*,*,z) = m1(*,*,z) + (img + x1)/2.
		N1 = N1 + 1
	    endelse
	    case (jt mod 4) of
                0: $
		    x21 = img
                1: $
		    x22 = img
		2: begin
		    corr2(*,*,z) = corr2(*,*,z) + x21*img
	            m2(*,*,z) = m2(*,*,z) + (img + x21)/2.
		    N2 = N2 + 1
		end
		3: begin
		    corr2(*,*,z) = corr2(*,*,z) + x22*img
	            m2(*,*,z) = m2(*,*,z) + (img + x22)/2.
		    N2 = N2 + 1
		end
		else: print,'Invalid value of jt'
	    endcase
	    case (jt mod 6) of
                0: $
		    x31 = img
                1: $
		    x32 = img
                2: $
		    x33 = img
		3: begin
		    corr3(*,*,z) = corr3(*,*,z) + x31*img
	            m3(*,*,z) = m3(*,*,z) + (img + x31)/2.
		    N3 = N3 + 1
		end
		4: begin
		    corr3(*,*,z) = corr3(*,*,z) + x32*img
	            m3(*,*,z) = m3(*,*,z) + (img + x32)/2.
		    N3 = N3 + 1
		end
		5: begin
		    corr3(*,*,z) = corr3(*,*,z) + x33*img
	            m3(*,*,z) = m3(*,*,z) + (img + x33)/2.
		    N3 = N3 + 1
		end
		else: print,'Invalid value of jt'
	    endcase
	    case (jt mod 8) of
                0: $
		    x41 = img
                1: $
		    x42 = img
                2: $
		    x43 = img
                3: $
		    x44 = img
		4: begin
		    corr4(*,*,z) = corr4(*,*,z) + x41*img
	            m4(*,*,z) = m4(*,*,z) + (img + x41)/2.
		    N4 = N4 + 1
		end
		5: begin
		    corr4(*,*,z) = corr4(*,*,z) + x42*img
	            m4(*,*,z) = m4(*,*,z) + (img + x42)/2.
		    N4 = N4 + 1
		end
		6: begin
		    corr4(*,*,z) = corr4(*,*,z) + x43*img
	            m4(*,*,z) = m4(*,*,z) + (img + x43)/2.
		    N4 = N4 + 1
		end
		7: begin
		    corr4(*,*,z) = corr4(*,*,z) + x44*img
	            m4(*,*,z) = m4(*,*,z) + (img + x44)/2.
		    N4 = N4 + 1
		end
		else: print,'Invalid value of jt'
	    endcase
	    case (jt mod 10) of
                0: $
		    x51 = img
                1: $
		    x52 = img
                2: $
		    x53 = img
                3: $
		    x54 = img
                4: $
		    x55 = img
		5: begin
		    corr5(*,*,z) = corr5(*,*,z) + x51*img
	            m5(*,*,z) = m5(*,*,z) + (img + x51)/2.
		    N5 = N5 + 1
		end
		6: begin
		    corr5(*,*,z) = corr5(*,*,z) + x52*img
	            m5(*,*,z) = m5(*,*,z) + (img + x52)/2.
		    N5 = N5 + 1
		end
		7: begin
		    corr5(*,*,z) = corr5(*,*,z) + x53*img
	            m5(*,*,z) = m5(*,*,z) + (img + x53)/2.
		    N5 = N5 + 1
		end
		8: begin
		    corr5(*,*,z) = corr5(*,*,z) + x54*img
	            m5(*,*,z) = m5(*,*,z) + (img + x54)/2.
		    N5 = N5 + 1
		end
		9: begin
		    corr5(*,*,z) = corr5(*,*,z) + x55*img
	            m5(*,*,z) = m5(*,*,z) + (img + x55)/2.
		    N5 = N5 + 1
		end
		else: print,'Invalid value of jt'
	    endcase
    	    N = N + 1
	    jt = jt + 1
        endif
    endfor
endfor

s =  (N*s - m^2)/(N*(N-1))
corr1 =  (N1*corr1 - m1^2)/(N1*(N1-1))
corr2 =  (N2*corr2 - m2^2)/(N2*(N2-1))
corr3 =  (N3*corr3 - m3^2)/(N3*(N3-1))
corr4 =  (N4*corr4 - m4^2)/(N4*(N4-1))
corr5 =  (N5*corr5 - m5^2)/(N5*(N5-1))
m = m/N

mask = s eq 0.
corr1 = (1-mask)*corr1/(s + mask)
corr2 = (1-mask)*corr2/(s + mask)
corr3 = (1-mask)*corr3/(s + mask)
corr4 = (1-mask)*corr4/(s + mask)
corr5 = (1-mask)*corr5/(s + mask)

corr = fltarr(hdr.xdim,hdr.ydim,5*hdr.zdim)
corr(*,*,0:hdr.zdim-1) = corr1
corr(*,*,hdr.zdim:2*hdr.zdim-1) = corr2
corr(*,*,2*hdr.zdim:3*hdr.zdim-1) = corr3
corr(*,*,3*hdr.zdim:4*hdr.zdim-1) = corr4
corr(*,*,4*hdr.zdim:5*hdr.zdim-1) = corr5

name = strcompress(string(fi.tails(hd.mother),FORMAT='(a,"_corr")'),/REMOVE_ALL)
paradigm(0) = -5
put_image,corr,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,5,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,-1,paradigm

widget_control,HOURGLASS=0

return
end
