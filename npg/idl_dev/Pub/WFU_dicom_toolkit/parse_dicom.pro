forward_function make_dicom_pheader


function replace_string,line,char = char,rep=rep
return,line
end

function replace_string,inline,char = char,rep=rep
line=inline
if not(keyword_set(rep)) then rep = ''
q = strpos(line,char)
if q ge 0 then line = strmid(line,0,q) + rep + strmid(line,q + 1,strlen(line))
q = strpos(line,char)
if q ge 0 then line = replace_string(line,char = char,rep=rep)
return,line
end

function parse_dicom,filename = filename
;----------------------------------------------------------------------------------
;This program will create a callable dicom header function (make_dicom_pheader.pro)
;based on the input dicom file, compile the function, and return the structure
;J. Maldjian 7-24-2000
;----------------------------------------------------------------------------------

if not(keyword_set(filename)) then filename = pickfile(title = 'Please Choose a dicom file to parse')
extract_pname,filename,pname
header_dump = pname + 'header_dump'
;dump_exe = '/export/home/fmri/DICOM_TOOLS/' + 'dcm_dump_file'
dump_exe = path_of('dicom_executable') + 'dcm_dump_file'
;spawn, '/export/home/fmri/DICOM_TOOLS/dcm_dump_file ' +  filename + ' > ' + header_dump
dump_cmd = dump_exe + ' ' +  filename + ' > ' + header_dump
spawn, dump_cmd
;outputfile = '/export/home/fmri/DICOM_TOOLS/make_dicom_pheader'
outputfile = pname + 'make_dicom_pheader'
separator = ';-----------------------------------------------------'
openw,unit,outputfile + '.pro',/get_lun
printf,unit,'function make_dicom_pheader'
printf,unit,separator
printf,unit,';This program will construct a structure of the dicom header '
printf,unit,';substructures include group, element, and value '
printf,unit,';The code in this file was automatically '
printf,unit,';generated using the parse_dicom routine '
printf,unit,';Filename used:  ' + filename 
printf,unit,separator
printf,unit,' '
printf,unit,separator
printf,unit,';Now define all the substructures'
printf,unit,separator
printf,unit,' '
text = read_txtfile(header_dump)
loop = n_elements(text)
;loop = 10
tab = string(9b)
k = -1
strucline = strarr(5000)
strucline(plus(k)) = "dheader = { $"
ending = ",$"
for i = 0,loop - 1 do begin
	line = text(i)
	;p = break_string(line,sep = '//')
	p=strsplit(line,'//',/extract)
	;print, line
	if n_elements(p) ge 2 then begin
		ids = break_string(p(0),sep = ' ')
		group = ids(0)
		element = ids(1)
		;-----------------------------------
		;Elements 0000 should not be touched
		;-----------------------------------
		ok = 1
		if element eq '0000' then ok = 0
		if group eq '0008' and element eq '0001' then ok = 0	;Unimplemented
		if group eq '0028' and element eq '0102' then ok = 0	;High_bit
		;if group eq '0038' and element eq '0300' then ok = 0	;Patient_location?
		if group eq '7fe0' then ok = 0				;Pixel data
		;if group eq '0028' and element eq '0100' then ok = 0	;Bits allocated
		;if group eq '0028' and element eq '0101' then ok = 0	;Bits stored
		;if group eq '0028' and element eq '0010' then ok = 0	;Rows
		;if group eq '0028' and element eq '0011' then ok = 0	;Columns

		if ok then begin
			if n_elements(p) gt 2 then value = p(2) else value=''
			;Rows and columns have an additional entry for some reason
			if group eq '0028' and (element eq '0010' or element eq '0011' or element eq '0100' or element eq '0101') then begin
				value = str_sep(value,' ')
				value = value(n_elements(value) - 1)
			endif				
			d = p(1)
			d = strtrim(d,2)
			bd = str_sep(d,' ')
			descriptor = ''
			for j = 1,n_elements(bd) -1 do begin
				bd(j)= replace_string(bd(j),char ="'")
				bd(j)= replace_string(bd(j),char ="/",rep ='_')
				bd(j)= replace_string(bd(j),char ="-",rep ='_')
				bd(j)= replace_string(bd(j),char =",",rep ='_')
				bd(j)= replace_string(bd(j),char ="&",rep ='_')
				bd(j)= replace_string(bd(j),char =")")
				bd(j)= replace_string(bd(j),char ="(")
				if (bd(j) ne '(RET)' ) then begin 
					if strlen(strtrim(bd(j),2)) eq 0 then $
					descriptor = descriptor + '_' else $
						descriptor = descriptor + '_' + bd(j)
				endif
			endfor
			descriptor = strmid(descriptor,1,strlen(descriptor))
			if strlowcase(descriptor) eq 'group_length' then descriptor = descriptor + '_' + group
			oline = descriptor + tab + tab + "={group:'"+ group + "',element:'" + element + "',value:'" + $
				value + "'}"
			;-------------------------------------------------
			;Check if this is a duplicate structure element
			;-------------------------------------------------
			repeater = 0
			if total(strpos(strucline,descriptor) + 1) gt 0 then begin
				repeater = 1
			endif
			if not(repeater) then begin
				strucline(plus(k)) = descriptor + ":" + descriptor +ending
				printf,unit,oline
			endif
			;print,'descr:   ',descriptor
			;print,'group:   ', group
			;print,'element: ', element
			;print,'value:   ', value
		endif
	endif
endfor
strucline = strucline(0:k)
lastline = strucline(k)
lastline = strmid(lastline,0,strlen(lastline)-2) + '}'
strucline(k) = lastline
printf,unit,' '
printf,unit,separator
printf,unit,';Now define the parent dicom header structure'
printf,unit,separator
printf,unit,' '
for i = 0,n_elements(strucline) - 1 do printf,unit,strucline(i)

printf,unit,' '
printf,unit,'return,dheader'
printf,unit,'end'

close_lun,unit	
extract_pname,outputfile,p
extract_fname,outputfile,fn
cd,p,current=here
resolve_routine,'make_dicom_pheader',/is_function
dheader = make_dicom_pheader()
cd,here
return,dheader
end
