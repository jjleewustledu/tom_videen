function open_square_image, filename = filename,error = err,header = header,imagesize = imagesize,$
byteheader = byteheader,text = text,trc = trc

lswap = 1
bswap = 0
;-------------------------------
;Added for TRC VBM images
;-------------------------------
if keyword_set(trc) and !little_endian then begin
	lswap = 0
	bswap = 1
endif


;-------------
;program will also work with GE scrapbook images
;---------------

if not(keyword_set(header)) then header = 7904
if not(keyword_set(imagesize)) then imagesize = 256
image = intarr(imagesize,imagesize)

if not(keyword_set(filename)) then $
filename = pickfile(title = 'CHOOSE LOC FILE',get_path = anatdir)

openr,swap_if_little_endian = lswap,swap_if_big_endian = bswap,unit,/GET_LUN,filename,error = err

valid = 1
if err eq 0 then valid = 0
status = fstat(unit)
if status.size eq 0 then begin
	err = 1
	valid = 1
endif
s64 = 64.0 * 64 * 2
;imagesize = fix(sqrt(fix(status.size/s64) * s64/2.0))
imagesize = (fix(sqrt((fix(fix(status.size/s64))))) *  64)
header = status.size - (imagesize * 2.0 * imagesize) 

;print, header
if header gt 0 then begin
	byteheader = bytarr(header)
	readu,unit,byteheader
endif

while valid eq 0 do begin


a = assoc(unit,intarr(imagesize,imagesize),header)
on_ioerror,bad_num
image = a(0)
valid = 1

bad_num: if not(valid) then begin
				if err eq 0 then close_lun,unit
				valid = 1 
				err = -1
				badboy,'INVALID FILE SELECTED',filename
 		endif

endwhile



if err eq 0 then  CLOSE_LUN,unit

;--------------------------------
;for GE scrapbook files
;-------------------------------
if strpos(filename,'.SCPT.') gt 0 then begin
	text = image
	text(*) = 0
	temp = where(image ge 32767 or image le -32767,count)
	if count gt 0 then begin
		text(temp) = 1000
		image(temp) = 0
		image(temp) = max(image) + 10
	endif
endif
	
;-------------------------------
;Must swap endian for linux os
;------------------------------
if !version.os eq 'linux' then image = swap_endian(image)
return,image

end
