;+
; NAME:
;    NImlostr
;
; PURPOSE:
;    Maximum Likelihood reconstruction algorithm for transmission 
;    tomography. Combined with Ordered Subsets (Hudson, Larkin, IEEE-TMI
;    1994;13:601-609). The algorithm is a simple gradient algorithm,
;    developed at K.U.Leuven.
;
;
; CALLING SEQUENCE:
;    recon = NImlostr(sinogram, blank)
;
; INPUTS:
;    SINOGRAM: the sinogram. First coordinate is the pixel in the projection,
;              second coordinate points to the projection angle.
;
;    BLANK: the blank scan. Dimensions similar to sinogram. Blank is the
;           scan without attenuating object, sinogram the scan with
;           attenuating object.
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
;      A value proportional the log-likelihood (similar to ML-EM) is
;      calculated for each iteration. In fact, the variable part of the
;      log-likelihood is calculated, and the maximum attainable
;      value (reached when calculated projection is identical to the
;      measurement) is subtracted. The log-likelihood should increase to
;      but never reach 0, except maybe for simulations.
;      If the keyword is omitted, the likelihood is not calculated.
;      If the keyword is set to a non-existing variable, that variable is
;      set to an array with consecutive likelihoods.
;      If the keyword is set to an array, the new array is appended.
;      For multiple subsets, only an approximate value is calculated, using
;      only the new reprojections of the current subset, together with the
;      older (and actually obsolete) reprojections of the previously processed
;      subsets. As a result, the approximate value is always lower than the
;      true value.
;      If multiple subsets are used in a second call to NImlostr, it is
;      recommended that you would provide the previously obtained CALCPROJMU.
;      If you don't, the procedure cannot use the reprojections of the
;      previous iteration, resulting in strong underestimations of the
;      likelihood. This does not influence the final reconstruction image.
;
;    CALCPROJMU
;      Calculated sinogram. If keyword is set but the image does not exist,
;      it is created. If it exists, it is reused. In subsequent calls to
;      NImlostr for the same image, the procedure needs the previously
;      calculated projection of the mu-values (the linear attenuation
;      coefficients) for the calculation of the likelihood, if multiple
;      subsets are used. The sinogram is calculated anyway, so no time is
;      saved by not providing it.
;      
;    FIRSTANGLE
;      Angle corresponding to first row of sinogram (in radians).
;      Default: 0.
;
;    DELTAANGLE
;      Angle between two consecutive projections (in radians).
;      Default: pi / number of angles
;
;    RELAXATION
;      Relaxation factor, multiplied with the update at every iteration.
;      To our experience, relaxation = 1 always converges, values up to 3 are
;      reasonable, higher values may lead to oscillations.
;      Default: 1
;
;    PCOEFF
;      If the keyword is not set or equal to zero, (back)projections are
;      computed with the RIEMANN command.
;      If the keyword is set to the result of a call to NIkul_coeff, the
;      (back)projections are computed by calling a routine from the
;      c-library.
;
;    - - - - - - - - - - - - - - - - - - - - - - - 
;    keywords for multimodel a-priori distrubition
;     - - - - - - - - - - - - - - - - - - - - - -
;    PRIORMEAN,
;    PRIORSTD,
;    PRIORWIDTH,
;    PRIORWEIGHT: see NImltrans.pro
;
;    - - - - - - - - - - - - - - - - - -
;    keywords for smoothing Gibbs prior.
;     - - - - - - - - - - - - - - - - -
;    MARKOVWEIGHT,
;    NEIGHBORWEIGHTS,
;    QUADRATIC,
;    HUBER,
;    GEMAN_MCCLURE,
;    DELTAMARKOV: see NImltrans.pro
;
;    - - - - - - - - - - - - - -
;    FESSLER:     see NImltrans.pro
;
;    CONTAMINATION: a sinogram representing the estimated
;       contamination (e.g. scatter, randoms, emission...).
;       See NImltrans for details.
;
; OUTPUTS:
;    RECON: the resulting reconstruction.
;
;    MEASBACK: When non-existing or equal to a single value, it is set to
;              the backprojection of the transmission scan. See input
;              parameters.
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
;    Calls NIsino_subset to define the sinogram subsets, and NImltrans for
;    the actual ML-gradient calculations.
;
; EXAMPLE:
;
; SEE ALSO: NImltrans
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, K.U.Leuven, Belgium
;                       sep-1995
;       Multimodal priors added:     Johan Nuyts, dec 1995
;       Markov Random priors added:  Johan Nuyts, mar 1997
;       Contamination correction:    Johan Nuyts, jan 1999.
;-


function NImlostr, sinogram, blank, nrsubsets = nrsubsets, recon = recon, $
              nriter = nriter, showsub = showsub, showfinal = showfinal, $
              likelihood = likelihood, calcprojmu = calcprojmu, $
              firstangle = firstangle, deltaangle = deltaangle, $
              relaxation = relaxation, onlysubset = onlysubset, $
              measback = measback, fwhm= fwhm, $
              norm_per_subset=norm_per_subset, $
              priormean = priormean, priorstd = priorstd, $
              priorweight = priorweight, priorwidth=priorwidth, $
              pcoeff=pcoeff, $
              markovweight = markovweight, deltamarkov=deltamarkov, $
              neighborweights=neighborweights, huber=huber, $
              quadratic=quadratic, geman_mcclure = geman_mcclure, $
              fessler = fessler, contamination=contamination

if n_elements(onlysubset) eq 0 then onlysubset = -1


; Eliminate negative values
; Allocate recon-image if not provided
;--------------------------
sino = sinogram > 0
if n_elements(nriter)    eq 0 then nriter    = 1
if n_elements(nrsubsets) eq 0 then nrsubsets = 1
sinosize   = size(sino)
nrangles   = sinosize(2)
if n_elements(recon) le 1 $
  then recon      = fltarr(sinosize(1), sinosize(1)) + 0.01
if n_elements(deltaangle) eq 0 then deltaangle = !pi / nrangles
if n_elements(firstangle) eq 0 then firstangle = 0
if n_elements(relaxation) eq 0 then relaxation = 1.0

; Allocate memory for calculated sinogram
;---------------------------------
iter = 0
if n_elements(calcprojmu) le 1 then calcprojmu = sino * 0

; Calculate upper limit of likelihood
;---------------------------
if n_elements(likelihood) eq 0 then likelihood = -1
lik        = 1
dummy      = sino > 1e-12
maxlik     = total(-dummy - dummy * alog(blank / dummy))

; Precompute backprojection of measurement using all projections, except
; if norm_per_subset is active.
;-----------------------------
IF n_elements(measback) LE 1 THEN $
  IF NOT keyword_set(norm_per_subset) THEN BEGIN
    if n_elements(contamination) le 1 then begin
      print,'backprojecting measurement...'
      NIproj, measback, NIconvolgauss(sino, fwhm=fwhm, dim=[0]), $
              firstangle, deltaangle, nrangles, /bil, /back, $
              pcoeff = pcoeff
    endif else begin
      print,'backprojecting corrected measurement...'
      tmpsino = (sino - contamination)^2 / (sino > 1.0)
      NIproj, measback, NIconvolgauss(tmpsino, fwhm=fwhm, dim=[0]), $
              firstangle, deltaangle, nrangles, /bil, /back, $
              pcoeff = pcoeff
    endelse
  ENDIF

; Loop of iterations and subsets
;-----------------------
for i = 1,nriter do begin
  subsetnum = 0
  for s = 0, nrsubsets - 1 do begin
    subset = NIsino_subset(nrsubsets, nrangles, subsetnum, subsetmask, $
                           previoussubset)
    IF keyword_set(norm_per_subset) THEN measback = 0
    if (onlysubset eq -1) or (s eq onlysubset) then begin
      NImltrans, recon, sino, blank, measback, firstangle, deltaangle, $
             likeli = lik, fwhm= fwhm, $
             calc = calcprojmu, subset = subset, relaxation = relaxation, $
             priormean = priormean, priorstd = priorstd, $
             priorweight = priorweight, priorwidth=priorwidth, $
             pcoeff = pcoeff, $
              markovweight = markovweight, deltamarkov=deltamarkov, $
              neighborweights=neighborweights, huber=huber, $
              quadratic=quadratic, geman_mcclure = geman_mcclure, $
              fessler = fessler, contamination=contamination
      lik = lik - maxlik
      if iter eq 0 and likelihood(0) eq -1 $
                        then likelihood = [lik] $
                        else likelihood = [likelihood, lik]
      if keyword_set(showsub) then NIimage, recon
      iter = iter + 1
    endif
  endfor
  if keyword_set(showfinal) then NIimage, recon
endfor
return, recon
end
