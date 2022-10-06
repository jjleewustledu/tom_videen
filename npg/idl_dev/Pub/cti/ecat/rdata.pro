;       $Revision:   1.1  $ $Date:   10/15/93 18:23:26  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RDATA	
;
; PURPOSE:
; 	Reads into the parameter data the matrix data at the positions
; 	given by offsets in the open file represented by unit.			
;
; CATEGORY:	
; 	CTI Matrix utility.		
;
; CALLING SEQUENCE:		
; 	result = rdata (dtype,unit,offsets,xd,yd,zd,data,SCALE=scale)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


FUNCTION  rdata, dtype, unit, offsets, xd, yd, zd, data, SCALE=scale

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 7) THEN BEGIN
      set_ecat_error, 2, "RDATA", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF
   sf = keyword_set(SCALE)
   IF (NOT sf) THEN $
      scale = replicate(1, zd)

   CASE !ECAT.data_types(dtype) OF

; Some of our images mistakenly have data_type=2 instead of
; data_type=6.  We have no VAX_I2 (data_type=2) images!
;    "VAX_I2" : $
;       BEGIN
;         IF (sf) THEN 		 $
;          data = fltarr(xd, yd, zd) $
;         ELSE $
;          data = intarr(xd, yd, zd)
;         d = intarr(xd, yd)
;         FOR i = 0, n_elements(offsets)-1 DO BEGIN
;            point_lun, unit, offsets(i)+!ECAT.blksize
;            readu, unit, d
;            IF (NOT !LITTLE_ENDIAN) THEN byteorder, d
;            data(*, *, i) = d*scale(i)
;         ENDFOR
;      END
; Therefore treat VAX_I2 as SUN_I2:

      "VAX_I2" : $
         BEGIN
         IF (sf) THEN 		 $
            data = fltarr(xd, yd, zd) $
         ELSE $
            data = intarr(xd, yd, zd)
         d = intarr(xd, yd)
         FOR i = 0, n_elements(offsets)-1 DO BEGIN
            point_lun, unit, offsets(i)+!ECAT.blksize
            readu, unit, d
            IF (!LITTLE_ENDIAN) THEN byteorder, d
            data(*, *, i) = d*scale(i) 
         ENDFOR
      END

      "SUN_I2" : $
         BEGIN
         IF (sf) THEN 		 $
            data = fltarr(xd, yd, zd) $
         ELSE $
            data = intarr(xd, yd, zd)
         d = intarr(xd, yd)
         FOR i = 0, n_elements(offsets)-1 DO BEGIN
            point_lun, unit, offsets(i)+!ECAT.blksize
            readu, unit, d
            IF (!LITTLE_ENDIAN) THEN byteorder, d
            data(*, *, i) = d*scale(i) 
         ENDFOR
      END

      "VAX_R4" : $
         BEGIN
         data = fltarr(xd, yd, zd)
         d = lonarr(xd, yd)
         FOR i = 0, n_elements(offsets)-1 DO BEGIN
            point_lun, unit, offsets(i)+!ECAT.blksize
            readu, unit, d
            IF (NOT !LITTLE_ENDIAN) THEN byteorder, d, /lswap
            data(*, *, i) = vtosf(d)*scale(i)
         ENDFOR
      END

      "SUN_R4" : $
         BEGIN
         data = fltarr(xd, yd, zd)
         d = fltarr(xd, yd)
         FOR i = 0, n_elements(offsets)-1 DO BEGIN
            point_lun, unit, offsets(i)+!ECAT.blksize
            readu, unit, d
            data(*, *, i) = d*scale(i)
            IF (!LITTLE_ENDIAN) THEN byteorder, d, lswap
         ENDFOR
      END

      ELSE : BEGIN
         set_ecat_error, 4, "RDATA", UNIT = unit
         RETURN, !ECAT.ERROR
      END
   ENDCASE

   d = 0
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RDATA", UNIT = unit
   RETURN, !ECAT.ERROR

END
