;*************************************************
function convert_paradigm,mri_paradigm_format,tdim
;*************************************************

paradigm = fltarr(tdim+1)
str = string(!BIN,"/expandf ",mri_paradigm_format)
case n_elements(str) of
    1: cmd = str
    2: cmd = str(0) + str(1)
    3: cmd = str(0) + str(1) + str(2)
    4: cmd = str + str(1) + str(2) + str(3)
    else: cmd = str + str(1) + str(2) + str(3) + str(4)
endcase
spawn,cmd,str
case n_elements(str) of
    1: paradigm_string = str
    2: paradigm_string = str(0) + str(1)
    3: paradigm_string = str(0) + str(1) + str(2)
    4: paradigm_string = str + str(1) + str(2) + str(3)
    else: paradigm_string = str + str(1) + str(2) + str(3) + str(4)
endcase
for i=1,tdim do begin
    code = strmid(paradigm_string,i-1,1) ; Extract character
    if(n_elements(code) gt 1) then begin
	cd = code(0)
	for j=1,n_elements(code)-1 do $
	    cd = cd + code(j)
	code = cd
      endif
    case code(0) of
	'-': paradigm(i-1) = -1
	'+': paradigm(i-1) =  1
	'x': paradigm(i-1) = 0
	else: begin
;	    stat=widget_message('Header contains invalid paradigm code.',/ERROR)
	    paradigm(*) = 1.
	    paradigm(0) = -2
	    i = tdim + 1
	  end
    endcase
endfor

return,paradigm
end
