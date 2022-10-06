pro view_movie,dim

if (n_elements(dim) eq 0) then dim=256 	; default size
data_dir = '/topo6b/mich/data/cti/nmr'
cd, data_dir, current=curdir
file = pickfile()
cd, curdir
openr,1,file
b = fstat(1)					;Get size
numframes = b.size /(long(dim)*long(dim))	;# of frames
image=assoc(1,bytarr(dim,dim))
xinteranimate,set=[dim,dim,numframes],title=file,/showload
loadct,15
for f=0,numframes-1 do begin
 tv,image(f)
 xinteranimate,frame=f,image=image(f),window=!d.window
endfor
close,1
xinteranimate
end
