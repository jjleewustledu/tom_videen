;--------------------------------------------------------------------------------------------------
;There are a suite of programs here for converting any images to Dicom (GE,FLATFILE,ANALYZE)
;syntax:  convert2dicom,locfile = locfile,images = images
;J. Maldjian, 7-24-2000
;--------------------------------------------------------------------------------------------------

function eliminate_badchars,byte_array,value=value
;----------------------------
;Gets rid of some bad actors
;---------------------------
if not(var_set(value)) then value = 0
nbyte_array=byte_array
temp = where(byte_array gt 9 and byte_array lt 13,count)
if count gt 0 then nbyte_array(temp) = value
temp = where(byte_array eq 0,count)
if count gt 0 then nbyte_array(temp) = value
return,nbyte_array
end

function check_patients,object = object,target = target
;-------------------------------------------------------------------------------------
;This program will do a quick check of object and target names and ids
;input a structure: target={name:'ptname', id:'1000000'}
;reply is also a structure:  {id_check:id_check,name_check:name_check,action:action}
;---------------------------------------------------------------------------------------
	id_check = 1
	name_check = 1
	action = 1
	title = 'Pt Name Check'
	text1 = 'Patient names and ids check out'
	cancel_button = 'Cancel' 
	overide_button = 'Overide'
	Ok_button = 'Continue'
	buttons = [cancel_button, ok_button]
	pt_fmri = strtrim(object.name,2)
	pt_smn = strtrim(target.name,2)
	id_fmri = strtrim(object.id,2)
	id_smn = strtrim(target.id,2)
	if long(id_fmri) eq long(id_smn) then id_check = 1
	if n_elements(pt_smn) eq n_elements(pt_fmri) then begin
		name_check = 1
		s = n_elements(pt_smn)
		for i = 0,s-1 do begin
				if pt_smn(i) ne pt_fmri(i) then begin
					title = 'Pt Name Error'
					name_check = 0
					text1 = 'Different Patients!'
					buttons = [cancel_button, overide_button]
				endif
		endfor
	endif
	if long(id_fmri) ne long(id_smn) then begin
		id_check = 0
		title = 'Pt ID Error'
		text1 = 'Different Patients!'
		if name_check then text1 = 'Different Patient ids,' + " But names check out." 
		buttons = [cancel_button, overide_button]
	endif
	if n_elements(pt_smn) ne n_elements(pt_fmri) then begin
		text1 = 'Unable to Confirm Pt Name'
		if id_check then text1 = text1 + " But id's check out."
		buttons = [cancel_button, ok_button]
	endif
	space = '     '
	if not(id_check) or not(name_check) then action = new_ok(buttons = buttons,message = [text1,'fmri: ' + pt_fmri + ' ' + strtrim(id_fmri,2),$
				'smn : ' + pt_smn + ' ' + strtrim(id_smn,2)],title = title,/bell)
	return,{id_check:id_check,name_check:name_check,action:action}
end

;-------------------------------------------------------------------------------------


function convert2dicom1,rh = rh,sh = sh,image_number=image_number,dh = dh

if not(keyword_set(dh)) then begin
	dh = make_dicom_header()
	if keyword_set(rh) then dh = ge2dicom(rh)
endif
dhnew = dh
tags = tag_names(dhnew)
dhnew.SOP_Instance_UID.value = dh.Series_Instance_UID.value + '.' + strtrim(image_number,2)
dhnew.Image_Number.value = strtrim(image_number,2)
rel = str_sep(dhnew.Image_Position_Patient.value,'\')
orientation_value=''
if is_tag(tag_names(dhnew),'patient_orientation') then begin
	orientation_value = dhnew.Patient_Orientation.value
	case orientation_value of
	'L\P': begin
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) + $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = rel(0) + '\' + rel(1) + '\' + dhnew.Slice_Location.value
		end
	'P\F': begin	
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) - $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = dhnew.Slice_Location.value + '\' + rel(1) + '\' + rel(2)
		end
	'L\F': begin	
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) - $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = rel(0)  + '\' + dhnew.Slice_Location.value + '\' + rel(2)
		end
	'P\L': begin
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) - $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = rel(0) + '\' + rel(1) + '\' + dhnew.Slice_Location.value
		end
	'F\P': begin	
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) + $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = dhnew.Slice_Location.value + '\' + rel(1) + '\' + rel(2)
		end

	'L\F': begin	
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) + $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = rel(0)  + '\' + dhnew.Slice_Location.value + '\' + rel(2)
		end
	else:	begin
			dhnew.Slice_Location.value = strtrim(float(dh.Slice_Location.value) + $
				float(dh.spacing_between_slices.value) * (image_number - 1),2)
			dhnew.Image_Position_Patient.value = rel(0) + '\' + rel(1) + '\' + dhnew.Slice_Location.value
		end

endcase
endif

if keyword_set(sh) then begin
	dhnew.slice_thickness.value	=	strtrim(sh.thick.value,2)
	dhnew.spacing_between_slices.value	=	strtrim(float(sh.thick.value) + float(sh.gap.value),2)
	if is_tag(tags,'Rows') then 	dhnew.Rows.value		=	strtrim(sh.y.value,2)
	if is_tag(tags,'Columns') then dhnew.Columns.value		=	strtrim(sh.x.value,2)
	dhnew.Pixel_Spacing.value 		=	strtrim(sh.pix.value,2)
	dhnew.study_date.value 		= 	strtrim(sh.date.value,2)
	dhnew.series_date.value		= 	strtrim(sh.date.value,2)
	dhnew.image_date.value 		= 	strtrim(sh.date.value,2)
	dhnew.accession_number.value 	= 	strtrim(sh.accession.value,2)
	dhnew.Institution_Name.value 	=	strtrim(sh.institution.value,2)
	dhnew.study_description.value 	= 	strtrim(sh.study_description.value,2)
	dhnew.series_description.value 	=   	strtrim(sh.series_description.value,2)
	if is_tag(tag_names(dhnew),'Additional_Patient_History') then dhnew.Additional_Patient_History.value 	=   	strtrim(sh.History.value,2)
	dhnew.patient_name.value 	= 	strtrim(sh.name.value,2)
	if is_tag(tag_names(dhnew),'other_patient_names') then dhnew.other_patient_names.value 	= 	strtrim(sh.name.value,2)
	dhnew.patient_id.value		= 	strtrim(sh.id.value,2)
	;dicomroot = '1.2.840'
	;hupid = '200000'
	;idlspmid = '200000'
	;------------------------------------
	;registered under IDLSPM with IANA
	;www.isi.edu
	;-------------------------------------
	dicomroot = '1.3.6.1.4.1'
	idlspmid = '12402'
	timestamp			= 	strtrim(sh.timestamp.value,2)
	patient_id			= 	strtrim(sh.id.value,2)
	;------------------------
	;No UIDs can lead with 0
	;------------------------
	accession_number 		= 	strtrim(long(sh.accession.value),2)
	series_number 			= 	strtrim(90 + fix(sh.series_number.value),2)
	image_number			=	strtrim(image_number,2)
	landmark 			= 	strtrim(string(sh.landmark.value,format = '(F20.2)'),2)
	;Study_Instance_UID		'1.2.840.113619.2.1.1.2702429029.814.936099513.377'
	;Series_Instance_UID		'1.2.840.113619.2.1.1.2702429029.814.936099513.634'
	;dh.SOP_Instance_UID.value = 	'1.2.840.113619.2.1.1.2702429029.814.936099513.654'
	;Frame_of_Reference_UID	=	'1.2.840.113619.2.1.1.2702429029.809.936099446.28'
	dhnew.series_number.value	= 	series_number
	dhnew.Study_Instance_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number 
	dhnew.Series_Instance_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number + '.' + series_number
	dhnew.Frame_of_Reference_UID.value = dicomroot + '.' + idlspmid + '.' + timestamp + '.' + accession_number + '.'  + landmark

endif
;---------------------------
;Set the series unique id
;--------------------------
dhnew.SOP_Instance_UID.value = dhnew.Series_Instance_UID.value + '.' + strtrim(image_number,2)
dhnew.Image_Number.value = strtrim(image_number,2)
;dhnew.acquisition_matrix.value = dhnew.rows.value + ' '  + dhnew.columns.value
if is_tag(tag_names(dhnew),'acquistion_matrix') then dhnew.acquisition_matrix.value = dhnew.columns.value + ' '  + dhnew.rows.value

return,dhnew

end




pro convert2dicom,locfile = locfile,images = images,analyze=analyze,$
	quiet = quiet,dh = dh,sh = sh,outputdir = outputdir,oh = oh,$
	imagelist = imagelist,tasktext = tasktext,run = run,$
	outputlist = outputlist,raw = raw,error = error,nowrite=nowrite,send_sh = send_sh
;--------------------------------------------------------------------------------------------------
;This program will take any images and create dicom files
;syntax:  convert2dicom,locfile = locfile,images = images 
;where locfile is the ge locfile (or i.000 image) which is in the same data space as what you plan to write
;The program will look for a template dicom header in the locfile directory, and use this to fill the fields
;if not found, it will use the ge raw header from the locfile
;images = the modified image volume to be written
;If you don't have a ge locfile, you can send an analyze file with the analyze = analyze.img option
;a widget will pop open allowing you to make some limited header mods
;You can optionally send oh=oh, in which oh is the object ge header for some automated description header mods
;You can also send imageslist = imagelist, a list of ge i.00x images to be converted
;J. Maldjian 7-24-2000
;--------------------------------------------------------------------------------------------------
error = 0
if not(keyword_set(outputdir)) then begin
	if keyword_set(locfile) then extract_pname,locfile,outputdir
	if keyword_set(analyze) then extract_pname,analyze,outputdir
	if not(keyword_set(outputdir)) then outputdir = pickdir(title = 'Choose directory to output DICOM images')
endif
cd,current = here
if not(keyword_set(run)) then run = 1
run = strtrim(run,2)

;-------------------------------------
;f0 	no one can change these fields
;f1     can change if no ge locheader
;f2	anyone can change
;f3 	not visible
;-------------------------------------
f0 = 1
f1 = 0
f2 = 0
f3 = 2


;----------------------------------------------------
;set some defaults in case no headers are provided
;----------------------------------------------------
filename = 'No locfile'
institution= 'IDLSPM'
thick = 5.0
gap = 0.0
x= 256
y= 256
pix = '0.8593750000\0.8593750000'
if not(keyword_set(tasktext)) then tasktext = ''
tasktext2 = tasktext
tasktext1 = tasktext
fusion = ' FUSION'
if keyword_set(oh) then begin
	tasktext1 = ''
	fusion = ''
endif
name= 'Research Patient'
id= '100000'
accession= '100000'
series_number = '1'
landmark = '100.00'
study_description = tasktext1 + ' FMRI'
series_description = 'FMRI'
history = tasktext1 +  ' FUSION'
timestamp = long(systime(1))
sdate = invert_systime(timestamp)
year = sdate.year
month = sdate.month
day = sdate.day
if strlen(month) eq 1 then month = '0' + month
if strlen(day) eq 1 then day = '0' + day
plane_name = 'Ax'

if keyword_set(imagelist) then begin
	image = open_square_image(filename = imagelist(0))
	sz = size(image)
	images = intarr(sz(1),sz(2),n_elements(imagelist))
	images(*,*,0) = image
	for i = 1,n_elements(imagelist) - 1 do images(*,*,i) = 	open_square_image(filename = imagelist(i))
endif

if keyword_set(locfile) then begin
	filename = locfile
	f1 = 1
	if keyword_set(raw) then rh = read_rawheader(filename = locfile) else rh = read_genheader(filename = locfile)
	;------------------------------------------------
	;see if there is an associated dicom fileheader
	;-----------------------------------------------
	dfile = findfile(locfile + '.dicom',count = count)
	if count gt 0 then begin
		dfile = uncompress(dfile(0))
		dhx = read_dicomheader(filename = dfile,dh = dh,rh = rh)
		template = dfile
	endif
	if not(keyword_set(images)) then lh = read_header(filename = locfile,get_cube = images)
	if not(keyword_set(dh)) then begin
		if keyword_set(raw) then rh = read_rawheader(filename = locfile) else rh = read_genheader(filename = locfile)
		sdate = invert_systime(rh.exam.ex_datetime)
		year = sdate.year
		month = sdate.month
		day = sdate.day
		if strlen(month) eq 1 then month = '0' + month
		if strlen(day) eq 1 then day = '0' + day
		anat_date = 'struc(' + month + '-' + day + '-' + year + ') '
		plane = fix(rh.image.im_plane)
		if plane gt 6 then begin
			plane2 = strupcase(string(rh.series.se_desc))
			axpos = strpos(plane2,'AX')
			sagpos = strpos(plane2,'SAG')
			corpos = strpos(plane2,'COR')
			if sagpos ge 0 then plane = 4	;'sagittal'
			if corpos ge 0 then plane = 6	;'coronal'
			if axpos ge 0 then  plane = 2	;'axial'
		endif

		Case plane of
			2: plane_name = 'Ax'
			4: plane_name = 'Sag'
			6: plane_name = 'Cor'
			else: plane_name = 'Ax'
		endcase

		pix 		= 	strtrim(rh.image.im_pixsize_x,2) + '\' + strtrim(rh.image.im_pixsize_y,2)
		id  		= 	strtrim(rh.exam.ex_patid,2)
		accession	= 	strtrim(rh.exam.ex_reqnum,2)
		name 		= 	strtrim(eliminate_badchars(rh.exam.ex_patname),2)
		timestamp	= 	strtrim(rh.exam.ex_datetime,2)
		;study_description = 	strtrim(strtrim(string(rh.exam.ex_desc),2) + ' ' + anat_date + tasktext1,2)
		;study_description = 	strtrim(string(rh.exam.ex_desc),2) + ' ' + strtrim('FMRI '  + tasktext1,2)
		study_description = 	'FMRI ' + anat_date
		;series_description = 	strtrim(strtrim(string(rh.series.se_desc),2) + ' ' + anat_date  + tasktext1,2) + fusion
		;series_description = 	strtrim('FMRI ' + strtrim(tasktext1,2),2) + ' ' + strtrim(string(rh.series.se_desc),2)
		;series_description = 	strtrim('FMRI ' + strtrim(tasktext1,2),2)
		;series_description = 	strtrim('FMRI ' + anat_date + tasktext1,2)
		series_description = 	plane_name + ' ' + strtrim(tasktext1,2)
		history 	= 	strtrim(string(eliminate_badchars(rh.exam.ex_hist)),2) + ' ' + anat_date
		landmark 	= 	strtrim(rh.series.SE_LMHOR,2)
		series_number = 	strtrim(rh.series.se_no,2)
		Institution =	strtrim(eliminate_badchars(rh.exam.ex_hospname),2)
		thick	=  rh.image.im_slthick
		gap = rh.image.im_scanspacing
	endif
endif
 

if keyword_set(analyze) then begin
	filename = analyze
	f1 = 0
	ah = read_analyze_header(filename = filename,volumein = cube)
	;ah = lh.an
	if not(keyword_set(images)) then images = cube
	pix 	= 	strtrim(ah.x_size,2) + '\' + strtrim(ah.y_size,2)
	thick	=  ah.z_size
	gap = 0
endif 


if keyword_set(dh) then begin
		pix 		= 	dh.Pixel_Spacing.value
		id  		= 	strtrim(dh.patient_id.value,2)
		accession	= 	strtrim(dh.accession_number.value,2)
		name 		= 	strtrim(dh.patient_name.value,2)
		name = replace_string(name,char = '^',rep = ',')


		date 		= 	strtrim(dh.study_date.value,2)
		year 		= 	fix(strmid(date,0,4))
		month		= 	fix(strmid(date,4,2))
		day 		= 	fix(strmid(date,6,2))
		anat_date = 'struc(' + strtrim(month,2) + '-' + strtrim(day,2) + '-' + strtrim(year,2) + ') '
		Orientation_value=''
		if is_tag(tag_names(dh),'patient_orientation') then orientation_value = dh.Patient_Orientation.value
		case orientation_value of
			'L\P':	plane_name = 'Ax'
			'P\F':	plane_name = 'Sag'
			'L\F':	plane_name = 'Cor'
			'P\L':	plane_name = 'Ax'
			'F\P':	plane_name = 'Sag'
			'F\L':	plane_name = 'Cor'
			else:	plane_name = 'Ax'
		endcase
		timestamp  	= 	strtrim(long(forward_systime(month,day,year)),2)
		;study_description = 	strtrim(dh.study_description.value,2) + tasktext1 
		study_description = 	'FMRI ' + anat_date
		;series_description = 	strtrim(dh.series_description.value,2) + tasktext1
		series_description = 	plane_name + ' ' + strtrim(tasktext1,2)
		;history 	= 	strtrim(dh.Additional_Patient_History.value,2) + ' ' + anat_date
		history 	= 	'FMRI ' + anat_date
		landmark 	= 	strtrim(dh.Frame_of_Reference_UID.value,2)
		landmark 	= 	str_sep(landmark,'.')
		landmark 	= 	landmark(n_elements(landmark) - 2)  + '.' + landmark(n_elements(landmark) - 1)
		series_number 	= 	strtrim(dh.Series_Number.value,2)
		Institution 	=	strtrim(dh.Institution_Name.value,2)
		thick		=  	float(dh.slice_thickness.value)
		gap 		= 	float(dh.spacing_between_slices.value) - float(dh.slice_thickness.value)
endif

if keyword_set(oh) then begin
		;timestamp = 		strtrim(oh.exam.ex_datetime,2)
		sdate0 = invert_systime(oh.exam.ex_datetime)
		year0 = sdate0.year
		month0 = sdate0.month
		day0 = sdate0.day
		if strlen(month0) eq 1 then month0 = '0' + month0
		if strlen(day0) eq 1 then day0 = '0' + day0
		func_date = 'fMRI(' + month0 + '-' + day0 + '-' + year0 + ') '
		;study_description = study_description  + ' ' + tasktext2
		study_description = 	func_date
		; + ' ' + strtrim(string(oh.exam.ex_desc),2)
		;series_description = series_description + ' + ' + func_date + tasktext2 + ' ' + strtrim(string(oh.series.se_desc),2)
		;series_description = series_description + ' + ' + func_date + tasktext2
		series_description = 	plane_name + ' ' + strtrim(tasktext2,2) 
		;history 	= 	history + ' ' + func_date ;+ ' ' + strtrim(string(oh.exam.ex_hist),2)
		o_id  		= 	strtrim(oh.exam.ex_patid,2)
		o_name 		= 	strtrim(oh.exam.ex_patname,2)
		ph_target = {id:id,name:name}
		ph_object = {id:o_id,name:o_name}
		response = check_patients(target = ph_target,object = ph_object)
		ok = response.action
		if not(response.id_check) and not(response.name_check) then begin
			ok = 0
			print,'Sorry, Double mismatch'
		endif
		if not(ok) then begin
			print,response
			print,'Patient Header Mismatch'
			print,'No Dicom File Fusion Performed'
			cd,here
			error = 1
			return
		endif
endif


sz = size(images)
z = 1
x = sz(1)
y = sz(2)
if sz(0) gt 2 then z = sz(3)
zsize = z
case plane_name of
	'Ax': run = run
	'Sag': run = run + 10
	'Cor': run = run + 20
	else: run = run
endcase
series_number = run

file= {fixed:f0,value:filename,title:'Filename'}
institution= {fixed:f0,value:institution,title:'Institution'}
thick = {fixed:f3,value:thick,title:'Slice Thickness'}
gap = {fixed:f3,value:gap,title:'Slice Gap'}
x= {fixed:f0,value:x,title:'x size'}
y= {fixed:f0,value:y,title:'y size'}
z= {fixed:f0,value:z,title:'z size'}
pix= {fixed:f3,value:pix,title:'pixel dimensions'}
series_number= {fixed:f2,value:series_number,title:'Series Number'}
timestamp= {fixed:f3,value:timestamp,title:'Time_date_stamp'}
landmark= {fixed:f3,value:landmark,title:'Landmark'}
name= {fixed:f1,value:name,title:'Patient Name'}
id= {fixed:f1,value:id,title:'Patient ID'}
accession= {fixed:f1,value:accession,title:'Accession Number'}
date= {fixed:f3,value:(year + month + day),title:'Date of Study'}
month= {fixed:f1,value:month,title:'Study Date:  Month'}
day= {fixed:f1,value:day,title:'Day'}
year= {fixed:f1,value:year,title:'Year'}
study_description= {fixed:f2,value:study_description,title:'Study Description'}
series_description= {fixed:f2,value:series_description,title:'Series Description'}
history= {fixed:f2,value:history,title:'Patient History'}
action = {fixed:f3,value:0,title:'Continue with Conversion'}

;------------------------------------------
;Define the substitution header structure
;------------------------------------------
sh = {   filename:file, $
	name:name,$
	id:id, $
	history:history, $
	study_description:study_description,$
	series_description:series_description,$
	date:date,$
	month:month,$
	day:day,$
	year:year,$
	accession:accession,$
	institution:institution,$
	timestamp:timestamp,$
	landmark:landmark,$
	series_number:series_number,$
	x:x,$
	y:y,$
	z:z,$
	pix:pix,$
	thick:thick,$
	gap:gap, $
	action:action}

;-----------------------------------------------------------
;No UIDs can lead with 0
;WFU accession numbers need 8 digits including leading 0
;-----------------------------------------------------------
;sh.accession.value = strtrim(long(sh.accession.value),2)

;---------------------------------------------
;Pop open the substitution header widget
;--------------------------------------------
if not(keyword_set(quiet)) then update_dicom_widget,header = sh
if keyword_set(send_sh) then sh = send_sh
if keyword_set(quiet) then sh.action.value = 1
if not(sh.action.value) then begin
	print,'Exiting Dicom Converter'
	error = 1
	return
end
;-------------------------------------------------------------
;Limit the field sizes otherwise header won't write properly
;------------------------------------------------------------
maxlen = 60
sh.history.value = strmid(sh.history.value,0,maxlen)
sh.study_description.value = strmid(sh.study_description.value,0,maxlen)
sh.series_description.value = strmid(sh.series_description.value,0,maxlen)
;sh.accession.value = strmid(strtrim(long(sh.accession.value),2),0,7)


if not(keyword_set(nowrite)) then begin
	;-------------------------------------------
	;Setup the output directory
	;-------------------------------------------
	cd,outputdir
	ptdir = sh.name.value + '_' + strtrim(sh.id.value,2)
	ptdir = replace_string(ptdir,char = ' ',rep = '_')
	ptdir = replace_string(ptdir,char = '^',rep = '_')
	ptdir = replace_string(ptdir,char = ',',rep = '_')
	ptdir = replace_string(ptdir,char = '?',rep = '_')
	ptdir = replace_string(ptdir,char = '@',rep = '_')
	ptdir = replace_string(ptdir,char = '$',rep = '_')
	ptdir = replace_string(ptdir,char = '|',rep = '_')
	ptdir = replace_string(ptdir,char = '#',rep = '_')
	ptdir = replace_string(ptdir,char = '!',rep = '_')
	ptdir = replace_string(ptdir,char = '&',rep = '_')
	ptdir = replace_string(ptdir,char = '*',rep = '_')
	ptdir = replace_string(ptdir,char = '>',rep = '_')
	ptdir = replace_string(ptdir,char = '<',rep = '_')
	ptdir = replace_string(ptdir,char = '/',rep = '_')
	ptdir = replace_string(ptdir,char = '\',rep = '_')

	spawn, "mkdir "+ ptdir
	spawn, "chmod 775 "+ ptdir
	cd,ptdir
	sdir = 'series' + strtrim(90 + fix(sh.series_number.value),2)
	spawn, "mkdir "+ sdir
	spawn, "chmod 775 "+ sdir
	cd,sdir
	output_ptdir = outputdir + ptdir + !separator + sdir + !separator

	;-----------------------------------------------------------
	;Compute the bits size for this image
	;There is a problem with floats in the DICOM conversion
	;-----------------------------------------------------------
	bits_size =strtrim(fix(bitsize(images)),2)
	if bits_size gt 16 then begin
		print,'converting images to integer'
		images = fix(images)
		bits_size =strtrim(fix(bitsize(images)),2)
	endif
	print,'bits_size = ',bits_size

	;-------------------------------------------
	;Now perform the conversion
	;and write the files
	;-------------------------------------------
	print,'Outputing dicom Images to:  ' + output_ptdir
	outputlist = strarr(zsize)
	for i = 0,zsize - 1 do begin
	;for i = 0,1 do begin	

		dhnew = convert2dicom1(rh = rh,sh = sh,image_number= i + 1,dh = dh)
		;-------------------------------------------
		;Set the bytesize and bits size for this image
		;-------------------------------------------
		dhnew.bits_allocated.value = bits_size
		dhnew.bits_stored.value = bits_size

		outuid = dhnew.SOP_Instance_UID.value
		outuid = str_sep(outuid,'.')
		outf = ''
		for j = 0,n_elements(outuid) - 2 do outf = outf + outuid(j) + '.'
		imnum = strtrim(outuid(n_elements(outuid)-1),2)
		if strlen(imnum) lt 2 then imnum = '00' + imnum
		if strlen(imnum) lt 3 then imnum = '0' + imnum
		outputfile = outf + imnum
		outputlist(i) = output_ptdir + outputfile
		print,'writing ' + outputfile
		write_dicom,image = reform(images(*,*,i)),dh = dhnew,$
			filename = outputfile,outputdir = output_ptdir,template = template
	endfor
endif

cd,here
end
