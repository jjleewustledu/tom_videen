;       $Revision:   1.1  $ $Date:   10/15/93 18:36:02  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RSUBHEADERS7
;
; PURPOSE:	
;		Read the appropriate type subheaders in the open file unit
;		at the given offsets into the parameter sh.		
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:
;		result = rsubheaders7 (ftype,unit,offsets,sh,scales)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


function rsubheaders7,ftype,unit,offsets,sh,scales

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"RSUBHEADERS7",UNIT=unit
	   return,!ECAT.ERROR
	endif

	case !ECAT.file_types(ftype) of

	   "IMG" : $
	      begin
		stat = rsh_img7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "SCN" : $
	      begin
		stat = rsh_scn7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "NRM" : $
	      begin
		stat = rsh_nrm7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "ATN" : $
	      begin
		stat = rsh_atn7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end
           "PM" : $
              begin
                stat = rsh_pm7(unit,offsets,sh)
                if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
                scales = sh(0:n_elements(sh)-1).scale_factor
              end
	   "V" : $
	      begin
		stat = rsh_V7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end
	   "S" : $
	      begin
		stat = rsh_S7(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end
	   "xxx" : begin
		print,"*** File type not supported."
 	      end
	   else  : begin
	      set_ecat_error,15,"RSUBHEADERS7",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	return,!ECAT.OK

io_error : set_ecat_error,1,"RSUBHEADERS7",UNIT=unit
           return,!ECAT.ERROR

end
