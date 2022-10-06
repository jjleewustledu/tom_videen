;       $Revision:   1.1  $ $Date:   10/15/93 18:35:04  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RSH_ATN7
;
; PURPOSE:
; 	Read the ecat7 attenuation subheader at the offset from the 
; 	open file.			
;
; CATEGORY:	
; 	CTI Matrix utility.		
;
; CALLING SEQUENCE:		
; 	result = rsh_atn7 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_atn7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_ATN7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_atn7
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN

      point_lun, unit, offset(i)

      in=intarr(7) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.data_type = in(0)
      shead.num_dimensions = in(1)
      shead.atten_type = in(2)
      shead.num_r_elements = in(3)
      shead.num_angles = in(4)
      shead.num_z_elements = in(5)
      shead.ring_difference = in(6)

      in=fltarr(14) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.x_resolution = in(0)
      shead.y_resolution = in(1)
      shead.z_resolution = in(2)
      shead.w_resolution = in(3)
      shead.scale_factor = in(4)
      shead.x_offset = in(5)
      shead.y_offset = in(6)
      shead.x_radius = in(7)
      shead.y_radius = in(8)
      shead.tilt_angle = in(9)
      shead.atten_coeff = in(10)
      shead.atten_min = in(11)
      shead.atten_max = in(12)
      shead.skull_thickness = in(13)

      in=1 & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.num_add_coeff = in

      in=fltarr(9) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.add_atten_coeff = in(0:7)
      shead.edge_finding_threshold = in(8)

      sh = [sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_ATN7", UNIT=unit
   RETURN, !ECAT.ERROR
END
