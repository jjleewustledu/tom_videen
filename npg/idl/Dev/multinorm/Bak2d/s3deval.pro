;+
; NAME:
;	S3DEVAL
;
; PURPOSE:
;	Compute values of the interpolant 3D surface at a point (x,y):
;	S(x,y) = sum(i=0,n-1) wi * Fi(x,y) + a + bx + cy + dz
;
; CATEGORY:
;	Surface fitting.
;
; CALLING SEQUENCE:
;	Result = S3DEVAL(Data, Basis, Coefficients, K)
;
; INPUTS:
; 	Pt:		Data point (x,y,z); lonarr(3)
; 	Basis:	Array of n basis points (xi,yi,zi); lonarr(n,3)
; 	W:		Set of n weighting coeffients plus 4 additional coefficients; fltarr(n+4)
; 	K:		Constant; float
;
; OUTPUT:
;	Surface value computed for the point.
;
; PROCEDURE:
; 	Based on Wang, et al, SPIE 2001;4322:1411-1422.
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

FUNCTION s3deval, pt, basis, w, const

   on_error, 2

   n = (size(basis))[1]			; num of basis points 
   ;if n_elements(w) NE n+4 then begin
   if n_elements(w) NE n+3 then begin
       message,"ERROR [s3deval]: number of basis points differs from coefficients"
       return, -1
   endif
   x = pt[0]
   y = pt[1]
   ;z = pt[2]
   a = w[n]
   b = w[n+1]
   c = w[n+2]
   ;d = w[n+3]

   s = 0.0D
   FOR i=0, n-1 DO BEGIN
      ;f = sqrt((x-basis[i,0])^2 + (y-basis[i,1])^2 + (z-basis[i,2])^2 + const^2)
      ;s = s + w[i]*f + a + b*x + c*y + d*z
      f = sqrt((x-basis[i,0])^2 + (y-basis[i,1])^2 + const^2)
      s = s + w[i]*f
   ENDFOR
   s = s + a + b*x + c*y 
   return, s
END

