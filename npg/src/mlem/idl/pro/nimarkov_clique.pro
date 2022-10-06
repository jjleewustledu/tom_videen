;+
; NAME:
;    NImarkov_clique
;
; PURPOSE:
;    Computes the contribution of a single markov clique. Normally called
;    from NImarkov
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    RESULT = NImarkov(img, neigbhorweights, weight = weight, $
;                       delta = delta, /gradient, ...)
;
; INPUTS:
;    IMG: the input image on which the logarithm of the Gibbs prior is to be
;         calculated.
;
;    NEIGHBORWEIGHTS: typically a 3x3 mask, containing the weights of each
;          neighbor. It is assumed that every pixel except the central one
;          forms a single clique together with the central pixel.
;          This means that for large neighborhoods, all pixels are treated
;          as neighbors of the central pixel.
;          The weight of the central pixel is ignored.
;
; KEYWORD PARAMETERS:
;
;    HUBER: when set, the Huber prior is used.
;          See Mumcuoglu et al, Phys Med Biol 1996; 41: 1777-1807
;
;    QUADRATIC: when set, the quadratic prior is used.
;          See Mumcuoglu et al, Phys Med Biol 1996; 41: 1777-1807
;
;    GEMAN_MCCLURE: when set, the Geman-Mcclure prior is used.
;          E. Mumcuoglu et al, IEEE Trans Med Imaging 1994; 13: 687-701
;
;    DELTA: For the quadratic prior, delta is the standard deviation:
;               prior = (x1 - x2)^2 / (2 * delta^2)
;        
;           For the Huber prior, delta has a similar meaning.
;           For |x1-x2| < delta, Huber is identical the the quadratic prior,
;           for |x1-x2| > delta, Huber becomes linear:
;                     (|x1 - x2| - delta/2) / delta
;
;           For the Geman and McClure prior, delta is also similar:
;           prior = (x1 - x2)^2 / (2 * delta^2 + (x1 - x2)^2)
;
;    WEIGHT: the prior is multiplied with this value.
;
;    GRADIENT: when set, the gradient of the log prior is computed. Otherwise
;              the log prior is computed. For inclusion in iterative
;              reconstruction, only the gradient is needed.
;
;    G2PRIOR: when present, receives an estimate for minus the second
;             derivative, which can be used to control the step size.
;             Similar as in NIgrad_priorvalues.
;
; OUTPUTS:
;    RESULT: the resulting image containing the prior or gradient of the
;            prior in every pixel.
;
; SEE ALSO:
;    NImarkov, NImltrans, NImlostr, NIgrad_priorvalues
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, march 1997
;-

;------------------------------------------------------------------------
;
; The Huber function has been slightly adapted, to give delta a physical
; meaning, and make it dimensionless. It is now similar to a standard
; deviation. For small |x1 - x2|, it reduces to the quadratic form, with
; standard deviation delta.
;
;                                 (x1 - x2)^2
; if |x1 - x2|  < delta     ==> - -----------
;                                  2 * delta^2
;
;                                  (x1 - x2) - delta / 2
; if |x1 - x2| >= delta     ==> - -----------------------
;                                       delta
;
function NIhuber, x1, x2, delta, weight, gradient = gradient

result = abs(x1 - x2)
daar1 = where(result le delta)
daar2 = where(result gt delta)

if not keyword_set(gradient) then begin
  if daar1(0) ne -1 then $
    result(daar1) = - result(daar1)^2 * (weight / (2.0 * delta * delta))

  if daar2(0) ne -1 then $
    result(daar2) = - (result(daar2) - delta / 2.0) * (weight / delta)

endif else begin
  if daar1(0) ne -1 then $
    result(daar1) = - (x1(daar1) - x2(daar1)) $
                      * (weight / (delta * delta))

  if daar2(0) ne -1 then $
    result(daar2) = - ((x1(daar2) gt x2(daar2)) * 2. - 1.) * weight / delta

endelse

return, result
end

;------------------------------------------------------------------------
; Geman and McClure, delta scaled such that for decreasing |x1 - x2|, it
; converges to the Huber prior and to the quadratic form.
;
;           (x1 - x2)^2
;  -   -----------------------
;      2 delta^2 + (x1 - x2)^2
;
;
function NIgeman_macclure, x1, x2, delta, weight, gradient = gradient

kwaddiff = (x1 - x2)^2

if not keyword_set(gradient) then begin
  return, - kwaddiff / (2. * delta*delta + kwaddiff) * weight

endif else begin
  dummy = kwaddiff + 2. * delta * delta
  return, - (2. * weight) * (x1 - x2) / dummy * (1 - kwaddiff / dummy)
endelse
end

;========================================================================
function NImarkov_clique, image, coloffset, rowoffset, g2prior = g2prior, $
            weight = weight, delta=delta, gradient = gradient, $
            huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure

if n_elements(weight) eq 0 then weight = 1.0

imsize = size(image)
ncols = imsize(1)
nrows = imsize(2)
if n_elements(delta) eq 0 then delta = max(image/7)

shiftimage = shift(image, -coloffset, -rowoffset)

if abs(coloffset) gt 0 then $
  if coloffset gt 0 $
    then shiftimage(ncols - coloffset : ncols-1, *) = $
              image(ncols - coloffset : ncols-1, *) $
    else shiftimage(0: -coloffset-1, *) = image(0: -coloffset-1, *)
if abs(rowoffset) gt 0 then $
  if rowoffset gt 0 $
    then shiftimage(*, nrows - rowoffset : nrows-1) = $
              image(*, nrows - rowoffset : nrows-1) $
    else shiftimage(*, 0: -rowoffset-1) = image(*, 0: -rowoffset-1)

;
; For minus the second derivative, we always use the second derivative of the
; quadratic form. For Huber or Geman, it is a good approximation if 
; |x1 - x2| is small, and an overestimation when |x1 - x2| is large.
; Since g2prior will be used in the denomitor in order to estimate the
; step size, a save estimate will be obtained. (Note that for Huber,
; g2prior is zero for large |x1-x2| and even negative for Geman.)
; Consequently, g2prior is the same for all pixels, and a single value
; is returned.
;
g2prior = weight / (delta * delta)

case 1 of
  keyword_set(huber) : $
     return, NIhuber(image, shiftimage, delta, weight, gradient=gradient)
  keyword_set(quadratic) : begin
     if keyword_set(gradient) $
       then return, - (image - shiftimage) * (weight / (delta * delta)) $
       else return, - (image - shiftimage)^2 * (weight / (2.0 * delta * delta))
     end
  keyword_set(geman_mcclure) : $
     return, NIgeman_macclure(image, shiftimage, delta, weight, $
                              gradient=gradient)
  else: begin
     print, "NImarkov_clique: select huber, quadratic or geman_mcclure prior"
     return, -1
     end
endcase

end
