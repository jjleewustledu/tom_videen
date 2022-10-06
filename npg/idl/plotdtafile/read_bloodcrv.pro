; $Id: read_bloodcrv.pro,v 1.3 1995/11/09 20:15:52 tom Exp $
;
;+
; NAME:
;   READ_BLOODCRV
;
; PURPOSE:
;   Read blood curve file.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   read_bloodcrv, curve, oxycont, hematocrit, header, filename
;
; INPUTS:
;   None.
;
; OUTPUTS:
;   curve:		time-activity curve with time = curve(0,*) and activity = curve(1,*)
;	oxycont:	oxygen content (ml/ml)
;	hematocrit:	hematocrit (percent)
;	header:		file header
;	bloodfile:	file name
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Nov. 1995
;-

pro read_bloodcrv, curve, oxycont, hematocrit, header, bloodfile

	openr,lu,bloodfile,/get_lun
	hour = 0L
	min = 0L
	sec = 0L
	readf,lu,hour,min,sec,num,oxycont,hematocrit,header,format='(I2,X,I2,X,I2,I6,F8.4,F6.1,2X,A)'
	curve = fltarr(2,num)
	readf,lu,curve
	close,lu
	free_lun,lu
end
