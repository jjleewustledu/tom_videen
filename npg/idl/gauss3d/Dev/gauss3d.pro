; $Id$
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
;	GAUSS3D, Infile, F0, Outfile
;
; INPUTS:
;	Infile:		input filename
;	F0:		cutoff frequency of Gaussian (cycles/cm)
;	Outfile:	output filename
;
; OUTPUTS:
;	Outfile:	filter image file
;
; PROCEDURE:
;	Read an image file, apply 3-D gaussian of specified f0
;	to each slice, and write to output file.
;
; MODIFICATION HISTORY:
;   Tom Videen, Dec, 1997
;-

PRO GAUSS3D, infile, f0, outfile
@VIDI.h

   on_error,2
   IF n_params() NE 3 THEN message,'Usage: gauss3d, infile, f0, outfile'
   
   version = strarr(3)
   version[0] = 'gauss3d'
   version[1] = '1.0'
   version[2] = '1997:12:10'
   
   img = {VIMAGE}
   uv = {img:[img,img]}
   uv.img[0].fname = infile
   uv.img[0].ff = 1
   uv.img[0].lf = 1
   uv.img[0].fp = 1
   uv.img[0].lp = 100
   fmode = 0
   msg = ''
   
   print,'Reading '+uv.img[0].fname+'...'
   IF (GETFRAME(uv, 0, msg) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 
   
   IF (uv.img[0].pdim[0] NE uv.img[0].pdim[1]) THEN BEGIN
      message, 'X- and Y-pixel sizes must be identical'
      exit
   ENDIF 
   halfwidth = 1.0/(2.0 * f0 * uv.img[0].pdim[0]) ; in pixels
   print,f0,halfwidth,uv.img[0].pdim[0]
   
   print,'Transverse filtering...'
   FOR z=0, uv.img[0].dim[2]-1 DO BEGIN 
      data = REFORM((*uv.img[0].data)[*,*,z,ff])
      (*uv.img[0].data)[*,*,z,ff] = GAUSS2D_CONVOL(data, halfwidth)
   ENDFOR 
   
   halfwidth = 1.0/(2.0 *f0 * uv.img[0].pdim[2]) ; in slices
   print,f0,halfwidth,uv.img[0].pdim[2]
   
   print,'Axial filtering...'
   SLICE_CONVOL, *uv.img[0].data, halfwidth

   uv.img[0].fname = outfile
   print,'Writing '+uv.img[0].fname+'...'
   IF (PUTINTERFILE(uv, 0, version, msg) NE 0) THEN BEGIN
      message, msg
      exit
   ENDIF 

END 
