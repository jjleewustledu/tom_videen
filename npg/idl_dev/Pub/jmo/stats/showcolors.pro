;**************************
pro showcolors,title,wd,dsp
;**************************
 
if(dsp.color_scale eq !TRUE) then $
    widget_control,GET_VALUE=scl_index,wd.color_scale_widget
wset,scl_index
minmax = fltarr(2)
minmax(0) = dsp.min
minmax(1) = dsp.max
nval = 10
x = alog10(dsp.max - dsp.min)
pow = fix(x)
mantissa = x - float(pow)
case !TRUE of
    (mantissa lt alog10(2)): inc = .2*10^pow
    (mantissa lt alog10(5)): inc = .5*10^pow
    else: inc = 1.*10^pow
endcase
nval = fix((dsp.max-dsp.min)/inc) + 2
yvals = fltarr(nval)
labels = strarr(nval)
case !TRUE of
    (inc gt 10000): fmt = '(e8.0)'
    (inc gt 1): fmt = '(f6.0)'
    (inc gt .0001): fmt = '(f8.4)'
    else: fmt = '(e8.0)'
endcase
yvals(0) = dsp.min
yvalmin = inc*(fix(dsp.min/inc))
labels(0) = strcompress(string(yvals(0),FORMAT=fmt),/REMOVE_ALL)
yvals(1) = inc*(fix(yvalmin/inc))
labels(1) = strcompress(string(yvalmin,FORMAT=fmt),/REMOVE_ALL)
for i=2,nval-1 do begin
    yvals(i) = yvalmin + inc*(i-1)
    labels(i) = strcompress(string(yvals(i),FORMAT=fmt),/REMOVE_ALL)
endfor
put_color_scale_jmo,!COLSCL_XOFF,!COLSCL_YOFF,minmax,nval,yvals,labels, $
                        ysize=!COLSCL_YSZ,TITLE=" "
wset,dsp.image_index
 
return
end
