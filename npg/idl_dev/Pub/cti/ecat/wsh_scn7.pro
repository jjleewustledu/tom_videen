;       $Revision:   1.1  $ $Date:   10/15/93 18:47:24  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	WSH_SCN7	
;
; PURPOSE:
; 	Write the ecat7 scan subheader.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = wsh_scn7 (unit,offset,sh)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_scn7, unit, offset, sh
   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_SCN7", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i) ; set the initial file pointer

      out = [sh(i).data_type, sh(i).num_dimensions, sh(i).num_r_elements, $
             sh(i).num_angles, sh(i).corrections_applied, $
             sh(i).num_z_elements, sh(i).ring_difference]

      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 14

      out = [sh(i).x_resolution, sh(i).y_resolution, $
             sh(i).z_resolution, sh(i).w_resolution]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 16

      out = bytarr(12) & writeu, unit, out & incr, wb, 12 ; reserved FOR gating

      out = [sh(i).gate_duration, sh(i).r_wave_offset, $
             sh(i).num_acpt_beats]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 12

      out = [sh(i).scale_factor]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 4

      out = [sh(i).scan_min, sh(i).scan_max]
      IF (!LITTLE_ENDIAN) THEN byteorder, out
      writeu, unit, out & incr, wb, 4

      out = [sh(i).prompts, sh(i).delayed, sh(i).multiples, $
             sh(i).net_trues]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 16

      out = fltarr(34)
      out(0:15) = sh(i).cor_singles
      out(16:31) = sh(i).uncor_singles
      out(32) = sh(i).tot_avg_cor
      out(33) = sh(i).tot_avg_uncor
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 136

      out = [sh(i).total_coin_rate, sh(i).frame_start_time, $
             sh(i).frame_duration]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 12

      out = [sh(i).dt_corr_fctr]
      IF (!LITTLE_ENDIAN) THEN byteorder, out, /lswap
      writeu, unit, out & incr, wb, 4

      out = intarr(8)           ;out(0:7) = sh(i).plysical_planes
      writeu, unit, out & incr, wb, 16
      out = bytarr(266) & writeu, unit, out & incr, wb, 266
   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_SCN7", UNIT=unit
   RETURN, !ECAT.ERROR

END
