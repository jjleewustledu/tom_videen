;**********************************
pro scatcor,data,scat,threshin,corr
;**********************************

nargs=n_params()
if(nargs eq 0) then begin
     print,'Usage: scatcor,data,scatter,true_threshold,corrected_data'
     return
endif

thresh = threshin(0) -.001
xdim = n_elements(data(*,0,0))
ydim = n_elements(data(0,*,0))
zdim = n_elements(data(0,0,*))

ydim1 = ydim/8
dat1 = fltarr(xdim,ydim1,zdim)
scat1 = fltarr(xdim,ydim1,zdim)
corr = fltarr(xdim,ydim1,zdim)
jj = 0
for j=0,ydim1-1 do begin
    dat1(*,j,*) = total(data(*,jj:jj+7,*),2) 
    scat1(*,j,*) = total(scat(*,jj:jj+7,*),2) 
    jj = jj + 8
endfor
dat1 = dat1*(scat1 gt 0)
thresh = 8*thresh

sumrmsp = 0
sumrms = 0
sumsum = 0
wrstrmsp = 0
bestrmsp = 1.e20
tmpdat = fltarr(xdim)
tmpdat1 = fltarr(xdim)
for k=0,zdim-1 do begin
    sumerr = 0
    rmsperr = 0
    sumdat = 0
    rms = 0
    for j=0,ydim1-1 do begin
	mask = dat1(*,j,k) gt thresh
	dil = replicate(1,11)
 	mask = dilate(mask,dil)
        tmpdat1(*) = (1-mask)*dat1(*,j,k)
        tmpdat(*) = tmpdat1(*) + (mask)*scat1(*,j,k)
        sumdat = sumdat + total(tmpdat1)
        corr(*,j,k) = tmpdat(*) - scat1(*,j,k)
        sumcorrow = total(corr(*,j,k))
	sumerr = sumerr + sumcorrow
	rowrmsp = sumcorrow^2
	rmsperr = rmsperr + rowrmsp
	rms = rms + total(corr(*,j,k)^2)
	if(rowrmsp gt wrstrmsp) then begin
	    wrstrmsp = rowrmsp
	    wrstj = j
	    wrstk = k
	endif
        if(rowrmsp lt bestrmsp) then  begin
	    bestrmsp = rowrmsp
	    bestj = j
	    bestk = k
	endif
    endfor

    sumscat = total(scat1(*,*,k))
    scatfrac = 100.*sumerr/sumscat
    sumerr = 100.*sumerr/sumdat
    rms = 100.*sqrt(ydim1*xdim*rms)/sumdat
    rmsperr = 100.*sqrt(ydim1*rmsperr)/sumdat
    sumsum = sumsum + sumerr
    sumrmsp = sumrmsp + rmsperr
    sumrms = sumrms + rms
    print,'Sinogram #',k,', Summed error: ',sumerr,'%,  RMSPE: ',rmsperr,'% RMSE: ',rms,'%',format='(a,i2,a,f6.2,a,f6.2,a,f6.2,a)'
endfor

print,'Maximum error at plane',wrstk,', row ',wrstj
print,'Minimum error at plane',bestk,', row ',bestj
print,'Mean sum error: ',sumsum/zdim,'%, Mean RMSPE : ',sumrmsp/zdim,'%, Mean RMS: ',sumrms/zdim,'%',format='(a,f6.2,a,f6.2,a,f6.2,a)'

return
end
