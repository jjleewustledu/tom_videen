

function quick_crop,image

s = size(image)
xsize = s(1)
ysize = s(2)
cimage = bytscl(image,top = 100)
cimage(0:xsize/4,*,*) = 0
cimage(xsize - xsize/4:*,*,*) = 0
cimage(*,0:ysize/4,*) = 0
cimage(*,ysize - ysize/4:*,*) = 0
k = histogram(cimage)
k(0:10) = 0
kd = deriv(smooth(k,10))
tm = min(kd,pos)
kd(0:pos) = -1
temp = min(kd,pos)
cutoff = pos + 1
temp = where(cimage lt pos)
cimage(temp) = 0	
return,smooth(cimage,3)
end


pro scalp_mra,filename = filename

;----------------------------------------------------
;filename = mra.raw.loc file
;This program will create a cropped smoothed mra loc
;----------------------------------------------------

if not(keyword_set(filename)) then filename = pickfile(title = 'Choose MRA_loc to repair')
loadfmrilocsa,locs,lc,flnm = filename
extract_pname,filename,pn
extract_fname,filename,fn
fnbase = strmid(fn,0,strlen(fn) - 4)
scalpname = pn + fnbase + '.edited.loc'
sz = size(locs)
xsize = sz(1)
ysize = sz(2)
nlocs = sz(3)
rh = read_genheader(filename = filename)
outstack = quick_crop(locs)
outfilename = scalpname
outcoords = lc

openw,/swap_if_little_endian,outunit,outfilename,/get_lun
writeu,outunit,rh
close_lun,outunit
openu,/swap_if_little_endian,outunit,outfilename,/get_lun
a=assoc(outunit,intarr(1),7900)
a(0)=replicate(nlocs,1)
a=assoc(outunit,fltarr(3,3,nlocs),7900+2)
a(0)=outcoords
a=assoc(outunit,intarr(xsize,ysize,nlocs),7900+2+4*9*nlocs)
a(0) = outstack
close_lun,outunit
end










pro fix_mra,filename = filename

if not(keyword_set(filename)) then filename = pickfile(title = 'Choose MRA_loc to repair')
loadfmrilocsa,locs,lc,flnm = filename
mips = locs(*,*,0:19)
rawstack = locs(*,*,20:*)
extract_pname,filename,pn
extract_fname,filename,fn
fnbase = strmid(fn,0,strlen(fn) - 4)
mipname = pn + fnbase + '.mip.loc'
rawname = pn + fnbase + '.raw.loc'
scalpname = pn + fnbase + '.edited.loc'
rawcoords = lc(*,*,20:*)
sz = size(locs)
xsize = sz(1)
ysize = sz(2)
rawlocs = sz(3) - 20
rh = read_genheader(filename = filename)


outfilename = rawname
outstack = rawstack
outcoords = rawcoords
nlocs = rawlocs
for i = 0,2 do begin
	if i eq 1 then begin
		outfilename = scalpname
		outstack = quick_crop(rawstack)
	endif
	if i eq 2 then begin
		nlocs = 20
		outfilename = mipname
		outstack = mips
		outcoords = lc(*,*,0:19)
	endif
		
	openw,/swap_if_little_endian,outunit,outfilename,/get_lun
	writeu,outunit,rh
	close_lun,outunit
	openu,/swap_if_little_endian,outunit,outfilename,/get_lun
	a=assoc(outunit,intarr(1),7900)
	a(0)=replicate(nlocs,1)
	a=assoc(outunit,fltarr(3,3,nlocs),7900+2)
	a(0)=outcoords
	a=assoc(outunit,intarr(xsize,ysize,nlocs),7900+2+4*9*nlocs)
	a(0) = outstack
	close_lun,outunit
endfor

spawn,'rm ' + filename

end


