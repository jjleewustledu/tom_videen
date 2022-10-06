;       $Revision:   1.1  $ $Date:   10/15/93 18:35:26  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RSH_NRM6
;
; PURPOSE:			
;		Read the normalization subheader at the offset from the open
;		file.
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:		
;		result = rsh_nrm6 (unit,offset,sh)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


function   rsh_nrm6,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_NRM6",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_nrm6
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)+126L
	   in=intarr(5) & readu,unit,in & IF (NOT !LITTLE_ENDIAN) THEN byteorder,in
	   shead.data_type = in(0)
	   shead.dimension_1 = in(3)
	   shead.dimension_2 = in(4)
	   in=intarr(23) & readu,unit,in
	   in=1L & readu,unit,in & in=vtosf(in)
	   shead.scale_factor = in
	   in=intarr(6) & readu,unit,in & IF (NOT !LITTLE_ENDIAN) THEN byteorder,in
	   shead.norm_hour = in(0)
	   shead.norm_minute = in(1)
	   shead.norm_second = in(2)
	   shead.norm_day = in(3)
	   shead.norm_month = in(4)
	   shead.norm_year = in(5)
	   in=lonarr(2) & readu,unit,in & in=vtosf(in)
	   shead.fov_source_width = in(0)
	   shead.ecat_calib_factor = in(1)
	   sh = [sh, shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_NRM6",UNIT=unit
           return,!ECAT.ERROR
end
