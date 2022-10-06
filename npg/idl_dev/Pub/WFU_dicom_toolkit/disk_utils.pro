; this will take care of /data/nfs and /nfs_auto
; for any type of string, either folder name
; or file name.
; standard path
function stpath,string


	path=string


	for i=0,n_elements(string)-1 do begin
		; Solaris type of file structure.
		to_replace='/nfs_auto/' 
		; only replace head of string
		if(strpos(string[i],to_replace) EQ 0) then $
			path[i]='/data/'+strmid(string[i],strlen(to_replace))
	endfor

	string=path

	for i=0,n_elements(string)-1 do begin
		; Linux type of file structure
		to_replace='/data/nfs/'
		if(strpos(string[i],to_replace) EQ 0) then $
			path[i]='/data/'+strmid(string[i],strlen(to_replace))
	endfor
		
	return,path

end



function correct_path,filenames_in
;This program will get the full correct path name
;accounting for linked files, and filenames without paths (ie '*.Z')

filenames = filenames_in
cd,current = home
if strmid(filenames(0),0,1) eq '/' then begin
	extract_pname,filenames(0),pn
	for i = 0,n_elements(filenames) - 1 do begin
		extract_fname,filenames(i),fn
		filenames(i) = fn
	endfor
	cd,stpath(pn)
endif
cd,current = here
for i = 0,n_elements(filenames) - 1 do filenames(i) = here + '/' + filenames(i)
cd,home
return,filenames
end



function break_string,input,separator = separator
	if not(keyword_set(separator)) then separator = ' '
	parts = str_sep(input,separator)
	j = n_elements(parts)
	parts2 = strarr(j)
	z = -1
	reply = ''
	for i = 0,j-1 do begin
		if strlen(strtrim(parts(i),2)) gt 0 then begin
			z = z + 1
			parts2(z) = strtrim(parts(i),2)
		endif
	endfor
	if z ge 0 then reply = parts2(0:z)
	return,reply
end
	
function disk_df,mountpoint,parts = parts
	;function returns structure of disk usage for filesystem in kilobytes
	;including available space
	server = path_of('hostname')
	rmount = ''
	mountpoint = correct_path(mountpoint)
	extract_pname,mountpoint,pn
	spawn,'ls ' + pn,quiet
	spawn,'df -k ' + pn,output
	if n_elements(output) eq 3 then output(1) = output(1) + ' ' + output(2)
	parts = break_string(output(1))
	descrip = break_string(output(0))
	fs = parts(0)
	crossmount = str_sep(parts(0),':')
	if n_elements(crossmount) gt 1 then begin
		server = crossmount(0)
		rmount = crossmount(1) 
	endif
		
	reply = {filesystem:parts(0),kbytes:float(parts(1)),used:float(parts(2)),avail:float(parts(3)),capacity:parts(4),mount:parts(5),server:server,rmount:rmount}
	return,reply
end

function disk_du,dir,parts = parts
	;function returns structure of disk usage for filesystem in kilobytes
	spawn,'du -ks ' + dir,output
	bout = byte(output(0))
	temp = where(bout eq 9,count)
	if count ne 0 then bout(temp) = 32
	output = string(bout)
	parts = break_string(output)
	reply = {directory:parts(1),used:float(parts(0))}
	return,reply
end
