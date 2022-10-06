; Color45
; gray-scale with rapid change in middle

r=intarr(256)
g=intarr(256)
b=intarr(256)

FOR i=0,79 DO r[i]=i/4
FOR i=0,95 DO r[80+i]=20+i*2.2
print,80+i,20+i*2.2

FOR i=0,79 DO r[176+i]=235+i/4
g[0:255]=r[0:255]
b[0:255]=r[0:255]

modifyct,45,"Gray S",r,g,b,file="/home/npggw/tom/idl/npg/color_tables/videen1.tbl"
