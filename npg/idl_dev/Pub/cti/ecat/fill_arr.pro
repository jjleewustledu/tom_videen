;       $Revision:   1.1  $ $Date:   10/15/93 18:21:34  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	FILL_ARR	
;
; PURPOSE:
;	Takes array a and makes it have a length equal to len.			
;
; CATEGORY:
;	CTI Matrix utilities			
;
; CALLING SEQUENCE:
;	fill_arr, a, len		
;
; INPUTS:	
;	a - input array	
;	len - specified array length	
;
; OUTPUTS:
;	a - array of length 'len'			
;
; HISTORY NOTES:		
;-

PRO	fill_arr, a, len

   on_error, 2
   n = n_elements(a)
   IF (n LT len) THEN a = [a, make_array(len-n,size = size(a))]
   RETURN
END
