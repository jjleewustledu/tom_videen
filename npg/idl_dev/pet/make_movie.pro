pro make_movie,lth,hth,x_angle,peel=peel
;
; Check arguments
;
if (n_elements(lth) eq 0) then lth = 0 		; image volume low threshold
if (n_elements(hth) eq 0) then hth = 100	; image volume high threshold in %
if (n_elements(x_angle) eq 0) then x_angle = 20 ; x view angle
if(keyword_set(peel)) then lpeel=1 else lpeel = 0
lth = (lth/100.)*256
hth = (hth/100.)*256
print,lth,hth
;
; Select data file and read it
;
data_dir = '/topo6b/mich/data/'
cd, data_dir, current=curdir
file = pickfile()
cd, curdir
;
image=recat(file,header,1,1,1,0,0,/nodata)
n_frames= header.ecat.nframes
n_planes= header.ecat.nplanes
dim_x=header.ecat.xd
dim_y=header.ecat.yd
plane_sep=header.ecat.planesep
sampling_dist=header.ecat.sampdist
slice_width=header.ecat.slicew
pixel_size=header.ecat.pix
zr = 0
if plane_sep ne 0 then zr = pixel_size / plane_sep
if zr eq 0 then zr = 0.5
;
Print," File              ",file
print," Number of frames  ",n_frames
print," Image size        ",dim_x
print," Number of planes  ",n_planes
print," Plane separation  ",plane_sep
;print," Sampling distance ",sampling_dist
print," Pixel_size        ",pixel_size
print," z ratio           ",zr
;
sth=50
answ='y'
if (n_planes gt 1) then begin
	image = bytarr(dim_x,dim_x,n_planes)
	for i=1,n_planes do begin
		print,"reading plane ....",i 
		tmp = bytscl(recat(file,header,1,i,1,0,0))<hth>lth - lth
 		if(lpeel) then begin
			roidef:
			tmp2 = 0 & tmp3 = 0 & cutout = 0
			tmp2 = rebin(tmp,512,512)
			tmp3 = bytarr(512,512)
			tvscl,tmp2
			cutout =defroi(512,512)
			tmp3(cutout) = tmp2(cutout)
			tvscl, tmp3
			read,' Do you agree with this contour (y/n)?',answ
			if(answ eq 'n') then goto, roidef
			profiles,tmp3
			xvaredit, sth
			tmp3 = bytscl(tmp3>sth -sth)
			tmp3= (256-tmp3)*(tmp3 gt 0)
			tvscl, tmp3
			read,' Do you agree with this threshold (y/n)?',answ
			if(answ eq 'n') then goto, roidef
			image(*,*,i-1) =rebin(tmp3,dim_x,dim_x)
		endif else image(*,*,i-1) =tmp
	endfor
end
if (n_frames gt 1) then begin
	image = bytarr(dim_x,dim_x,n_frames)
	for i=1,n_frames do begin
		print,"reading frame ....",i 
		tmp = bytscl(recat(file,header,i,1,1,0,0))<hth>lth - lth
 		if(lpeel) then begin
			roidef2:
			tmp2 = 0 & tmp3 = 0 & cutout = 0
			tmp2 = rebin(tmp,512,512)
			tmp3 = bytarr(512,512)
			tvscl,tmp2
			cutout =defroi(512,512)
			tmp3(cutout) = tmp2(cutout)
			tvscl, tmp3
			read,' Do you agree with this contour (y/n)?',answ
			if(answ eq 'n') then goto, roidef2
			profiles,tmp3
			xvaredit, sth
			tmp3 = bytscl(tmp3>sth -sth)
			tmp3= (256-tmp3)*(tmp3 gt 0)
			tvscl, tmp3
			read,' Do you agree with this threshold (y/n)?',answ
			if(answ eq 'n') then goto, roidef2
			image(*,*,i-1) =rebin(tmp3,dim_x,dim_x)
		endif else image(*,*,i-1) = tmp
	endfor
end
s=size(image)
dims = s(1:3)
d = [ 0., dims(0), 0., dims(1), 0., dims(2)]
IF zr gt 1. THEN BEGIN
	x = (zr-1)/2.
	d(0) = [-x * dims(0), (x+1) * dims(0), -x * dims(1), (x+1) * dims(1)]
ENDIF ELSE BEGIN
	x = (1-zr)/2
	d(4) = [-x * dims(2), (x+1) * dims(2)]
ENDELSE
;
iso = 10
print," Now shading ..."
shade_volume,image,iso,v,p,low=1
print,strtrim((size(v))(2),2)+' Vertices, ' + strtrim((size(p))(1)/4,2) + ' Polygons.'
;
; Creating and saving movie
;
loadct,15
strput,file,'mov',strpos(file,'img') 
openw, 2 , file
numframes=36
xinteranimate,set=[256,256,numframes],title=file,/showload
for f=0,numframes-1 do begin
 z_angle = f*10 
 scale3, xrange=d(0:1), yrange=d(2:3), zrange=d(4:5), ax = x_angle, az = z_angle
 image=polyshade(v,p,/t3d)
 writeu,2,image
 tv,image
 xinteranimate,frame=f,image=image,window=!d.window
endfor
close,2
;
xinteranimate
end


 
