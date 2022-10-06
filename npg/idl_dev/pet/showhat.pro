;------------------------------------------------------------------------------
;	procedure showhat_draw
;------------------------------------------------------------------------------

PRO showhat_draw, size=size

common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  comco, 	contour,ncpts,head_axis

common shc1,	xrot, zrot, oaxes, caxes, symmetry, sh_image, win_num, $
 		xmin,ymin,zmin,xmax,ymax,zmax, $
		colnum, thick, psym, exchxy, img_thr

common shc2,	Xtransp,oaxeson,oaxesoff,caxeson,caxesoff,symmetryon,symmetryoff,imageon,imageoff, $
		imagetra,black,red,green,blue,yellow,violet,lblue,white, $
		t1,t2,t3,t4,t5,t6,t7,t8,t9, nameid, message, zmess,xmess

common shc3,	vue

WSET, win_num
colsym = colnum + 1 
if (colsym eq 7) then colsym = 0

set_plot,'z'
if(keyword_set(size)) then device,set_resolution=[size,size]
erase
scale3, xrange=[xmin,xmax],yrange=[ymin,ymax],zrange=[zmin,zmax],ax=xrot,az=zrot
	
if(exchxy eq -1) then t3d,/xyexch

if (symmetry eq 1) then plots,head_axis,/t3d,col= colsym,thick=2

z_norm= !z.s(0)+!z.s(1)*head_axis(2)
plot,contour(0,*),contour(1,*),xrange=[xmin,xmax],yrange=[ymin,ymax],zrange=[zmin,zmax],$
		xstyle=5,ystyle=5,zstyle=5,zvalue=z_norm,/t3d, col=colnum

; x.window needs to be specified for axes ... the trick ...

zd=[zmin,zmax]#[1,1]
if(oaxes eq 1) then begin 
	 surface,zd,xrange=[xmin,xmax],yrange=[ymin,ymax],zrange=[zmin,zmax], $
	 /t3d,/nodata,/noerase,xstyle=1,ystyle=1,zstyle=1, $	
	 xtitle='X',ytitle='Y',ztitle='Z',col=7

endif else begin
	surface,zd,xrange=[xmin,xmax],yrange=[ymin,ymax], zrange=[zmin,zmax],/t3d,/nodata,/noerase,xstyle=4,ystyle=4,zstyle=4
endelse

if(caxes eq 1) then begin
  axis,0,0,0,xax=0,/t3d, col=7,xsty=1
  axis,0,0,0,yax=0,/t3d, col=7,ysty=1
  axis,0,0,0,zax=2,/t3d, col=7,zsty=1
endif

if(image_there eq 1 and sh_image eq 1) then begin
	zxmin = - xis * pixel_size * .5 + x_origin
	zxmax =   xis * pixel_size * .5 + x_origin
	zymin = - yis * pixel_size * .5 + y_origin
	zymax =   yis * pixel_size * .5 + y_origin
	pat=image1
	px = [zxmin, zxmax, zxmax, zxmin] 
	py = [zymin, zymin, zymax, zymax]
	if (image_rotation ne 0.) then pat=rot(pat,0.,/interp)
	if(!order eq 1) then $
		polyfill,px,py,/t3d,pattern=pat,image_coord=[[0,yis],[xis,yis],[xis,0],[0,0]], $
		image_interp= 1, trans=img_thr, z=head_axis(2)

	if(!order eq 0) then $
		polyfill,px,py,/t3d,pattern=pat,image_coord=[[0,0],[xis,0],[xis,yis],[0,yis]], $
		image_interp= 1, trans=img_thr, z=head_axis(2)		
endif


xyouts,0.2,0.95,'Head Contours from PET',siz = 1.5,/normal,col=3
vue=tvrd()
set_plot,getenv('IDL_DEVICE')		; set_plot,'x'
tv,vue

WIDGET_CONTROL, message,SET_VALUE = " Ready "

END


;------------------------------------------------------------------------------
;	procedure showhat_ev
;------------------------------------------------------------------------------

PRO showhat_ev, event  

common  comim,	imgfile,images,image1,img_size,slice_width,zoom,xoff,yoff,rotoff, $
		image2,image_there
common  comco, 	contour,ncpts,head_axis

common shc1,	xrot, zrot, oaxes, caxes, symmetry, sh_image, win_num, $
 		xmin,ymin,zmin,xmax,ymax,zmax, $
		colnum, thick, psym, exchxy, img_thr

common shc2,	Xtransp,oaxeson,oaxesoff,caxeson,caxesoff,symmetryon,symmetryoff,imageon,imageoff,$
		imagetra,black,red,green,blue,yellow,violet,lblue,white, $
		t1,t2,t3,t4,t5,t6,t7,t8,t9, nameid, message, zmess,xmess

common shc3,	vue

common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr
	

WIDGET_CONTROL, event.id, GET_UVALUE = eventval		;find the user value
							;of the widget where
							;the event occured
CASE eventval OF

 "TRANSPARENCY": BEGIN
		WIDGET_CONTROL, event.id, GET_VALUE = th
		WIDGET_CONTROL,Xtransp, SET_UVALUE = imagetra
		img_thr = fix(th *2.55)
		showhat_draw
		zval='Transparency set at ' + strcompress(string(th)) +' %'
		WIDGET_CONTROL, message, SET_value = zval
	      END

 "T1": BEGIN
		zrot = (zrot + 10) mod 360
		IF(zrot LT 0) THEN zrot = 360 + zrot
		showhat_draw
		zval='Z rotation angle =' + strcompress(string(zrot))
		WIDGET_CONTROL, zmess, SET_value = zval
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T2": BEGIN
		zrot = (zrot - 10) mod 360
		IF(zrot LT 0) THEN zrot = 360 + zrot
		showhat_draw
		zval='Z rotation angle =' + strcompress(string(zrot))
		WIDGET_CONTROL, zmess, SET_value = zval
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T3": BEGIN
		xrot = (xrot - 10) mod 360
		IF(xrot LT 0) THEN xrot = 360 + xrot
		showhat_draw
		xval='X rotation angle =' + strcompress(string(xrot))
		WIDGET_CONTROL, xmess, SET_value = xval
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T4": BEGIN
		xrot = (xrot + 10) mod 360
		IF(xrot LT 0) THEN xrot = 360 + xrot
		showhat_draw
		xval='X rotation angle =' + strcompress(string(xrot))
		WIDGET_CONTROL, xmess, SET_value = xval
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T5": BEGIN	;shrink
		xmin = xmin * 1.25
		ymin = ymin * 1.25
		zmin = zmin * 1.25
		xmax = xmax * 1.25
		ymax = ymax * 1.25
		zmax = zmax * 1.25
		;!x.s= [-xmin,1]/(xmax-xmin)
		;!y.s= [-ymin,1]/(ymax-ymin)
		;!z.s= [-zmin,1]/(zmax-zmin)
		showhat_draw
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T6": BEGIN	;grow
		xmin = xmin * 0.8
		ymin = ymin * 0.8
		zmin = zmin * 0.8
		xmax = xmax * 0.8
		ymax = ymax * 0.8
		zmax = zmax * 0.8
		;!x.s= [-xmin,1]/(xmax-xmin)
		;!y.s= [-ymin,1]/(ymax-ymin)
		;!z.s= [-zmin,1]/(zmax-zmin)
		showhat_draw
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

    "T7": BEGIN	;z reverse
		zmin = -zmin 
		zmax = -zmax 
		;!z.s= [-zmin,1]/(zmax-zmin)
		showhat_draw
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END
 
   "T8": BEGIN	;x<>y
		; in case x,y boundaries are not the same
		t = xmin & xmin = ymin & ymin = t
 		t = xmax & xmax = ymax & ymax = t
		;!x.s= [-xmin,1]/(xmax-xmin)
		;!y.s= [-ymin,1]/(ymax-ymin)
		exchxy = - exchxy 
		showhat_draw
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END

  "T9": BEGIN	;reset
		zrot = 0.		; 30
		xrot = 90.		; 20
		xmin = -20 & ymin = -20 & zmin = -20
		xmax =  20 & ymax =  20 & zmax =  20
		;!x.s= [-xmin,1]/(xmax-xmin)
		;!y.s= [-ymin,1]/(ymax-ymin)
		;!z.s= [-zmin,1]/(zmax-zmin)
	        oaxes = 1
	        caxes = 1
	        symmetry = 1
		exchxy = 1
		showhat_draw
		zval='Z rotation angle =' + strcompress(string(zrot))
		WIDGET_CONTROL, zmess, SET_value = zval
		xval='X rotation angle =' + strcompress(string(xrot))
		WIDGET_CONTROL, xmess, SET_value = xval
		WIDGET_CONTROL, event.id, SET_BUTTON = 0
	      END


  "OAXESOFF":  IF(event.select EQ 1) THEN BEGIN
	         oaxes = 0
		 showhat_draw
	      ENDIF

  "OAXESON":  IF(event.select EQ 1) THEN BEGIN
	         oaxes = 1
		 showhat_draw
	      ENDIF
 
 "CAXESOFF":  IF(event.select EQ 1) THEN BEGIN
	         caxes = 0
		 showhat_draw
	      ENDIF

 "CAXESON":  IF(event.select EQ 1) THEN BEGIN
	         caxes = 1
		 showhat_draw
	      ENDIF

 "SYMMETRYOFF":  IF(event.select EQ 1) THEN BEGIN
	         symmetry = 0
		 showhat_draw
	      ENDIF

 "SYMMETRYON":  IF(event.select EQ 1) THEN BEGIN
	         symmetry = 1
		 showhat_draw
	      ENDIF

"IMAGEOFF":  IF(event.select EQ 1) THEN BEGIN
	         sh_image = 0
		 showhat_draw
	      ENDIF

 "IMAGEON":  IF(event.select EQ 1) THEN BEGIN
	         sh_image = 1
		 showhat_draw
	      ENDIF

  "SAVEPS":     BEGIN
		WIDGET_CONTROL, message,SET_VALUE = " Now creating Postscript, be patient ..."
	      	WIDGET_CONTROL, nameid, GET_VALUE = psfile
		set_plot,'PS'
		device,/color,BITS_PER_PIXEL=8,filename=psfile(0)
		;
		; swap some colors for Color postscript on Tektronix/PhaserII
		;
		vue (where(vue eq 7)) = 3			; white to blue
		vue (where(vue eq 0)) = 7			; black to white
		tv,vue
		device,/close
		set_plot,getenv('IDL_DEVICE')
		WIDGET_CONTROL, message,SET_VALUE = " Ready "
	     END

  "PALETTE": BEGIN
		XLoadct,group=event.top
		r_curr(0)=   0 	& g_curr(0)=   0 	& b_curr(0)=  0	; black
		r_curr(1)= 255	& g_curr(1)=   0 	& b_curr(1)=  0	; red
		r_curr(2)=   0 	& g_curr(2)= 255 	& b_curr(2)=  0	; green
		r_curr(3)=   0 	& g_curr(3)=   0 	& b_curr(3)=255	; blue
		r_curr(4)= 255 	& g_curr(4)= 255 	& b_curr(4)=  0 ; yellow
		r_curr(5)= 255 	& g_curr(5)=   0 	& b_curr(5)=255	; violet
		r_curr(6)=   0 	& g_curr(6)= 255 	& b_curr(6)=255	; lblue
		r_curr(7)= 255 	& g_curr(7)= 255 	& b_curr(7)=255	; white
		tvlct,r_curr, g_curr, b_curr			; Load clut
		r_orig = r_curr & g_orig = g_curr & b_orig = b_curr
	      END

  "BLACK":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 0
		 showhat_draw
	      ENDIF
  "RED":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 1
		 showhat_draw
	      ENDIF
  "GREEN":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 2
		 showhat_draw
	      ENDIF
  "BLUE":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 3
		 showhat_draw
	      ENDIF
  "YELLOW":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 4
		 showhat_draw
	      ENDIF
  "VIOLET":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 5
		 showhat_draw
	      ENDIF
 "LBLUE":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 6
		 showhat_draw
	      ENDIF
  "WHITE":  IF(event.select EQ 1) THEN BEGIN
	         colnum = 7
		 showhat_draw
	      ENDIF

  "FILENAME": 

  "EXIT": WIDGET_CONTROL, event.top, /DESTROY

   ELSE:; MESSAGE, "Event User Value Not Found"

ENDCASE

END ;============= end of showhat event handling routine task =============



;------------------------------------------------------------------------------
;	procedure showhat
;------------------------------------------------------------------------------

PRO showhat, GROUP = GROUP, SIZE=size, PSFILE = PSFILE


common  comco, 	contour,ncpts,head_axis

common shc1,	xrot, zrot, oaxes, caxes, symmetry, sh_image, win_num, $
 		xmin,ymin,zmin,xmax,ymax,zmax, $
		colnum, thick, psym, exchxy, img_thr

common shc2,	Xtransp,oaxeson,oaxesoff,caxeson,caxesoff,symmetryon,symmetryoff,imageon, imageoff, $
		imagetra,black,red,green,blue,yellow,violet,lblue,white, $
		t1,t2,t3,t4,t5,t6,t7,t8,t9, nameid, message, zmess,xmess

common shc3,	vue

common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr

IF(XRegistered("showhat")) THEN RETURN
IF(NOT(KEYWORD_SET(PSFILE)))THEN PSFILE = "showhat.ps"
IF(NOT(KEYWORD_SET(size)))THEN SIZE = 400

zrot = 20.
xrot = 30.

r_curr(0)=   0 	& g_curr(0)=   0 	& b_curr(0)=  0	; black
r_curr(1)= 255	& g_curr(1)=   0 	& b_curr(1)=  0	; red
r_curr(2)=   0 	& g_curr(2)= 255 	& b_curr(2)=  0	; green
r_curr(3)=   0 	& g_curr(3)=   0 	& b_curr(3)=255	; blue
r_curr(4)= 255 	& g_curr(4)= 255 	& b_curr(4)=  0 ; yellow
r_curr(5)= 255 	& g_curr(5)=   0 	& b_curr(5)=255	; violet
r_curr(6)=   0 	& g_curr(6)= 255 	& b_curr(6)=255	; lblue
r_curr(7)= 255 	& g_curr(7)= 255 	& b_curr(7)=255	; white
tvlct,r_curr, g_curr, b_curr				; Load clut
r_orig = r_curr & g_orig = g_curr & b_orig = b_curr

xmin = -20 & ymin = -20 & zmin = -20
xmax =  20 & ymax =  20 & zmax =  20
;!x.s= [-xmin,1]/(xmax-xmin)
;!y.s= [-ymin,1]/(ymax-ymin)
;!z.s= [-zmin,1]/(zmax-zmin)

thick = !p.thick
psym = !p.psym
exchxy = 1
!x.charsize=2 & !y.charsize=2 & !z.charsize=2
nameid =0L
sh_image = 0			; no image to superpose
img_thr = 77

showhatbase = WIDGET_BASE(TITLE = "Head Contour Display", /COLUMN)

XPdMenu, [	'"Postscript"			SAVEPS',	$
		'"Color"			PALETTE',	$
		'"Done"				EXIT'		$
	 ], showhatbase

thebase = WIDGET_BASE(showhatbase, /ROW)

Xtools = WIDGET_BASE(thebase, /COLUMN) 
;label1 = WIDGET_LABEL(Xtools, VALUE = "Tools")
tools = WIDGET_BASE(Xtools, /COLUMN, /EXCLUSIVE) 

controls = [							$
		;dnz.bmdef
		[						$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 248B, 255B, 063B, 000B],			$
		[128B, 007B, 000B, 224B, 001B],			$
		[112B, 000B, 000B, 000B, 014B],			$
		[136B, 000B, 016B, 000B, 016B],			$
		[052B, 000B, 048B, 000B, 056B],			$
		[172B, 000B, 080B, 000B, 032B],			$
		[124B, 000B, 144B, 000B, 040B],			$
		[164B, 007B, 016B, 001B, 048B],			$
		[012B, 248B, 031B, 062B, 056B],			$
		[036B, 000B, 000B, 228B, 033B],			$
		[004B, 000B, 000B, 008B, 062B],			$
		[012B, 000B, 000B, 016B, 048B],			$
		[036B, 000B, 000B, 016B, 032B],			$
		[008B, 000B, 000B, 008B, 000B],			$
		[112B, 000B, 000B, 004B, 000B],			$
		[128B, 007B, 000B, 002B, 000B],			$
		[000B, 248B, 031B, 001B, 000B],			$
		[000B, 000B, 144B, 000B, 000B],			$
		[000B, 000B, 080B, 000B, 000B],			$
		[000B, 000B, 048B, 000B, 000B],			$
		[000B, 000B, 016B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B]			$
		],						$
		;upz.bmdef
		[						$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 252B, 255B, 031B, 000B],			$
		[128B, 007B, 000B, 224B, 001B],			$
		[112B, 000B, 000B, 000B, 014B],			$
		[008B, 000B, 008B, 000B, 017B],			$
		[028B, 000B, 012B, 000B, 044B],			$
		[004B, 000B, 010B, 000B, 053B],			$
		[020B, 000B, 009B, 000B, 062B],			$
		[012B, 128B, 008B, 224B, 037B],			$
		[028B, 124B, 248B, 031B, 048B],			$
		[132B, 039B, 000B, 000B, 036B],			$
		[124B, 016B, 000B, 000B, 032B],			$
		[012B, 008B, 000B, 000B, 048B],			$
		[004B, 008B, 000B, 000B, 036B],			$
		[000B, 016B, 000B, 000B, 016B],			$
		[000B, 032B, 000B, 000B, 014B],			$
		[000B, 064B, 000B, 224B, 001B],			$
		[000B, 128B, 248B, 031B, 000B],			$
		[000B, 000B, 009B, 000B, 000B],			$
		[000B, 000B, 010B, 000B, 000B],			$
		[000B, 000B, 012B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B]			$
		],						$
		;dnx.bm
		[						$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 128B, 255B, 001B, 000B],			$
		[000B, 064B, 169B, 002B, 000B],			$
		[000B, 160B, 254B, 007B, 000B],			$
		[000B, 160B, 068B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 008B, 016B, 000B, 000B],			$
		[000B, 008B, 016B, 000B, 000B],			$
		[000B, 008B, 144B, 001B, 000B],			$
		[000B, 008B, 112B, 006B, 000B],			$
		[000B, 008B, 016B, 008B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 006B, 096B, 000B],			$
		[000B, 008B, 001B, 128B, 000B],			$
		[000B, 136B, 000B, 000B, 001B],			$
		[000B, 200B, 015B, 240B, 003B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 016B, 004B, 008B, 000B],			$
		[000B, 016B, 004B, 008B, 000B],			$
		[000B, 016B, 005B, 008B, 000B],			$
		[000B, 080B, 004B, 008B, 000B],			$
		[000B, 032B, 147B, 004B, 000B],			$
		[000B, 160B, 042B, 005B, 000B],			$
		[000B, 064B, 149B, 002B, 000B],			$
		[000B, 128B, 255B, 001B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B]			$
		],						$
		;upx.bm
		[						$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 128B, 255B, 001B, 000B],			$
		[000B, 064B, 149B, 002B, 000B],			$
		[000B, 160B, 042B, 005B, 000B],			$
		[000B, 032B, 147B, 004B, 000B],			$
		[000B, 080B, 004B, 008B, 000B],			$
		[000B, 016B, 005B, 008B, 000B],			$
		[000B, 016B, 004B, 008B, 000B],			$
		[000B, 016B, 004B, 008B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 200B, 015B, 240B, 003B],			$
		[000B, 136B, 000B, 000B, 001B],			$
		[000B, 008B, 001B, 128B, 000B],			$
		[000B, 008B, 006B, 096B, 000B],			$
		[000B, 008B, 008B, 016B, 000B],			$
		[000B, 008B, 016B, 008B, 000B],			$
		[000B, 008B, 112B, 006B, 000B],			$
		[000B, 008B, 144B, 001B, 000B],			$
		[000B, 008B, 016B, 000B, 000B],			$
		[000B, 008B, 016B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 016B, 032B, 000B, 000B],			$
		[000B, 160B, 068B, 000B, 000B],			$
		[000B, 160B, 254B, 007B, 000B],			$
		[000B, 064B, 169B, 002B, 000B],			$
		[000B, 128B, 255B, 001B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B]			$
		],						$
		;shrink.bm
		[						$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 073B, 000B, 000B],			$
		[000B, 000B, 042B, 000B, 000B],			$
		[000B, 000B, 028B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[128B, 255B, 255B, 255B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[132B, 000B, 000B, 000B, 033B],			$
		[136B, 000B, 000B, 000B, 017B],			$
		[144B, 000B, 000B, 000B, 009B],			$
		[191B, 000B, 000B, 000B, 253B],			$
		[144B, 000B, 000B, 000B, 009B],			$
		[136B, 000B, 000B, 000B, 017B],			$
		[132B, 000B, 000B, 000B, 033B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 000B, 000B, 000B, 001B],			$
		[128B, 255B, 255B, 255B, 001B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 028B, 000B, 000B],			$
		[000B, 000B, 042B, 000B, 000B],			$
		[000B, 000B, 073B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B],			$
		[000B, 000B, 008B, 000B, 000B]			$
		],						$
		;grow.bm
		[						$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[248B, 255B, 255B, 255B, 031B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 028B, 000B, 016B],			$
		[008B, 000B, 042B, 000B, 016B],			$
		[008B, 000B, 073B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 008B, 000B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 001B, 000B, 000B, 016B],			$
		[136B, 000B, 000B, 128B, 016B],			$
		[072B, 000B, 000B, 000B, 017B],			$
		[232B, 063B, 000B, 000B, 018B],			$
		[072B, 000B, 000B, 252B, 023B],			$
		[136B, 000B, 000B, 000B, 018B],			$
		[008B, 001B, 000B, 000B, 017B],			$
		[008B, 000B, 000B, 128B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[008B, 000B, 016B, 000B, 016B],			$
		[008B, 000B, 016B, 000B, 016B],			$
		[008B, 000B, 016B, 000B, 016B],			$
		[008B, 000B, 016B, 000B, 016B],			$
		[008B, 000B, 146B, 000B, 016B],			$
		[008B, 000B, 084B, 000B, 016B],			$
		[008B, 000B, 056B, 000B, 016B],			$
		[008B, 000B, 016B, 000B, 016B],			$
		[008B, 000B, 000B, 000B, 016B],			$
		[248B, 255B, 255B, 255B, 031B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B],			$
		[000B, 000B, 000B, 000B, 000B]			$
		]						$
	   ]

t1 = WIDGET_BUTTON(tools, VALUE = controls(*,*,0), UVALUE = "T1")
t2 = WIDGET_BUTTON(tools, VALUE = controls(*,*,1), UVALUE = "T2")
t3 = WIDGET_BUTTON(tools, VALUE = controls(*,*,2), UVALUE = "T3")
t4 = WIDGET_BUTTON(tools, VALUE = controls(*,*,3), UVALUE = "T4")
t5 = WIDGET_BUTTON(tools, VALUE = controls(*,*,4), UVALUE = "T5")
t6 = WIDGET_BUTTON(tools, VALUE = controls(*,*,5), UVALUE = "T6")
t7 = WIDGET_BUTTON(tools, VALUE = " Z Rev", UVALUE = "T7")
t8 = WIDGET_BUTTON(tools, VALUE = " X<>Y ", UVALUE = "T8")
t9 = WIDGET_BUTTON(tools, VALUE = " Front", UVALUE = "T9")

Xdispbase = WIDGET_BASE(thebase, /COLUMN)
Xdisplay = WIDGET_DRAW(xdispbase, XSIZE = SIZE, YSIZE = SIZE, RETAIN = 2)
anglebase = WIDGET_BASE(xdispbase,/ROW, XPAD = 10, SPACE = 30)
zval='Z rotation angle =' + strcompress(string(zrot))
xval='X rotation angle =' + strcompress(string(xrot))
zmess  = WIDGET_LABEL(anglebase, VALUE = zval)
xmess  = WIDGET_LABEL(anglebase, VALUE = xval)

Xtransp = WIDGET_BASE(thebase, /COLUMN)
label6 = WIDGET_LABEL(Xtransp, VALUE = " Transparency ")
ithr = fix(img_thr/2.55)
imagetra = widget_slider(Xtransp, ysize = size, VALUE = ithr , /vertical, $
		  MINIMUM = 0, MAXIMUM = 100, UVALUE = "TRANSPARENCY")

Xcontrols = WIDGET_BASE(showhatbase, /COLUMN)


Xaxes = WIDGET_BASE(Xcontrols, /ROW)

Xoaxes = WIDGET_BASE(Xaxes, /ROW)
label2 = WIDGET_LABEL(Xoaxes, VALUE = " Outer Axes ")
oaxesb = WIDGET_BASE(Xoaxes,/ROW, /EXCLUSIVE)
oaxeson = WIDGET_BUTTON(oaxesb,  VALUE = "Show", UVALUE = "OAXESON")
oaxesoff = WIDGET_BUTTON(oaxesb, VALUE = "Hide", UVALUE = "OAXESOFF")

Xcaxes = WIDGET_BASE(Xaxes, /ROW)
label3 = WIDGET_LABEL(Xcaxes, VALUE = " Centered Axes ")
caxesb = WIDGET_BASE(Xcaxes,/ROW, /EXCLUSIVE)
caxeson = WIDGET_BUTTON(caxesb,  VALUE = "Show", UVALUE = "CAXESON")
caxesoff = WIDGET_BUTTON(caxesb, VALUE = "Hide", UVALUE = "CAXESOFF")

Xsymmetry = WIDGET_BASE(Xaxes,/ROW)
label4 = WIDGET_LABEL(Xsymmetry, VALUE = " Head Axe ")
symmetryb = WIDGET_BASE(Xsymmetry,/ROW, /EXCLUSIVE)
symmetryon = WIDGET_BUTTON(symmetryb, VALUE = "Show", UVALUE = "SYMMETRYON")
symmetryoff = WIDGET_BUTTON(symmetryb, VALUE = "Hide",UVALUE = "SYMMETRYOFF")

Ximage = WIDGET_BASE(Xaxes,/ROW)
label5 = WIDGET_LABEL(Ximage, VALUE = " Image ")
imageb = WIDGET_BASE(Ximage,/ROW, /EXCLUSIVE)
imageon = WIDGET_BUTTON(imageb, VALUE = "Show", UVALUE = "IMAGEON")
imageoff = WIDGET_BUTTON(imageb, VALUE = "Hide",UVALUE = "IMAGEOFF")

Xcolor = WIDGET_BASE(Xcontrols,/ROW)
label7 = WIDGET_LABEL(Xcolor, VALUE = " Hat Color ")
hcolor = WIDGET_BASE(Xcolor,/ROW, /EXCLUSIVE)
black   = WIDGET_BUTTON(hcolor, VALUE = "Black",   UVALUE = "BLACK")
red   = WIDGET_BUTTON(hcolor, VALUE = "Red",   UVALUE = "RED")
green = WIDGET_BUTTON(hcolor, VALUE = "Green", UVALUE = "GREEN")
blue  = WIDGET_BUTTON(hcolor, VALUE = "Blue" , UVALUE = "BLUE")
yellow  = WIDGET_BUTTON(hcolor, VALUE = "Yellow" , UVALUE = "YELLOW")
violet  = WIDGET_BUTTON(hcolor, VALUE = "Violet" , UVALUE = "VIOLET")
lblue  = WIDGET_BUTTON(hcolor, VALUE = "Light Blue" , UVALUE = "LBLUE")
white = WIDGET_BUTTON(hcolor, VALUE = "White", UVALUE = "WHITE")

Xsave = WIDGET_BASE(Xcontrols, /ROW, /frame)
label6 = WIDGET_LABEL(Xsave, VALUE = " Save screen as ")
nameid = WIDGET_TEXT(Xsave, VALUE = psfile, XSIZE = 40, YSIZE = 1, $
		/EDITABLE, UVALUE = "FILENAME")

statusbase = WIDGET_BASE(Xcontrols, /ROW, XPAD = 10, SPACE = 30)
statuslabel = WIDGET_LABEL(statusbase, VALUE = " Status ")
message = WIDGET_LABEL(statusbase, VALUE = " Ready ")



WIDGET_CONTROL, showhatbase, /REALIZE			;create the widgets
							;that is defined
WIDGET_CONTROL, oaxeson, /SET_BUTTON
oaxes=1 
WIDGET_CONTROL, caxeson, /SET_BUTTON
caxes=1
WIDGET_CONTROL, symmetryon, /SET_BUTTON
symmetry=1 
WIDGET_CONTROL, imageoff, /SET_BUTTON
sh_image=0 
WIDGET_CONTROL, red, /SET_BUTTON
colnum=1
 
WIDGET_CONTROL, Xdisplay, GET_VALUE = win_num

showhat_draw,size=size

XManager, "showhat", showhatbase, $			;register the widgets
		EVENT_HANDLER = "showhat_ev", $	;with the XManager
		GROUP_LEADER = GROUP

END ;================ end of showhat background task =====================



