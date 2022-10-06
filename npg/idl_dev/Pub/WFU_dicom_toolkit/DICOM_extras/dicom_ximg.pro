pro write_dicomloc,filename = filename,cube = cube,rh = rh,coords = coords,dh = dh,nlocs = nlocs
;----------------------------------------------------------------------------------
;This program will convert ge pacs dicom files to locs
;It reads the dicom headers directly, and correctly fills magnet transform information
;I haven't found the dicom field for the landmark (rh.series.se_lmhor) yet
;For now, a random landmark is generated once for all the series in a study
;syntax:   dicom_ximg,dir = dir
;Note: it will unzip and untar as well, so you don't have to touch the ge pacs files
;once they've been ftp'd over
;I haven't tested this for CT images, you're better off using dicom_ximgconv
;----------------------------------------------------------------------------------
	print,'writing  ', filename
	print,nlocs
	wcoords = coords(*,*,0:nlocs - 1)	
	openw,/swap_if_little_endian,outunit,filename,/get_lun
	writeu,outunit,rh
	close_lun,outunit
	openu,/swap_if_little_endian,outunit,filename,/get_lun
	a=assoc(outunit,intarr(1),7900)
	a(0)=replicate(nlocs,1)
	a=assoc(outunit,fltarr(3,3,nlocs),7900+2)
	a(0)=wcoords
	a=assoc(outunit,intarr(dh.x,dh.y,nlocs),7900+2+4*9*nlocs)
	a(0) = cube
	close_lun,outunit
end



;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------



pro dicom_ximg,dir = dir,chop = chop,philips = philips,keep = keep,outpath = outpath

;----------------------------------------------------------------------------------
;This program will convert ge pacs dicom files to locs
;It reads the dicom headers directly, and correctly fills magnet transform information
;I haven't found the dicom field for the landmark (rh.series.se_lmhor) yet
;The landmark is generated from the dicom frame_of_reference field
;syntax:   dicom_ximg,dir = dir
;Note: it will unzip and untar as well, so you don't have to touch the ge pacs files
;once they've been ftp'd over
;J. Maldjian 7-24-2000
;----------------------------------------------------------------------------------


;Use a widget interface to choose inpath if not given as keyword
dicompath = '/data/'
cd,current = home
if path_of('hostname') eq 'kyle' then cd,dicompath
if (not keyword_set(dir)) then begin
   dir=pickdir(title="Please select the dir where images are located")
endif
if not(keyword_set(outpath)) then outpath = dir
inpath = dir
cd, inpath


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;unzip files using gunzip
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
filespec = 'MR*'
;filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
;make_filelist drops path, but we cd to inpath above
filels = findfile(filespec +  '.Z',count = fcount)

swap = 0
if fcount le 0 then begin
	filespec = 'GE*'
	;filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
	filels = findfile(filespec +  '.Z',count = fcount)
	swap = 1
endif
if fcount le 0 then begin
	filespec = 'CT*'
	;filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
	filels = findfile(filespec +  '.Z',count = fcount)
	swap = 1
endif
cmd = 'gunzip '
cmd = 'uncompress '
if fcount gt 0 then for i = 0,fcount -1 do spawn,cmd + inpath + filels(i)



;;;;;;;;;;;;;;;;;;;;;;
;;;;untar files
;;;;;;;;;;;;;;;;;;;;;;;
;filels = make_filelist(path = inpath,filespec = filespec,filecount = fcount)
filels = findfile(filespec,count = fcount)
if fcount le 0 then begin
	filespec = 'GE*'
	filels = findfile(filespec,count = fcount)
	swap = 1
endif
if fcount le 0 then begin
	filespec = 'CT*'
	filels = findfile(filespec,count = fcount)
	swap = 1
endif

cmd = 'tar -xvf '
for i = 0,fcount -1 do begin
	s = strlen(filels(i))
	mid = strmid(filels(i),s-3,3)
	if mid eq 'tar' then begin
		print,'untarring and removing ' + filels(i) + ' ...'
		 spawn,cmd + filels(i),result
		if not(keyword_set(keep)) then spawn, 'rm ' + inpath + filels(i)
	endif
endfor


;-----------------------------------------------
;Construc a list of the number of series
;-----------------------------------------------
dcmseries = findfile(inpath + '*.001',count = numseries)
if not(var_set(siemens)) then siemens = 0
if numseries le 0 then begin
	dcmseries = findfile(inpath + '*I0001*',count = numseries)
	siemens = 1
endif
	
if keyword_set(chop) then reply = 1
if keyword_set(philips) then reply = 0
if numseries le 0 then begin
	if not(var_set(reply)) then reply = new_ok(buttons = $
			['   PHILIPS   ','   CHOP   ','   OTHER  '],message = 'What type of  Dicom Images?',/bell)
	if reply eq 1 then begin
		chop = 1
		dcmseries = compute_chop_series(inpath,error = error)
		if not(error) then numseries = n_elements(dcmseries)
	endif
	if reply eq 0 then begin
		;print,'PHILLIPS'
		dcmseries = findfile(inpath + '*.1',count = numseries)
		if numseries eq 0 then begin
			dcmseries = findfile(inpath + '*',count = numseries)
			numseries = 1
			dcmseries = dcmseries(0)
		endif
	endif
endif

if numseries le 0 then begin
	dcmseries = findfile(inpath + '*.1',count = numseries)
endif


if numseries le 0 then begin
	print,'No .001 images detected...quitting'
	return
endif



;---------------------------------------
;Construct a loc file for each series
;---------------------------------------
last_study_id = ''
landmark = long(1000.0*(randomu(seed))) > 2
for snum = 0,numseries - 1 do begin
	dcmfile = dcmseries(snum)
	dh = read_dicomheader(filename = dcmfile,error = error,coords = coords,image = image,rh = rh,chop = chop)
	;nimages = fix(dh.number_of_temporal_positions.value)
	
	;----------------------------------------------------
	;If the study ID changed, then we need a new landmark
	;----------------------------------------------------
	if (dh.study_id ne last_study_id) or strlen(strtrim(dh.study_id,2)) eq 0 then begin
		last_study_id = dh.study_id
		landmark = long(1000.0*(randomu(seed))) > 2
	endif
	;------------------------------------------------------------------------------
	;If frame of reference field was nonzero, then use it for landmark
	;This field is actually a UID (ie., 123.123456.12341234.12341245.234), 
	;we are using a 'computation' in read_dicomheader to generate
	;a number like XX.XXX  based on the last 2 fields in the UID
	;------------------------------------------------------------------------------
	if rh.series.se_lmhor eq 0 then rh.series.se_lmhor = landmark else landmark = rh.series.se_lmhor
	;outfile=uppath +strlowcase(strtrim(hdr.image.im_psd_iname,2))+"_"+ plane $
	;	+  ":" + string(hdr.image.im_exno,format='(I5.5)') $
	;	+  ":"+ string(hdr.image.im_seno,format='(I2.2)') $
	;	+  ":"+ string(echo+1,format='(I1.1)')             $
	;	+   "."   + string(rep+1,format='(I1.1)')+".loc"
	ptname = str_sep(dh.name,',',/remove_all)
	ptname = ptname(0)
	outfilename = outpath + strlowcase(dh.psd_name) + "_" + dh.plane_name $
		+ "_" + dh.study_id + "_" + dh.series_number + "_"  + ptname + '_' $
		+ strtrim(string(dh.date),2) + '_' + strtrim(string(dh.z),2) + ".loc"
	outfilename = replace_string(outfilename,char = '*',rep = '')
	;print,'writing to ',outfilename
	lop_off = 3
	if siemens then lop_off = 8
	fsearch = strmid(dcmfile,0,strlen(dcmfile) - lop_off)
	dcmimages = findfile(fsearch + '*[0-9]*',count = numimages) 
	nlocs = numimages
	if keyword_set(chop) then begin
		dcmimages = expand_list(filename = dcmfile,ext = '.dcm',/chop)
		nlocs = n_elements(dcmimages)
		numimages = nlocs
	endif
	parse_filename,f=dcmimages(0),ext = ext,base = dcmbase,path = dcmpath
	if not(siemens) then begin
		nlist = fix(strmid(dcmimages,strlen(dcmbase) + strlen(dcmpath) + 1,max(strlen(dcmimages)) + 1))
		dcmimages = dcmimages(sort(nlist))
	endif

	coords = fltarr(3,3,numimages)
	locstack = intarr(dh.x,dh.y,numimages)
	;---------------------------------------
	;Load up all the images in this series
	;and construct the coords matrix
	;---------------------------------------
	;print, dcmimages(0)
	print,numimages
	startloc = 0
	this_series = 0
	series_count = 0
	mseries = strarr(2)
	for inum = 0,numimages - 1 do begin
		if inum mod 10 eq 0 then print, strtrim(numimages - inum,2) + ' remaining'
		ih = read_dicomheader(filename = dcmimages(inum),image = image,chop = chop,rh = rh2)
		if inum eq 0 then last_thick = ih.slthick
		locstack(*,*,inum) = image
		coords(*,*,inum) = ih.ras
		;---------------------------------------------------------------------
		;Fix to deal with CT scans that appear to be 1 series
		;But are actually 2 acquisitions with different slice thickness
		;--------------------------------------------------------------------
		if ih.slthick ne last_thick then begin
			series_count = series_count + 1			
			locs_in_this_series = (inum - startloc)
			rh2.series.se_lmhor = landmark
			rh2.image.im_slquant = fix(locs_in_this_series)
			rh2.series.se_numimages = long(locs_in_this_series)
			ih.z = locs_in_this_series
			this_series = this_series + 1
			nextfilename = outpath + strlowcase(dh.psd_name) + "_" + dh.plane_name $
				+ "_" + dh.study_id + "_" + dh.series_number + "." $
				+  strtrim(this_series,2) + "_"  + ptname + '_' $
				+ strtrim(string(dh.date),2) + '_' + strtrim(string(ih.z),2) + ".loc"
			nextfilename = replace_string(nextfilename,char = '*',rep = '')
			write_dicomloc,filename = nextfilename,cube = locstack(*,*,startloc:inum - 1),$
				rh = rh2,coords = coords(*,*,startloc:inum - 1),dh = ih,$
				nlocs = locs_in_this_series
			startloc = inum
			if series_count eq 1 then mseries(series_count - 1) = nextfilename
		endif
		last_thick = ih.slthick	
	endfor
	;------------------------------------
	;Write the file
	;------------------------------------


	locs_in_this_series = (inum - startloc)
	rh2.series.se_lmhor = landmark
	rh2.image.im_slquant = fix(locs_in_this_series)
	rh2.series.se_numimages = long(locs_in_this_series)
	ih.z = locs_in_this_series
	this_series = this_series + 1
	nextfilename = outpath + strlowcase(dh.psd_name) + "_" + dh.plane_name $
		+ ":" + dh.study_id + ":" + dh.series_number + "." + strtrim(this_series,2) + ":"  + ptname + ':' $
		+ strtrim(string(dh.date),2) + ':' + strtrim(string(ih.z),2) + ".loc"
	nextfilename = replace_string(nextfilename,char = '*',rep = '')
	write_dicomloc,filename = nextfilename,cube = locstack(*,*,startloc:inum - 1),rh = rh2,coords = coords(*,*,startloc:inum - 1),dh = ih,$
		nlocs = locs_in_this_series
	series_count = series_count + 1
	;----------------------------------------------------------------------------
	;If this is CT data, write a single resampled volume for multiple series
	;----------------------------------------------------------------------------
	if series_count eq 2 then begin
		mseries(series_count - 1) = nextfilename
		if dh.modality eq 'CT' then begin
			print,'Multiple CT series detected, Resampling to single volume....'
			;print,'file1 = ' + mseries(0)
			;print,'file2 = ' + mseries(1)
			;return
			outct = resampleCT(file1 = mseries(0),file2 = mseries(1),thick= 5)
		endif
	endif


	;openw,/swap_if_little_endian,outunit,outfilename,/get_lun
	;writeu,outunit,rh
	;close_lun,outunit
	;openu,/swap_if_little_endian,outunit,outfilename,/get_lun
	;a=assoc(outunit,intarr(1),7900)
	;a(0)=replicate(nlocs,1)
	;a=assoc(outunit,fltarr(3,3,nlocs),7900+2)
	;a(0)=coords
	;a=assoc(outunit,intarr(dh.x,dh.y,nlocs),7900+2+4*9*nlocs)
	;a(0) = locstack
	;close_lun,outunit
	;-----------------------------------------------------------------------------
	;copy 1 dicom file as a representative header, then remove the dicom images
	;-----------------------------------------------------------------------------
	spawn,'cp '  + dcmimages(0) + ' ' + nextfilename + '.dicom'
	if not(keyword_set(keep)) and not keyword_set(chop) then spawn,'rm ' + fsearch + '*'
	if keyword_set(chop) and not(keyword_set(keep)) then for i = 0,numimages - 1 do remove,dcmimages(i)
	if dh.psd_name eq '3dtof' then fix_mra,filename = outfilename
endfor

cd,home
end

