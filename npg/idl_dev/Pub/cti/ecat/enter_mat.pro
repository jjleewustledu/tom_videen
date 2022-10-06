;       $Revision:   1.1  $ $Date:   10/15/93 18:13:38  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	ENTER_MAT	
;
; PURPOSE:			
;	ENTER_MAT updates the directory with a new entry. Enter_mat returns
;	!ECAT.OK on success and !ECAT.ERROR on failure.
;
; CATEGORY:	
;	CTI Matrix utility		
;
; CALLING SEQUENCE:		
;	stat = enter_mat(unit,matnum,dir,dirinfo,dirblk,nextblk,nblks,direntry)
;
; INPUTS:
;	unit - 
;	matnum -
;	dir -
;	dirinfo -
;	dirblk -
;	nextblk -
;	nblks -
;	direntry -			
;
; KEYWORD PARAMETERS:	
;	None.	
;
; OUTPUTS:
;	stat - !ECAT.OK on success, !ECAT.ERROR on failure.			
;
;-

FUNCTION enter_mat, unit, matnum, dir, dirinfo, dirblk, nextblk, nblks, direntry

   on_error, 2
   on_ioerror, io_error

   IF (n_params() NE 8) THEN BEGIN
      set_ecat_error, 2, "ENTER_MAT", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF
   point_lun, unit, (dirblk-1)*!ECAT.BLKSIZE
   d = lonarr(4, 32)
   readu, unit, d & IF (NOT !LITTLE_ENDIAN) THEN byteorder, d, /lswap
   ind = where(d(0,*) EQ 0, free)
   IF (free EQ 0) THEN BEGIN
      set_ecat_error, 5, "ENTER_MAT", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF
   direntry = [long(matnum), nextblk, nextblk+nblks-1L, 1L]
   d(0, 0) = d(0, 0)-1
   d(3, 0) = d(3, 0)+1
   d(*, ind(0)) = direntry
   point_lun, unit, (dirblk-1)*!ECAT.BLKSIZE
   byteorder, d, /lswap & writeu, unit, d
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "ENTER_MAT", UNIT = unit
   RETURN, !ECAT.ERROR
END
