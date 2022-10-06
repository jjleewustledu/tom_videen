function resampleCT,file1 = file1,file2 = file2,thick= thick
;---------------------------------------------------------------------------------
;This program will create a single locfile from 2 CT series
;(ie an infratentorial and a supratentorial series)
;The locfile is resampled to 5 mm thick slices in a contiguous 256x256xzsize volume
;zsize is computed based on slice thickness and 15 cm coverage
;---------------------------------------------------------------------------------


if not(keyword_set(start)) then start = [0.0,0,-60]
if not(keyword_set(step)) then step = [2.0,2,1]
outmatrix = [256,256,40]
if not(keyword_set(thick)) then thick = 5.0
newthick = thick
outmatrix(2) = 150/newthick
fh1 = read_header(filename = file1,get_cube = supra_cube)
fh2 = read_header(filename = file2,get_cube = infra_cube)
ih = fh2
sh = fh1
infra = file2
supra = file1

if fh1.magnet_transform(3,2) lt fh2.magnet_transform(3,2) then begin
	ih = fh1
	sh = fh2
	infra = file1
	supra = file2
	cube1 = supra_cube
	supra_cube = infra_cube
	infra_cube = temporary(cube1)
endif
slthick = sh.an.z_size
;-------------------------------------------------
;Start new volume 6 cm below supratentorial cube)
;-------------------------------------------------
start(2) = -(60.0/slthick)
step(2) = 1.0*newthick/slthick

hstep = step
tm = find_object2target(object = infra,target = supra)
unity = fltarr(4,4)
for i = 0,3 do unity(i,i) = 1
infrav = tran3d(volumein = infra_cube,outmatrix = outmatrix,matrix = tm, $
	step = step, start = start)
step = hstep
suprav = tran3d(volumein = supra_cube,outmatrix = outmatrix,matrix = unity, $
	step = step, start = start)
step = hstep
k = compute_tran_total(matrix = tm,start = start,step = step)
lastslice = transpose([0,0,ih.z - 1,1]) # k
lastslice = round(lastslice(2))
done = 0
lslice = lastslice
;-------------------------------------------
;Determine interface between the 2 volumes
;------------------------------------------
for i = (lastslice - 1),(lastslice + 2) do begin
	if total(infrav(*,*,i)>0) lt total(suprav(*,*,i)>0) then begin
		if not(done) then lslice = i
		done = 1
	endif
endfor
lcube = infrav
for i = lslice,outmatrix(2) - 1 do lcube(*,*,i) = suprav(*,*,i)
;imviewer,images = lcube,/scale,title = 'Combined'
;-----------------------------------------------------
;Correct the header fields in the resampled volume
;-----------------------------------------------------
newmt = invert(k) # ih.magnet_transform
lh = sh
lh.magnet_transform = newmt
lh.x = outmatrix(0)
lh.y = outmatrix(1)
lh.z = outmatrix(2)
lh.an.x_dim = lh.x
lh.an.y_dim = lh.y
lh.an.z_dim = lh.z
lh.an.x_size = sh.an.x_size * step(0)
lh.an.y_size = sh.an.y_size * step(1)
lh.an.z_size = sh.an.z_size * step(2)
extract_pname,supra,p
extract_fname,supra,f
parse_filename,filename = supra,basename = basename,path = p
outfile = p + 'rv_' + basename + '.locs'
write_header,filename = outfile,header = lh,cube = fix(lcube)


;reply = {infrav:infrav,suprav:suprav,lcube:lcube,tm:tm,start:start,$
;step:step,outmatrix:outmatrix,supra:supra,infra:infra,sh:sh,ih:ih,fname:outfile}

return,outfile

end
