; $Id: write_dta.pro,v 1.1 1995/11/08 22:09:38 tom Exp $
;
;+
; NAME:
;   WRITE_DTA
;
; PURPOSE:
;   Append to blood curve file.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   write_dta, curve, scantype, scanid, bmax, oxycont, hematocrit, start, length, filename
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
;   Written by: Tom Videen, Nov. 1995
;-

pro write_dta, curve, scantype, scanid, bmax, oxycont, hematocrit, start, length, lu

	printf,lu, format='(I1,1X,A)', scantype, scanid
	printf,lu, start, length
	printf,lu, bmax
	printf,lu, oxycont, hematocrit
	indx = where(curve(0,*) gt 0.)
	n = size(indx)
	num = n(1)
	printf, lu, num
	for i=0,num-1 do begin
		printf, lu, format='(2(F10.1))', curve(0:1,indx(i))
	endfor
;	printf, lu, format='(I,1000(/2(F10.1)))', num, curve(0:1,indx)
end
