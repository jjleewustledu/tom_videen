
function convert_SMN,smn = smn,filename = filename


if not(keyword_set(SMN)) then smn = '/export/home/fmri/SMN_DICOM/mr01.img'
zsize = 124
hs = 512
smn_cube = uintarr(256,256,zsize)
image = uintarr(256,256)
openr,/swap_if_little_endian,unit,/get_lun,smn
filehdr = bytarr(512)
print,'Opening SMN file ' + smn + ' ...'
imagehdr = filehdr
imagehdr_array = bytarr(512,zsize)
readu,unit,filehdr
for i = 0,zsize - 1 do begin

	readu,unit,imagehdr
	readu,unit,image
	imagehdr_array(*,i) = imagehdr
	smn_cube(*,*,i) = rotate(image,7)
endfor
close_lun,unit
reply = {cube:smn_cube,filehdr:filehdr,imagehdr:imagehdr_array}
return,reply

end





function test_smn

fmri_file =  '/export/home/fmri/SMN_DICOM/my_8bit_01.img'
smn_file = '/export/home/fmri/SMN_DICOM/mr01.img
out_file = '/export/home/fmri/SMN_DICOM/test01.img'
zsize = 124
image = uintarr(256,256)
out_cube = uintarr(256,256,124)
fmri_cube = bytarr(256,256,124)
openr,/swap_if_little_endian,unit,/get_lun,fmri_file
readu,unit,fmri_cube
close_lun,unit
factor = 256.0		;(for bytarr) ;(use 2 for ge loc file - intarr) 
out_cube(*) = fmri_cube(*) * factor 
filehdr = bytarr(512)
imagehdr = filehdr
print,'Performing SMN file conversion to ' + out_file
print,'SMN file = ' + smn_file
print, 'Input file = ' + fmri_file
openr,/swap_if_little_endian,smn_unit,/get_lun,smn_file
openw,/swap_if_little_endian,out_unit,/get_lun,out_file
readu,smn_unit,filehdr
writeu,out_unit,filehdr
for i = 0,zsize - 1 do begin
	readu,smn_unit,imagehdr
	readu,smn_unit,image
	writeu,out_unit,imagehdr
	writeu,out_unit,rotate(reform(out_cube(*,*,i)),7)
endfor
close_lun,smn_unit
close_lun,out_unit
out_cube = convert_smn(smn = out_file)
return,out_cube
end

pro test_header,header

s = n_elements(header)

tfile = '/export/home/fmri/SMN_DICOM/theader.txt'

openw,/swap_if_little_endian,unit,/get_lun,tfile
for i = 0,s - 1 do printf,unit,i,' :  ',header(i),'  :  ', string(header(i))
close_lun,unit
end




pro dicom2smn,path = path

;----------------------------------------------------------------
;This will take a dicom tar file all the way to a normalized loc
;so we can use it with idl_spm97
;---------------------------------------------------------------

;---------------------------------------------
;Convert the compressed tar file to 16-bit images
;----------------------------------------------
print,'Executing dicom_ximgconv...'
dicom_ximgconv,dir = path,examdir = examdir
smnpath = path + examdir + '/'
dicom_locsort,path = path

;------------------------------------------------
;perform the spm normalization
;necessary since this file has no RAS coordinates and
;was done at a different seesion
;------------------------------------------------
auto_norm,datapath = smnpath,/noplastic

loclist = findfile(smnpath + '*:dc:*.loc',count = count)
locfile = find_t1(loclist = loclist,sequence_type = sequence_type,tal = search4talloc,$
		template_name = template_name,/quiet)
extract_fname,locfile,locfname

;----------------------------------------
;remove excess baggage
;----------------------------------------
spawn,'rm ' + smnpath + '*' + locfname + '*.img'
spawn,'rm ' + smnpath + '*' + locfname + '*.hdr'

;-----------------------------------------------
;Run the manual normalization if this one sucked
;------------------------------------------------
title = 'Are You Happy?'
message = ['If the automated looks good then hit OK', 'If it sucks then run the manual routine']
run_manual = new_ok(buttons = ['Norm is OK', 'Run Manual Instead'],message = message,title = title,/bell)
if run_manual then picktal,filename = locfile,/convert
print,'DONE!'
end

