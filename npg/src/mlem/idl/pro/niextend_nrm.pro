;+
; NAME:
;    NIextend_nrm
;
; PURPOSE:
;    Very simple extension of the sinogram, to avoid problems with zero
;    values at the edges.
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    result = NIextend_nrm( nrm )
;
; INPUTS:
;    NRM: image containing the normalization coefficients.
;
; OUTPUTS:
;    RESULT: the extended sinogram.
;
; PROCEDURE:
;    Any value lower than 1/10 x mean(nrm) is set to the mean of the
;    normalization image.
;    More sophisticated methods (using individual detector efficiencies and
;    geometric efficiencies) have been reported in the literature.
;    As a first approximation, this simplified extension is acceptable,
;    because the edges of the normalized sinogram contain very little
;    information.
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 9 jan 1996
;-

function NIextend_nrm, nrm

meannrm = mean(nrm)
result = nrm

daar = where(nrm lt 0.1 * meannrm)
if daar(0) ne -1 then $
  result(daar) = mean(nrm(where(nrm gt 0.1)))

return, result

end