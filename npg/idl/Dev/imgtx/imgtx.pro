; $Id: imgtx.pro,v 1.6 2000/08/11 19:40:13 tom Exp $
;+
; NAME:
;	IMGTX
;
; PURPOSE:
;	Filter an image with 3-D Gaussian.
;
; CATEGORY:
;	Image processing
;
; CALLING SEQUENCE:
;	IMGTX 
;
; INPUTS: passed from Environment variables
;	Infile:	 input filename
;	Outfile: output filename
;
; OUTPUTS:
;	Outfile: filtered image file
;
; PROCEDURE:
;	Read an image file, apply texture filter
;	and write to output file in Interfile format
;
; MODIFICATION HISTORY:
;   Tom Videen, May 2002
;-

PRO IMGTX
@IMAGE_UTILS.h

   on_error,2
   
   infile = ''
   outfile = ''
   f0 = 0.0
   infile = getenv('INFILE')
   outfile = getenv('OUTFILE')
   
   extra = strarr(3)
   extra[0] = 'conversion program := imgtx'
   extra[1] = 'program version    := 1.0'
   extra[2] = 'program date       := 2002:05:08'
   
   img = {VIMAGE}
   img.fname = infile
   img.ff = 1
   img.lf = 1
   img.fp = 1
   img.lp = 300
   fmode = 0
   msg = ''
   
   print,'Reading '+img.fname+'...'
   IF (GETFRAME(img, msg) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   
   nx = img.dim[0]
   ny = img.dim[1]
   nz = img.dim[2]
   tmp = fltarr(nx,ny,nz)
   ;loc = fltarr(3,3,3)
   out = fltarr(nx,ny,nz)
   tmp[*,*,*] = *img.data
   
   ;FOR k = 1, nz-2 DO BEGIN
   FOR k = 14, 14 DO BEGIN
      FOR  j = 1, ny-2 DO BEGIN
         FOR i = 1, nx-2 DO BEGIN
			loc = tmp[i-1:i+1,j-1:j+1,k-1:k+1]
			t0 = tmp[i,j,k]
			t1 = abs(t0-max(loc))
			t2 = abs(t0-min(loc))
            out[i, j, k] = max(t1,t2)
         ENDFOR
      ENDFOR
   ENDFOR
   
   (*img.data) = out

   print,'Writing '+outfile+'...
   IF (PUTINTERFILE(img, outfile, msg, EXTRA=extra) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 

END 
