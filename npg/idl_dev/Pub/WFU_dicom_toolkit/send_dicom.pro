pro send_dicom,hostname = hostname,files = files,aetitle = aetitle,calltitle = calltitle,port = port,ip = ip

;--------------------------------------------------------------------------------------------------
;This program will send dicom images into a PACS or DICOM database
;syntax:  send_dicom,hostname = hostname,files = files,aetitle = aetitle,calltitle = calltitle,port = port
;You can call this program simply using:
;	send_dicom
;It will prompt you for the directory of file names, and it will read the entries in the 
;	dicom_host_table.txt file
;This is a tab-delimitted file, with the format described in the file.
;Command line options when not using the dicom_host_table.txt file:
;hostname = the hostname of the PACS or DICOM Recipient
;files = string array of dicom filenames to send
;aetitle = application entity title
;calltitle = application call title (Not necessary)
;port = port on DICOM recipient to send images (4006)
;ip = ipaddress of DICOM recipient
;hostname = hostname of DICOM reciipient (Can be same as ip)
;For the GE AW workstations all you need to send is the hostname (The defaults should work)
;J. Maldjian 7-24-2000
;--------------------------------------------------------------------------------------------------

multiple = 0
if not(keyword_set(files)) then begin
	fn = pickfile(title = 'Choose first dicom image from send directory')
	extract_pname,fn,pname
	extract_fname,fn,fname
	filter = pname + strmid(fname,0,strlen(fname) - 4) + '*'
	files = findfile(filter,count = fcount)
	if fcount le 0 then begin
		print,'No files found'
		return
	endif
endif
if not(keyword_set(hostname)) then begin
	dicom_options,reply = doptions
	multiple = 1
	hostname = doptions.host
	if not(doptions.send) then begin
		print,'Quitting...Dicom send option not selected'
		return
	endif

endif
	

if not(keyword_set(port)) then port = '4006'
if not(keyword_set(aetitle)) then aetitle = 'xxx'
sendcmd = path_of('dicom_executable') + 'send_image '
fcount = n_elements(files)
flist = ''
for i = 0,fcount - 1 do flist = flist + ' ' + files(i)
;-----------------------------------------------
;Dicom executables only compiled for Solaris now
;Attempt to rsh through alexia
;-----------------------------------------------
;pingme=path_of('shell_scripts') + 'pingme'
;if !little_endian then begin
;	print,'Dicom CTN binaries only compiled for Solaris'
;	rsh_ip='alexia'
;	spawn,pingme + ' ' + rsh_ip,host_alive,r2
;	if host_alive(0) eq '1' then sendcmd = 'rsh alexia ' + sendcmd $
;		else begin
;			print,'Exiting Dicom Send'
;			return
;		endelse
;endif	

if multiple eq 1 then begin
	temp = where(doptions.sendlist gt 0,nhosts)
	if nhosts le 0 then begin
		print,'No Hosts selected, exiting...'
		return
	endif
	for i = 1,nhosts do begin
		aetitle = doptions.lookup.aetitle(temp(i-1))
		calltitle = doptions.lookup.calltitle(temp(i-1))
		port = doptions.lookup.port(temp(i-1))
		hostname = doptions.lookup.hosts(temp(i-1))
		ip = doptions.lookup.ip(temp(i-1))
		host_alive = pingme(ip)
		;spawn,pingme + ' ' + ip,host_alive,r2
		if host_alive eq '1' then begin
			cmd = sendcmd + '-a ' + aetitle + ' -c ' + calltitle + ' ' + ip + ' ' + port + ' ' + flist + ' &'
			print,'sending dicom images to host  ' + hostname + ' ...'
			spawn,cmd,reply
		endif else print, 'Error:  ' + hostname + ' ' + ip + ' is not alive' 
	endfor
endif

if multiple eq 0  then begin
	if not(keyword_set(ip)) then ip = hostname
	if not(keyword_set(hostname)) then hostname = ip
	if not(keyword_set(calltitle)) then calltitle = hostname
	host_alive = pingme(ip)
	if host_alive(0) eq '1' then begin
		cmd = sendcmd + '-a ' + aetitle + ' -c ' + calltitle + ' ' + ip + ' ' + port + ' ' + flist + ' &'
		print,'sending dicom images to host  ' + hostname + ' ' + ip + ' ...'
		spawn,cmd,reply
	endif else print, 'Error:  ' + hostname + ' ' + ip + ' is not alive'
endif


;cmd = sendcmd + '-a ' + aetitle + ' -c ' + calltitle + ' ' + hostname + ' ' + port + ' '
;extract_pname,files(0),p
;spawn,'echo $$',r1
;temp_list = p + 'dicom_send_filelist_' + strtrim(r1(0),2)
;print,'writing filelist to ' + temp_list
;openw,unit,temp_list,/get_lun
;for i = 0,fcount - 1 do printf,unit,files(i)
;close_lun,unit
;cmd2 = "'" + cmd + "'"
;delete_flag = '1'
;send_cmd = path_of('shell_scripts') + 'repeat_command ' + cmd2 + ' ' + temp_list + ' ' + delete_flag + ' &'
;print,'send command is:'
;print, send_cmd
;spawn,send_cmd


end
