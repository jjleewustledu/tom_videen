;+
; NAME:
;    NIml
;
; PURPOSE:
;    Apply a single ML-EM iteration, possibly restricted to a single subset.
;
;
; CALLING SEQUENCE:
;     NIml, recon, sino, firstangle, deltaangle
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


pro NIml, recon, sino, firstangle, deltaangle, $
          likelihood = likelihood, calculsino = calculsino, subset = subset, $
          attenuation = attenuation, spectatten=spectatten, pcoeff = pcoeff, $
          backatten = backatten, fwhm=fwhm

  small    = max(sino) / 1e6
  sinosize = size(sino)
  nrangles = sinosize(2)

  ; Default: 1 subset
  ;------------------
  if n_elements(subset) eq 0 then subset = indgen(nrangles)
  IF n_elements(fwhm) EQ 0 THEN fwhm = 0

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

  ; Convolve the sinogram if fwhm has been specified.
  ;--------------------------------------------------
  IF fwhm NE 0 THEN BEGIN
    calculsino(*,subset,*) = NIconvolgauss(calculsino(*,subset,*), $
                                           fwhm=fwhm, dim=[0])
    IF sinosize(0) EQ 3 THEN $
      IF sinosize(3) GT 1 THEN $
        calculsino(*,subset,*) = NIconvolgauss(calculsino(*,subset,*), $
                                           fwhm=fwhm, dim=[2])
  ENDIF
  
  ; devide measurement by reprojection
  ;--------
  noemer = calculsino(*,subset,*)
  isnul = where(noemer le small)
  if isnul(0) ne -1 then noemer(isnul) = small
  ratio = sino
  ratio(*,subset,*) = ratio(*,subset,*) / noemer
  
  ; Backproject this sinogram.
  ; First part of the backproj is convolution with the PSF.
  ; Second part of the backproj is the attenuation, if an attenuation file
  ; is provided. In that case, every backprojection pixel must also be divided
  ; by the backprojection of the weighting coefficients.
  ; Third part is the backprojection, possibly with spect attenuation.
  ;---------
  ; Multiply with attenuation
  ;---------
  ; Convolve the sinogram if fwhm has been specified.
  ;--------------------------------------------------
  IF fwhm NE 0 THEN BEGIN
    ratio(*,subset,*) = NIconvolgauss(ratio(*,subset,*), $
                                      fwhm=fwhm, dim=[0])
    IF sinosize(0) EQ 3 THEN $
      IF sinosize(3) GT 1 THEN $
        ratio(*,subset,*) = NIconvolgauss(ratio(*,subset,*), $
                                          fwhm=fwhm, dim=[2])
  ENDIF

  if n_elements(attenuation) gt 1 then $
    ratio(*,subset,*) = ratio(*,subset,*) * attenuation(*,subset,*)
  back = recon * 0

  ; Backproject the ratio-sinogram
  ;-----
  NIproj, back, ratio, firstangle, deltaangle, nrangles, /bil, /back, $
          subset = subset, pcoeff=pcoeff, spectatten = spectatten

  ; If PET-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(attenuation) gt 1 and n_elements(backatten) le 1 then begin
    backatten = recon * 0
    NIproj, backatten, attenuation, firstangle, deltaangle, nrangles, $
            /bil, /back, subset = subset, pcoeff=pcoeff
    ;print, 'NIml: computing backprojecion of PET-attenuation weights'
  endif
  ; If SPECT-attenuation is required, compute the sum of all weights in 
  ; every reconstruction pixel
  ;----
  if n_elements(spectatten) gt 1  and n_elements(backatten) le 1 then begin
    backatten = recon * 0
    allones   = sino * 0 + 1
    NIproj, backatten, allones, firstangle, deltaangle, nrangles, $
           /bil, /back, subset = subset, spectatten=spectatten, pcoeff=pcoeff
    ;print, 'NIml: computing backprojecion of SPECT-attenuation weights'
  endif
  ; If an image backatten has been generated, use it to normalize for
  ; the sum of weights.
  ;-----
  if n_elements(attenuation) gt 1 or n_elements(spectatten) gt 1 then $
    back = back / (backatten > (max(backatten) / 1000.))

  
  ; Multiply with current reconstruction
  ;---------
  recon = recon * back

  if n_elements(likelihood) ne 0 then begin
    isnul = where(calculsino le 0)
    if isnul(0) ne -1 then calculsino(isnul) = 1e-15
    likelihood = total(-calculsino + sino * alog(calculsino))
  endif

end





