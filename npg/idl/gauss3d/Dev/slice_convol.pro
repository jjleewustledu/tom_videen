; $Id: slice_convol.pro,v 1.3 1997/04/08 21:13:59 tom Exp $
;+
; NAME:
;	SLICE_CONVOL
;
; PURPOSE:
;	Convolution across slices using a 1-D Gaussian kernel.
;
; CATEGORY:
;   Image processing
;
; CALLING SEQUENCE:
;	SLICE_CONVOL, Matrix, Halfwidth
;
; INPUTS:
;	Matrix = 2-D matrix (float)
;	Halfwidth = half-width of gaussian (in pixels)
;
; OUTPUTS:
;	Matrix = 2-D matrix (float)
;
; RESTRICTIONS:
;	Matrix should be of type FLOAT
;	
; FUNCTION:
;	CONVOL is used to convolve mat with a gaussian with specified halfwidth in pixels.
;
; MODIFICATION HISTORY:
;	Tom Videen, Dec, 1997
;-

PRO SLICE_CONVOL, mat, halfwidth

   on_error,2
   IF n_params() NE 2 THEN message,'Usage: slice_convol, matrix, halfwidth'
   sz = size(mat)
   z = fltarr(sz(3))
   prec = 3                     ; larger values for greater precision
   sigma = halfwidth/2.354
   n_pts = round(prec*sigma)*2 + 1 ; number of points in kernel (odd)
   IF n_pts GE sz(3) THEN $
      n_pts = ((sz(3)-1)/2)*2 + 1 ; kernel size < array size
   x = (findgen(n_pts) - n_pts/2) ; x locations to compute kernel
   gaus = exp(-0.5*(x/sigma)^2) ; gaussian of width sigma
   
   ;; Extract a slice vector, convolve, then replace the slice vector

   FOR i = 0, sz(1)-1 DO BEGIN 
      FOR j = 0, sz(2)-1 DO  BEGIN 
         FOR k= 0, sz(3)-1 DO z(k) = mat(i,j,k)
         w = convol(z,gaus,/edge_truncate)/total(gaus)
         FOR k= 0, sz(3)-1 DO mat(i,j,k) = w(k)
      ENDFOR 
   ENDFOR 
END 
