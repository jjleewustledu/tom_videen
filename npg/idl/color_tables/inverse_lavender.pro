; Color48

r=intarr(256)
g=intarr(256)
b=intarr(256)

rf = 1.
gf = 1.
bf = 2.5

r[0] = 255 
g[0] = 255
b[0] = 255

FOR n=1,255 DO BEGIN $
	& r[n] = (rf*(255-n) < 255) > 2 $
	& g[n] = (gf*(255-n) < 255) > 2 $
	& b[n] = (bf*(255-n) < 255) > 2 

modifyct,48,"Inverse Lavender",r,g,b,file="/usr/local/npg/idl/color_tables/videen3.tbl"

