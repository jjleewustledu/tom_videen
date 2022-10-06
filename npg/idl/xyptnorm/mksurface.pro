;+
; NAME:
;       MKSURFACE
;
; PURPOSE:
;       This function returns an array with values given
;       input-coordinates and a two-dimensional polynomial fit. It
;       allows to have unequal degrees of polynomial functions in the
;       x- and y-direction. In IRAF speak (http://iraf.net) you get an
;       xorder=xdegree+1 and yorder=ydegree+1 with xterms=full (full
;       cross-terms) surface fit.
;
; CATEGORY:
;       Curve and surface fitting.
;
; CALLING SEQUENCE:
;       result = MKSURFACE(x, y, kx)
;
; INPUTS:
;       x: array with the x-coordinates
;       y: array with the y-coordinates
;       kx: an ydegree+1 by xdegree+1 array with the polynomial
;       coefficients
;
; KEYWORDS:
;       None
;
; OUTPUT:
;       result: the requested surface
;
; PROCEDURE:
;       A surface given by array z as a function of x and y. The
;       function is:
;           F(x,y) = Sum over i and j of kx[j,i] * x^i * y^j
;       where kx is given as input.
;
; MODIFICATION HISTORY:
;       05/01/2008, Eduard Westra    Initial creation
;
;-
FUNCTION mksurface, x, y, kx
  COMPILE_OPT STRICTARRSUBS, IDL2
  ON_ERROR, 2
  IF N_ELEMENTS(x) NE N_ELEMENTS(y) THEN MESSAGE, 'Not equal length input arrays...'
  result = DBLARR(N_ELEMENTS(x))
  sz = SIZE(kx)
  FOR i = 0, sz[2]-1 DO BEGIN
      FOR j = 0, sz[1]-1 DO BEGIN
          result += kx[j,i] * x^i * y^j
      ENDFOR
  ENDFOR
  RETURN, result
END
