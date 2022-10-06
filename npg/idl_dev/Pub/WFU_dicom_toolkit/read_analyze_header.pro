function read_analyze_header, filename = filename,get_cube = cube,noswap = noswap,nospm = nospm,$
	labheader = labheader,datatype = datatype,scale = scale,volumein= volumein,slice = slice, time = time
;--------------------------------------------------------------------------
;Reads analyze files and returns modified header structure
;ah = read_analyze_header(filename = analyze_filename, get_cube = image)
;J. Maldjian
;---------------------------------------------------------------------------
swap_endian = 0

if not(keyword_set(filename)) then  filename = pickfile(/read,title = 'choose an analyze type file',filter = '*.hdr')
extract_fname,filename,fname
fcomp = strmid(fname,0,4)

;if not(var_set(nospm)) and ((fcomp eq 'spmF') or (fcomp eq 'spmT') or (fcomp eq 'spmZ') ) then begin
;	reply = read_spmheader99(filename = filename,get_cube = cube)
;	return,reply
;endif

s = strlen(filename)
headerfile =  strmid(filename,0,s - 4) + '.hdr'
aheader = make_analyze_header()			;construct analyze header
hoffset = aheader.sizeof_hdr
xsize = aheader.x_dim
ysize = aheader.y_dim
zsize = aheader.z_dim

openr,swap_endian = swap_endian,unit,headerfile,/get_lun,error=openerr			;open testfile
readu,unit,aheader
close_lun,unit

if aheader.sizeof_hdr ne 348 then begin
	swap_endian = abs(swap_endian - 1)
	openr,unit,/get_lun,headerfile,swap_endian = swap_endian
	aheader = make_analyze_header()
	readu,unit,aheader
	close_lun,unit
endif



s = strlen(filename)
imagefile = strmid(filename,0,s - 4) + '.img'
hoffset = aheader.sizeof_hdr
hoffset = 0
xsize = aheader.x_dim
ysize = aheader.y_dim
zsize = aheader.z_dim
tsize = aheader.t_dim

if var_set(slice) then begin
	if slice ge aheader.z_dim then begin
		errtxt = ' TOO MANY Z SLICES!'
		goto, ioerr
	endif
endif else $
	slice = indgen(aheader.z_dim)

if var_set(time) then begin
	if time ge aheader.t_dim then begin
		errtxt = ' TOO MANY TIME SLICES!'
		goto, ioerr
	endif
endif else $
	time = indgen(aheader.t_dim)


;read image data


datatype = aheader.datatype
openr,swap_endian = swap_endian,unit,imagefile,/get_lun,error=openerr
;if datatype eq 2 then a = assoc(unit,bytarr(xsize,ysize,zsize,tsize),hoffset)
;if datatype eq 4 then a = assoc(unit,intarr(xsize,ysize,zsize,tsize),hoffset)
;if datatype eq 16 then a = assoc(unit,fltarr(xsize,ysize,zsize,tsize),hoffset)
;if datatype eq 32 then a = assoc(unit,dblarr(xsize,ysize,zsize,tsize),hoffset)
img = assoc(unit, make_array(aheader.x_dim, aheader.y_dim, aheader.z_dim, aheader.t_dim, $
	type = idl_datatype(aheader.datatype)), hoffset)


cube = reform((img(0))(*,*,slice,time))
close_lun,unit
if aheader.scale lt 1d-50 then aheader.scale = 0
aheader.scale = not(float(aheader.scale)) + aheader.scale
;----------------------------------------------------------------------
;If the image has a scale factor then we must convert the intensities
;and update the data field types appropriately so idl to matlab conversions
;are handled properly, and set the scale back to 1.0 since we
;have converted it to float
;-----------------------------------------------------------------------
scale = 1
if aheader.scale ne 1 then begin
	scale = aheader.scale
	factor = 1
	if scale lt 1e-5 then begin
		scale = double(scale)
		factor = 2
	endif 
	cube = cube * scale
	aheader.datatype = 16 * factor
	aheader.bits = 32 * factor
	aheader.scale = 1.0
endif
;-------------------------------------------------------------------
;Insert the talairach transform 
;correct the transform for IDL coordinate system if acquired in matlab
;---------------------------------------------------------------------
idl_correct = 0
if strpos(strupcase(string(aheader.descrip)),'SPM') ge 0 then idl_correct = 1.0
mt = fltarr(4,4)
vox_sizes = [aheader.x_size,aheader.y_size,aheader.z_size]
for i = 0,2 do mt(i,i) = vox_sizes(i)
if total(aheader.orig) eq 0 then aheader.orig = [aheader.x_dim,aheader.y_dim,aheader.z_dim,0]/2
tal_orig = -(aheader.orig - idl_correct)*vox_sizes
mt(3,0:2) = tal_orig
mt(3,3) = 1
if total(aheader.magnet_transform) eq 0 then aheader.magnet_transform = mt
if aheader.landmark eq 0 then aheader.landmark = 1
true_mt = aheader.magnet_transform
if keyword_set(labheader) then begin
		h = make_lab_header()
		typef    = 'LOCS'
		messagef = ''
		parentf  = ''
		planef   = 'axial'

		typef    = string(typef   , format = '(A20)' )
		messagef = string(messagef, format = '(A50)' )
		parentf  = string(parentf , format = '(A100)')
		planef   = string(planef  , format = '(A10)' )
		h.fov= aheader.x_size * aheader.x_dim > aheader.y_size * aheader.y_dim
		tt = h.tal_transform
		if aheader.landmark eq 1 then tt = mt
		aheader={vchar:'V', version:h.version,an:aheader,$
		x:aheader.x_dim,y:aheader.y_dim,z:aheader.z_dim,time:aheader.t_dim,$
		magnet_transform:true_mt,magnet_tr_parent:parentf,landmark:aheader.landmark,direction:1.0,$
		FOV:h.fov,forward:0,startslice:0,type:typef,message:messagef,parent:parentf,$
		plane_name:planef,filter_name:messagef,plane:2,filter_cutoff:0.0,$
		ontime:0.0,offtime:0.0,inc:0.0,intro:0.0,offset:0.0,shift:0.0,$
		TE:0.0,TR:0.0,PT_WEIGHT:0.0,headersize:2048,$
		tal_transform:tt,excess:h.excess}
endif
volumein=cube
return,aheader

ioerr: 
if openerr eq 0 then close_lun, unit
err = -1
badboy, errtxt, filename
return, err



end



pro make_4D_analyze,filename = filename,P = P,outfilename = outfilename,remove = remove
;-------------------------------------
;converts list of analyze files to 4D
;-------------------------------------
if not(keyword_set(P)) then begin
	if not(keyword_set(filename)) then filename = pickfile(title = 'Choose first analyze file for 4D conversion',filter = '*.img')
	extract_pname,filename,pname
	extract_fname,filename,fname
	filter = strmid(fname,0,((strlen(fname) - 8) > 1) )
	P = findfile(filter + '*.img')
endif
extract_pname,(P(0)),pname
extract_fname,(P(0)),fname
fname = strmid(fname,0,((strlen(fname) - 4) > 1) )
aheader = read_analyze_header(filename = P(0))
aheader.t_dim = n_elements(P)
if not(keyword_set(outfilename)) then outfilename = pname + fname + '_4D'
openw,unit,outfilename + '.hdr',/get_lun,/swap_if_little_endian,error=openerr	
writeu,unit,aheader
close_lun,unit
openw,unit,outfilename + '.img',/get_lun,/swap_if_little_endian,error=openerr
datatype = aheader.datatype
hoffset = 0
img = assoc(unit, make_array(aheader.x_dim, aheader.y_dim, aheader.z_dim, $
	type = idl_datatype(aheader.datatype)), hoffset)

for i = 0,n_elements(P) - 1 do begin
	ah = read_analyze_header(filename = P(i),get_cube = cube)
	if keyword_set(remove) then begin
			s = strlen(P(i))
			basefile =  strmid(P(i),0,s - 4)
			spawn, 'rm ' + basefile + '.img'
			spawn, 'rm ' + basefile + '.hdr'
	endif
	;writeu,unit,cube
	img(i) = cube
endfor
close_lun,unit
end











