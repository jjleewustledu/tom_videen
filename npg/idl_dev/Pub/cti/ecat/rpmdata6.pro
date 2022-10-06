;       $Revision:   1.2  $ $Date:   10/29/93 17:22:24  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	RPMDATA6	
;
; PURPOSE:	
; 	Read the ECAT6 polar map data from the file represented by
; 	unit.		
;
; CATEGORY:	
; 	CTI Matrix utility		
;
; CALLING SEQUENCE:		
; 	result = rpmdata6 (unit, sh, offsets, data)
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

FUNCTION rpmdata6, unit, sh, offsets, data

   size = sh(0).num_rings + total(sh(0).sect_per_ring)
   d = intarr(size)
   data = fltarr(n_elements(offsets), size)
   FOR i=0, n_elements(offsets)-1 DO BEGIN
      print, sh(i).num_rings + total(sh(i).sect_per_ring)
      point_lun, unit, offsets(i)+!ECAT.blksize
      readu, unit, d
      IF (!LITTLE_ENDIAN) THEN byteorder, d
      data(i, *) = d * sh(i).scale_factor
   ENDFOR

END
