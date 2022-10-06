;       $Revision:   1.2  $ $Date:   10/15/93 18:13:20  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
;	DATA_SIZE	
;
; PURPOSE:
;	Compute the size of the data in blocks			
;
; CATEGORY:
;	CTI matrix read utility			
;
; CALLING SEQUENCE:	
;	Stat = DATA_SIZE (Dtype, Dimsize)	
;
; INPUTS:	
;	Dtype - data type
;	Dimsize - dimension size		
;
; KEYWORD PARAMETERS:	
;	None.	
;
; OUTPUTS:	
;	Stat - data size (in blocks) or an error condition. 		
;		
;-

FUNCTION DATA_SIZE,Dtype,Dimsize

   on_error,2
   IF (n_params() NE 2) THEN BEGIN
      set_ecat_error,2,"DATA_SIZE"
      RETURN,!ECAT.ERROR
   ENDIF

   CASE !ECAT.data_types(dtype) of
      "VAX_I2" : size = 2L*dimsize
      "SUN_I2" : size = 2L*dimsize
      "VAX_R4" : size = 4L*dimsize
      "SUN_R4" : size = 4L*dimsize
      ELSE : BEGIN
         set_ecat_data,0,"DATA_SIZE",UNIT=unit
         RETURN,!ECAT.ERROR
      END
   ENDCASE
   
   IF (size LT !ECAT.BLKSIZE) THEN RETURN,1L
   IF ((size MOD !ECAT.BLKSIZE) EQ 0) THEN RETURN,long(size/!ECAT.BLKSIZE)
   RETURN,long(float(size)/!ECAT.BLKSIZE)+1L

END
