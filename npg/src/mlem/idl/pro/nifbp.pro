;+
; NAME:
;   NIfbp
;
; PURPOSE:
;   Computes the reconstruction from a sinogram using filtered backprojection.
;
; CALLING SEQUENCE:
;   NIfbp, recon, sinogram
;
; INPUTS:
;
;   SINOGRAM
;      the sinogram image to be reconstructed.
;      The dimensions are:
;           fltarr(nrcols, nrrows)
;      or, for multiple slices:
;           fltarr(nrcols, nrrows, nrplanes)
; 
; KEYWORD PARAMETERS:
;
;   FILTER
;      The low pass filter to be applied (in combination with the ramp
;      filter) for noise suppression. See NIfilterfunction for the
;      possible values.  Default is 'rect', a rectangular filter, with
;      transition from 1 to 0 at the frequency CUTOFF_FREQ.
;   
;   CUTOFF_FREQ
;      The cut-off frequency of the filter, expressed in cycles/pixel:
;      the sample frequency is 1, the Nyquist is 0.5. The
;      interpretation depends on the filter.  See NIfilterfunction for
;      further comments.
;     
;   POWER
;      See NIfilterfunction.
;
;   STARTANGLE
;      the angle of the first projection in radians.
;
;   DELTAANGLE
;      angle, in radials, between two consecutive projections.
;      Filtered backprojection is designed for reconstruction of 180
;      degrees projection data. The routine accepts 360 degrees data,
;      but it is more efficient to add opposite projections (mirroring
;      one of them) and apply a 180 degrees reconstruction.  FBP of
;      360 degrees data is mathematically identical to FBP of the
;      added (not averaged!)  opposite projections. Divide by 2 if you
;      want the effect of averaging opposite projections.  Default is
;      pi / number-of-projections.
;
;   ZEROPADDING
;      Number of zeros that must be appended to both sides of the
;      sinogram, to improve the Fourier-decomposition  of the sinogram.
;      Default=0
;
;   PCOEFF
;      When set to 0 or not provided, the RIEMANN command is used to
;      compute the backprojection.
;      When set to the value returned by NIkul_coeff, the routine NIkul_proj
;      is used to compute the backprojection. This routines contains a
;      call to the backprojector in our C-library.
;
;   SUBSET
;      When set, only the projection lines belonging to this subset are
;      used. A subset is defined as an array of row indices for the sinogram.
;
;   SHOWIMG
;      When set, each plane is displayed as soon as it has been reconstructed.
;
; OUTPUTS
;
;   IMAGE 
;      The image that is reconstructed from the sinogram
;
; MODIFICATION HISTORY:
;    written by:   Swana Vanacker, Stefaan Vandenberghe dec 1995
;    Modification:
;       jan 1996, Johan Nuyts: several bugs fixed
;       apr 1996, Johan Nuyts: addition of keyword PCOEFF.
;-


PRO NIfbp, recon, sinogram, zeropadding = zeropadding, filter = filter, $
           power = power, cutoff_freq = cutoff_freq, startangle = startangle, $
           deltaangle = deltaangle, pcoeff=pcoeff, subset = subset, $
           showimg = showimg, demo=demo

;Default values
;-------------
sinosize = size(sinogram)
nrdet    = sinosize(1)
nrangles = sinosize(2)
if sinosize(0) eq 3 then nrplanes = sinosize(3) $
                    else nrplanes = 1

pi = acos(-1)
if n_elements(deltaangle)  eq 0 then deltaangle  = pi / nrangles
if n_elements(startangle)  eq 0 then startangle  = 0.
if n_elements(power)       eq 0 then power       = 10
if n_elements(cutoff_freq) eq 0 then cutoff_freq = 0.5
if n_elements(zeropadding) eq 0 then zeropadding = 20
if n_elements(filter)      eq 0 then filter      = 'rect'
if n_elements(subset)      eq 0 then subset      = indgen(nrangles)
n_subset = n_elements(subset)

; Allocate a reconstruction image if not available
;-------------------------------------------------
if n_elements(recon) le 1 then $
  recon = fltarr(nrdet, nrdet, nrplanes)
tmprecon = fltarr(nrdet,nrdet)

;definition of frequencyvector as an array of the frequencies where the filter
;must be calculated
;----------------------------------------------------------------------------
newnrdet         = nrdet + 2*zeropadding
filtervector = NIfilterfunction(filter, newnrdet, cutoff_freq = cutoff_freq, $
                                power = power, /ramp)

for plane = 0, nrplanes-1 do begin

  ;A new sinogram is created by adding zeros at both sides of the old sinogram.
  ;----------------------------------------------------------------------------
  newsinogram      = fltarr(newnrdet,nrangles)
  newsinogram(zeropadding:zeropadding+nrdet-1,*) $
     = sinogram (0:nrdet - 1, *, plane)

  ;1D.Fouriertransformation for every angle (row) of the sinogram.
  ;---------------------------------------------------------------
  fouriersin = complexarr(newnrdet,n_subset)
  for i=0, n_subset -1 do $
    fouriersin(*,i)= FFT(newsinogram(*,subset(i)))

  ;Filtering of the Fouriertransformation of the sinogram
  ;------------------------------------------------------
  for i = 0,n_subset-1 do $
    fouriersin(*,i) = fouriersin(*,i) * filtervector
  
  ;1D inverse fouriertransformation of the sinogram for each angle
  ;---------------------------------------------------------------
  filteredsinogram = fltarr(newnrdet,nrangles)

  for i = 0,n_subset-1 do $
    filteredsinogram(*,subset(i)) = FFT(fouriersin(*,i), /inverse)

  ;Reduction of the sinogram to its original size
  ;----------------------------------------------
  filteredsinogram = filteredsinogram(zeropadding : nrdet+zeropadding -1,*)


  if not keyword_set(demo) then begin
    ;Backprojection of the filtered sinogram
    ;---------------------------------------
    NIproj, tmprecon, filteredsinogram, startangle, deltaangle, /new, $
            /bilinear, /backproject, pcoeff=pcoeff, subset=subset

    ; Multiply with deltaangle, which is the equivalent of d theta in the
    ; analogue integral expression.
    ;---------------------------------------
    IF keyword_set(showimg) THEN NIimage, tmprecon
  endif else begin
    tmprecon(*) = 0
    teller = 0.0
    for i = 0, n_elements(subset)-1 do begin
      NIproj, tmprecon,  filteredsinogram, startangle, deltaangle, $
            /bilinear, /backproject, pcoeff=pcoeff, subset=subset(i)
      teller = teller + 1.0 / float(n_elements(subset))
      if teller gt 1/40. then begin
        NIimage,tmprecon, /noerase
        teller = 0.0
      endif
    endfor
  endelse
  recon(*,*,plane) = tmprecon * !pi
endfor

end



