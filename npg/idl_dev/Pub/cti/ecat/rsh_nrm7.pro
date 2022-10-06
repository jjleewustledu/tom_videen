;       $Revision:   1.1  $ $Date:   10/15/93 18:35:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RSH_NRM7	
;
; PURPOSE:
; 	Read the ecat7 normalization subheader at the offset from
; 	the open file.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rsh_nrm7 (unit,offset,sh)
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_nrm7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_NRM", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_nrm7
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN

      point_lun, unit, offset(i)

      in=intarr(6) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.data_type = in(0)
      shead.num_dimensions = in(1)
      shead.num_r_elements = in(2)
      shead.num_angles = in(3)
      shead.num_z_elements = in(4)
      shead.ring_difference = in(5)

      in=fltarr(5) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.scale_factor = in(0)
      shead.norm_min = in(1)
      shead.norm_max = in(2)
      shead.fov_source_width = in(3)
      shead.norm_quality_factor = in(4)

      in=1 & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.norm_qual_fctr_code = in

      sh = [sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_NRM7", UNIT=unit
   RETURN, !ECAT.ERROR
END
