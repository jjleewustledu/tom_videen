; $Id: dat2dta.pro,v 1.18 2007/12/24 17:01:46 tom Exp $
;
;+
; NAME:
;   DAT2DTA
;
; PURPOSE:
;   Create a bloodcurve file for metproc.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   dat2dta pdif.dat scaler output.dta
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Feb 2010.
;-

pro dat2dta

;	Initializations

	ncurves=1L
	scantype = 2
	start = 0L
	oxycont=0.
	hematocrit=0.
	header = ''
	str=''
	root=''
	scanid = ''
	read, prompt='Subject ID: ',root
	dtafile = root + '.dta'

	scannum=1
	scantype = 2
	length = 40
	scanid = ho1
		
    openr,testlu,str,/get_lun, err=err
	if (err NE 0) then begin
		print,'Cannot open ',str
		goto, newscanid
	endif else begin
		close,testlu
		free_lun,testlu
	endelse

	read_dta, crv, oxycont, hematocrit, header, bloodfile

	t2 = max(indx)
	t1 = t2-1
	time1 = hcrv(0,t1)
	val1 = hcrv(1,t1)
	time2 = hcrv(0,t2)
	val2 = hcrv(1,t2)

;	Write DTA file

	if (scannum eq 1) then begin
		openw,lu,dtafile,/get_lun
		printf,lu,format='("@ @@ ",A)',header
		printf,lu, 1
	endif
	shiftcrv(1,*) = shiftcrv(1,*)*bmax
	write_dta, shiftcrv, scantype, scanid, bmax, oxycont, hematocrit, start, length, lu
	close,lu
	free_lun,lu
	exit
end
