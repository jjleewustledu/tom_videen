function handleDefined, handle
;+
; NAME: handleDefined.pro
;
; PURPOSE: Does the handle have a defined value
;
; CATEGORY: handle manipulation
;
; CALLING SEQUENCE: result = handleDefined(handle)
; 
; INPUTS:
;     handle : the handle to test
;
; OUTPUTS:
;     1 == value defined
;     0 == value undefined.  This can result from the following.
;     Handle is undefined, handle is not a valid handle, value is undefined.
;
; MODIFICATION HISTORY:
;      4 Jan 96 Initial Coding PMW
;-
if n_elements(handle) eq 0 then return, 0

if handle_info(handle, /valid) then begin
    handle_value, handle, value, /no_copy
    if n_elements(value) eq 0 then return, 0
    
    handle_value, handle, value, /set, /no_copy
    return, 1
endif else return, 0
end
