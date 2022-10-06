;       $Revision:   1.1  $ $Date:   10/15/93 18:28:04  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RDIR7
;
; PURPOSE:
; 	Read the directory of the file indicated by either the open
; 	file unit or the file indicated by the keyword file.			
;
; CATEGORY:			
; 	CTI Matrix utility
;
; CALLING SEQUENCE:
; 	result = rdir7 (unit,dir,dirinfo,FILE=file)		
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY:
;-

FUNCTION  rdir7, unit, dir, dirinfo, FILE = file

   on_error, 2
   on_ioerror, io_error
   IF (n_params() LT 2) THEN BEGIN
      IF (keyword_set(FILE)) THEN      $
       set_ecat_error, 2, "RDIR7", FILE = file $
      ELSE                 $
       set_ecat_error, 2, "RDIR7", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF

   BLKSIZE = !ECAT.BLKSIZE
   DIRBLK1 = !ECAT.DIRBLK1
   dirblk  = lonarr(4, 32)
   IF (keyword_set(FILE)) THEN openr, unit, file, /get_lun
   point_lun, unit, (DIRBLK1-1)*BLKSIZE
   readu, unit, dirblk
   IF (!LITTLE_ENDIAN) THEN byteorder, dirblk, /lswap
   dir = dirblk(*, 1:*)
   dirinfo = dirblk(*, 0)

   WHILE (dirblk(1, 0) NE DIRBLK1) DO BEGIN
      point_lun, unit, long(BLKSIZE*(dirblk(1, 0)-1))
      readu, unit, dirblk
      IF (!LITTLE_ENDIAN) THEN byteorder, dirblk, /lswap
      dirinfo = [[dirinfo], [dirblk(*, 0)]]
      dir  = [[dir], [dirblk(*, 1:*)]]
   END
   dir = [dir, (((dir(1, *) GT 0)*dir(1, *))-1)*!ECAT.blksize >0]
   IF (keyword_set(FILE)) THEN free_lun, unit
   dirblk = 0
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RDIR7", UNIT = unit
   RETURN, !ECAT.ERROR
END
