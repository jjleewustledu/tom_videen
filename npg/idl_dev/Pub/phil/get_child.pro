function get_child, ptr, n, howMany, parent = parent
;+
; NAME: get_child.pro
;
; PURPOSE: Recursive routine that gets the nth child / sibling of a handle.
;
; CATEGORY: Handle Routines
;
; CALLING SEQUENCE:
;       child = get_child(ptr, n[, howMany])
; 
; INPUTS:
;     ptr : a pointer
;       n : Which child handle to return
; howMany : Which call to get_child is this?
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS: none
;
; OUTPUTS: The handle to the nth child / sibling of the ptr. 0 is
;          returned if ptr is not a valid handle.
;
; OPTIONAL OUTPUTS: none.
;
; COMMON BLOCKS: none.
;
; SIDE EFFECTS: none.
;
; RESTRICTIONS: none.
;
; PROCEDURE: Straightforward
;
; EXAMPLE:
;    IDL> h1 = handle_create()
;    IDL> h2 = handle_create(h1)
;    IDL> h3 = handle_create(h1)
;    IDL> h4 = handle_create(h1)
;    IDL> h5 = handle_create(h2)
;    IDL> h6 = handle_create(h2)
;    IDL> h7 = handle_create(h2)
;    IDL> h8 = handle_create(h2)
;    IDL> print,h1,h2,h3,h4,h5,h6,h7,h8
;               9          10          11          12          13          14
;              15          16
;    IDL> print,get_child(h1,2)
;              11
;    IDL> print,get_child(h1,3)
;              12
;    IDL> print,get_child(h2,3)
;               0
;    IDL> print,get_child(h2,1)
;              11
;    IDL> print,get_child(h2,1,/parent)
;              13
;    IDL> print,get_child(h2,4,/parent)
;              16
;    IDL> print,get_child(h2,4)
;               0
; MODIFICATION HISTORY:
;     13 Dec 96 Version 1.0  PMW
;     16 Jan 97 Version 1.1 
;          Added keyword parent to search get a child's children.
;          Added default for howMany
;-

if n_elements(howMany) eq 0 then howMany = 1

;--- Make sure that it's valid
if handle_info(ptr, /valid) then begin
    pflag = (keyword_set(parent) or (handle_info(ptr, /parent) eq 0))
    if pflag then begin
        nextChild = handle_info(ptr, /first_child)
    endif else begin
        nextChild = handle_info(ptr, /sibling)
    endelse

    if n ne howMany then begin
        nextChild = get_child(nextChild, n, howMany+1)
    endif
    return, nextChild
endif else return, 0L
end
