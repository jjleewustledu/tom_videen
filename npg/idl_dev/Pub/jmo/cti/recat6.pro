;       $Revision:   1.2  $ $Date:   10/26/93 16:32:52  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:	
;		RECAT6	
;
; PURPOSE:	
;		Reads the given frame, plane, gate,data, and bed data from
;		the file in the file parameter. A structure containing 
;		information about the file and data are returned in the parameter 
;		ecat_data.
;
; CATEGORY:	
;		CTI Matrix utility		
;
; CALLING SEQUENCE:	
;		result = recat6 (file,f,p,g,d,b,ecat_data, 	$
;	         NO_SCALE=no_scale,NO_DATA=no_data,ZOOM=zoom,	$
;		 MAIN=main,DIR=dir,DIRINFO=dirinfo,		$
;		 ANAT_SORT=anat_sort)		
;
;
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RECAT6
;	RECAT READS THE GIVEN F,P,G,D,B (FRAME, PLANES, GATES, DATA, BEDS)
;	DATA FROM THE FILE IN THE FILE PARAMETER. A STRUCTURE CONTAINING
;	INFORMATION ABOUT THE FILE AND DATA ARE RETURNED IN THE PARAMTER
;	ECAT_DATA. THE KEYWORD NO_SCALE PREVENTS THE SCALING OF THE DATA.
;	THE KEYWORD NO_DATA RETURNS NO DATA, BUT JUST HEADER INFO. RECAT
;	RETURNS !ECAT.OK ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;

function   recat6,file,f,p,g,d,b,ecat_data, 			$
	         NO_SCALE=no_scale,NO_DATA=no_data,ZOOM=zoom,	$
		 MAIN=main,DIR=dir,DIRINFO=dirinfo,		$
		 ANAT_SORT=anat_sort			

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"RECAT6",FILE=file
	   return,!ECAT.ERROR
	endif
	ecat_data = 0

	openr,unit,file,/get_lun
	stat = rmh6(unit,mh)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(MAIN)) then begin
	   ecat_data = {mh:mh}
	   return,ecat_data
	endif
	stat = rdir6(unit,directory,dinfo)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(DIR)) then dir = directory
	if(keyword_set(DIRINFO)) then dirinfo = dirinfo
	if(keyword_set(ANAT_SORT)) then begin
	   if(anat_sort(directory,mh) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	endif
	dvals = diffval(directory(0,*))
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	expand_mat,f,p,g,d,b,dvals
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	f=fix(f) & p=fix(p) & g=fix(g) & d=fix(d) & b=fix(b)
	mats = mkmats(f,p,g,d,b,directory)
	if((mats(0) EQ !ECAT.ERROR) ) then begin
; JMO	   set_ecat_error,3,"RECAT6",UNIT=unit
	   return,!ECAT.ERROR
	endif
	n=n_elements(mats(0,*))
	name = file
	ftype = mh.file.file_type
	dtype = mh.file.data_type
	if(rsubheaders6(ftype,unit,mats(1,*),sh,scales) EQ !ECAT.ERROR) then $
	   return,!ECAT.ERROR
	data = 0
	xd = 0
	yd = 0
	zd = n
	zf = 1

	; set the data size values for types other than the polar map
	if (ftype lt 5) then begin
	  xd = sh(0).dimension_1
	  yd = sh(0).dimension_2
	endif

	if(NOT keyword_set(NO_DATA)) then begin

	   ; handle special case of reading polar map data
	   if (ftype eq 5) then begin
	      stat = rpmdata6(unit, sh, mats(1,*), data)
	   endif else begin

	     if(keyword_set(NO_SCALE)) then begin
	        stat = rdata(dtype,unit,mats(1,*),xd,yd,zd,data) 
	     endif else begin
	        stat = rdata(dtype,unit,mats(1,*),xd,yd,zd,data,SCALE=scales)
	     endelse
	     if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	     if(keyword_set(ZOOM)) then begin
	        z = [zoom*xd, zoom*yd]
	        if(zoom LT 1) then 			$
	           m = [xd mod z(0), yd mod z(1)]	$
	        else					$
	           m = [zoom*xd mod xd, zoom*yd mod yd]
	        if(total(m) GT 0) then begin
	           z = [xd,yd]
	           zoom = 1
	        endif
	        d = make_array(z(0),z(1),zd,SIZE=size(data(*,*,0)))
	        xd = z(0) & yd = z(1)
	        for i=0,zd-1 do d(*,*,i) = rebin(data(*,*,i),z(0),z(1))
	        data = temporary(d)
	        zf = zoom
	     endif
	   endelse
	endif
	ecat_data = {fname:name, 		$
		     ftype:ftype,		$
		     dtype:dtype,		$
		     nmats:n,			$
		     mats:mats,			$
		     xdim:xd,			$
		     ydim:yd,			$
		     zdim:n_elements(sh),	$
	             zoom:zf,			$
		     mh:mh,			$
		     sh:sh,			$
		     data:data               	$
		    }

	free_lun,unit
	return,!ECAT.OK

io_error : set_ecat_error,0,"RECAT6",UNIT=unit
           return,!ECAT.ERROR
end
