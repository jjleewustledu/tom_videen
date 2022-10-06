FUNCTION NIfbp_prefilter, projection, zeropadding, $
             filter = filter, power = power, cutoff_freq = cutoff_freq

imgsize =  size(projection)
nrcol =  imgsize(1)
nrrow =  imgsize(2)

if n_elements(power)       eq 0 then power       = 10
if n_elements(cutoff_freq) eq 0 then cutoff_freq = 0.5
if n_elements(zeropadding) eq 0 then zeropadding = 20
if n_elements(filter)      eq 0 then filter      = 'rect'

newnrcol = nrcol + 2*zeropadding
newnrrow = nrrow + 2*zeropadding

newnrdet =  newnrcol >  newnrrow

newimg =  fltarr(newnrdet, newnrdet)

coloffset = fix((newnrdet - nrcol)/2)
rowoffset = fix((newnrdet - nrrow)/2)

newimg(coloffset:(coloffset+nrcol-1), rowoffset:(rowoffset+nrrow-1)) $
 = projection

filtervector = NIfilterfunction(filter, newnrdet, cutoff_freq = cutoff_freq, $
                              power = power, /twoD)
newimg(*) =  FFT(FFT(newimg) * filtervector, /inverse)
img =  newimg(coloffset:(coloffset+nrcol-1), rowoffset:(rowoffset+nrrow-1))
return, img
END
