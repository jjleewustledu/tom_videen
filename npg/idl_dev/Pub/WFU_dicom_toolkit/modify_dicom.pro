pro modify_dicom,image = image,filename = filename,values = values,dh = dh

;-------------------------------------------------------
;This program will modify a dicom file with image data
;-------------------------------------------------------

;-----------------------------------------------------------
;Generate the values structure to update some header info
;-----------------------------------------------------------
if not(keyword_set(values)) then begin
	ID_Study_Description =  "TEST FOR FMRI MODIFICATIONS"
	ID_Series_Description = "AXIAL T1 POST GAD FUSED TO FMRI"
	;Class_UID = '1.0.000.10008.5.1.4.1.1.4'
	;k = get_dcm_value(filename = fn,description = 'Study Instance UID')
	;Study_Instance_UID  =  '1.2.840.113619.2.1.1.2702429029.814.936099513.377'
	;Series_Instance_UID = '1.2.840.113619.2.1.1.2702429029.814.936099513.634'
	;Study_ID	=	'2956707'
	values={study_desc:ID_Study_Description,series_desc:ID_Series_Description}
endif
;-----------------------------------------------------
;write the header info to a formatted textfile for 
;ascii input to dcm_modify_object
;-----------------------------------------------------
extract_pname,filename,p
textfile = p + 'mods'
openw,unit,textfile,/get_lun
printf,unit,'0008 1030 "' + values.study_desc + '"'
printf,unit,'0008 103e "' + values.series_desc + '"'
;printf,unit,'0008 0016 "' + values.class_uid + '"'
;printf,unit,'0200 000d "' + values.study_uid + '"'
;printf,unit,'0200 000e "' + values.series_uid + '"'
;printf,unit,'0200 0010 "' + values.study_id + '"'
;printf,unit,'0200 0011 "' + values.series_num + '"'
close_lun,unit
;----------------------------------------------------------------------------
;0008 0016    // ID SOP Class UID   	//1.0.000.10008.5.1.4.1.1.4
;0020 000d    //         REL Study Instance UID//1.2.840.113619.2.1.1.2702429029.814.936099513.377
;0020 000e    //        REL Series Instance UID//1.2.840.113619.2.1.1.2702429029.814.936099513.634
;0020 0010    //                   REL Study ID//2956707 
;0020 0011    //              REL Series Number//6 
;0020 0012    //         REL Acquisition Number// 1
;0008 1030  "TEST FOR FMRI MODIFICATIONS"
;0008 103e  "AXIAL T1 POST GAD FUSED TO FMRI"
;------------------------------------------------------------------------------

dimage = get_dcm_value(filename = filename,description = 'Pixel Data')
sz = size(dimage)
xdim = sz(1)
ydim = sz(2)
;bytesize * xdim *ydim
imsize = 1.0*sz(0) * xdim*ydim	

;-------------------------------------------------------
;Modify the header fields and generate new dicom file
;-------------------------------------------------------
outputfile = filename + '.fmri.dcm'
cmd = '/export/home/fmri/DICOM_TOOLS/dcm_modify_object -i ' + textfile + ' ' + filename + ' ' + outputfile
spawn,cmd

;---------------------------
;Compute header offset
;---------------------------
openr,unit,outputfile,/get_lun
fsize = fstat(unit)
close_lun,unit
hsize = fsize.size - imsize

;-------------------------------------------------------
;Modify the image portion now
;swap endian if this is a big-endian machine (sparc)
;-------------------------------------------------------
swap_endian = !big_endian
openu,unit,outputfile,/get_lun,swap_endian = swap_endian
a = assoc(unit,intarr(xdim,ydim),hsize)
a(0) = image
close_lun,unit

;-------------------------------------------------------
;Now test it
;-------------------------------------------------------
dh = read_dicomheader(filename = outputfile,image = dimage)
print, total(dimage - image)
tvscl,dimage
end






