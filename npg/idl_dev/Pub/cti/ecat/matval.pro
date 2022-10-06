;       $Revision:   1.1  $ $Date:   10/15/93 18:18:06  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:	
;		MATVAL	
;
; PURPOSE:
;		Returns an array with the frame, plane, gate, data, and
;		bed corresponding to the matrix number given by matnum.			
;
; CATEGORY:	
;		CIT Matrix utility		
;
; CALLING SEQUENCE:
;		result = matval(matnum)		
;
; INPUTS:
;		matnum - matrix number			
;
; OUTPUTS:	
;		result - 5 element vector containing:
;				result(0) = frame
;				result(1) = plane
;				result(2) = gate
;				result(3) = data
;				result(4) = bed
;
; HISTORY NOTES:		
;-


function   matval,matnum	

	on_error,2
	if(n_params() NE 1) then begin
	   ecat_set_error,2,"MATVAL"
	   return,!ECAT.ERROR
	endif
	hi_plane  = (ishft(long(matnum),-1)  AND '300'XL)
	lo_plane  = (ishft(long(matnum),-16) AND 'FF'XL)
	hi_data	  = (ishft(long(matnum),-9)  AND '4'XL)
	lo_data   = (ishft(long(matnum),-30) AND '3'XL)
	mval0 = (matnum AND '1FF'XL)
	mval1 = (lo_plane OR hi_plane)
	mval2 = (ishft(long(matnum),-24) AND '3F'XL)
	mval3 = (lo_data OR hi_data)
	mval4 = (ishft(long(matnum),-12) AND 'F'XL)
	mval  = [mval0,mval1,mval2,mval3, mval4]
	return,mval
end 
