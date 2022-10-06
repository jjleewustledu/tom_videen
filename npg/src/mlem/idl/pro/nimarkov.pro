;+
; NAME:
;    NImarkov
;
; PURPOSE:
;    Compute the Gibbs prior or its gradient, and optionally an estimate of its
;    second derivative. The gradient and second derivative can be used to
;    compute a step which maximizes the a priori probability.
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    result = NImarkov(image, [neighborweights,] $
;            weight = weight, delta=delta, [/gradient,] $
;            [/huber  (or)  /quadratic  (or)  /geman_mcclure], $
;            g2prior = g2prior, likelihood=likelhood)
;
; INPUTS:
;    IMAGE : The input image.
;
; OPTIONAL INPUTS:
;    NEIGHBORWEIGHTS:  typically a 3x3 mask, containing the weights of each
;          neighbor. It is assumed that every pixel except the central one
;          forms a single clique together with the central pixel.
;          This means that for large neighborhoods, all pixels are treated
;          as neighbors of the central pixel.
;          The weight of the central pixel is ignored.
;
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
;    LIKELIHOOD: When this variable exists, it is set to the log likelihood
;                of the prior. This requires the computation of the 
;                prior value in every pixel. When gradient is set, the
;                computation time for the likelihood is significant, because
;                it requires the calculation of the prior in every pixel.
;
; OUTPUTS:
;    RESULT: The resulting image, contain in every pixel the prior or the
;            gradient of the prior.
;
;
; RESTRICTIONS:
;    For neighborhoods larger than 3x3, all pixel-pairs consisting of the
;    central pixel and any neighborhood pixel are regarded as a clique.
;
; PROCEDURE:
;    Calls NImarkov_clique for every clique.
;
; SEE ALSO:
;    NImarkov_clique, NImltrans, NImlostr, NIgrad_priorvalues
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, march 1997
;-

function NImarkov, image, neighborweights, $
            weight = weight, delta=delta, gradient = gradient, $
            huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure, $
            g2prior = g2prior, likelihood = likelhood

if n_elements(weight) eq 0 then weight = 1
if n_elements(neighborweights) eq 0 then $
    neighborweights = [[1.,1.,1.], [1.,0,1.], [1.,1.,1.]] / 8.
nsize = size(neighborweights)

centercol = fix((nsize(1) - 1) / 2)
centerrow = fix((nsize(2) - 1) / 2)

imsize = size(image)
if imsize(0) ne 2 then begin
  print, "NImarkov: only implemented for two dimensions" + string(7b)
  return,-1
endif
result = fltarr(imsize(1), imsize(2))

g2prior = 0
for col = 0, nsize(1)-1 do begin
  for row = 0, nsize(2)-1 do begin
    if (col ne centercol) or (row ne centerrow) then begin
      cliqueweight = weight * neighborweights(col, row)
      IF cliqueweight NE 0 THEN BEGIN
        coloffset = col - centercol
        rowoffset = row - centerrow
        result = result + NImarkov_clique(image, coloffset, rowoffset, $
              weight = cliqueweight, delta=delta, gradient = gradient, $
              huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure,$
              g2prior = tmpg2prior)
        g2prior = g2prior + tmpg2prior
      ENDIF
    ENDIF
  ENDFOR
ENDFOR

if n_elements(likelihood) ne 0 then begin
  if not keyword_set(gradient) then begin
    likelihood = total(result)
  endif else begin
    dummy = NImarkov(image, neighborweights, weight = weight, delta=delta, $
            huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure)
    likelihood = total(dummy)
  endelse
endif

return, result
end
