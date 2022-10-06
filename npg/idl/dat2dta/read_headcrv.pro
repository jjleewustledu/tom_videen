;Id$
;
;+
; NAME:
;   READ_HEADCRV
;
; PURPOSE:
;   Read headcurve file.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   read_headcrv, curve, rootname
;
; INPUTS:
;   rootname:	filename root
;
; OUTPUTS:
;   curve:		time-activity curve with time = curve(0,*) and activity = curve(1,*)
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Nov. 1995
;   Modified: TOV May 1996 to read 4 column .plt files
;   Modified: TOV May 1996 to read distinguish 7 column .r files  (beta 7.0)
;		from 8 column .r files (7.0A)
;   Modified: TOV Jul 1998 to read frame number from 961 .r files
;-

pro read_headcrv, curve, rootname

	str = ""
	str1 = ""
	str2 = ""
	exists = 0
	on_ioerror, try1
	headfile = rootname + '.plt'
	openr,lu,headfile,/get_lun
	exists = 1
try1:	if not exists then begin
		on_ioerror, try2
		headfile = rootname + '.r'
		openr,lu,headfile,/get_lun
		exists = 1
try2:		if not exists then begin
			print,'Error: Cannot open headcurve file ', headfile
			stop
		endif
	endif

;   Get number of lines in headfile

	command = 'wc ' + headfile
	spawn, command, result
	reads, result, num
	if num eq 0 then begin
    	print, headfile, ' is empty'
	endif

	readf,lu,str
	reads,str,str1,str2,format='(A4,42X,A7)'
	case str1 of
    	'Acqu': begin
        	pet = '953'
        	num = num-3
        	headcrv = lonarr(2,num)
        	readf,lu,str
        	readf,lu,str,format='(14X,A9)'
			if (str eq '  Prompts') then begin
        		readf,lu,headcrv,format='(63X,I12,38X,I12)'
        		indx = where(headcrv(1,*) eq min(headcrv(1,*)))
        		init = indx(0)
        		num = num - init
        		curve = fltarr(3,num)
				tmpdat = fltarr(1,num)
				tmpdat[0,*] = 2
        		curve = [0.001*headcrv(1,init:*), headcrv(0,init:*), tmpdat]
			endif else if (str eq 'Net Trues') then begin
        		readf,lu,headcrv,format='(11X,I12,10X,I12)'
        		indx = where(headcrv(1,*) eq min(headcrv(1,*)))
        		init = indx(0)
        		num = num - init
        		curve = fltarr(3,num)
				tmpdat = fltarr(1,num)
        		curve = [0.001*headcrv(1,init:*), headcrv(0,init:*), tmpdat]
				for i=1,num-1 do begin
					curve(1,i) = curve(1,i) / (curve(0,i) - curve(0,i-1))
				endfor
			endif
    	end
    	'time': begin
        	pet = '961'
        	num = num-1
        	curve = fltarr(3,num)
			if (str2 eq 'singles') then begin
        		headcrv = lonarr(8,num)
			endif else begin
        		headcrv = lonarr(7,num)
			endelse
        	readf,lu,headcrv
        	curve = [0.001*headcrv(0,*), headcrv(3,*), headcrv(6,*)]
    	end
    	else: begin
        	print,headfile,' is not a headcurve'
    	end
	endcase
	close,lu
	free_lun,lu
end
