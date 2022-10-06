;       $Revision:   1.1  $ $Date:   10/15/93 18:23:02  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	PMH7	
;
; PURPOSE:	
; 	Prints the different structures	that compose the ECAT7 main header
; 	structure to the screen.	
;
; CATEGORY:
; 	CTI Matrix utility			
;
; CALLING SEQUENCE:	
; 	pmh7, mh, GANTRY=gantry, SCAN=scan		
; INPUTS:
; 	mh - main header structure to be printed.			
;
; KEYWORD PARAMETERS:	
; 	GANTRY - print the gantry structure of the main header.
; 	SCAN - print the scan structure of the main header.	
;
; OUTPUTS:			
; 	Information printed to screen.
;
; HISTORY NOTES:		
;-
 	  
PRO pmh7, mh, GANTRY=gantry, SCAN=scan

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "PMH7"
      RETURN
   ENDIF

   help, /struct, mh
   print
   format='("INITIAL BED POSITION  = ", F9.4)'
   print, FORMAT = format, mh.init_bed_position
   format='("BED POSITION ", I2, 6X, "  = ", F9.4)'
   FOR i = 1, mh.num_bed_pos DO BEGIN
      print, FORMAT = format, i, mh.bed_position(i-1)
   END
   print
   IF (keyword_set(GANTRY)) THEN BEGIN
      help, /struct, mh.gantry
   ENDIF
   IF (keyword_set(SCAN)) THEN BEGIN
      print
      help, /struct, mh.scan
   ENDIF
END
