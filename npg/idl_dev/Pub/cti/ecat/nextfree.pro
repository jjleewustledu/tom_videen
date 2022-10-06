;       $Revision:   1.1  $ $Date:   10/15/93 18:21:58  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	NEXTFREE	
;
; PURPOSE:
; 	Take a directory and directory information from the rdir
; 	routine, check for the next free directory position or
; 	create a new directory block if necessary, and return the block
; 	number for the next free block.
; 				
;
; CATEGORY:
; 	CTI Matrix utility			
;
; CALLING SEQUENCE:		
; 	result = nextfree (unit,matnum,nblks,dir,dirinfo,ecat6=ecat6)
;
; INPUTS:	
; 	unit -
; 	matnum -
; 	nblks -
;	dir -
; 	dirinfo -
;
; KEYWORD PARAMETERS:	
; 	ecat6 - 	
;
; OUTPUTS:
; 	result - 			
;
; HISTORY NOTES:		
;-

FUNCTION nextfree, unit, matnum, nblks, dir, dirinfo, ecat6 = ecat6

   on_error, 2
   IF (n_params() NE 5) THEN BEGIN
      set_ecat_error, 2, "NEXTFREE", UNIT = unit
      RETURN, !ECAT.ERROR
   ENDIF

   ind = where(dirinfo(0,*) GT 0, anyfree)
   IF (anyfree EQ 0) THEN BEGIN
      IF (keyword_set(ECAT6)) THEN BEGIN
         stat = mknewdirblk6(unit, dir, dirinfo)
      ENDIF ELSE stat = mknewdirblk7(unit, dir, dirinfo)

      IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   ENDIF

   ndir = n_elements(dirinfo(0, *))
   IF (ndir EQ 1) THEN 		$
    dirblk = !ECAT.DIRBLK1	$
   ELSE 			$			
    dirblk = dirinfo(1, ndir-2)
   ind = where(dir(2,*) GT 0, nmats)
   IF (nmats EQ 0) THEN BEGIN
      nextblk = !ECAT.DIRBLK1+1L
   ENDIF ELSE BEGIN
      nextblk = dir(2, nmats-1)+1L
      IF (nextblk Eq dirblk) THEN nextblk = dirblk+1L
   ENDELSE
   
   stat = enter_mat(unit, matnum, dir, dirinfo, dirblk, nextblk, nblks, direntry)
   IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   dirinfo(0, ndir-1) = dirinfo(0, ndir-1)-1
   dirinfo(3, ndir-1) = dirinfo(3, ndir-1)+1
   ind = where(dir(0, *) EQ 0)
   dir(*, ind(0)) = [direntry, (direntry(1)-1)*!ECAT.BLKSIZE]
   
   RETURN, nextblk

END
;
