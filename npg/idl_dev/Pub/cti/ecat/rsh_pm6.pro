;       $Revision:   1.1  $ $Date:   10/15/93 18:35:36  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RSH_PM6	
;
; PURPOSE:
; 	Read the polar map subheader at the offset from the open
; 	file unit.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rsh_pm6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_pm6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_PM6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_pm6
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)
      in=1 & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.type = in
      readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.num_rings = in 

      in=intarr(32) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.sect_per_ring = in

      in=lonarr(32) & readu, unit, in & in = vtosf(in)
      shead.ring_pos = in

      in=intarr(42) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.ring_angle = in(0:31)
      shead.start_angle = in(32)
      shead.long_axis_left = in(33:35)
      shead.long_axis_right = in(36:38)
      shead.position_data = in(39)
      shead.image_min = in(40)
      shead.image_max = in(41)

      in=lonarr(5) & readu, unit, in & in = vtosf(in)
      shead.scale_factor = in(0)
      shead.x_origin = in(1)
      shead.y_origin = in(2)
      shead.rotation = in(3)
      shead.pixel_size = in(4)

      in=lonarr(2) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in, /lswap
      shead.frame_duration = in(0)
      shead.frame_start_time = in(1)
      
      in=intarr(2) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.proc_code = in(0)
      shead.quant_units = in(1)

      in=bytarr(40) & readu, unit, in & in=string(in)
      shead.annotation = in 
      
      sh=[sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_PM6", UNIT=unit
   RETURN, !ECAT.ERROR
END
