
;***************
pro imgplot,corr
;***************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: plot,corr
     return
endif

xdim = n_elements(corr(*,0,0))
ydim = n_elements(corr(0,*,0))
zdim = n_elements(corr(0,0,*))


for j=0,ydim-1 do begin
    plot,corr(*,j,0)
    wait,.05
endfor

return
end

;*******************************
pro corrplot,emis,emis_corr,clip
;*******************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: corrplot,emis,emis_corr,clip
     return
endif

xdim = n_elements(emis(*,0,0))
ydim = n_elements(emis(0,*,0))
zdim = n_elements(emis(0,0,*))

minmax = fltarr(2)
;minmax(0) = min(emis_corr(*,*,z))
minmax(0) = -clip
minmax(1) = clip

line = fltarr(xdim)
for i=0,xdim-1 do begin
    line(i) = 0.
endfor

for z=0,zdim-1 do begin
    y = fix(ydim*randomu(seed))
    plot,emis(*,y,z),yrange=minmax
    oplot,emis_corr(*,y,z),psym=4
    oplot,line,symsize=.5
    wait,.03
endfor

return
end

;**********************************
pro scatplot,data,scat,diff,z,angle
;**********************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatplot,data,scat,corr,z[,angle]
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))

if(n_elements(angle) eq 0) then begin
    ymin = 0
    ymax = ydim-1
endif else begin
    ymin = angle
    ymax = angle
endelse

minmax = fltarr(2)
minmax(0) = min(diff(*,*,z))
minmax(1) = max(data(*,*,z))

for j=ymin,ymax do begin
    plot,scat(*,j,z),yrange=minmax
    oplot,diff(*,j,z),psym=4
    oplot,data(*,j,z),psym=6
    wait,.1
endfor

return
end

;************************************
pro scatstat,emis_3d_clip_file,thresh
;************************************

nargs=n_params()
if(nargs lt 2) then begin
     print,'Usage: scatstat,emis_3d_clip_file,thresh'
     return
endif

close,/all
spawn,'ls *final.cub',scatfiles
nfile=n_elements(scatfiles)

for i=0,nfile-1 do begin
    if i eq 0 then rd_cub_subset,emis_3d_clip_file,scatfiles(0),data
    rd_cub,scatfiles(i),scat
    pos = strpos(scatfiles(i),'.cub')
    tmp = strmid(scatfiles(i),0,pos)
    outfil = string(tmp,'_corr.cub')
    print,scatfiles(i)
    scatcor,data,scat,thresh,corr,outfil,emis_3d_clip_file
endfor


return
end

;*************************************************************
pro scatstat1,emis_3d_clip_file,scatfile,thresh,data,scat,corr
;*************************************************************

nargs=n_params()
if(nargs lt 6) then begin
     print,'Usage: scatstat1,emis_3d_clip_file,scatfile,thresh,data,scat,corr'
     return
endif

close,/all

rd_cub_subset,emis_3d_clip_file,scatfile,data
rd_cub,scatfile,scat
pos = strpos(scatfile,'.cub')
tmp = strmid(scatfile,0,pos)
outfil = string(tmp,'_corr.cub')
scatcor,data,scat,thresh,corr,outfil,emis_3d_clip_file

return
end

;*****************************************************************
pro scatstat_self,emis_3d_clip_file,scatfile,thresh,data,scat,corr
;*****************************************************************

nargs=n_params()
if(nargs lt 6) then begin
     print,'Usage: scatstat1,emis_3d_clip_file,scatfile,thresh,data,scat,corr'
     return
endif

close,/all

rd_cub_subset,emis_3d_clip_file,scatfile,data
rd_cub,scatfile,scat
pos = strpos(scatfile,'.cub')
tmp = strmid(scatfile,0,pos)
outfil = string(tmp,'_corr.cub')
scatcor_self,data,scat,thresh,corr,outfil,emis_3d_clip_file

return
end




;**************************
pro scatcomp,data,scat,corr
;**************************

nargs=n_params()
if(nargs lt 3) then begin
     print,'Usage: scatcomp,data,scat,corr'
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))
y = fltarr(xdim,ydim,3*zdim)

zz=0
for z=0,zdim-1 do begin
    y(*,*,zz+0) = data(*,*,z)
    y(*,*,zz+1) = scat(*,*,z)
    y(*,*,zz+2) = corr(*,*,z)
    zz = zz + 3
endfor
display_glob,y

return
end

;*****************************************************
pro clip,input,output,thresh,lv,uv,UVAL=uval,LVAL=lval
;*****************************************************

nargs=n_params()
if(nargs lt 3) then begin
    print,'Usage: clip,input,output,threshold,lval,uval,LVAL,UVAL
    print,'Set UVAL or LVAL for lval and/or uval to be used.'  
    return
endif

xdim = n_elements(input(*,0,0))
ydim = n_elements(input(0,*,0))
zdim = n_elements(input(0,0,*))

output = fltarr(xdim,ydim,zdim)
if keyword_set(lval) then begin
    if keyword_set(uval) then begin
	print,'Imposing both limits.',lv,uv
	for i=0,xdim-1 do begin
	    for j=0,ydim-1 do begin
	        for k=0,zdim-1 do begin
	            if(input(i,j,k) ge thresh) then output(i,j,k) = uv $
		    else output(i,j,k) = lv
	        endfor
	    endfor
	endfor
    endif else begin
;	Only LVAL is set.
	print,'Imposing lval.
	for i=0,xdim-1 do begin
	    for j=0,ydim-1 do begin
	        for k=0,zdim-1 do begin
	            if(input(i,j,k) ge thresh) then output(i,j,k)=input(i,j,k) $
		    else output(i,j,k) = lv
	        endfor
	    endfor
	endfor
    endelse
endif else begin
;   LVAL not set.
    if keyword_set(uval) then begin
	print,'Imposing uval.'
	for i=0,xdim-1 do begin
	    for j=0,ydim-1 do begin
	        for k=0,zdim-1 do begin
	            if(input(i,j,k) ge thresh) then output(i,j,k) = uv $
		    else output(i,j,k) = input(i,j,k)
	        endfor
	    endfor
	endfor
    endif else begin
	print,'No keywords set, clip does nothing.'
    endelse
endelse

return
end

;*********************************************************
pro rodwin
;pro rodwin,emis_2d_file,tran_rodwin_file,blank_rodwin_file
;*********************************************************

; Calculate ratio of emission counts in rod-windowed transmission sinogram
; to emission sinogram formed from fully 3d data.
; emis_2d_file: 31 plane emission sinogram formed from 3d scan with get2dsino
; tran_rodwin_file: Rod-windowed transmission scan with source in place.
; blank_rodwin_file: Rod-windowed transmission scan with no source.

;rd_cub,emis_2d_file,emis
;rd_cub,tran_rodwin_file,tran
;rd_cub,blank_rodwin_file,blank
rd_cub,'emis_2d.cub',emis
rd_cub,'tran_rdwin.cub',tran
rd_cub,'blank_rdwin.cub',blank

;clip,emis,emis_clip,500,1,0,/LVAL
clip,emis,mask,500,0,1,/LVAL,/UVAL
emis_diff = tran - blank
;clip,emis_diff,emis_diff_clip,100,0,0,/LVAL

;scl = mask*emis_diff/emis_clip
x = mask*emis_diff
y = mask*emis

for z=0,30 do begin
    n = total(mask(*,*,z))
    s = total(x(*,*,z))/total(y(*,*,z))
    print,z,s,n
end

return
end

;************
pro surf,func
;************

shade_surf,func,charsize=2,xtitle='Transverse Position',ytitle='Angle',ztitle='Scattered Coincidences in Counts',/horizontal


return
end

;***************************
pro scatdis,data,scat,corr,z
;***************************

nargs=n_params()
if(nargs lt 3) then begin
     print,'Usage: scatdis,data,scat,corr,z'
     return
endif

xdim = n_elements(data(*,0,z))
ydim = n_elements(data(0,*,z))
y = fltarr(xdim,ydim,3)

acorr = abs(corr(*,*,z))
y(*,*,0) = data(*,*,z)
y(*,*,1) = scat(*,*,z)
y(*,*,2) = acorr
display_glob,y

return
end


;*********************************************
pro scorplot,sino0,sino1,sino2,sino3,sino4,z,y
;*********************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scorplot,sino0,sino1,sino2,sino3,sino4,z[,y]
     return
endif

xdim = n_elements(sino0(*,0,0))
ydim = n_elements(sino0(0,*,0))

if(n_elements(y) eq 0) then begin
    ymin = 0
    ymax = ydim-1
endif else begin
    ymin = y
    ymax = y
endelse

minmax = fltarr(2)
line = fltarr(xdim)
for i=0,xdim-1 do begin
    line(i) = 1.
endfor

for j=ymin,ymax do begin
    minmax(0) = min(sino1(*,j,z))
    minmax(1) = max(sino0(*,j,z))
    plot,sino0(*,j,z),yrange=minmax,symsize=.5
    oplot,sino1(*,j,z),psym=4,symsize=.5 ; Diamond
    oplot,sino2(*,j,z),psym=6,symsize=.5 ; Square
    oplot,sino3(*,j,z),psym=5,symsize=.5 ; Triangle
    oplot,sino4(*,j,z),psym=2,symsize=.5 ; Asterisk
    oplot,line,symsize=.5
    wait,.2
endfor

return
end

;**********************************
pro scatiff,filnam,data,scat,corr,z
;**********************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatiff,filnam,data,scat,corr,z
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))

x = fltarr(xdim,ydim,3)
x(*,*,0) = data(*,*,z)
x(*,*,1) = scat(*,*,z)
x(*,*,2) = corr(*,*,z)

xtile = tile(x)
xscl = bytscl(xtile)
tiff_write,filnam,xscl,1

return
end

;**********************************
pro scatgif,filnam,data,scat,corr,z
;**********************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatgif,filnam,data,scat,corr,z
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))

x = fltarr(xdim,ydim,3)
xflip = fltarr(xdim*3,ydim)
x(*,*,0) = data(*,*,z)
x(*,*,1) = scat(*,*,z)
x(*,*,2) = corr(*,*,z)

;Layout images side-by-side.
xtile = tile(x)

; Flip image top to bottom.
for y=0,ydim-1 do begin
    tmp = xtile(*,y)
    xflip(0,y) = x(*,ydim - y - 1)
    xflip(0,ydim - y - 1) = tmp
endfor

;Scale to range of 0-255.
xscl = bytscl(xflip)

;Write as GIF image.
write_gif,filnam,xscl

return
end

;************************************
pro scatcor_all,data,data_corr,thresh
;************************************
nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatcor,data,scatter-corrected-data,thresh'
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))

mask = data gt thresh
mask = 1. - mask
mskdat = mask*data
scat = total(mskdat)
total = total(data)
mskdat = mask*data_corr
residual_tot = total(data_corr)
residual_scat = total(mskdat)
print,'Total: ',total,' Scattered: ',scat,' SF: ',100.*scat/total,'% Residual SF: ',100.*residual_scat/residual_tot

return

sumsumdat = 0
sumrms = 0
sumsum = 0
wrstrms = 0
bestrms = 1.e20
for k=0,zdim-1 do begin
    sumerr = 0
    rmserr = 0
    sumdat = 0;
    for j=0,ydim-1 do begin
        sumcorrow = total(mskdat(*,j,k))
	sumerr = sumerr + sumcorrow
	rowrms = sumcorrow*sumcorrow
	rmserr = rmserr + rowrms
	if(rowrms gt wrstrms) then begin
	    wrstrms = rowrms
	    wrstj = j
	    wrstk = k
	endif
        if(rowrms lt bestrms) then  begin
	    bestrms = rowrms
	    bestj = j
	    bestk = k
	endif
    endfor

    sumdat = total(data(*,*,k))
    sumrms = sumrms + rmserr
    sumsum = sumsum + sumerr
    sumsumdat = sumsumdat + sumdat
    sumerr = 100.*sumerr/sumdat
    rmserr = 100.*sqrt(ydim*rmserr)/sumdat
    print,'Sinogram #',k,', Summed error: ',sumerr,'%,  RMS error: ',rmserr,'%',format='(a,i3,a,f6.2,a,f6.2,a)'
endfor

print,'Maximum error at plane',wrstk,', row ',wrstj
print,'Minimum error at plane',bestk,', row ',bestj
sumrms = 100.*sqrt(ydim*zdim*rmserr)/sumsumdat
sumsum = 100.*sumsum/sumsumdat
print,'Mean sum error: ',sumsum,'%, Mean RMS error: ',sumrms,'%',format='(a,f6.2,a,f6.2)'

return
end

;************************************************
pro grantdat,datahd,scathd,datach,scatch,hdr,plot
;************************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: grantdat,datahd,scathd,datach,scatch,hdr,plot
     return
endif

hdr = strarr(9)
hdr(0,*) = 'Row'
hdr(1,*) = 'hd1_dat'
hdr(2,*) = 'hd1_scat'
hdr(3,*) = 'hd2_dat' 
hdr(4,*) = 'hd2_scat'
hdr(5,*) = 'ch1_dat' 
hdr(6,*) = 'ch1_scat'
hdr(7,*) = 'ch2_dat' 
hdr(8,*) = 'ch2_scat'

ind = findgen(128)
plot = fltarr(128,9)
plot(*,0) = ind
plot(*,1) = datahd(*,164,7)
plot(*,2) = scathd(*,164,7)

plot(*,3) = datahd(*,26,11)
plot(*,4) = scathd(*,26,11)

plot(*,5) = datahd(*,53,6)
plot(*,6) = scathd(*,53,6)

plot(*,7) = datahd(*,12,2)
plot(*,8) = scathd(*,12,2)

return
end

;*********************
pro iterdat,filnam,y,z
;*********************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: iterdat,filnam,y,z
     return
endif

rd_cub,'emis_sino_corr_32x32_48_s0.cub',iter0
rd_cub,'emis_sino_corr_32x32_48_s1.cub',iter1
rd_cub,'emis_sino_corr_32x32_48_s2.cub',iter2
rd_cub,'emis_sino_corr_32x32_48_s3.cub',iter3

hdr = strarr(5)
hdr(0,*) = 'Row'
hdr(1,*) = 'Iter0'
hdr(2,*) = 'Iter1'
hdr(3,*) = 'Iter2' 
hdr(4,*) = 'Iter3'

ind = findgen(128)
plot = fltarr(128,5)
plot(*,0) = ind
plot(*,1) = iter0(*,y,z)
plot(*,2) = iter1(*,y,z)
plot(*,3) = iter2(*,y,z)
plot(*,4) = iter3(*,y,z)

plotfile,filnam,hdr,plot

minmax = fltarr(2)
minmax(0) = min(iter1(*,y,z))
minmax(1) = max(iter0(*,y,z))
plot,iter0(*,y,z),yrange=minmax
oplot,iter1(*,y,z)
oplot,iter2(*,y,z)
oplot,iter3(*,y,z)

return
end

;****************************************************
pro scatcor_self,data,scat,thresh,corr,outfil,datafil
;****************************************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatcor,data-file,scatter-file,scatter-corrected-data'
     return
endif

xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))
corr = fltarr(xdim,ydim,zdim)

meandat = fltarr(xdim,ydim,zdim)
for k=0,zdim-1 do meandat(*,*,0) = meandat(*,*,0) + data(*,*,k)
meandat(*,*,0) = meandat(*,*,0)/zdim
for k=1,zdim-1 do meandat(*,*,k) = meandat(*,*,0)
plot,meandat(*,0,0)
oplot,data(*,0,0)

pos = strpos(outfil,'.cub')
tmp = strmid(outfil,0,pos)
txtfil = string(tmp,'.txt')

sumrms = 0
sumsum = 0
wrstrms = 0
bestrms = 1.e20
for k=0,zdim-1 do begin
    sumerr = 0
    rmserr = 0
    sumdat = 0;
    for j=0,ydim-1 do begin
        tmpdat1 = (data(*,j,k) lt thresh)*data(*,j,k)
        tmpdat = tmpdat1(*) + (data(*,j,k) gt thresh)*scat(*,j,k)
        tmpmean1 = (data(*,j,k) lt thresh)*meandat(*,j,k)
        tmpmean = tmpmean1(*) + (data(*,j,k) gt thresh)*scat(*,j,k)
        sumdat = sumdat + total(tmpdat1)
        corr(*,j,k) = tmpdat(*) - tmpmean(*)
        sumcorrow = total(corr(*,j,k))
	sumerr = sumerr + sumcorrow
	rowrms = sumcorrow*sumcorrow
	rmserr = rmserr + rowrms
	if(rowrms gt wrstrms) then begin
	    wrstrms = rowrms
	    wrstj = j
	    wrstk = k
	endif
        if(rowrms lt bestrms) then  begin
	    bestrms = rowrms
	    bestj = j
	    bestk = k
	endif
    endfor

    sumscat = total(scat(*,*,k))
    scatfrac = 100.*sumerr/sumscat
    sumerr = 100.*sumerr/sumdat
    rmserr = 100.*sqrt(ydim*rmserr)/sumdat
    sumsum = sumsum + sumerr
    sumrms = sumrms + rmserr
    print,'Sinogram #',k,', Summed error: ',sumerr,'%,  RMS error: ',rmserr,'%',format='(a,i2,a,f6.2,a,f6.2,a)'
endfor

print,'Maximum error at plane',wrstk,', row ',wrstj
print,'Minimum error at plane',bestk,', row ',bestj
print,'Mean sum error: ',sumsum/zdim,'%, Mean RMS error: ',sumrms/zdim,'%',format='(a,f6.2,a,f6.2)'

;wr_cub,outfil,datafil,corr

return
end

