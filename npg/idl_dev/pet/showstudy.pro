;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   SHOWSTUDY.PRO   **********
;
;	Allow to get a general overview of a study 
;	and to produce a color PS file. The size of the page is 
;	adjusted to get a full A4 page. A previewer allows
;	to check color substitutions prior to send the PS 
;	file to the printer. The file is created where in the same
;	directory as the original data.
;
;	- Load a given study (images/scan) and move forward/backward 
;	  in the list. The display start at the selected matrix.
;	- Compress data (rebin in both dimensions) in order to get more
;	  data per page
;	- Bias remove unwanted background.
;	- Colors: usual CLUT tool
;	- Movie, animate the images from the 1st shown on the screen 
;	  till the end of the list;
;	  specially made to see gated nmr studies where the first images 
;	  are transversal sections for positionning.
;
;	  note: xinteranimate bugs
;	   1) set initial speed : click again on the widget
;	   2) !order ariable is not accounted for
;
;	VERSION 1:	 JULY 25,1992		 Christian MICHEL 
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pro read_matrix,new=new

common 	shsc1, nf,np,ng,nb,matrix_data,matfile,f,p,g,b, $
	plane_sep,sixel_size,pixel_size,slice_width, $
	x_size,y_size,mats,specs,index,maxindex,maximage,nx,ny,ntot

common shsc2, shsbase,message,history,nameid,draw,matlist,win_num, $
	next,previous,bthr,old_bthr

	lf = "\x0a" 
	data_dir = '/topo6b/mich/data/cti'
	cd, data_dir, current=curdir
	matfile = pickfile(GROUP=group,TITLE='Select Matrix file',FILTER=['img','scn'])
	if (matfile eq '') then return
	openr,unit,matfile,/get_lun
	stat=rmh(unit,mh)
	if(stat eq !ECAT.ERROR) then begin
		print, !ECAT.ERROR
		return
	endif
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
;
;	
;
	mats(*,0) = matval(dir(0,0))
	f= mats(0,0) & p= mats(1,0) & g= mats(2,0) & d= mats(3,0) & b= mats(4,0)
	stat=recat(matfile,f,p,g,d,b,data)
	x_size	= data.xdim
	y_size	= data.ydim
	matrix_data = bytarr(x_size,y_size,ntot)
;
	WIDGET_CONTROL, message, SET_VALUE = " Reading matrix file, please be patient ..."
	for i =0, ntot-1 do begin
		mats(*,i) = matval(dir(0,i))
		f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
		specs(i) = strcompress('frame '+ string(f) +' plane '+string(p)+' gate '+string(g)+ $
			' data '+string(d)+' bed '+string(b))
	       stat=recat(matfile,f,p,g,d,b,data)
	       if(stat eq !ECAT.ERROR) then begin 
		   print, !ECAT.ERROR
		   return
	       end
	      matrix_data(*,*,i)=bytscl(data.data)
	end
;
	ftype	= data.ftype
	x_size	= data.xdim
	y_size	= data.ydim
	if(ftype eq 2) then begin
		slice_width = data.sh.slice_width
		pixel_size = data.sh.pixel_size
		info =  ' => New file loaded with ' + string(ntot) + ' images ' + lf  $
 			+ ' Image             : ' + matfile + lf $
			+ ' Dimensions        : ' + string(x_size) +'x' + string(y_size) + lf $
			+ ' Slice width       : ' + string(slice_width) + lf $
			+ ' Pixel size        : ' + string(pixel_size) + lf
	end 
	if(ftype eq 1 ) then begin
		plane_sep = data.mh.scan.plane_separation
		pixel_size = data.sh.sample_distance
		info =  ' => New file loaded '  + string(ntot) + ' scan' + lf  $
 			+ ' Scan              : ' + matfile + lf $
			+ ' Dimensions        : ' + string(x_size) +'x' + string(y_size) + lf $
			+ ' Plane separation  : ' + string(plane_sep) + lf $
			+ ' Pixel size        : ' + string(pixel_size) + lf
	end 
        WIDGET_CONTROL, history, set_value = info,/append
	if(not keyword_set(new)) then wdelete,win_num

	while ((nx*x_size) gt 768) do nx = nx-1
	while ((ny*y_size) gt 512) do ny = ny-1
	window,/free ,xs=nx*x_size, ys=ny*y_size,retain=2,title=matfile
	win_num = !d.window
  	widget_control,matlist,set_value=specs
	psfile = matfile
	strput,psfile,'.ps ',strpos(matfile,'.') 
	psfile = strcompress(psfile,/remove_all)	
	widget_control, nameid, set_value = psfile
	maximage = nx*ny
end

pro display_matrix

common 	shsc1, nf,np,ng,nb,matrix_data,matfile,f,p,g,b, $
	plane_sep,sixel_size,pixel_size,slice_width, $
	x_size,y_size,mats,specs,index,maxindex,maximage,nx,ny,ntot

common shsc2, shsbase,message,history,nameid,draw,matlist,win_num, $
	next,previous,bthr,old_bthr
;
	widget_control, next,/sensitive
	widget_control, previous,/sensitive
	if(index eq maxindex) then widget_control, next,sensitive=0
	if(index eq 0) then widget_control, previous,sensitive=0
	wset,win_num
	erase
	xs = nx*x_size
	ys = ny*y_size
	for i=index,maxindex do begin
		f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
		label=''
		if(nf gt 0) then label = label + strcompress(' f '+ string(f))
		if(np gt 0) then label = label + strcompress(' p '+ string(p))
		if(ng gt 0) then label = label + strcompress(' g '+ string(g))
		if(nb gt 0) then label = label + strcompress(' b '+ string(b))
		tvscl, matrix_data(*,*,i)>bthr - bthr, i-index
		xpos =  ((i-index) mod nx )*x_size
		ypos =  ys -(((i-index)/nx)*y_size) - 25
		;device,set_graphics=7
		xyouts,xpos,ypos,label,/device
		;device,set_graphics=3
	end
	;device,set_graphics=7
	xyouts,250,500,matfile,/device
	;device,set_graphics=3
end

pro Showstudy_event, event

common 	shsc1, nf,np,ng,nb,matrix_data,matfile,f,p,g,b, $
	plane_sep,sixel_size,pixel_size,slice_width, $
	x_size,y_size,mats,specs,index,maxindex,maximage,nx,ny,ntot

common shsc2, shsbase,message,history,nameid,draw,matlist,win_num, $
	next,previous,bthr,old_bthr

common colors,	r_orig,g_orig,b_orig,r_curr,g_curr,b_curr

ON_ERROR, 2

WIDGET_CONTROL, event.id, GET_UVALUE = eventval

lf = "\x0a" 
CASE eventval OF
    'FILENAME': 

    'SAVEPS': BEGIN
		WIDGET_CONTROL, message,SET_VALUE = " Now creating Postscript, be patient ..."
	      	WIDGET_CONTROL, nameid, GET_VALUE = psfile
		wset,win_num
		vue=tvrd()
		vmin=min(vue)
	        vmax=max(vue)
		vue (where(vue eq vmax)) = 1			; max to 1
		vue (where(vue eq vmin)) = vmax			; 0 to max (black to white)
		r_orig = r_curr & g_orig = g_curr & b_orig = b_curr
		r_curr(!d.n_colors-1) =255 & g_curr(!d.n_colors-1) =255 & b_curr(!d.n_colors-1) =255
		tvlct,r_curr,g_curr,b_curr
		set_plot,'PS'
		device,/color,BITS_PER_PIXEL=8,filename=psfile(0),/landscape
		tv,vue
		device,/close
		set_plot,getenv('IMSLIDL_DEVICE')
		tvlct,r_orig,g_orig,b_orig
		WIDGET_CONTROL, message,SET_VALUE = " Ready "
		info =  ' => Postscript file created :' + psfile + lf
		WIDGET_CONTROL, history,SET_VALUE =  info,/append
		vue=0
	     END
 
   'PREVIEW': BEGIN
		WIDGET_CONTROL, message,SET_VALUE = " Now simulating Postscript, be patient ..."
		wset,win_num
		vue=tvrd()
		vmin=min(vue)
	        vmax=max(vue)
		vue (where(vue eq vmax)) = 1			; max to 1
		vue (where(vue eq vmin)) = vmax			; 0 to max (black to white)
		r_orig = r_curr & g_orig = g_curr & b_orig = b_curr
		r_curr(!d.n_colors-1) =255 & g_curr(!d.n_colors-1) =255 & b_curr(!d.n_colors-1) =255
		tvlct,r_curr,g_curr,b_curr
		tv,vue
		WIDGET_CONTROL, message,SET_VALUE = " PS result shown during 5 seconds ..."
		wait,5
		tvlct,r_orig,g_orig,b_orig
		vue=0
	        display_matrix
		WIDGET_CONTROL, message,SET_VALUE = " Ready "
	     END

    'CLUT' : xloadct, GROUP = event.top

   'LOAD': begin
	     nx=6 & ny=5		; 30 matrixes per page
	     read_matrix
	     if  (matfile eq '') then return
	     index=0
	     display_matrix
	     WIDGET_CONTROL, message,SET_VALUE = " Ready "
	   end  

   'MOVIE': begin
		numframes= ntot - index
		if(numframes lt 2) then return
		xinteranimate,set=[x_size,y_size,numframes],title='Movie',/showload
		for i=0,numframes-1 do begin
		  k=i+index
 		  xinteranimate,frame=i,image=matrix_data(*,*,k) ; slower but allows to load undisplayed data
		end
		xinteranimate,20		; does not work at speed 20 although the widget is correct bug ???
	    end

   'BIAS': begin
            xvaredit,bthr,name='Bias threshold [0-255]'
	    old_bthr = bthr
	    display_matrix
	    info = ' Bias performed with threshold at '+ string(bthr)            
            WIDGET_CONTROL, history, set_value = info,/append
	   end

   'REBIN': begin
	    x_size=x_size/2
	    y_size=y_size/2
	    nx=2*nx & ny=2*ny
	    WIDGET_CONTROL, message, SET_VALUE = " Rebinning data, please be patient ..."
	    tmp = bytarr(x_size,y_size,ntot)
	    for i =0, ntot-1 do begin
	    	tmp(*,*,i) = rebin(matrix_data(*,*,i),x_size,y_size)
	    end
	    matrix_data = tmp
	    index=0
	    display_matrix
	    info = ' Data rebinned '+ string(x_size)+'x'+ string(y_size)          
            WIDGET_CONTROL, history, set_value = info,/append
	    WIDGET_CONTROL, message,SET_VALUE = " Ready "
	   end

   'NEXT': begin
	    index=index+1
	    display_matrix
	   end

   'PREVIOUS': begin
	  index=index-1
	  display_matrix
	end

   'SELECT': begin
	      previndex = index
	      index=event.index
	      if (index eq previndex) then return
	      display_matrix
	   end

   'DONE': begin
		wdelete,win_num
		WIDGET_CONTROL, event.top, /DESTROY
	   end

ENDCASE

END


PRO Showstudy, GROUP=GROUP

common 	shsc1, nf,np,ng,nb,matrix_data,matfile,f,p,g,b, $
	plane_sep,sixel_size,pixel_size,slice_width, $
	x_size,y_size,mats,specs,index,maxindex,maximage,nx,ny,ntot

common shsc2, shsbase,message,history,nameid,draw,matlist,win_num, $
	next,previous,bthr,old_bthr

ON_ERROR, 2

nx=6 & ny=5		; 30 matrixes per page
bthr = 10

psfile='study.ps'
shsbase = WIDGET_BASE(TITLE = 'Simple Image & Scan Viewer',/COLUMN)
shscntrl1 = widget_base(shsbase,/row)
load = WIDGET_BUTTON(shscntrl1, VALUE = "Load Study", UVALUE = 'LOAD')
color = WIDGET_BUTTON(shscntrl1, VALUE = "Colors", UVALUE = 'CLUT')
preview = WIDGET_BUTTON(shscntrl1, VALUE = "Preview", UVALUE = 'PREVIEW')
postscript = WIDGET_BUTTON(shscntrl1, VALUE = "Postscript", UVALUE = 'SAVEPS')
done = WIDGET_BUTTON(shscntrl1, VALUE = "Exit", UVALUE = 'DONE')

shscntrl2 = widget_base(shsbase,/row)
previous = WIDGET_BUTTON(shscntrl2, VALUE = "<<", UVALUE = 'PREVIOUS')
next = WIDGET_BUTTON(shscntrl2, VALUE = ">>", UVALUE = 'NEXT')
rebin = WIDGET_BUTTON(shscntrl2, VALUE = "Compress", UVALUE = 'REBIN')
bias = WIDGET_BUTTON(shscntrl2, VALUE = "Bias", UVALUE = 'BIAS')
movie = WIDGET_BUTTON(shscntrl2, VALUE = "Movie", UVALUE = 'MOVIE')

status = WIDGET_BASE(shsbase, /ROW, XPAD = 10, SPACE = 30)
statuslabel = WIDGET_LABEL(status, VALUE = " Status ")
message = WIDGET_LABEL(status, VALUE = " Starting Showstudy ... ")
history = WIDGET_TEXT(shsbase, VALUE = 'History ....', xsize=70,YSIZE = 8,/scroll,/FRAME)
save = WIDGET_BASE(shsbase, /ROW, /frame)
label = WIDGET_LABEL(save, VALUE = " Save screen as ")
nameid = WIDGET_TEXT(save, VALUE = psfile, XSIZE = 40, YSIZE = 1,/EDITABLE, UVALUE = 'FILENAME')
matlabel = WIDGET_LABEL(shsbase, VALUE = "Select starting matrix or use << and >> buttons")
matlist = WIDGET_LIST(shsbase,VALUE = '',UVALUE='SELECT',YSIZE = 5)
WIDGET_CONTROL, shsbase, /REALIZE
read_matrix,/new
if  (matfile eq '') then return
index=0
display_matrix
WIDGET_CONTROL, message,SET_VALUE = " Ready "
XMANAGER, "Showstudy", shsbase, GROUP_LEADER=GROUP

END
