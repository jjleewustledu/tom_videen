;***********************************
pro rect_plot,x1,x2,y1,y2,COLOR=colr
;***********************************
 
if keyword_set(color) then $
    color = 175 $
else $
    color = colr
 
x = intarr(5)
y = intarr(5)
 
x(0) = x1
y(0) = y1
x(1) = x1
y(1) = y2
x(2) = x2
y(2) = y2
x(3) = x2
y(3) = y1
x(4) = x1
y(4) = y1

plots,x,y,/DEV,/NOCLIP,COLOR=color
 
return
end
