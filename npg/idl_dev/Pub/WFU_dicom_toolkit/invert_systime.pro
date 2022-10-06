pro datetime_conv,month,day,year,hour,min,sec,datetimestamp

	dt=long(datetimestamp)

	caldat,(julday(1,1,1970)+dt/86400),month,day,year

	dt=dt - 86400*(dt/86400)
	hour=dt/3600

	dt=dt-3600*(dt/3600)
	min=dt/60

	sec=dt-60*(dt/60)

return
end



function days_in,year = year,month = month

leap = 0
if year mod 4 eq 0 then leap = 1
days = 365 + leap
md = days
if keyword_set(month) then begin
	month = month < 13
	case month of
	
	1: md = 31
	2: md = 28 + leap
	3: md = 31
	4: md = 30
	5: md = 31
	6: md = 30
	7: md = 31
	8: md = 31
	9: md = 30
	10: md = 31
	11: md = 30
	12: md = 31
	13: md = 0
	endcase
	if month eq 13 then badboy, 'bad month'
endif
reply = md
return,reply
end




function invert_systime,num
;------------------------------------------------
;this program will return the month/day/year format
;for a systime(1) input
;print, invert_systime(systime(1))
;for use with exam.EX_DATETIME field in genheader
;J. Maldjian
;-------------------------------------------------
datetimestamp = num
datetime_conv,month,day,year,hour,min,sec,datetimestamp
reply = {month:strtrim(month,2),day:strtrim(day,2),year:strtrim(year,2),$
		hour:strtrim(hour,2),min:strtrim(min,2),sec:strtrim(sec,2)}
return,reply
end
	



function forward_systime,month,day,year

Diff =   2.1086677e+11
if strlen(strtrim(year,2)) le 2 then begin
	prefix = '20'
	if strmid(strtrim(year,2),0,1) eq '9' then prefix = '19'
	if strlen(strtrim(year,2)) eq 1 then prefix = '200'
	year = prefix + strtrim(year,2)
endif
jthen = julday(month,day,year)
reply = (jthen*60.0*60.0*24.0 - diff)
return,reply

end


