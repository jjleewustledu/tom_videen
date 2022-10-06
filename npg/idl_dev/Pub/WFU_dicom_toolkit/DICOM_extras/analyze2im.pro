pro analyze2im,filename = filename,outfile = outfile,coords = coords,dicomfile = dicomfile,rotation = rotation,remove = remove
;-------------------------------------------------------------------------------
;Converts a 4D analyze file to a .im file
;Assumes it started as a dicomfile
;note that the coords matrix will not be filled unless a dicom file is provided
;Even then, it hasn't been tested
;I don't know how the correct raw header rotation matrices will affect the kluged rotation here
;---------------------------------------------------------------------------------
ah = read_analyze_header(filename = filename,get_cube = stack)
if not(keyword_set(rotation)) then rotation = 1
if not(keyword_set(outfile)) then outfile = strmid(filename,0,strlen(filename) - 1)
rh = mk_rawheader_lx()
rh.rec.rdbm_rev = 7.00
rh.exam.ex_magstrength = 1500
;-------------------------------------------
;get the x and y matrix size and the nex
;Load up some raw data header fields
;-------------------------------------------
rh.rec.nslices = ah.z_dim
rh.rec.frame_size = long(ah.x_dim)
rh.rec.nframes = long(ah.y_dim)
rh.rec.navs = ah.t_dim
rh.rec.reps = 1
rh.image.im_slquant = fix(ah.z_dim)

if keyword_set(dicomfile) then begin
	ih = read_dicomheader(filename = dicomfile,dh = dh,rh = gh,err = err,mosaic = mosaic)
	rh.series = gh.series
	rh.image = gh.image
	rh.exam = gh.exam
endif
for i = 0,ah.t_dim -1 do begin
	stack1=reform(stack(*,*,*,i))
	rotate_set,stack1,rotation
	stack(*,*,*,i) = stack1
endfor
write_im_file, hdr = rh, stack,outfile = outfile
if keyword_set(remove) then begin
	spawn,'rm ' + strmid(filename,0,strlen(filename) - 3) + 'hdr'
	spawn,'rm ' + filename
endif

end
