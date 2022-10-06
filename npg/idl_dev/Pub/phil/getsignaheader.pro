;            File: $RCSfile: getsignaheader.pro,v $
;          Author: $Author: williams $
;         version: $Revision: 2.0 $
;   Last Modified: $Date: 1997/01/26 19:29:57 $   
;+
; NAME: GETSIGNAHEADER.PRO
;
; PURPOSE: Reads the header portion of a signa image
;
; CATEGORY: DATA I/O
;
; CALLING SEQUENCE: result = getsignaheader(path, filename)
; 
; INPUTS: 
;   path     : The path to the file
;   filename : The name of the file
;
; OPTIONAL INPUTS:
;   none
;	
; KEYWORD PARAMETERS:
;   getraw = return the raw header. Used in case user wants additional
;            information not contained in the data_info structure
;            returned. 
;
; OUTPUTS:
;   result is a structure containing the following info about the
;   image.  If GETRAW is set then the raw image header is returned.
;
; OPTIONAL OUTPUTS:
;   none
;
; COMMON BLOCKS:
;   none
;
; SIDE EFFECTS:
;   none
;
; RESTRICTIONS:
;   Only used with Signa 5.x data
;
; PROCEDURE:
;   Straightforward
;
; EXAMPLE:
;   Straightforward
;
;
; MODIFICATION HISTORY:
;    $Log: getsignaheader.pro,v $
;    Revision 2.0  1997/01/26  19:29:57  williams
;    New version.
;
;
;-
function getsignaheader, path, filename, getraw = getraw

cd, path, current = home

get_lun,head_lun
openr,head_lun,filename
junk = fstat(head_lun)
data = bytarr(junk.size)
readu,head_lun,data
free_lun,head_lun

header = bytarr(156)
header(0:155) = data(0:155)

offset = long(header,4)
cols = long(header,8)
rows = long(header,12)
junk1 = long(header,16)

if keyword_set(getraw) then return, data(0:offset-1)

;--- Why I want to be consistent w/ bruker is beyond my comprehension
case junk1 of
    long(1)  : datatype = '_1BIT'
    long(8)  : datatype = '_8BIT_SGN_INT'
    long(16) : datatype = '_16BIT_SGN_INT'
    long(24) : datatype = '_24BIT_SGN_INT'
endcase

ImgHstoff      = long(header,80)
ImgHstlen      = long(header,84)
SuiteHeadoff   = long(header,124)
SuiteHeadlen   = long(header,128)
ExamHeadoff    = long(header,132)
ExamHeadlen    = long(header,136)
SeriesHeadoff  = long(header,140)
SeriesHeadlen  = long(header,136)
ImHeadoff      = long(header,148)
ImHeadlen      = long(header,152)

imheader = bytarr(imheadlen)
imheader(0:imheadlen-1) = data(imheadoff:imheadoff+imheadlen-1)
examheader = bytarr(examheadlen)
examheader(0:examheadlen-1) = data(examheadoff:examheadoff+examheadlen-1)

slthick = float(imheader,26)
fovx = float(imheader,34)
fovy = float(imheader,38)

;--- divide by 1000 to go from usec to msec
imagetime = float(imheader, 22) / 1e6
tr = long(imheader,194) / 1000.
te = long(imheader,202) / 1000.
navgs = float(imheader,218)

;--- get stuff from the examheader
name = string(examheader(97:97+25))
study_number = fix(examheader, 8)
pat_id       = string(examheader(84:84+13))
ageN         = fix(examheader, 122)
ageNotation  = fix(examheader, 124)
case ageNotation of
    0 : age = strcompress(ageN,/remove_all) + ' years'
    1 : age = strcompress(ageN,/remove_all) + ' months'
    2 : age = strcompress(ageN,/remove_all) + ' days'
    else : age = strcompress(ageN, /remove_all)
endcase
sexN         = fix(examheader, 126)
case sexN of
    1: sex = 'male'
    2: sex = 'female'
    else : sex = 'unknown'
endcase
foo       = long(examheader, 128) / 1000.0
mass = strcompress(foo, /remove_all) + ' kg'
refPhysician = string(examheader(212:212+33))

pxlsz = [fovx, fovy, slthick] / [cols, rows, 1] 
scantime = 0.0

info = { DATA_INFO, $
         type          : 'GE', $ ; GE or Bruker?
         path          : path,$ ; database path
         size          : junk.size, $ ; datafile size
         offset        : offset, $ ; offset to the data
         datatype      : datatype, $ ; Short, long, etc.
         te            : te, $  ; Echo Time
         tr            : tr, $  ; Rep Time
         matrix        : [cols, rows, 0], $ ; get slices from readsigna
         fov           : [fovx, fovy, slthick], $
         pxlsz         : pxlsz, $
         scantime      : scantime, $ ; Time for one scan
         imagetime     : imagetime, $ ; Time for entire dataset
         name          : name, $
         study_number  : study_number, $
         pat_id        : pat_id, $
         age           : age, $
         sex           : sex, $
         mass          : mass}

cd, home
return,info
end

