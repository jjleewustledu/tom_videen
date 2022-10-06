pro histoplot
; Initializations

csffile='csf.stats'
whitefile='white.stats'
grayfile='gray.stats'
clotfile='clot.stats'

x=fltarr(60,4)
bin=(lindgen(60))*2+1000
a = 0L
b = 0L

y=lonarr(10000)
n=0
openr,1,csffile
while not eof(1) do begin
	readf,1,a,b
	if (b gt 0) then begin
		for j=1,b do begin
			y[n]=a
			n=n+1
		endfor
	endif
	a = (a-1000)/2
	x[a,0] = x[a,0] + b
endwhile
close,1
x[*,0] = x[*,0]/float(max(x[*,0]))
mean=moment(y[0:n-1],SDEV=sd)
print,'CSF    ',mean[0],sd

y=lonarr(10000)
n=0
openr,1,whitefile
while not eof(1) do begin
	readf,1,a,b
	if (b gt 0) then begin
		for j=1,b do begin
			y[n]=a
			n=n+1
		endfor
	endif
	a = (a-1000)/2
	x[a,1] = x[a,1] + b
endwhile
close,1
x[*,1] = x[*,1]/float(max(x[*,1]))
mean=moment(y[0:n-1],SDEV=sd)
print,'White  ',mean[0],sd

y=lonarr(10000)
n=0
openr,1,grayfile
while not eof(1) do begin
	readf,1,a,b
	if (b gt 0) then begin
		for j=1,b do begin
			y[n]=a
			n=n+1
		endfor
	endif
	a = (a-1000)/2
	x[a,2] = x[a,2] + b
endwhile
close,1
x[*,2] = x[*,2]/float(max(x[*,2]))
mean=moment(y[0:n-1],SDEV=sd)
print,'Gray   ',mean[0],sd

y=lonarr(10000)
n=0
openr,1,clotfile
while not eof(1) do begin
	readf,1,a,b
	if (b gt 0) then begin
		for j=1,b do begin
			y[n]=a
			n=n+1
		endfor
	endif
	a = (a-1000)/2
	x[a,3] = x[a,3] + b
endwhile
close,1
x[*,3] = x[*,3]/float(max(x[*,3]))
mean=moment(y[0:n-1],SDEV=sd)
print,'Clot   ',mean[0],sd

set_plot,'X'	
loadct,0
;set_plot,'ps'
;device, filename="p5067_ct.ps",xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
plot,bin,x[*,0],psym=0, xr=[1000,1120], $
	charsize=1.4,charthick=2,xthick=2,ythick=2, $
	xtitle="Attenuation",ytitle="Relative Number", $
	title="P5067"
oplot,bin,x[*,1]
oplot,bin,x[*,2]
oplot,bin,x[*,3]
;device,/close
end
