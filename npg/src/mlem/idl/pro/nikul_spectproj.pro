;+
; NAME:
;    NIkul_spectproj
;
; PURPOSE:
;    Compute projection or backprojection using coefficients pre-computed
;    by NIkul_coeff.
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    NIkul_spectproj, image, sinogram, attenimg, projnum, prcf
;
; INPUTS:
;    IMAGE: the reconstruction image. When keyword BACKPROJECT is set, the
;           backprojection is ADDED to this image.
;
;    SINOGRAM: When keyword BACKPROJECT is zero or not set, the sinogram
;              receives the projection values.
;
;    ATTENIMG: Attenuation image. Pixel values contain linear attenuation
;              coefficient per pixel.
;
;    PROJNUM: the number of the current projection (row number in the 
;             sinogram).
;
;    PRCF:    pointer obtained from NIkul_coeff.
;
;    
; KEYWORD PARAMETERS:
;    BACKPROJECT: when set, a backprojection from SINOGRAM into IMAGE is
;                computed. When not set or zero, a projection from IMAGE into
;                SINOGRAM is computed.
;
; OUTPUTS:
;    The output is IMAGE or SINOGRAM, depending on the keyword BACKPROJECT.
;
;
; COMMON BLOCKS:
;    NC_lib_common, required to find the reconstruction library.
;
;
; PROCEDURE:
;    Calls a module from the reconstruction library.
;
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 10 april 1996
;-

pro NIkul_spectproj, image, sinogram, attenimg, projnum, prcf, $
                     backproject=backproject

common NC_lib_common

  if n_elements(image) le 1 then image = attenimg * 0

  projnum = fix(projnum)

  if keyword_set(backproject) $
    then dummy = call_external(NC_recon, 'NIkul_abproj', image, $
                               sinogram, attenimg, projnum, prcf) $
    else dummy = call_external(NC_recon, 'NIkul_aproj', image, $
                               sinogram, attenimg, projnum, prcf)

end
