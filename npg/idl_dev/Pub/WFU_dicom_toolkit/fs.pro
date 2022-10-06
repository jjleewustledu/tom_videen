; routine to display path for a given procedure
; or function name. It handles weird fomatting
; problems that the HELP function has.


pro fs,str

	help,/source,output=ohelp

	; here we fix problem of line splitting
	nel=n_elements(ohelp)
	ohelp2=strarr(nel)
	k=0
	for i=0,nel-1 do begin
		ln=str_sep(ohelp[i],'  /')
		if(n_elements(ln) GT 1) then begin
			ohelp2[k]=ohelp[i]
			k=k+1
		endif else begin

			ohelp2[k]=ohelp[i]

			pron=str_sep(ohelp[i],'/')
			if(n_elements(pron) GT 1) then $
				print,'Something wrong with',ohelp[i]

			; check whether should merge it with next string
			if(i+1 LT nel) then begin
				next_ln=str_sep(ohelp[i+1],'/')
				if(n_elements(next_ln) GT 1) then begin
					ohelp2[k]=ohelp2[k]+ohelp[i+1]
					i=i+1
				endif
			endif

			k=k+1

		endelse

	endfor

	ohelp=ohelp2

	pos=strpos(strupcase(ohelp),strupcase(str))
	print,' '
	ind=where(pos ge 0) & s=size(ind)
	if(s[0] GT 0) then begin
		print,'Procedures and Functions found:'
		print,format='(a)',ohelp[where(pos ge 0)]
	endif else $
		print,'No matches found'
end
