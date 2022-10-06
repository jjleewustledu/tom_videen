;       $Revision:   1.1  $ $Date:   10/15/93 18:41:16  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	VTOSF	
;
; PURPOSE:
; 	Convert VAX floats to Sun floats			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = vtosf (bits)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION  vtosf, bits

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "VTOSF"
      RETURN, 0.0
   ENDIF

   IF (NOT !LITTLE_ENDIAN ) THEN byteorder, bits, /lswap
   sign = where((bits AND '8000'xl) NE 0, scount)
   exponent = (ishft(bits, -7) AND 'ff'x) - 128 ; exponent - bias
   fraction = ishft(bits AND '7f'x, 16)   + $ ; high 7 bits of fraction
      ishft(bits AND 'ffff0000'xl, -16) + $ ; low 16 bits of fraction
      '800000'xl                ; hidden bit = 1
   z = where(bits EQ 0, count)
   IF (count GT 0) THEN BEGIN   ; set the zeros
      fraction(z) = 0.0
      exponent(z) = 0
   ENDIF
   x = fraction*(2.0^(exponent-24.0))
   IF (scount GT 0) THEN x(sign) = -x(sign)
   RETURN, x
END
