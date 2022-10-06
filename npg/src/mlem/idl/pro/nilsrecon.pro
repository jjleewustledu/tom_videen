;+
; NAME:
;    NIlsrecon
;
; PURPOSE:
;    Apply a single least squares iteration, possibly restricted to a single
;    subset.
;
; CALLING SEQUENCE:
;     NIlsrecon, recon, sino, variance, firstangle, deltaangle
;
; INPUTS:
;    RECON
;      The initial reconstruction image, which may be the result of a previous
;      iteration. Note that ML-EM is unable to convert zeros in something else,
;      since it is a multiplicative algorithm. Do not supply initial images
;      with pixels set to zero, unless you want them to stay zero.
;
;    SINO
;      The sinogram: first coordinate points to the detector or line of
;      response, the second to the projection angle.
;
;    VARIANCE
;      An image of same dimensions as sinogram, containing the (estimated)
;      variance on each of the sinogram pixels. Since variance occurs in the
;      denominator, it should NOT contain zero values.
;      For emission sinograms, the variance equals the measured count.
;      When not provided, the variance of all pixels is assumed to be 1.
;
;    FIRSTANGLE
;      First projection angle. Zero for vertical projection.
;
;    DELTAANGLE
;      Angle between the projections (in radians)
;
;	
; KEYWORD PARAMETERS:
;    LIKELIHOOD
;      If this array exists, the likelihood of the initial reconstruction image
;      is appended. The likelihood of the resulting image is only calculated
;      in the next iteration. The likelihood for subiterations is always an
;      underestimation. For an accurate calculation of the likelihood, a
;      complete projection of the image must be calculated. It is the goal
;      of Ordered Subsets to avoid this calculation.
;
;    CALCULSINO
;      Provides and recieves the calculated sinogram. If subsets are used,
;      the calculation of the likelihood needs the previously calculated
;      sinogram to compute a decent value. If you don't provide it, an internal
;      one filled with zeros is generated. It then takes one iteration using
;      all subsets before the likelihood value is meaningful.
;
;    SUBSET
;      An array indicating the rows of the sinogram that are included in the
;      subsets. The routine NIsino_subset generates subsets.
;
;    ATTENUATION
;      An image of the same size as sino, containing the attenuation factors,
;      and any other factor affecting the sensitivity of a LOR.
;      For the ECAT 931 PET, this the inverse (pixel by pixel) of the atn-file,
;      which contains the attenuation correction.
;      Originally designed for PET-applications. For SPECT, however, one may
;      may use it to apply uniformity correction.
;
;    SPECTATTEN
;      An image of the same size as the reconstruction image, containing an
;      attenuation map. The pixels must contain linear attenuation coefficients
;      per pixelsize. (See also NIproj.)
;
;    PCOEFF
;      If the keyword is not set or equal to zero, (back)projections are
;      computed with the RIEMANN command.
;      If the keyword is set to the result of a call to NIkul_coeff, the
;      (back)projections are computed by calling a routine from the
;      c-library.
;
;    BACKATTEN
;      The backprojection of the weighting. This image must be computed when
;      attenuation or spectatten or specified. If this variable is set to
;      an existing image (more than one element), that image is reused.
;      Otherwise, the image is computed.
;
; OUTPUTS:
;    RECON is updated. 
;
; OPTIONAL OUTPUTS:
;    If provided, also CALCULSINO and LIKELIHOOD are updated.
;
; COMMON BLOCKS:
;    none
;
; SIDE EFFECTS:
;    none
;
;
; PROCEDURE:
;    Calls NIproj to calculate the projection and the backprojection.
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, K.U.Leuven, Belgium
;                       aug-1995
;-


pro NIlsrecon, recon, sino, variance, firstangle, deltaangle, $
          likelihood = likelihood, calculsino = calculsino, subset = subset, $
          attenuation = attenuation, spectatten=spectatten, pcoeff = pcoeff, $
          backatten = backatten, relaxation = relaxation, emission=emission, $
          backmlem= backmlem

  if n_elements(relaxation) eq 0 then relaxation = 1.0

  small    = max(sino) / 1e6
  sinosize = size(sino)
  nrdet    = sinosize(1)
  nrangles = sinosize(2)

  if n_elements(variance) eq 0 then variance = fltarr(nrdet, nrangles) + 1.0

  ; Default: 1 subset
  ;------------------
  if n_elements(subset) eq 0 then subset = indgen(nrangles)

  ; project the current reconstruction
  ;--------
  if n_elements(calculsino) eq 0 $
    then calculsino = 0 * sino $
    else calculsino(*,subset,*) = 0
  NIproj, recon, calculsino, firstangle, deltaangle, nrangles, /bil, $
          subset = subset, spectatten = spectatten, pcoeff=pcoeff
          
  ; Attenuate if requested
  ; (Not needed for the algorithm: attenuation cancels in the ratio.
  ;  However: done to obtain a calculated sinogram that can be compared
  ;  directly to the measured one.)
  ;---------
  if n_elements(attenuation) gt 1 then $
    calculsino(*,subset,*) = calculsino(*,subset,*) * attenuation(*,subset,*)
  
  ; subtract reprojection from measurement and divide by variance
  ;---------------------------------------
  verschil = (sino - calculsino) / variance
  
  ; Backproject this sinogram.
  ; First part of the backprojection is the attenuation, if an attenuation file
  ; is provided. In that case, every backprojection pixel must also be divided
  ; by the backprojection of the weighting coefficients.
  ;---------
  ; Multiply with attenuation
  ;---------
  if n_elements(attenuation) gt 1 then $
    verschil(*,subset,*) = verschil(*,subset,*) * attenuation(*,subset,*)
  back = recon * 0

  ; Backproject the difference-sinogram
  ;-----
  NIproj, back, verschil, firstangle, deltaangle, nrangles, /bil, /back, $
          subset = subset, pcoeff=pcoeff, spectatten = spectatten

  ; If PET-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(attenuation) gt 1 and n_elements(backatten) le 1 then begin
    backatten = recon * 0
    NIproj, backatten, attenuation/variance, firstangle, deltaangle, $
            nrangles, /bil, /back, subset = subset, pcoeff=pcoeff
    ;print, 'NIml: computing backprojecion of PET-attenuation weights'
  endif
  ; If SPECT-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(spectatten) gt 1  and n_elements(backatten) le 1 then begin
    backatten = recon * 0
    NIproj, backatten, 1./variance, firstangle, deltaangle, nrangles, $
           /bil, /back, subset = subset, spectatten=spectatten, pcoeff=pcoeff
  endif
  if n_elements(backatten) le 1 then begin
    backatten = recon * 0
    NIproj, backatten, 1./variance, firstangle, deltaangle, nrangles, $
            /bil, /back, subset = subset, pcoeff=pcoeff
  endif
  
  ; For the MLEM-step.
  ; Precompute the backprojection of the weights if attenuation is specified.
  ; If not done, NIml will do it the first time, but then the computation
  ; is restricted to the subset if one is in use.
  ;----------------------------
  ; If PET-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(attenuation) gt 1 and n_elements(backmlem) le 1 $
    AND NOT keyword_set(norm_per_subset) then BEGIN
    print, 'Precomputing backmlem...'
    backmlem = recon * 0
    NIproj, backmlem, attenuation, startangle, deltaangle, nrangles, $
            /bil, /back, pcoeff=pcoeff
    print, 'done.'
  endif
  
  ; If SPECT-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(spectatten) gt 1  and n_elements(backmlem) le 1 $
    AND NOT keyword_set(norm_per_subset) then begin
    print, 'Precomputing backmlem...'
    backmlem = recon * 0
    allones   = sino * 0 + 1
    NIproj, backmlem, allones, startangle, deltaangle, nrangles, $
           /bil, /back, spectatten=spectatten, pcoeff=pcoeff
    print, 'done.'
  endif
  ; 
  ;
  weight1 = float(relaxation)/float(nrdet) $
            / (backatten > (max(backatten) / 1000.))
  IF (n_elements(backmlem) GT 1) $ 
    THEN   weight2 = recon / (backmlem >  max(backmlem)/1000.)$
    ELSE   weight2 = recon
  IF keyword_set(emission) THEN weight = weight1 > weight2 $
                           ELSE weight = weight1
  
  ; Multiply with current reconstruction
  ;---------
  recon = recon  + weight * back

  if n_elements(likelihood) ne 0 then begin
    isnul = where(calculsino le 0)
    if isnul(0) ne -1 then calculsino(isnul) = 1e-15
    likelihood = total(-calculsino + sino * alog(calculsino))
  endif

end





