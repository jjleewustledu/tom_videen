;function sort_imagelist,imagelist
;template = imagelist(0)
;parse_filename,f = template,base = base,path = fpath
;base = strupcase(base)
;------------------
;Check if ESI format
;------------------
;if (strpos(base,'E') ge 0) and (strpos(base,'S') ge 1) and (strpos(base,'I') ge 2) then begin
					


function get_dcm_value,filename = filename,description = description,print = print,group = group,element = element

;----------------------------------------------------------------------------
;This program will return the values of individual elements in a dicom header
;description should = text description of the field you want from
;the dicom_dictionary
;example:
;sl_thick = get_dcm_value(filename = dicom_file,description = 'Slice Thickness')
;you can also provide string values for group and element
;J. Maldjian  7-24-2000
;---------------------------------------------------------------------------

value = ''
obj = OBJ_NEW('IDLffDICOM')
read = obj->Read(filename)
if not(keyword_set(group)) and not(keyword_set(element)) then begin
	ref = obj->GetReference(DESCRIPTION=description)
	group = obj->GetGroup(REFERENCE = ref[0])
	element = obj->GetElement(REFERENCE = ref[0])
	ptr = obj->GetValue(group(0),element(0))
	endif else begin
	ptr = obj->GetValue(convert2base(group(0),base = 16),convert2base(element(0),base = 16))
endelse
OBJ_DESTROY,obj
s = size(ptr)
if s(0) eq 0 then return,''
valid_pointer = ptr_valid(ptr)
if valid_pointer(0) then value = *ptr(0)
ptr_free,ptr
if keyword_set(print) then begin
	print,group,element
	help,value
	print,value
endif
return,value
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



function read_dicomheader ,filename = filename,error = error,coords = coords,image = image,rh = rh,chop = chop,dh = dh,mosaic = mosaic

;--------------------------------------------------------------------------
;This program will read a dicom file and return some useful descriptors
;It will also fill a genesis header with appropriate values from the dicom header
;including magnet transform information
;coords is the coords which should go into csmemp.loc files
;-------------------------------------------------------------------------

error = 0
if not(keyword_set(filename)) then filename = pickfile(title = 'Choose a Dicom Header',filter = '*.dcm')

good = query_dicom(filename)
if not(good) then begin
	error = 1
	return,0
endif
;print, filename
dh = parse_dicom(filename = filename)
dtags = tag_names(dh)
image = read_dicom(filename)
s = size(image)
xdim = s(1)
ydim = s(2)
h = {x:xdim,y:ydim,z:0l,date:'',description:'',name:'',id:'',$
		slthick:0.0,gap:0.0,pix:[0.0,0.0,0.0], $
		tr:0l,te:0l,ti:0l,flip:0,$
		plane:0,fov:0.0,dfov_rect:0.0,pos:[0.0,0.0,0.0],ras:fltarr(3,3),$
		psd_name:'',series_number:'',study_id:'',tesla:'',$
		plane_name:'',modality:'',study_description:''}

header=	{date:'Study Date', $		;19990112
	series_description:'Series Description',  $  	; 'POST GD AX SPGR WHOLE HEAD'
	study_description:'Study Description',	$
	manufacturer:'Manufacturer',$
	modality:'Modality',	$
	patient_name: 'Patient Name', $	 		;'YERKES^CAROL T
	patient_id: 'Patient ID', $			; '04371246'
	patient_weight:'Patient Weight',$		;  ' 58.967000'
	sequence_name:'Sequence Name', $	 	;'fgre3d'
	slice_thickness:'Slice Thickness', $		; '1.500000'
	TR:'Repetition Time',   $			; ' 11.100000'
	TE:'Echo Time',		$			;'2.000000'
	TI:'Inversion Time',	$			;
	number_of_averages:'Number of Averages'	, $	; '1.00000'
	echo_number:'Echo Number'	,$		; ' 1'
	spacing_between_slices:'Spacing Between Slices'	, $	; '1.500000'
	version:'Software Version'	,$		; '07'
	acquisition_matrix:'Acquisition Matrix'	,$	;uint(4)  [0 256 192 0]
	flip_angle:'Flip Angle'	,$			; '10'
	study_id : 'Study ID'	,$			;'2731154'
	series_number:'Series Number'	,$		;'6 '
	image_number:'Image Number'	,$		;'1 '
	image_orientation:'Image Orientation' ,$	;'1.00\0.0\0.0\0.0\1.0\0.0'
	patient_orientation:'Patient Orientation'  ,$	; 'L\P' = ax, 'P\F ' = sag ; 'L\F' = cor
	image_position:'Image Position' ,$ 		; ' -120.000000\ -114.199997\-71.099998'
	images_in_acquisition:'Images in Acquisition'	,$ ; '124'
	slice_location:'Slice Location'	,$		; '-71.0999984741'
	rows:'Rows'			,$			;uint 256
	columns:'Columns'		,$			;uint 256
	pixel_spacing:'Pixel Spacing'	,$			;' 0.9375000000\0.9375000000'
	tesla: 'Magnetic Field Strength' , $ 			; '15000 '
	frame_of_reference_uid: 'Frame of Reference UID'}	;'1.2.840.113619.2.1.1.2702429029.809.936099446.28'

rh = mk_genheader_lx()


	field = get_dcm_value(filename = filename,description = header.date)
	;field = string(field, format = '(A30)')
	;rh.series.se_desc = byte(field)
	h.date = strtrim(field,2)
	if strlen(h.date) eq 8 then begin
		year = fix(strmid(h.date,0,4))
		month = fix(strmid(h.date,4,2))
		day = fix(strmid(h.date,6,2))
		systhen = forward_systime(month,day,year)
		rh.exam.ex_datetime = systhen
	endif

	field = get_dcm_value(filename = filename,description = header.study_description)
	h.study_description = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.modality)
	h.modality = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.series_description)
	field = string(field, format = '(A30)')
	rh.series.se_desc = byte(field)
	h.description = strtrim(field,2)


	field = get_dcm_value(filename = filename,description = header.patient_name)
	;---------------------------------------
	;exchange '^' with ',' in this field
	;-------------------------------------
	field = replace_string(field,char = '^',rep = ',')
	field = string(field, format = '(A25)')
	rh.exam.ex_patname = byte(field)
	h.name = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.patient_id)
	field = replace_string(field,char = '^',rep = ' ')
	field = string(field, format = '(A13)')
	rh.exam.ex_patid = byte(field)
	h.id = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.patient_weight)
	field = float(field)
	rh.exam.ex_patweight = field	



	field = get_dcm_value(filename = filename,description = header.sequence_name)
	field = string(field, format = '(A14)')
	rh.image.im_psd_iname = byte(field)
	h.psd_name = strtrim(field,2)



	field = get_dcm_value(filename = filename,description = header.slice_thickness)
	field = float(field)
	;--------------------------------------------
	;Sometimes negative values come through
	;--------------------------------------------
	field=abs(field)
	rh.image.im_slthick = field
	h.slthick = field

	field = get_dcm_value(filename = filename,description = header.tr)
	field = long(float(field) * 1000)
	rh.image.im_tr = field
	h.tr = field

	field = get_dcm_value(filename = filename,description = header.te)
	field =long(float(field) * 1000)
	rh.image.im_te = field 
	h.te = field

	field = get_dcm_value(filename = filename,description = header.ti)
	field = long(float(field) * 1000)
	rh.image.im_ti = field 
	h.ti = field

	field = get_dcm_value(filename = filename,description = header.flip_angle)
	field = fix(field)
	rh.image.im_mr_flip = field
	h.flip = field

	field = get_dcm_value(filename = filename,description = header.study_id)
	h.study_id = strtrim(field,2)


	field = get_dcm_value(filename = filename,description = header.series_number)
	h.series_number = strtrim(field,2)
	field = fix(field)
	rh.image.im_seno = field
	rh.series.se_no = field


	field = get_dcm_value(filename = filename,description = header.echo_number)
	field = fix(field)
	rh.image.im_numecho = field

	field = get_dcm_value(filename = filename,description = header.spacing_between_slices)
	field = float(field)
	;---------------------------------------------------------------
	;Sometimes negative gap values come through or 0 comes through
	;---------------------------------------------------------------
	field = abs(field) > h.slthick
	slice_add = field
	rh.image.im_scanspacing = field - (rh.image.im_slthick)
	;print,'gap ',field, rh.image.im_slthick
	h.gap = field - (rh.image.im_slthick)

	field = get_dcm_value(filename = filename,description = header.tesla)
	h.tesla = strtrim(field,2)


;------------------------------------------
;Set pixel dimensions,fov, and matrix sizes
;-----------------------------------------

	field = get_dcm_value(filename = filename,description = header.pixel_spacing)
	pixels = str_sep(field,'\')
	x_size = float(pixels(0))
	y_size = float(pixels(1))
	h.pix = [x_size,y_size,(h.slthick + h.gap)]

	bad_dims = 0	

	field = get_dcm_value(filename = filename,description = header.acquisition_matrix)
	sf = size(field)
	if sf(0) eq 0 then field = [0,xdim,ydim,0]
	dim_x = float(field(1))
	dim_y = float(field(2))

	rows = get_dcm_value(filename = filename,description = header.rows)
	columns = get_dcm_value(filename = filename,description = header.columns)
	rows = fix(rows)
	columns = fix(columns)
	
	if rows ne xdim or columns ne ydim then bad_dims = 1
	if (dim_x * dim_y * rows * columns) eq 0 then bad_dims = 1
	if bad_dims then begin
		dim_x = float(xdim)
		dim_y = float(ydim)
		rows = fix(dim_y)
		columns = fix(dim_x)
	endif

	field = get_dcm_value(filename = filename,description = header.images_in_acquisition)
	nlocs = fix(field)
	if keyword_set(chop) or keyword_set(siemens) then manufacturer = 'SIEMENS'
	manufacturer = strupcase(get_dcm_value(filename = filename,description = header.manufacturer))
	mosaic = (strpos(strupcase(get_dcm_value(filename = filename,description = "Image Type")),'MOSAIC') + 1) < 1
	if mosaic then begin
		xy = fix(strsplit(dh.acquisition_matrix.value,/extract))
		sz = size(image)
		maxdim = max(xy)
		nslices = ((1.0 *sz(1)) * sz(2) ) / (maxdim * maxdim)
		nlocs = nslices
		dim_x = float(maxdim)
		dim_y = float(maxdim)
		rows = fix(dim_y)
		columns = fix(dim_x)
		h.x = dim_x
		h.y = dim_y
		image = siemens_mosaic(maxdim = maxdim,image = image)
	endif

	;if nlocs eq 0 and manufacturer eq 'SIEMENS' then begin
	;	imlist = expand_list(filename = filename,/chop,ext = '.dcm')
	;	nlocs = n_elements(imlist)
	;endif
	if nlocs eq 0 then begin
		parse_filename,f = filename,base = base,path = fpath,ext = ext
		imlist = findfile(fpath + base + '*[0-9]*',count = nlocs)
		if nlocs eq 0 then imlist = findfile(fpath + base + '*' + ext,count = nlocs)
	endif
	rh.image.im_slquant = fix(nlocs)
	rh.series.se_numimages = long(nlocs)
	h.z = fix(nlocs)



	dfov = float(columns * x_size)
	dfov_rect = float(rows * y_size)
	h.fov = dfov
	h.dfov_rect = dfov_rect
		

	rh.image.im_pixsize_x  = x_size
	rh.image.im_pixsize_y = y_size
	rh.image.im_imatrix_x = columns
	rh.image.im_imatrix_y = rows
	rh.image.im_dfov = dfov
	rh.image.im_dfov_rect = dfov_rect
	rh.image.im_dim_x = dim_x
	rh.image.im_dim_y = dim_y




	field = get_dcm_value(filename = filename,description = header.patient_orientation)
	LFP = str_sep(field,'\')
	ax1 = strtrim(lfp(0),2)
	ax2 = ax1
	if n_elements(lfp) gt 1 then  ax2 = strtrim(lfp(1),2)
	plane = 2	; 'axial'
	if (ax1 eq 'L' and ax2 eq 'P') or (ax2 eq 'L' and ax1 eq 'P') then plane = 2	;'axial'
	if (ax1 eq 'P' and ax2 eq 'F') or (ax2 eq 'P' and ax1 eq 'F') then plane = 4	;'sagittal'
	if (ax1 eq 'L' and ax2 eq 'F') or (ax2 eq 'L' and ax1 eq 'F') then plane = 6	; 'coronal'
	if plane eq 2 then h.plane_name = 'axial'
	if plane eq 4 then h.plane_name = 'sagittal'
	if plane eq 6 then h.plane_name = 'coronal'
	if plane eq 8 then h.plane_name = 'oblique'	

	rh.image.im_plane = fix(plane)
	h.plane = fix(plane)

	field = get_dcm_value(filename = filename,description = header.image_position)
	RAS = str_sep(field,'\')
	if n_elements(ras) lt 3 then ras = ['0','0','0']
	R = float(ras(0))
	A = float(ras(1))
	S = float(ras(2))
	h.pos = [R,A,S]

	;rh.series.se_start_loc = S


	field = get_dcm_value(filename = filename,description = header.frame_of_reference_uid)
	landmark = field(0)
	if strtrim(landmark,2) eq '' then landmark = '0.0.0.0'
	landmark = str_sep(landmark,'.')
	l1 = landmark(n_elements(landmark) - 2)
	l1 = strmid(l1,strlen(l1) - 4,strlen(l1))
	;l1 = strtrim(fix(1.0*landmark(n_elements(landmark) - 2)),2)
	if ( (fix(l1) gt 9999.0) or (fix(l1) lt 0) ) then $
			l1 = strtrim(fix(double(landmark(n_elements(landmark) - 2)) mod 100),2)
	landmark = float(l1  + '.' + landmark(n_elements(landmark) - 1))
	
	rh.series.se_lmhor = landmark

	coords = fltarr(3,3,nlocs)
	if plane eq 2 then begin
		tlhc = [-R,-A,S]
		trhc = [ (tlhc(0) - dfov), tlhc(1),tlhc(2)]
		brhc = [ (tlhc(0) - dfov),(tlhc(1) - dfov), tlhc(2)]
	endif
	if plane eq 4 then begin 
		tlhc = [-R,-A,S]
		trhc = [tlhc(0), tlhc(1) - dfov, tlhc(2)]
		brhc = [tlhc(0),tlhc(1) - dfov,tlhc(2) - dfov] 
	endif

	if plane eq 6 then begin
		tlhc = [-R,-A,S]
		trhc = [(tlhc(0) - dfov),tlhc(1),tlhc(2)]
		brhc = [(tlhc(0) - dfov),tlhc(1),(tlhc(2) - dfov)]
	endif
		
	rh.image.im_tlhc = tlhc
	rh.image.im_trhc = trhc
	rh.image.im_brhc = brhc
	coords(*,0,0) =  tlhc
	coords(*,1,0) = trhc
	coords(*,2,0) = brhc
	k = coords(*,*,0)
	h.ras = reform(k)
	for i = 0,nlocs -1 do coords(*,*,i) = k

	if plane eq 2 then begin
		direction = 1.0
		if tlhc(2) gt 40 then direction = -1.0
		for i = 1,nlocs -1 do coords(2,*,i) = coords(2,*,i-1) + (direction * slice_add)
	endif
		
	if plane eq 4 then begin
		direction = 1.0
		if tlhc(0) gt 10 then direction = -1.0
		for i = 1,nlocs -1 do coords(0,*,i) = coords(0,*,i-1) + (direction * slice_add)
	endif
	
	if plane eq 6 then begin
		direction = 1.0
		if tlhc(1) gt 10 then direction = -1.0
		for i = 1,nlocs -1 do coords(1,*,i) = coords(1,*,i-1) + (direction * slice_add)
	endif

;---------------------------------
;Now Fill the rest of the ge header
;---------------------------------
if is_tag(dtags,'accession_number') then $
	rh.exam.ex_reqnum = 	byte(string(dh.accession_number.value,format = '(A13)'))
if is_tag(dtags,'modality') then $
	rh.exam.ex_typ	=	byte(string(dh.modality.value,format = '(A3)')) 
if is_tag(dtags,'Institution_Name') then $
	rh.exam.ex_hospname=	byte(string(dh.Institution_Name.value,format = '(A34)'))
if is_tag(dtags,'Referring_Physicians_Name') then $
	rh.exam.ex_refphy=	byte(string(dh.Referring_Physicians_Name.value,format = '(A33)'))
if is_tag(dtags,'Station_Name') then $
	rh.series.se_pansysid=	byte(string(dh.Station_Name.value,format = '(A9)'))
	
if is_tag(dtags,'study_description') then $
	rh.series.se_prtcl=	byte(string(dh.study_description.value ,format = '(A26)'))
if is_tag(dtags,'study_description') then $
	rh.exam.ex_desc=	byte(string(dh.study_description.value,format = '(A23)'))
if is_tag(dtags,'series_description') then $
	rh.series.se_desc=	byte(string(dh.series_description.value,format = '(A30)'))
if is_tag(dtags,'Name_of_Physicians_Reading_Study') then $
	rh.exam.ex_diagrad=	byte(string(dh.Name_of_Physicians_Reading_Study.value,format = '(A33)'))
if is_tag(dtags,'patient_name') then $
	rh.exam.ex_patname=	byte(string(dh.patient_name.value,format = '(A25)'))
if is_tag(dtags,'patient_id') then $
	rh.exam.ex_patid=	byte(string(dh.patient_id.value ,format = '(A13)'))
if is_tag(dtags,'patient_weight') then $
	rh.exam.ex_patweight=	float(dh.patient_weight.value)
;Patient_Birthdate		={group:'0010',element:'0030',value:'19570303'}
sexnum = 0
sex = 'F'
if is_tag(dtags,'Patient_Sex') then $
	sex=dh.Patient_Sex.value
if sex eq 'M' then sexnum=1
rh.exam.ex_patsex = sexnum
if is_tag(dtags,'Patient_Age') then $
	rh.exam.ex_patage=	fix(dh.Patient_Age.value)
if is_tag(dtags,'Additional_Patient_History') then $
	rh.exam.ex_hist=	byte(string(dh.Additional_Patient_History.value,format = '(A61)'))


if is_tag(dtags,'Contrast_Bolus_Agent') then $
	rh.image.im_contrastiv=	byte(string(dh.Contrast_Bolus_Agent.value,format = '(A7)'))
;----------------------------------------------------------------------------
;strtrim(rh.image.SE_DESC,2) : Head,Sag,2D,Spin Echo,None
;Scanning_Sequence		={group:'0018',element:'0020',value:'SE'}
;Sequence_Variant		={group:'0018',element:'0021',value:'NONE'}
;Scan_Options		={group:'0018',element:'0022',value:'FC\SP'}
;MR_Acquisition_Type		={group:'0018',element:'0023',value:'2D'}
;-----------------------------------------------------------------------------
if is_tag(dtags,'sequence_name') then $
	rh.image.im_psd_iname	= byte(string(dh.sequence_name.value,format = '(A14)'))
;Angio_Flag		={group:'0018',element:'0025',value:'Y'}
if is_tag(dtags,'slice_thickness') then $
	rh.image.im_slthick=	float(dh.slice_thickness.value)
if is_tag(dtags,'repetition_time') then $
	rh.image.im_tr=		long(long(dh.repetition_time.value)*1000) 
if is_tag(dtags,'echo_time') then $
	rh.image.im_te=		long(long(dh.echo_time.value)*1000)
if is_tag(dtags,'inversion_time') then $
	rh.image.im_ti=		long(long(dh.inversion_time.value)*1000) 
if is_tag(dtags,'Number_of_Averages') then $
	rh.image.IM_NEX=	float(dh.Number_of_Averages.value) 
if is_tag(dtags,'Imaging_Frequency') then $
	rh.image.IM_XMTFREQ=	long(long(dh.Imaging_Frequency.value)*1e7)
;Imaged_Nucleus		={group:'0018',element:'0085',value:'H1'}
if is_tag(dtags,'echo_number') then $
	rh.image.im_echonum=	fix(dh.echo_number.value) 
if is_tag(dtags,'magnetic_field_strength') then begin
	mfs = float(dh.magnetic_field_strength.value)
	if mfs  lt 10 then mfs = mfs * 1000.0
	rh.exam.ex_magstrength=	long(mfs)
endif
if is_tag(dtags,'spacing_between_slices') then $
	rh.image.im_scanspacing= float(dh.spacing_between_slices.value) - float(dh.slice_thickness.value) 

if is_tag(dtags,'Reconstruction_Diameter') then $
	rh.image.im_dfov=	float(dh.Reconstruction_Diameter.value) 
;dh.Reconstruction_Diameter.value = strtrim(rh.image.im_dfov_rect,2)
if is_tag(dtags,'Receiving_Coil') then $
	rh.image.IM_CNAME=	byte(string(dh.Receiving_Coil.value,format = '(A18)'))
;dh.Transmitting_Coil.value =strtrim(rh.image.IM_CNAME,2)
;dh.Acquisition_Matrix.value = strtrim(rh.image.im_imatrix_x,2) + ' ' + strtrim(rh.image.im_imatrix_y,2)
if is_tag(dtags,'flip_angle') then $
	rh.image.im_mr_flip=	fix(dh.flip_angle.value) 
;Variable_Flip_Angle		={group:'0018',element:'1315',value:'N'}
if is_tag(dtags,'SAR') then $
	rh.image.im_saravg=	float(dh.SAR.value)
;dh.Patient_Position.value='HFS'
if is_tag(dtags,'Study_ID') then $
	rh.exam.ex_no=	fix(dh.Study_ID.value)
if is_tag(dtags,'Series_Number') then $
	rh.series.se_no=	fix(dh.Series_Number.value)
;Acquisition_Number		={group:'0020',element:'0012',value:'1'}
if is_tag(dtags,'Image_Number') then $
	rh.image.im_no=		fix(dh.Image_Number.value)



;-----------------------------------------------------------------------------------------------------------------------------------
;RAS coordinates; Handle this upstairs
;----------------------------------------
;plane = 2
;if is_tag(dtags,'Patient_Orientation') then begin $
;	pid = dh.Patient_Orientation.value
;	Case pid of 
;		'L\P': plane = 2
;		'P\F':plane = 4
;		'L\F':plane = 6
;		else: plane = 2
;	endcase
;endif
;rh.image.im_plane = plane
;if is_tag(dtags,'Image_Position_Patient') then begin $
;	RAS = str_sep(strdh.Image_Position_Patient.value,'\')
;	rh.image.im_tlhc = -RAS(0)
;	rh.image.im_trhc = -RAS(1)
;	rh.image.im_brhc = RAS(2)
;endif
;Image_Orientation_Patient		={group:'0020',element:'0037',value:'1.000000\0.000000\0.000000\0.000000\1.000000\0.000000'}
;if is_tag(dtags,'Rows') then $
;	rh.image.im_imatrix_y=	dh.Rows.value	
;if is_tag(dtags,'Columns') then $
;	rh.image.im_imatrix_x=	dh.Columns.value
;if is_tag(dtags,'Pixel_Spacing') then begin
;	pix_space = str_sep(dh.Pixel_Spacing.value,'\')
;	rh.image.im_pixsize_x=	pixspace(0)
;	rh.image.im_pixsize_y=	pixspace(1)
;endif
;-----------------------------------------------------------------------------------------------------------------------------------

if is_tag(dtags,'Images_in_Acquisition') then $
	rh.image.im_slquant=	fix(dh.Images_in_Acquisition.value)
;Position_Reference_Indicator		={group:'0020',element:'1040',value:'NA'}
if is_tag(dtags,'Slice_Location') then begin
	Case plane of
		2: rh.image.im_loc = 	float(dh.Slice_Location.value)
		4: rh.image.im_loc = 	-float(dh.Slice_Location.value)
		6: rh.image.im_loc = 	float(dh.Slice_Location.value)
		else:  rh.image.im_loc = float(dh.Slice_Location.value) 
	endcase
endif




return,h		

end

