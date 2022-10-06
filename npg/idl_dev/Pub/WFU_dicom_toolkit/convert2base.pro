;----------------------------------------
;Programs for converting to hexadecimal
;given string input
;J. Maldjian 7-24-2000
;reply = convert2base('0010',base = 16)
;---------------------------------------

function convert2hex,char,base = base

case strlowcase(char) of
	'0': num = 0
	'1': num = 1
	'2': num = 2
	'3': num = 3
	'4': num = 4
	'5': num = 5
	'6': num = 6
	'7': num = 7
	'8': num = 8
	'9': num = 9
	'a': num = 10
	'b': num = 11
	'c': num = 12
	'd': num = 13
	'e': num = 14
	'f': num = 15
	else: num = 0
endcase
if num gt (base -1) then num = -1
return,num
 end  






function convert2base,number,base = base

if not(keyword_set(base)) then base = 16
base = long64(base)
len = strlen(number)
reply = long(0)
pos = -1
for i = 0,len - 1 do begin
	nextchar = strmid(number,len - (i+1),1)
	num = convert2hex(nextchar,base = base)
	if num ge 0 then begin
		pos = pos + 1
		reply = reply + long64((base^pos)*num)
	endif
	;help, reply
endfor
if reply lt 2l^30 then reply = long(reply)
;if reply lt 2^14 then reply = fix(reply)
return,reply
end
	
	
