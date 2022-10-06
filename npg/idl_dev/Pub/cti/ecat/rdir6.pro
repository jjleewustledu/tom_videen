;       $Revision:   1.1  $ $Date:   10/15/93 18:23:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RDIR6
;
; PURPOSE:	
; 	Read the ECAT6 directory of the file indicated either by
; 	the open file unit or the file indicated by the keyword file.		
;
; CATEGORY:
; 	CTI Matrix utility.			
;
; CALLING SEQUENCE:		
; 	result = rdir6 (unit,dir,dirinfo,FILE=file)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION rdir6, unit, dir, dirinfo, FILE = file

   on_error, 2
   on_ioerror, io_error
   IF (n_params() LT 2) THEN BEGIN
      IF (keyword_set(FILE)) THEN  $
         set_ecat_error, 2, "RDIR6", FILE = file $
      ELSE     $
         set_ecat_error, 2, "RDIR6", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF

   BLKSIZE = !ECAT.BLKSIZE
   DIRBLK1 = !ECAT.DIRBLK1
   dirblk = lonarr(4, 32)
   IF (keyword_set(FILE)) THEN openr, unit, file, /get_lun
   point_lun, unit, (DIRBLK1-1)*BLKSIZE
   readu, unit, dirblk
   IF (NOT !LITTLE_ENDIAN) THEN byteorder, dirblk, /lswap
   dir = dirblk(*, 1:*)
   dirinfo = dirblk(*, 0)
   
   WHILE (dirblk(1, 0) NE DIRBLK1) DO BEGIN
      point_lun, unit, long(BLKSIZE*(dirblk(1, 0)-1))
      readu, unit, dirblk
      IF (NOT !LITTLE_ENDIAN) THEN byteorder, dirblk, /lswap
      dirinfo = [[dirinfo], [dirblk(*, 0)]]
      dir = [[dir], [dirblk(*, 1:*)]]
   END
   dir = [dir, (((dir(1, *) GT 0)*dir(1, *))-1)*!ECAT.blksize >0]
   IF (keyword_set(FILE)) THEN free_lun, unit
   dirblk = 0
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "RDIR6", UNIT = unit
   RETURN, !ECAT.ERROR
END
