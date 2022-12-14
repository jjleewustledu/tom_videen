;*****************************
function box_smoth,image,width
;*****************************

; Smooth image with a boxcar kernel.
; Specify width in voxels.

if(width le 1.) then begin
    smooth_image = image
    return,smooth_image
endif

tails = fix((width+.99999)/2)
kdim = 2*tails + 1
 
f = fltarr(kdim)
f(0) = 1.
x = .5
w = width/2.
for i=1,tails do begin
    if(x le w-1) then $
        f(i) = 1. $
    else $
        f(i) = w - x
    x = x + 1.
end

info = size(image)
xdim = info(1)
ydim = info(2)
if(info(0) eq 3) then begin
    krnl = fltarr(kdim,kdim,kdim)
    zdim = info(3)
    for i=0,kdim-1 do begin
        ii = abs(i - kdim/2)
        for j=0,kdim-1 do begin
            jj = abs(j - kdim/2)
            for k=0,kdim-1 do begin
                kk = abs(k - kdim/2)
                krnl(i,j,k) = f(ii)*f(jj)*f(kk)
	    end
        end
    end
    krnl(*,*,*) = krnl(*,*,*)/total(krnl)
endif else begin
    zdim = 0
    krnl = fltarr(kdim,kdim)
    for i=0,kdim-1 do begin
        ii = abs(i - kdim/2)
        for j=0,kdim-1 do begin
            jj = abs(j - kdim/2)
            krnl(i,j) = f(ii)*f(jj)
        end
    end
    krnl(*,*) = krnl(*,*)/total(krnl)
endelse

smooth_image = convol(image,krnl,/CENTER,/EDGE_TRUNCATE)

return,smooth_image
end
