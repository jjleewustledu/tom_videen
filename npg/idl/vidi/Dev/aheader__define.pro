PRO header__define

   MH =  {MainHeader,	$
          file:			'',	$ ; File name
          file_format:		0,	$ ; File format, MATRIX_REV6, MATRIX_REV7 eg.
          number_format:	0,	$ ; Number type: e.g., 1=byte, 2=short, 3=float
          sw_version:		0,	$ ; Software version number
          filetype:		0,	$ ; DataSetType (ECAT) or rec version (PETT VI)
          nplane:		0,	$ ; Number of slices per frame
          nframe:		0,	$ ; Number of frames collected
          ngate:		0,	$ ; Number of gates collected
          nbed:			0,	$ ; Number of bed position
          scanner_type:		0,	$ ; 953, 961, 6
          date_time:		'',	$ ; Creation time of original data
          start_time:		0,	$
          wobble_speed:		0,	$
          patnum:		'',	$
          bed_elevation:	0.0,	$
          init_bed_pos:		0.0,	$
          bed_off:		fltarr (16),	$
          slicesep:		0.0,	$
          calibration_factor:	0.0,	$
          isotope:		'',	$
          radiopharmaceutical:	'',	$
          orientation:		0,	$ ; 0=neurological (right-on-right), 1=radiological
          scale_flag:		0,	$ ; post-recon scaling: 0=none, 1=scaled by pr_scaler
          pr_scaler:		0.0,	$ ; post-recon scaling factor; 0 if none
          pr_filter:		0,	$ ; see pettable.h for codes
          pett6_flag:		0,	$ ; 1 = ECAT file created from a PETT VI image
          zorigin:		0,	$ ; Slice number corresponding atlas z-origin
          sw_revision:		'',	$ ; Software version which created this file
          filetime:		'',	$ ; Creation time of this file
          gen_data:		'',	$ ; Whatever
          scanner:		'',	$ ; Scanner name for archive path
          acquisition:		'',	$ ; Acquisition name for archive path
          reconstruction:	'',	$ ; Reconstruction name for archive path
          filtername:		''	$ ; Filter name for archive path
         }

   SH =  {SubHeader,	$
          number_format:	0,	$ ; Data type
          file_format:		0,	$
          ndim:			0,	$
          dim1:			0,	$ ; X dimension (pixels)
          dim2:			0,	$ ; Y dimension (pixels)
          dim3:			0,	$ ; Z dimension (slices)
          ring_diff:		0,	$
          total:		0,	$
          delayeds:		0,	$
          multiples:		0,	$
          trues:		0,	$
          count_rate:		0,	$
          scale_factor:		0.0,	$
          calibration_factor:	0.0,	$
          min:			0,	$
          max:			0,	$
          deadtime_corr:	0.0,	$
          decay_corr:		0.0,	$
          sample_size_1:	0.0,	$ ; cm
          sample_size_2:	0.0,	$ ; cm
          sample_size_3:	0.0,	$ ; cm
          start_time:		0,	$
          duration:		0,	$ ; msec
          filter_code:		0,	$
          filter_cutoff_frequency: 0.0,	$
          filter_resolution:	0.0,	$
          filter_ramp_slope:	0.0,	$
          filter_order:		0.0,	$
          filter_scatter_fraction: 0.0,	$
          filter_scatter_slope:	0.0,	$
          comment:		''	$
         }

   HEAD = {Header,	$
           fp:		0L,		$ ; file pointer (for Interfile and PETT VI)
           mode:	strarr (3),	$ ; file access mode 'r','w','a'
           mh:		{MainHeader},	$ ; generic mainheader
           sh:		{SubHeader},	$ ; generic subheader
           m6:		{M6header},	$ ; Matrix 6 header
           m7:		{M7header}	$ ; Matrix 7 header
;;;      p6:		{PETT6HDR},	$ ; PETT VI header
;;;      ifh:		{IFheader},	$ ; Interfile header
   }

END 
