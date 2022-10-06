function get_handle_value, ptr, NO_COPY = NO_COPY
;+
; NAME: get_handle_value
;
; PURPOSE: A more sensible way to get handle values
;
; CATEGORY: Handles
;
; CALLING SEQUENCE: value = get_handle_value(ptr[, /NO_COPY])
; 
; INPUTS: A handle
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS: 
;    NO_COPY: See help for HANDLE_VALUE
;
; OUTPUTS: If ptr is valid then the ptr's value is returned.
;
; MODIFICATION HISTORY:
;          23 Dec 96 Initial writting PMW
;-
;--- A More sensible way to get a handle value

ON_ERROR, 2

if not(keyword_set(no_copy)) then no_copy = 0
if handle_info(ptr, /valid) then begin
    handle_value, ptr, value, no_copy = no_copy
    return, value
endif
error_string = strtrim(ptr,2) + ' is an invalid pointer.'
message, error_string
end
