;       $Revision:   1.1  $ $Date:   10/15/93 18:47:04  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_NRM7	
;
; PURPOSE:	
; 	Write the ecat7 normalization subheader.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = wsh_nrm7 (unit,offset,sh)	
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_nrm7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_NRM7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i) ; set the initial file pointer

      out = [sh(i).data_type, sh(i).num_dimensions, sh(i).num_r_elements, $
             sh(i).num_angles, sh(i).num_z_elements, $
             sh(i).ring_difference]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 12

      out = [sh(i).scale_factor, sh(i).norm_min, $
             sh(i).norm_max, sh(i).fov_source_width, $
             sh(i).norm_quality_factor]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 20

      out = [sh(i).norm_qual_fctr_code]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 2

      out = bytarr(478) & writeu, unit, out & incr, wb, 478

   ENDFOR
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_NRM7", UNIT=unit
   RETURN, !ECAT.ERROR
END
