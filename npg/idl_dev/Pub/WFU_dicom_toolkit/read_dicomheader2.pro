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








function read_dicomheader2,filename = filename,image = image,coords = coords,error = error,numimages = numimages
;--------------------------------------------------------------------------
;This program will read a dicom file and return some useful descriptors
;dh = read_dicomheader2(filename = dicomfile,image = image,coords = coords, numimages = numimages)
;image = image in dicom file.  For mosaics it gets converted to a 3D volume.
;dh.an = analyze header
;coords is the coords which should go into csmemp.loc files
;Only send numimages for Philips fMRI files
;These files don't have the number of slices in the dicom header, so we compute it from the number of temporal positions and numimages
;For Philips fMRI files, send numimages (# of files in the directory) so that the program doesn't have to recompute it with each read
;J. Maldjian 5-8-03
;--------------------------------------------------------------------------


error = 0
if not(keyword_set(filename)) then filename = pickfile(title = 'Choose a Dicom Header',filter = '*.dcm')

good = query_dicom(filename)
if not(good) then begin
	error = 1
	return,0
endif
image = read_dicom(filename)
s = size(image)
xdim = s(1)
ydim = s(2)
an = make_analyze_header()
h = {x:xdim,y:ydim,z:0l,t:1l,date:'',series_time:'',description:'',name:'',id:'',$
		slthick:0.0,gap:0.0,pix:[0.0,0.0,0.0], $
		tr:0l,te:0l,ti:0l,flip:0,$
		plane:0,fov:0.0,dfov_rect:0.0,pos:[0.0,0.0,0.0],ras:fltarr(3,3),$
		psd_name:'',series_number:'',study_id:'',tesla:'',$
		plane_name:'',modality:'',study_description:'',landmark:0.0,systime:0.0,mosaic:0,an:an}

header=	{date:'Study Date', $		;19990112
	series_description:'Series Description',  $  	; 'POST GD AX SPGR WHOLE HEAD'
	study_description:'Study Description',	$
	series_time:'Series Time',	$	;'192540.437000'
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
	number_of_temporal_positions:'Number of Temporal Positions', $ ;  field in Philips fMRI files for number of volumes
	frame_of_reference_uid: 'Frame of Reference UID'}	;'1.2.840.113619.2.1.1.2702429029.809.936099446.28'

	field = get_dcm_value(filename = filename,description = header.date)
	;field = string(field, format = '(A30)')
	;rh.series.se_desc = byte(field)
	h.date = strtrim(field,2)
	if strlen(h.date) eq 8 then begin
		year = fix(strmid(h.date,0,4))
		month = fix(strmid(h.date,4,2))
		day = fix(strmid(h.date,6,2))
		systhen = forward_systime(month,day,year)
		h.systime = systhen
	endif

	field = get_dcm_value(filename = filename,description = header.series_time)
	h.series_time = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.study_description)
	h.study_description = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.modality)
	h.modality = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.series_description)
	field = string(field, format = '(A30)')
	h.description = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.patient_name)
	;---------------------------------------
	;exchange '^' with ',' in this field
	;-------------------------------------
	field = replace_string(field,char = '^',rep = ',')
	field = string(field, format = '(A25)')
	h.name = strtrim(field,2)

	field = get_dcm_value(filename = filename,description = header.patient_id)
	field = string(field, format = '(A13)')
	field = replace_string(field,char = '^',rep = ' ')
	h.id = strtrim(field,2)


	field = get_dcm_value(filename = filename,description = header.patient_weight)
	field = float(field)



	field = get_dcm_value(filename = filename,description = header.sequence_name)
	field = string(field, format = '(A14)')
	h.psd_name = strtrim(field,2)
	field = get_dcm_value(filename = filename,description = header.slice_thickness)
	field = float(field)
	;--------------------------------------------
	;Sometimes negative values come through
	;--------------------------------------------
	field=abs(field)
	h.slthick = field

	field = get_dcm_value(filename = filename,description = header.tr)
	field = long(float(field) * 1000)
	h.tr = field

	field = get_dcm_value(filename = filename,description = header.te)
	field =long(float(field) * 1000)
	h.te = field

	field = get_dcm_value(filename = filename,description = header.ti)
	field = long(float(field) * 1000)
	h.ti = field

	field = get_dcm_value(filename = filename,description = header.flip_angle)
	field = fix(field)
	h.flip = field

	field = get_dcm_value(filename = filename,description = header.study_id)
	h.study_id = strtrim(field,2)


	field = get_dcm_value(filename = filename,description = header.series_number)
	h.series_number = strtrim(field,2)
	field = fix(field)


	field = get_dcm_value(filename = filename,description = header.echo_number)
	field = fix(field)

	field = get_dcm_value(filename = filename,description = header.spacing_between_slices)
	field = float(field)
	;---------------------------------------------------------------
	;Sometimes negative gap values come through or 0 comes through
	;---------------------------------------------------------------
	field = abs(field) > h.slthick
	slice_add = field
	h.gap = field - h.slthick

	field = get_dcm_value(filename = filename,description = header.tesla)
	h.tesla = strtrim(field,2)


;------------------------------------------
;Set pixel dimensions,fov, and matrix sizes
;-----------------------------------------

	field = get_dcm_value(filename = filename,description = header.pixel_spacing)
	pixels = str_sep(field,'\')
	if pixels(0) eq '' then pixels = ['1','1']
	x_size = float(pixels(0))
	y_size = float(pixels(1))
	h.pix = [x_size,y_size,(h.slthick + h.gap)]

	bad_dims = 0	

	field = get_dcm_value(filename = filename,description = header.acquisition_matrix)
	sf = size(field)
	if sf(0) eq 0 then field = [0,xdim,ydim,0]
	dim_x = float(field(1))
	dim_y = float(field(2))
	if dim_x eq 0 then begin
		dim_x = float(field(0))
		dim_y = float(field(3))	
	endif
	acquisition_matrix = [dim_x,dim_y]

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
	manufacturer = strupcase(get_dcm_value(filename = filename,description = header.manufacturer))
	mosaic = (strpos(strupcase(get_dcm_value(filename = filename,description = "Image Type")),'MOSAIC') + 1) < 1
	h.mosaic = mosaic

	if mosaic then begin
		xy = fix(acquisition_matrix)
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
	
	if keyword_set(numimages) and nlocs eq 0 then nlocs = numimages
	if nlocs eq 0 then begin
		dcmlist = generate_dicom_filelist(filename = filename)
		nlocs = n_elements(dcmlist)
	endif
	field = get_dcm_value(filename = filename,description = header.number_of_temporal_positions)
	;-------------------------------------------
	;Check if this is a Philips fMRI file
	;------------------------------------------
	if field ne '' then begin
		h.t = long(field)
		nlocs = nlocs/h.t
	endif		
	nlocs = nlocs > 1
	h.z = fix(nlocs)
	dfov = float(columns * x_size)
	dfov_rect = float(rows * y_size)
	h.fov = dfov
	h.dfov_rect = dfov_rect
		
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

	h.plane = fix(plane)

	field = get_dcm_value(filename = filename,description = header.image_position)
	RAS = str_sep(field,'\')
	if n_elements(ras) lt 3 then ras = ['0','0','0']
	R = float(ras(0))
	A = float(ras(1))
	S = float(ras(2))
	h.pos = [R,A,S]

	field = get_dcm_value(filename = filename,description = header.frame_of_reference_uid)
	landmark = field(0)
	if strtrim(landmark,2) eq '' then landmark = '0.0.0.0'
	landmark = str_sep(landmark,'.')
	l1 = landmark(n_elements(landmark) - 2)
	l1 = strmid(l1,strlen(l1) - 4,strlen(l1))
	if ( (fix(l1) gt 9999.0) or (fix(l1) lt 0) ) then $
			l1 = strtrim(fix(double(landmark(n_elements(landmark) - 2)) mod 100),2)
	landmark = float(l1  + '.' + landmark(n_elements(landmark) - 1))
	h.landmark = landmark

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

;-----------------------
;Load up analyze header
;-----------------------
an = make_analyze_header()
h.an.dims = 3
h.an.x_dim = h.x
h.an.y_dim = h.y
h.an.z_dim = h.z
h.an.t_dim = h.t
if h.t gt 1 then h.an.dims = 4
;---------------------------------------------------------------------------
;Check if the image is an unsigned integer and convert to signed integer
;---------------------------------------------------------------------------
if typecode(image) eq 12 then image = fix(image)
h.an.datatype = an_datatype(image)
case h.an.datatype of
	 2:h.an.glmax	= 255
	 4:h.an.glmax	= 32767
	 8:h.an.glmax	= (2L^31)-1
	16:h.an.glmax	= 1 ; because glmax is a long integer, not a float
	64:h.an.glmax	= 1
endcase
h.an.glmin = 0
h.an.bits  = 8 * bytesize(idl_datatype(h.an.datatype))
h.an.x_size = h.pix(0)
h.an.y_size = h.pix(1)
h.an.z_size = h.pix(2) 
h.an.landmark = h.landmark
;----------------------------------------------------------------------
;Compute magnet affine transform (display --> magnet acquisition space)
;----------------------------------------------------------------------
if h.z gt 1 then begin
	mt = coords2mt(coords = coords,dims = [h.x,h.y,h.z])
	h.an.magnet_transform = mt
endif
h.an.scale = 1
return,h
end
