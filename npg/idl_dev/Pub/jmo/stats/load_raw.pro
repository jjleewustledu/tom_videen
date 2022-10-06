;*************************************************************************************
function load_raw,fi,st,dsp,wd,stc,img_type,help,filnam,xdim,ydim,zdim,tdim,num_type,dxdy,dz, $
    scl,type,paradigm,img
;*************************************************************************************

; Load unformatted data.

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Loads a single XYZ frame of data using associated variables.')
endif
file = pickfile(/read,FILTER='*',PATH=fi.path(0), $
                            GET_PATH=fi.path,/NOCONFIRM)
if(file eq '') then return,!ERROR
if(n_elements(file) gt 1) then $
    filnam = file(0)+file(1) $
else $
    filnam = file

;   Get number format.
labels = strarr(3)
labels(0) = string('Byte')
labels(1) = string('Short integer')
labels(2) = string('Floating point')
index = get_button(labels)
case index of
    0: num_type = !BYTE
    1: num_type = !SHORT
    2: num_type = !FLOAT
endcase

;Get file size, prompt for x, y dimensions, calcuate z and prompt.
cmd = string('ls -l ',filnam," | gawk '{print $5}'")
spawn,cmd,csize
size = long(csize)
value = strarr(4)
labels = strarr(4)
value(0) = string('64')
value(1) = string('64')
value(2) = string('17')
value(3) = string('0')
labels(0) = string("X dimension: ")
labels(1) = string("Y dimension: ")
labels(2) = string("Z dimension: ")
labels(3) = string("Offset (bytes): ") ; Offset into file in bytes.
dims = get_str(4,labels,value)
xdim = long(dims(0))
ydim = long(dims(1))
zdim = long(dims(2))
offset = long(dims(3))
labels(0) = string("Time dimension: ")
labels(1) = string('XY pixel size')
labels(2) = string('Plane thickness')
value(0) = size/(xdim*ydim*zdim*num_type)
value(1) = string('1')
value(2) = string('1')
dims = get_str(3,labels,value)
tdim = fix(dims(0))
dxdy = float(dims(1))
dz   = float(dims(2))
type = !FLOAT_ARRAY
fi.n = fi.nfiles
get_lun,lun
fi.lun(fi.n) = lun
openr,lun,filnam
case num_type of
    !FLOAT: img = assoc(lun,fltarr(xdim,ydim,/NOZERO),offset)
    !SHORT: img = assoc(lun,intarr(xdim,ydim,/NOZERO),offset)
    !BYTE:  img = assoc(lun,bytarr(xdim,ydim,/NOZERO),offset)
endcase

paradigm = fltarr(tdim)

return,!OK
end
