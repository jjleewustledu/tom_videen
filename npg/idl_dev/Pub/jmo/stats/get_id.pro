;**************************
function get_id,dsp,cstm,pr
;**************************

;Purpose: Get id of any currently displayed window.

nwin = dsp.nwindows + cstm.nwindows + pr.nwindows + 1
if(nwin le 0) then begin
    return,-1
endif
labels = strarr(nwin)
ids = intarr(nwin)
ids(0) = dsp.image_index
labels(0) = string('Working window')
j = 1
for i=0,dsp.nwindows-1 do begin
    if(dsp.id(i) gt 0) then begin
	ids(j) = dsp.id(i)
        labels(j) = string(i,FORMAT='("Display all #",i3)')
        j = j + 1
    endif
end
for i=0,cstm.nwindows-1 do begin
    if(cstm.id(i) gt 0) then begin
	ids(j) = cstm.id(i)
        labels(j) = string(i,FORMAT='("Custom Display #",i3)')
        j = j + 1
    endif
end
for i=0,pr.nwindows-1 do begin
    if(pr.id(i) gt 0) then begin
	ids(j) = pr.id(i)
        labels(j)=string(i,FORMAT='("Profile #",i3)')
        j = j + 1
    endif
end
if(j le 0) then begin
    return,-1
endif
lab1 = strarr(j)
lab1 = labels(0:j-1)
index = get_button(lab1)
id = ids(index)

return,id
end
