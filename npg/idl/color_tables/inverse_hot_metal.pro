; Color49

r=intarr(256)
g=intarr(256)
b=intarr(256)

r[0:255] = 255 
g[0] = 255
b[0] = 255

rr = 255
gg = 255
bb = 255

FOR n=1,150 DO BEGIN $
	& r[n] = rr $
	& g[n] = gg $
	& b[n] = bb $
	& rr = rr > 0 $
	& gg = gg-1 > 0 $
	& bb = bb-3 > 0 

FOR n=151,255 DO BEGIN $
	& r[n] = rr $
	& g[n] = gg $
	& b[n] = bb $
	& rr = rr-1 > 0 $
	& gg = gg-1 > 0 $
	& bb = bb-1 > 0


modifyct,49,"Inverse Hot Metal",r,g,b,file="/usr/local/npg/idl/color_tables/videen3.tbl"

