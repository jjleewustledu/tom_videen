;Id$
;
;+
; NAME:
;   GETNUM
;
; PURPOSE:
;   Get an number
;
; CATEGORY:
;	I/O
;
; CALLING SEQUENCE:
;   getnum, q, n, nmin, nmax
;
; INPUTS:
;	q:			prompt
;   nmin:		minimum allowed valued for n
;   nmax:		maximum allowed valued for n
;
; OUTPUTS:
;   n:		value
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Nov. 1995
;-

pro getnum, q, n, nmin, nmax

	valid = 0
    on_ioerror, l1
	str = strtrim(string(n),2)
	q = q + ' <' + str + '> '
l0: read, prompt=q, str
	if strlen(str) ne 0 then begin
		case (size(n))(1) of
			1:	n = byte(str)
			2:	n = fix(str)
			3:	n = long(str)
			4:	n = float(str)
			5:	n = double(str)
			else:	begin
				print,'Error: getnum read numeric values only'
				stop
			end
		endcase
	endif
    valid = 1
l1: if not valid then begin
		print,'Error: Enter a number between ',nmin, ' and ', nmax
    	goto, l0
    endif
	if (n lt nmin) or (n gt nmax) then begin
		valid = 0
		print,'Error: Enter a number between ',nmin, ' and ', nmax
		goto, l0
	endif	
end
