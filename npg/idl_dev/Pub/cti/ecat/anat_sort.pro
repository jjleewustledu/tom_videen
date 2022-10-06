;       $Revision:   1.1  $ $Date:   10/15/93 18:13:06  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
;	ANAT_SORT
;
; PURPOSE:	
;	Sorts the given matrix directory into anatomical order
;
; CATEGORY:	
;	CTI matrix read utility
;
; CALLING SEQUENCE:		
;	Stat = ANAT_SORT (Dir, Mh)
;
; INPUTS:			
;	Dir - directory list to be ordered.
;	Mh - main header
;
;
; KEYWORD PARAMETERS:		
;	None.
;
; OUTPUTS:			
;	Dir - ordered directory list.
;
; RESTRICTIONS:			
;	Works with ECAT type 6 matrix file.
;
;-


FUNCTION ANAT_SORT,Dir,Mh

   on_error,2
   IF (n_params() NE 2) THEN BEGIN
      set_ecat_error,2,"ANAT_SORT"
      RETURN,!ECAT.ERROR
   ENDIF

   init_pos = mh.scan.init_bed_position
   offsets = mh.scan.bed_offset
   plane_separation = mh.scan.plane_separation
   dir = dir(*,where(dir(0,*) GT 0))
   matnums = dir(0,*)
   nbeds = mh.file.nbeds
   nplanes=mh.file.nplanes

   slice_loc=0.0
   bo=[0,offsets]
   FOR i=0,nbeds DO BEGIN
      init_pos = init_pos + bo(i)
      FOR j=0,nplanes-1 DO BEGIN
         slice_loc=[slice_loc, init_pos + j*plane_separation]
      ENDFOR
   ENDFOR
   index=sort(slice_loc(1:*))
   dir=dir(*,index)
   RETURN,!ECAT.OK
END
