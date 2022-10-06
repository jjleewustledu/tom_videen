;+
; NAME:
;	WARP2D
;
; PURPOSE
;	2-D warp of array STUDY based upon gradient GX,GY of difference with REF.
;	The dimensions of GX,GY define the possibly downsampled arrays upon which
;	the warp is defined.
;
; INPUTS:
;   REF:	Reference or standard (2-D data array)
;   STUDY:	Study or reslice (2-D data array)
;   NEW:	Current Deformed Study (2-D data array)
;   GX,GY:	Gradient (2-D floating-point arrays)
;	NDX,NDY: Total shift (all iterations) (2-D floating-point arrays)
;	F0:		Filter cutoff (smaller value -> greater smoothing; 0.1)
;
; OUTPUT:
;	NDX,NDY: Total shift (all iterations) (2-D floating-point arrays)
;   NEW:	Newly Deformed Study (2-D data array)
;
; REVISION HISTORY:
;	Jun 2002 Tom Videen
;-

PRO WARP2D, REF, STUDY, NEW, GX, GY, NDX, NDY, F0
   COMMON GRIDLOCK

   on_error, 2
   nx = (size(ref))[1]          ; original image size
   ny = (size(ref))[2]
   mx = (size(gx))[1]           ; resampled image size (for gradient definition and warp)
   my = (size(gx))[2]

   dx = fltarr(mx,my)
   dy = fltarr(mx,my)
   fac = 1.0

   dif = downsample(new-ref,mx,my)
   ix = where (gx NE 0,cntx)
   iy = where (gy NE 0,cnty)

   dx[ix] = fac*dif[ix]*gx[ix]/(gx[ix]*gx[ix] + dif[ix]*dif[ix])
   dy[iy] = fac*dif[iy]*gy[iy]/(gy[iy]*gy[iy] + dif[iy]*dif[iy])

   fdx = gauss(dx,f0)
   fdy = gauss(dy,f0)
   ndx = ndx + upsample(fdx,nx,ny)
   ndy = ndy + upsample(fdy,nx,ny)
   fdx = gauss(ndx,0.2)
   fdy = gauss(ndy,0.2)

   FOR y=0,ny-1 DO BEGIN
      FOR x=0,nx-1 DO BEGIN
         xx = x-fdx[x,y]
         yy = y-fdy[x,y]
         new[x,y] = interpolate(study,xx,yy,/cubic)
      ENDFOR 
   ENDFOR 
END
