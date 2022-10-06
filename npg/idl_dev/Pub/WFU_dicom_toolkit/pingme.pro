function pingme,ip,r1 = r1
;------------------------------------------------------------------
;program will ping a remote host
;the -w is for timeout, it is already in the pingme executable for linux and solaris
;the additional command line option will be ignorned for those OS
;it is added here for windows to timeout
;------------------------------------------------------------------
pingme_cmd = path_of('dicom_executable') + 'pingme ' + ip + ' -w 5'
spawn,pingme_cmd,r1,r2
host_alive = r1(0)
if !version.os eq 'Win32' then begin
	host_alive = '0'
	r1 = strlowcase(r1)
	pos = where(strpos(r1,'received') ge 0,count)
	pos = pos(0)
	if count gt 0 then begin
		received = strsplit(r1(pos),/extract)
		pos = where(strpos(received,'received') ge 0)
		pos = pos(0) + 2
		if received(pos) gt 0 then host_alive = 1
	endif
endif
if host_alive ne '1' then print,'No response from ',ip, ' ', r1
return,host_alive
end
