; Color46
; Bronson-like
cp videen0.tbl videen1.tbl
y
idl

r=intarr(256)
g=intarr(256)
b=intarr(256)

k = 0
FOR i=0,24 DO r[k+i]=0
FOR i=0,24 DO g[k+i]=0
FOR i=0,24 DO b[k+i]=0

k = 25
FOR i=0,24 DO r[k+i]=100
FOR i=0,24 DO g[k+i]=100
FOR i=0,24 DO b[k+i]=100

k = 50
FOR i=0,24 DO r[k+i]=150
FOR i=0,24 DO g[k+i]=50
FOR i=0,24 DO b[k+i]=150

k = 75
FOR i=0,24 DO r[k+i]=0
FOR i=0,24 DO g[k+i]=180
FOR i=0,24 DO b[k+i]=180

k = 100
FOR i=0,24 DO r[k+i]=0
FOR i=0,24 DO g[k+i]=0
FOR i=0,24 DO b[k+i]=255

;green
k = 125
FOR i=0,24 DO r[k+i]=0
FOR i=0,24 DO g[k+i]=255
FOR i=0,24 DO b[k+i]=0

; red
k = 150
FOR i=0,24 DO r[k+i]=255
FOR i=0,24 DO g[k+i]=0
FOR i=0,24 DO b[k+i]=0

; orange (contact 1)
k = 175
FOR i=0,24 DO r[k+i]=255
FOR i=0,24 DO g[k+i]=150
FOR i=0,24 DO b[k+i]=30

; blue/green (contact 0)
k = 200
FOR i=0,11 DO r[k+i]=0
FOR i=0,11 DO g[k+i]=255
FOR i=0,11 DO b[k+i]=255

; magenta (contact 3)
k = 212
FOR i=0,12 DO r[k+i]=255
FOR i=0,12 DO g[k+i]=0
FOR i=0,12 DO b[k+i]=255

; yellow (contact 2)
k = 225
FOR i=0,24 DO r[k+i]=255
FOR i=0,24 DO g[k+i]=255
FOR i=0,24 DO b[k+i]=0

; white
k = 250
FOR i=0,5 DO r[k+i]=255
FOR i=0,5 DO g[k+i]=255
FOR i=0,5 DO b[k+i]=255

modifyct,46,"Hues",r,g,b,file="/home/npggw/tom/idl/npg/color_tables/videen1.tbl"

exit
cp videen1.tbl /usr/local/npg/idl/color_tables/videen.tbl
y

