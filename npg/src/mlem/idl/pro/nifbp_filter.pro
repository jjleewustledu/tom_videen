function NIfbp_filter, sinogram, zeropadding = zeropadding, $
             filter = filter, power = power, cutoff_freq = cutoff_freq, $
             subset = subset
             

  sinosize = size(sinogram)
  nrdet    = sinosize(1)
  nrangles = sinosize(2)
  if n_elements(subset) eq 0 then subset = indgen(nrangles)
  n_subset = n_elements(subset)

  if n_elements(power)       eq 0 then power       = 10
  if n_elements(cutoff_freq) eq 0 then cutoff_freq = 0.5
  if n_elements(zeropadding) eq 0 then zeropadding = 20
  if n_elements(filter)      eq 0 then filter      = 'rect'

  ;A new sinogram  is created by adding zeros at both sides of the old sinogram.
  ;-----------------------------------------------------------------------------
  newnrdet         = nrdet + 2*zeropadding
  newsinogram      = fltarr(newnrdet,nrangles)
  newsinogram(zeropadding:zeropadding+nrdet-1, *) $
     = sinogram (0:nrdet - 1, *)

  ;1D.Fouriertransformation for every angle (row) of the sinogram.
  ;---------------------------------------------------------------
  fouriersin = complexarr(newnrdet,nrangles)
  for i=0, n_subset -1 do $
    fouriersin(*,i)= FFT(newsinogram(*,subset(i)))

  ;Filtering of the Fouriertransformation of the sinogram
  ;------------------------------------------------------
  filtervector = NIfilterfunction(filter, newnrdet, cutoff_freq = cutoff_freq, $
                                power = power, /ramp)
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

  return, filteredsinogram
end
