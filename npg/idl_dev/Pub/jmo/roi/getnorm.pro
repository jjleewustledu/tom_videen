function getnorm,filnam,maxring

norms = fltarr(maxring)
print,filnam,maxring
for i=1,maxring do begin
    plane = plnnum_f3d(i,i)
    sh = rd_shdr(filnam,plane,1,0)
    norms(i-1) = sh.ecat_calib_factor
print,i,plane,sh.ecat_calib_factor,sh.scale_factor
endfor
print,norms

return,norms
end
    
