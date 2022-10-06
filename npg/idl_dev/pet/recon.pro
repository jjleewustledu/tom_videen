;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   RECON.PRO   **********
;
;	Image reconstruction from projection. (BKFP Method)
;
;	Due to speed, this is more a demo program which explains how
; 	backprojection and filtering work rather then a day to day tool.
;	Direct connection with a backprojection engine may change this view.
;	(Backprojection is slow, Filtering is fast...) 
;	The filter is displayed after each modification according to
;	user's design.
;	The image build-up is also displayed after each backprojection.
;	Due to timing problems (interpreted code), it is strongly 
;	recommended to use an angular rebinning factor from 2 to 8
;	(also called "vue mashing").
;	Zoom adjustement with 2 decimals allows to get integer values
;	for pixel_size in mm/pixel.
;	NB: Care has been taken to ensure quantitative reconstruction.
;
;	Possible developments:
;
;	1) more safety on GUI: lock widgets while status is not ready.
;	2) save images with correct headers (...WECAT expected soon)
;	3) drive reconstruction parameters (x,y) and rotation from the 
;	contour routine	(see make_contour.pro)	
;
;	VERSION 1:	 JULY 30,1992		 Christian MICHEL 
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pro make_filter

common bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2
common bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

distfun = FINDGEN(fft_size)				; ramp	
distfun = distfun < (fft_size- distfun)			; symmetrize around fft_size/2
distfun(0) = 1e-4					; avoid division by 0
x = distfun/(2.*nprojs*cutoff)				; 2.*nprojs*cutoff (=.5) is the max frequency
case filter of
1: filfun = x*(x le 1)					; ideal
2: filfun = x / (1. + x^(2 * order))			; butter
3: filfun = x*(x le 1.)*(0.5+0.5*cos(!pi*x))		; hann
4: filfun = x*(x le 1.)*(0.54+0.46*cos(!pi*x))		; hamm
5: filfun = x*((x le .5)*(1.-6.*x*x*(1-x)) + (x gt .5 and x le 1.)*(2.*(1.-x)^3)) ; parzen
endcase
filfun=filfun*cutoff					; constant slope slope
wset,win_num2
;plot_io,shift(abs(fft(filfun,-1)),fft_size/2) 		; power spectrum
xx = findgen(nprojs) /float(2*nprojs) 
yy=fltarr(nprojs)
for i=0,nprojs-1 do yy(i) = filfun(i)
plot,xx,yy,psym=10,xticks=5,yrange=[0.,0.5]
end

pro bkproj

common bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2
common bkpc4, nf,np,ng,nb,scnfile,scn,f,p,g,b,mats,specs,index,maxindex,lf
common bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

WSET, win_num1		; erase image board
erase
image(*,*)=0.		; reset images
image2(*,*)=0.
zoomf = zoom/100.
pixel_size = sixel_size(index)*float(nprojs)/(float(imgsize)*zoomf)
zf = float(nprojs) / float(imgsize)/ zoomf
;
;pixel_size = sixel_size(index)/zoomf			; at customer's taste definition
;zf = 1./ zoomf

WIDGET_CONTROL, history, SET_VALUE = " Pixel size "+strcompress(string(pixel_size))+" cm",/append

fft_size = 512		; min value
while (fft_size lt 2*nprojs) do fft_size = fft_size * 2
fft_offset = (fft_size - nprojs) / 2
proj = fltarr(fft_size)
;
make_filter		; make 1-D filter
ftypes = ["Ramp","Butterworth", "Hann", "Hamming","Parzen"]
info = ' Filter parameters'+ lf 
info = info + ' Function :'+ ftypes(filter-1) 
if (filter eq 2) then info = info + ' order ' + strcompress(string(order))
info = info + ' cutoff at '+ strcompress(string(cutoff))
WIDGET_CONTROL, history, SET_VALUE = info,/append
;
;	backproject filtered projection
;
rx0 = imgsize/2. - xoff/pixel_size
ry0 = imgsize/2. + yoff/pixel_size
dshift = (256 - imgsize)/2
WIDGET_CONTROL, message, SET_VALUE = "Backprojection starting ... be patient"
WSET, win_num1		; erase image board
dt = !pi/float(nviews)
for j=0,nviews-1 do begin
   for i=0,nprojs-1 do begin
 	proj(i+fft_offset) = scan(i,j)
   end
   ftproj = FFT(proj, -1)
   proj = float(FFT(ftproj * filfun, 1))
   theta = j*dt+ rotoff
   rdx = cos(theta)*zf
   rdy = sin(theta)*zf
   r0 = fft_size/2 - rx0*rdx - ry0*rdy
   info='Now backprojecting view '+ strcompress(string(j)) + ' (from '+ strcompress(string(nviews-1))+')'
   WIDGET_CONTROL, message, SET_VALUE = info
   bkproj_view, proj, r0, rdx, rdy
   tvscl, image, dshift, dshift
end
;
;	Display image
;
print, max(image)
scalef = (!pi /nvorig) * nprojs		; 1st part from bkp, 2d due to fft
image = image * scalef
wset,win_num1
image2 = rebin(image,256,256)
tvscl, image2
WIDGET_CONTROL, message, SET_VALUE = "Ready"

end 
;--------------------------------------------------------------------------
pro bkproj_view, proj, r0, rdx, rdy

common bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2

for y=0,imgsize-1 do begin
  r = r0
  for x=0,imgsize-1 do begin
    j = fix(r) & w = r-j
    image(x,y) = image(x,y) + (1.0-w)*proj(j) + w*proj(j+1)
    r  = r + rdx
  end
r0 = r0 + rdy
end 
end
;-----------------------------------------------------------------------------
pro read_norm

common 	bkpc3, norm,norm_read
common 	bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

	ON_ERROR, 2
	normfile = '/topo6b/mich/data/cti/911/normalsta.nrm'
	stat=recat(normfile,1,1,1,0,0,nrm)
	norm=nrm.data(*,*,0)
	WIDGET_CONTROL, history, SET_VALUE = " 911 Normalization file read ",/append
	norm_read = 1
end
;------------------------------------------------------------------------------------------
pro read_scan, new=new

common	bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common  bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2
common 	bkpc3, norm,norm_read
common 	bkpc4, nf,np,ng,nb,scnfile,scn,f,p,g,b,mats,specs,index,maxindex,lf
common 	bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

	ON_ERROR, 2
	data_dir = '/topo6b/mich/data/cti'
	cd, data_dir, current=curdir
	scnfile = pickfile(GROUP=group,TITLE='Select CTI image',FILTER=['scn'])
	cd, curdir
	if (scnfile eq '') then return
	openr,unit,scnfile,/get_lun
	stat=rmh(unit,mh)
	if(stat eq !ECAT.ERROR) then print, !ECAT.ERROR
	nf = mh.file.nframes
	np = mh.file.nplanes
	ng = mh.file.ngates
	nb = mh.file.nbeds
	f = 0 & p = 0 & g = 0 & b = 0
	if (nf gt 0) then f='*'
	if (np gt 0) then p='*'
	if (ng gt 0) then g='*'
	if (nb gt 0) then b='*'
	expand,mh,f,p,g,b
	f=fix(f) & p=fix(p) & g=fix(g) & b=fix(b)
	mf = n_elements(f)
	mp = n_elements(p)
	mg = n_elements(g)
	mb = n_elements(b)
	ntot  = mf*mp*mg*mb
	maxindex = ntot-1
	mats=lonarr(5,ntot)
	specs=strarr(ntot)
	stat = rdir(unit,dir)
	free_lun,unit
	for i =0, ntot-1 do begin
		mats(*,i) = matval(dir(0,i))
		f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
		specs(i) = strcompress('frame '+ string(f) +' plane '+string(p)+' gate '+string(g)+ $
			' data '+string(d)+' bed '+string(b))
	end
	;print,mats
	f= mats(0,0) & p =mats(1,0) & g= mats(2,0) & d= mats(3,0) & b= mats(4,0)
	if (nf gt 0) then f = '*'
	if (np gt 0) then p = '*'
	WIDGET_CONTROL, message, SET_VALUE = "Reading scan file, please be patient ..."
	stat=recat(scnfile,f,p,g,d,b,scn)
	if(stat eq !ECAT.ERROR) then print, !ECAT.ERROR
	nprojs	= scn.xdim
	nviews	= scn.ydim
	nvorig = nviews
	sixel_size = scn.sh.sample_distance

	info =  lf + ' => New scan loaded ' + lf  $
 		+ ' Scan              : ' + scnfile + lf $
		+ ' Dimensions        : ' + string(nprojs) +' x' + string(nviews) + lf
        WIDGET_CONTROL, history, set_value = info,/append
	IF(NOT KEYWORD_SET(NEW)) THEN BEGIN	; destroy previous boards
		WIDGET_CONTROL, xdraw3, destroy=1
		WIDGET_CONTROL, matlabel, destroy=1
		WIDGET_CONTROL, matlist, destroy=1
	END
	mod_scan = 0
	xdraw3 = WIDGET_BASE(bkpdraw,/column)
	ldraw3 = WIDGET_LABEL(xdraw3, VALUE = " Scan ")
	draw3 = WIDGET_DRAW(xdraw3,/FRAME, RETAIN = 2, XSIZE = nprojs, YSIZE = nviews)
  	matlabel = WIDGET_LABEL(bkpbase, VALUE = "Select matrix or use << and >> buttons")
  	matlist = WIDGET_LIST(bkpbase,VALUE = specs, UVALUE = 'SETMATRIX', YSIZE = 5)
	WSET, win_num1		; erase image board 
	erase
;
end
;--------------------------------------------------------------------------------------------------
pro display_scan

common  bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common 	bkpc4, nf,np,ng,nb,scnfile,scn,f,p,g,b,mats,specs,index,maxindex,lf
common 	bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3,$
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

WIDGET_CONTROL, draw3, GET_VALUE = win_num3
WSET, win_num3
scan=scn.data(*,*,index)>0
TVSCL, scan
msg = ' Displaying scan ...' + lf + ' ' + specs(index) + lf + ' Sixel size ' + strcompress(string(sixel_size(index))) 
WIDGET_CONTROL, history, set_value = msg,/append
WIDGET_CONTROL, message, SET_VALUE = "Ready"
end
;----------------------------------------------------------------------------------------------------
pro recon_event, event

common	bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common  bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2
common 	bkpc3, norm,norm_read
common 	bkpc4, nf,np,ng,nb,scnfile,scn,f,p,g,b,mats,specs,index,maxindex,lf
common 	bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3,draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

ON_ERROR, 2

WIDGET_CONTROL, event.id, GET_UVALUE = eventval

;print,eventval
;help,/str,event
	
CASE eventval OF
   
 'DRAW_WIN_EVENT1': BEGIN			; in zoomed image (256x256)
			WSET, win_num1
			IF event.press EQ 1 THEN BEGIN
			  i= event.x & j = event.y
			  if !order eq 1 then k = 255 - j else k = j
			  position = strcompress(string(i)) + ',' + strcompress(string(j))
			  if (i ge 0 and i le 255 and k ge 0 and k le 255) then begin
			     val = string(image2(i,k))
			  endif else begin
			     val = 'undefined'
			  endelse
			  info = ' Image value at position (' + position + ')' + val  
                          WIDGET_CONTROL, history, set_value = info,/append
			ENDIF
		     END
 'PROJECTION': BEGIN
		maxv = max(scan) 			;Get extrema
		minv = max([1.,maxv/1000.])
		window,/free ,xs=300, ys=300,retain=2,title='Total projection' 
		new_w = !d.window
		wset,new_w
		ptot=fltarr(nprojs)
		for i=0,nprojs-1 do for j=0,nviews-1 do ptot(i) = scan(i,j)
		plot_io,findgen(nprojs),ptot,yrange=[minv,maxv],xstyle=1,psym=10		             
		xyouts,.03,.97,specs(index),/normal
		xyouts,.03,.03,scnfile,/normal
           END

 'PROFILE': BEGIN
		WSET, win_num1
		profiles,image2,order=!order		             
	     	return
             END


'NORMALIZE': BEGIN
		if (norm_read eq 0) then read_norm
		WSET, win_num3
		scan= scan*norm	             
                TVSCL, scan
		info = ' Normalizing 911 scan'            
         	WIDGET_CONTROL, history, set_value = info,/append
		WIDGET_CONTROL, message, SET_VALUE = "Ready"
           END

 'REBIN': BEGIN
		nviews=nviews/2
		mod_scan = 1
		;print,' Before rebinning ',total(scan)
		scan=2.*rebin(scan,nprojs,nviews) 	; since rebin averages !!!
		;print,' After rebinning ',total(scan)
             	info = ' Angle rebinned by a factor 2'           
         	WIDGET_CONTROL, history, set_value = info,/append
		WIDGET_CONTROL, xdraw3, destroy=1
		xdraw3 = WIDGET_BASE(bkpdraw,/column)
		ldraw3 = WIDGET_LABEL(xdraw3, VALUE = " Rebinned scan ")
		draw3 = WIDGET_DRAW(xdraw3,/FRAME, RETAIN = 2, XSIZE = nprojs, YSIZE = nviews)
                TVSCL, scan
          END
 
'RECON': BEGIN
             	info = ' Starting reconstruction, be patient ...'           
         	WIDGET_CONTROL, history, set_value = info,/append
		bkproj
          END

'RAMP': begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 1
	make_filter
	end

'BUTTER':begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 1
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 1
 	filter = 2
	make_filter
	end

'HANN': begin
      	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 3
	make_filter
	end

'HAMM':begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
      	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 4
	make_filter
	end

'PARZEN': begin
     	WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
     	WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
	filter = 5
	make_filter
	end

'ORDER': begin
	  widget_control, ordersl, get_value = temp
	  if(temp ne order) then begin
	    order = temp
	  endif
  	WIDGET_CONTROL, orderlabel, SET_VALUE = "Order " + strcompress(STRING(order))
	make_filter
  	end

'CUTOFF':BEGIN
	widget_control, cutoffsl, get_value = temp
 	cutoff = temp/20. * 0.5
  	WIDGET_CONTROL, cutofflabel, SET_VALUE = "Cutoff at " + strcompress(STRING(cutoff))
	make_filter
	END
 
'TRANSLATION':BEGIN
	transl= [xoff,yoff]
	xvaredit,transl,name='Translation'
	xoff=transl(0) & yoff=transl(1)
  	WIDGET_CONTROL, history, SET_VALUE = "Translation " + strcompress(STRING(xoff))+ strcompress(string(yoff)),/append
	END

'ROTATION':BEGIN
	angle=rotoff*180./!pi
	xvaredit,angle,name='Rotation'
	rotoff= angle*!pi/180.
  	WIDGET_CONTROL, history, SET_VALUE = "Rotation " + strcompress(STRING(angle)),/append
	END

'ZOOM': begin
	  widget_control, zoomsl, get_value = temp
	  if(temp ne zoom) then begin
	    zoom = temp
	  endif
        zoomf = zoom/100.
  	WIDGET_CONTROL, zoomlabel, SET_VALUE = "Zoom " + strcompress(STRING(zoomf))
	pixel_size = sixel_size(index)*float(nprojs)/(float(imgsize)*zoomf)
	WIDGET_CONTROL, message, SET_VALUE = "Pixel size "+strcompress(string(pixel_size))+" cm"
  	end

'CLUT': BEGIN
		xloadct, GROUP = event.top
        END

'LOADSCAN': BEGIN
		read_scan
		index = 0
		display_scan
		widget_control, next,/sensitive		; more than 1 matrix 
	  	if(index eq maxindex) then widget_control, next,sensitive=0
		widget_control, previous,sensitive=0
	   end

'NEXT': begin
	  index=index+1
	  if (mod_scan eq 1 ) then begin
		mod_scan = 0
		nviews = nvorig
		WIDGET_CONTROL, xdraw3, destroy=1
		xdraw3 = WIDGET_BASE(bkpdraw,/column)
		ldraw3 = WIDGET_LABEL(xdraw3, VALUE = " Rebinned scan ")
		draw3 = WIDGET_DRAW(xdraw3,/FRAME, RETAIN = 2, XSIZE = nprojs, YSIZE = nviews)
	  end
	  display_scan
	  if(index eq maxindex) then widget_control, next,sensitive=0
	  if(index eq 1) then widget_control, previous,/sensitive
 	  WSET, win_num1		; erase image board 
	  erase
	end

'PREVIOUS': begin
	  index=index-1
	  if (mod_scan eq 1 ) then begin
		mod_scan = 0
		nviews = nvorig
		WIDGET_CONTROL, xdraw3, destroy=1
		xdraw3 = WIDGET_BASE(bkpdraw,/column)
		ldraw3 = WIDGET_LABEL(xdraw3, VALUE = " Rebinned scan ")
		draw3 = WIDGET_DRAW(xdraw3,/FRAME, RETAIN = 2, XSIZE = nprojs, YSIZE = nviews)
	  end
	  display_scan
	  if(index eq maxindex-1) then widget_control, next,/sensitive
	  if(index eq 0) then widget_control, previous,sensitive=0
 	  WSET, win_num1		; erase image board 
	  erase
	end

'SETMATRIX': begin
	  previndex = index
	  index=event.index
	  if (index eq previndex) then return
	  if (mod_scan eq 1 ) then begin
		mod_scan = 0
		nviews = nvorig
		WIDGET_CONTROL, xdraw3, destroy=1
		xdraw3 = WIDGET_BASE(bkpdraw,/column)
		ldraw2 = WIDGET_LABEL(xdraw3, VALUE = " Rebinned scan ")
		draw3 = WIDGET_DRAW(xdraw3,/FRAME, RETAIN = 2, XSIZE = nprojs, YSIZE = nviews)
	  end
	  display_scan
	  widget_control, next,sensitive=1
	  widget_control, previous,sensitive=1
	  if(index eq maxindex) then widget_control, next,sensitive=0
	  if(index eq 0) then widget_control, previous,sensitive=0
 	  WSET, win_num1		; erase image board 
	  erase
	end

'DONE' : WIDGET_CONTROL, event.top, /DESTROY

ENDCASE

END

PRO recon, GROUP=GROUP

common	bkpc1, scan,nviews,nprojs,sixel_size,nvorig
common  bkpc2, image,imgsize,xoff,yoff,rotoff,zoom,filter,cutoff,order,filfun,fft_size, $
		pixel_size,image2
common 	bkpc3, norm,norm_read
common 	bkpc4, nf,np,ng,nb,scnfile,scn,f,p,g,b,mats,specs,index,maxindex,lf
common 	bkpc5, bkpbase,bkpdraw,bkpars,history,xdraw1,xdraw2,xdraw3, draw1,draw2,draw3, $
		message,matlist,matlabel,win_num1,win_num2,win_num3,mod_scan, $
		previous, next,ordersl,orderlabel,cutoffsl,cutofflabel,zoomsl,zoomlabel

ON_ERROR, 2

lf = "\x0a"

imgsize=128
fft_size = 512				; min value
image=fltarr(imgsize,imgsize)
image2=fltarr(2*imgsize,2*imgsize)

xoff = 0. & yoff = 0. & rotoff = 0. & filter = 3 & order = 2

bkpbase = WIDGET_BASE(TITLE = 'Reconstruction',/COLUMN)

control1 = WIDGET_BASE(bkpbase,/ROW)
loadscn = WIDGET_BUTTON(control1, VALUE = "Load Scan", UVALUE = 'LOADSCAN')
previous = WIDGET_BUTTON(control1, VALUE = "<<", UVALUE = 'PREVIOUS')
next = WIDGET_BUTTON(control1, VALUE = ">>", UVALUE = 'NEXT')
normal = WIDGET_BUTTON(control1, VALUE = "Normalize", UVALUE = 'NORMALIZE')
rebin = WIDGET_BUTTON(control1, VALUE = "Rebin Angle", UVALUE = 'REBIN')
proj = WIDGET_BUTTON(control1, VALUE = "Projection", UVALUE = 'PROJECTION')
clut = WIDGET_BUTTON(control1, VALUE = "CLUT", UVALUE = 'CLUT')
exit = WIDGET_BUTTON(control1, VALUE = "Exit", UVALUE = 'DONE')

control2 = WIDGET_BASE(bkpbase,/ROW)
recon = WIDGET_BUTTON(control2, VALUE = "Reconstruct", UVALUE = 'RECON')
profil = WIDGET_BUTTON(control2, VALUE = "Image profiles", UVALUE = 'PROFILE')

bkpdraw = WIDGET_BASE(bkpbase,/ROW)
xdraw1 = WIDGET_BASE(bkpdraw,/column)
ldraw1 = WIDGET_LABEL(xdraw1, VALUE = " Image ")
draw1 = WIDGET_DRAW(xdraw1,/button_events, /FRAME, UVALUE = 'DRAW_WIN_EVENT1', $
	RETAIN = 2, XSIZE = 256, YSIZE = 256)

bkpars = WIDGET_BASE(xdraw1,/ROW)
translation = WIDGET_BUTTON(bkpars, VALUE = " Translation (x,y) ", UVALUE = 'TRANSLATION')
rotate = WIDGET_BUTTON(bkpars, VALUE = " Rotation ", UVALUE = 'ROTATION')
zoombase=WIDGET_BASE(xdraw1,/column)
zoomsl = WIDGET_SLIDER(zoombase,VALUE=150,MINIMUM=1,MAXIMUM=1000,UVALUE = 'ZOOM',/SUPPRESS_VALUE)
zoom = 150.
zoomlabel = WIDGET_LABEL(zoombase, VALUE = "Zoom " + strcompress(STRING(zoom/100.)))

xdraw2 = WIDGET_BASE(bkpdraw,/column)
ldraw2 = WIDGET_LABEL(xdraw2, VALUE = " Filter ")
draw2 = WIDGET_DRAW(xdraw2, /FRAME, RETAIN = 2, XSIZE = 256, YSIZE = 256)

ftypes = ["Ramp","Butter", "Hann", "Hamming","Parzen"]
fuser = ["RAMP","BUTTER", "HANN", "HAMM","PARZEN"]
XMenu, ftypes, xdraw2, UVALUE=fuser,/EXCLUSIVE, /ROW, BUTTONS = filterset, TITLE = "Filter"
cutoffsl = WIDGET_SLIDER(xdraw2,XSIZE=40,VALUE=20,MINIMUM=1,MAXIMUM=20, $
		UVALUE = 'CUTOFF',/SUPPRESS_VALUE)
cutoff = 0.5 
cutofflabel = WIDGET_LABEL(xdraw2, VALUE = "Cutoff at " + strcompress(STRING(cutoff)))
ordersl = WIDGET_SLIDER(xdraw2,XSIZE=40,VALUE=2,MINIMUM=1,MAXIMUM=10,UVALUE = 'ORDER',/SUPPRESS_VALUE)
orderlabel = WIDGET_LABEL(xdraw2, VALUE = "Order " + strcompress(string(order)))


historylabel = WIDGET_LABEL(bkpbase, VALUE = " History ")
history= WIDGET_TEXT(bkpbase, VALUE = 'History ....', xsize=70,YSIZE = 8,/scroll,/FRAME)
status = WIDGET_BASE(bkpbase, /ROW, /FRAME, XPAD = 10, SPACE = 30)
statuslabel = WIDGET_LABEL(status, VALUE = " Status ")
message = WIDGET_LABEL(status, VALUE = "Image reconstruction ... ")

WIDGET_CONTROL, bkpbase, /REALIZE

WIDGET_CONTROL, draw1, GET_VALUE = win_num1
WIDGET_CONTROL, draw2, GET_VALUE = win_num2
WIDGET_CONTROL, ORDERSL, SENSITIVE = 0
WIDGET_CONTROL, ORDERlabel, SENSITIVE = 0
WIDGET_CONTROL, filterset(2), /SET_BUTTON		; Hann

print,' Reading first scan, please be patient ...'
read_scan,/new
if( scnfile eq '') then return

norm_read = 0
index=0
display_scan
make_filter
widget_control, next,/sensitive		; more than 1 matrix 
if(index eq maxindex) then widget_control, next,sensitive=0
widget_control, previous,sensitive=0

loadct,14	; UCLA

XMANAGER, "recon", bkpbase, GROUP_LEADER=GROUP

END
