pro set_handle_value, ptr, value, NO_COPY = NO_COPY
;+
; NAME: set_handle_value
;
; PURPOSE: a more sensible way to set handle values
;
; CATEGORY: Handles
;
; CALLING SEQUENCE: set_handle_value, ptr, value
; 
; INPUTS:
;    ptr : the handle
;  value : the value
;
; KEYWORD PARAMETERS:
;  NO_COPY : See help for HANDLE_VALUE
;
; OUTPUTS: none
;
; MODIFICATION HISTORY:
;     23 Dec 96 Initial Coding  PMW
;-

if not(keyword_set(no_copy)) then no_copy = 0

if handle_info(ptr, /valid) then $
  handle_value, ptr, value, /set, no_copy = no_copy $
else begin
    error_string = strtrim(ptr,2) + ' is an invalid pointer.'
    message, error_string
endelse
end

