function convert_matrix2,matrix,forward = forward,dims = dims,quiet = quiet
;this program will take Dave's magnet transform
;and output in the form of my magnet transform, or vice-versa
;--------------------------------------------------
;Dave's origin is in upside down space, ie = tlhc
;my origin is 0,0,0 (or blhc)
;forward flag means from dave to me
;J. Maldjian
;--------------------------------------------------
tmatrix = fix(10*matrix)
forward = 0
if tmatrix(3) eq 0 and tmatrix(7) eq 0 then forward = 1
ysize = dims(1)
inverty = fltarr(4,4)
for i = 0,3 do inverty(i,i) = 1
inverty(1,1) = -1
if forward then begin
	mtout = inverty # transpose(matrix)
	blhc = transpose([0, -(ysize - 1), 0,1]) # mtout
	mtout(3, 0:2) = blhc(0:2)
endif
if not(forward) then begin
	mtout = matrix
	tlhc = transpose([0, (ysize - 1), 0,1]) # mtout
	mtout(3, 0:2) = tlhc(0:2)
	mtout = transpose(mtout) # inverty
endif

return, mtout

end


function coords2mt, coords = coords,dims = dims
;------------------------------------------
;Program return the magnet_transform
;given the coords matrix and image dims
;------------------------------------------

xd=dims(0)
yd=dims(1)
zd=dims(2)
md=xd>yd

xv=(coords(*,1,0)-coords(*,0,0))  / md
yv=(coords(*,2,0)-coords(*,1,0) ) / md
if zd gt 1 then zv=(coords(*,0,1)-coords(*,0,0)) else zv = [0, 0, 0]
orig=coords(*,0,0) +(0.5+(md-xd)/2.)*xv+(0.5+(md-yd)/2.)*yv
mt= [  xv, 0, yv, 0, zv, 0, orig, 1]
mt=reform(mt,4,4)
mtt = convert_matrix2(mt,dims = dims)
return,mtt
end







