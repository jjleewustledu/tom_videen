pro xbm2idl, file, invert = invert
;+
; NAME: XBM2IDL.PRO
;
; PURPOSE: Converts a x bitmap file to an idl bitmap definition file
;          suitable for including in widget apps for bitmap buttons.
;
; CATEGORY: WIDGETS
;
; CALLING SEQUENCE: 
;     xmb2idl, 'filename'
; 
; INPUTS:
;     file : file name w/o extension.
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS:
;      invert : Used to invert the X bitmap file
;
; OUTPUTS: none
;
; OPTIONAL OUTPUTS: none
;
; COMMON BLOCKS: none
;
; SIDE EFFECTS: Writes a file to the current directory called file.bm
;
; RESTRICTIONS: X bitmap files must have extension '.xbm'
;
; PROCEDURE: Straighforward
;
; MODIFICATION HISTORY:
;        20 Dec 96 Initial Coding. PMW
;-

ON_ERROR, 1

if not(keyword_set(invert)) then invert = 0
; This array, indexed by an ASCII character, returns the hex value associated
; with that character in the least significant 4 bits of the byte.
lo_ascii=bytarr(256, /nozero)
lo_ascii((byte('0'))(0):(byte('9'))(0)) = bindgen(10)
lo_ascii((byte('A'))(0):(byte('F'))(0)) = bindgen(6)+10B
lo_ascii((byte('a'))(0):(byte('f'))(0)) = bindgen(6)+10B

; This array, indexed by an ASCII character, returns the hex value associated
; with that character in the most significant 4 bits of the byte.
hi_ascii=bytarr(256, /nozero)
hi_ascii((byte('0'))(0):(byte('9'))(0)) = bindgen(10)*16B
hi_ascii((byte('A'))(0):(byte('F'))(0)) = (bindgen(6)+10)*16B
hi_ascii((byte('a'))(0):(byte('f'))(0)) = (bindgen(6)+10)*16B

xbmfile = file + '.xbm'
idlfile = file + '.bm'
openr, unit, xbmfile, /get_lun

; The width
line = ''
while (((p=strpos(line, '_width'))) eq -1) do begin
  if (eof(unit)) then message,'Not an X11 bitmap file: ' + FILE
  readf, unit, line
endwhile
x = long(strmid(line, p+6, 32767))

; The height
readf, unit, line
while (((p=strpos(line, '_height'))) eq -1) do readf, unit, line
y = long(strmid(line, p+7, 32767))

; The result array
  x_bytes = (x + 7) / 8
  BITMAP=bytarr(X_BYTES, Y, /nozero)

; Get to the start of the data
while (((p=strpos(line, '[]'))) eq -1) do readf, unit, line

; Read it
readf, unit, line
pos=0
for i = 0L, x_bytes * y - 1 do begin
  pos = strpos(line, '0x', pos)
  if (pos eq -1) then begin
    readf, unit, line
    pos = strpos(line, '0x')
  endif
  tmp = byte(strmid(line, pos+2, 2))
  pos=pos+4
  bitmap(i) = hi_ascii(tmp(0)) + lo_ascii(tmp(1))
endfor
free_lun, unit

if invert then bitmap = byte(abs(bitmap-255))

IF !VERSION.OS NE "MacOS" THEN newline = string(10B) $
ELSE newline = string(13B)
OPENW, unit, idlfile, /GET_LUN
WRITEU, unit, + $
  ";" + file + " bitmap" + newline + $
  "; definition" + newline
WRITEU, unit, file + " = [				$"
WRITEU, unit, newline
FOR j = 0, y - 1 DO BEGIN
    line = string(bitmap(*,j), $
                  format = '("		[",(i3.3,"B",:,", "))')
    WRITEU, unit, line
    WRITEU, unit, "]"
    IF(j LT y-1) THEN $
      WRITEU, unit ,",			$" + newline $
    ELSE WRITEU, unit, "			$" + newline
ENDFOR
WRITEU, unit, "		]"
FREE_LUN, unit
END
