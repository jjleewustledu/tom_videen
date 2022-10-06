;       $Revision:   1.1  $ $Date:   10/15/93 18:41:22  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	WDATA
;
; PURPOSE:
; 	Write the data into the file represented by unit.			
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = wdata (unit,ftype,dtype,offsets,data)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION wdata, unit, ftype, dtype, offsets, data

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 5) THEN BEGIN
      set_ecat_error, 2, "WDATA", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF

   s = size(data(*, *, 0))
   size = s(1)*s(2)
   pad = 0

   CASE !ECAT.data_types(dtype) of
      "VAX_I2" : $
         BEGIN
         pad = 0L 
         IF (size*2L LT !ECAT.BLKSIZE) THEN BEGIN
            leftover = (!ECAT.BLKSIZE/2L)-size
            pad = intarr(s(1), leftover/s(1))
         ENDIF ELSE BEGIN
            r = (size*2L) MOD !ECAT.BLKSIZE
            IF (r GT 0) THEN BEGIN
               leftover = (!ECAT.BLKSIZE/2L)-r
               pad = intarr(s(1), leftover/s(1))
            ENDIF
         ENDELSE
         byteorder, data
         FOR i=0, n_elements(offsets)-1 DO BEGIN
            d = data(*, *, i)
            IF (pad) THEN d = [[data(*, *, i)], [pad]]
            a = assoc(unit, d, offsets(i))
            a(0) = d
         ENDFOR       
      END

      "VAX_R4" : $
         BEGIN
         pad = 0L 
         IF (size*4L LT !ECAT.BLKSIZE) THEN BEGIN
            leftover = (!ECAT.BLKSIZE/4L)-size
            pad = fltarr(s(1), leftover/s(1))
         ENDIF ELSE BEGIN
            r = (size*4L) MOD !ECAT.BLKSIZE
            IF (r GT 0) THEN BEGIN
               leftover = (!ECAT.BLKSIZE/2L)-r
               pad = fltarr(s(1), leftover/s(1))
            ENDIF
         ENDELSE
         FOR i=0, n_elements(offsets)-1 DO BEGIN
            d = stovf(data(*, *, i))
            IF (pad) THEN d = [[d], [pad]]
            a = assoc(unit, d, offsets(i))
            a(0) = d
         ENDFOR       
      END

      "SUN_I2" : $
         BEGIN
         pad = 0L 
         IF (size*2L LT !ECAT.BLKSIZE) THEN BEGIN
            leftover = (!ECAT.BLKSIZE/2L)-size
            pad = intarr(s(1), leftover/s(1))
         ENDIF ELSE BEGIN
            r = (size*2L) MOD !ECAT.BLKSIZE
            IF (r GT 0) THEN BEGIN
               leftover = (!ECAT.BLKSIZE/2L)-r
               pad = intarr(s(1), leftover/s(1))
            ENDIF
         ENDELSE
         FOR i=0, n_elements(offsets)-1 DO BEGIN
            d = data(*, *, i)
            IF (pad) THEN d = [[data(*, *, i)], [pad]]
            a = assoc(unit, d, offsets(i))
            a(0) = d
         ENDFOR    
      END

      "SUN_R4" : $
         BEGIN
         pad = 0L 
         IF (size*4L LT !ECAT.BLKSIZE) THEN BEGIN
            leftover = (!ECAT.BLKSIZE/4L)-size
            pad = fltarr(s(1), leftover/s(1))
         ENDIF ELSE BEGIN
            r = (size*4L) MOD !ECAT.BLKSIZE
            IF (r GT 0) THEN BEGIN
               leftover = (!ECAT.BLKSIZE/2L)-r
               pad = fltarr(s(1), leftover/s(1))
            ENDIF
         ENDELSE
         FOR i=0, n_elements(offsets)-1 DO BEGIN
            d = data(*, *, i)
            IF (pad) THEN d = [[data(*, *, i)], [pad]]
            a = assoc(unit, d, offsets(i))
            a(0) = d
         ENDFOR    
      END

      ELSE : BEGIN
         set_ecat_data, 0, "WDATA", UNIT=unit
         RETURN, !ECAT.ERROR
      END
   ENDCASE

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "WDATA", UNIT=unit
   RETURN, !ECAT.ERROR

END
