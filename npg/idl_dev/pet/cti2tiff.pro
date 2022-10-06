pro cti2tiff
cd, '/topo6b/mich/data/cti/nmr', current=curdir
file = pickfile()
cd, curdir
image = rebin(bytscl(RECAT(file,header,1,13,1,0,0))>10<100,512,512)
tvscl,image
;
; modify color scale before writing tiff  image
;
common colors,r_orig,g_orig,b_orig,red,green,blue
xloadct

tiff_write,'image.tiff',image,red=red,green=green,blue=blue
end
