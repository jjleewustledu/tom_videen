;       $Revision:   1.1  $ $Date:   10/15/93 18:35:42  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RSH_PM7
;
; PURPOSE:	
; 	Read the ecat7 polar map subheader at the offset from the
; 	open file unit.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rsh_pm7 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; rsh_pm7
;       rsh_pm reads THE polar map SUBHEADER AT OFFSET FROM THE OPEN
;       FILE, UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND
;       !ECAT.ERROR OTHERWISE.

FUNCTION  rsh_pm7, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_PM7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_pm7
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)
      in=intarr(3) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.data_type = in(0)
      shead.type = in(1)
      shead.num_rings = in(2)

      in=intarr(32) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.sect_per_ring = in

      in=fltarr(32) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.ring_pos = in

      in=intarr(42) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.ring_angle = in(0:31)
      shead.start_angle = in(32)
      shead.long_axis_left = in(33:35)
      shead.long_axis_right = in(36:38)
      shead.position_data = in(39)
      shead.image_min = in(40)
      shead.image_max = in(41)

      in=fltarr(2) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.scale_factor = in(0)
      shead.pixel_size = in(1)

      in=lonarr(2) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.frame_duration = in(0)
      shead.frame_start_time = in(1)

      in=intarr(2) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in
      shead.proc_code = in(0)
      shead.quant_units = in(1)

      in=bytarr(40) & readu, unit, in
      shead.annotation = in

      in=lonarr(3) & readu, unit, in & IF (!LITTLE_ENDIAN) THEN bytorder, in, /lswap
      shead.gate_duration = in(0)
      shead.r_wave_offset = in(1)
      shead.num_acpt_beats = in(2)

      in=bytarr(50) & readu, unit, in
      shead.pm_protocol = in(0:19)
      shead.dbase_name = in(20:49)

      sh=[sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_PM7", UNIT=unit
   RETURN, !ECAT.ERROR
END
