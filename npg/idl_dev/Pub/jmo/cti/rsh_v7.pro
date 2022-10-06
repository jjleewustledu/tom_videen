;       $Revision:   1.1  $ $Date:   10/15/93 18:35:22  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RSH_V7
;
; PURPOSE:	
;		Read the image subheader at the offset from the open file.		
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:		
;		result = rsh_v7 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


function   rsh_v7,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_V7",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_img7
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin

	   point_lun,unit,offset(i)

	   in=intarr(5) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in
	   shead.data_type = in(0)
	   shead.num_dimensions = in(1)
	   shead.x_dimension = in(2)
	   shead.y_dimension = in(3)
	   shead.z_dimension = in(4)

	   in=fltarr(5) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.z_offset = in(0)
	   shead.x_offset = in(1)
	   shead.y_offset = in(2)
	   shead.recon_zoom = in(3)
	   shead.scale_factor = in(4)

	   in=intarr(2) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in
	   shead.image_min = in(0)
	   shead.image_max = in(1)

	   in=fltarr(3) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.x_pixel_size = in(0)
	   shead.y_pixel_size = in(1)
	   shead.z_pixel_size = in(2)

	   in=lonarr(2) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.frame_duration = in(0)
	   shead.frame_start_time = in(1)

	   in=1 & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in
	   shead.filter_code=in

	   in=fltarr(7) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.x_res = in(0)
	   shead.y_res = in(1)
	   shead.z_res = in(2)
	   shead.x_rot_angle = in(3)
	   shead.y_rot_angle = in(4)
	   shead.z_rot_angle = in(5)
	   shead.dec_corr_fctr = in(6)

	   in=lonarr(4) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.corr_applied = in(0)
	   shead.gate_duration = in(1)
	   shead.r_wave_offset = in(2)
	   shead.num_acpt_beats = in(3)

	   in=fltarr(3) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.fil_cutoff_freq = in(0)
	   shead.fil_dc_comp = in(1)
	   shead.fil_rs = in(2)

	   in=1 & readu,unit,in& if (!LITTLE_ENDIAN) then byteorder,in
	   shead.fil_order=in

	   in=fltarr(2) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.fil_sf = in(0)
	   shead.fil_scat_slope = in(1)

	   in=bytarr(40) & readu,unit,in 
	   shead.annotation = in

	   in=fltarr(9) & readu,unit,in & if (!LITTLE_ENDIAN) then byteorder,in,/lswap
	   shead.loc.da_x_rot_angle = in(0)
	   shead.loc.da_y_rot_angle = in(1)
	   shead.loc.da_z_rot_angle = in(2)
	   shead.loc.da_x_trans = in(3)
	   shead.loc.da_y_trans = in(4)
	   shead.loc.da_z_trans = in(5)
	   shead.loc.da_x_scale_f = in(6)
	   shead.loc.da_y_scale_f = in(7)
	   shead.loc.da_z_scale_f = in(8)
	
	   sh=[sh,shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_V7",UNIT=unit
           return,!ECAT.ERROR
end
;
