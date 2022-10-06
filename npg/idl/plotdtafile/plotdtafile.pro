;+
; NAME:
;   PLOTDTAFILE
;
; PURPOSE:
;   Plot a bloodcurve file 
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   plotdtafile
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Mar 2007
;-

pro plotdtafile

;	Initializations

	loadct,39
	ncurves=0L
	scantype = 0
	start = 0L
	shift = 0L
	shiftwater = 0L
	oxycont=0.
	hematocrit=0.
	header = ''
	str=''
	pid=''
	scanid = ''
    pid = getenv('PID')
	dtafile = pid + '.dta'
	openr,lu,dtafile,/get_lun
	readf,lu,str
	readf,lu,ncurves
	print,ncurves,' Curves'
	if (ncurves eq 0) then stop

	scrnsize = get_screen_size()
	xloc = scrnsize[0]/2
	IF (xloc GT 1270) THEN xloc = 1270
	window,xpos=xloc

	for scannum=1, ncurves do begin
		; read DTA
		read_dta, curve, scantype, scanid, bmax, oxycont, hematocrit, start, length, lu
		print, scantype, ' ', scanid, bmax, oxycont, hematocrit, start, length
		;goto, nextcrv

		; read DCV
		bloodfile = pid + scanid + '.dcv'
        openr,testlu,bloodfile,/get_lun, err=err
		if (err NE 0) then begin
			print,'Cannot open ',bloodfile
			exit
		endif else begin
			close,testlu
			free_lun,testlu
		endelse

		read_bloodcrv, bcrv, oxycont, hematocrit, header, bloodfile
		zero_time, bcrv, 'Artery', 0, 1
		bmax = max(bcrv(1,*))
		bcrv(1,*) = bcrv(1,*)/bmax

		; read R or PLT
		headfile = pid + scanid
		read_headcrv, hcrv, headfile
		indx = where (hcrv(1,*) gt 0)
		t2 = max(indx)
		t1 = t2-1
		nframes = strtrim(string(fix(hcrv[2,t2])),2)
		print, nframes,'-Frame Study'
		firstfrm = fix(hcrv(2,0))
		zerotime = hcrv(0,0)

		zero_time, hcrv, 'Head', time0, frmnum
		hmax = max(hcrv(1,*))
		hcrv(1,*) = hcrv(1,*)/hmax

		shiftcrv = bcrv(0:1,*)
		t1 = 0.
		t2 = max(shiftcrv(0,*))
		plot_bloodcrv, shiftcrv, hcrv, t1, t2

		indx = where (hcrv(1,*) gt 0)
		t2 = max(indx)
		t1 = t2-1

;	Align the two curves

		t1 = 0.
		t2 = max(shiftcrv(0,*))
		print,' '
		print,'Shift the blood curve to align it with the head curve'
		a = ''
		if (scantype eq 1) then shift = shiftwater
		if (scantype eq 3) then shift = 0
		range = ['z','u','s','p','c']
next:	getstr, 'Zoom, Unzoom, Shift, Print, Continue', a, range
		case 1 of
			(a eq 'z'):	begin
				getnum, 'Zoom from',t1, t1, t2
				getnum, '       to',t2, t1, t2
				plot_bloodcrv, shiftcrv, hcrv, t1, t2
				a = 's'
				goto, next
			end
			(a eq 'u'):	begin
				t1 = 0.
				t2 = max(shiftcrv(0,*))
				plot_bloodcrv, shiftcrv, hcrv, t1, t2
				a = 'c'
				goto, next
			end
			(a eq 's'):	begin
				shift0 = 0
nextshift:		getnum, 'Shift blood curve',shift,-300, 300
				shiftcrv(0,*) = bcrv(0,*) + shift
				plot_bloodcrv, shiftcrv, hcrv, t1, t2
				if (shift ne shift0) then begin
					shift0 = shift
					goto, nextshift
				endif
				a = 'c'
				goto, next
			end
			(a eq 'p'):	begin
				print_bloodcrv, shiftcrv, hcrv, t1, t2, headfile, start, length, shift
				a = 'c'
				goto, next
			end
			(a eq 'c'):
			else:	begin
				print,'Enter z, u, s, p, or c'
				goto, next
			end
		endcase
		if (scantype eq 2) then shiftwater = shift
		shiftcrv(1,*) = shiftcrv(1,*)*bmax
nextcrv: print,'---'
	endfor
	close,lu
	free_lun,lu
	exit
end
