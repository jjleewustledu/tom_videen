pro test_wecat

	!quiet = 1
	FORMR   = '("TOTAL READ  TIME = ",F7.3,2X,"TIME/PLANE = ",F7.3)'
	FORMW   = '("TOTAL WRITE TIME = ",F7.3,2X,"TIME/PLANE = ",F7.3)'

	print
	print
	print,"     THIS TEST CREATES A 128 X 128 X 32 ARRAY, EACH PLANE"
	print,"     BEING SET TO DIST(128), AND WRITES THIS DATA AS AN"
	print,"     ECAT FILE WITH 2 FRAMES, PLANES, GATES, DATA, AND BEDS."
	print,"     WHEN THE FILE IS MODIFIED, THE FIRST AND LAST MATRIX IS"
	print,"     REPLACED WITH LOG OF THE MODULUS OF THE FFT OF THE"
	print,"     DIST(128) ARRAY, WITH THE APPROPRIATE SHIFTING. THE TIMES"
	print,"     REPORTED ARE APPROXIMATE AND ARE MEASURED IN SECONDS."
	print
	print
	wait,10

	mh    = !ECAT.mh
	imgsh = !ECAT.sh_img
	scnsh = !ECAT.sh_scn
	atnsh = !ECAT.sh_atn
	nrmsh = !ECAT.sh_nrm
	data  = fltarr(128,128,32)
	f     = lindgen(2)+1
	p     = lindgen(2)+1
	g     = lindgen(2)+1
	d     = lindgen(2)
	b     = lindgen(2)

;	fill in main header

;	MH_File_Info
;---------------------------------

	mh.file.original_file_name = "WECAT.TEST" ; NON ESSENTIAL

;	THE FOLLOWING ARE ESSENTIAL HEADER DATA

	mh.file.nframes = n_elements(f)
	mh.file.nplanes = n_elements(p)
	mh.file.ngates  = n_elements(g)
	mh.file.nbeds   = n_elements(b)-1

; 	DATA TYPE AND FILE TYPE ARE ESSENTIAL - BUT WILL BE FILLED
;	IN BELOW. ISOTOPE_CODE AND ISOTOPE_HALFLIFE ARE ESSENTIAL
;	IF THE FILE WILL POSSIBLY BE USED IN AN APPLICATION WHERE 
;	THIS DATA IS NEEDED. PATIENT INFO SHOULD BE FILLED IN AT 
;	THE TIME OF THE SCAN, BUT USUALLY IS ONLY USED FOR
;	INFORMATIONAL PURPOSES.

;       MH_Scan_Info
;---------------------------------

	mh.scan.calibration_factor = 1.0
	mh.scan.calibration_units  = 2
	mh.scan.init_bed_position  = 0.0
	mh.scan.bed_offset(0)      = 10.0
	mh.scan.plane_separation   = .3375
	
;	ALL OTHER DATA IN MH.SCAN SHOULD BE FILLED IN ALSO. FOR BREVITY,
;	ONLY THE REALLY IMPORTANT DATA IS FILLED IN.

;	Img_Subheader
;----------------------------------

; 	EVERYTHING HERE IS ESSENTIAL FOR BASIC PROCESSING.
;	OTHER ITEMS MAY BE NEEDED FOR A PARTICULAR APPLICATION.

	imgsh.data_type      	= 6
	imgsh.num_dimensions 	= 2
	imgsh.dimension_1    	= 128
	imgsh.dimension_2	= 128
	imgsh.pixel_size	=.25
	imgsh.slice_width	= .3375
	imgsh.frame_duration	= 600000L ; not really essential for artificial
	imgsh.frame_start_time  = 0L      ; data.
	imgsh.quant_units	= 2
	imgsh.ecat_calibration_fctr = 1.0

;	THE QUANT_SCALE AND IMAGE MAX AND MIN IS COMPUTED BY WECAT.
	

;	THE Img_Recon_Info SECTION CONTAINS ONLY INFORMATIONAL DATA

;	Scn_Subheader
;----------------------------------

;	THESE ARE THE MOST NEEDED DATA.

	scnsh.data_type 	= 6
	scnsh.dimension_1	= 128
	scnsh.dimension_2	= 128
	scnsh.loss_correction_fctr = 1.0

;	EVERYTHING ELSE SHOULD BE FILLED IN BUT IS NON ESSENTIAL
;	FOR ARTIFICIAL DATA.


;	Atn_Subheader
;------------------------------------

	atnsh.data_type		= 5
	atnsh.dimension_1	= 128
	atnsh.dimension_2	= 128
	atnsh.scale_factor	= 1.0

;	Nrm_Subheader
;-------------------------------------

	nrmsh.data_type		= 5
	nrmsh.dimension_1 	= 128
	nrmsh.dimension_2	= 128
	nrmsh.scale_factor	= 1.0




;******************************************************************************

	loadct,3
	window,xsize=128*6,ysize=128*6,title="WECAT TEST"


;	WRITE AN IMAGE FILE


	print
	print,"WECAT IMAGE FILE TEST ..."
	print,"MAKING DATA ..."
	data = fltarr(128,128,32)
	for i=0,31 do data(*,*,i) = dist(128)
	newd = data(*,*,0)
	newd = fft(newd,1)
	newd = shift(newd,64,64)
	newd = abs(alog(abs(newd) > 0.0))
	mh.file.data_type = 6
	mh.file.file_type = 2
	file = "wecat.img"
	sh = replicate(imgsh,32)
	print,"WRITING WECAT.IMG ..."
	before = systime(1)
	stat = wecat(file,f,p,g,d,b,sh,data,/NEW,MH=mh)
	after = systime(1)
	if(stat EQ !ECAT.OK) then print,"WECAT.IMG CREATED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,format=formw,after-before,(after-before)/32.0
	print,"READING WECAT.IMG ..."
	before = systime(1)
	stat = recat(file,"*","*","*","*","*",ecat)
	after = systime(1)
	print,format=formr,after-before,(after-before)/32.0
	print,"DISPLAYING ..."	
	for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5	
	print,"CHANGING MATRIX 1,1,1,0,0 (first) AND 2,2,2,1,1 (last) ..."
	stat = wecat(file,1,1,1,0,0,sh(0),newd)
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	stat = wecat(file,2,2,2,1,1,sh(31),newd)
	if(stat EQ !ECAT.OK) then print,"WECAT.IMG MODIFIED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,"READING WECAT.IMG ..."
	print,"DISPLAYING ..."	
	stat = recat(file,"*","*","*","*","*",ecat)
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5

;	WRITE A SCAN FILE

	print
	print,"WECAT SCAN FILE TEST ..."
	print,"MAKING DATA ..."
	data = fltarr(128,128,32)
	for i=0,31 do data(*,*,i) = dist(128)
	newd = data(*,*,0)
	newd = fft(newd,1)
	newd = shift(newd,64,64)
	newd = abs(alog(abs(newd) > 0.0))
	mh.file.data_type = 6
	mh.file.file_type = 1
	file = "wecat.scn"
	sh = replicate(scnsh,32)
	print,"WRITING WECAT.SCN ..."
	before = systime(1)
	stat = wecat(file,f,p,g,d,b,sh,data,/NEW,MH=mh)
	after = systime(1)
	if(stat EQ !ECAT.OK) then print,"WECAT.SCN CREATED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,format=formw,after-before,(after-before)/32.0	
	print,"READING WECAT.SCN ..."
	before = systime(1)
	stat = recat(file,"*","*","*","*","*",ecat)
	after = systime(1)
	print,format=formr,after-before,(after-before)/32.0
	print,"DISPLAYING ..."		
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5	
	print,"CHANGING MATRIX 1,1,1,0,0 (first) AND 2,2,2,1,1 (last) ..."
	stat = wecat(file,1,1,1,0,0,sh(0),newd)
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	stat = wecat(file,2,2,2,1,1,sh(31),newd)
	if(stat EQ !ECAT.OK) then print,"WECAT.SCN MODIFIED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,"READING WECAT.SCN ..."
	stat = recat(file,"*","*","*","*","*",ecat)
	print,"DISPLAYING ..."	
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5

;	WRITE AN ATTENUATION FILE

	print
	print,"WECAT ATTENUATION FILE TEST ..."
	print,"MAKING DATA ..."
	data = fltarr(128,128,32)
	for i=0,31 do data(*,*,i) = dist(128)
	newd = data(*,*,0)
	newd = fft(newd,1)
	newd = shift(newd,64,64)
	newd = abs(alog(abs(newd) > 0.0))
	mh.file.data_type = 5
	mh.file.file_type = 3
	file = "wecat.atn"
	sh = replicate(atnsh,32)
	print,"WRITING WECAT.ATN ..."
	before = systime(1)
	stat = wecat(file,f,p,g,d,b,sh,data,/NEW,MH=mh)
	after = systime(1)
	if(stat EQ !ECAT.OK) then print,"WECAT.ATN CREATED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,format=formw,after-before,(after-before)/32.0	
	print,"READING WECAT.ATN ..."
	before = systime(1)
	stat = recat(file,"*","*","*","*","*",ecat)
	after = systime(1)
	print,format=formr,after-before,(after-before)/32.0
	print,"DISPLAYING ..."
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5	
	print,"CHANGING MATRIX 1,1,1,0,0 (first) AND 2,2,2,1,1 (last) ..."
	stat = wecat(file,1,1,1,0,0,sh(0),newd)
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end	
	stat = wecat(file,2,2,2,1,1,sh(31),newd)
	if(stat EQ !ECAT.OK) then print,"WECAT.ATN MODIFIED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,"READING WECAT.ATN ..."
	stat = recat(file,"*","*","*","*","*",ecat)
	print,"DISPLAYING ..."
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5

;	WRITE A NORMALIZATION FILE

	print
	print,"WECAT NORMALIZATION FILE TEST ..."
	print,"MAKING DATA ..."
	data = fltarr(128,128,32)
	for i=0,31 do data(*,*,i) = dist(128)
	newd = data(*,*,0)
	newd = fft(newd,1)
	newd = shift(newd,64,64)
	newd = abs(alog(abs(newd) > 0.0))
	mh.file.data_type = 5
	mh.file.file_type = 4
	file = "wecat.nrm"
	sh = replicate(nrmsh,32)
	print,"WRITING WECAT.NRM ..."
	before = systime(1)
	stat = wecat(file,f,p,g,d,b,sh,data,/NEW,MH=mh)
	after = systime(1)
	if(stat EQ !ECAT.OK) then print,"WECAT.NRM CREATED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,format=formw,after-before,(after-before)/32.0	
	print,"READING WECAT.NRM ..."
	before = systime(1)
	stat = recat(file,"*","*","*","*","*",ecat)
	after = systime(1)
	print,format=formr,after-before,(after-before)/32.0
	print,"DISPLAYING ..."	
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5	
	print,"CHANGING MATRIX 1,1,1,0,0 (first) AND 2,2,2,1,1 (last) ..."
	stat = wecat(file,1,1,1,0,0,sh(0),newd)
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	stat = wecat(file,2,2,2,1,1,sh(31),newd)
	if(stat EQ !ECAT.OK) then print,"WECAT.NRM MODIFIED ..."
	if(stat EQ !ECAT.ERROR) then begin
	   print,"*** WECAT ERROR ***"
	   wdelete & return
	end
	print,"READING WECAT.NRM ..."
	stat = recat(file,"*","*","*","*","*",ecat)
	print,"DISPLAYING ..."
	erase & for i=0,ecat.nmats-1 do tvscl,ecat.data(*,*,i),i,/order
	wait,5

;	TEST COMPLETE
	
	print
	print,"TEST COMPLETE ..."
	!quiet = 0
	wdelete
	return
end




	
	
	

	


	

	
	
