;****************************************
function read_ifh,file_name,scale_factors
;****************************************

;Purpose: Read interfile header.
; John Ollinger, 10/16/95

ifh = {InterFile_Header, $
	modality:string(' '),		$
	orig_system:string(' '),		$
	conversion_program:string(' '),	$
	program_version:string(' '),	$
	program_date:string(' '),		$
	original_institution:string(' '),	$
	data_file:string(' '),		$
	patient_ID:string(' '),		$
	study_date:string(' '),		$
	number_format:string(' '),	$
	bytes_per_pixel:0,	$
	num_dimensions:0,	$
	matrix_size_1:0,	$
	matrix_size_2:0,	$
	matrix_size_3:0,	$
	matrix_size_4:0,	$
	volume_sf:0.,		$
	zoom_factor_1:0,	$
	zoom_factor_2:0,	$
	zoom_factor_3:0,	$
	mat_init_element_1:string(' '),	$
	mat_init_element_2:string(' '),	$
	mat_init_element_3:string(' ')	$
	}

;xdim=fix(strmid(s,strpos(s,":=")+2,len-strpos(s,":=")-2))
s = string(" ")
get_lun,lu
openr,lu,file_name

eof = 0
while (eof eq 0) do begin
    readf,lu,s,FORMAT='(a)'
    if(strpos(s,"image modality") ge 0) then $
	ifh.modality = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"originating system") ge 0) then $
	ifh.orig_system = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"conversion program") ge 0) then $
	ifh.conversion_program = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"program version") ge 0) then $
	ifh.program_version = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"program date") ge 0) then $
	ifh.program_date = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"original institution") ge 0) then $
	ifh.original_institution = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"name of data file") ge 0) then $
	ifh.data_file = strcompress(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2),/REMOVE_ALL)
    if(strpos(s,"patient ID") ge 0) then $
	ifh.patient_ID = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"study date") ge 0) then $
	ifh.study_date = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"number format") ge 0) then $
	ifh.number_format = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"number of bytes per pixel") ge 0) then $
	ifh.bytes_per_pixel = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"number of dimensions") ge 0) then $
	ifh.num_dimensions = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"matrix size [1]") ge 0) then $
	ifh.matrix_size_1 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"matrix size [2]") ge 0) then $
	ifh.matrix_size_2 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"matrix size [3]") ge 0) then begin
	ifh.matrix_size_3 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
	scale_factors = fltarr(ifh.matrix_size_3)
	plane = 0
    endif
    if(strpos(s,"matrix size [4]") ge 0) then $
	ifh.matrix_size_4 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"volume scale factor") ge 0) then $
	ifh.volume_sf = float(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"scaling factor (mm/pixel) [1]") ge 0) then $
	ifh.zoom_factor_1 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"scaling factor (mm/pixel) [2]") ge 0) then $
	ifh.zoom_factor_2 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"scaling factor (mm/pixel) [3]") ge 0) then $
	ifh.zoom_factor_3 = fix(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
    if(strpos(s,"matrix initial element [1]") ge 0) then $
	ifh.mat_init_element_1 = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"matrix initial element [2]") ge 0) then $
	ifh.mat_init_element_2 = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"matrix initial element [3]") ge 0) then $
	ifh.mat_init_element_3 = strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2)
    if(strpos(s,"storage scale factor") ge 0) then begin
	scale_factors(plane) = float(strmid(s,strpos(s,":=")+2,strlen(s)-strpos(s,":=")-2))
	plane = plane + 1
    endif
    eof = eof(lu)
endwhile

free_lun,lu

return,ifh
end
