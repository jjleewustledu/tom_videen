function get_imdicom_info,filename = filename
;-----------------------------------------------
;This is the imdicom version of get_iminfo
;-----------------------------------------------

gethdrinfo56,   nslices, nimages, xd, yd, nex, rh, filename,vrgfxres = vrgfxres
axoffs = 0
rh = read_rawheader(filename = filename,offset = gh_offset)
openr,/swap_if_little_endian,unit,filename,/get_lun
a=assoc(unit,intarr(1),gh_offset)
tmparr=a(0)
nlocs=tmparr(0)
a=assoc(unit,fltarr(3,3,nlocs),gh_offset + 2)
coord=a(0)
close_Lun,unit
rasmatrix = get_ras_pfile(filename = filename,coord = coord,locs = bytarr(xd,yd,nslices))
reply = {coord:coord,xd:xd,yd:yd,zdim:nslices,tdim:nimages,axoffs:axoffs,rasmatrix:rasmatrix}
return,reply
end
