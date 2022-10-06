FUNCTION readsigna, path, data_info = data_info
;+
; NAME:  readsigna
;
; PURPOSE: 
;       To read in a stack of Genesis data saved in ginx format
;
; CATEGORY: 
;       File I/O
;
; CALLING SEQUENCE: 
;       Result = readsigna(path[, data_info = data_info])
;
; INPUTS:
;       path - The path for the image files.  Readimg assumes this is a path
;              off the home directory and uses the Unix environment $HOME
;              variable
;       prefix - The prefix for the image files.  Readimg gets the file name
;                by taking the prefix and appending the present file # and 
;                '.MR'.  Presently it only handles files from ximg
;       first - The first file # to read.
;       last - The last file # to read.
;
; OPTIONAL INPUTS:
;               None.
;
; KEYWORD PARAMETERS:
;       id - This is for compatiblilty with widgets. If it is set then messages
;            are sent to the widget id instead of the stdio.
;       ginx - flag to set to one if the images were transferred using the ginxtool
;
; OUTPUTS:
;       Result - A 3D array of the images. 
;
; OPTIONAL OUTPUTS:
;               None.
;
; COMMON BLOCKS:
;               None.
;
; SIDE EFFECTS:
;               None.
;
; RESTRICTIONS:
;             The image path must be a path off the home directory.
;
; PROCEDURE:
;            Readimg reads in the first file and gets the x and y resolution
;            from it.  There for all files should have the same resolution.
;            It also assumes that the data are all 2 byte integers.
;
; EXAMPLE:
;      IDL> axial = readsigna('/GEDB/GINX/GENESIS/MRS/5555/555/001')
;
; MODIFICATION HISTORY:
;       Jan 7 96 Version 1.0 Initial Coding PMW
;-

;--- Set up error handler
CATCH, error
if error ne 0 then begin
    print,!err_string
    return, -1
endif

;--- Check that it was called properly
if n_params() eq 0 then message,'Usage: Result = readsigna(path[, data_info = data_info])'

;--- make sure the dir exists
exist = direxist(path)
if not(exist(0)) then message,'Invalid directory: ' + path

cd, path, current = home
filter = 'I.*'
fileList = findfile(filter, count = count)

if count eq 0 then message, 'No image files found in: ' + path

;--- Get the data_info structure
info = getsignaheader(path, fileList(0))
info.matrix(2) = count
cols = info.matrix(0)
rows = info.matrix(1)
slices = info.matrix(2)

im = intarr(cols,rows)
data = intarr(cols,rows,slices)
raw = bytarr(info.size)
for i = 0,slices-1 do begin
    print,'Reading file '+filelist(i)
    get_lun,signa_lun
    openr,signa_lun,path+filelist(i)
    readu,signa_lun,raw
    free_lun,signa_lun
    im=fix(raw,info.offset,cols,rows)

;--- rotate the images to correct orientation
    im=rotate(temporary(im),7)
    data(*,*,i)=im(*,*)
endfor

cd, home
return, data
end


