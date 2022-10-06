;       $Revision:   1.1  $ $Date:   10/15/93 18:13:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	DSCALE	
;
; PURPOSE:
;	DSCALE computes the scale factor necessary for sinogram and image data 
;	to be in the range -32768 to 32767.			
;
; CATEGORY:	
;	CTI Matrix utility.		
;
; CALLING SEQUENCE:		
;	Scale_Factor = DSCALE (Data)
;
; INPUTS:			
;	Data - data to be scaled.
;
; KEYWORD PARAMETERS:	
;	None.	
;
; OUTPUTS:
;	Data - scaled data.
;	Scale_Factor - scale factor computed. -1 is returned if an
;                      error was detected.
;	
;-

FUNCTION dscale, data

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "DSCALE"
      RETURN, -1.0
   ENDIF
   s = size(data)
   IF (s(0) EQ 2) THEN zdim = 1 ELSE zdim = s(3)
   sf = fltarr(zdim)
   FOR i = 1, zdim DO BEGIN
      dmin = 0.0
      dmax = abs(max(data(*, *, i-1), min=dmin))
      dmin = abs(dmin)
      svec  = [dmin, dmax, (1.0e-7)]
      abmax = max(svec)
      sf(i-1) = abmax/32767.0
      data(*, *, i-1) = data(*, *, i-1)*(32767.0/abmax)
   ENDFOR
   data = fix(data)
   RETURN, sf

END
