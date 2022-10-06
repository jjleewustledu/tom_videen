function siemens_mosaic,filename = filename,maxdim = maxdim,image = image,acquisition_matrix = acquisition_matrix,use_acq = use_acq
;----------------------------------------------------------------------------------------------------------------------
;Program will break a mosaic image into a 3D volume
;volume = siemens_mosaic(image = image,maxdim = maxdim)
;where image is the 2D mosaic image, and maxdim is the maximum dim in x or y for an individual image
;ie for an acquisition of 40x64, maxdim would be 64
;Since I only had 1 mosaic to work with, my assumptions are based on that file:
;Mosaic is square, with equal number of images across and down
;Images are resampled with Matrix sizes squared off, even if acquired assymetrically
;If you want to try to return the asymetric counterpart, set acquisition_matrix = the true acquisition matrix [40,64]
;and set use_acq = 1  (but note that the siemens mosaic appears to be resampled to square, so this is not adviseable)
;J. Maldjian 5-8-03
;---------------------------------------------------------------------------------------------------------------------


;if keyword_set(filename) then begin
;	ih = read_dicomheader(filename = filename,image = image,dh = dh,rh = rh,coords = coords)
;	acquisition_matrix = fix(strsplit(dh.acquisition_matrix.value,/extract))
;	xdim = xy(0)
;	ydim = xy(1)
;	maxdim = max(xy)
;endif
if not(keyword_set(acquisition_matrix)) then acquisition_matrix = [maxdim,maxdim]
maxdim = max(acquisition_matrix)
sz = size(image)
width = sz(1)
height = sz(2)
nslices = ((1.0 *sz(1)) * sz(2) ) / (maxdim * maxdim)
xend = -1
ystart = 0
;---------------------------------------------------------------------------------------
;even though the siemens images may have been acquired at 40x64
;they appear to be resampled in the mosaic to 64x64 (image in x larger than expected)
;----------------------------------------------------------------------------------------
xdim = maxdim
ydim = maxdim
if keyword_set(use_acq) then begin
	xdim = acquisition_matrix(0)
	ydim = acquisition_matrix(1)
endif
imstartx = (maxdim - xdim)/2
imstarty = (maxdim - ydim)/2
volume = intarr(xdim,ydim,nslices)
for i = 0,nslices - 1 do begin
	xstart = xend + 1
	xend = xstart + maxdim - 1
	if xstart ge width then begin
		xstart = 0
		ystart = yend + 1
	endif
	yend = ystart + maxdim - 1		 	
	xend = xstart + maxdim - 1
	next_image = image(xstart:xend,ystart:yend)
	volume(*,*,i) = next_image(imstartx:imstartx + xdim - 1, imstarty:imstarty + ydim - 1) 
endfor
return,volume
end
