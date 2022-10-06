PRO analyzehdr__define

   hkey = {HEADER_KEY,		$
           sizeof_hdr   : 0L,	$ ; required--byte size of header file
           data_type    : bytarr(10),$
           db_name      : bytarr(18),$
           extents      : 0L,	$ ; required--16384
           session_error: 0,	$
           regular      : 0B,	$ ; required--'r'=regular
           hkey_un0     : 0B 	$
          }

   dime = {IMAGE_DIMENSION,	$
           dim        : intarr(8),$
           unused8    : 0,	$
           unused9    : 0,	$
           unused10   : 0,	$
           unused11   : 0,	$
           unused12   : 0,	$
           unused13   : 0,	$
           unused14   : 0,	$
           datatype   : 0,	$ ; 1=1bit/pix,2=8b,4=16b(short),8=32b(signed),16=float
           bitpix     : 0,	$ ; bits/pixel
           dim_un0    : 0,	$
           pixdim     : fltarr(8),$ ; real world values of dimensions mm ms
           funused8   : 0.0,	$
           funused9   : 0.0,	$
           funused10  : 0.0,	$
           funused11  : 0.0,	$
           funused12  : 0.0,	$
           funused13  : 0.0,	$
           compressed : 0.0,	$
           verified   : 0.0,	$
           glmax      : 0L,	$
           glmin      : 0L	$ ; required
          }

   hist = {DATA_HISTORY,		$
           descrip    : bytarr(80),$
           aux_file   : bytarr(24),$
           orient     : 0B,	$
           originator : bytarr(10),$
           generated  : bytarr(10),$
           scannum    : bytarr(10),$
           patient_id : bytarr(10),$
           exp_date   : bytarr(10),$
           exp_time   : bytarr(10),$
           hist_un0   : bytarr(3), $
           views      : 0L,	$
           vols_added : 0L,	$
           start_field: 0L,	$		
           field_skip : 0L,	$
           omax       : 0L,	$
           omin       : 0L,	$
           smax       : 0L,	$
           smin       : 0L	$
          }

   tmp = {ANALYZEHDR, hk:hkey, dime:dime, hist:hist}

END
