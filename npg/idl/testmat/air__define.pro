PRO AIR__DEFINE
	;; AIR file structure (Roger Woods, 2002)

	tmp1 = {keyinfo,	$
    	bits:	0L,		$ ;
    	x_dim:	0L,		$ ;
    	y_dim:	0L,		$ ;
    	z_dim:	0L,		$ ;
    	x_size:	0.0D,	$ ;
    	y_size:	0.0D,	$ ;
    	z_size:	0.0D	$ ;
	}

	tmp2 = {air,		$
		e:			dblarr(4,4),	$,
    	std_file:	bytarr(128),	$;
    	std:		{keyinfo},		$;
    	res_file:	bytarr(128),	$;
    	res:		{keyinfo},		$;
    	comment:	bytarr(128),	$;
    	std_hash:	0L,				$;
    	res_hash: 	0L,				$;
    	std_volume:	0,				$;
    	res_volume:	0,				$;
    	reserved:	bytarr(116)		$;
	}
END
