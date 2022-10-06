;       $Revision:   1.1  $ $Date:   10/15/93 18:46:58  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WSH_NRM6
;
; PURPOSE:	
; 	Write the ECAT6 normalization subheader.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:	
; 	result = wsh_nrm6 (unit,offset,sh)	
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_nrm6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_NRM6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i)
      out = bytarr(126) & writeu, unit, out & incr, wb, 126
      out = sh(i).data_type
      byteorder, out & writeu, unit, out & incr, wb, 2
      out = bytarr(4) & writeu, unit, out & incr, wb, 4
      out = [sh(i).dimension_1, sh(i).dimension_2]
      byteorder, out & writeu, unit, out & incr, wb, 4
      out = bytarr(46) & writeu, unit, out & incr, wb, 46
      out = stovf(sh(i).scale_factor)
      writeu, unit, out & incr, wb, 4
      out = [sh(i).norm_hour, sh(i).norm_minute, $
             sh(i).norm_second, sh(i).norm_day, $
             sh(i).norm_month, sh(i).norm_year]
      byteorder, out & writeu, unit, out & incr, wb, 12
      out = stovf(sh(i).fov_source_width)
      writeu, unit, out & incr, wb, 4
      out = bytarr(310) & writeu, unit, out & incr, wb, 310
   ENDFOR
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_NRM6", UNIT=unit
   RETURN, !ECAT.ERROR
END
