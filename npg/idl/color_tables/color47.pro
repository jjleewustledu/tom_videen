; Color47

r=intarr(256)
g=intarr(256)
b=intarr(256)

rf = 7./15.
gf = 7./15.
bf = 15./15.

FOR n=1,255 DO BEGIN $
	& r[n] = rf*n*1.6 < 255 $
	& g[n] = gf*n*1.6 < 255 $
	& b[n] = bf*n*2.0 < 255 

modifyct,47,"Lavender",r,g,b,file="/usr/local/npg/idl/color_tables/videen.tbl"

