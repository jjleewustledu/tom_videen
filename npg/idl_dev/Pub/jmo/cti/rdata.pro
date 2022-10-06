;       $Revision:   1.1  $ $Date:   10/15/93 18:23:26  $
;       Copyright(c) 1993 CTI PET Systems, Inc.

;+
; NAME:	
;		RDATA	
;
; PURPOSE:
;		Reads into the parameter data the matrix data at the positions
;		given by offsets in the open file represented by unit.			
;
; CATEGORY:	
;		CTI Matrix utility.		
;
; CALLING SEQUENCE:		
;		result = rdata (dtype,unit,offsets,xd,yd,zd,data,SCALE=scale)
; INPUTS:			
;
; KEYWORD PARAMETERS:		
;
; OUTPUTS:			
;
; HISTORY NOTES:		
;-


function   rdata,dtype,unit,offsets,xd,yd,zd,data,SCALE=scale

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"RDATA",UNIT=unit
	   return,!ECAT.ERROR
	endif

	sf = keyword_set(SCALE)
	if(NOT sf) then $
	   scale = replicate(1, zd)

	case !ECAT.data_types(dtype) of
	   "VAX_I2" :  $
	   begin
	      if(sf) then 		 $
	         data = fltarr(xd,yd,zd) $
	      else $
	         data = intarr(xd,yd,zd)
	      d = intarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         IF (NOT !LITTLE_ENDIAN) THEN byteorder,d
	         data(*,*,i) = d*scale(i)
	      endfor
	   end

	   "VAX_R4" : $
	   begin
	      data = fltarr(xd,yd,zd)
	      d = lonarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         IF (NOT !LITTLE_ENDIAN) THEN byteorder,d,/lswap
	         data(*,*,i) = vtosf(d)*scale(i)
	      endfor
	   end

	   "SUN_I2" : $
	   begin
	      if(sf) then 		 $
	         data = fltarr(xd,yd,zd) $
	      else $
	         data = intarr(xd,yd,zd)
	      d = intarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         IF (!LITTLE_ENDIAN) THEN byteorder,d
	         if (scale(i) eq 0.) then scale(i) = 1.
	         data(*,*,i) = d*scale(i) 
	      endfor
	   end

	   "SUN_R4" : $
	   begin
	      data = fltarr(xd,yd,zd)
	      d = fltarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         data(*,*,i) = d*scale(i)
	         IF (!LITTLE_ENDIAN) THEN byteorder,d,lswap
	      endfor
	   end

	   else : begin
	      set_ecat_error,4,"RDATA",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	d = 0
	return,!ECAT.OK

io_error : set_ecat_error,1,"RDATA",UNIT=unit
;           return,!ECAT.ERROR
; Modification by JMO 3/17/95
return,!ECAT.OK

end
