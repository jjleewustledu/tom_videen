;+
; NAME:
;    NIproj
;
; PURPOSE:
;    Calculate a projection of a reconstruction image into a sinogram, or
;    calculate a backprojection from the sinogram into the reconstruction
;    image.
;    The operation can be restricted to a subset of projections.
;
;
; CALLING SEQUENCE:
;    NIproj, image, sinogram, firstangle, deltangle
;
; INPUTS:
;    IMAGE
;      The reconstruction image.
;      Must be provided in the case of projection.
;      In the case of backprojection, it is created if it does not exist.
;      The image must be two or three dimensional. The third dimension is
;      regarded as perpendicular to the reconstruction plane.
;
;    SINOGRAM
;      The sinogram image.
;      Must be provided in the case of backprojection.
;      In the case of projection, it is created if it does not exist.
;      If both sinogram and image are provided, they must have the same number
;      of dimensions.
;
;    FIRSTANGLE
;      Angle in radians corresponding to the first projection. Set it to 
;      zero if the first projection is vertical.
;      Default: 0.
;      Ignored when PCOEFF is non-zero.
;
;    DELTAANGLE
;      Angle between two consecutive projections in radians.
;      Ignored when PCOEFF is non-zero.
;
; OPTIONAL INPUTS:
;    NRANGLES
;      Provide it if the sinogram to be calculated is not yet allocated.
;      If it is calculated, the number of angles equals the number of rows
;      (second coordinate) in the sinogram.
;      Ignored when PCOEFF is non-zero.
;	
; KEYWORD PARAMETERS:
;    SUBSET
;      Set it to an array containing the indices of the projections that must
;      be involved in the calculations.
;      Default: all projections.
;
;    BILINEAR
;      If set, bilinear interpolation is applied.
;      Otherwise, nearest neighbor interpolation is applied.
;      See RIEMANN.
;      Ignored when PCOEFF is non-zero (uses always bilinear interpolation).
;
;    CENTER
;      Set this to the center of the projection image. Different from RIEMANN,
;      the default is (number_of_projection_pixels - 1.0) / 2.0.
;      (See RIEMANN).
;      Ignored when PCOEFF is non-zero.
;
;    COR
;      Position of the center of rotation in the image.
;      Default = [nr_columns - 1., nr_rows - 1] / 2. This is different from
;      the default provided by RIEMANN.
;      (See RIEMANN).
;      Ignored when PCOEFF is non-zero.
;
;    D
;      Sampling distance. See RIEMANN
;      Ignored when PCOEFF is non-zero.
;
;    BACKPROJECT
;      If set, a backprojection is applied.
;      If not, a projection is applied.
;      See RIEMANN
;
;    NEW
;      If set, the output image is cleared before storing the result of
;      projection or backprojection. Otherwise, the result is added to the
;      contents of the output image.
;
;    PCOEFF
;      If the keyword is not set or equal to zero, (back)projections are
;      computed with the RIEMANN command.
;      If the keyword is set to the result of a call to NIkul_coeff, the
;      (back)projections are computed by calling a routine from the
;      c-library.
;
;    SPECTATTEN
;      If this keyword is not set or equal to zero, no attenuation is
;      assumed. (For PET-type attenuation, the non-attenuated projections can
;      be multiplied with the attenuation along each projection line to
;      produced attenuated projections.)
;      For spect-type attenuation, the attenuation must be included in
;      the projection calculations.  For SPECT-type attenuation, set
;      SPECTATTEN to an image containing the linear attenuation
;      coefficients per pixel. In this case, the keyword PCOEFF must
;      be set as well, since RIEMANN cannot compute attenuated
;      projections.
;
; OUTPUTS:
;    IMAGE or SINOGRAM is updated, depending on the value of the BACKPROJECTION
;    keyword.
;
; COMMON BLOCKS:
;    none
;
; SIDE EFFECTS:
;    none
;
; PROCEDURE:
;    Carries out a few initializations and calls RIEMANN to do the job.
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, K.U.Leuven, Belgium
;                       aug-1995
;-


pro NIproj, image, sinogram, firstangle, deltaangle, nrangles, $
            bilinear = bilinear, center = center, cor = cor, d = d, $
            backproject = backproject, subset = subset, new=new, $
            pcoeff = pcoeff, spectatten=spectatten, maxim=maxim

common NIkul_common, nrriemann, nrkulproj

; Default values
;---------------
if n_elements(firstangle) eq 0 then firstangle = 0
if not keyword_set(bilinear)    then bilinear = 0
if n_elements(d) eq 0           then d = 1
if not keyword_set(backproject) then backproject = 0
if n_elements(pcoeff) eq 0      then begin
  pcoeff = 0
  if keyword_set(maxim) then begin
    print, 'NIproj: maximum intensity projection requires pcoeff'
    return
  endif
endif
if pcoeff eq 0 and n_elements(spectatten) gt 1 then begin
  image = -1
  print, 'NIproj: if SPECTATTEN is set, PCOEFF must be set as well!'
  return
endif
if n_elements(nrriemann) eq 0 then nrriemann = 0L
if n_elements(nrkulproj) eq 0 then nrkulproj = 0L

; Allocate a sinogram if it does not exist already
; The size is that of the largest projection in the image
; (See IDL-help for RIEMANN)
;-------------------------------------------------
if n_elements(sinogram) le 1 then begin
  if backproject then begin
    image = 0
    print, 'NIproj: no sinogram given' + string(7b)
    return
  endif
  imsize = size(image)
  nrdet = imsize(1) / d
  if imsize(0) lt 3 then sinogram = fltarr(nrdet, nrangles) $
                    else sinogram = fltarr(nrdet, nrangles, imsize(3))
endif

; Allocate an image if it does not exist.
; The size is such that its largest projection will fit
; in the sinogram matrix.
;------------------------------------------------------
if n_elements(image) le 1 then begin
  if not backproject then begin
    sinogram = 0
    print, 'NIproj: no image given' + string(7b)
    return
  endif
  sinosize = size(sinogram)
  nrcols = sinosize(1) * d
  if sinosize(0) lt 3 then image = fltarr(nrcols, nrcols) $
                      else image = fltarr(nrcols, nrcols, sinosize(3))
endif

; Clear the images if requested
;------------------------------
if keyword_set(new) then $
  if backproject then image    = image    * 0.0 $
                 else sinogram = sinogram * 0.0

; Calculate the centers of the image.
; The defaults used by RIEMANN are not the ones we want to use:
; RIEMANN uses nrpixels/2, we want (nrpixels-1)/2.
;---------------------------------------
sinosize = size(sinogram)
imgsize  = size(image)

if sinosize(0) lt 3 then nplanes = 1 $
                    else nplanes = sinosize(3)

if n_elements(center) eq 0 then center = 0.5 * (sinosize(1) - 1.0)
if n_elements(cor) eq 0    then cor    = 0.5 * (imgsize(1:2) - 1.0)

; Calculate nrangles. If the caller supplies both the sinogram and the
; number of angles, check whether this is ok.
;---------------------------------------------------------------------
sinosize = size(sinogram)
if n_elements(nrangles) eq 0 then nrangles = sinosize(2)
if nrangles ne sinosize(2) then begin
  if nrangles gt 1 and sinosize(0) gt 1 then begin
    print, 'NIproj: sinogram size and nrangles not identical' + string(7b)
    return
  endif
endif
if n_elements(subset) eq 0      then subset = indgen(nrangles)
if n_elements(deltaangle) eq 0 then deltaangle = !pi/float(nrangles)

for plane = 0, nplanes-1 do begin
  ; 3D projection: copy the sinogram, because riemann may wish to
  ; add to its contents, and cannot add directly to sinogram(*,*,plane).
  ; Copy also image, otherwise an implicit copy is forced for every subset.
  ; Afterwards, the tmpsino is put back in the original sinogram in the
  ; case of projection, and the tmpimg in the case of backprojection.
  ;
  ; In the case of backprojection, the newly calculated values are divided
  ; by the number of projections.
  ;==============
  tmpsino  = sinogram(*,*,plane)
  tmpimg   = image(*,*,plane)
  if n_elements(spectatten) gt 1 then tmpatten = spectatten(*,*,plane)

  tmpsize = size(tmpsino)
  if tmpsize(0) eq 1 then tmpsino = reform(tmpsino,tmpsize(1),1, /overwrite)

  if pcoeff eq 0 then begin
    for j = 0, n_elements(subset)-1 do begin
      i = subset(j)
      riemann, tmpsino, tmpimg, firstangle + i * deltaangle, row = i, $
               backproject = backproject, bilinear = bilinear, d = d, $
               cor = cor, center = center
      nrriemann = nrriemann + 1
    endfor
  endif else begin
    for j = 0, n_elements(subset)-1 do begin
      i = subset(j)
      if n_elements(spectatten) le 1 $
        then NIkul_proj, tmpimg, tmpsino, i, pcoeff, backproject=backproject, $
                         maxim=maxim $
        else NIkul_spectproj, tmpimg, tmpsino, tmpatten, i, pcoeff, $
             backproject=backproject
      nrkulproj = nrkulproj + 1
    endfor
  endelse
  if backproject eq 0 then sinogram(*,*,plane) = tmpsino $
                      else image(*,*,plane)    = tmpimg / n_elements(subset)
  endfor
end



