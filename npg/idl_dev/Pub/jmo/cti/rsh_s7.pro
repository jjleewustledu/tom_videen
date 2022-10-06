;       $Revision:   1.1  $ $Date:   10/15/93 18:35:52  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RSH_S7
;
; PURPOSE:	
;		Read the ecat7 sinogram subheader at the offset from the
;		open file unit.		
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:		
;		result = rsh_scn7 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


function   rsh_S7,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_SCN7",UNIT=unit
	   return,!ECAT.ERROR
	endif

	stat = fstat(unit)
	if(stat.open EQ 0) then begin
           set_ecat_error,6,"RSH_SCN7"
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_scn7
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)

	   in=intarr(72) & readu,unit,in & if (!LITTLE_ENDIAN) then bytorder,in
	   shead.data_type = in(0)
	   shead.num_dimensions = in(1)
	   shead.num_r_elements = in(2)
	   shead.num_angles = in(3)
	   shead.corrections_applied = in(4)
	   shead.ring_difference = in(69)
	   for i=0,shead.ring_difference-1 do $
	       shead.num_z_elements = shead.num_z_elements + in(5+i)
	   shead.axial_compression = in(71)

	   in=fltarr(4) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.x_resolution = in(0)
	   shead.y_resolution = in(1)
	   shead.z_resolution = in(2)
	   shead.w_resolution = in(3)

	   ; this is fill. Reserved for gating
	   in=intarr(6) & readu,unit,in & if (!LITTLE_ENDIAN) then bytorder,in

	   in=lonarr(3) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.gate_duration = in(0)
	   shead.r_wave_offset = in(1)
	   shead.num_acpt_beats = in(2)

	   in=fltarr(1) & readu,unit,in & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.scale_factor = in(0)

	   in=intarr(2) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in
	   shead.scan_min = in(0)
	   shead.scan_max = in(1)

	   in=lonarr(4) & readu,unit,in & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.prompts = in(0)
	   shead.delayed = in(1)
	   shead.multiples = in(2)
	   shead.net_trues = in(3)

	   in=fltarr(2) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.cor_singles = in(0)
	   shead.uncor_singles = in(1)

	   in=lonarr(3) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.total_coin_rate = in(0)
	   shead.frame_start_time = in(1)
	   shead.frame_duration = in(2)

	   in=fltarr(1) & readu,unit,in  & if (!LITTLE_ENDIAN) then bytorder,in,/lswap
	   shead.dt_corr_fctr = in(0)

	   in=lonarr(8) & readu,unit,in
	   shead.phy_planes = in

	   sh = [sh,shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_SCN7",UNIT=unit
           return,!ECAT.ERROR
end
