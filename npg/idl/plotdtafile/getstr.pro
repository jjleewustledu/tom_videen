;Id$
;
;+
; NAME:
;   GETSTR
;
; PURPOSE:
;   Get a lowercase string which is an element of a set of strings
;
; CATEGORY:
;	I/O
;
; CALLING SEQUENCE:
;   getstr, q, s0, set
;
; INPUTS:
;	q:		prompt
;   set:	allowed strings (array of lowercase strings)
;
; OUTPUTS:
;   s0:		string
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Nov. 1995
;-

pro getstr, q, s0, allowed

    on_ioerror, l1
	s1 = ''
	q = q + ' (' 
	num = (size(allowed))(1) - 1
	if (num gt 0) then $
		for n=0,num-1 do $
			q = q + allowed(n) + ','
l1:	q1 = q + allowed(num) + ') <' + s0 + '> '
	read, prompt=q1, s1
	if (strlen(s1) eq 0) then s1 = s0
	s0 = strlowcase (s1)
	indx = where (allowed eq s0, count)
	if (count eq 0) then begin
		print,'Error: Enter a value from: ', allowed
		goto, l1
	endif	
end
