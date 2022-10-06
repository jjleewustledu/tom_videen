;+
; NAME:
;    NImlosem
;
; PURPOSE:
;    Maximum Likelihood expectation maximisation (Shepp, Vardi, IEEE TMI 1982,
;    MI-1:113-122  and Lange, Carson, J Comput Assist Tomog, 1984, 8:306-316).
;    The procedure also implements Ordered Subsets (Hudson, Larkin, IEEE-TMI
;    1994;13:601-609).
;
;
; CALLING SEQUENCE:
;    recon = NImlosem(sinogram)
;
; INPUTS:
;    SINOGRAM: the sinogram. First coordinate is the pixel in the projection,
;              second coordinate points to the projection angle.
;
;	
; KEYWORD PARAMETERS:
;
;    RECON
;      The initial image. If not provided, a homogenous image is used.
;      Supply the previous reconstruction if you want to continue iterating
;      on the same image.
;      
;    NRITER
;      Number of iterations. For multiple subsets, one iteration includes
;      one ML-EM-iteration per subset.
;
;    NRSUBSETS
;      The number of subsets. Any number is accepted, it does not have to
;      be devider of the number of projections. If it is not, the subsets
;      will not have the same amount of projections.
;      See Hudson and Larkin - paper.
;      For multiple subsets, the calculation of the likelihood is approximate,
;      and is always lower than the true value. See the LIKELIHOOD keyword.
;      Default: 1.
;
;    ONLYSUBSET
;      If set, only the subset with the number ONLYSUBSET will be used in the
;      ML-computation. When not set, the ML-computation is carried out for
;      all subsets.
;
;    SHOWSUB
;      When set, shows the reconstruction after each sub-iteration in the
;      current display window.
;
;    SHOWFINAL
;      When set, shows the reconstruction after each iteration in the current
;      display window.
;
;    LIKELIHOOD
;      A value proportional the log-likelihood (See Shepp and Vardi-paper) is
;      calculated for each iteration. In fact, the variable part of the
;      log-likelihood is calculated, and devided by the maximum attainable
;      value (reached when calculated projection is identical to the
;      measurement). The log-likelihood should increase but never reach 1,
;      except maybe for simulations.
;      If the keyword is omitted, the likelihood is not calculated.
;      If the keyword is set to a non-existing variable, that variable is
;      set to an array with consecutive likelihoods.
;      If the keyword is set to an array, the new array is appended.
;      For multiple subsets, only an approximate value is calculated, using
;      only the new reprojections of the current subset, together with the
;      older (and actually obsolete) reprojections of the previously processed
;      subsets. As a result, the approximate value is always lower than the
;      true value.
;      If multiple subsets are used in a second call to NImlosem, it is
;      recommended that you would provide the previously obtained CALCSINO.
;      If you don't, the procedure cannot use the reprojections of the
;      previous iteration, resulting in strong underestimations of the
;      likelihood. This does not influence the final reconstruction image.
;
;    CALCSINO
;      Calculated sinogram. If keyword is set but the image does not exist,
;      it is created. If it exists, it is reused. In subsequent calls to
;      NImlosem for the same image, the procedure needs the previously
;      calculated sinogram for the calculation of the likelihood, if multiple
;      subsets are used. The sinogram is calculated anyway, so no time is
;      saved by not providing it.
;      
;    STARTANGLE
;      Angle corresponding to the first projection (in radians). Default: 0.
;
;    DELTAANGLE
;      Angle between two consecutive projections (in radians).
;      Default: pi / number of angles
;
;    ATTENUATION
;      An image of the same size as sino, containing the attenuation factors.
;      For the ECAT 931 PET, this the inverse (pixel by pixel) of the atn-file,
;      which contains the attenuation correction.
;
;    PCOEFF
;      If the keyword is not set or equal to zero, (back)projections are
;      computed with the RIEMANN command.
;      If the keyword is set to the result of a call to NIkul_coeff, the
;      (back)projections are computed by calling a routine from the
;      c-library.
;
;    BACKATTEN
;      In every iteration, the backprojected ratio of measurement and
;      prediction must be divided by the sum of detection probabilities.
;      This sum equals 1 of no PET- or SPECT-attenuation is specified.
;      If there is attenuation, the sum of detection probabilities is
;      computed by backprojecting the a sinogram with all pixels set to 1.
;      If backatten does not exist or is set to a single value, this
;      backprojection is computed. If backatten is set, it is reused.
;
;    NORM_PER_SUBSET
;      In theory, BACKATTEN should be recomputed for every subset.
;      However, a lot of time is saved by computing BACKATTEN once using all
;      projections, and using it also for individual subsets.
;      When NORM_PER_SUBSET is set, BACKATTEN is recomputed for every subset.
;      Otherwise, it is just computed once using all projections.
;
; OUTPUTS:
;    RECON: the resulting reconstruction.
;
;
; COMMON BLOCKS:
;    Calls NIimage, which reads the lookup table block.
;
; SIDE EFFECTS:
;    Displays an image in the current display window, if the keyword
;    SHOWSUB or SHOWFINAL is set.
;
; RESTRICTIONS:
;    The projections are calculated using the RIEMANN function, which expects
;    that the projection array contains more pixels than the dimension of
;    the reconstruction image. This routine uses the same number of columns
;    for the projection as for the reconstruction image, apparantly without
;    adverse effects.
;
; PROCEDURE:
;    Calls NIsino_subset to define the sinogram subsets, and NIml for
;    the actual ML-EM calculations.
;
; EXAMPLE:
;    See batchfile NIexample_ml.pro.
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, K.U.Leuven, Belgium
;                       aug-1995
;-


function NImlosem, sinogram, nrsubsets = nrsubsets, recon = recon, $
                   nriter = nriter, showsub = showsub, showfinal = showfinal, $
                   likelihood = likelihood, calcsino = calcsino, $
                   startangle = startangle, deltaangle = deltaangle, $
                   attenuation = attenuation, spectatten = spectatten, $
                   backatten = backatten, norm_per_subset=norm_per_subset, $
                   onlysubset = onlysubset, pcoeff = pcoeff, fwhm=fwhm


if n_elements(onlysubset) eq 0 then onlysubset = -1
if n_elements(startangle) eq 0 then startangle = 0

; Eliminate negative values
; Allocate recon-image if not provided
;--------------------------
sino = sinogram > 0
if n_elements(nriter)    eq 0 then nriter    = 1
if n_elements(nrsubsets) eq 0 then nrsubsets = 1
sinosize   = size(sino)
nrangles   = sinosize(2)
if sinosize(0) lt 3 then nplanes = 1 $
                    else nplanes = sinosize(3)
if n_elements(recon) le 1 $
  then recon      = fltarr(sinosize(1), sinosize(1), nplanes) + 1
if n_elements(deltaangle) eq 0 then deltaangle = acos(-1) / nrangles

; Allocate memory for calculated sinogram
;---------------------------------
iter = 0
if n_elements(calcsino) le 1 then calcsino = sino * 0

; Calculate upper limit of likelihood
;---------------------------
if n_elements(likelihood) eq 0 then likelihood = -1
lik        = 1
dummy      = sino
isnul      = where(dummy le 1e-15)
if isnul(0) ne -1 then dummy(isnul) = 1e-15
maxlik     = total(-dummy + dummy * alog(dummy))

; Precompute the backprojection of the weights if attenuation is specified.
; If not done, NIml will do it the first time, but then the computation
; is restricted to the subset if one is in use.
;----------------------------
; If PET-attenuation is required, compute the sum of all weights in 
; every reconstruction pixel
;----
if n_elements(attenuation) gt 1 and n_elements(backatten) le 1 $
  AND NOT keyword_set(norm_per_subset) then BEGIN
  print, 'Precomputing backatten...'
  backatten = recon * 0
  NIproj, backatten, attenuation, startangle, deltaangle, nrangles, $
          /bil, /back, pcoeff=pcoeff
  print, 'done.'
endif

; If SPECT-attenuation is required, compute the sum of all weights in 
; every reconstruction pixel
;----
if n_elements(spectatten) gt 1  and n_elements(backatten) le 1 $
  AND NOT keyword_set(norm_per_subset) then begin
  print, 'Precomputing backatten...'
  backatten = recon * 0
  allones   = sino * 0 + 1
  NIproj, backatten, allones, startangle, deltaangle, nrangles, $
         /bil, /back, spectatten=spectatten, pcoeff=pcoeff
  print, 'done.'
endif

; Loop of iterations and subsets
;-----------------------
for i = 1,nriter do begin
  subsetnum = 0
  for s = 0, nrsubsets - 1 do begin
    subset = NIsino_subset(nrsubsets, nrangles, subsetnum, subsetmask, $
                           previoussubset)
    IF keyword_set(norm_per_subset) AND nrsubsets GT 1 THEN backatten =  0
    if (onlysubset eq -1) or (s eq onlysubset) then begin
      NIml, recon, sino, startangle, deltaangle, likeli = lik, $
            pcoeff = pcoeff, calc = calcsino, subset = subset, $
            attenuation = attenuation, spectatten=spectatten, $
            backatten = backatten, fwhm=fwhm
      lik = lik / maxlik
      if iter eq 0 and likelihood(0) eq -1 $
                        then likelihood = [lik] $
                        else likelihood = [likelihood, lik]
      if keyword_set(showsub) then NIimage, recon, /glob
      iter = iter + 1
    endif
  endfor
  if keyword_set(showfinal) then NIimage, recon, /glob
endfor
return, recon
end

