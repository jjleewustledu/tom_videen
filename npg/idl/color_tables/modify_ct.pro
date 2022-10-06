; $Id: modify_ct.pro,v 1.8 1999/01/26 22:37:27 alan Exp $
;+
; NAME:
;	MODIFY_CT 
;
; PURPOSE:
;	Update the specified color table file.
;
; CATEGORY:
;	Z4 - Image processing, color table manipulation.
;
; CALLING SEQUENCE:
;	MODIFY_CT, Itab, Name, R, G, B, Filename
;
; INPUTS:
;	Itab:	The table to be updated, numbered from 0 to 255.  If the
;		table entry is greater than the next available location
;		in the table, then the entry will be added to the table
;		in the available location rather than the index specified
;		by Itab.  On return, Itab will contain the index for the
;		location that was modified or extended.  The table 
;		can be loaded with the IDL command:  LOADCT, Itab.
;
;	Name:	A string up to 32 characters long that contains the name for 
;		the new color table.
;
;	R:	A 256-element vector that contains the values for the red
;		color gun.
;
;	G:	A 256-element vector that contains the values for the green
;		color gun.
;
;	B:	A 256-element vector that contains the values for the blue
;		color gun.
;
;	FILENAME: Color table filename to be modified.
;		The file must exist and be a copy of the colors1.tbl file.
;
; OUTPUTS:
;	Itab: The index of the entry which was updated, 0 to 255.  This
;		may be different from the input value of Itab if the
;		input value was greater than the next available location
;		in the table.  If this was the case the entry was added to
;		the table in the next available location instead of leaving
;		a gap in the table.
;
; COMMON BLOCKS:
;	None.
;
; SIDE EFFECTS:
;	The color table file is modified.
;
; MODIFICATION HISTORY:
;	Aug, 1982, DMS, Written.
;	Unix version, 1987, DMS.
;	ACY, 9/92, Update for new color table structure, Add FILE keyword.
;		   Allow extending table.
;	WSO, 1/95, Updated for new directory structure
;	TOV, 11/2000, copied from release 5.3 and modified.
;	
;-

PRO MODIFY_CT, ITAB, NAME, R, G, B, Filename
   ON_IOERROR, bad
   ON_ERROR, 2                  ; Return to caller if an error occurs
   
   IF (itab LT 0) OR (itab GT 255) THEN BEGIN
      MESSAGE, 'Color table number out of range.'
      RETURN
   ENDIF
   
   GET_LUN, iunit
   OPENU, iunit, filename, /BLOCK
   ntables = 0b
   readu, iunit, ntables
   print,ntables

   IF (itab LT ntables) THEN BEGIN ; Update an existing record
      aa = ASSOC(iunit, bytarr(32, ntables), ntables*768L+1) ; Read the record
      a = aa[0]                 ; Get name record
      a[*,itab] = 32B           ; Blank out old name
      a[0:strlen(name)-1, itab] = byte(name) ; Add new name
      aa[0] = a                 ; Update table names
      aa = ASSOC(iunit, bytarr(256), 1) ; Update vector; skip past count
      aa[itab*3]   = byte(R)    ; Insert Red/Green/Blue
      aa[itab*3+1] = byte(G)
      aa[itab*3+2] = byte(B)

   ENDIF ELSE BEGIN             ; Add a new record at end of table
      itab = ntables
      aa = ASSOC(iunit, bytarr(32,ntables), ntables*768L+1) ; Read record
      a = aa[0]                 ; Get name record
      aa = ASSOC(iunit, bytarr(256), ntables*768L+1) ; Update vectors
      aa[0] = byte(R)           ; Insert Red/Green/Blue
      aa[1] = byte(G)
      aa[2] = byte(B)
      aa = ASSOC(iunit, bytarr(32,ntables+1), (ntables+1)*768L+1) ; Read names
      temp = bytarr(32) + 32B	; Add new name to end
      temp[0:strlen(name)-1] = byte(name)
      allnames = bytarr(32, ntables+1)
      allnames[*, 0:ntables-1] = a
      allnames[*, ntables] = temp
      aa[0] = allnames          ; Write the names out
      aa = ASSOC(iunit, bytarr(1)) ; Update ntables count
      aa[0] = [ntables + 1B]
   ENDELSE

   FREE_LUN, iunit
   RETURN
bad:
   MESSAGE, /CONTINUE, 'Error writing file: ' + filename + ', ' + !err_string
END
