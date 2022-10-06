;*******************************
function read_4dfp_rec,file_name
;*******************************

;Purpose: Read interfile header.
; John Ollinger, 10/16/95

rec = {recfile_Header, $
	bitsperpix:0,	$
	xdim:0,	$
	ydim:0,	$
	zdim:0,	$
	tdim:0,	$
	xpix:0.,$
	ypix:0.,$
	zpix:0.	$
	}

s = string(" ")
get_lun,lu
openr,lu,file_name

eof = 0
while (eof eq 0) do begin
    readf,lu,s,FORMAT='(a)'
    if(strpos(s,"bitsperpix") ge 0) then $
	rec.bitsperpix = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"xdim") ge 0) then $
	rec.xdim = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"ydim") ge 0) then $
	rec.ydim = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"zdim") ge 0) then $
	rec.zdim = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"vdim") ge 0) then $
	rec.tdim = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"tdim") ge 0) then $
	rec.tdim = fix(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"xpix") ge 0) then $
	rec.xpix = float(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"ypix") ge 0) then $
	rec.ypix = float(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    if(strpos(s,"zpix") ge 0) then $
	rec.zpix = float(strcompress(strmid(s,strpos(s,":")+1,strlen(s)),/REMOVE_ALL))
    eof = eof(lu)
endwhile

free_lun,lu

return,rec
end
