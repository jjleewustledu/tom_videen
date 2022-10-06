;       $Revision:   1.2  $ $Date:   12/07/93 14:07:54  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RSUBHEADERS6
;
; PURPOSE:
;		Read the appropriate type subheaders in the open file
;		unit at the given offsets into the parameter sh.			
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:		
;		result = rsubheaders6 (ftype,unit,offsets,sh,scale)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

function rsubheaders6,ftype,unit,offsets,sh,scales

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"RSUBHEADERS6",UNIT=unit
	   return,!ECAT.ERROR
	endif

	case !ECAT.file_types(ftype) of

	   "IMG" : $
	      begin
		stat = rsh_img6(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
 		if (sh(0).ecat_calibration_fctr ne 0.0) then begin
	          scales = sh(0:n_elements(sh)-1).quant_scale * $
			sh(0:n_elements(sh)-1).ecat_calibration_fctr
		endif else begin
                  scales = sh(0:n_elements(sh)-1).quant_scale
		endelse
	      end

	   "SCN" : $
	      begin
		stat = rsh_scn6(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "NRM" : $
	      begin
		stat = rsh_nrm6(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "ATN" : $
	      begin
		stat = rsh_atn6(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "PM" : $
	      begin
		stat = rsh_pm6(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   else  : begin
	      set_ecat_error,15,"RSUBHEADERS6",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	return,!ECAT.OK

io_error : set_ecat_error,1,"RSUBHEADERS6",UNIT=unit
           return,!ECAT.ERROR

end
