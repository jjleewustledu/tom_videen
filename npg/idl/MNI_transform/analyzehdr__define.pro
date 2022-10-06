PRO ANALYZEHDR__DEFINE
	;; Analyze header file structure (dbh.h 1995)

	tmp1 = {hdrkey,		$
    	sizeof_hdr:	0L,			$ ;
    	data_type:	bytarr(10),	$;
    	db_name:	bytarr(18),	$;
    	extents:	0L,			$ ;
    	session_error:	0,		$ ;
    	regular:	bytarr(1),	$ ;
    	hkey_un0:	bytarr(1)	$ ;
	}

	tmp2 = {hdrdime,		$
    	dim:		intarr(8),		$;
    	vox_units:	bytarr(4),		$;
    	cal_units:	bytarr(8),		$;
    	unused1:	0,				$;
    	datatype: 	0,				$;
    	bitpix:		0,				$;
    	dim_un0:	0,				$;
    	pixdim:		fltarr(8),		$;
    	vox_offset:	0.0,			$;
    	roi_scale:	0.0,			$;
    	funused1:	0.0,			$;
    	funused2:	0.0,			$;
    	cal_max:	0.0,			$;
    	cal_min:	0.0,			$;
    	compressed:	0L,				$;
    	verified:	0L,				$;
    	glmax:		0L,				$;
    	glmin:		0L				$;
	}

	tmp3 = {hdrhist,		$
        descrip:    bytarr(80),     $;
    	aux_file:	bytarr(24),		$;
    	orient:		bytarr(1),		$;
    	originator:	bytarr(10),		$;
    	generator:	bytarr(10),		$;
    	scannum:	bytarr(10),		$;
    	patient_id:	bytarr(10),		$;
    	exp_date:	bytarr(10),		$;
    	exp_time:	bytarr(10),		$;
    	hist_un0:	bytarr(3),		$;
        views:      0L,             $;
        vols_added: 0L,             $;
        start_field: 0L,            $;
        field_skip: 0L,             $;
        omax:       0L,             $;
        omin:       0L,             $;
        smax:       0L,             $;
    	smin: 		0L				$;
	}

	tmp4 = {analyzehdr,		$
		hk:		{hdrkey},	$
		dime:	{hdrdime},	$
		hist:	{hdrhist}	$
	}
END
