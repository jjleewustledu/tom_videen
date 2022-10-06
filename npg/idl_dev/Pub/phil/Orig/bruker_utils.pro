;            File: $RCSfile: bruker_utils.pro,v $
;          Author: $Author: williams $
;         version: $Revision: 1.3 $
;   Last Modified: $Date: 1997/01/24 14:42:56 $
pro readreco, path, rows, cols, slices, dim, wordtype, fovx, fovy, fovz, $
              id = id
;+
; NAME: READRECO
;
; PURPOSE: Get info out of the reco file from a Bruker Biospec MRI Scanner
;
; CATEGORY: Bruker Utilities
;
; CALLING SEQUENCE: 
;   readreco, path[,rows, cols, slices, dim, wordtype, fovx, fovy,
;             fovz, id = id]
; 
; INPUTS: 
;   path : path to the reco file
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS:
;    id : the widget id of a text widget for info
;
; OUTPUTS: none
;
; OPTIONAL OUTPUTS:
;             rows : # of rows
;             cols : # of cols
;           slices : # of slices
;              dim : 2D or 3D
;         wordtype : Type of data
; fovx, fovy, fovz : Field of view
;
; COMMON BLOCKS: none
;
; MODIFICATION HISTORY:
;   17 Sep 96 Initial Coding. PMW
;-

a = ""
b = ""
rows = 0
cols = 0
slices = 0

;open the reco file
;note path needs to have the trailing /
file = path+"reco"

;  Test to see if file exists and is readable by user

spawn, ['/bin/sh', '-c', 'test -f '+file+' -a -r '+file+'; echo $?'], result, /noshell

IF fix(result(0)) EQ 1 THEN BEGIN
    IF n_elements(id) EQ 0 THEN $
      message,"RECO file is missing... Can't read 2dseq file without it." $
    ELSE $
      widget_control, id, $
      set_value = "RECO file is missing..."
    return
END

get_lun,reco_lun
openr,reco_lun,file

;find the reco size
while ((not eof(reco_lun)) and (strpos(a,"##$RECO_size") NE 0)) do begin
	readf,reco_lun,a
endwhile
if eof(reco_lun) then begin point_lun,reco_lun,0
endif else begin
    temp = strpos(a,"( ")
    dim = strmid(a,temp+2,1)
    readf,reco_lun,b
    case dim of
        '2': reads,b,cols,rows
        
        '3': reads,b,cols,rows,slices
    endcase
endelse

while ((not eof(reco_lun)) and (strpos(a, "##$RECO_transpose_dim") ne 0)) do begin
    readf,reco_lun,a
endwhile
if eof(reco_lun) then begin 
    point_lun,reco_lun,0
    trans = 0
endif else begin
    temp = strpos(a,'=')
    trans = fix(strmid(a,temp+1,strlen(a)-1))
endelse

;what kind of data is it?
while ((not eof(reco_lun)) and (strpos(a, "##$RECO_wordtype") NE 0)) do begin
	readf,reco_lun,a
endwhile
if eof(reco_lun) then begin point_lun,reco_lun,0
endif else begin
    temp = strpos(a,"=")
    wordtype = strmid(a,temp+1,strlen(a)-1)
endelse

while ((not eof(reco_lun)) and (strpos(a,"##$RECO_fov") NE 0)) do begin
	readf,reco_lun,a
endwhile
if eof(reco_lun) then begin point_lun,reco_lun,0
endif else begin
    readf,reco_lun,b
    case dim of
        '2' : reads,b,fovx,fovy ;if 2d data have to get slice thickness
                                ;from the imnd
        '3' : reads,b,fovx,fovy,fovz
    endcase
endelse

if trans then begin
    temp = rows
    rows = cols
    cols = temp
endif

free_lun,reco_lun
end

;---------------------------------------------------------------------------------
pro readimnd, path, slices, tr, te, navgs, slice_thick, nexpts, scantime, $
              id = id

;+
; NAME: READIMND
;
; PURPOSE: Get info out of the imnd file from a Bruker Biospec MRI Scanner
;
; CATEGORY: Bruker Utilities
;
; CALLING SEQUENCE: 
;   readimnd, path[, slices, tr, te, navgs, slice_thick, nexpts, scantime,
;                  id = id]
; 
; INPUTS: 
;   path : path to the reco file
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS:
;    id : the widget id of a text widget for info
;
; OUTPUTS: none
;
; OPTIONAL OUTPUTS:
;           slices : # of slices
;               tr : Rep Time
;               te : Echo Time
;            navgs : # of averages
;      slice_thick : slice thickness
;           nexpts : # fo experiments
;         scantime : Time for 1 scan
;
; COMMON BLOCKS: none
;
; MODIFICATION HISTORY:
;   17 Sep 96 Initial Coding. PMW
;-
a=""
b=""
file = path+"imnd"

;  Test to see if file exists and is readable by user

spawn, ['/bin/sh', '-c', 'test -f '+file+' -a -r '+file+'; echo $?'], result, /noshell

IF fix(result(0)) EQ 1 THEN BEGIN
    IF n_elements(id) EQ 0 THEN $
      message,'File does not exist or you do not have the correct permissions. Try again.' $
    ELSE $
      widget_control, id, $
      set_value = "IMND file is missing... "
    return
END

get_lun,imnd_lun
openr,imnd_lun,file

; I believe that this is the only string that is not present in all
; imnd's however, adding eof check to all...

while ((not eof(imnd_lun)) and (strpos(a,"##$IMND_n_expts") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin point_lun,imnd_lun,0
endif else begin
    temp = strpos(a,"=")
    nexpts=fix(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(imnd_lun)) and (strpos(a,"##$IMND_recov_time") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin ; maybe I should find ANG_TR?
    point_lun,imnd_lun,0
    while ((not eof(imnd_lun)) and $
           (strpos(a,"##$ANG_TR") NE 0)) do begin
        readf,imnd_lun,a
    endwhile
    if eof(imnd_lun) then begin ; didn't find any tr values in IMND!
        tr = 0.00
    endif else begin
        temp = strpos(a,"=")
        tr=float(strmid(a,temp+1,strlen(a)-1))
    endelse
endif else begin
    temp = strpos(a,"=")
    tr=float(strmid(a,temp+1,strlen(a)-1))
endelse

; imnd_recov_time and ang_tr are in different places in the imnd so put
; this here to start the file search from begining.
point_lun,imnd_lun,0  

while ((not eof(imnd_lun)) and (strpos(a, "##$IMND_echo_time") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin point_lun,imnd_lun,0
endif else begin
    temp = strpos(a,"=")
    te=float(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(imnd_lun)) and (strpos(a, "##$IMND_n_averages") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin point_lun,imnd_lun,0
endif else begin
    temp = strpos(a,"=")
    navgs=float(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(imnd_lun)) and (strpos(a, "##$IMND_n_slices") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin point_lun,imnd_lun,0
endif else begin
    temp = strpos(a,"=")
    slices=fix(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(imnd_lun)) and (strpos(a, "##$IMND_slice_thick") NE 0)) do begin
    readf,imnd_lun,a
endwhile
if eof(imnd_lun) then begin point_lun,imnd_lun,0
endif else begin
    temp = strpos(a,"=")
    slice_thick=float(strmid(a,temp+1,strlen(a)-1))
endelse

free_lun,imnd_lun
end

;---------------------------------------------------------------------------

pro readacqp, path, slices, tr, te, slice_thick, scantime, $
              id = id


;+
; NAME: READACQP
;
; PURPOSE: Get info out of the acqp file from a Bruker Biospec MRI Scanner
;
; CATEGORY: Bruker Utilities
;
; CALLING SEQUENCE: 
;   readacqp, path[, slices, tr, te, navgs, slice_thick, scantime,
;                  id = id]
; 
; INPUTS: 
;   path : path to the reco file
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS:
;    id : the widget id of a text widget for info
;
; OUTPUTS: none
;
; OPTIONAL OUTPUTS:
;           slices : # of slices
;               tr : Rep Time
;               te : Echo Time
;            navgs : # of averages
;      slice_thick : slice thickness
;         scantime : Time for 1 scan
;
; COMMON BLOCKS: none
;
; MODIFICATION HISTORY:
;   17 Sep 96 Initial Coding. PMW
;-
a=""
b=""
file = path+"acqp"

;  Test to see if file exists and is readable by user

spawn, ['/bin/sh', '-c', 'test -f '+file+' -a -r '+file+'; echo $?'], result, /noshell

IF fix(result(0)) EQ 1 THEN BEGIN
    IF n_elements(id) EQ 0 THEN $
      message,'File does not exist or you do not have the correct permissions. Try again.' $
    ELSE $
      widget_control, id, $
      set_value = "ACQP file is missing... "
    return
END

get_lun,acqp_lun
openr,acqp_lun,file

x = 0
y = 0
b = ''
while ((not eof(acqp_lun)) and (strpos(a,"##$ACQ_size") NE 0)) do begin
    readf,acqp_lun,a
endwhile
if eof(acqp_lun) then begin point_lun,acqp_lun,0
endif else begin
    readf, acqp_lun, b
    reads, b, x, y
endelse

while ((not eof(acqp_lun)) and (strpos(a,"##$NR") NE 0)) do begin
    readf,acqp_lun,a
endwhile
if eof(acqp_lun) then begin point_lun,acqp_lun,0
endif else begin
    temp = strpos(a,"=")
    slices = fix(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(acqp_lun)) and (strpos(a, "##$ACQ_slice_thick") NE 0)) do begin
    readf,acqp_lun,a
endwhile
if eof(acqp_lun) then begin point_lun,acqp_lun,0
endif else begin
    temp = strpos(a,"=")
    slice_thick=float(strmid(a,temp+1,strlen(a)-1))
endelse

while ((not eof(acqp_lun)) and $
       (strpos(a, "##$ACQ_inter_echo_time") NE 0)) do begin
    readf,acqp_lun,a
endwhile
if eof(acqp_lun) then begin point_lun,acqp_lun,0
endif else begin
    readf, acqp_lun, a
    te=float(a)
endelse

while ((not eof(acqp_lun)) and (strpos(a,"##$ACQ_repetition_time") NE 0)) do begin
    readf,acqp_lun,a
endwhile
if eof(acqp_lun) then begin point_lun,acqp_lun,0
endif else begin
    readf, acqp_lun, a
    tr=float(a)
endelse

free_lun,acqp_lun
scantime = (x/2.0)*tr*1000.
end

;---------------------------------------------------------------------------------
function readbruker, file_path, $
                     data_info = data_info, $
                     preview = preview, $
                     id = id


;+
; NAME: READBRUKER
;
; PURPOSE: Read a 2dseq file from a Bruker Biospec MRI scanner.
;
; CATEGORY: Bruker Utilities
;
; CALLING SEQUENCE: 
;   image = readbruker(file_path[, data_info = data_info, preview = preview,
;               id = id])
; 
; INPUTS: 
;   file_path : path to the 2dseq file
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS:
; data_info : named var to return info structure
;        id : the widget id of a text widget for info
;   preview : for widget support to load 1 image to preview
;
; OUTPUTS: 
;   a 2D or 3D array depending on 2dseq
;
; OPTIONAL OUTPUTS: none
;
; COMMON BLOCKS: none
;
; RESTRICTIONS:
;    All complimentary files should be in the correct
;    place. i.e. IMND, ACQP, RECO, etc.
;
; MODIFICATION HISTORY:
;   17 Sep 96 Initial Coding. PMW
;-
;make sure there is a trailing '/' at the end of the path.  if not add one.
trailing = strmid(file_path,strlen(file_path)-1,1)
IF (trailing NE '/') THEN $
  file_path = file_path+'/'

; - make sure that file_path is a real directory and user has
;   executable access. If the test is tru then result returns '0' so
;   have to fix and not it

spawn, ['/bin/sh', '-c', 'test -d '+file_path+' -a -x '+file_path+' ; echo $?'], result, /noshell

IF fix(result(0)) EQ 1 THEN BEGIN
    IF n_elements(id) EQ 0 THEN $
      message,file_path+' is an invalid directory. Try again.' $
    ELSE $
      widget_control, id, set_value = 'Dir is invalid.'
    return,-1
END

; data file is always 2dseq
file = file_path+'2dseq'

;  Test to see if file exists and is readable by user

spawn, ['/bin/sh', '-c', 'test -f '+file+' -a -r '+file+'; echo $?'], result, /noshell

IF fix(result(0)) EQ 1 THEN BEGIN
    IF n_elements(id) EQ 0 THEN $
      message,'File does not exist or you do not have the correct permissions. Try again.' $
    ELSE $
      widget_control, id, set_value = 'No 2dseq in this directory...'
    return,-1
END

print,'Reading...'
get_lun,data_lun
openr,data_lun,file
fileinfo = fstat(data_lun)
data = bytarr(fileinfo.size)
readu,data_lun,data
free_lun,data_lun

; Get the info for this 2dseq file from the reco and imnd files
print,'Reading reco...'
readreco, file_path, rows, cols, slices, dim, datatype, fovx, fovy, fovz, $
  id = id
if n_elements(datatype) eq 0 then return, -1

if not keyword_set(preview) then begin
; the path to the imnd is different 2 levels up from the 2dseq
    imndpath = strmid(file_path,0,strpos(file_path,"pdata"))
print,'Reading acqp...'
    readacqp, imndpath, acqpslices, tr, te, slice_thick, scantime, $
      id = id
    if n_elements(tr) eq 0 then return, -1
; if this was a 2d data set get the slices from the imnd
    case dim of
        '2' : begin
            fovz = slice_thick
            slices = acqpslices
            if n_elements(nexpts) then slices = imndslices*nexpts
        end
; can't think of anyhting that needs to be done if 3d sequence
        '3' : begin
        end
        else:
    endcase

;### Must add imagetime
    imagetime = 0.0
    pxlsz =  [fovx, fovy, fovz] / [cols, rows, slices]
; data_info is a structure that contains pertanent info about the data, duhhhh
    data_info = { DATA_INFO, $
                  type          : 'BR', $  ; GE or Bruker?
                  path          : file_path, $
                  size          : fileinfo.size, $
                  offset        : 0, $  ;want to be consistent w/ signa
                  datatype      : datatype, $
                  TE            : te, $
                  TR            : tr, $
                  MATRIX        : [cols, rows, slices], $
                  FOV           : [fovx, fovy, fovz], $
                  pxlsz         : pxlsz, $
                  scantime      : scantime, $ ; Time for one scan
                  imagetime     : imagetime, $ ; Time for entire dataset
                  name          :'Bruker Data', $
                  study_number  : 0L, $
                  pat_id        : '', $
                  age           : '', $
                  sex           : '', $
                  mass          : ''}
    
endif else begin                ; load only one preview image
    slices = 1
    fovz = 1
endelse

case datatype of 
    '_32BIT_SGN_INT': stack = long(data,0,cols,rows,slices)
    '_16BIT_SGN_INT': stack = fix(data,0,cols,rows,slices)
    '_8BIT_SGN_INT' : stack = byte(data,0,cols,rows,slices)
endcase

;--- The data is read in transposed.  So transpose and rotate it to
;    put it right side up.

for i = 0,slices-1 do begin
    stack(*,*,i) = rotate(temporary(stack(*,*,i)),7)
endfor

if keyword_set(preview) then begin
    scale_r = rows/128
    scale_c = cols/128
    stack = rebin(temporary(stack),cols/scale_c,rows/scale_r)
endif

return,stack
end
