function fileExists, file, path, $
                     FOLLOW_DIR = FOLLOW_DIR, $
                     GET_PATH = GET_PATH
;+
; NAME: FILEEXISTS.PRO
;
; PURPOSE: Determines the existence of a file.
;
; CATEGORY: File I/O
;
; CALLING SEQUENCE: result = fileExists(file[, path, /follow_dir, /get_path])
; 
; INPUTS:
;    file : a string containing the file name
;
; OPTIONAL INPUTS:
;   path : The given path to search. If not given then searching
;          begins at the current directory
;	
; KEYWORD PARAMETERS:
;  FOLLOW_DIR : If set then subdirectories are recursively searched.
;  GET_PATH   : If set and file exists then the path to the file is
;               returned.
; OUTPUTS:
;   If GET_PATH is specified and the search was successful then the
;   path to the file is returned. Otherwise 1 is returned if the file
;   exists and 0 if the file does not.
;
; OPTIONAL OUTPUTS: none
;
; COMMON BLOCKS: none.
;
; SIDE EFFECTS: none.
;
; RESTRICTIONS: none.
;
; PROCEDURE:
;    IDL> t = fileExists('file.pro',/follow,/get)
;
; MODIFICATION HISTORY:
;    13 Dec 96 Initial Coding. PMW
;    29 Dec 96 Added FOLLOW_DIR keyword for recursive searches
;    30 Dec 96 Added GET_PATH keyword to return path if file is found.
;-

followFlag = keyword_set(follow_dir)
getFlag    = keyword_set(get_path)

if n_elements(path) ne 0 then begin
    mypath = path
    pushd, mypath
endif
cd, current = home

list = findfile()

dirs = direxist(list)
foundDirs  = where(dirs eq 1, dircount)
foundFiles = where(dirs eq 0, filecount)

if filecount eq 0 then filelist = '' else filelist = list(foundFiles)
if dircount  eq 0 then dirlist  = '' else dirlist  = list(foundDirs)

if filecount ne 0 then begin
    index = where(filelist eq file, result)
    if result then goto, done
endif

;---- This part doesn't work just yet -------------------
;--- Haven't found anything search subdirectories?
result = 0
if followflag then begin
    if ((dircount ne 0) and (dirlist(0) ne '')) then begin
        for i = 0, dircount-1 do begin
            result = fileExists(file, dirlist(i), /follow, get_path = getFlag)
            if strtrim(result,2) ne '0' then goto,done2
        endfor
    endif
endif
done:
cd, current = newDir

done2:
;--- If a dir was pushd then popd it
if n_elements(mypath) ne 0 then begin
    popd
endif

if strtrim(result,2) ne '0' then begin
    if n_elements(newDir) eq 0 then newDir = strtrim(result,2)
    if getFlag then begin
        return, newDir
    endif else return, 1
endif else return, 0

end
















