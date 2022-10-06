;       $Revision:   1.1  $ $Date:   10/15/93 18:47:20  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WSH_SCN6
;
; PURPOSE:	
; 	Write the ecat6 scan subheader.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = wsh_scn6 (unit,offset,sh)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  wsh_scn6, unit, offset, sh
   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "WSH_SCN6", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   FOR i=0, n_elements(offset)-1 DO BEGIN
      wb = 0L
      point_lun, unit, offset(i)
      out = bytarr(126) & writeu, unit, out & incr, wb, 126
      out = sh(i).data_type
      byteorder, out & writeu, unit, out & incr, wb, 2
      out = bytarr(4) & writeu, unit, out & incr, wb, 4
      out = [sh(i).dimension_1, sh(i).dimension_2, $
             sh(i).smoothing, sh(i).processing_code]
      byteorder, out & writeu, unit, out & incr, wb, 8
      out = bytarr(6) & writeu, unit, out & incr, wb, 6
      out = stovf(sh(i).sample_distance)
      writeu, unit, out & incr, wb, 4
      out = bytarr(16) & writeu, unit, out & incr, wb, 16
      out = stovf(sh(i).isotope_halflife)
      writeu, unit, out & incr, wb, 4
      out = sh(i).frame_duration_sec
      byteorder, out & writeu, unit, out & incr, wb, 2
      out = [sh(i).gate_duration, sh(i).r_wave_offset]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 8
      out = bytarr(2) & writeu, unit, out & incr, wb, 2
      out = stovf(sh(i).scale_factor)
      writeu, unit, out & incr, wb, 4
      out = intarr(5)
      out(3:4) = [sh(i).scan_min, sh(i).scan_max]
      byteorder, out & writeu, unit, out & incr, wb, 10
      out = [sh(i).prompts, sh(i).delayed, $
             sh(i).multiples, sh(i).net_trues]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 16
      out = bytarr(104) & writeu, unit, out & incr, wb, 104
      out = lonarr(34)
      out(0:15) = sh(i).cor_singles
      out(16:31) = sh(i).uncor_singles
      out(32) = sh(i).tot_avg_cor
      out(33) = sh(i).tot_avg_uncor
      out = stovf(out) & writeu, unit, out & incr, wb, 136
      out = [sh(i).total_coin_rate, sh(i).frame_start_time, $
             sh(i).frame_duration]
      byteorder, out, /lswap & writeu, unit, out & incr, wb, 12
      out = stovf(sh(i).loss_correction_fctr)
      writeu, unit, out & incr, wb, 4
      out = bytarr(44) & writeu, unit, out & incr, wb, 44
   ENDFOR

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WSH_SCN6", UNIT=unit
   RETURN, !ECAT.ERROR

END
