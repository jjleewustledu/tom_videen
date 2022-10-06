;************************************************************
function read_t88,filnam,xdim,ydim,zdim,tdim,dxdy,dz
;************************************************************

plane = 1
frame = 1
gate = 1
data = 0
bed = 0
stat = recat6(filnam,frame,plane,gate,data,bed,dt,/NO_SCALE,/NO_DATA,/ZOOM)
if (stat EQ !ECAT.ERROR) then begin
  print, "Error reading ",filnam," frame ",frame
  stat = -1
  return,0
endif
xdim = dt.sh.dimension_1
ydim = dt.sh.dimension_2
zdim = dt.mh.file.nplanes
tdim = 1
dxdy = dt.sh.pixel_size
dz   = dt.sh.slice_width
imgout = fltarr(xdim,ydim,zdim)
for plane=1,zdim do begin
    stat = recat6(filnam, frame, plane, gate, 0, 0, dt, ZOOM=1)
    if (stat EQ !ECAT.ERROR) then begin
        print, "Error reading plane",plane," of ",filnam
        stat = -1
        return,0
    endif
    imgout(*,*,plane-1) = dt.data
endfor

return,imgout
end
