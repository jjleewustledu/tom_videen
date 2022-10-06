function  make_analyze_header
;-------------------------------------------------------------------------
;This constructs an analyze type header for compatibility with AIR and SPM
;this assumes 1 bytarr = 1 char
;header measures 348 when written
;ah = make_analyze_header()
;J. Maldjian
;--------------------------------------------------------------------------

typbyte = byte(0)
TYPINT=FIX(0)
TYPLON=LONG(0)
TYPFLT=FLOAT(0)
TYPDBL=double(TYPFLT)

;--------------------------------------
;SPM99 analyze header structure
;--------------------------------------
analyze_hdr = { sizeof_hdr:		typlon		,$
		pad1:			bytarr(28)	,$
		extents:		typlon		,$
		pad2:			bytarr(2)	,$
		regular:		'r'		,$
		pad3:			bytarr(1)	,$
		dims:			typint		,$
		x_dim:			typint		,$
		y_dim:			typint		,$
		z_dim:			typint		,$
		t_dim:			typint		,$
		pad4:			bytarr(20)	,$
		datatype:		typint		,$
		bits:			typint		,$
		pad5:			bytarr(6)	,$
		x_size:			typflt	,$
		y_size:			typflt	,$
		z_size:			typflt	,$
		pad6:			bytarr(48)	,$
		glmax:			typlon		,$
		glmin:			typlon		,$
		descrip:		bytarr(80)	,$
		pad7:			bytarr(120)	}

analyze_hdr.extents = 16384
analyze_hdr.sizeof_hdr = 348
;return,analyze_hdr

;---------------------------------------------------------------------------
;Modified analyze header structure
;incorporating magnet transform (display --> magnet aquisition coordinates)
;and landmark field (allows cross-referencing/reformating between series) 
;--------------------------------------------------------------------------

mtr = fltarr(4,4)
for i = 0,3 do mtr(i,i) = 1.0
analyze_hdr = { sizeof_hdr:		typlon		,$ ; header_key
		data_type:		bytarr(10)	,$
		db_name:		bytarr(18)	,$
		extents:		typlon		,$
		session_error:		typint		,$
		regular:		'r'		,$
		hkey_un0:		typbyte		,$
		dims:			typint		,$ ; image_dimension
		x_dim:			typint		,$
		y_dim:			typint		,$
		z_dim:			typint		,$
		t_dim:			typint		,$
		rest_of_dim:		intarr(3)	,$
		vox_units:		bytarr(4)	,$
		cal_units:		bytarr(8)	,$
		unused1:		typint		,$
		datatype:		typint		,$
		bits:			typint		,$
		dim_un0:		typint		,$
		pixdim0:		typflt		,$
		x_size:			typflt		,$
		y_size:			typflt		,$
		z_size:			typflt		,$
		rest_of_pixdim:		fltarr(4)	,$
		vox_offset:		typflt		,$
		scale:			typflt		,$
		funused2:		typflt		,$
		funused3:		typflt		,$
		cal_max:		typflt		,$
		cal_min:		typflt		,$
		compressed:		typlon		,$
		verified:		typlon		,$
		glmax:			typlon		,$
		glmin:			typlon		,$
		descrip:		bytarr(80)	,$ ; data history
		pad7:			bytarr(24) 	,$
		orient:			bytarr(1)	,$ ; orientation field (1-5)
		orig:			intarr(4)	,$
		magnet_transform:	mtr		,$ ;	(64 bytes)
		landmark:		typflt		,$  ;	(4 bytes)
		pad8:			bytarr(19)	}  ;	(87 - 68)	

analyze_hdr.extents = 16384
analyze_hdr.sizeof_hdr = 348
analyze_hdr.vox_units(0:1) = byte('mm')
return,analyze_hdr



end

;--------------------------------
;SPM2 new header fields
;may interfere with magnet transform
;--------------------------------

;descrip:		bytarr(80)	,$ ; data history
;aux_file:		bytarr(24) 	,$
;orient:		bytarr(1)	,$ ; orientation field (1-5, not used in SPM2)
;orig:			intarr(5)	,$
;generated:		bytarr(10)	,$
;scannum:		bytarr(10)	,$
;patient_id:		bytarr(10)	,$
;exp_date:		bytarr(10)	,$
;exp_time:		bytarr(10)	,$
;hist_un0:		bytarr(3)	,$
;views:			typlon		,$
;vols_added:		typlon		,$
;start_field:		typlon		,$
;field_skip:		typlon		,$
;omax:			typlon		,$
;omin:			typlon		,$
;smax:			typlon		,$
;smin:			typlon		

;---------------------------
;SPM2 extended header fields
;---------------------------
;mat:		double(mtr)	
;unused:	bytarr(384)
