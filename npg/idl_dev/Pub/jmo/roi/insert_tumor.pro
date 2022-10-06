;******************************
pro insert_tumor,image,xd,yd,zd
;******************************

value = fltarr(2)
label = strarr(2)
value(0) = 1
value(1) = .2
label(0) = string("Diameter of tumor in cm (1 or 2): ")
label(1) = string("Relative intensity of tumor: ")
value = get_str(2,label,value)
diameter = fix(value(0))
intensity = float(value(1))

if(diameter eq 1) then begin
    image(xd-1:xd+2,yd:yd+1,  zd) = intensity
    image(xd:xd+1  ,yd-1:yd+2,zd) = intensity
    image(xd:xd+1  ,yd:yd+1,zd-1:zd+1) = intensity
endif else begin
    image(xd-3:xd+4,yd-1:yd+1, zd:zd+1) = intensity
    image(  xd:xd+1,yd-3:yd+4, zd:zd+1) = intensity
    image(xd-1:xd+2,yd-3:yd+3, zd:zd+1) = intensity
    image(xd-2:xd+3,yd-2:yd+2, zd:zd+1) = intensity

    image(xd-1:xd+2,yd:yd+1  ,zd+2) = intensity
    image(xd:xd+1  ,yd-1:yd+2,zd+2) = intensity

    image(xd-1:xd+2,yd:yd+1  ,zd-1) = intensity
    image(xd:xd+1  ,yd-1:yd+2,zd-1) = intensity

    image(xd:xd+1  ,yd:yd+1  ,zd+3) = intensity
    image(xd:xd+1  ,yd:yd+1  ,zd-2) = intensity
endelse

return
end
     
