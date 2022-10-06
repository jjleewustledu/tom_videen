;*******************************************************************************
pro get_trend,filnum,tfirst,tlast,wd,fi,stc,hdr,paradigm_in,slope,intcpt,t_rest1
;*******************************************************************************

s  = fltarr(hdr.xdim,hdr.ydim)
sx = fltarr(hdr.xdim,hdr.ydim)
sy = fltarr(hdr.xdim,hdr.ydim)
sxx = fltarr(hdr.xdim,hdr.ydim)
sxy = fltarr(hdr.xdim,hdr.ydim)
den = fltarr(hdr.xdim,hdr.ydim)
slope  = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
intcpt = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)

paradigm = paradigm_in
t_rest1 = tfirst
N_rest1 = 0
tdim = n_elements(paradigm)
while(paradigm(t_rest1) lt 1) do begin
    if(paradigm(t_rest1) lt 0) then $
	N_rest1 = N_rest1 + 1
    t_rest1 = t_rest1 + 1
    if(t_rest1 ge tdim) then goto,END1
endwhile 
END1:
if(t_rest1 eq tdim) then  begin
    t_rest1 = 0
    N_rest1 = 0
endif
for i=0,t_rest1-1 do $
    paradigm(i) = 0

N = 0
for t=tfirst,tlast do begin
    if(paradigm(t) lt 0) then $
        N = N + 1
endfor

s(*,*) = N
for z=0,hdr.zdim-1 do begin
    widget_control,wd.error,SET_VALUE=string('Plane: ',z+1)
    sy(*,*) = 0.
    sxy(*,*) = 0.
    sx1 = 0.
    sxx1 = 0.
    for t=tfirst,tlast do begin
        if(paradigm(t) lt 0) then begin
            image = get_image(z+t*hdr.zdim,fi,stc,FILNUM=filnum+1)
            sy = sy + image
            sxy = sxy + t*image
            sx1 = sx1 + t
            sxx1 = sxx1 + t^2
        endif
    end
    sx(*,*) = sx1
    sxx(*,*) = sxx1
    den           = s*sxx - sx^2
    slope(*,*,z)  = (s*sxy -sx*sy)/den
    intcpt(*,*,z) = (sxx*sy - sx*sxy)/den
endfor

return
end
