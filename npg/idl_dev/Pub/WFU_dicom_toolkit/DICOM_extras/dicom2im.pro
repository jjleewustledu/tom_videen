function sort_imagelist,imagelist
template = imagelist(0)
parse_filename,f = template,base = base,path = fpath,ext = ext
base = strupcase(base)
sortlist = imagelist
;------------------
;Check if ESI format
;------------------
if (strpos(base,'E') ge 0) and (strpos(base,'S') ge 1) and (strpos(base,'I') ge 2) then begin
	frontend = strlen(fpath)
	istart = strpos(base,'I') + frontend
	iend = strlen(base) + frontend - istart - 1
	print,base,istart,iend
	sortlist = imagelist(sort(fix(strmid(imagelist,istart + 1,iend))))
endif
return,sortlist
end


pro dicom2im,filename = filename,dir = dir,keep = keep,discard = discard
;-------------------------------------------------------------------------
;This program will take a directory of dicom fMRI images
;(1 image per file, multiple temporal phases)
; and generate a single stack with a .imdicom ending
;It will create an output directory above the parent DICOM directory
;set discard = number of initial volumes to throw away (optional)
;Use the cycle_dicom program (below) to cycle through multiple 
;Dicom subdirectories of anatomics and functionals
;The stack header is a .im type header (a pfile header) + coords matrix
;The .imdicom header consists of:
; genesis pfile raw header
;number of slices (not total images)
;coords matrix (3,3,nslices)
;indivual volumes
;the read_header command will return the fh and the first volume
;gethdrinfo will work on this .imfile
;restorest will work through restorest_imdicom (automaticaly routes)
;get_iminfo will work through get_imdicom_info (automatically routes)
;rh.rec.rdbm_rev  = 7.001 identifies this as an imdicom and not an LX .im
;---------------------------------------------------------------------------
if not(keyword_set(filename)) and keyword_set(dir) then begin
	filename = findfile(dir + '*[0=9]*',count = numimages)
	if numimages le 0 then begin
		print,'No Images found in ' + dir
		return
	endif
	filename = filename(0)
endif
ih = read_dicomheader(filename = filename,dh = dh,rh = gh,err = err,mosaic = mosaic)
if err then begin
	print,'Error reading ' + filename
	return
endif



dcmfile = filename

;--------------------------------
;Generate patient labels
;--------------------------------
parse_filename,f = dcmfile,base = fsearch,path = dcmpath,ext = ext1
dcmpath1 = dcmpath
ptname = str_sep(ih.name,',',/remove_all)
ptname = ptname(0)
ptid = str_sep(dh.patient_id.value,',',/remove_all)
ptid = ptid(0)
study_time = dh.series_time.value
hour = strmid(study_time,0,2)
minute = strmid(study_time,2,2)
second = strmid(study_time,4,2) 
year = strmid(ih.date,2,2)
month = strmid(ih.date,4,2)
day = strmid(ih.date,6,2)
;ptlabel = ptname + '_' + ptid  + '.' + hour + '_' + minute + '_' + second + '.' + month + '_' + day + '_' + year
ptlabel = ptname +  '.' + hour + '_' + minute + '_' + second + '.' + month + '_' + day + '_' + year
ptdir = ptname + '_' + month + '_' + day + '_' + year
extract_pname,dcmpath,p,parents = parents
outpath = '/'
for i = 0,n_elements(parents)-4 do outpath = outpath + parents(i) + '/'

;-------------------------------
;Make the output directory
;-------------------------------
spawn, "cd " + outpath + "; mkdir "+ ptdir
spawn, "cd " + outpath + "; chmod 775 "+ ptdir
outpath = outpath + ptdir + '/'
inpath = outpath

;------------------------------------------------------
;Determine if this is an anatomic or functional run
;------------------------------------------------------
nimages = 0
if is_tag(tag_names(dh),'number_of_temporal_positions') then nimages = fix(dh.number_of_temporal_positions.value)
if mosaic then begin
	nimages = numimages ;Number not specified in Siemens Dicom header, volumes = number of mosaic files 
endif

if nimages le 1 then begin
	if strpos(strupcase(dh.manufacturer.value),'PHILIPS') ge 0 then philips = 1
	if strpos(strupcase(dh.manufacturer.value),'SIEMENS') ge 0 then siemens = 1
	print,'Running dicom_ximg with '
	print,dcmpath
	dicom_ximg,dir = dcmpath,philips = philips,keep = keep,outpath = outpath
	return
end

rh = mk_rawhdr_lx(offset = offset)

;----------------------------------------------------------
;identifies this as an imdicom file and not a regular .im
;----------------------------------------------------------
rh.rec.rdbm_rev = 7.001
;gh = mk_genheader_lx()
rh.series = gh.series
rh.image = gh.image
rh.exam = gh.exam
xdim = ih.x 
ydim = ih.y


fsearch = dcmpath + fsearch + '.'
dcmimages = findfile(fsearch + '*[0-9]*',count = numimages)
nlocs = numimages
parse_filename,f=dcmimages(0),ext = ext,base = dcmbase,path = dcmpath
nlist = fix(strmid(dcmimages,strlen(dcmbase) + strlen(dcmpath) + 1,max(strlen(dcmimages)) + 1))
dcmimages = dcmimages(sort(nlist))
nslices = numimages/nimages
nfiles = numimages

if mosaic then begin
	;-------------------------------------------------------------------------------
	;nslices means images per volume
	;nimages means number of volumes
	;numimages means total image files assuming 1 image per file (nslices * nimages)
	;nlocs means total image files assuming 1 image per file (nslices * nimages
	;nfiles means number of files to open
	;-------------------------------------------------------------------------------
	dcmimages = findfile(dcmpath1 + '*' + ext1,count = nimages)
	nslices = ih.z
	nlocs = nslices *nimages
	numimages = nlocs
	nfiles = nimages
endif

;---------------------------------------
;See if we want to chuck some volumes
;and adjust header fields appropriately
;----------------------------------------
if keyword_set(discard) then begin
	if mosaic then begin
		dcmimages = dcmimages(discard:*) 
		numimages = numimages - discard
		nfiles = nfiles - discard
	endif else begin
		dcmimages = dcmimages((discard * nslices) : *)
		numimages = numimages - (discard * nslices)
		nfiles = nfiles - (discard * nslices)
	endelse
	nimages = nimages - discard
	;rh.series.se_numimages = long(numimages)
endif

;-------------------------------------------
;get the x and y matrix size and the nex
;Load up some raw data header fields
;-------------------------------------------
rh.rec.nslices = nslices
rh.rec.frame_size = long(xdim)
rh.rec.nframes = long(ydim)
rh.rec.navs = nimages
rh.rec.reps = 1
rh.image.im_slquant = fix(nslices)


;------------------------------------------------------
;Load up all the images one temporal phase at a time
;and construct the coords matrix for 1 volume
;------------------------------------------------------
coords = fltarr(3,3,nslices)
locstack = intarr(ih.x,ih.y,nslices)


;outfilename = inpath + strlowcase(ih.psd_name) + "_" + ih.plane_name $
;		+ ":" + ih.study_id + ":" + ih.series_number + ":"  + ptname + ':' $
;		+ strtrim(string(ih.date),2) + ':' + strtrim(string(ih.z),2) + ".im"
outfilename = outpath + ptlabel + ".im"


print,'outfilename = ',outfilename
print,'numimages =',numimages
print,'nimages = ',nimages
print,'nslices = ',nslices

;-----------------------------------------------------------------------------
;Now cycle through the images generating 1 volume at a time
;The initial volume will be slow since we are constructing the coords matrix
;-----------------------------------------------------------------------------
for inum = 0,nfiles - 1 do begin
	filenum = inum
	slice_counter = inum mod nslices
	if slice_counter eq 0 then print, strtrim(nfiles - inum,2) + ' remaining'
	if mosaic then begin
		ih = read_dicomheader(filename = dcmimages(inum),image = locstack,rh = rh2,coords = coords)
		slice_counter = nslices - 1
		filenum = ((inum + 1) * nslices) - 1
	endif else begin
		if inum ge nslices then image = read_dicom(dcmimages(inum)) else begin $
			ih = read_dicomheader(filename = dcmimages(inum),image = image,chop = chop,rh = rh2)
			coords(*,*,inum) = ih.ras
		endelse
		locstack(*,*,slice_counter) = fix(image)
	endelse
	;--------------------------------------
	;Write file 1 volume at a time
	;--------------------------------------
	if slice_counter eq nslices - 1 then begin
		if filenum eq (nslices - 1) then begin 
			print,'writing header'
			openw,/swap_if_little_endian,outunit,outfilename,/get_lun
			writeu,outunit,rh
			close_lun,outunit
			openu,/swap_if_little_endian,outunit,outfilename,/get_lun
			a=assoc(outunit,intarr(1),offset)
			a(0)=replicate(nslices,1)
			a=assoc(outunit,fltarr(3,3,nslices),offset+2)
			a(0)=coords
			a=assoc(outunit,intarr(ih.x,ih.y,nslices),offset+2+4*9*nslices)
		endif
		print,'writing volume ', filenum/nslices + 1
		a(filenum/nslices) = locstack
	endif
endfor
close_lun,outunit
;restorest_imdicom,outfilename,stack,lcoords
;for i = 0,nfiles -1 do rotate_set,stack(*,*,*,i),4
;rh = read_rawheader(filename = outfilename)
;write_im_file, pfile = outfilename, stack
print,'All Done'
end


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


pro restorest_imdicom,filename,stack,lcoords
;-----------------------------------------------------------
;Program for reading full stack from .imdicom file
;standard LX rawheader followed by coords info for 1 volume
;images are volumes over time
;------------------------------------------------------------
gethdrinfo56,   nslices, nimages, xdim, ydim, nex, rh, filename,vrgfxres = vrgfxres
rh = read_rawheader(filename = filename,offset = gh_offset)
openr,/swap_if_little_endian,unit,filename,/get_lun
a=assoc(unit,intarr(1),gh_offset)
tmparr=a(0)
nlocs=tmparr(0)
a=assoc(unit,fltarr(3,3,nlocs),gh_offset + 2)
lcoords=a(0)
status = fstat(unit)
a=assoc(unit,intarr(xdim,ydim,nslices,nimages),(gh_offset + 2)+36*nlocs)
stack=a(0)
close_lun,unit
end



pro cycle_dicom2im,dir = dir,keep = keep,discard = discard
	keep = 1
	if not(keyword_set(dir)) then dir = pickdir(title = 'Choose directory of DICOM folders')
	cd,dir,current = here
	dirnames = getdirs()
	if n_elements(dirnames) gt 1 then begin
		for i = 1,n_elements(dirnames) - 1 do begin
			nextdir = dir + dirnames(i) + '/'
			print,'Working on ' + nextdir
			print,'Remaining Series = ',n_elements(dirnames) - i 
			dicom2im,dir = nextdir,keep = keep,discard = discard
		endfor
	endif
	cd,here
end

	
