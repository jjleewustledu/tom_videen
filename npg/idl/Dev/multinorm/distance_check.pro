;+
; NAME:
;	DISTANCE_CHECK
;
; PURPOSE:
;	Check distance between points.
;
; INPUT:
;	Points:	Array of x,y,z coordinates
;	N1, N2:	Indices of first and last points to compare
;	N0:		Index of reference point
;	Dist0:	Comparison distance
;
; OUTPUT:
;	Returns 1 if point, n, is not < dist0 from another point, n1-n2.
;	Returns 0 if point, n, is < dist0 from another point, n1-n2.
;
; MODIFICATION HISTORY:
;	Sep 2002 - Tom Videen
;-

FUNCTION DISTANCE_CHECK, points, n1, n2, n0, dist0

   IF (N2 LT N1) THEN RETURN, 1

   x0 = points[n0,0]
   y0 = points[n0,1]
   refdist = dist0^2

   FOR n = n1, n2 DO BEGIN
      x1 = points[n,0]
      y1 = points[n,1]
      chkdist = (x1-x0)^2 + (y1-y0)^2
      IF (chkdist LT refdist) THEN RETURN, 0
   ENDFOR
   RETURN, 1
END
