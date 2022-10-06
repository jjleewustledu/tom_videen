function make_dicom_output_directory,filename = filename,dh = dh
;----------------------------------------------------------------------------------
;Creates a directory to hold the analyze files based on subject name and study date
;J. Maldjian 
;----------------------------------------------------------------------------------
;--------------------------------
;Generate patient labels
;--------------------------------
if not(keyword_set(dh)) then dh = read_dicomheader2(filename = filename)
dcmfile = filename
extract_pname,dcmfile,dcmpath
ptname = str_sep(dh.name,',',/remove_all)
ptname = ptname(0)
ptid = str_sep(dh.id,',',/remove_all)
ptid = ptid(0)
study_time = dh.series_time
hour = strmid(study_time,0,2)
minute = strmid(study_time,2,2)
second = strmid(study_time,4,2) 
year = strmid(dh.date,2,2)
month = strmid(dh.date,4,2)
day = strmid(dh.date,6,2)
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
return,outpath
end



pro dicom2analyze,dir = dir,keep = keep,outpath = outpath,discard = discard,dh = dh,last_dh = last_dh,create_imfile = create_imfile

;----------------------------------------------------------------------------------
;This program will convert dicom files to analyze
;It will work on anatomics, functionals, and mosaics
;GE, Philips, Siemens tested
;It reads the dicom headers directly, and correctly fills magnet transform information
;I haven't found the dicom field for the landmark (rh.series.se_lmhor) yet
;The landmark is generated from the dicom frame_of_reference field
;syntax:   dicom2analyze,dir = dir,discard = discard
;set discard to the number of volumes to discard for any fMRI acquistions
;J. Maldjian 5-10-2003
;----------------------------------------------------------------------------------

dcmreply = generate_dicom_series(directory = dir)
dcmseries = dcmreply.dcmseries
numseries = dcmreply.numseries

;---------------------------------------
;Construct an analyze file for each series
;---------------------------------------
last_study_id = ''
last_ptid = ''
last_date=''
if keyword_set(last_dh) then begin
	last_study_id = last_dh.study_id
	last_ptid = last_dh.id
	last_date = last_dh.date
endif
landmark = long(1000.0*(randomu(seed))) > 2
for snum = 0,numseries - 1 do begin
	print,'Series remaining = ',numseries - snum
	dcmfile = dcmseries(snum)
	print, dcmfile
	dh = read_dicomheader2(filename = dcmfile,error = error,coords = coords,image = image)
	if not(keyword_set(outpath)) then dicom_outpath = make_dicom_output_directory(filename = dcmfile,dh = dh) else dicom_outpath = outpath
	;cd, dir,current = home
	;----------------------------------------------------
	;If the study ID changed, then we need a new landmark
	;----------------------------------------------------
	if (dh.study_id ne last_study_id) or (dh.id ne last_ptid) or (dh.date ne last_date) or strlen(strtrim(dh.study_id,2)) eq 0 then begin
		print,'Study ID changed'
		print, 'last study id: ',last_study_id,' new id: ',dh.study_id
		print, 'last pt id: ',last_ptid,' new id: ',dh.id
		print, 'last study date: ',last_date,' new date: ',dh.date
		last_study_id = dh.study_id
		last_ptid = dh.id
		last_date = dh.date
		landmark = long(1000.0*(randomu(seed))) > 2
	endif
	;------------------------------------------------------------------------------
	;If frame of reference field was nonzero, then use it for landmark
	;This field is actually a UID (ie., 123.123456.12341234.12341245.234), 
	;we are using a 'computation' in read_dicomheader to generate
	;a number like XX.XXX  based on the last 2 fields in the UID
	;------------------------------------------------------------------------------
	if dh.landmark eq 0 then dh.landmark = landmark else landmark = dh.landmark
	ptname = str_sep(dh.name,',',/remove_all)
	ptname = ptname(0)
	dcmimages = generate_dicom_filelist(filename = dcmfile)
	numimages = n_elements(dcmimages)
	hnumimages = numimages 
	fourd = ''	
	if (dh.mosaic or dh.t gt 1) then begin
		if keyword_set(discard) then begin
			if dh.mosaic and (numimages gt discard) then begin
				dcmimages = dcmimages(discard:*) 
				numimages = numimages - discard
			endif
			if not(dh.mosaic) and (numimages gt (discard*dh.z)) then begin
				dcmimages = dcmimages((discard * dh.z) : *)
				numimages = numimages - (discard * dh.z)
			endif
		endif
		if dh.mosaic then fourd = '_' + strtrim(fix(numimages),2) + '_4D' else fourd = '_' + strtrim(fix(numimages/dh.z),2) + '_4D'
	endif
	outfilename = dicom_outpath + strlowcase(dh.psd_name) + "_" + dh.plane_name $
		+ "_" + dh.study_id + "_" + dh.series_number + "_"  + ptname + '_' $
		+ strtrim(string(dh.date),2) + '_' + strtrim(string(dh.z),2) + fourd
	outfilename = replace_string(outfilename,char = '*',rep = '')

	;------------------------------------
	;Construct the analyze .img file
	;The analyze file can be 3-D or 4-D
	;------------------------------------
	openw,unit,outfilename + '.img',/get_lun,/swap_if_little_endian
	print,'writing analyze file to ', outfilename + '.img'

	volfreq = 5
	for i = 0,numimages - 1 do begin
			
		if dh.mosaic then begin
			dh1 = read_dicomheader2(filename = dcmimages(i),image = image,numimages = hnumimages)
			if (1.0*i mod volfreq eq 0) then print,'Remaining volumes ', numimages - i
			for j = 0,dh1.z - 1 do image(*,*,j) = rotate(image(*,*,j),7) 
		endif else begin $
			if ( (1.0*i/dh.z) mod volfreq eq 0) then print,'Remaining volumes ', (numimages/dh.z) - (1.0*i/dh.z)
			;if i lt dh.z then begin
			;	dh1 = read_dicomheader2(filename = dcmimages(i),image = image,numimages = hnumimages) 
			;	coords(*,*,i) = dh1.ras
			;endif else 
			if i eq 0 then dh1 = read_dicomheader2(filename = dcmimages(i),image = image,numimages = hnumimages) else $
				image = read_dicom(dcmimages(i))
			image = rotate(image,7)
		endelse
		writeu,unit,image
	endfor
	close_lun,unit
	;-----------------------------------------------------------------------
	;Sometimes Emory sends slice-time ordered instead of acquisition ordered
	;-----------------------------------------------------------------------
	an = dh1.an
	slice_time=0
	if an.t_dim gt 1 then begin
		dh3 = read_dicomheader2(filename = dcmimages(0))
		dh4 = read_dicomheader2(filename = dcmimages(1))
		if total(abs(dh3.pos - dh4.pos)) eq 0 then begin
			print,'Slice-time ordered DICOM detected'
			slice_time=1
			print,'Analyze volume will be re-ordered for acquisition order'
			stack = make_array(an.x_dim, an.y_dim, an.t_dim, an.z_dim, $
				type = idl_datatype(an.datatype))
			openr,unit,outfilename + '.img',/get_lun,/swap_if_little_endian
			readu,unit,stack
			close_lun,unit
			openw,unit,outfilename + '.img',/get_lun,/swap_if_little_endian
			for i = 0,an.t_dim - 1 do writeu,unit,reform(stack(*,*,i,*))
			close_lun,unit
			print,'Reordering complete'			
		endif
	endif
	print,'Generating coords matrix...'
	for i = 0,an.z_dim-1 do begin
		if slice_time then image_offset = (1.0*i*an.t_dim) + i else image_offset=i
		dh1 = read_dicomheader2(filename = dcmimages(image_offset),numimages=hnumimages)
		coords(*,*,i) = dh1.ras
	endfor
	
	;------------------------------------
	;Construct the analyze .hdr file
	;------------------------------------
	openw,unit,outfilename + '.hdr',/get_lun,/swap_if_little_endian
	an.magnet_transform = coords2mt(coords = coords,dims = [dh1.x,dh1.y,dh1.z])
	an.landmark = landmark
	if keyword_set(discard) then begin
		if an.t_dim gt discard then an.t_dim = an.t_dim - discard
	endif
	if dh1.mosaic then begin
		an.t_dim = numimages
		an.dims = 4	
	endif

	writeu,unit,an
	close_lun,unit
	;-----------------------------------------------------------------------------
	;copy 1 dicom file as a representative header, then remove the dicom images
	;-----------------------------------------------------------------------------
	spawn,'cp '  + dcmimages(0) + ' ' + outfilename + '.dicom'
	;if not(keyword_set(keep)) and not keyword_set(chop) then spawn,'rm ' + fsearch + '*'
	;if not(keyword_set(keep)) then for i = 0,numimages - 1 do remove,dcmimages(i)
	if an.t_dim gt 1 and keyword_set(create_imfile) then begin
		analyze2im,filename = outfilename + '.img',coords = coords,dicomfile = outfilename + '.dicom',rotation = rotation
	endif

endfor

;cd,home
end



pro cycle_dicom,dir = dir,keep = keep,discard = discard,create_imfile = create_imfile
;------------------------------------------------------------------------------
;Program will cycle through a directory with DICOM subdirectories
;generating analyze files from any DICOM file types encountered
;cycle_dicom,dir = dir,discard = discard
;set discard to the number of volumes to throw out of any fMRI acquisitions
;J. Maldjian
;------------------------------------------------------------------------------

	keep = 1
	if not(keyword_set(dir)) then dir = pickdir(title = 'Choose directory of DICOM folders')
	cd,dir,current = here
	dirnames = getdirs()
	if n_elements(dirnames) gt 1 then begin
		for i = 1,n_elements(dirnames) - 1 do begin
			nextdir = dir + dirnames(i) + '/'
			print,'Working on ' + nextdir
			print,'Remaining Series = ',n_elements(dirnames) - i 
			dicom2analyze,dir = nextdir,keep = keep,discard = discard,last_dh = last_dh,dh = dh,create_imfile = create_imfile
			last_dh = dh
		endfor
	endif
	cd,here
end

