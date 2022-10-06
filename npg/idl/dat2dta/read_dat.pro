; $Id: read_dat.pro,v 1.3 1995/11/09 20:15:52 tom Exp $
;
;+
; NAME:
;   READ_BLOODCRV
;
; PURPOSE:
;   Read dat file (VOI rgt values from vstat)
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   read_dat, curve, filename
;
; INPUTS:
;   filename.
;
; OUTPUTS:
;   curve.
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, 2010.
;-

pro read_dat, curve, filename

	openr,lu,filename,/get_lun
	str = ''
	readf,lu,str
	readf,lu,str
	readf,lu,str
	readf,lu,str
	readf,lu,str
	readf,lu,str
	curve = fltarr(2,num)
	readf,lu,curve
	close,lu
	free_lun,lu
end
