;+
; NAME:
;    NIcbp
;
; PURPOSE:
;    Reconstruction from projections using convolution backprojection.
;    Convolution in the space domain avoids artifacts due to the FFT in
;    classical filtered backprojection.
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    NIcbp, recon, sinogram
;
; INPUTS:
;    SINOGRAM: a 2D or 3D array. If 3D, it is considered as a set of 2D
;              sinograms.
;
;
; KEYWORD PARAMETERS:
;    STARTANGLE:
;      Angle in radians corresponding to the first projection.
;      Vertical projection: 0.  Positive is clockwise.
;      Default: 0.
;      Ignored when PCOEFF is non-zero.
;
;    DELTAANGLE
;      Angle in radians between two consecutive projections.
;      Designed for 180 degrees reconstruction. Add opposite projections
;      (after mirroring) in the case of 360 degrees acquisition.
;      Alternatively, apply directly to 360 degrees acquistion, at the cost
;      of double computation time.
;
;   PCOEFF
;      When set to 0 or not provided, the RIEMANN command is used to
;      compute the backprojection.
;      When set to the value returned by NIkul_coeff, the routine NIkul_proj
;      is used to compute the backprojection. This routines contains a
;      call to the backprojector in our reconstruction C-library.
;      
;   SUBSET
;      When set, only the projection lines belonging to this subset are
;      used. A subset is defined as an array of row indices for the sinogram.
;
; OUTPUTS:
;    RECON
;      Resulting reconstruction image.
;
;
; RESTRICTIONS:
;    Only the spatial domain implementation of the ramp filter is supported.
;    Projections can be smoothed prior to reconstruction, or the resulting
;    reconstruction can be smoothed to implement smoothing reconstruction
;    filters.
;
; PROCEDURE:
;    Convolves the sinogram rows with spatial domain implementation of the
;    ramp filter, and calls NIproj for the backprojection. Convolution
;    is carried out using a convolution mask of maximum length
;    (2 x nrcolums + 1) to obtain optimal performance.
;
; SEE ALSO:
;    NIfbp:  filtered backprojection via FFT.
;    NIproj: projector/backprojector
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 29-april-1997
;-

PRO NIcbp, recon, sinogram, startangle = startangle, deltaangle=deltaangle, $
           pcoeff=pcoeff, subset=subset, halfmask=halfmask


;Default values
;-------------
sinosize = size(sinogram)
nrdet    = sinosize(1)
nrangles = sinosize(2)
if sinosize(0) eq 3 then nrplanes = sinosize(3) $
                    else nrplanes = 1

if n_elements(deltaangle)  eq 0 then deltaangle  = !pi / nrangles
if n_elements(startangle)  eq 0 then startangle  = 0.
if n_elements(subset)      eq 0 then subset      = indgen(nrangles)
IF n_elements(halfmask)    EQ 0 THEN halfmask =  nrdet
n_subset = n_elements(subset)

; Allocate a reconstruction image if not available
;-------------------------------------------------
if n_elements(recon) le 1 then $
  recon = fltarr(nrdet, nrdet, nrplanes)
tmprecon = fltarr(nrdet,nrdet)

mask = fltarr(2 * halfmask + 1)
xas  = findgen(2 * halfmask + 1) - halfmask

daarodd  = where(xas ne fix(xas/2)*2)
mask(daarodd) = - 1. / !pi^2 / (xas(daarodd))^2
mask(where(xas eq 0)) = 1./4.

;first = round((float(nrdet) + 1.) / 2.)
first = round((halfmask*2.0 +1. - nrdet)/2.)

for plane = 0, nrplanes-1 do begin
  sino = sinogram(*,*,plane)
  for row = 0, nrangles-1 do begin
     IF (halfmask*2+1) GT nrdet THEN BEGIN
        therow = convol(mask, rotate(sino(*,row),2), /edge_tr)
        sino(*,row) = therow(first:first+nrdet-1)
     ENDIF ELSE BEGIN
        sino(*,row) = convol(sino(*,row), mask, /edge_tr)
     ENDELSE
  endfor
  NIproj, tmprecon, sino, startangle, deltaangle, /new, /bilinear, $
        /backproject, pcoeff=pcoeff, subset=subset
  recon(*,*,plane) = tmprecon * !pi
endfor
end

