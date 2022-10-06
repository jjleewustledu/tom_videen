; $Id: read_dta.pro,v 1.1 1995/11/08 22:09:38 tom Exp $
;
;+
; NAME:
;   READ_DTA
;
; PURPOSE:
;   Read single blood curve from dta file.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   read_dta, curve, scantype, scanid, bmax, oxycont, hematocrit, start, length, filename
;
; INPUTS:
;   lu:			logical unit for open dta file
;
; OUTPUTS:
;   curve:		time-activity curve with time = curve(0,*) and activity = curve(1,*)
;	scantype:	type of scan (1-6)
;	scanid:		scan ID
;	bmax:		maximum count rate
;	oxycont:	oxygen content (ml/ml)
;	hematocrit:	hematocrit (percent)
;	start:		start time (sec)
;	length:		scan length (sec)
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Mar 2007.
;-

pro read_dta, curve, scantype, scanid, bmax, oxycont, hematocrit, start, length, lu

	readf,lu, format='(I1,1X,A)', scantype, scanid
	readf,lu, start, length
	readf,lu, bmax
	readf,lu, oxycont, hematocrit
	readf,lu, num
	time = 0.0
	cnts = 0.0
	for i=0,num-1 do begin
		readf, lu, format='(2(F10.1))', time, cnts
		if (i EQ 0) then begin
			curve = [time, cnts]
		endif else begin
			curve = [[curve],[time, cnts]]
		endelse
	endfor
end
