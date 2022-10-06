pro write_dicom,image = image,dh = dh,filename = filename,outputdir = outputdir,template = template
;--------------------
;J. Maldjian
;-------------------

sz = size(image)
xdim = sz(1)
ydim = sz(2)

;------------------------------------------------------------------
;The generic 256x256 template will now work with all image sizes
;------------------------------------------------------------------
if not(keyword_set(template)) then template = path_of('dicom') + 'TEMPLATES/' + 'dicom_MR_' +   strtrim(xdim,2) + 'x' +  strtrim(ydim,2)
template = findfile(template,count = count)
if count le 0 then template = path_of('dicom') + 'TEMPLATES/' + 'dicom_MR_256x256'
template = template(0)
dcm_modify_exe = path_of('dicom_executable')  + 'dcm_modify_object'
if not(keyword_set(filename)) then begin
	outuid = dh.SOP_Instance_UID.value
	outuid = str_sep(outuid,'.')
	outf = ''
	for j = 0,n_elements(outuid) - 2 do outf = outf + outuid(j) + '.'
	imnum = strtrim(outuid(n_elements(outuid) -1),2)
	if strlen(imnum) lt 2 then imnum = '00' + imnum
	if strlen(imnum) lt 3 then imnum = '0' + imnum
	outputfile = outf  + imnum
	filename = outputfile
endif
outputfile = outputdir + filename
cd,outputdir,current = here

;dcm_modify_object -i hscript.temp  dfile testh
;---------------------------------------
;Create the header modification script
;---------------------------------------

textfile = outputdir + 'hscript.temp'
tags = tag_names(dh)
openw,unit,textfile,/get_lun
for i = 0,n_elements(tags) - 1 do begin
	line = strtrim(dh.(i).group,2) + ' ' + strtrim(dh.(i).element,2) + ' "' + strtrim(dh.(i).value,2) + '"'
	printf,unit,line
endfor
;printf,unit,'7fe0 0010 "131072"'
close_lun,unit
;------------------------
;Modify the Header
;------------------------
cmd = dcm_modify_exe + ' -i ' + textfile + ' ' + template + ' ' + outputfile
;-----------------------------------------------
;Dicom executables only compiled for Solaris now
;Attempt to rsh through alexia
;-----------------------------------------------
;if !little_endian then begin
;	print,'Dicom CTN binaries only compiled for Solaris'
;	rsh_ip='alexia'
;	pingme=path_of('shell_scripts') + 'pingme'
;	spawn,pingme + ' ' + rsh_ip,host_alive,r2
;	if host_alive(0) eq '1' then cmd = 'rsh alexia ' + cmd $
;		else begin
;			print,'Exiting Dicom Write'
;			return
;		endelse
;endif	
	
;print,cmd
spawn,cmd

;---------------------------
;Compute the image bytesize
;---------------------------
imsize = 1.0*bytesize(typecode(image)) * xdim*ydim


;-----------------------------------------------
;Get the new dicom byteheader and header offset
;-----------------------------------------------
nimage = open_square_image(filename = outputfile,byteheader = byteheader,header = header)
swap_endian = !big_endian
openw,unit,outputfile,/get_lun,swap_endian = swap_endian
;-----------------------------------------------------------------------------------------------
;first overwrite the header only up to the pixel image group length field (7fe0 0010 type: long)
;then write the true image size to complete the header
;then write the image
;-----------------------------------------------------------------------------------------------
writeu,unit,byteheader(0:(header - 1) - 4)
writeu,unit,long(imsize)
writeu,unit,rotate(image,7)
close_lun,unit


;---------------------------
;Compute header offset
;---------------------------
;openr,unit,outputfile,/get_lun
;fsize = fstat(unit)
;close_lun,unit
;hsize = fsize.size - imsize

;-------------------------------------------------------
;Modify the image portion now
;swap endian if this is a big-endian machine (sparc)
;-------------------------------------------------------
;swap_endian = !big_endian
;openu,unit,outputfile,/get_lun,swap_endian = swap_endian
;a = assoc(unit,intarr(xdim,ydim),hsize)
;a(0) = rotate(image,7)
;close_lun,unit


;---------------------
;now test open the image
;--------------------
;lh = read_dicomheader(filename = outputfile,image = nimage)
;print,total(nimage - image)
;tvscl,nimage

cd,here

end



