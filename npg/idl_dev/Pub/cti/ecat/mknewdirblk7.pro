;       $Revision:   1.1  $ $Date:   10/15/93 18:21:46  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	MKNEWDIRBLK7	
;
; PURPOSE:			
; 	Creates a new directory block for an ECAT7 matrix file in the 
; 	file reqpresented by unit. The next free block is returned on 
; 	success. 
;
; 	The directory block for ECAT7 files is stored in Sun (BIG_ENDIAN)
; 	format. When writing the block on a LITTLE_ENDIAN base system, need
; 	to swap the byteorder before writing.
;;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = mknewdirblk7 (unit,dir,dirinfo,NEW=new)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION mknewdirblk7, unit, dir, dirinfo, NEW = new
   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 3) THEN BEGIN
      set_ecat_error, 2, "MKNEWDIRBLK7", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF

   IF (keyword_set(NEW)) THEN BEGIN
      dirblk = lonarr(4, 32)
      dirblk(*, 0) = [31, !ECAT.DIRBLK1, 0, 0]
      dir = [dirblk(*, 1:*), transpose(lonarr(31))]
      dirinfo = dirblk(*, 0)
      point_lun, unit, !ECAT.BLKSIZE
      IF (!LITTLE_ENDIAN) THEN byteorder, dirblk, /lswap
      writeu, unit, dirblk
      RETURN, !ECAT.OK
   ENDIF

   dirblk = lonarr(4, 32)
   ndirblks = n_elements(dirinfo(0, *))
   IF (ndirblks EQ 1) THEN   $
    prevblk = !ECAT.DIRBLK1  $
   ELSE   $
    prevblk = dirinfo(1, ndirblks-2)
   ind = where(dir(2,*) GT 0, nmats)
   IF (nmats EQ 0) THEN BEGIN
      set_ecat_error, 5, "MKNEWDIRBLK7", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF
   lastblk = dir(2, ind(nmats-1))

   dirblk(*, 0) = [31L, 0L, 0L, 0L]
   dirblk(1, 0) = !ECAT.DIRBLK1
   dirblk(2, 0) = prevblk
   dirblk(3, 0) = 0L
   dir = [[dir], [lonarr(5, 31)]]
   dirinfo = [[dirinfo], [dirblk(*, 0)]]
   point_lun, unit, lastblk*!ECAT.BLKSIZE
   wdir = dirblk
   IF (!LITTLE_ENDIAN) THEN byteorder, wdir, /lswap
   writeu, unit, wdir
   point_lun, unit, (dirblk(2, 0)-1)*!ECAT.BLKSIZE
   prevdir = lonarr(4)
   readu, unit, prevdir
   IF (!LITTLE_ENDIAN) THEN byteorder, prevdir, /lswap
   newdirblk = lastblk+1L
   prevdir(1) = newdirblk
   point_lun, unit, (dirblk(2, 0)-1)*!ECAT.BLKSIZE
   IF (!LITTLE_ENDIAN) THEN byteorder, prevdir, /lswap
   writeu, unit, prevdir
   dirinfo(1, n_elements(dirinfo(0, *))-2) = newdirblk

   RETURN, !ECAT.OK

   io_error : set_ecat_error, 1, "MKNEWDIRBLK7", UNIT = unit
   RETURN, !ECAT.ERROR
END
