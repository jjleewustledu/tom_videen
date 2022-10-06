; $Id: gauss3d.pro,v 1.7 2009/01/21 21:07:23 tom Exp tom $
;+
; NAME:
;	GAUSS3D
;
; PURPOSE:
;	Filter an image with 3-D Gaussian.
;
; CATEGORY:
;	Image processing
;
; CALLING SEQUENCE:
;	GAUSS3D
;
; INPUTS: passed from Environment variables
;	Infile:	 input filename
;	Outfile: output filename
;	F0:	 cutoff frequency of Gaussian (cycles/cm)
;
; OUTPUTS:
;	Outfile: filtered image file
;
; PROCEDURE:
;	Read an image file, apply 3-D gaussian of specified f0
;	and write to output file in Interfile format
;
; MODIFICATION HISTORY:
;   Tom Videen, Dec, 1998
;   Tom Videen, Jul, 2000 - read and write Interfile type float 
;-

PRO GAUSS3D
@IMAGE_UTILS.h

   on_error,2
   
   infile = ''
   outfile = ''
   f0 = 0.0
   infile = getenv('INFILE')
   outfile = getenv('OUTFILE')
   f0 = getenv('HALF_FREQUENCY')
   f0 = f0*0.1	; convert to cycles/mm
   
   extra = strarr(3)
   extra[0] = 'conversion program := gauss3d'
   extra[1] = 'program version    := 1.5'
   extra[2] = 'program date       := 2000:07:07'
   
   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 600
   fmode = 0
   msg = ''
   
   print,'Reading '+img.fname+'...'
   IF (GETFRAME(img, msg) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   
   nx = img.dim[0]
   ny = img.dim[1]
   nz0 = img.dim[2]
   IF (nz0 LE 49) THEN nz = 64 $
   ELSE IF (nz0 LE 100) THEN nz = 128 $
   ELSE IF (nz0 LE 256) THEN nz = 256 $
   ELSE IF (nz0 LE 512) THEN nz = 512 $
   ELSE BEGIN
      print,'This is one heck of a big image and you might want to resize it before filtering...'
      exit
   ENDELSE 
   dx = 1.0/(nx * img.pdim[0])
   dy = 1.0/(ny * img.pdim[1])
   dz = 1.0/(nz * img.pdim[2])
   
   z0 = nz-nz0
   z1 = z0+nz0-1
   tmp = fltarr(nx,ny,nz)
   tmp[*,*,z0:z1] = *img.data
   
   tmp = FFT(tmp,/OVERWRITE)
   
   print,'Filtering...' ; (this loop is the slowest part of the algorithm)
   fc = -ALOG(2.)/f0^2
   FOR k = 0, nz-1 DO BEGIN
      IF (k LE nz/2) THEN fz = dz*k ELSE fz = dz*(nz-k)
      FOR  j = 0, ny-1 DO BEGIN
         IF (j LE ny/2) THEN fy = dy*j ELSE fy = dy*(ny-j)
         FOR i = 0, nx-1 DO BEGIN
            IF (i LE nx/2) THEN fx = dx*i ELSE fx = dx*(nx-i)
            f = fx^2 + fy^2 + fz^2
            fac = EXP(f*fc)  ; Floating underflow can occur here
            tmp[i, j, k] = fac * tmp[i, j, k]
         ENDFOR
      ENDFOR
   ENDFOR
   
   tmp = FFT(tmp, /INVERSE)
   (*img.data)[*,*,*,ff-img[0].ff] = float(EXTRAC(tmp,0,0,z0,nx,ny,nz0))

   print,'Writing '+outfile+'... (Floating underflow warning is harmless)'
   IF (PUTINTERFILE(img, outfile, msg, EXTRA=extra) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 

END 
