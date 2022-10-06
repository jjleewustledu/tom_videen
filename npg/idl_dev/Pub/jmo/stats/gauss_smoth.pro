;******************************
function gauss_smoth,image,fwhm
;******************************

; Smooth image with a Gaussian kernel.
; Specify fwhm in voxels.

if(fwhm le 0.) then begin
    smooth_image = image
    return,smooth_image
endif

sigma = fwhm/2.35
kdim = 2*round(3*sigma) + 1
 
f = fltarr(kdim)
krnl = fltarr(kdim,kdim)
fm1 = gauss_pdf(0.5/sigma)
f(0) = 2*(fm1 - 0.5)
x = 1.5
for i=1,kdim/2 do begin
    f0= gauss_pdf(x/sigma)
    f(i) = f0 - fm1
    fm1 = f0
    x = x + 1.
end
for i=0,kdim-1 do begin
    for j=0,kdim-1 do begin
        ii = abs(i - kdim/2)
        jj = abs(j - kdim/2)
        krnl(i,j) = f(ii)*f(jj)
    end
end
krnl(*,*) = krnl(*,*)/total(krnl)

info = size(image)
xdim = info(1)
ydim = info(2)
if(info(0) eq 3) then $
    zdim = info(3) $
else $
    zdim = 0

if(zdim eq 0) then begin
     smooth_image = convol(image,krnl,/CENTER,/EDGE_TRUNCATE)
endif else begin
     smooth_image = fltarr(xdim,ydim,zdim)
     for z=0,zdim-1 do begin
	img = convol(image(*,*,z),krnl,/CENTER,/EDGE_TRUNCATE)
	smooth_image(*,*,z) = img
     endfor
endelse

return,smooth_image
end
