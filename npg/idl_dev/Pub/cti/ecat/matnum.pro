;       $Revision:   1.1  $ $Date:   10/15/93 18:18:02  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	MATNUM	
;
; PURPOSE:
;	Returns a matrix number for frame, plane, gate, data, bed.			
;
; CATEGORY:	
;	CTI Matrix utility		
;
; CALLING SEQUENCE:		
;	result = matnum(frame,plane,gate,data,bed)
;
; INPUTS:	
;	frame - frame value
;	plane - plane value
;	gate - gate value
;	data - data value
;	bed - bed value		
;
; OUTPUTS:	
;	result - matrix number created from the frame, plane, gate, data
;		and bed parameters.		
;
; HISTORY NOTES:		
;-

FUNCTION   matnum, frame, plane, gate, data, bed
   
   on_error, 2
   IF (n_params() NE 5) THEN BEGIN
      set_ecat_error, 2, "MATNUM"
      RETURN, !ECAT.ERROR
   ENDIF

   hi_plane = (long(plane) AND '300'XL)
   lo_plane = (long(plane) AND 'FF'XL)
   hi_data = (long(data) AND '4'XL)
   lo_data = (long(data) AND '3'XL)

   mat = ((long(frame) AND '1FF'XL) OR $ 
          (ishft((long(bed) AND 'F'XL), 12)) OR $
          (ishft(lo_plane, 16))	OR $
          (ishft(hi_plane, 1))	OR $
          (ishft((long(gate) AND '3F'XL), 24)) OR $
          (ishft(lo_data, 30))	OR $
          (ishft(hi_data, 9)))
   
   RETURN, mat
END
