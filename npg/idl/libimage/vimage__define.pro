PRO vimage__define

   idx = {Image_Index,$
          frm:	0,	$
          pln:	0,	$
          gate:	0,	$
          data:	0,	$
          bed:	0	$
         }

   tmp = {VIMAGE,	$
          fname:	'',		$ ; file name
          system:	0,		$ ; system type (953, 961, 220)
          h:		{Header},	$ ; image file headers
          ntype:	0,		$ ; number format
          itype:	0,		$ ; image type
          order:	1,		$ ; image array order
          dim:		lonarr(4),	$ ; image array dimensions
          pdim:		fltarr(4),	$ ; image voxel dimensions (mm)
          org:		fltarr(4),	$ ; image array origin
          fp:	 	0L,		$ ; first index to dim[2]
          lp:	 	0L,		$ ; last index to dim[2]
          p:		1L,		$ ; current index to dim[2]
          ff:	 	0L,		$ ; first index to dim[3]
          lf:	 	0L,		$ ; last index to dim[3]
          f: 		1L,		$ ; current index to dim[3]
          scale:	1.0,		$ ; scaling factor
          gcal:		1.0,		$ ; global calibration factor
          ical:		1.0,		$ ; individual (slice/frame) calibration factor
          proc:		0,		$ ; type of processing
                                	$ ; (0=none, 1=CBF, 2=CBV, 3=OEF, 4=CMRO2)
          mtype:	0,		$ ; hdr type (for metabolic processing)
          mcal:		fltarr(10),	$ ; metabolic calibration factors
					$ ; 0=PETT Conversion Factor (PETTCONV)
					$ ; 1=Flow Coefficient       (AFLOW)
					$ ; 2=Flow Coefficient       (BFLOW)
					$ ; 3=Volume Factor          (BVFACTOR)
					$ ; 4=Oxygen Coefficient     (AOXYGEN)
					$ ; 5=Oxygen Coefficient     (BOXYGEN)
					$ ; 6=Oxygen Coefficient     (AWATER)
					$ ; 7=Oxygen Coefficient     (BWATER)
					$ ; 8=Oxygen Content         (OXYCONT)
					$ ; 9=Oxygen Counts          (O2CNTS)
          start:	lonarr(100),	$ ; frame start time (ms)
          len:		lonarr(100),	$ ; frame duration (ms) (LE 0 -> no data)
          min: 		0.0,		$ ; data min value
          max: 		0.0,		$ ; data max value
          data:		ptr_new()	$ ; pointer to first data element
         }

END
