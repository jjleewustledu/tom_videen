;       $Revision:   1.1  $ $Date:   10/15/93 18:35:46  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RSH_SCN6
;
; PURPOSE:
; 	Read the sinogram subheader at the offset from the open
; 	file unit.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rsh_scn6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  rsh_scn6, unit, offset, sh

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "RSH_SCN6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   stat = fstat(unit)
   IF (stat.open EQ 0) THEN BEGIN
      set_ecat_error, 6, "RSH_SCN6"
      RETURN, !ECAT.ERROR
   ENDIF

   shead = !ECAT.sh_scn6
   sh = [shead]
   FOR i=0, n_elements(offset)-1 DO BEGIN
      point_lun, unit, offset(i)+126L
      in=intarr(10) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.data_type = in(0)
      shead.dimension_1 = in(3)
      shead.dimension_2 = in(4)
      shead.smoothing = in(5)
      shead.processing_code = in(6)
      in=1L & readu, unit, in & in=vtosf(in)
      shead.sample_distance = in
      in=intarr(8) & readu, unit, in
      in=1L & readu, unit, in & in=vtosf(in)
      shead.isotope_halflife = in
      in=1 & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.frame_duration_sec = in
      in=lonarr(2) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in, /lswap
      shead.gate_duration = in(0)
      shead.r_wave_offset = in(1)
      in=1 & readu, unit, in
      in=1L & readu, unit, in & in=vtosf(in)
      shead.scale_factor = in
      in=intarr(5) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.scan_min = in(3)
      shead.scan_max = in(4)
      in=lonarr(4) & readu, unit, in & IF (NOT !LITTLE_ENDIAN) THEN byteorder, in, /lswap
      shead.prompts = in(0)
      shead.delayed = in(1)
      shead.multiples = in(2)
      shead.net_trues = in(3)
      in=intarr(52) & readu, unit, in
      in=lonarr(38) & readu, unit, in
      shead.cor_singles = vtosf(in(0:15))
      shead.uncor_singles = vtosf(in(16:31))
      shead.tot_avg_cor = vtosf(in(32))
      shead.tot_avg_uncor = vtosf(in(33))
      hold = in(34:36)
      IF (NOT !LITTLE_ENDIAN) THEN byteorder, hold, /lswap
      shead.total_coin_rate = hold(0)
      shead.frame_start_time = hold(1)
      shead.frame_duration = hold(2)
      shead.loss_correction_fctr = vtosf(in(37))
      in=lonarr(8) & readu, unit, in &IF (NOT !LITTLE_ENDIAN) THEN byteorder, in
      shead.phy_planes = in
      sh = [sh, shead]
   ENDFOR

   sh = sh(1:*)
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RSH_SCN6", UNIT=unit
   RETURN, !ECAT.ERROR
END
