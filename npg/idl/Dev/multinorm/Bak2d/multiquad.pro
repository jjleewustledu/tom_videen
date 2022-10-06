;+
; NAME:
;	MULTIQUAD
;
; PURPOSE:
;	Computes values of a single basis function, Fi(x,y,z) for given data(x,y,z),
;   basis point (xi, yi, zi) and constant k.
;
; CATEGORY:
;	Surface fitting.
;
; CALLING SEQUENCE:
;	Result = MULTIQUAD(Data, Basis, K)
;
; INPUTS:
; 	Data:	Array of all data points (x,y,z)
; 	Basis:	Single basis point (xi,yi,zi)
; 	K:		Constant
;
; OUTPUT:
;	Fi(x,y,z) = sqrt((x-xi)^2 + (y-yi)^2 + (z-zi)^2 + k^2)
;
; PROCEDURE:
; 	Based on Wang, et al, SPIE 2001;4322:1411-1422.
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

FUNCTION multiquad, data, basis, k

   on_error, 2

   n = (size(data))[2]		; num of points to fit
   x = data[0,*]
   y = data[1,*]
   z = data[2,*]
   xi = point[0]
   yi = point[1]
   zi = point[2]
   f = sqrt((x-xi)^2 + (y-yi)^2 + (z-zi)^2 + k^2)
   return, f
END

