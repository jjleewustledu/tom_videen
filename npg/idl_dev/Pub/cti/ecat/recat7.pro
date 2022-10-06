;       $Revision:   1.2  $ $Date:   10/29/93 17:13:32  $
;       Copyright(c) 1993 CTI PET Systems, Inc.
;+
; NAME:		
; 	RECAT7
;
; PURPOSE:			
; 	Reads the given f,p,g,d,b (frame, planes, gates, data, beds)
; 	data from the file in the file parameter. a structure containing
; 	information about the file and data are returned in the paramter
; 	ecat_data. 
;
; CATEGORY:
; 	CTI Matrix utility			
;
; CALLING SEQUENCE:		
;	result = recat7 (file, f, p, g, d, b, ecat_data, 	$
;		NO_SCALE=no_scale, NO_DATA=no_data, ZOOM=zoom,	$
;		MAIN=main,DIR=dir, DIRINFO=dirinfo, ANAT_SORT=anat_sort)			
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

FUNCTION  recat7, file, f, p, g, d, b, ecat_data,  $
                  NO_SCALE=no_scale, NO_DATA=no_data, ZOOM=zoom, $
                  MAIN=main, DIR=dir, DIRINFO=dirinfo, ANAT_SORT=anat_sort  

   on_error, 2
   on_ioerror, io_error
   IF (n_params() NE 7) THEN BEGIN
      set_ecat_error, 2, "RECAT7", FILE=file
      RETURN, !ECAT.ERROR
   ENDIF
   ecat_data = 0
   openr, unit, file, /get_lun
   stat = rmh7(unit, mh)
   IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   IF (keyword_set(MAIN)) THEN BEGIN
      ecat_data = {mh:mh}
      RETURN, !ECAT.OK
   ENDIF
   stat = rdir7(unit, directory, dinfo)
print,stat
   IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   IF (keyword_set(DIR)) THEN dir = directory
   IF (keyword_set(DIRINFO)) THEN dirinfo = dirinfo
   IF (keyword_set(ANAT_SORT)) THEN BEGIN
      IF (anat_sort(directory, mh) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   ENDIF
   dvals = diffval(directory(0, *))
   IF (dvals(0) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   expand_mat, f, p, g, d, b, dvals
   IF (dvals(0) EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
   f = fix(f) & p = fix(p) & g = fix(g) & d = fix(d) & b = fix(b)
   mats = mkmats(f, p, g, d, b, directory)
   IF ((mats(0) EQ !ECAT.ERROR) ) THEN BEGIN
      set_ecat_error, 3, "RECAT", UNIT=unit
      RETURN, !ECAT.ERROR
   ENDIF
   n=n_elements(mats(0, *))
   name = file
   ftype = mh.file_type
   IF (rsubheaders7(ftype, unit, mats(1, *), sh, scales) EQ !ECAT.ERROR) THEN $
      RETURN, !ECAT.ERROR
   data = 0
print,ftype, !ECAT.file_types(ftype)
   
   CASE !ECAT.file_types(ftype) OF ; get the size of the data
      
      "IMG" : $
         BEGIN
         xd = sh(0).x_dimension
         yd = sh(0).y_dimension
         dtype = sh(0).data_type
      END

      "SCN" : $
         BEGIN
         xd = sh(0).num_r_elements
         yd = sh(0).num_angles
         dtype = sh(0).data_type
      END

      "NRM" : $
         BEGIN
         xd = sh(0).num_r_elements
         yd = sh(0).num_angles
         dtype = sh(0).data_type
      END

      "ATN" : $
         BEGIN
         xd = sh(0).num_r_elements
         yd = sh(0).num_angles
         dtype = sh(0).data_type
      END

      "PM" : $
         BEGIN
         xd = 0
         yd = 0
         dtype = sh(0).data_type
      END

      ELSE : $
         BEGIN
         xd = 0
         yd = 0
         dtype = 6
      END
   ENDCASE

   zd = n
   zf = 1
   IF (NOT keyword_set(NO_DATA)) THEN BEGIN
      IF (ftype EQ 5) THEN BEGIN ; handle special CASE of reading polar map data
         stat = rpmdata7(unit, sh, mats(1, *), data)
      ENDIF ELSE BEGIN
         IF (keyword_set(NO_SCALE)) THEN  $
            stat = rdata(dtype, unit, mats(1, *), xd, yd, zd, data) $
         ELSE    $ 
            stat = rdata(dtype, unit, mats(1, *), xd, yd, zd, data, SCALE=scales)
         IF (stat EQ !ECAT.ERROR) THEN RETURN, !ECAT.ERROR
         IF (keyword_set(ZOOM)) THEN BEGIN
            z = [zoom*xd, zoom*yd]
            IF (zoom LT 1) THEN  $
               m = [xd mod z(0), yd mod z(1)] $
            ELSE   $
               m = [zoom*xd mod xd, zoom*yd mod yd]
            IF (total(m) GT 0) THEN BEGIN
               z = [xd, yd]
               zoom = 1
            ENDIF
            d = make_array(z(0), z(1), zd, SIZE=size(data(*, *, 0)))
            xd = z(0) & yd = z(1)
            FOR i=0, zd-1 DO d(*, *, i) = rebin(data(*, *, i), z(0), z(1))
            data = temporary(d)
            zf = zoom
         ENDIF
      ENDELSE
   ENDIF
   ecat_data = {fname:name,  $
                ftype:ftype, $
                dtype:dtype, $
                nmats:n,  $
                mats:mats, $
                xdim:xd,  $
                ydim:yd,  $
                zdim:n_elements(sh), $
                zoom:zf,  $
                mh:mh,  $
                sh:sh,  $
                data:data $
               }

   free_lun, unit
   RETURN, !ECAT.OK

   io_error : set_ecat_error, 0, "RECAT7", UNIT=unit
   RETURN, !ECAT.ERROR
END
