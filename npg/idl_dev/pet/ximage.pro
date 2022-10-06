;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   XIMAGE.PRO   **********
;
;	Read Display and Process for images and scans	
;
;	VERSION   JULY 10,1992	: Christian MICHEL
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pro read_image

common 	xic1, win_num1,win_num2,nf,np,ng,nb,image1,image2, $
	file,f,p,g,b,plane_sep,slice_width,pixel_size, $
	x_size,y_size,sc

common 	xic2, xibase,xidraw,label1,label2,label3, $
	text1,text2,slide1,slide2,slide3,slide4, $
	draw1,draw2,message,buttondebug

	ON_ERROR, 2
	WIDGET_CONTROL, message, SET_VALUE = " Reading matrix file ..."
	WSET, win_num1
	stat=recat(file,f,p,g,0,b,data)
	if(stat eq !ECAT.ERROR) then WIDGET_CONTROL, message ,SET_VALUE = !ECAT.ERROR
	ftype	= data.ftype
	x_size	= data.xdim
	y_size	= data.ydim
	ssize = '( ' + string(x_size) + ' x ' + string(y_size) + ')'
	if(ftype eq 2) then begin
		slice_width = data.sh.slice_width
		pixel_size = data.sh.pixel_size
	end 
	if(ftype eq 1 ) then begin
		plane_sep = data.mh.scan.plane_separation
		pixel_size = data.sh.sample_distance
	end

	if( x_size ne y_size) then begin
		text = " Non square matrix " + ssize
		WIDGET_CONTROL, message ,SET_VALUE = text
		sc = max([x_size,y_size])/256.
	endif
	if (x_size eq y_size) then begin	; normal images are always 64 or 128 or 256
		image1 = rebin(data.data,256,256)
		WIDGET_CONTROL, message ,SET_VALUE = 'image size set to 256x256'
		x_size = 256 & y_size =256
	endif else begin			; scan case
		if (x_size lt 256 and y_size lt 256) then begin
			image1 = data.data
		        WIDGET_CONTROL, message ,SET_VALUE = 'original matrix size '
		endif else begin		; large image (wholebody viewer wbp)
			image1 = poly_2d(data.data,[0,0,sc,0],[0,sc,0,0],1,256,256)
			WIDGET_CONTROL, message ,SET_VALUE = " polywarped data ..."
			x_size = 256 & y_size =256
		endelse
	endelse
	;hdrout = header
	erase
        TVSCL, image1
	image2 = bytscl(image1)
	WSET, win_num2
	erase
        TVSCL, image2
	lf = "\x0a"
	specs = string(f) + string(p) + string(g) + string(b)
	if(ftype eq 2) then begin
		info =  ' => Loading new image.... ' + lf $ 
			+ ' Image            : ' + file + lf $
			+ ' Dimensions       : ' + ssize + lf $
			+ ' Specifications   : ' + specs + lf $
			+ ' Pixel size       : ' + string(pixel_size) + lf $
			+ ' Slice width      : ' + string(slice_width) + lf
	end 
	if(ftype eq 1) then begin
		info =  ' => Loading new scan.... ' + lf $ 
			+ ' Scan             : ' + file + lf $
			+ ' Dimensions       : ' + ssize + lf $
			+ ' Specifications   : ' + specs + lf $
			+ ' Pixel size       : ' + string(pixel_size) + lf $
			+ ' Plane separation : ' + string(plane_sep) + lf
	end 
        WIDGET_CONTROL, text1, set_value = info,/append
	WIDGET_CONTROL,label3,set_value = $
		'Original matrix                                     Original matrix'
	WIDGET_CONTROL, message, SET_VALUE = " Ready"
	return
end

pro ximage_event, event

common 	xic1, win_num1,win_num2, nf,np,ng,nb,image1,image2,$
	file,f,p,g,b,plane_sep,slice_width,pixel_size, $
	x_size,y_size,sc

common 	xic2,xibase, xidraw,label1,label2,label3, $
	text1,text2,slide1,slide2,slide3,slide4, $
	draw1,draw2,message,buttondebug

common 	xic3, eslide1, eslide2,eslide3,eslide4,elabel2,debug

ON_ERROR, 2

WIDGET_CONTROL, event.id, GET_UVALUE = eventval

if debug eq 1 then HELP, /STRUCT, event 
	
lf = "\x0a" 

CASE eventval OF
   'DEBUG': BEGIN
		debug = -debug
		WIDGET_CONTROL, buttondebug, destroy=1
		if (debug eq -1) then $
		  buttondebug = WIDGET_BUTTON(xibase,VALUE='Do NOT print X events',UVALUE='DEBUG', /NO_RELEASE)
		if (debug eq 1) then $
		  buttondebug = WIDGET_BUTTON(xibase,VALUE='Print X events',UVALUE='DEBUG', /NO_RELEASE)
		WIDGET_CONTROL, xibase, /REALIZE
		return
           END

   'SAVE': BEGIN
		WIDGET_CONTROL, message ,SET_VALUE = ' Not yet implemented ...'
		return
             END

   'DRAW_WIN_EVENT1': BEGIN
			WSET, win_num1
			IF event.press EQ 1 THEN BEGIN
			  i= event.x & j = event.y
			  if !order eq 1 then k = y_size - j else k = j
			  position = string(i) + string(j)
			  if (i ge 1 and i le x_size and k ge 1 and k le y_size) then begin
			     v1 = string(image1(i,k)) & v2 = string(long(image2(i,k)))
			  endif else begin
			     v1 = 'undefined' & v2 = 'undefined'
			  endelse
			  info1 =  ' Left image  : ' + v1
			  info2 =  ' Right image : ' + v2
			  info = ' Pixel value at position (' + position + ')' + lf + info1 + lf + info2 
                          WIDGET_CONTROL, text2, set_value = info
			ENDIF
			IF event.press EQ 0 THEN BEGIN
               		  WIDGET_CONTROL, text2, set_value = ''
			ENDIF
		     END
   
  'DRAW_WIN_EVENT2': BEGIN
			WSET, win_num2
			IF event.press EQ 1 THEN BEGIN
			  i= event.x & j = event.y
			  if !order eq 1 then k = y_size - j else k = j
			  position = string(i) + string(j)
			  if (i ge 1 and i le x_size and k ge 1 and k le y_size) then begin
			     v1 = string(image1(i,k)) & v2 = string(long(image2(i,k)))
			  endif else begin
			     v1 = 'undefined' & v2 = 'undefined'
			  endelse
			  info1 =  ' Left image  : ' + v1
			  info2 =  ' Right image : ' + v2
			  info = ' Pixel value at position (' + position + ')' + lf + info1 + lf + info2 
                          WIDGET_CONTROL, text2, set_value = info
			ENDIF
			IF event.press EQ 0 THEN BEGIN
               		  WIDGET_CONTROL, text2, set_value = ''
			ENDIF
		     END
 'PROFILE1': BEGIN
		WSET, win_num1
		profiles,image1,order=!order		             
	     	return
             END
 
 'PROFILE2': BEGIN
		WSET, win_num2
		profiles,image2,order=!order		             
	     	return
             END
 
   'ROTATE': BEGIN
		WSET, win_num2
		angle=0
		xvaredit,angle
		tmp = execute('image2=rot_int(image2,angle)')	             
		info = ' Image rotated by '+ string(angle)            
         	WIDGET_CONTROL, text1, set_value = info,/append
		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Rotated matrix'
               TVSCL, image2
	     	return
             END

   'INVERT': BEGIN
		WSET, win_num2
		tmp = execute('image2= 256-image2')	             
		info = ' Image inverted'            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Inverted matrix'
               TVSCL, image2
	     	return
             END

    'BIAS': BEGIN
		WSET, win_num2
		bthr=0
		xvaredit,bthr
		tmp = execute('image2=(image2>bthr) - bthr')	             
		info = ' Bias performed with threshold at '+ string(bthr)            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Biased matrix'
                TVSCL, image2
	     	return
             END

    'TRUNCATE': BEGIN
		WSET, win_num2
		tthr=0
		xvaredit,tthr
		tmp = execute('image2=image2<tthr')	             
		info = ' Trucation performed with threshold at ' + string(tthr)            
         	WIDGET_CONTROL, text1, set_value = info,/append
  		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Truncated matrix'
               TVSCL, image2
	     	return
             END
 
   'SMOOTH3': BEGIN
		WSET, win_num2
		tmp = execute('image2=smooth(image2,3)')	             
		info = ' Smoothing performed (kernel size = 3)'            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Smoothed 3x3 matrix'
                TVSCL, image2
	     	return
	    END

   'SMOOTH5': BEGIN
		WSET, win_num2
		tmp = execute('image2=smooth(image2,5)')	             
		info = ' Smoothing performed (kernel size = 5)'            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Smoothed 5x5 matrix'
                TVSCL, image2
	     	return
             END

    'SMOOTH7': BEGIN
		WSET, win_num2
		tmp = execute('image2=smooth(image2,7)')	             
		info = ' Smoothing performed (kernel size = 7)'            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Smoothed 7x7 matrix'
                TVSCL, image2
	     	return
             END
  
 'ROBERTS': BEGIN
		WSET, win_num2
		tmp = execute('image2=roberts(image2)')	 
		info = ' Roberts edge detection performed'            
         	WIDGET_CONTROL, text1, set_value = info,/append
                TVSCL, image2
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Edge matrix (Roberts)'
	     	return
             END
 
  'SOBEL': BEGIN
		WSET, win_num2
		tmp = execute('image2=sobel(image2)')	             
 		info = ' Sobel edge detection performed'            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Edge matrix (Sobel)'
                TVSCL, image2
	     	return
           END

 'CONTOUR': BEGIN
		WSET, win_num2
		bthr1 = 50
		tmp = execute('image2=(image2>bthr1) - bthr1')	             
		tmp = execute('image2=sobel(image2)')	
		bthr2 = 50
		tmp = execute('image2=(image2>bthr2) - bthr2')	             
 		info = ' Bias performed with threshold at '+ string(bthr1)  +lf          
		info = info + ' Sobel edge detection performed' + lf           
		info = info + ' Bias performed with threshold at '+ string(bthr2)            
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Contour matrix'
                TVSCL, image2
	     	return
             END

   'PEEL': BEGIN
		WSET, win_num2
		bthr1 = 10
		tmp = execute('image2=(image2>bthr1) - bthr1')
		tmp = execute('image3=image2')	             
		tmp = execute('image2=sobel(image2)')	             
		tmp = execute('image2=smooth(image2,3)')	             
		tmp = execute('image2=(image3-image2)>0')	             
		tmp = execute('image2=(256-image2) * (image2 ne 0)')	             
		info = ' Peeling skull ...'           
         	WIDGET_CONTROL, text1, set_value = info,/append
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Peeled Brain'
                TVSCL, image2
	     	return
	    END

   'CLUT': BEGIN
		xloadct, GROUP = event.top
                return
             END

   'LOAD': BEGIN
		image1 = 0 & image2 = 0
		f = 1 & p = 1 & g = 1 & b =0	; initial values
		data_dir = '/topo6b/mich/data/cti'
		cd, data_dir, current=curdir
		file = pickfile(GROUP=group,TITLE='Select CTI Image',FILTER=['img','imp','scn','wbp'])
		if (file eq '') then return
		; not yet implemented
	        ; filout=file
	        ; strput,filout,'imp',strpos(file,'img') 	; imp extension for processed image
		cd, curdir
;
		openr,unit,file,/get_lun
		stat=rmh(unit,mh)
	        if(stat eq !ECAT.ERROR) then WIDGET_CONTROL, message ,SET_VALUE = !ECAT.ERROR
		nf = mh.file.nframes
		np = mh.file.nplanes
		ng = mh.file.ngates
		nb = mh.file.nbeds
		nsum = nf+np+ng+nb
		if (nsum eq 0) then WIDGET_CONTROL, message ,SET_VALUE = ' Main header is not complete ...'
		stat=rdir(unit,dir)		; Guess first matrix number
		ispecs = matval(dir(0))
		f= ispecs(0) & p= ispecs(1) & g= ispecs(2) & d= ispecs(3) & b= ispecs(4)
		free_lun,unit
;
		if (elabel2 eq 0) then label2 = WIDGET_LABEL(xibase, VALUE = 'Select Specifications :')
		elabel2 = 1
		if (eslide1 eq 1) then 	begin
			WIDGET_CONTROL, slide1, destroy=1
			eslide1 = 0
		end
		if (eslide2 eq 1) then 	begin
			WIDGET_CONTROL, slide2, destroy=1
			eslide2 = 0
		end
		if (eslide3 eq 1) then 	begin
			WIDGET_CONTROL, slide3, destroy=1
			eslide3 = 0
		end
		if (eslide4 eq 1) then 	begin 
			WIDGET_CONTROL, slide4, destroy=1
			eslide4 = 0
		end

		if nf gt 1 then begin
			eslide1 = 1
 			slide1 = WIDGET_SLIDER(xibase, MAXIMUM = nf, MINIMUM = 1, $
				TITLE="Frame",VALUE = f, UVALUE = "FRAME",XSIZE = 256)
		end 

		if np gt 1 then begin
			eslide2 = 1
			slide2 = WIDGET_SLIDER(xibase, MAXIMUM = np, MINIMUM = 1, $
				TITLE="Plane",VALUE = p, UVALUE = "PLANE",XSIZE = 256)
		end 
		if ng gt 1 then begin
			eslide3 = 1
			slide3 = WIDGET_SLIDER(xibase, MAXIMUM = ng, MINIMUM = 0, $
				TITLE="Gate",VALUE = g, UVALUE = "GATE",XSIZE = 256)
		end 
		if nb gt 1 then begin
			eslide4 = 1
			slide4 = WIDGET_SLIDER(xibase, MAXIMUM = nb, MINIMUM = 0, $
				TITLE="Bed",VALUE = b, UVALUE = "BED",XSIZE = 256)
		end

		WIDGET_CONTROL, draw1, SENSITIVE=1
		WIDGET_CONTROL, draw2, SENSITIVE=1
                WIDGET_CONTROL, label1, SENSITIVE=1
		specs = string(f) + string(p) + string(g) + string(b)
		WIDGET_CONTROL, xibase, /REALIZE
		read_image
 		WIDGET_CONTROL,label3,set_value = $
 		'Original matrix                                     Original matrix'
		return
             END

    'FRAME': BEGIN
		WIDGET_CONTROL, xibase, SET_UVALUE = slide1
		f = event.value
		read_image
		return
	     END
 
   'PLANE': BEGIN
		WIDGET_CONTROL, xibase, SET_UVALUE = slide2
		p = event.value
		read_image
		return
	     END

    'GATE': BEGIN
		WIDGET_CONTROL, xibase, SET_UVALUE = slide3
		g = event.value
		read_image
		return
	     END

    'BED': BEGIN
		WIDGET_CONTROL, xibase, SET_UVALUE = slide4
		b = event.value
		read_image
		return
	     END

   'DONE': BEGIN
                WIDGET_CONTROL, event.top, /DESTROY
           END
ENDCASE

END

PRO ximage, GROUP=GROUP

common 	xic1, win_num1,win_num2, nf,np,ng,nb,image1,image2, $
	file,f,p,g,b,plane_sep,slice_width,pixel_size, $
	x_size,y_size,sc

common 	xic2,xibase,xidraw,label1,label2,label3, $
	text1,text2,slide1,slide2,slide3,slide4, $
	draw1,draw2,message,buttondebug

common 	xic3, eslide1, eslide2,eslide3,eslide4,elabel2,debug

ON_ERROR, 2

eslide1 = 0 & eslide2 = 0 & eslide3 = 0 & eslide4 = 0 & elabel2 = 0 & debug = -1

xibase = WIDGET_BASE(TITLE = 'Image & Scan Processing',/COLUMN)


XPdMenu, [	'"Load Study"           LOAD',		$
		'"Colors"               CLUT',		$
		'"Profiles"		{',		$
		'"on original ..."	PROFILE1',	$
		'"on processed ..."	PROFILE2',	$
		'}',					$
		'"Processing" 	{', 		$
		'"Smooth"		{',		$
		'"3x3"			SMOOTH3',	$
		'"5x5"			SMOOTH5',	$
		'"7x7"			SMOOTH7',	$
		'}',					$
		'"Bias"			BIAS',		$
		'"Truncate"		TRUNCATE',	$
		'"Sobel "		SOBEL',		$
		'"Roberts"		ROBERTS', 	$
		'"Contour"		CONTOUR', 	$
		'"Rotate"		ROTATE', 	$
		'"Invert"		INVERT', 	$
		'"Peel Skull"		PEEL', 		$
		'}',					$
		'"Save as"		SAVE',		$
		'"Exit"			DONE' ], xibase

status = WIDGET_BASE(xibase, /ROW, XPAD = 10, SPACE = 30)
statuslabel = WIDGET_LABEL(status, VALUE = " Status ")
message = WIDGET_LABEL(status, VALUE = " Starting ximage ... ")

label3 = WIDGET_LABEL(xibase, VALUE = '')

xidraw = WIDGET_BASE(xibase,/row)

draw1 = WIDGET_DRAW(xidraw, $
	/BUTTON_EVENTS, $	;generate events when buttons pressed
	/FRAME, $
	UVALUE = 'DRAW_WIN_EVENT1', $
	RETAIN = 2, $		;make sure draw is redrawn when covered
	XSIZE = 256, $
	YSIZE = 256)

draw2 = WIDGET_DRAW(xidraw, $
	/BUTTON_EVENTS, $	;generate events when buttons pressed
	/FRAME, $
	UVALUE = 'DRAW_WIN_EVENT2', $
	RETAIN = 2, $		;make sure draw is redrawn when covered
	XSIZE = 256, $
	YSIZE = 256)

text1 = WIDGET_TEXT(xibase, VALUE = 'History ....', xsize=70,YSIZE = 8,/scroll,/FRAME)

label1 = WIDGET_LABEL(xibase, VALUE = 'Use left mouse to see coordinates and value')
text2 = WIDGET_TEXT(xibase, VALUE = '', xsize=70,YSIZE = 3,/FRAME)
buttondebug = WIDGET_BUTTON(xibase,VALUE='Do NOT print X events',UVALUE='DEBUG', /NO_RELEASE)	

WIDGET_CONTROL, draw1, SENSITIVE=0
WIDGET_CONTROL, draw2, SENSITIVE=0
WIDGET_CONTROL, label1, SENSITIVE=0

WIDGET_CONTROL, xibase, /REALIZE

WIDGET_CONTROL, draw1, GET_VALUE = win_num1
WIDGET_CONTROL, draw2, GET_VALUE = win_num2
WIDGET_CONTROL, message, SET_VALUE = " Please load data ..."

XMANAGER, "ximage", xibase, GROUP_LEADER=GROUP

END


