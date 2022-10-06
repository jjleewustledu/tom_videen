function create_dicom_lookup

dt = path_of('dicom') + 'dicom_host_table.txt'
dt = read_txtfile(dt)
count = n_elements(dt)
;hosts = strarr(count + 1)
hosts = strarr(count)
aetitle = hosts
calltitle = hosts
port = hosts
ip = hosts

for i = 0,count - 1 do begin
	line = break_string(dt(i),sep = string(9b))
	hosts(i) = line(0)
	aetitle(i) = line(1)
	calltitle(i) = line(2)
	port(i) = line(3)
	ip(i) = line(4)
endfor
;hosts(i) = 'other'
;aetitle(i) = 'xxx'
;calltitle(i) = 'other'
;port(i) = '4006'
;ip(i) = ''


lookup = {	hosts:hosts,$
		aetitle:aetitle, $
		calltitle:calltitle,$
		port:port,$
		ip:ip  }
return,lookup
end
