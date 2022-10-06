FUNCTION NIchang, sino, nriter= nriter, spectatten= spectatten, $
                  zeropadding = zeropadding, $
                  filter = filter, power = power, cutoff_freq = cutoff_freq, $
                  startangle = startangle, deltaangle = deltaangle, $
                  pcoeff=pcoeff, show= show


 IF n_elements(nriter) EQ 0 THEN nriter = 1

 sinosize = size(sino)
 nrdet = sinosize(1)
 nrproj = sinosize(2)
 IF sinosize(0) EQ 3 THEN nrplanes = sinosize(3) $
                     ELSE nrplanes = 1
 changmap = spectatten * 0

 ; Compute the Chang postcorrection matrix, by attenuated backprojection
 ; of a sinogram of pixelvalues set to 1, and apply 1/x.
 ;=========
 NIproj, changmap, fltarr(nrdet, nrproj, nrplanes)+1.0, startangle, $
         deltaangle, spectatten= specatten, pcoeff=pcoeff, /back

 changmap =  1./ (changmap > 0.01)

 img =   fltarr(nrdet, nrdet, nrplanes)
 recon = img
 calcsino = sino * 0

 FOR iter= 0, nriter - 1 DO BEGIN
   corrsino = sino - calcsino
   NIfbp, img, corrsino, zeropadding = zeropadding, $
          filter = filter, power = power, cutoff_freq = cutoff_freq, $
          startangle = startangle, deltaangle = deltaangle, $
          pcoeff=pcoeff
   img = img * changmap
   recon = recon + img
   IF keyword_set(show) THEN NIimage, recon, /global
   IF iter LT (nriter - 1) THEN BEGIN
     NIproj, recon, calcsino, startangle, deltaangle, spectatten=spectatten, $
          pcoeff=pcoeff, /new
   ENDIF
 ENDFOR

 return, recon

END



