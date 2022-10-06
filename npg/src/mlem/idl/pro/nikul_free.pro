;+
; NAME:
;    NIkul_free
;
; PURPOSE:
;    Deallocate memory allocated by NIkul_coeff.
;
; CATEGORY:
;    Reconstruction
;
; CALLING SEQUENCE:
;    NIkul_free, pcoeff
;
; INPUTS:
;    PCOEFF: the pointer to the memory allocated by the external c-code,
;            and returned by NIkul_coeff.
;            PCOEFF is set to zero.
;
; COMMON BLOCKS:
;    NC_lib_common: required to find the reconstruction library.
;
; RESTRICTIONS:
;    Calling the routine with an invalid pointer will cause a core dump.
;    Calling with a pointer equal to zero will cause an error message.
;
; PROCEDURE:
;    Calls the deallocation routine in the reconstruction c-library.
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 10 apr 1996
;-

pro NIkul_free, prcf

common NC_lib_common

if n_elements(prcf) eq 0 then begin
  print,'NIkul_free: pointer does not exist'
  return
endif

if prcf eq 0 then begin
  print,"NIkul_free: pointer is already 0"
  return
endif
prcf = call_external(NC_recon, 'NIkul_free', prcf)

end