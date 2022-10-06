; $Id: gauss2d_convol.pro,v 1.2 1997/04/08 21:13:39 tom Exp $
;+
; NAME:
;	GAUSS2D_CONVOL
;
; PURPOSE:
;	2-D convolution with Gaussian kernel
;
; CATEGORY:
;   Image processing
;
; CALLING SEQUENCE:
;	Result = GAUSS2D_CONVOL (Matrix, Halfwidth)
;
; INPUTS:
;	Matrix = 2-D matrix (float)
;	Halfwidth = half-width of gaussian (in pixels)
;
; RESTRICTIONS:
;	Matrix should be of type FLOAT
;
; PROCEDURE:
;	CONVOL is used to convolve mat with a gaussian with specified halfwidth in pixels.
;
; MODIFICATION HISTORY:
;	Tom Videen, Dec, 1997.
;-

FUNCTION GAUSS2D_CONVOL, mat, halfwidth

   on_error,2
   IF n_params() NE 2 THEN message,'Usage: result = gauss2d_convol(matrix, halfwidth)'
   sz = size(mat)
   mat0 = mat
   prec = 4                     ; larger values for greater precision
   sig = halfwidth/2.354
   n_pts = round(prec*sig)*2 + 1 ; number of points in kernel (odd)
   x = (findgen(n_pts) - n_pts/2) ; x locations to compute kernel
   gaus = exp(-0.5*(x/sig)^2)   ; gaussian of width sig

   FOR i=0,sz(1)-1 DO BEGIN     ; convolution on columns of mat
      y = rotate(mat(i,*),4)
      z = convol(y,gaus,/edge_truncate)/total(gaus)
      mat0(i,*) = rotate(z,4)
   ENDFOR 
   FOR i=0,sz(2)-1 DO BEGIN     ; convolution on rows of mat0
      y = mat0(*,i)
      mat(*,i) = convol(y,gaus,/edge_truncate)/total(gaus)
   ENDFOR 

   RETURN,mat
END 
 
