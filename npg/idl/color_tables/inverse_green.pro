; Color50

r=intarr(256)
g=intarr(256)
b=intarr(256)

g[0:255] = 255 
r[0] = 255
b[0] = 255

rr = 255
gg = 255
bb = 255

FOR n=1,50 DO BEGIN $
	& r[n] = rr $
	& g[n] = gg $
	& b[n] = bb $
	& gg = gg > 0 $
	& rr = rr-2 > 0 $
	& bb = bb-2 > 0 

FOR n=51,255 DO BEGIN $
	& r[n] = rr $
	& g[n] = gg $
	& b[n] = bb $
	& gg = gg-1 > 0 $
	& rr = rr-1 > 0 $
	& bb = bb-1 > 0


modifyct,49,"Inverse Green",r,g,b,file="/usr/local/npg/idl/color_tables/videen3.tbl"

