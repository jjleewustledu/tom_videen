;+
; NAME:
;       SFIT_XY
;
; PURPOSE:
;       This function determines a two-dimensional polynomial fit to a
;       surface sampled over any kind of grid. It allows to have
;       unequal degrees of polynomial functions in the x- and
;       y-direction, unlike the intrinsic SFIT function that can only
;       do fitting in equal degrees in the x- and y-direction. In IRAF
;       speak (http://iraf.net) you get an xorder=xdegree+1 and
;       yorder=ydegree+1 with xterms=full (full cross-terms) surface
;       fit.
;
; CATEGORY:
;       Curve and surface fitting.
;
; CALLING SEQUENCE:
;       result = SFIT_XY(x, y, z, xdegree, ydegree[, sigma=sigma|, weight=weight])
;
; INPUTS:
;       x: array with the x-coordinates
;       y: array with the y-coordinates
;       z: array with the z-coordinates
;       xdegree: maximum degree for the power of the x-term of the
;       polynomial
;       ydegree: maximum degree for the power of the y-term of the
;       polynomial
;
; KEYWORDS:
;       sigma: an array with the sqrt of the variance for each z[i] at
;       (x[i], y[i]) to give a weight to each point. The final weight
;       is 1/sigma[i]^2.
;       weight: an array with the weight for each z[i] at (x[i], y[i]).
;       kx : is the resulting array with coefficients. See output.
;
; OUTPUT:
;       result: an ydegree+1 by xdegree+1 array with all the
;       coefficients for the fitted polynomial function of x and
;       y. Using mksurface (fit = mksurface(x, y, result)) the fitted
;       surface can be constructed.
;
; PROCEDURE:
;       Fit a surface given by array z as a function of x and y. The
;       function fitted is:
;           F(x,y) = Sum over i and j of kx[j,i] * x^i * y^j
;       where kx is returned as the result or a keyword.
;
; MODIFICATION HISTORY:
;       05/01/2008, Eduard Westra    Initial creation
;       09/01/2008, EW               Added weight and sigma keywords
;
;-
FUNCTION lz, x, y, z, i, j, w
  RETURN, TOTAL(w*z*x^i*y^j)
END

FUNCTION ls, x, y, i, j, w
  RETURN, TOTAL(w*x^i*y^j)
END

FUNCTION sfit_xy, x, y, z, xdegree, ydegree, kx=kx, sigma=sigma, weight=weight
  COMPILE_OPT STRICTARRSUBS, IDL2
  ON_ERROR, 2
  IF N_ELEMENTS(x) EQ 0 OR N_ELEMENTS(y) EQ 0 OR N_ELEMENTS(z) EQ 0 THEN MESSAGE, 'Empty x, y or z...'
  IF N_ELEMENTS(x) NE N_ELEMENTS(y) THEN MESSAGE, 'Unequal x and y...'
  IF N_ELEMENTS(x) NE N_ELEMENTS(z) THEN MESSAGE, 'Unequal x and z...'

  IF N_ELEMENTS(sigma) NE 0 AND N_ELEMENTS(weight) NE 0 THEN MESSAGE, 'You cannot specify uncertainties and weights at the same time...'
  IF N_ELEMENTS(sigma) NE 0 THEN BEGIN
     IF N_ELEMENTS(sigma) NE N_ELEMENTS(x) THEN MESSAGE, 'Incorrect number of uncertainties...'
     weight = 1d/sigma^2
  ENDIF
  IF N_ELEMENTS(weight) NE 0 THEN BEGIN
     IF N_ELEMENTS(weight) NE N_ELEMENTS(x) THEN MESSAGE, 'Incorrect number of weights...'
  ENDIF
  IF N_ELEMENTS(sigma) EQ 0 AND N_ELEMENTS(weight) EQ 0 THEN weight = DBLARR(N_ELEMENTS(x)) + 1d

  m = (ULONG(xdegree)+1) * (ULONG(ydegree)+1)
  IF N_ELEMENTS(x) LT m THEN MESSAGE, 'Not enough points to get a unique solution...'
  a  = DBLARR(m, m)
  b  = DBLARR(m)
  pw = DBLARR(m, m, 2)
  FOR j = 0UL, ydegree DO FOR i = 0UL, xdegree DO BEGIN
     k    = (xdegree+1) * j + i
     b[k] = lz(x, y, z, i, j, weight)
     pw[k, 0, *] = [i, j]
     pw[0, k, *] = [i, j]
  ENDFOR

  FOR i = 1UL, m-1 DO FOR j = 1UL, m-1 DO BEGIN
     pw[i, j, *] = REFORM(pw[i, 0, *]) + REFORM(pw[0, j, *])
  ENDFOR

  FOR i = 0UL, m-1 DO FOR j = 0UL, m-1 DO BEGIN
     a[i,j] = ls(x, y, pw[i, j, 0], pw[i, j, 1], weight)
  ENDFOR

  kx = LA_LINEAR_EQUATION(a, b, /DOUBLE)
  kx = TRANSPOSE(REFORM(kx, (xdegree+1), (ydegree+1)))
  RETURN, kx
END
