;*************************************
function encode_paradigm,paradigm,tdim
;*************************************

; Encode paradigm into ASCII string as defined by Avi.

last_state = paradigm(0)
count = 0
code = ''
for i=0,tdim-1 do begin
    if(paradigm(i) eq last_state) then begin
	count = count + 1
    endif else begin
	if(count eq 1) then begin
	    case last_state of
	        -1: code = code + '-'
	         0: code = code + 'x'
	         1: code = code + '+'
	        else: code = ''
	    endcase
	endif else begin
	    case last_state of
	        -1: code = code + strcompress(string(count,'-'),/REMOVE_ALL)
	         0: code = code + strcompress(string(count,'x'),/REMOVE_ALL)
	         1: code = code + strcompress(string(count,'+'),/REMOVE_ALL)
	        else: code = ''
	    endcase
	endelse
        count = 1
        last_state = paradigm(i)
    endelse
endfor

if(count eq 1) then begin
    case last_state of
        -1: code = code + '-'
         0: code = code + 'x'
         1: code = code + '+'
        else: code = ''
    endcase
endif else begin
    case last_state of
        -1: code = code + strcompress(string(count,'-'),/REMOVE_ALL)
         0: code = code + strcompress(string(count,'x'),/REMOVE_ALL)
         1: code = code + strcompress(string(count,'+'),/REMOVE_ALL)
        else: code = ''
    endcase
endelse

print,code
return,code
end
