;       $Revision:   1.1  $ $Date:   10/15/93 18:22:04  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:	
; 	PMH6	
;
; PURPOSE:	
; 	Prints the different structures	that compose the ECAT6 main header
; 	structure to the screen.	
;
; CATEGORY:
; 	CTI Matrix utility			
;
; CALLING SEQUENCE:	
; 	pmh6, mh, FILE=file, PAT=pat, SCAN=scan	
;
; INPUTS:
; 	mh - main header structure to be printed.			
;
; KEYWORD PARAMETERS:	
; 	FILE - print the file structure of the main header.
; 	PAT - print the patient structure of the main header.
; 	SCAN - print the scan structure of the main header.	
;
; OUTPUTS:			
; 	Information printed to screen.
;
; HISTORY NOTES:		
;-
 	  
PRO pmh6, mh, FILE=file, PAT=pat, SCAN=scan

   on_error, 2
   IF (n_params() NE 1) THEN BEGIN
      set_ecat_error, 2, "PMH6"
      RETURN
   ENDIF

   help, /struct, mh
   print
   IF (keyword_set(FILE)) THEN BEGIN
      format = '("SCAN START -> ", I2, ":", I2, ":", I2, " on ", I2, "/", I2, "/", I4)'
      print, FORMAT = format, mh.file.scan_start(*, 1), mh.file.scan_start([1, 0, 2], 0)
      print
      help, /struct, mh.file
   ENDIF
   IF (keyword_set(PAT)) THEN BEGIN
      print
      print, "PATIENT INFO"
      print, "------------"
      print, "  STUDY NAME    : ", mh.pat(0)
      print, "  PATIENT ID    : ", mh.pat(1)
      print, "  PATIENT NAME   : ", mh.pat(2)
      print, "  PATIENT SEX    : ", mh.pat(3)
      print, "  PATIENT AGE    : ", mh.pat(4)
      print, "  PATIENT HEIGHT  : ", mh.pat(5)
      print, "  PATIENT WEIGHT  : ", mh.pat(6)
      print, "  PATIENT DEXTERITY : ", mh.pat(7)
      print, "  PHYSICIAN NAME  : ", mh.pat(8)
      print, "  OPERATOR NAME   : ", mh.pat(9)
      print, "  STUDY DESCRIPTION : ", mh.pat(10)
   ENDIF
   IF (keyword_set(SCAN)) THEN BEGIN
      print
      help, /struct, mh.scan
      print
      format='("GANTRY", 10X, ": TILT = ", F9.4, " ROTATION = ", F9.4)'
      print, FORMAT = format, mh.scan.gantry
      format='("TRUES THRESHOLD : LOWER = ", I4, 10X, "UPPER = ", I4)'
      print, FORMAT = format, mh.scan.true_thresh
      format='("INITIAL BED POSITION  = ", F9.4)'
      print, FORMAT = format, mh.scan.init_bed_position
      format='("BED POSITION ", I2, 6X, "  = ", F9.4)'
      FOR i = 1, mh.file.nbeds DO BEGIN
         print, FORMAT = format, i, mh.scan.bed_offset(i-1)
      END
   ENDIF
END
