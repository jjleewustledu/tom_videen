;+
; NAME:
;    NImltrans
;
; PURPOSE:
;    Iterative maximum likelihood reconstruction from a transmission scan
;    and a blank scan.
;
; CATEGORY:
;    Reconstruction from projections.
;
; CALLING SEQUENCE:
;    NImltrans, recon, sino, blank, measback, firstangle, deltaangle
;
; INPUTS:
;
;    RECON: Both input and output: it is the current reconstruction estimate,
;           which is updated in each call to NImltrans.
;
;    SINO: The measured transmission sinogram. Currently, this can only be
;          a 2D sinogram.
;
;    BLANK: The measured blank sinogram. This is the image of the transmission
;           source, obtained in absence of the attenuating object.
;           It is assumed that
;            1) the blank-sinogram is free of noise, all the noise is
;               attributed to Poisson noise in the transmission study.
;            2) the acquisition time, transmission source strength and
;               camera sensitivity (including dead time etc) are identical
;               in both scans. If this is not the case, the blank scan should
;               normalized for those differences.
;           BLANK and SINO must have identical size.
;
;    MEASBACK: The backprojection of the measured sinogram. If not provided,
;              or equal to a single value, it is computed.
;              The intention is that the user provides a non-existing variable,
;              which is initialized in the first call to NImltrans and reused
;              in subsequent calls.
;
;    FIRSTANGLE: The first projection angle in radians. It has the same
;                meaning as in NIproj.
;     
;    DELTAANGLE: The difference in projection angle between two consecutive
;                projections. It has the same meaning as in NIproj.
;    
;	
; KEYWORD PARAMETERS:
;
;    LIKELIHOOD: If this variable exists, it receives the likelihood computed
;                by comparing the computed transmission study (using the
;                input image RECON before updating it) with the measured
;                sinogram. If a prior is used, it is taken into account.
;
;    CALCPROJMU: The computed projections (sinogram) of the attenuation
;                coefficients. If provided, the old values are overwritten
;                by the new ones. If not provided, an internal, temporary
;                one is created.
;                When using subsets, it is adviced to provide it, otherwise
;                the approximate calculation of the likelihood after a
;                subiteration will be nonsense. 
;
;    SUBSET:     A subset, as produced by NIsino_subsets.pro.
;
;    FWHM:       The resolution, specified as Full Width at Half
;                Maximum in pixels. When specified, sinograms are
;                smoothed accordingly in 1D, corresponding to 2D
;                resolution correction within the slice.
;
;    RELAXATION: The additive update is multiplied with relaxation. In our
;                limited experience, good convergence is obtained for values
;                between 0 and 3. Higher values may lead to oscillations.
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
;    PRIORMEAN: When set to a weight value, the program takes into account
;            an a-priori probability distribution of attenuation
;            coefficients. PRIORMEAN is an array of likely a-priori
;            attenuation coefficients.  For this parameter and the 3
;            following ones, see NIgrad_priorvalues.
;
;    PRIORSTD: For each mean value, also a standard deviation can be
;              specified, specifying how (un)likely deviations from
;              the mean prior value are. This corresponds to the
;              relative strength of each mode of the prior.
;
;    PRIORWIDTH: A number specifying the relative width of the
;           prior. So one can have a strong prior (small priorstd) with a
;           large range (large priorwidth).
;
;    PRIORWEIGHT: Single value, representing the weight assigned to
;           the entire a-priori distribution.
;
;    - - - - - - - - - - - - - - - - - -
;    keywords for smoothing Gibbs prior.
;     - - - - - - - - - - - - - - - - -
;    MARKOVWEIGHT: When set to a value, a Gibbs prior is applied to
;       constrain the solution. One of the keywords QUADRATIC, HUBER or
;       GEMAN_MCCLURE should be set to select the prior function.
;       For this parameter and the five following ones, see NImarkov.
;
;    NEIGHBORWEIGHTS: A 3x3 neighborhood containing the weights of the
;       corresponding cliques. The central weight is ignored.
;
;    QUADRATIC: When set, the quadratic prior function is used.
;
;    HUBER: When set, the Huber prior function is used.
;
;    GEMAN_MCCLURE: When set, the Geman & McClure prior function is used.
;
;    DELTAMARKOV: The quadratic prior is actually the logarithm of a Gaussian
;       a-priori distribution. DELTA is the standard deviation of that
;       Gaussian. For pixels with a value that differs only slightly from
;       that of the neighborhood, Huber and Geman_McClure converge to the
;       quadratic function with standard deviation DELTA. If the difference
;       is large relative to DELTA, Huber becomes linear and Geman McClure
;       saturates to a constant value of 1.
;
;    - - - - - - - - - - - - - -
;
;    FESSLER:  The algorithm is:
;           
;                relaxation   (calcback - measback   + gradient-of-priors)
;     delta mu = ---------- * --------------------------------------------
;                 reconcols   numerator + relaxatie * sum-second-deriv-priors
;
;     If the priors are turned off the ML-gradient ascent algorithm is
;     obtained:
;
;                relaxatie    calcback - measback
;     delta mu = --------- *  --------------------
;                reconcols        numerator
;
;     where calcback is the backproj. of the calculated projections
;     and   measback is the backproj. of the measured projections.
;
;     Our own algorithm uses      numerator == calcback.
;     When FESSLER is set, we use numerator == measback.
;
;     Near the solution, this makes virtually no difference, because
;     calcback and measback will be very similar.
;     Using numerator == measback, the algorithm becomes a member of
;     Fessler's class of algorithms with guaranteed
;     convergence. However, we find that in this case convergence is
;     poor when starting from an image with zero's or very small
;     values. Using numerator == calcback makes the algorithm less
;     sensitive to the starting image.
;
;
;    CONTAMINATION: a sinogram representing the estimated
;       contamination (e.g. scatter, randoms, emission...). The
;       algorithm mentioned above is then changed to take into account
;       this contribution. The estimate is supposed to be free of
;       noise, the actual measured contamination is supposed to
;       contribute to the Poisson noise:
;
;            relaxatie   bckp{calcproj - sino * calcproj/(calcproj + contam)}
; delta mu = --------- * ----------------------------------------------------
;            reconcols   bckp{ (sino - contam)^2/sino }
;
; OUTPUTS:
;    
;    RECON: The updated reconstruction. See input parameters.
;
;    MEASBACK: When non-existing or equal to a single value, it is set to
;              the backprojection of the transmission scan. See input
;              parameters.
;
;
; MODIFICATION HISTORY:
; 	Written by:  Johan Nuyts, sep 1995
;-

pro NImltrans, recon, sino, blank, measback, firstangle, deltaangle, $
      likelihood = likelihood, calcprojmu = calcprojmu, subset = subset, $
      fwhm= fwhm, $
      relaxation = relaxation, pcoeff = pcoeff, $
      priormean = priormean, priorstd = priorstd, $
      priorweight = priorweight, priorwidth = priorwidth, $
      markovweight = markovweight, deltamarkov=deltamarkov, $
      neighborweights=neighborweights, $
      huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure, $
      fessler=fessler, contamination=contamination

  sinosize = size(sino)
  nrangles = sinosize(2)

  ; Default: 1 subset
  ;==================
  if n_elements(subset) eq 0       then subset = indgen(nrangles)
  if n_elements(priorweight) eq 0  then priorweight = 0  
  if n_elements(relaxation) eq 0   then relaxation = 1
  if n_elements(markovweight) eq 0 then markovweight = 0
  IF n_elements(fwhm) EQ 0         THEN fwhm = 0

  ; Calculate backprojection of measured sinogram.
  ; Only needed when not done before.
  ; If contamination is present, it calculates instead the
  ; backprojection of (sino - contamination)^2/sino.
  ; In both cases, measback only depends on measured data, and
  ; must not be recomputed (unless the number of subsets changes).
  ;
  ; measback = bckp(sino) or
  ;            bckp((sino - contamination)^2 / sino)
  ;==================
  if n_elements(measback) le 1 then begin
    if n_elements(contamination) le 1 then begin
      NIproj, measback, NIconvolgauss(sino, fwhm=fwhm, dim=[0]), $
              firstangle, deltaangle, nrangles, /bil, /back, $
              pcoeff = pcoeff, subset=subset
    endif else begin
      tmpsino = (sino - contamination)^2 / (sino > 1.0)
      NIproj, measback, NIconvolgauss(tmpsino, fwhm=fwhm, dim=[0]), $
              firstangle, deltaangle, nrangles, /bil, /back, $
              pcoeff = pcoeff, subset=subset
    endelse
  endif

  ; Find size of reconstruction
  ;==================
  reconsize = size(recon)
  reconcols = reconsize(1)
  
  ; project the current reconstruction and attenuate the blank
  ;==================
  if n_elements(calcprojmu) le 1 $
    then calcprojmu = 0 * sino $
    else calcprojmu(*,subset) = 0
  calcsino = 0 * calcprojmu
  NIproj, recon, calcprojmu, firstangle, deltaangle, nrangles, /bil, $
          subset=subset, pcoeff = pcoeff
  tmpsino = NIconvolgauss(exp(-calcprojmu(*,subset)), fwhm=fwhm, dim=[0])
  calcsino(*,subset) = blank(*,subset) * tmpsino
  
  ; Backproject the attenuated blank.
  ; If contamination present, backproject the difference of
  ; attenuated blank and weighted measurement
  ;
  ; calcback = bckp(calcsino) or      ; our version
  ;            bckp(calcsino - sino)  ; Fessler version
  ;            bckp(calcsino - sino * calcsino / (calcsino + contamination)
  ;==================
  if n_elements(contamination) le 1 then begin
    if keyword_set(fessler) $
      then NIproj, calcback, $
            NIconvolgauss(calcsino - sino, fwhm=fwhm, dim=[0]), $
            firstangle, deltaangle, nrangles, /bil, /back, $
            subset=subset, pcoeff = pcoeff $
      else NIproj, calcback, NIconvolgauss(calcsino, fwhm=fwhm, dim=[0]), $
            firstangle, deltaangle, nrangles, /bil, /back, $
            subset=subset, pcoeff = pcoeff
  endif else begin
    tmpsino = calcsino - sino * (calcsino / ((calcsino + contamination) > 1.))
    NIproj, calcback, NIconvolgauss(tmpsino, fwhm=fwhm, dim=[0]), $
            firstangle, deltaangle, nrangles, /bil, /back, $
            subset=subset, pcoeff = pcoeff
  endelse 

  ; Calculate numerator and denominator of the ML-expression.
  ; if no contamination:
  ;    calcback - measback             calcback
  ;    -------------------   or        --------  (= fessler)
  ;        calcback                    measback
  ;
  ;                          or
  ;
  ;   calcback
  ;   -------- (= with contamination)
  ;   measback
  ;
  ; Calculate our iterative reconstruction formula
  ;             relaxatie * (calcback - measback   + priorweight * gprior)
  ; delta mu = ------------------------------------------------------------
  ;             reconcols * noemer + relaxatie * priorweight * g2prior
  ;
  ;
  ; Note that NIproj normalizes for the number of projections that are
  ; involved in the operation (so scaling is independent of nr
  ; subsets). Consequently, we have to multiply with nrangles to
  ; obtain unscaled backprojection, which can be combined with the prior.
  ;==================
  if n_elements(contamination) le 1 and not keyword_set(fessler) then begin
      teller = (calcback - measback) * nrangles
      noemer = float(reconcols * nrangles) * calcback > 1e-15
  endif else begin
      teller = calcback * nrangles
      noemer = float(reconcols * nrangles) * measback > 1e-15
  endelse
  priorlik  = 0
  markovlik = 0
  
 ; multimodal a priori image values
 ;=================================
 if (n_elements(priormean) ne 0) and (priorweight ne 0) then begin
    if n_elements(likelihood) eq 0 $
      then NIgrad_priorvalues, gprior, g2prior, recon, priormean, priorstd, $
                               priorwidth = priorwidth $
      else NIgrad_priorvalues, gprior, g2prior, recon, priormean, priorstd, $
                               priorwidth = priorwidth, lik=priorlik
    teller = teller +              priorweight * gprior
    noemer = noemer + relaxation * priorweight * g2prior
  endif

  ; Gibbs prior
  ;============
  if (markovweight ne 0) then begin
    if n_elements(neighborweights) eq 0 then begin
      neighborweights = [[0.7, 1, 0.7], [1.0, 0, 1.0], [0.7, 1, 0.7]]
      neighborweights = neighborweights / total(neighborweights)
    endif
    if n_elements(likelihood) eq 0 $
      then gmarkov = NImarkov(recon, neighborweights, weight=markovweight, $
              huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure,$
                delta=deltamarkov, /gradient, g2prior=g2markov) $
      else gmarkov = NImarkov(recon, neighborweights, weight=markovweight, $
              huber=huber, quadratic=quadratic, geman_mcclure = geman_mcclure,$
              delta=deltamarkov, /gradient, g2prior=g2markov, lik=markovlik)
    teller = teller +              gmarkov
    noemer = noemer + relaxation * g2markov
  endif

  recon = (recon + float(relaxation) * teller / noemer) > 0.0
  ; Calculate the likelihood if requested
  ;==================
  if n_elements(likelihood) ne 0 then begin
    likelihood = total(- calcsino - sino * calcprojmu) + priorlik + markovlik
  endif  
end
