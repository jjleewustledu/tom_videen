;       $Revision:   1.1  $ $Date:   10/15/93 18:40:50  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	STOVF	
;
; PURPOSE:	
; 	Changes Sun floats to VAX floats		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:
; 	result = stovf (f)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION stovf, f

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "STOVF"
      RETURN, 0.0
   ENDIF

   bits = long(f, 0, n_elements(f))
   sign = where((bits AND '80000000'xl) NE 0, scount) ; Sign bit
   expon = ishft(bits, -23) AND 'ff'x ; Exponent
   fraction = bits AND '7fffff'xl ; Coefficient
   vax = ishft((expon+2), 7) +   $ ; Exponent
      (ishft(fraction, -16) AND '7f'xl) + $ ; Most sig fraction
      ishft(fraction AND 'ffff'xl, 16)
   if scount GT 0 THEN vax(sign) = vax(sign) OR '8000'xl ; Add sign bit
   IF (NOT(!LITTLE_ENDIAN)) THEN byteorder, vax, /lswap
   RETURN, vax
END 
