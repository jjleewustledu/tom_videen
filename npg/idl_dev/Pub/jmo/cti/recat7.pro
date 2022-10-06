;       $Revision:   1.2  $ $Date:   10/29/93 17:13:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:		
;		RECAT7
;
; PURPOSE:			
;		Reads the given f,p,g,d,b (frame, planes, gates, data, beds)
;		data from the file in the file parameter. a structure containing
;		information about the file and data are returned in the paramter
;		ecat_data. 
;
; CATEGORY:
;		CTI Matrix utility			
;
; CALLING SEQUENCE:		
;		result = recat7 (file,f,p,g,d,b,ecat_data, 	$
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
;
;-

function   recat7,file,f,p,g,d,b,ecat_data, 			$
	         NO_SCALE=no_scale,NO_DATA=no_data,ZOOM=zoom,	$
		 MAIN=main,DIR=dir,DIRINFO=dirinfo,		$
		 ANAT_SORT=anat_sort			

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"RECAT7",FILE=file
	   return,!ECAT.ERROR
	endif
	ecat_data = 0

	openr,unit,file,/get_lun
	stat = rmh7(unit,mh)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(MAIN)) then begin
	   ecat_data = {mh:mh}
	   return,!ECAT.OK
	endif
	stat = rdir7(unit,directory,dinfo)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(DIR)) then dir = directory
	if(keyword_set(DIRINFO)) then dirinfo = dirinfo
; Anat sort does not work for version 7.
;	if(keyword_set(ANAT_SORT)) then begin
;	   if(anat_sort(directory,mh) EQ !ECAT.ERROR) then return,!ECAT.ERROR
;	endif
	dvals = diffval(directory(0,*))
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	expand_mat,f,p,g,d,b,dvals
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	f=fix(f) & p=fix(p) & g=fix(g) & d=fix(d) & b=fix(b)
	mats = mkmats(f,p,g,d,b,directory)
	if((mats(0) EQ !ECAT.ERROR) ) then begin
	   set_ecat_error,3,"RECAT",UNIT=unit
	   return,!ECAT.ERROR
	endif
	n=n_elements(mats(0,*))
	name = file
	ftype = mh.file_type
	if(rsubheaders7(ftype,unit,mats(1,*),sh,scales) EQ !ECAT.ERROR) then 	$
	   return,!ECAT.ERROR
	data = 0

	; get the size of the data
	case !ECAT.file_types(ftype) of

	   "IMG" : $
	      begin
		xd = sh(0).x_dimension
		yd = sh(0).y_dimension
		zd = sh(0).z_dimension
		dtype = sh(0).data_type
	      end

	   "SCN" : $
	      begin
		xd = sh(0).num_r_elements
		yd = sh(0).num_angles
		zd = sh(0).num_z_elements
		dtype = sh(0).data_type
	      end

	   "NRM" : $
	      begin
		xd = sh(0).num_r_elements
		yd = sh(0).num_angles
		zd = sh(0).num_z_elements
		dtype = sh(0).data_type
	      end

	   "ATN" : $
	      begin
		xd = sh(0).num_r_elements
		yd = sh(0).num_angles
		zd = sh(0).num_z_elements
		dtype = sh(0).data_type
	      end

	   "PM" : $
	      begin
		xd = 0
		yd = 0
		dtype = sh(0).data_type
	      end

	   "V" : $
	      begin
		xd = sh(0).x_dimension
		yd = sh(0).y_dimension
		zd = sh(0).z_dimension
		offsets = lonarr(zd)
	        sf = scales(0)
		scales = fltarr(zd)
		scales(*) = sf
		offsets(0) = mats(1,0) + !ECAT.blksize ; Account for subheader.
		for i=1,zd-1 do $
		    offsets(i) = offsets(i-1) + 2*long(xd)*long(yd)
		dtype = sh(0).data_type
	      end

	   "S" : $
	      begin
		xd = sh(0).num_r_elements
		yd = sh(0).num_angles
		zd = sh(0).num_z_elements
		offsets = lonarr(zd)
	        sf = scales(0)
		scales = fltarr(zd)
		scales(*) = sf
		offsets(0) = mats(1,0) + !ECAT.blksize ; Account for subheader.
		for i=1,zd-1 do $
		    offsets(i) = offsets(i-1) + 2*long(xd)*long(yd)
		dtype = sh(0).data_type
	      end

	   else  : $
	      begin
		xd=0
		yd=0
		dtype=6
	      end
	endcase

	zf = 1
	if(NOT keyword_set(NO_DATA)) then begin

           ; handle special case of reading polar map data
           if (ftype eq 5) then begin
              stat = rpmdata7(unit, sh, mats(1,*), data)
           endif else begin

	     if(keyword_set(NO_SCALE)) then 			$
	       stat = rdata(dtype,unit,offsets,xd,yd,zd,data)  $
	     else 						$ 
	       stat = rdata(dtype,unit,offsets,xd,yd,zd,data,SCALE=scales)
	     if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	     if(keyword_set(ZOOM)) then begin
	       z = [zoom*xd, zoom*yd]
	       if(zoom LT 1) then 			$
	         m = [xd mod z(0), yd mod z(1)]		$
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

io_error : set_ecat_error,0,"RECAT7",UNIT=unit
           return,!ECAT.ERROR
end
