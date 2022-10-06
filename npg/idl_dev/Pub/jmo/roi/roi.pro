;**********************
function get_plnfrm,dim
;**********************

;Returns planes(2). planes(1) is minimum, planes(2) = maximum

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path,regpath,$
       wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel,wsave,wload, $
       wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype,image_index, $
       plot_index,woprof,lcol,wdone,wslicer,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       wtiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd

common getpln_comm,wpln_min,wpln_max,wok,plnfrm_sel,wfrm,maxfrm,wdat,maxdata,mindata,maxbed,wbed

maxpln = dim(2)
maxfrm = dim(3)
if(imgtype eq !CTI) then begin
    mindata = dim(4)
    maxdata = dim(5)
endif else begin
    mindata = 0
    maxdata = 0
endelse
maxbed = dim(6)

plnfrm_sel = intarr(6)
plnfrm_sel(*) = 0
getpln = widget_base(title='Planes',/COLUMN)

;if(imgtype eq !CTI) then min = 1 else min = 0
min = 1
max = maxpln+min-1
if(min eq max) then max = max - 1
wpln_min = widget_slider(getpln,Title='Starting Plane', $
					MINIMUM=min,MAXIMUM=max)
wpln_max = widget_slider(getpln,Title='Ending Plane', $
			MINIMUM=min,MAXIMUM=max,VALUE=max)
if(imgtype eq !CTI) then begin
    if (maxfrm gt 1) then wfrm = widget_slider(getpln,Title='Frame', $
					MINIMUM=1,MAXIMUM=maxfrm,VALUE=1)
    if (maxdata gt mindata) then wdat = widget_slider(getpln,Title='Data', $
					MINIMUM=mindata,MAXIMUM=maxdata,VALUE=1)
    if (maxbed gt 0) then wbed = widget_slider(getpln,Title='Bed Position', $
					MINIMUM=0,MAXIMUM=maxbed,VALUE=0)
endif
wok = widget_button(getpln,value='OK',uvalue='done')

widget_control,getpln,/REALIZE
xmanager,'get_plnfrm',getpln,/MODAL

return,plnfrm_sel
end

;**********************
pro get_plnfrm_event,ev
;**********************

common roi_comm

common getpln_comm

if(ev.id eq wok) then  begin
    widget_control,wpln_min,GET_VALUE=min
    widget_control,wpln_max,GET_VALUE=max
    if(max lt 0) then max = 0
    plnfrm_sel(0) = min
    plnfrm_sel(1) = max
    if(imgtype eq !CTI and (maxfrm gt 1)) then begin
	widget_control,wfrm,GET_VALUE=frm
	plnfrm_sel(2) = frm
    endif else begin
	plnfrm_sel(2) = 1
    endelse
    if(imgtype eq !CTI and (maxbed gt 0)) then begin
	widget_control,wbed,GET_VALUE=bed
	plnfrm_sel(4) = bed
    endif else begin
	plnfrm_sel(4) = 0
    endelse
    if(imgtype eq !CTI and (maxdata gt mindata)) then begin
	widget_control,wdat,GET_VALUE=ndata
	plnfrm_sel(3) = ndata
    endif else begin
	plnfrm_sel(3) = mindata
    endelse
    widget_control,ev.top,/DESTROY
endif

end

;************************************************
function get_str,number,labels,values,WIDTH=width
;************************************************

common getint_comm,num,strs,intbase

common roi_comm

intbase = widget_base(/COLUMN)

num = number
strs = strarr(num)
if(keyword_set(width)) then xsize = width else xsize = 15
for i=0,num-1 do strs(i) = values(i)
case number of

    1: begin
	wd.getint1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    2: begin
	wd.getint1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	wd.getint2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    3: begin
	wd.getint1 = cw_field(intbase,TITLE=labels(0),VALUE=values(0), $
					XSIZE=xsize,/ALL_EVENTS)
	wd.getint2 = cw_field(intbase,TITLE=labels(1),VALUE=values(1), $
					XSIZE=xsize,/ALL_EVENTS)
	wd.getint3 = cw_field(intbase,TITLE=labels(2),VALUE=values(2), $
					XSIZE=xsize,/ALL_EVENTS)
       end

    else: begin
	widget_control,werror,SET_VALUE=string("Too many integer requeste, get_str")
	return,0
       end
    endcase

wd.getintok = widget_button(intbase,value='OK')

widget_control,intbase,/REALIZE
xmanager,'get_str',intbase,/MODAL

return,strs
end

;********************
pro get_str_event,ev
;********************

common getint_comm,num,strs,intbase

common roi_comm

case ev.id of

    wd.getint1: strs(0) = ev.value

    wd.getint2: strs(1) = ev.value

    wd.getint3: strs(2) = ev.value

    wd.getintok: widget_control,ev.top,/DESTROY

    else: widget_control,werror, $
			SET_VALUE=string("Invalid event in get_str_event")

endcase

return
end

;***********************************
function get_true_false,title,labels
;***********************************

common getint_comm,num,strs,intbase

common roi_comm

intbase = widget_base(/COLUMN)
wlabel = widget_base(intbase,/COLUMN)
buttons = widget_base(intbase,/ROW)

wroinum   = widget_label(wlabel,value=title)
wd.get_true = widget_button(buttons,VALUE=labels(0))
wd.get_false = widget_button(buttons,VALUE=labels(1))

widget_control,intbase,/REALIZE
xmanager,'get_true_false',intbase,/MODAL

return,strs
end

;**************************
pro get_true_false_event,ev
;**************************

common getint_comm,num,strs,intbase

common roi_comm

strs = !TRUE
case ev.id of

    wd.get_true: strs = !TRUE

    wd.get_false: strs = !FALSE

    else: widget_control,werror, $
			SET_VALUE=string("Invalid event in get_str_event")

endcase

widget_control,ev.top,/DESTROY

return
end

;***********************
function emis_mask,image
;***********************

common roi_comm

max = max(image)
x = image gt max/4
s=replicate(1,3,3)
c = dilate(erode(x,s),s)
spawn,string('ls ',!SHARE_LIB),result
test = strpos(result,!SHARE_LIB)
if(test(0) eq 0) then $
    stat=call_external(!SHARE_LIB,'_emis_mask',c,n_elements(c(*,0)),n_elements(c(0,*))) $
else $
    widget_control,werror,SET_VALUE=string('Shared library ',!SHARE_LIB,' not found.')

mask = float(c)

return,mask
end

;***********************************************
function reslice,image,xdim,ydim,zdim,ydout,styp
;***********************************************

;styp: type of reslicing, "sag" or "cor".

if(styp eq 'sag') then type = 1 else type = 2

imgout = fltarr(xdim,ydout,ydim)

passtype = bytarr(7)
passtype(2:6) = 1
stat=call_external(!SHARE_LIB,'_reslice',image,imgout,xdim,ydim,zdim, $
					ydout,type,/F_VALUE,VALUE=passtype)

return,imgout
end

;*****************
function osf2sun,x
;*****************

;Convert x from alpha float (64-bit) to Sun float (32 bit)


len = n_elements(x)
imgout = dblarr(len)

passtype = bytarr(3)
passtype(2) = 1
stat=call_external(!SHARE_LIB,'_osf2sun',x,imgout,len,/F_VALUE,VALUE=passtype)

x = float(imgout)

return,x
end

;****************************************
function get_image,listname,filename,stat
;****************************************

common roi_comm

common images,a,b,c,d,e,f,g,h,ii,jj,kk,ll,mm,nn,oo,pp, $
	file_names,file_onlies,paths,nfiles,wrtpath, $
	current_file,nblt,bltimg,list_names,tophand,handle_ids

stat = 1
ifile = -1
for i=0,nfiles-1 do begin
    if(listname eq list_names(i)) then begin
	current_file = i
	i = !NUM_FILES-1
    endif
endfor

filename = file_names(current_file)

if(current_file lt 0) or (current_file ge !NUM_FILES) or (nfiles lt 1) then begin
    widget_control,werror,SET_VALUE=string('Invalid file designated.')
    stat = 0
    return,0
endif

handle_value,handle_ids(current_file),imgdat

if((imgdat.scl gt 1) or (imgdat.scl lt 1)) then $
    imgdat.img = imgdat.img*imgdat.scl

if(view eq !TRANSVERSE) then $
    return,imgdat

if(view eq !SAGITAL) then begin
    widget_control,/HOURGLASS
    erase
    xd = imgdat.ydim
    yd = imgdat.zmax
    zm = imgdat.xdim
    spawn,string('ls ',!SHARE_LIB),result
    test = strpos(result,!SHARE_LIB)
    if(test(0) ne 0) then begin
        widget_control,werror,SET_VALUE=string('Shared library ',$
						!SHARE_LIB,' not found.')
        jimg = fltarr(xd,yd,zm)
        for i=0,xd-1 do begin
            for j=0,yd-1 do begin
    	        jimg(i,j,*) = imgdat.img(*,i,j)
	    endfor
        endfor
        jimg = congrid(jimg,xd,yd,zm)
    endif else begin
        ydout = fix(float(yd)*imgdat.dz/imgdat.dxdy)
        jimg = reslice(imgdat.img,imgdat.xdim,imgdat.ydim,imgdat.zmax,ydout(0),'sag')
    endelse
    xmg = reverse(jimg)
    jimgdat = {name:'',xdim:xd,ydim:ydout(0),zmax:zm,zmin:0,img:xmg(*,*,*), $
				dxdy:imgdat.dxdy,dz:imgdat.dz,scl:1.}
    widget_control,HOURGLASS=0
    return,jimgdat
endif

if(view eq !CORONAL) then begin
    widget_control,/HOURGLASS
    erase
    xd = imgdat.xdim
    yd = imgdat.zmax
    zm = imgdat.ydim
    spawn,string('ls ',!SHARE_LIB),result
    test = strpos(result,!SHARE_LIB)
test(0) = 0
print,'Dummied up reslice.'
    if(test(0) ne 0) then begin
        widget_control,werror,SET_VALUE=string('Shared library ',$
						!SHARE_LIB,' not found.')
        jimg = fltarr(xd,yd,zm)
        for i=0,xd-1 do begin
            for j=0,yd-1 do begin
    	        jimg(i,j,*) = imgdat.img(i,*,j)
    	    endfor
        endfor
;        jimg = congrid(jimg,xd,yd,zm)
	ydout = yd
    endif else begin
        ydout = fix(float(yd)*imgdat.dz/imgdat.dxdy)
        jimg = reslice(imgdat.img,imgdat.xdim,imgdat.ydim,imgdat.zmax, $
								ydout(0),'cor')
    endelse
    jimgdat = {name:'',xdim:xd,ydim:yd,zmax:zm,zmin:0,img:jimg(*,*,*), $
					dxdy:imgdat.dxdy,dz:imgdat.dz,scl:1.}
    widget_control,HOURGLASS=0
    return,jimgdat
endif

end

;********************************************************************
pro put_image,image,xdim,ydim,minpln,maxpln,listname,filnam,xdxdy,xdz
;********************************************************************

common roi_comm

common images

if(nfiles ge !NUM_FILES-1) then begin
    widget_control,werror,SET_VALUE=string('Too many files opened.')
    return
endif

xx = {name:'',xdim:xdim(0),ydim:ydim(0),zmax:maxpln(0),zmin:minpln(0), $
					img:image,dxdy:xdxdy,dz:xdz,scl:1.}

handle_ids(nfiles) = handle_create(tophand,VALUE=xx)

cmd = string('set y = ',filnam,'; echo $y:t')
spawn,cmd,fileonly
cmd = string('set y = ',filnam,'; echo $y:h')
spawn,cmd,lpath
list_names(nfiles) = listname
file_names(nfiles) = filnam
file_onlies(nfiles) = fileonly
paths(nfiles) = lpath
current_file = nfiles
nfiles = nfiles + 1

widget_control,wfiles,SET_VALUE=list_names

return
end

;************************
pro put_scale,ifile,scale
;************************

common roi_comm

common images

if(ifile lt 0) or (ifile ge !NUM_FILES-1) then begin
    widget_control,werror,SET_VALUE=string('Too many files opened.')
    return
endif

handle_value,handle_ids(ifile),xx
xx.scl = scale

return
end

;****************
pro math_event,ev
;****************

common roi_comm

common images

common math_comm,wa,wb,wdd,wc,wsum,wdif,wmlt,wdiv,wmfile,wmdone,geta,getb,getc, $
	wlaba,wlabb,wlabc,afil,bfil,cfil,dfil,wsmo,wabs,wlog,wval,mathval, $
	wexp,wscl,wrebin,wpad,wclip,wscatcor,wimgdiv,getd,wlabd

case ev.id of

    wmdone: widget_control,ev.top,/DESTROY

    wa: begin
        geta = !TRUE
	widget_control,wa,SENSITIVE=0
    end

    wb: begin
        getb = !TRUE
	widget_control,wb,SENSITIVE=0
    end

    wc: begin
        getc = !TRUE
	widget_control,wc,SENSITIVE=0
    end

    wdd: begin
        getd = !TRUE
	widget_control,wdd,SENSITIVE=0
    end

    wmfile: begin
	if(geta eq !TRUE) then begin
	    geta = !FALSE
            afil = ev.index
	    widget_control,wlaba,SET_VALUE=list_names(afil)
	    widget_control,wa,/SENSITIVE
        endif
	if(getb eq !TRUE) then begin
	    getb = !FALSE
            bfil = ev.index
	    widget_control,wlabb,SET_VALUE=list_names(bfil)
	    widget_control,wb,/SENSITIVE
        endif
	if(getc eq !TRUE) then begin
	    getc = !FALSE
            cfil = ev.index
	    widget_control,wlabc,SET_VALUE=list_names(cfil)
	    widget_control,wc,/SENSITIVE
        endif
	if(getd eq !TRUE) then begin
	    getd = !FALSE
            dfil = ev.index
	    widget_control,wlabd,SET_VALUE=list_names(dfil)
	    widget_control,wdd,/SENSITIVE
        endif
    end

    wdif: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	cimage = aimg.img - bimg.img
	if(stat eq 0) then return
	listname = strcompress(string(list_names(afil),'-',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wsum: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	cimage = aimg.img + bimg.img
	listname = strcompress(string(list_names(afil),'+',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wmlt: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	cimage = aimg.img * bimg.img
	listname = strcompress(string(list_names(afil),'_mlt_',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wdiv: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	aimg.img = smooth(aimg.img,3)
	bimg.img = smooth(bimg.img,3)
	zdim = n_elements(aimg.img(0,0,*))
        label = strarr(2)
        label(0) = string('Mask')
        label(1) = string("Don't Mask")
        title = string('Mask images before dividing?')
        lcmask = get_true_false(title,label)
	if(lcmask eq !TRUE) then begin
            for pln=0,zdim-1 do begin
	        mask = emis_mask(aimg.img(*,*,pln))
	        aimg.img(*,*,pln) = mask*aimg.img(*,*,pln)
	        mask = emis_mask(bimg.img(*,*,pln))
	        bimg.img(*,*,pln) = mask*bimg.img(*,*,pln)
	    endfor
	endif
	denset = 1.e*20*(bimg.img eq 0.)
	cimage = aimg.img / (bimg.img + denset)
	listname = strcompress(string(list_names(afil),'_div_',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wnorm: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	zdim = n_elements(aimg.img(0,0,*))
        label = strarr(2)
        label(0) = string('Smooth')
        label(1) = string("Don't Smooth")
        title = string('Smooth images before normalizing?')
        lcsmoth = get_true_false(title,label)
	if(lcsmoth eq !TRUE) then begin
            widget_control,/HOURGLASS
	    img = smooth(aimg.img,3)
	    img = smooth(img,3)
	    bimg.img = smooth(bimg.img,3)
	    bimg.img = smooth(bimg.img,3)
            widget_control,HOURGLASS=0
	endif else begin
	    img = aimg.img
	endelse
	suma = 0
	sumb = 0
        label = strarr(2)
        label(0) = string('Mask')
        label(1) = string("Don't Mask")
        title = string('Mask images before normalizing?')
        lcmask = get_true_false(title,label)
	if(lcmask eq !TRUE) then begin
            for pln=0,zdim-1 do begin
                widget_control,/HOURGLASS
	        mask = emis_mask(img(*,*,pln))
	        suma = suma + total(img(*,*,pln)*mask)
	        mask = emis_mask(bimg.img(*,*,pln))
	        sumb = sumb + total(bimg.img(*,*,pln)*mask)
                widget_control,HOURGLASS=0
	    endfor
	endif else begin
	    suma = total(img)
	    sumb = total(bimg.img)
	endelse
        widget_control,/HOURGLASS
	scl = sumb/suma
	print,'Scale factor: ',scl
	img = scl*aimg.img
	listname = strcompress(string(list_names(afil),'_norm_to_',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,img,n_elements(img(*,0,0)),n_elements(img(0,*,0)), $
		  aimg.zmin,n_elements(img(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wrebin: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	xdima = n_elements(aimg.img(*,0,0))
	ydima = n_elements(aimg.img(0,*,0))
	zdim = n_elements(aimg.img(0,0,*))
	value = 1
	label = string("Factor to shrink or expand image: ")
	scl = float(get_str(1,label,value))
	xdimb = 2*(fix(scl*xdima)/2)
	ydimb = 2*(fix(scl*ydima)/2)
        cimage = fltarr(xdima(0),ydima(0),zdim(0))
	xpad = (xdima(0) - xdimb(0))/2
	ypad = (ydima(0) - ydimb(0))/2
	for z=0,zdim(0)-1 do begin
	    img = rebin(aimg.img(*,*,z),xdimb(0),ydimb(0))
 	    cimage(xpad(0):xpad(0)+xdimb(0)-1,ypad(0):ypad(0)+ydimb(0)-1,z) = img(*,*)
	endfor
	listname = strcompress(string(list_names(afil),"_rebin_",scl),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wd.axialavg: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	xdima = n_elements(aimg.img(*,0,0))
	ydima = n_elements(aimg.img(0,*,0))
	zdim = n_elements(aimg.img(0,0,*))
	cimage = fltarr(xdima,ydima,2)
        cimage(*,*,0) = aimg.img(*,*,0)
	for z=1,zdim-1 do begin
            cimage(*,*) = cimage(*,*) + aimg.img(*,*,z)
	endfor
	cimage(*,*,1) = cimage(*,*,0)
	listname = strcompress(string(list_names(afil),"_axial_avg"),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wd.flip: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	cimage = reverse(aimg.img)
	listname = strcompress(string(list_names(afil),"flip"),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wpad: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	zdim = n_elements(aimg.img(0,0,*))
	xdima = n_elements(aimg.img(*,0,0))
	ydima = n_elements(aimg.img(0,*,0))
	value = intarr(2)
	label = strarr(2)
	value(0) = xdima
	value(1) = ydima
	label(0) = string("New X dimension: ")
	label(1) = string("New Y dimension: ")
	dims = get_str(2,label,value)
	xdimb = fix(dims(0))
	ydimb = fix(dims(1))
        widget_control,/HOURGLASS
        bimage = fltarr(xdimb(0),ydima(0),zdim)
        cimage = fltarr(xdimb(0),ydimb(0),zdim)
	xpad = (xdimb(0) - xdima)/2
	ypad = (ydimb(0) - ydima)/2
	if(xpad gt 0) then begin
	    bimage(xpad:xpad+xdima-1,*,*) = aimg.img(*,*,*)
	endif else begin
	    xpad = -xpad
	    bimage(*,*,*) = aimg.img(xpad:xpad+xdimb(0)-1,*,*)
	endelse
	if(ypad gt 0) then begin
	    cimage(*,ypad:ypad+ydima-1,*) = bimage(*,*,*)
	endif else begin
	    ypad = -ypad
	    cimage(*,*,*) = bimage(*,ypad:ypad+ydimb(0)-1,*)
	endelse
	listname = strcompress(string(list_names(afil),'_pad',xdimb(0),'x',ydimb(0)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,xdimb(0),ydimb(0), $
		  aimg.zmin,aimg.zmax,listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wclip: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	zdim = n_elements(aimg.img(0,0,*))
	xdima = n_elements(aimg.img(*,0,0))
	ydima = n_elements(aimg.img(0,*,0))
	value = 0.
	label = string("Clip value: ")
	clip = float(get_str(1,label,value))
        label = strarr(2)
        label(0) = string('Upper')
        label(1) = string("Lower")
        title = string('Type of threshold?')
        lcthresh = get_true_false(title,label)
        widget_control,/HOURGLASS
	if(lcthresh eq !TRUE) then $
	    aimg.img = aimg.img < clip(0) $
	else $
	    aimg.img = aimg.img > clip(0)
	listname = strcompress(string(list_names(afil),'_clip_',clip(0)),/REMOVE_ALL)
	fileonly = listname
	put_image,aimg.img,xdima,ydima, $
		  aimg.zmin,aimg.zmax,listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wd.pln2sino: begin
	aimg = get_image(list_names(afil),fildummy,stat)
        widget_control,/HOURGLASS
	sino = fltarr(aimg.xdim,aimg.zmax,aimg.ydim)

        for z=0,aimg.zmax-1 do begin
   	    for y=0,aimg.ydim-1 do begin
		sino(*,z,y) = aimg.img(*,y,z)
	    endfor
	endfor
	listname = strcompress(string(list_names(afil),'_sino'),/REMOVE_ALL)
	put_image,sino,aimg.xdim,aimg.zmax, $
		  1,aimg.ydim,listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wscatcor: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	print,list_names(bfil)
	value = 1.
	label = string("Clip at: ")
  	thresh = float(get_str(1,label,value))
        widget_control,/HOURGLASS
	numray = n_elements(aimg.img(*,0,0))
	numang = n_elements(aimg.img(0,*,0))
	zdim   = n_elements(aimg.img(0,0,*))
	data = aimg.img(*,*,0:zdim-1) < thresh(0)
	numray_scat = n_elements(bimg.img(*,0,0))
	pad = (numray - numray_scat)/2
        if(pad(0) gt 0) then begin
	    scat = fltarr(numray,numang,zdim)
            scat(pad(0):pad(0)+numray_scat-1,*,*) = bimg.img(*,*,*)
	    data(0:pad-1,*,*) = 0.
	    data(numray-pad:numray-1,*,*) = 0.
        endif else begin
	    scat = bimg.img
	    data = data * (scat gt 0)
        endelse
	scatcor,data,scat,thresh,corr
        label = strarr(2)
        label(0) = string('Save')
        label(1) = string("Don't Save")
        title = string('Save stats?')
        lcsave = get_true_false(title,label)
	if(lcsave eq !TRUE) then begin
 	    file1 = string("data_",list_names(afil))
            put_image,data,numray,numang,1,zdim,file1,file1,1,1
;	    file1 = string("scat_",list_names(bfil))
;	    put_image,scat,numray,numang,1,zdim,file1,file1,1,1
	    file1 = string("corr_",list_names(bfil))
	    corr = rebin(corr,numray,numang,zdim)
	    corr(*,*,*) = corr(*,*,*)/8
	    put_image,corr,numray,numang,1,zdim,file1,file1,1,1
	endif
        widget_control,HOURGLASS=0
    end

    wd.scatmc: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
;	cimg = get_image(list_names(cfil),fildummy,stat)
	if(stat eq 0) then return
	dimg = get_image(list_names(cfil+1),fildummy,stat)
	if(stat eq 0) then return
	eimg = get_image(list_names(cfil+3),fildummy,stat)
	if(stat eq 0) then return
	fimg = get_image(list_names(dfil),fildummy,stat)
	if(stat eq 0) then return
	value = 1.
	label = string("Clip at: ")
  	thresh = float(get_str(1,label,value))
        widget_control,/HOURGLASS
	numray_data = n_elements(aimg.img(*,0,0))
	numray = n_elements(bimg.img(*,0,0))
	numray_mc = n_elements(dimg.img(*,0,0))
	numang = n_elements(aimg.img(0,*,0))
	zdim   = n_elements(aimg.img(0,0,*))
	pad = (numray_data - numray)/2
	data = fltarr(numray,numang,zdim)
        data = aimg.img(pad:pad+numray-1,*,*)
	scat = bimg.img

        label = strarr(2)
        label(0) = string('Denormalize')
        label(1) = string("Estimated Flux")
        title = string('Denormalize and scale or use flux?')
        lcdenorm = get_true_false(title,label)
	pad = (numray - numray_mc)/2
	if(lcdenorm eq !TRUE) then begin
	    nrmfil = pickfile(/read,FILTER='*.nrm',PATH=path(0),/NOCONFIRM)
	    widget_control,/HOURGLASS
	    if(nrmfil eq '') then return
            rd_cti,nrmfil,stat,norm,xdim,ydim,zzdim,dxdy,scale_factor, $
	        		MIN_PLANE=1,MAX_PLANE=zdim,FRAME=1,DATA=0
	    widget_control,/HOURGLASS
	    mcscat = fltarr(numray,numang,zdim)
	    mcmult = fltarr(numray,numang,zdim)
	    mcscat(pad:numray_mc+pad-1,*,*) = dimg.img
	    mcmult(pad:numray_mc+pad-1,*,*) = eimg.img
	    for z=0,zdim-1 do begin
                mcscat(*,*,z) = scale_factor(z)*mcscat(*,*,z)/norm(*,*,z)
                mcmult(*,*,z) = scale_factor(z)*mcmult(*,*,z)/norm(*,*,z)
                scat(*,*,z) = scale_factor(z)*scat(*,*,z)/norm(*,*,z)
	    endfor
	    scl = total(scat(16:143,*,*))/total(mcscat) ;Scale total events.
	    mcscat(*,*,*) = scl*mcscat(*,*,*)
	    mcmult(*,*,*) = scl*mcmult(*,*,*)
	    print,"Scale factor: ",scl
 	    file1 = string("denorm_mcscat_",list_names(bfil))
            put_image,mcscat,xdim(0),ydim(0),1,zdim,file1,file1,1,1
 	    file1 = string("denorm_mult_",list_names(bfil))
            put_image,mcmult,xdim(0),ydim(0),1,zdim,file1,file1,1,1
 	    file1 = string("denorm_scat_",list_names(bfil))
            put_image,scat,xdim(0),ydim(0),1,zdim,file1,file1,1,1
	    put_scale,cfil,scl
	    put_scale,cfil+1,scl
	    put_scale,cfil+2,scl
	    put_scale,cfil+3,scl
	    print,"Error between data and scatter esimate."
	    scatcor,data,scat,thresh,corr
	    print,"Error between data and Monte Carlo."
            data(0:15,*,*) = 0
            data(144:159,*,*) = 0
	    scatcor,data,mcscat,thresh,corr
	    print,"Error between Scatter estimate and Monte Carlo."
            scat(0:15,*,*) = 0
            scat(144:159,*,*) = 0
	    scatcor,scat,mcscat,thresh,corr
	endif else begin
	    widget_control,/HOURGLASS
	    scl = total(scat(pad:pad+numray_mc-1,*,*))/total(dimg.img)
	    put_scale,cfil,scl
	    put_scale,cfil+1,scl
	    put_scale,cfil+2,scl
	    put_scale,cfil+3,scl
	    print,"Monte Carlo scale factor: ",scl
	    pad = (numray - numray_mc)/2
	    data = aimg.img(pad:pad+numray_mc-1,*,*)
	    scat = bimg.img(pad:pad+numray_mc-1,*,*)
	    mcscat = scl*dimg.img
	    mcmult = scl*eimg.img
	    mult = fimg.img(pad:pad+numray_mc-1,*,*)
	    mask = data gt thresh(0)
	    dil = replicate(1,11)
	    for pln=0,zdim-1 do begin
   		for y=0,numang-1 do begin
		    mask(*,y,pln) = dilate(mask(*,y,pln),dil)
		endfor
	    endfor
	    mask(*,*,*) = 1. - mask(*,*,*)
	    xdsamp = 8
	    ydsamp = 16
	    multdiff = mult - mcmult
	    scatdiff = scat - mcscat
	    scatdatdiff = mask*(scat - data)
	    mcdatdiff = mask*(mcscat - data)
	    num = numray_mc - (numray_mc*numang*zdim-total(mask))/(zdim*numang)/xdsamp
	    openw,lurms,'rms.txt',/GET_LUN
	    for pln=0,zdim-1 do begin
   		for yy=0,numang/ydsamp-1 do begin
		    y = yy*ydsamp
		    dmult = 0.
		    dscat = 0.
		    dscatdat = 0.
		    dmcdat = 0.
	            for xx=0,numray_mc/xdsamp-1 do begin
		        x = xx*xdsamp
			dmult = dmult + total(multdiff(x:x+xdsamp-1,y:y+ydsamp-1,pln))^2
			dscat = dscat + total(scatdiff(x:x+xdsamp-1,y:y+ydsamp-1,pln))^2
			dscatdat = dscatdat + total(scatdatdiff(x:x+xdsamp-1,y:y+ydsamp-1,pln))^2
			dmcdat = dmcdat + total(mcdatdiff(x:x+xdsamp-1,y:y+ydsamp-1,pln))^2
		    endfor
		    rmult = 100.*sqrt(dmult*numray_mc/xdsamp)/total(mcmult(*,y:y+ydsamp-1,pln))
		    rscat = 100.*sqrt(dscat*numray_mc/xdsamp)/total(mcscat(*,y:y+ydsamp-1,pln))
		    rscatdat = 100.*sqrt(dscatdat*num)/total(data(*,y:y+ydsamp-1,pln))
		    rmcdat = 100.*sqrt(dmcdat*num)/total(data(*,y:y+ydsamp-1,pln))
	    	    printf,lurms,pln,yy,rmult,rscat,rscatdat,rmcdat
		    print,"plane: ",pln," Angle: ",yy,rmult,rscat,rscatdat,rmcdat
		endfor
	    endfor
	    close,lurms
	    free_lun,lurms
	endelse
        widget_control,HOURGLASS=0
    end

    wd.denorm: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	zdim   = n_elements(aimg.img(0,0,*))
	nrmfil = pickfile(/read,FILTER='*.nrm',PATH=path(0),/NOCONFIRM)
	widget_control,/HOURGLASS
	if(nrmfil eq '') then return
        rd_cti,nrmfil,stat,norm,xdim,ydim,zzdim,dxdy,scale_factor, $
	        		MIN_PLANE=1,MAX_PLANE=zdim,FRAME=1,DATA=0
	widget_control,/HOURGLASS
	for z=0,zdim-1 do begin
            aimg.img(*,*,z) = scale_factor(z)*aimg.img(*,*,z)/norm(*,*,z)
	endfor
 	file1 = string("denorm_",list_names(afil))
        put_image,aimg.img,xdim(0),ydim(0),1,zdim,file1,file1,1,1
    end

    wimgdiv: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	bimg = get_image(list_names(bfil),fildummy,stat)
	if(stat eq 0) then return
	xdima = n_elements(aimg.img(*,0,0))
	ydima = n_elements(aimg.img(0,*,0))
	zdim = n_elements(aimg.img(0,0,*))
        widget_control,/HOURGLASS
	num = smooth(aimg.img,7)
	num = smooth(num,7)
	den = smooth(bimg.img,7)
	den = smooth(den,7)
        for pln=0,zdim-1 do begin
	    mask = emis_mask(num(*,*,pln))
	    num(*,*,pln) = num(*,*,pln)*mask
	    den(*,*,pln) = den(*,*,pln)*mask
	endfor
	den = den + (den le 0)
        img = num/den
        img = img > 0
	listname = strcompress(string(list_names(afil),'_divc_',list_names(bfil)),/REMOVE_ALL)
	fileonly = listname
	put_image,img,xdima,ydima, $
		  aimg.zmin,aimg.zmax,listname,listname,aimg.dxdy,aimg.dz
        widget_control,HOURGLASS=0
    end

    wabs: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	listname = strcompress(string(list_names(afil),'.abs'),/REMOVE_ALL)
	fileonly = listname
	cimage = abs(aimg.img)
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wsmo: begin
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	label = strarr(1)
	value = fltarr(1)
	label(0) = string("Width of boxcar filter: ")
	value(0) = 3
	width = fix(get_str(1,label,value))
        widget_control,/HOURGLASS
	if(2*(width(0)/2) eq width(0)) then begin
	    widget_control,werror,SET_VALUE=string('*** Filter width must be even.  ***')
	    return
	endif    
	cimage = smooth(aimg.img,width(0))
	listname = strcompress(string(list_names(afil),'.smo',width(0)),/REMOVE_ALL)
	fileonly = listname
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wlog: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	listname = strcompress(string(list_names(afil),'.log'),/REMOVE_ALL)
	fileonly = listname
	cimage = alog(aimg.img)
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wexp: begin
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	listname = strcompress(string(list_names(afil),'.exp'),/REMOVE_ALL)
	fileonly = listname
	cimage = exp(aimg.img)
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wval: begin
        widget_control,wval,GET_VALUE=smathval
 	mathval = float(smathval)
    end

    wscl: begin
        if(n_elements(mathval) eq 0) then begin
	    widget_control,werror,SET_VALUE=string('*** c is undefined.  ***')
	    return
	endif
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	listname = strcompress(string(list_names(afil),'.scl_',mathval),/REMOVE_ALL)
	fileonly = listname
	cimage = mathval(0)*aimg.img
	put_image,cimage,n_elements(cimage(*,0,0)),n_elements(cimage(0,*,0)), $
		  aimg.zmin,n_elements(cimage(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
	widget_control,wmfile,SET_VALUE=list_names
        widget_control,HOURGLASS=0
    end

    wd.threshset: begin
	label = strarr(3)
	value = fltarr(3)
	label(0) = string("Lower threshold: ")
	label(1) = string("Upper threshold: ")
	label(2) = string("Value: ")
	value(0) = 0
	value(1) = 0
	value(2) = 0
	th = float(get_str(3,label,value))
        widget_control,/HOURGLASS
	aimg = get_image(list_names(afil),fildummy,stat)
	if(stat eq 0) then return
	aimg.img = smooth(aimg.img,5)
;	Now get rid of single pixels at the edge.
	kernel = fltarr(3,3)
	kernel(0,0) = 1./16.
	kernel(0,1) = 2./16.
	kernel(0,2) = 1./16.
	kernel(1,*) = 2.*kernel(0,*)
 	kernel(2,*) = kernel(0,*)
        for pln=0,n_elements(aimg.img(0,0,*))-1 do begin
	    aimg.img(*,*,pln) = convol(aimg.img(*,*,pln),kernel,/CENTER)
	endfor
	ximg = aimg.img gt th(0)
	yimg = aimg.img lt th(1)
	aimg.img = float(ximg*yimg)*th(2)
	listname = strcompress(string(list_names(afil),'.thresh_',th(2)),/REMOVE_ALL)
	fileonly = listname
	put_image,aimg.img,n_elements(aimg.img(*,0,0)),n_elements(aimg.img(0,*,0)), $
		  aimg.zmin,n_elements(aimg.img(0,0,*)),listname,listname,aimg.dxdy,aimg.dz
        widget_control,HOURGLASS=0
    end

    else: widget_control,werror,SET_VALUE=string('*** Unknown event encountered in math_event. ***')

endcase

return
end


;*******
pro math
;*******

common roi_comm

common math_comm

common images

geta = !FALSE
getb = !FALSE
getc = !FALSE
getd = !FALSE

mathbase = widget_base(title='Image Math',/COLUMN)
mathtop  = widget_base(mathbase,/ROW)
mathtoplcol = widget_base(mathtop,/COLUMN)
mathtoprcol = widget_base(mathtop,/COLUMN)
mathmid0 = widget_base(mathbase,/ROW)
mathmid  = widget_base(mathbase,/ROW)
mathmid1 = widget_base(mathbase,/ROW)
mathmid2 = widget_base(mathbase,/ROW)
mathbot  = widget_base(mathbase,/ROW)

wa    = widget_button(mathtoplcol,value='A:')
wb    = widget_button(mathtoplcol,value='B:')
wc    = widget_button(mathtoplcol,value='C:')
wdd    = widget_button(mathtoplcol,value='D:')

if(current_file lt 0) then current_file = 0
afil = current_file
wlaba = widget_label(mathtoprcol,value=list_names(current_file))
wlabb = widget_label(mathtoprcol,value='                          ')
wlabc = widget_label(mathtoprcol,value='                          ')
wlabd = widget_label(mathtoprcol,value='                          ')

wval    = cw_field(mathmid0,TITLE='c: ',VALUE='1',/RETURN_EVENTS,XSIZE=10)
wmdone= widget_button(mathmid0,value='Done')
mathval = 1.

wsum  = widget_button(mathmid,value='A+B')
wdif  = widget_button(mathmid,value='A-B')
wmlt  = widget_button(mathmid,value='A*B')
wdiv  = widget_button(mathmid,value='A/B')
wnorm = widget_button(mathmid,value='Norm A to B ')
wrebin = widget_button(mathmid,value='Rebin A')
wd.axialavg = widget_button(mathmid,value='Axial average')
wd.flip = widget_button(mathmid,value='Flip')

wabs  = widget_button(mathmid1,value='Abs(A)')
wsmo  = widget_button(mathmid1,value='Smo(A)')
wlog  = widget_button(mathmid1,value='Log(A)')
wexp  = widget_button(mathmid1,value='Exp(A)')
wscl  = widget_button(mathmid1,value='cA')
wpad  = widget_button(mathmid1,value='Pad/Crop A')
wclip  = widget_button(mathmid1,value='Clip A')
wd.pln2sino = widget_button(mathmid1,value='Plane to sino')

wscatcor  = widget_button(mathmid2,value='A data B scat')
wimgdiv   = widget_button(mathmid2,value='A/B clipped')
wd.scatmc = widget_button(mathmid2,value='A data B scat C MC D mult')
wd.denorm = widget_button(mathmid2,value='Denormalize A')
wd.threshset = widget_button(mathmid2,value='Threshold and set A')

wmfile = widget_list(mathbot,VALUE=list_names,YSIZE=9,XSIZE=120)

widget_control,mathbase,/REALIZE
xmanager,'math',mathbase,/MODAL

return
end


;***********************
function get_profile,row
;***********************

common roi_comm

common evcomm,filnam,plane,image,xdim,ydim,maxpln,minpln,roi,nverts,nroi,vol, $
	roinum,zoom,nprof,wprtplt,wcolscl,image_minmax,blt_minmax, $
	error,wsag,wcor,mcustom,mview,filindex,st,xd,yd

device, set_graphics=6             ;Set XOR mode
cursor,x,y,/WAIT,/DEV	;Get current position.
widget_control,werror,SET_VALUE=string('Left: Select row, Right: Plot profile.')
if(horzprof eq !TRUE) then begin
   y1 = y - zoom*st.profwidth/2
   y2 = y + zoom*st.profwidth
   if(st.profwidth gt 1) then $
        row=(fix((!DRAW_SIZE-y2-1)/zoom)+fix((!DRAW_SIZE-y1-1)/zoom))/2 $
    else $
       	row = fix((!DRAW_SIZE - y1 -1)/zoom)
    widget_control,werror,SET_VALUE=string('Row: ',row)
    y1 = y - zoom*st.profwidth/2
    y2 = y1 + zoom*st.profwidth
    plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(st.profwidth gt 1) then $
        plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
    y1m1 = y1
    y2m1 = y2
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
        y1 = y - zoom*st.profwidth/2
        y2 = y + zoom*st.profwidth
	y1p = !DRAW_SIZE - y1 - 3
	y2p = !DRAW_SIZE - y2 - 3
        if(y1p/zoom lt 0) then  y1 = !DRAW_SIZE - 3
        if(y1p/zoom gt ydim(0)-1) then y1 = !DRAW_SIZE-1-zoom*(ydim(0)-1)
        if(y2p/zoom lt 0) then  y2 = !DRAW_SIZE - 3
        if(y2p/zoom gt ydim(0)-1) then y2 = !DRAW_SIZE-1-zoom*(ydim(0)-1)
        if(y1 ne y1m1) or (y2 ne y2m1) then begin
            plots,[0,!DRAW_SIZE],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then $
                plots,[0,!DRAW_SIZE],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
            plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then $
                plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
            if(st.profwidth gt 1) then $
                row=(fix((!DRAW_SIZE-y2-1)/zoom)+fix((!DRAW_SIZE-y1-1)/zoom))/2 $
	    else $
        	row = fix((!DRAW_SIZE - y1 -1)/zoom)
            widget_control,werror,SET_VALUE=string('Row: ',row)
    	    y1m1 = y1
    	    y2m1 = y2
        endif
    endrep until !err eq 4
    if(st.profwidth gt 1) then begin
        row1 = fix((!DRAW_SIZE - y2 -1)/zoom)
	if(row1 lt 0) then row1 = 0
        row2 = fix((!DRAW_SIZE - y1 -1)/zoom)
	if(row2 ge ydim(0)) then row2 = ydim(0) - 1
        row  = (row1 + row2)/2
        prof = total(image(*,row1:row2,plane),2)/(row2 - row1 + 1)
    endif else begin
        row = fix((!DRAW_SIZE - y1 -1)/zoom)
        prof = image(*,row,plane)
    endelse
endif else begin ; Vertical profile.
    x1 = x - zoom*st.profwidth/2
    x2 = x1 + zoom*st.profwidth
    if(st.profwidth gt 1) then row=(x1+x2)/(2*zoom) else row=x1/zoom
    widget_control,werror,SET_VALUE=string('Column: ',row)
    x1 = x - zoom*st.profwidth/2
    x2 = x + zoom*st.profwidth
    plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(st.profwidth gt 1) then $
        plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    x1m1 = x1
    x2m1 = x2
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
	x1 = x - zoom*st.profwidth/2
	x2 = x1 + zoom*st.profwidth
        if(x1/zoom lt 0) then x1 = 0
        if(x2/zoom lt 0) then x2 = 0
        if(x1/zoom gt xdim(0)-1) then x1 = zoom*xdim(0)
        if(x2/zoom gt xdim(0)-1) then x2 = zoom*xdim(0)
        if(x1 ne x1m1) or (x2 ne x2m1) then begin
            plots,[x1m1,x1m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(st.profwidth gt 1) then $
                plots,[x2m1,x2m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
            plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(st.profwidth gt 1) then $
                plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then row=(x1+x2)/(2*zoom) else row=x1/zoom
            widget_control,werror,SET_VALUE=string('Column: ',row)
    	    x1m1 = x1
    	    x2m1 = x2
        endif
    endrep until !err eq 4
    if(st.profwidth gt 1) then begin
	if(x1 lt 0) then x1 = 0
	if(x2/zoom ge xdim(0)) then x2 = zoom*(xdim(0)-1)
        row =  (x1+x2)/(2*zoom)
	row1 = x1/zoom
	row2 = x2/zoom
        prof = total(image(row1:row2,*,plane),1)/(row2 - row1 + 1)
    endif else begin
        row =  x1/zoom
        prof = image(row,*,plane)
    endelse
endelse

device,SET_GRAPHICS=3	;Resume normal graphics.

return,prof

end

;************************
function get_zprofile,row
;************************

common roi_comm

common evcomm

xxx = intarr(16)
device,set_graphics=6,cursor_image=xxx             ;Set XOR mode, Blank cursor
cursor,x,y,/WAIT,/DEV	;Get current position.
widget_control,werror,SET_VALUE=string('Left: Select row, Right: Plot profile.')
   y1 = y - zoom*st.profwidth/2
   y2 = y + zoom*st.profwidth
   if(st.profwidth gt 1) then $
        row=(fix((!DRAW_SIZE-y2-1)/zoom)+fix((!DRAW_SIZE-y1-1)/zoom))/2 $
    else $
       	row = fix((!DRAW_SIZE - y1 -1)/zoom)
    y1 = y - zoom*st.profwidth/2
    y2 = y1 + zoom*st.profwidth
    plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(st.profwidth gt 1) then $
        plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
    y1m1 = y1
    y2m1 = y2
    x1 = x - zoom*st.profwidth/2
    x2 = x1 + zoom*st.profwidth
    if(st.profwidth gt 1) then row=(x1+x2)/(2*zoom) else row=x1/zoom
    widget_control,werror,SET_VALUE=string('Row: ',row,' Column: ',row)
    x1 = x - zoom*st.profwidth/2
    x2 = x + zoom*st.profwidth
    plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(st.profwidth gt 1) then $
        plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    x1m1 = x1
    x2m1 = x2
    repeat begin
        cursor,x,y,/WAIT,/DEV	;Get current position.
        y1 = y - zoom/2
        y2 = y + zoom
	y1p = !DRAW_SIZE - y1 - 3
	y2p = !DRAW_SIZE - y2 - 3
        if(y1p/zoom lt 0) then  y1 = !DRAW_SIZE - 3
        if(y1p/zoom gt ydim(0)-1) then y1 = !DRAW_SIZE-1-zoom*(ydim(0)-1)
        if(y2p/zoom lt 0) then  y2 = !DRAW_SIZE - 3
        if(y2p/zoom gt ydim(0)-1) then y2 = !DRAW_SIZE-1-zoom*(ydim(0)-1)
	x1 = x - zoom/2
	x2 = x1 + zoom
        if(x1/zoom lt 0) then x1 = 0
        if(x2/zoom lt 0) then x2 = 0
        if(x1/zoom gt xdim(0)-1) then x1 = zoom*xdim(0)
        if(x2/zoom gt xdim(0)-1) then x2 = zoom*xdim(0)
        if(y1 ne y1m1) or (y2 ne y2m1) then begin
            plots,[0,!DRAW_SIZE],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then $
                plots,[0,!DRAW_SIZE],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
            plots,[0,!DRAW_SIZE],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then $
                plots,[0,!DRAW_SIZE],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
            if(st.profwidth gt 1) then $
                row=(fix((!DRAW_SIZE-y2-1)/zoom)+fix((!DRAW_SIZE-y1-1)/zoom))/2 $
	    else $
        	row = fix((!DRAW_SIZE - y1 -1)/zoom)
            widget_control,werror,SET_VALUE=string('Row: ',row)
    	    y1m1 = y1
    	    y2m1 = y2
        endif
        if(x1 ne x1m1) or (x2 ne x2m1) then begin
            plots,[x1m1,x1m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(st.profwidth gt 1) then $
                plots,[x2m1,x2m1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
            plots,[x1,x1],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	    if(st.profwidth gt 1) then $
                plots,[x2,x2],[0,!DRAW_SIZE],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    	    if(st.profwidth gt 1) then row=(x1+x2)/(2*zoom) else row=x1/zoom
            widget_control,werror,SET_VALUE=string('Column: ',row)
    	    x1m1 = x1
    	    x2m1 = x2
        endif
    endrep until !err eq 4
    row = fix((!DRAW_SIZE - y1 -1)/zoom)
    col = x1/zoom
    prof = image(col,row,*)

device,SET_GRAPHICS=3,/cursor_crosshair	;Resume normal graphics.
return,prof
end


;*******************************************
function roistats,verts,nvrts,pln,test_image
;*******************************************

common roi_comm

common evcomm

stats = fltarr(7)
stats(*) = -1
if(nvrts gt 0) then begin
    pixels = polyfillv(verts(0,0,0:nvrts-1,0),verts(0,0,0:nvrts-1,1),$
							xdim(0),ydim(0))
    if(pixels(0) lt 0) then begin
	print,"Polyfillv returned zero pixels for ROI #",roinum," in roistats"
	return,-1
    endif
endif else begin
    widget_control,werror,SET_VALUE=string('Error in roistats, Plane: ',pln,' nvrts: ',nvrts)
    return,stats
endelse

;max = max(image)

npix = n_elements(pixels)
row = pixels/xdim(0)
col = pixels - row*xdim(0)
sum = 0.
sumsq = 0
for i=0,npix-1 do begin
    sum = sum + image(col(i),row(i),pln)
    sumsq = sumsq + image(col(i),row(i),pln)^2
;    test_image(col(i),row(i)) = max
endfor

stats(0) = sum
stats(1) = sum/npix
var = (npix*sumsq - sum^2)/(npix*(npix-1))
stats(2) = sqrt(var)/abs(stats(1))
stats(3) = sum
stats(4) = sumsq
stats(5) = npix
stats(6) = var
return,stats
end


;**********************
function get_roinum,x,y
;**********************

common evcomm

index = xdim(0)*fix((!DRAW_SIZE -  y - 3)/zoom) + x/zoom
for iroi=0,nroi(plane)-1 do begin
    pixels = polyfillv(roi(plane,iroi,0:nverts(plane,iroi)-1,0), $
	               roi(plane,iroi,0:nverts(plane,iroi)-1,1),xdim(0),xdim(0))
    if(pixels(0) lt 0) then begin
	print,"Polyfillv returned zero pixels for ROI #",roinum," in roistats"
	return,-1
    endif
    for i = 0,n_elements(pixels)-1 do begin
	if(index eq pixels(i)) then return,iroi
    endfor
endfor

return,-1
end

;*******************
pro display_event,ev
;*******************

common disp_comm,wdisp_done

if(ev.id eq wdisp_done) then widget_control,ev.top,/DESTROY

end

;****************
pro plot_dead,arg
;****************

common plot_comm,wplot_done,subtitle,xr,plotfiles,sym,wplot,prof_on, $
       plot_title

common roi_comm

prof_on = !FALSE
widget_control,wvprof,/SENSITIVE

return
end

;******************
pro plnplt_dead,arg
;******************

common plot_comm

common roi_comm

fl.plnplt = !FALSE

return
end


;***************
pro roi_event,ev
;***************

common roi_comm

common evcomm

common disp_comm,wdisp_done

common plot_comm

common images

common volume_data,slicer_vol

type = tag_names(ev,/STRUCTURE)

on_error,2
widget_control,werror,SET_VALUE=string("    ")
if(error eq !TRUE) then begin
    widget_control,werror,SET_VALUE=string(' ')
    error = !FALSE
endif

if(ev.id eq wdraw) then begin
    if(ev.type eq !RELEASE) then return ; Only process button presses.
    if(n_elements(xdim) eq 0) then return ;No images loaded.
    if(ev.type ne 0) then return
    if(prof_on eq !TRUE) then begin
	if(ev.press eq 2) then begin
	    prof_on = !FALSE 
	endif else begin
	    widget_control,werror,SET_VALUE=string('Exit profile mode with center button.')
	    ev.id = woprof
	endelse
    endif
endif

if(ev.id eq wd.repeat_action) then begin
    ev.id = st.last_evid 
endif
if((ev.id ne wdraw) and (ev.id ne wd.nxtvol) and (ev.id ne wnextpln) and (ev.id ne wlstpln)) then $
    st.last_evid = ev.id

case ev.id of

    wdone: widget_control,ev.top,/DESTROY

    wvprof: begin
	horzprof = !FALSE
	prof = get_profile(row)	; Interactively determine profile to plot.
	prof_on = !TRUE
	profile,prof,row,fileonly(0)
    end

    wd.zprof: begin
	prof = get_zprofile(row)	; Interactively determine profile to plot.
	zero_mean = prof - total(prof)/n_elements(prof)
	profile,zero_mean,row,fileonly(0)
    end

    whprof: begin 
	horzprof = !TRUE
	prof = get_profile(row)	; Interactively determine profile to plot.
	prof_on = !TRUE
	profile,prof,row,fileonly(0)
    end

    woprof: begin 
        if(prof_on eq !FALSE) then return
	prof = get_profile(row)	; Interactively determine profile to plot.
	overplot,prof,row,fileonly
    end

    whprofsave: begin
	plotfile = pickfile(/read,FILTER='*.plt',PATH=wrtpath(0), $
					GET_PATH=wrtpath,file="Profile.plt")
        if(plotfile eq '') then return
print,wrtpath
	openw,luprof,plotfile(0),/GET_LUN
	x = fltarr(nprof+1)
	for i=0,n_elements(profdat(*,0))-1 do begin
	    for j=0,nprof do x(j) = profdat(i,j)
	    printf,luprof,x,FORMAT='(10g)'
	endfor
	close,luprof
	free_lun,luprof
    end

    wd.profsum: begin
	sums = fltarr(nprof)
	for i=0,nprof-1 do begin
	    sums(i) = total(profdat(*,i+1))
	end
	widget_control,werror,SET_VALUE= $
		string(sums,FORMAT='("Profile sums: ",5g)')
    end

    wprtplt: begin
	set_plot,'ps'
	device,YSIZE=20,YOFFSET=4,filename=!PLOTFILE
	yr = fltarr(2)
	xr = fltarr(2)
	xr(0) = profdat(0,0)
	xr(1) = profdat(n_elements(profdat(*,0))-1,0)
	yr(0) = min(profdat(*,1:nprof))
	if(yr(0) gt 0) then yr(0) = 0
	yr(1) = 1.25*max(profdat(*,1:nprof))
	prof = fltarr(n_elements(profdat(*,0)))
	for i=0,nprof do begin
	    case i of
	        1: symbol = !SQUARE
	        2: symbol = !TRIANGLE
	        3: symbol = !PLUS
	        4: symbol = !STAR
                else: symbol = !DOT
	    endcase
	    stat = execute(profcmd(i))
	endfor
	legend,plotfiles(0:nprof-1),PSYM=sym,BOX=0
	device,/CLOSE
        cmd = string('lpr -',!PRINTER,' ',!PLOTFILE)
	spawn,cmd
	set_plot,'X'
	widget_control,werror,SET_VALUE=string('Plot printed to ',!PRINTER)
	refresh_display,image_index,plane
    end

    wd.gaussfit: begin
	len = n_elements(profdat(*,0))
	data = fltarr(2*len) ; Zero pad for fitting routine.
	x = indgen(2*len)
	data(len/2:len+len/2-1) = profdat(*,nprof)
	fit = gaussfit1(x,data,params)
	mean = params(1) - (2*len - 1)/2
	label = strcompress(string(params(0),mean,params(2), $
		FORMAT='(f10.0,"exp(-.5(x - ",f8.3,")**2/",f8.3,"**2)")'))
	symtype = st.symtype
	fitplot = fit(len/2:len+len/2-1)
	st.symtype = !LINE
	overplot,fit,0,label
	st.symtype = symtype
	print,'Mean: ',mean,' Variance: ',params(2),' Scale: ',params(0)
    end

    wd.fftprof: begin
	len = n_elements(profdat(*,0))
	data = fltarr(2*len) ; Zero pad for fft routine.
	data(0:len-1) = profdat(*,nprof)
	ft = fft(data,-1)
	mag = abs(ft)
	mag(0) = 0
	profile,mag,0,fileonly(0)
    end

    wd.prof_gif: begin
;	Write current image to a tif file.
        cmd = string(filnam,plane, $
		FORMAT='("set y = ",a,"; echo $y:r\_p",i2.2,".gif")')
	spawn,cmd,gif_file
	gif_file = pickfile(/read,FILTER='*.gif',PATH='.', $
			GET_PATH=wrtpath,FILE=gif_file)
	if(gif_file eq '') then return
	cmd = string('set y = ',filnam,'; echo $y:h')
	spawn,cmd,wrtpath
	wset,plot_index
 	write_gif,gif_file(0),tvrd()
	wset,image_index
	widget_control,werror,SET_VALUE= $
		string(gif_file(0),FORMAT='("Current plane written to ",a)')
        error = !TRUE
    end

    wd.profwidth1: st.profwidth = 1
    wd.profwidth2: st.profwidth = 2
    wd.profwidth4: st.profwidth = 4
    wd.profwidth8: st.profwidth = 8
    wd.profwidth16: st.profwidth = 16

    wd.profwidthx: begin
	value = st.profwidth
	label = string("Profile width: ")
	pw = fix(get_str(1,label,value))
	st.profwidth = pw(0)
    end

    wd.profsym: begin
	st.symtype = !SYMBOL
	widget_control,wd.profsym,SENSITIVE=0
	widget_control,wd.profline,/SENSITIVE
	widget_control,wd.profsymline,/SENSITIVE
    end

    wd.profline: begin
	st.symtype = !LINE
	widget_control,wd.profline,SENSITIVE=0
	widget_control,wd.profsym,/SENSITIVE
	widget_control,wd.profsymline,/SENSITIVE
    end

    wd.profsymline: begin
	st.symtype = !SYMLINE
	widget_control,wd.profline,/SENSITIVE
	widget_control,wd.profsym,/SENSITIVE
	widget_control,wd.profsymline,SENSITIVE=0
    end

    wd.wsmo121: begin 
        widget_control,/HOURGLASS
	kernel = fltarr(3,3)
	kernel(0,0) = 1./16.
	kernel(0,1) = 2./16.
	kernel(0,2) = 1./16.
	kernel(1,*) = 2.*kernel(0,*)
 	kernel(2,*) = kernel(0,*)
        for pln=0,maxpln(0)-1 do begin
	    image(*,*,pln) = convol(image(*,*,pln),kernel,/CENTER)
	endfor
        widget_control,HOURGLASS=0
	refresh_display,image_index,plane
    end

    wd.wsmobox: begin
        widget_control,/HOURGLASS
	image = smooth(image,3)
        widget_control,HOURGLASS=0
	refresh_display,image_index,plane
    end

    wd.wsmobox5: begin
        widget_control,/HOURGLASS
	image = smooth(image,5)
        widget_control,HOURGLASS=0
	refresh_display,image_index,plane
    end

    wd.wsmobox7: begin
        widget_control,/HOURGLASS
	image = smooth(image,7)
        widget_control,HOURGLASS=0
	refresh_display,image_index,plane
    end

    wmath: begin
;	Call image math module.
	math
    end

    wd.calc: begin
	imgmath
    end

    wmask: begin
;	Set values outside support of image to zero.
	widget_control,/HOURGLASS
        for pln=0,maxpln(0)-1 do begin
	    mask = emis_mask(image(*,*,pln))
	    image(*,*,pln) = image(*,*,pln)*mask
	endfor
        refresh_display,image_index,plane
	widget_control,HOURGLASS=0
    end

    wd.histo_pln: begin
	xmax = max(image(*,*,plane))
	histo = histogram(round(1000*(image(*,*,plane)/xmax)),OMIN=hmin,OMAX=hmax)
	scl = xmax/1000.
	title = string("Histogram of plane")
	profile,histo,plane,fileonly,XMIN=hmin*scl,XMAX=hmax*scl,TITLE=title
	widget_control,werror,SET_VALUE=strcompress( $
		string('Print histogram from Profile menu.'))
    end	

    wd.histo_all: begin
	xmax = max(image)
	histo = histogram(round(1000*(image/xmax)),OMIN=hmin,OMAX=hmax)
	scl = xmax/1000.
	label = string(fileonly,FORMAT='(a,"_all")')
	title = string("Histogram of volume.")
	profile,histo,plane,label,XMIN=hmin*scl,XMAX=hmax*scl,TITLE=title
	widget_control,werror,SET_VALUE=strcompress( $
		string('Print histogram from Profile menu.'))
    end	

    wd.rotate: begin
	value = string('0')
	label = string("Rotation angle in degrees: ")
	theta = float(get_str(1,label,value))
	widget_control,/HOURGLASS
	for pln=0,maxpln(0)-1 do begin
	    image(*,*,pln) = rot(image(*,*,pln),theta(0),/INTERP)
 	end
        refresh_display,image_index,plane
	widget_control,HOURGLASS=0
    end

    wd.transpose: begin
	widget_control,/HOURGLASS
	for pln=0,maxpln(0)-1 do $
	    image(*,*,pln) = transpose(image(*,*,pln))
        refresh_display,image_index,plane
	widget_control,HOURGLASS=0
    end

    wd.wscatstat: begin
	widget_control,/HOURGLASS
	widget_control,werror,SET_VALUE=strcompress(string('Scatter file: ',filnam))
	scatstat1,'emis_3d_clipped.cub',filnam,1100,data,scat,corr
 	xd = n_elements(corr(*,0,0))
 	yd = n_elements(corr(0,*,0))
 	zd = n_elements(corr(0,0,*))
        put_image,corr,xd,yd,0,zd,'scatter_corr','scatter_corr',1,1
        put_image,data,xd,yd,0,zd,'scatter_data','scatter_corr',1,1
	widget_control,HOURGLASS=0
    end

    wd.wstats: begin 
       min = min(image(*,*,plane))
       max = max(image(*,*,plane))
       sum = total(image(*,*,plane))
       mean = sum/(xdim*ydim)
       print,filnam
       out = strcompress(string('Plane: ',plane+1,'  Minimum: ',min,'  Maximum: ',max,'  Sum: ',sum,' Mean: ',mean))
       print,out
    end

    wd.stats_by_pln: begin 
        widget_control,/HOURGLASS
	statfile = strcompress(string(list_names(current_file), $
						"_pln.stats"),/REMOVE_ALL)
	openw,lustats,statfile(0),/GET_LUN
        print,filnam
	for pln=minpln-1,maxpln(0)-1 do begin
            min = min(image(*,*,pln))
            max = max(image(*,*,pln))
            sum = total(image(*,*,pln))
            mean = sum/xdim/ydim
            out = strcompress(string('Plane: ',pln+1,'  Minimum: ',min,'  Maximum: ',max,'  Sum: ',sum,' Mean: ',mean))
            print,out
    	    printf,lustats,out
	endfor
	close,lustats
	free_lun,lustats
	print,"Statistics written to ",statfile(0)
        widget_control,HOURGLASS=0
    end

    wd.wvstats: begin 
        widget_control,/HOURGLASS
        min = min(image(*,*,*))
        max = max(image(*,*,*))
        sum = total(image(*,*,*))
        mean = sum/(xdim*ydim)
        print,filnam
        out = strcompress(string('Minimum: ',min,'  Maximum: ',max,'  Sum: ',sum,' Mean: ',mean))
        print,out
        widget_control,HOURGLASS=0
    end
    
    wdisp: begin
	erase
        refresh_display,image_index,plane
    end

    wmovie: begin
        widget_control,/HOURGLASS
        for pln=0,maxpln(0)-1 do begin
            refresh_display,image_index,pln
	    widget_control,werror,SET_VALUE=string(pln)
	endfor
        refresh_display,image_index,plane
        widget_control,HOURGLASS=0
    end

    wnextpln: begin
;        if(imgtype eq !CTI) then pln = plane + 1 else pln = plane
	pln = plane + 1
        if(pln ge st.pln_sld_max) then return
	plane = plane + 1
;        if(imgtype eq !CTI) then pln = plane + 1 else pln = plane
	pln = plane + 1
        widget_control,wsld_pln,SET_VALUE=pln
	st.last_vol_num = -1
	roinum = 0
	if(view eq !TRANSVERSE) then $
	    widget_control,wvol,SET_VALUE=string(vol(plane,roinum))
        refresh_display,image_index,plane
    end

    wlstpln: begin
;	if(imgtype eq !CTI) then pln = plane + 1 else pln = plane
	pln = plane + 1
        if(pln le st.pln_sld_min) then return
	plane = plane - 1
;	if(imgtype eq !CTI) then pln = plane + 1 else pln = plane
	pln = plane + 1
        widget_control,wsld_pln,SET_VALUE=pln
	st.last_vol_num = -1
	roinum = 0
	if(plane le maxpln(0)-1) then $
    	    widget_control,wvol,SET_VALUE=string(vol(plane,roinum))
        refresh_display,image_index,plane
    end

    wd.nxtvol: begin
	st.last_vol_num = st.last_vol_num + 1
	if(roinum ge 0) then begin
	    vol(plane,roinum) = st.last_vol_num
	    widget_control,wvol,SET_VALUE=string(vol(plane,roinum))
	endif
    end
    
    wzoomin: begin
        zoom = 2*zoom
        refresh_display,image_index,plane
    end

    wzoomout: begin
        zoom = zoom/2
        refresh_display,image_index,plane
    end

    wdreg: begin
;	Draw regions.
	for iroi=0,nroi(plane)-1 do begin
	    draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
 	endfor
    end

    wtiff: begin
;	Write current image to a tif file.
        cmd = string(filnam,plane, $
		FORMAT='("set y = ",a,"; echo $y:r\_p",i2.2,".gif")')
	spawn,cmd,tiff_file
	tiff_file = pickfile(/read,FILTER='*.gif',PATH='.', $
			GET_PATH=wrtpath,FILE=tiff_file)
	if(tiff_file eq '') then return
	cmd = string('set y = ',filnam,'; echo $y:h')
	spawn,cmd,wrtpath
;	wrtif,tiff_file(0),image,plane
	wset,wd.custom_id
 	write_gif,tiff_file(0),tvrd()
	wset,image_index
	widget_control,werror,SET_VALUE= $
		string(tiff_file(0),FORMAT='("Current plane written to ",a)')
        error = !TRUE
    end

    wd.wxwdgif: begin
;	Write current image to a xwd file.
        cmd = string(filnam, $
		FORMAT='("set y = ",a,"; echo $y:r\.gif")')
	spawn,cmd,xwd_file
	giff_file = pickfile(/read,FILTER='*.gif',PATH=wrtpath(0), $
					GET_PATH=wrtpath,FILE=xwd_file)
	if(giff_file eq '') then return
	cmd = string('set y = ',filnam,'; echo $y:h')
	spawn,cmd,wrtpath
	cmd = string('xwd -nobdrs | xwdtopnm | ppmtogif > ',giff_file(0))
	spawn,cmd
	widget_control,werror,SET_VALUE= $
		string(giff_file(0),FORMAT='("Window written to ",a)')
        error = !TRUE
    end

    wd.ldfilter: begin
	value = string('')
	label = string("Filter for body of file name: ")
	st.ldfilter = string(get_str(1,label,value),FORMAT='("*",a)')
    end

    wd.wxwdtif: begin
;	Write current image to a xwd file.
        cmd = string(filnam, $
		FORMAT='("set y = ",a,"; echo $y:r\.tif")')
	spawn,cmd,xwd_file
	tiff_file = pickfile(/read,FILTER='*.tif',PATH=wrtpath(0), $
					GET_PATH=wrtpath,FILE=xwd_file)
	if(tiff_file eq '') then return
	cmd = string('xwd -nobdrs | xwdtopnm | pnmtotiff > ',tiff_file(0))
print,cmd,wrtpath
	spawn,cmd
	widget_control,werror,SET_VALUE= $
		string(tiff_file(0),FORMAT='("Window written to ",a)')
        error = !TRUE
    end

    wxwd: begin
;	Write current image to a xwd file.
        cmd = string(filnam,plane, $
		FORMAT='("set y = ",a,"; echo $y:r\_p",i2.2,".xwd")')
	spawn,cmd,xwd_file
	xwd_file = pickfile(/read,FILTER='*.xwd',PATH=wrtpath(0), $
					GET_PATH=wrtpath,FILE=xwd_file)
	if(xwd_file eq '') then return
	cmd = string('xwd > ',xwd_file)
	spawn,cmd
	widget_control,werror,SET_VALUE= $
		string(xwd_file(0),FORMAT='("Window written to ",a)')
        error = !TRUE
    end

    wd.chngname: begin
	value = fileonly
	label = string("New name: ")
	newname = get_str(1,label,value)
	list_names(current_file) = newname
	widget_control,wfiles,SET_VALUE=list_names
    end

    wsave: begin
	; Save regions.
        cmd = string('set y = ',filnam,'; echo $y:r\.reg')
	spawn,cmd,defile
	roifile = pickfile(/read,FILTER='*.reg',PATH=regpath(0), $
					GET_PATH=regpath,FILE=defile)
	if(roifile eq '') then return
	openw,lusave,roifile(0),/GET_LUN
	writeu,lusave,maxpln
	writeu,lusave,nroi
	writeu,lusave,nverts
	writeu,lusave,roi
	writeu,lusave,vol
	close,lusave
	free_lun,lusave
    end

    wd.roiwht: st.roicolor = !WHITE_INDEX
    wd.roiylw: st.roicolor = !YELLOW_INDEX
    wd.roired: st.roicolor = !RED_INDEX
    wd.roigrn: st.roicolor = !GREEN_INDEX
    wd.roiblu: st.roicolor = !BLUE_INDEX

    wload: begin
	; Load regions.
	roifile = pickfile(/read,FILTER='*.reg',PATH=regpath(0), $
				GET_PATH=regpath,/NOCONFIRM)
	if(roifile eq '') then return
        if(maxpln(0) < !MAX_ROI_PLN) then max = !MAX_ROI_PLN else max = maxpln(0)
max = 31
        roi = intarr(max+1,!MAX_ROI,!MAX_ROI_VRTS,2)
        nverts = intarr(max+1,!MAX_ROI)
        nroi = intarr(max)
        vol = intarr(max+1,!MAX_ROI)
        vol(*,*) = -1
        cd,regpath(0)
	openu,luload,roifile(0),/GET_LUN
	readu,luload,maxpln
	readu,luload,nroi
	readu,luload,nverts
	readu,luload,roi
	readu,luload,vol
	close,luload
	free_lun,luload
	roinum = 0
        refresh_display,image_index,plane
	widget_control,wvol,/SENSITIVE
	widget_control,wsave,/SENSITIVE
    end

    wsreg: begin
;	Sum regions.
	cmd = string('set y = ',filnam,'; echo $y:r\.stats')
	spawn,cmd,roifile
	openw,fdr,roifile(0),/GET_LUN,ERROR=err
	if(err ne 0) then begin
	    print,'Error opening ',roifile,' writing stats to tmp.stats'
	    openw,fdr,'test.stats',/GET_LUN,ERROR=err
	    if(err ne 0) then begin
		widget_control,werror, $
			SET_VALUE=string("*** Could not open roi file. ***")
		return
	    endif
	endif
	print,'File: ',filnam
	printf,fdr,'File: ',filnam
	print,'Statistics printed to ',roifile(0)
	sumroi,volmean,voxcv,volvol,roisums,fdr
	for i=0,!MAX_VOL-1 do begin
	    if(volvol(i) gt 0.) then begin
	        print,i,volmean(i),100.*voxcv(i),100*voxcv(i)/volvol(i),volvol(i), $
		    FORMAT="('Vol: ',i2,', Mean: ',g,', Voxel CV: ',f7.2,'%, CV of Mean: ',f7.2,'percent, # voxels: ',g)"
	        printf,fdr,i,volmean(i),100.*voxcv(i),100*voxcv(i)/volvol(i),volvol(i), $
		    FORMAT="('Vol: ',i2,', Mean: ',g,', Voxel CV: ',f7.2,'%, CV of Mean: ',f7.2,'%, # voxels: ',g)"
 	    endif
	endfor
	close,fdr
	free_lun,fdr
    end

    wd.roiirreg: begin
	define_roi,!IRREG_ROI
    end

    wd.roisq: begin
	define_roi,!SQUARE_ROI
    end

    wd.roicrc: begin
	define_roi,!CIRC_ROI
    end

    wd.roimov: begin
	device, set_graphics=6             ;Set XOR mode
        if(roinum lt 0 or roinum gt !MAX_ROI-1) then return
	xy = fltarr(nverts(plane,roinum),2)
	widget_control,werror,SET_VALUE=string('Left: Move, Right: Exit move.')
        cursor,x,y,/WAIT,/DEV   ;Get current position.
	xm1 = x
	ym1 = y
	repeat begin
	    draw_roi,roi(plane,roinum,*,*),nverts(plane,roinum)
	    xy(*,0) = (x - xm1)/zoom
	    xy(*,1) = (ym1 - y)/zoom
	    roi(plane,roinum,0:nverts(plane,roinum)-1,*) = roi(plane,roinum,0:nverts(plane,roinum)-1,*) + xy
	    draw_roi,roi(plane,roinum,*,*),nverts(plane,roinum)
	    xm1 = x
	    ym1 = y
            cursor,x,y,/WAIT,/DEV   ;Get current position.
	    if( ((x-xm1) gt 4) and ((y-ym1) gt 4)) then begin
		xm1 = x
		ym1 = y
	    endif
	endrep until !ERR eq 4
        device,SET_GRAPHICS=3   ;Resume normal graphics.
        refresh_display,image_index,plane
    end

    wd.roipcpy: begin
	; Copy ROIs from another plane.
	label = string("Plane: ")
;	if(imgtype eq !CTI) then begin
	    if(plane gt 0) then $
		value = plane $
	    else $
		value = 1
;	endif else begin
;	    if(plane gt 0) then $
;		value = plane - 1 $
;	    else $
;		value = 0
;	endelse
        pln = fix(get_str(1,label,value,WIDTH=3))
;	if(imgtype eq !CTI) then pln = pln - 1
	pln = pln - 1
	roi(plane,*,*,*) = roi(pln,*,*,*)
	nverts(plane,*) = nverts(pln,*)
	nroi(plane) = nroi(pln)
        refresh_display,image_index,plane
    end

    wd.roicpy: begin
	for pln=0,maxpln(0)-1 do begin
;	    if(imgtype eq !CTI) then jpln = pln + 1 else jpln = pln
	    jpln = pln + 1
     	    for iroi=0,nroi(plane)-1 do begin
		if(pln ne plane) then begin
	            roi(pln,iroi,*,*) = roi(plane,iroi,*,*)
	            nverts(pln,iroi) = nverts(plane,iroi)
		endif
	    endfor
            nroi(pln) = nroi(plane)
	endfor
    end

    wd.roiplnplt: begin
	st.symtype = !SYMLINE
	sumroi,volmean,volcv,volvol,roisums
	if(roinum lt 0) then begin
	    widget_control,werror,SET_VALUE=string('*** Invalid ROI number.  ***')
	    return
	endif
	widget_control,werror,SET_VALUE=string('Plotting region #',roinum)
	profile,roisums(*,roinum),roinum,fileonly
	widget_control,wd.roiplnoplt,/SENSITIVE
    end

    wd.roiplnoplt: begin
	sumroi,volmean,volcv,volvol,roisums
	if(roinum lt 0) then begin
	    widget_control,werror,SET_VALUE=string('***Invalid ROI number.***')
	    return
	endif
	overplot,roisums(*,roinum),roinum,fileonly
    end

    wd.nrmplt: begin
	nrmfil = pickfile(/read,FILTER='*.nrm',PATH=path(0),/NOCONFIRM)
        widget_control,/HOURGLASS
	if(nrmfil eq '') then return
	norms = getnorm(nrmfil,16)
	st.symtype = !SYMLINE
	profile,norms,0,nrmfil
        widget_control,HOURGLASS=0
    end

    wd.nrmoplt: begin
	nrmfil = pickfile(/read,FILTER='*.nrm',PATH=path(0),/NOCONFIRM)
	if(nrmfil eq '') then return
        widget_control,/HOURGLASS
	norms = getnorm(nrmfil,16)
	overplot,norms,0,nrmfil
        widget_control,HOURGLASS=0
    end
    
    wdel: begin 
;     Delete Region
      if(roinum lt 0) then return
      if(roinum ge !MAX_ROI) then return
      case 1 of
	(nroi(plane) lt 1): begin
	    widget_control,werror,SET_VALUE=string('No regions defined') ;
            error = !TRUE
	end
	(nroi(plane) eq 1): begin
	    nroi(plane) = 0
            refresh_display,image_index,plane
 	end
	(nroi(plane) gt 1): begin
	    nroi(plane) = nroi(plane) - 1
	    roi(plane,roinum,*,*) = roi(plane,nroi(plane),*,*)
	    nverts(plane,roinum) =  nverts(plane,nroi(plane))
	    vol(plane,roinum) = vol(plane,nroi(plane))
            refresh_display,image_index,plane
	end
      endcase
      roinum = 0
    end

    wdelall: begin 
;	Delete all regions.
	for pln=0,maxpln(0)-1 do begin
	    nroi(pln) = 0
	endfor
	roinum = 0
    	display,image(*,*,plane),ZOOM=zoom
	widget_control,wvol,SENSITIVE=0
	widget_control,wsave,SENSITIVE=0
    end

    wcolor: begin
;	set_colors
	xloadct,NCOLORS=!LEN_COLORTAB
    end

    wd.wldimg: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.img')
	load_image,filter
    end

    wd.wldt88: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.t88')
	load_image,filter
    end

    wd.wldscn: begin
	imgtype = !CTI
	load_image,"*.scn"
    end

    wd.wldatn: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.atn')
	load_image,filter
    end

    wd.wldnrm: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.nrm')
	load_image,filter
    end

    wd.wldv: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.v')
	load_image,filter
    end

    wd.wldS: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.S')
	load_image,filter
    end

    wd.wldN: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.N')
	load_image,filter
    end

    wd.wldA: begin
	imgtype = !CTI
	filter = string('*',st.ldfilter,'*.A')
	load_image,filter
    end

    wd.mhdrdmp: begin
	file = pickfile(/read,FILTER='*',PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
	if(file eq '') then return
	mhdrdmp7,file
    end

    wd.sbhdrdmp: begin
	file = pickfile(/read,FILTER='*',PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
	if(file eq '') then return
        value = strarr(2)
        label = strarr(2)
        value(0) = string('1')
        value(1) = string('1')
        label(0) = string("Plane: ")
        label(1) = string("Frame: ")
        dims = get_str(2,label,value)
	plane = fix(dims(0))
	frame = fix(dims(1))
	data  = 0
	sbhdrdmp7,file,plane,frame,data
    end

    wldcub: begin
	imgtype = !CUBE
	filter = string('*',st.ldfilter,'*.cub')
	load_image,filter
    end

   wd.ldraw_int: begin
	imgtype = !RAW_LONG
	load_image,"*"
   end

   wd.ldraw_sht: begin
	imgtype = !RAW_SHORT
	load_image,"*"
   end

   wd.ldmri: begin
	imgtype = !MRI_SHORT
	load_image,"*.img"
   end

   wd.ldraw_flt: begin
	imgtype = !RAW_FLT
	load_image,"*"
   end

   wd.ldraw_byt: begin
	imgtype = !RAW_BYT
	load_image,"*"
   end

   wd.ldifh: begin
	imgtype = !INTERFILE
	load_image,"*.ifh"
   end

   wd.wrtraw_flt: begin
     filnam = pickfile(/read,FILTER="*.raw",PATH=path(0), $
                                GET_PATH=path)
     if(filnam eq '') then return
     get_lun,luw
     openw,luw,filnam
     writeu,luw,image
     close,luw
   end

   wd.wldmonte_all: begin
	imgtype = !MONTE_ALL
	load_image,"*.im"
   end

   wd.wldmonte_scat: begin
	imgtype = !MONTE_SCAT
	load_image,"*.comp"
   end

    wd.wrdtiff: begin
	imgtype = !TIFF
	load_image,"*.tiff"
    end

    wdelfil: begin
        widget_control,/HOURGLASS
	delete_image
        widget_control,HOURGLASS=0
    end

    wslicer: begin
	tmp = smooth(image,3)
	slicer_vol = fltarr(xdim(0),ydim(0),xdim(0))
	pad = (xdim(0) - maxpln(0))/2
	slicer_vol(*,*,pad+1:xdim(0)-pad) = tmp(*,*,*)
	value = 3.13/(6.75/2)
	label = string("Ratio of pixel width to slice thickness: ")
	aspcorr = float(get_str(1,label,value))
	ori = string("ORI",0,1,2,0,0,0,30,30,aspcorr)
	slicer,COMMAND=ori
;	set_colors
        loadct,!BLACK_BODY,NCOLORS=!LEN_COLORTAB ;Load Black Body color table.
    end

    wsurf: begin
	xsurface,image(*,*,plane)
    end

    wd.wdispglob: begin
	displayglob = !TRUE
	widget_control,wd.wdispglob,SENSITIVE=0
	widget_control,wd.wdisploc,/SENSITIVE
    end

    wd.wdisploc: begin
	displayglob = !FALSE
	widget_control,wd.wdispglob,/SENSITIVE
	widget_control,wd.wdisploc,SENSITIVE=0
    end

    wcolscl: begin
	colbase = widget_base(title='Colors',/COLUMN)
        wd.colscl_draw = widget_draw(colbase,xsize=!COLSCL_WIN_XSZ, $
	    ysize=!COLSCL_WIN_YSZ,RETAIN=2,KILL_NOTIFY=string('colscl_dead'))
	widget_control,wd.colscl_draw,/REALIZE
	fl.colscl = !TRUE
	showcolors,'ROI',image_minmax
    end

    wdisplay: begin
	disp = widget_base(title=filnam,/COLUMN)
        zoom1 = 128/xdim(0)
	if(zoom1 lt 1) then zoom1 = 1
	if(zoom < zoom1) then zoom1 = zoom
	dx = xdim(0)*zoom1
 	dy = ydim(0)*zoom1
	ncol = fix(1024/dx)
	nrow = fix(maxpln(0)/ncol)
	if(nrow*ncol lt maxpln(0)) then nrow = nrow + 1
	wimg = widget_draw(disp,xsize=ncol*dx+4,ysize=nrow*dy+4,RETAIN=2)

	widget_control,disp,/REALIZE
	widget_control,GET_VALUE=index,wimg
	wset,index
	if(displayglob eq !TRUE) then $
	    display_glob,image,min,max,ZOOM=zoom1 $
	else $
            display,image,min,max,ZOOM=zoom1
	wset,image_index
    end

    wsld_pln: begin 
        widget_control,wsld_pln,GET_VALUE=plane 
;        if(imgtype eq !CTI) then plane = plane - 1
	plane = plane - 1
	if(plane lt 0) then plane = 0
        refresh_display,image_index,plane
    end

    wd.tumor: begin
        insert_tumor,image,xd,yd,plane
	refresh_display,image_index,plane
    end

    wd.smogauss: begin
        value = strarr(2)
        label = strarr(2)
        value(0) = string('2')
        value(1) = string('1.5')
        label(0) = string("FWHM in xy plane: ")
        label(1) = string("FWHM axially: ")
        fwhms = get_str(2,label,value)
	fwhmxy = float(fwhms(0))
	fwhmz  = float(fwhms(1))
	widget_control,/HOURGLASS
	img_smoth = gauss_smoth(image,fwhmxy)
	for x=0,xdim-1 do begin
	    img = gauss_smoth(img_smoth(x,*,*),fwhmz)
	    img_smoth(x,*,*) = img
	endfor
	widget_control,HOURGLASS=0
    end

    wdraw: begin 
	ev.x = ev.x + 1
	ev.y = ev.y + 1
	if(ev.x lt 0) then ev.x = 0
	if(ev.x gt !DRAW_SIZE-1) then ev.x = !DRAW_SIZE-1
	if(ev.y lt 0) then ev.y = 0
	if(ev.y gt !DRAW_SIZE-1) then ev.y = !DRAW_SIZE-1
        if(ev.type eq 0) then begin
	    roinum = get_roinum(ev.x,ev.y)
	    if(roinum ge 0) then begin
                widget_control,wroinum,SET_VALUE=string(roinum, $
							FORMAT='("Region: ",i2)')
                widget_control,wvol,SET_VALUE=string(vol(plane,roinum))
	    endif
	    xd = ev.x/zoom
	    yd = (!DRAW_SIZE - ev.y - 1)/zoom
	    if(xd gt xdim(0)-1) then xd = xdim(0) - 1
	    if(yd gt ydim(0)-1) then yd = ydim(0) - 1
	    val = strcompress(string('(',xd,',',yd,'): ',image(xd,yd,plane)),/REMOVE_ALL)
	    widget_control,werror,SET_VALUE=val
	endif
    end

    wvol: begin 
        widget_control,ev.id,GET_VALUE=svol
	if(roinum ge 0) then begin
	    vol(plane,roinum) = fix(svol)
            widget_control,ev.id,SET_VALUE=svol
	    st.last_vol_num = vol(plane,roinum)
	endif
    end

    wfiles: begin
	filindex = ev.index
	getfile
    end

    wtrans: begin
	view = !TRANSVERSE
	filindex = current_file
	getfile
    end

    wsag: begin
	view = !SAGITAL
	filindex = current_file
	getfile
    end

    wcor: begin
	view = !CORONAL
	filindex = current_file
	getfile
    end

    wd.build3x2: begin ;Create window for display to be built.
	custbase = widget_base(title='Custom Display',/COLUMN)
        wd.custom_draw = widget_draw(custbase,xsize=!CUSTOM_XSZ, $
	    ysize=!CUSTOM_YSZ,RETAIN=2,KILL_NOTIFY=string('custom_dead'))
	widget_control,custbase,/REALIZE
        widget_control,GET_VALUE=custom_id,wd.custom_draw
	wd.custom_id = custom_id
	fl.custom = !TRUE
	bltimg = fltarr(xdim(0),ydim(0),!MAX_BLTIMG)
	nblt = 0
	widget_control,wldctiblt,/SENSITIVE
	widget_control,wanno,/SENSITIVE
    end

    wd.build2x3: begin ;Create window for display to be built.
	custbase = widget_base(title='Custom Display',/COLUMN)
        wd.custom_draw = widget_draw(custbase,xsize=!CUSTOM_YSZ, $
	    ysize=!CUSTOM_XSZ,RETAIN=2,KILL_NOTIFY=string('custom_dead'))
	widget_control,custbase,/REALIZE
        widget_control,GET_VALUE=custom_id,wd.custom_draw
	wd.custom_id = custom_id
	fl.custom = !TRUE
	bltimg = fltarr(xdim(0),ydim(0),!MAX_BLTIMG)
	nblt = 0
	widget_control,wldctiblt,/SENSITIVE
	widget_control,wanno,/SENSITIVE
    end

    wd.buildcustom: begin ;Create window for display to be built.
        value = strarr(2)
        label = strarr(2)
        value(0) = string('512')
        value(1) = string('512')
        label(0) = string("X dimension: ")
        label(1) = string("Y dimension: ")
        sizes = get_str(2,label,value)
	xsize = fix(sizes(0)) + 8
	ysize = fix(sizes(1)) + 8
	custbase = widget_base(title='Custom Display',/COLUMN)
        wd.custom_draw = widget_draw(custbase,xsize=xsize, $
	    ysize=ysize,RETAIN=2,KILL_NOTIFY=string('custom_dead'))
	widget_control,custbase,/REALIZE
        widget_control,GET_VALUE=custom_id,wd.custom_draw
	wd.custom_id = custom_id
	fl.custom = !TRUE
	bltimg = fltarr(xdim(0),ydim(0),!MAX_BLTIMG)
	nblt = 0
	widget_control,wldctiblt,/SENSITIVE
	widget_control,wanno,/SENSITIVE
    end

    wd.dispsep: begin
	sepbase = widget_base(title='Display',/COLUMN)
	sepdraw = widget_draw(sepbase,X_SCROLL_SIZE=!DRAW_SCROLL, $
	    Y_SCROLL_SIZE=!DRAW_SCROLL, $
	    XSIZE=!DRAW_SIZE,YSIZE=!DRAW_SIZE,RETAIN=2)
	widget_control,sepbase,/REALIZE
        widget_control,GET_VALUE=sep_id,sepdraw
	refresh_display,sep_id,plane
	wset,image_index
    end

    wldctiblt: begin ;Display selected image.
	if(fl.custom ne !TRUE) then begin
	    widget_control,werror,SET_VALUE=string('*** Custom display not initialized. ***')
	    return
	endif
        if  (n_elements(image(*,0,0)) gt n_elements(bltimg(*,0,0))) or $
            (n_elements(image(0,*,0)) gt n_elements(bltimg(0,*,0))) then begin
	    widget_control,werror,SET_VALUE=string('Invalid image dimensions.')
            error = !TRUE
	    return
        endif
        bxdim = n_elements(bltimg(*,0,nblt))
	bydim = n_elements(bltimg(0,*,nblt))
	nxdim = n_elements(image(*,0,plane))
	nydim = n_elements(image(0,*,plane))
        if(bxdim lt nxdim) then begin
	    widget_control,werror,SET_VALUE=string('*** Custom images must have same dimensions. ***')
            error = !TRUE
	    return
        endif
        if(bydim lt nydim) then begin
	    widget_control,werror,SET_VALUE=string('*** Custom images must have same dimensions. ***')
            error = !TRUE
	    return
        endif
	xpad = (bxdim - nxdim)/2
	ypad = (bydim - nydim)/2
	bltimg(xpad:bxdim-xpad-1,ypad:bydim-ypad-1,nblt) = image(*,*,plane)
	nblt = nblt + 1
	wset,wd.custom_id
	if(displayglob eq !TRUE) then $
	    display_glob,bltimg,min,max,ZOOM=zoom,NPANES=nblt $
	else $
            display,bltimg,min,max,ZOOM=zoom,NPANES=nblt
	blt_minmax(0) = min
	blt_minmax(1) = max
	if(fl.colscl eq !TRUE) then $
	    showcolors,'Custom',blt_minmax
	wset,image_index
    end

    wanno: begin ;Annotate display window.
	annotate,WINDOW=wd.custom_id
    end

else: print,'*** Unknown event encountered in roi_event. ***

endcase

return
end

;******************************************
pro sumroi,volmean,voxcv,volvol,roisums,fdr
;******************************************

common roi_comm

common evcomm

widget_control,/HOURGLASS
volsum  = fltarr(!MAX_VOL)
volmean = fltarr(!MAX_VOL)
volsumsq = fltarr(!MAX_VOL)
voxcv = fltarr(!MAX_VOL)
volvol = fltarr(!MAX_VOL)
volvol(*) = 0
maxroi = 0
for pln=0,maxpln(0)-1 do begin
    if(nroi(pln) gt maxroi) then maxroi = nroi(pln)
endfor
if(maxroi eq 0) then begin
    widget_control,werror,SET_VALUE=string('*** No regions defined. ***')
    return
endif
roisums = fltarr(maxpln(0),maxroi)

for pln=0,maxpln(0)-1 do begin
;    if(imgtype eq !CTI) then jpln = pln + 1 else jpln = pln
    jpln = pln + 1
;    test_image = image(*,*,pln)
    for iroi=0,nroi(pln)-1 do begin
        stats = roistats(roi(pln,iroi,*,*),nverts(pln,iroi),pln,test_image)
	roisums(pln,iroi) = stats(0)
	if((vol(pln,iroi) ge 0) and (stats(5) gt 0)) then begin
	    volsum(vol(pln,iroi))  = volsum(vol(pln,iroi)) + stats(3)
	    volsumsq(vol(pln,iroi))  = volsumsq(vol(pln,iroi)) + stats(4)
	    volvol(vol(pln,iroi)) = volvol(vol(pln,iroi)) + stats(5)
;	    print,jpln,iroi,vol(pln,iroi)," sum ",stats(3)," sumsq ",stats(4),stats(5)
	endif
	if(n_elements(fdr) gt 0) then $
	    printf,fdr,jpln,iroi,vol(pln,iroi),stats(1),stats(2),stats(5),stats(6),$
	    FORMAT='("Plane: ",i2," ROI: ",i2," Vol: ",i2," Mean: ",g," CV: ",g," # voxels: ",g," Variance: ",g)'
    endfor
    for iroi=0,nroi(plane)-1 do begin
        draw_roi,roi(plane,iroi,*,*),nverts(plane,iroi)
    endfor
;    if(nroi(pln) gt 0) then display,test_image(*,*),ZOOM=zoom
endfor
for ivol=0,!MAX_VOL-1 do begin
    if(volvol(ivol) gt 0) then begin
	nvox = volvol(ivol)
	volmean(ivol) = volsum(ivol)/nvox
	var = (nvox*volsumsq(ivol) $
		- volsum(ivol)^2)/(nvox*(nvox-1))
	voxcv(ivol) = sqrt(var)/abs(volmean(ivol))
    endif
endfor
widget_control,HOURGLASS=0

return
end

;******************
pro define_roi,type
;******************

common roi_comm

common evcomm

; Define region.
if(view ne !TRANSVERSE) then begin
    widget_control,werror,SET_VALUE=string('ROIs can only be drawn on transverse views.')
    error = !TRUE
    return
endif
if(plane ge !MAX_ROI_PLN) then begin
    widget_control,werror,SET_VALUE=string('Array bounds on roi exceeded.')
    error = !TRUE
    return
endif
roinum = nroi(plane)
if (roinum ge !MAX_ROI) then begin
    widget_control,werror,SET_VALUE=string('Array bounds on roi exceeded.')
    error = !TRUE
    return
endif
widget_control,wroinum,SET_VALUE=string(roinum,FORMAT='("Region: ",i2)')
vol(plane,roinum) = -1
widget_control,wvol,SET_VALUE=string(vol(plane,roinum))
widget_control,werror,SET_VALUE=string('Left: mark point, Middle: Erase last point, Right: Close region') ;
case type of
    !IRREG_ROI: verts =  defroi(!DRAW_SIZE-2,!DRAW_SIZE-2,/NOFILL)
    !CIRC_ROI: begin
	label = string("Diameter of Circle: ")
        value = st.last_crc_diam
	diameter = float(get_str(1,label,value))
	st.last_crc_diam = diameter(0)
	Cursor, xx, yy, /WAIT, /DEV
	verts =  circle_plot([xx,yy],zoom*diameter/2)
    end
    !SQUARE_ROI: begin
	label = string("Length of side: ")
        value = st.last_sq_side
	side = float(get_str(1,label,value))
	st.last_sq_side = side(0)
	Cursor, xx, yy, /WAIT, /DEV
	verts =  square_plot([xx,yy],zoom*side)
    end
endcase
nverts(plane,roinum) = n_elements(verts(*,0))
roi(plane,roinum,0:nverts(plane,roinum)-1,*) = verts(*,*)
nc1 = !d.table_size-1
roi(plane,roinum,*,1) = !DRAW_SIZE - roi(plane,roinum,*,1) - 1
roi(plane,roinum,*,*) = roi(plane,roinum,*,*)/zoom

draw_roi,roi(plane,roinum,*,*),nverts(plane,roinum)
nroi(plane) = nroi(plane) + 1
roimode = !FALSE
prof_on = !FALSE
widget_control,wvol,/SENSITIVE
widget_control,wsave,/SENSITIVE

return
end


;**********
pro getfile
;**********

common roi_comm

common evcomm

common images

widget_control,/HOURGLASS
listname = list_names(filindex)
imgdat = get_image(listname,filnam,stat)
if(stat eq 0) then return
cmd = string('set y = ',filnam,'; echo $y:t')
spawn,cmd,fileonly
xdim = imgdat.xdim
ydim = imgdat.ydim
minpln = imgdat.zmin
maxpln = imgdat.zmax
image = imgdat.img

;Ensure plane number in range.
pln = plane + 1
if(minpln eq maxpln) then mx = maxpln - 1 else mx = maxpln
if(pln gt mx) then pln = mx
if(pln lt minpln(0))   then pln = minpln(0)
plane = pln - 1
if(plane lt 0) then plane = 0
st.pln_sld_min = minpln
st.pln_sld_max = mx
widget_control,wsld_pln,SET_SLIDER_MIN=minpln,SET_SLIDER_MAX=mx
widget_control,wsld_pln,SET_VALUE=pln
refresh_display,image_index,plane
widget_control,HOURGLASS=0

return
end

;*********************************************************
pro profile,prof,row,file,XMIN=xmin,XMAX=xmax,TITLE=xtitle
;*********************************************************

common roi_comm

common evcomm

common plot_comm

roimode = !FALSE
plotbase = widget_base(TITLE='Plot',/COLUMN)
wplot = widget_draw(plotbase,xsize=400,ysize=500,COLORS=2,RETAIN=2,KILL_NOTIFY=string('plot_dead'))
widget_control,plotbase,/REALIZE
widget_control,GET_VALUE=plot_index,wplot
wset,plot_index
xr=fltarr(2)
if(keyword_set(XMIN)) then xr(0) = xmin else xr(0) = 0
if(keyword_set(XMAX)) then xr(1) = xmax else xr(1) = n_elements(prof) + xr(0)
if(keyword_set(TITLE)) then plot_title = xtitle(0) else plot_title=file
profdat = fltarr(n_elements(prof),20)
del = (xr(1) - xr(0))/float(n_elements(prof))
profdat(0,0) = xr(0)
for i=1,n_elements(prof)-1 do $
    profdat(i,0) = profdat(i-1) + del
profdat(*,1) = prof
;if(horzprof eq !TRUE) then xr(1) = xdim(0) else xr(1) = ydim(0)
yr=fltarr(2)
yr(0) = min(prof)
if(yr(0) gt 0) then yr(0) = 0
yr(1) = 1.25*max(prof)
profcmd = strarr(10)
legcol = intarr(10)
subtitle = string(FORMAT='("Row/Column: ",i3)',row)
sym = intarr(1)
case st.symtype of
    !LINE: sym(0) = 0
    !SYMBOL: sym(0) = !SQUARE
    !SYMLINE: sym(0) = -!SQUARE
    else: sym(0) = 0
endcase
plot,profdat(*,0),prof,BACKGROUND=0,COLOR=!WHITE_INDEX,XRANGE=xr,FONT=0,SYMSIZE=.4, $
    XSTYLE=1,SUBTITLE=subtitle,XTITLE=string('Index'),PSYM=sym(0), $
    YTITLE=string('Image value'),TITLE=plot_title(0),YRANGE=yr
prof(*) = 0
oplot,prof,COLOR=!WHITE_INDEX

profcmd(0) = strcompress(string('plot,profdat(*,0),profdat(*,1),BACKGROUND=!WHITE_INDEX,COLOR=0,XRANGE=xr,FONT=0,XSTYLE=1,XTITLE=string("Row/Column index"),YTITLE=string("Image value"),TITLE=plot_title(0),YRANGE=yr,PSYM=sym(0),SYMSIZE=.4'))

profcmd(1) = string('oplot,prof,COLOR=0')
plotfiles = strarr(10)
plotfiles(0) = strcompress(string(file,plane+1,row, $
			FORMAT='(a,"_p",i,"_",i)'),/REMOVE_ALL)
col  = !WHITE_INDEX
legend,plotfiles(0),PSYM=sym(0),COLORS=col,TEXTCOLORS=col,BOX=0

wset,image_index
widget_control,whprofsave,/SENSITIVE
nprof = 1

return
end



;**************************
pro overplot,prof,row,label
;**************************

common roi_comm

common evcomm

common plot_comm

wset,plot_index
sym1 = sym
sym = intarr(nprof+1)
sym(0:nprof-1) = sym1
case nprof of
    1: sym(nprof) = !TRIANGLE
    2: sym(nprof) = !PLUS
    3: sym(nprof) = !STAR
    4: sym(nprof) = !DOT
    5: sym(nprof) = !TRIANGLE
    6: sym(nprof) = !PLUS
    7: sym(nprof) = !STAR
    8: sym(nprof) = !DOT
    else: begin
	widget_control,werror,SET_VALUE=string('*** Too many profiles.  ***')
	wset,image_index
	prof_on = !FALSE
 	return
    end
endcase
case st.symtype of
    !LINE: sym(nprof) = 0
    !SYMLINE: sym(nprof) = -sym(nprof)
    else: sym(nprof) = sym(nprof)
endcase

nprof = nprof + 1
if(n_elements(prof) gt n_elements(profdat(*,nprof))) then begin
    off = (n_elements(prof) - n_elements(profdat(*,nprof)))/2
    profdat(*,nprof) = prof(off:n_elements(profdat(*,nprof))+off-1) 
endif else begin
    off = (n_elements(profdat(*,nprof)) - n_elements(prof))/2
    profdat(off:n_elements(prof)+off-1,nprof) = prof 
endelse
color = !LEN_COLORTAB+nprof-1
if(color gt !D.N_COLORS-1) then color = !LEN_COLORTAB+nprof-5
oplot,profdat(*,nprof),PSYM=sym(nprof-1),COLOR=color,SYMSIZE=.4
profcmd(nprof) = string('oplot,profdat(*,i),COLOR=0,PSYM=',sym(nprof-1),',SYMSIZE=.4')
plotfiles(nprof-1) = strcompress(string(label,plane+1,row, $
			FORMAT='(a,"_p",i,"_",i)'),/REMOVE_ALL)
col = intarr(nprof)
col(*) = !WHITE_INDEX
legend,plotfiles(0:nprof-1),PSYM=sym,COLORS=col,TEXTCOLORS=col,BOX=0
wset,image_index

return
end


;***********************
pro draw_roi,verts,nvrts
;***********************

common evcomm

nc1 = !d.table_size-1
for n=0,nvrts-2 do begin
    plots, [verts(0,0,n,0),verts(0,0,n+1,0)]*zoom, $
           [!DRAW_SIZE-zoom*verts(0,0,n,1),!DRAW_SIZE-zoom*verts(0,0,n+1,1)],$
           /DEVICE,COLOR=st.roicolor,/NOCLIP
endfor
plots, [verts(0,0,0,0),verts(0,0,nvrts-1,0)]*zoom, $
       [!DRAW_SIZE-zoom*verts(0,0,0,1),!DRAW_SIZE-zoom*verts(0,0,nvrts-1,1)],$
       /DEVICE,COLOR=st.roicolor,/NOCLIP        ;Complete polygon

end


;******
pro roi
;******

common roi_comm

common images

common evcomm

common plot_comm

@roi_init.h
set_plot,'X'
path = '.'
regpath = '.'
file_names = strarr(!NUM_FILES)
file_onlies = strarr(!NUM_FILES)
list_names = strarr(!NUM_FILES)
paths = strarr(!NUM_FILES)
nfiles = 0
prof_on = !FALSE
imgtype = !CTI
view = !TRANSVERSE
displayglob = !FALSE
horzprof = !TRUE
nprof = 0
colscl = !NEVER
image_minmax = fltarr(2)
blt_minmax = fltarr(2)
image_minmax(1) = 255
blt_minmax(1) = 255
error = !FALSE
woprof = -12345
plane = 0
roimode = !FALSE
fl.custom = !FALSE
tophand = handle_create()
handle_ids = lonarr(!NUM_FILES)
current_file = -1

set_colors ;Setup plotting colors and color table length.
loadct,!BLACK_BODY,NCOLORS=!LEN_COLORTAB ;Load Black Body color table.

base    = widget_base(title='Image Analysis',/COLUMN)
menrow  = widget_base(base,/ROW)
drawrow = widget_base(base,/ROW)
butrow  = widget_base(base,/ROW)
filerow = widget_base(base,/COLUMN)

mload   = widget_button(menrow,value='Files',MENU=3)
mproc   = widget_button(menrow,value='Process',MENU=3)
mdisp   = widget_button(menrow,value='Display',MENU=3)
mprof     = widget_button(menrow,value='Profiles',MENU=3)
mroi      = widget_button(menrow,value='ROI',MENU=3)
wd.repeat_action = widget_button(menrow,value='Repeat')

wd.mldcti     = widget_button(mload,value='Load CTI',MENU=3)
wd.wldimg     = widget_button(wd.mldcti,value='.img')
wd.wldt88     = widget_button(wd.mldcti,value='.t88')
wd.wldscn     = widget_button(wd.mldcti,value='.scn')
wd.wldatn     = widget_button(wd.mldcti,value='.atn')
wd.wldnrm     = widget_button(wd.mldcti,value='.nrm')
wd.wldv     = widget_button(wd.mldcti,value='.v')
wd.wldS     = widget_button(wd.mldcti,value='.S')
;wd.wldN     = widget_button(wd.mldcti,value='.N')
;wd.wlda     = widget_button(wd.mldcti,value='.a')
wd.mldelse    = widget_button(mload,value='Load other',MENU=3)
wldcub        = widget_button(wd.mldelse,value='Load Cube')
wd.ldifh        = widget_button(wd.mldelse,value='Load Interfile')
wd.ldmri  = widget_button(wd.mldelse,value='Load MRI')
wd.ldraw_byt  = widget_button(wd.mldelse,value='Byte')
wd.ldraw_int  = widget_button(wd.mldelse,value='Long integer')
wd.ldraw_sht  = widget_button(wd.mldelse,value='Short integer')
wd.ldraw_flt  = widget_button(wd.mldelse,value='Floating point')
wd.wldmonte   = widget_button(wd.mldelse,value='Load Monte Carlo',MENU=3)
wd.wldmonte_all = widget_button(wd.wldmonte,value='Whole Monte Carlo')
wd.wldmonte_scat= widget_button(wd.wldmonte,value='Compare to Real')
wd.wrdtiff= widget_button(wd.mldelse,value='Load TIFF')
wd.mshow    = widget_button(mload,value='Show',MENU=3)
wd.mhdrdmp  = widget_button(wd.mshow,value='Main header')
wd.sbhdrdmp = widget_button(wd.mshow,value='Subheader')
wdelfil   = widget_button(mload,value='Delete Image')
wd.mwrite  = widget_button(mload,value='Write',MENU=3)
wd.wrtraw_flt  = widget_button(wd.mwrite,value='Write raw float')
wtiff     = widget_button(wd.mwrite,value='Write GIF')
wd.wxwdgif = widget_button(wd.mwrite,value='Write Xwd-gif')
wd.wxwdtif = widget_button(wd.mwrite,value='Write Xwd-tiff')
wxwd      = widget_button(wd.mwrite,value='Write Xwd')
wd.chngname = widget_button(mload,value='Rename image')
wd.ldfilter = widget_button(mload,value='Name filter')
wdone     = widget_button(mload,value='Exit')


whprof       = widget_button(mprof,value='Horz Profile')
wvprof       = widget_button(mprof,value='Vert Profile')
wd.zprof      = widget_button(mprof,value='Z Profile')
whprofsave   = widget_button(mprof,value='Save Profiles')
wprtplt      = widget_button(mprof,value='Print Profiles')
wd.profsum   = widget_button(mprof,value='Sum Profiles')
wd.gaussfit  = widget_button(mprof,value='Fit Gaussian')
wd.fftprof  = widget_button(mprof,value='Fourier Transform')
wd.prof_gif  = widget_button(mprof,value='Save profile to GIF')
wd.profwidth = widget_button(mprof,value='Profile width',MENU=3)
wd.profwidth1= widget_button(wd.profwidth,value='1')
wd.profwidth2= widget_button(wd.profwidth,value='2')
wd.profwidth4= widget_button(wd.profwidth,value='4')
wd.profwidth8= widget_button(wd.profwidth,value='8')
wd.profwidth16= widget_button(wd.profwidth,value='16')
wd.profwidthx= widget_button(wd.profwidth,value='Other')
wd.msymbol   = widget_button(mprof,value='Plot Type',MENU=3)
wd.profsym   = widget_button(wd.msymbol,value='Symbols Only')
wd.profline  = widget_button(wd.msymbol,value='Lines Only')
wd.profsymline = widget_button(wd.msymbol,value='Lines and Symbols')

wd.msmooth= widget_button(mproc,value='Smooth Images',MENU=3)
wd.wsmo121= widget_button(wd.msmooth,value='1-2-1')
wd.wsmobox= widget_button(wd.msmooth,value='3x3x3 Boxcar')
wd.wsmobox5=widget_button(wd.msmooth,value='5x5x5 Boxcar')
wd.wsmobox7=widget_button(wd.msmooth,value='7x7x7 Boxcar')
wmath     = widget_button(mproc,value='Image Math')
wd.calc   = widget_button(mproc,value='Image Calculator')
wd.mstats = widget_button(mproc,value='Statistics',MENU=3)
wd.wstats = widget_button(wd.mstats,value='Plane Stats')
wd.wvstats = widget_button(wd.mstats,value='Volume Stats')
wd.stats_by_pln = widget_button(wd.mstats,value='Plane-by-plane Stats')
wmask     = widget_button(mproc,value='Mask Images')
wd.mhisto  = widget_button(mproc,value='Plot histogram',MENU=3)
wd.histo_pln = widget_button(wd.mhisto,value='Histogram of plane')
wd.histo_all = widget_button(wd.mhisto,value='Histogram of volume')
wd.rotate = widget_button(mproc,value='Rotate')
wd.transpose = widget_button(mproc,value='Transpose')
wd.tumor = widget_button(mproc,value='Insert tumor')
;wd.smogauss = widget_button(mproc,value='Smooth with 3D Gaussian')

wcolor    = widget_button(mdisp,value='Color Tables')
wcolscl   = widget_button(mdisp,value='Color Scale')
wzoomin   = widget_button(mdisp,value='Zoom In')
wzoomout  = widget_button(mdisp,value='Zoom Out')
wdisplay  = widget_button(mdisp,value='Display All')
wdisp     = widget_button(mdisp,value='Redisplay')
wd.dispsep = widget_button(mdisp,value='New display')

mcustom   = widget_button(mdisp,value='Custom',MENU=3)
wd.build3x2 = widget_button(mcustom,value='Create 3x2 display')
wd.build2x3 = widget_button(mcustom,value='Create 2x3 display')
wd.buildcustom = widget_button(mcustom,value='Create any size display')
wldctiblt = widget_button(mcustom,value='Add Image')
wanno     = widget_button(mcustom,value='Annotate')

mview     = widget_button(mdisp,value='View',MENU=3)
wtrans    = widget_button(mview,value='Transverse')
wsag      = widget_button(mview,value='Sagital')
wcor      = widget_button(mview,value='Coronal')

wmovie    = widget_button(mdisp,value='Cine')
wd.mscale = widget_button(mdisp,value='Scaling',MENU=3)
wd.wdispglob = widget_button(wd.mscale,value='Global Scale')
wd.wdisploc  = widget_button(wd.mscale,value='Local Scale')
wslicer   = widget_button(mdisp,value='Slicer')
wsurf     = widget_button(mdisp,value='Surface Plot')

wd.mroitype = widget_button(mroi,value='Define ROI',MENU=3)
wd.roiirreg  = widget_button(wd.mroitype,value='Irregular ROI')
wd.roisq  = widget_button(wd.mroitype,value='Square ROI')
wd.roicrc  = widget_button(wd.mroitype,value='Circular ROI')
wd.roimov  = widget_button(mroi,value='Move ROI')
wdel      = widget_button(mroi,value='Delete ROI')
wdelall   = widget_button(mroi,value='Delete All ROIs')
wdreg     = widget_button(mroi,value='Refresh ROIs')
wsreg     = widget_button(mroi,value='Sum ROIs')
wd.roipcpy= widget_button(mroi,value='Copy ROIs')
wload     = widget_button(mroi,value='Load ROIs')
wsave     = widget_button(mroi,value='Save ROIs')
wd.roicolor = widget_button(mroi,value='Color',MENU=3)
wd.roiwht = widget_button(wd.roicolor,value='White')
wd.roiylw = widget_button(wd.roicolor,value='Yellow')
wd.roired = widget_button(wd.roicolor,value='Red')
wd.roigrn = widget_button(wd.roicolor,value='Green')
wd.roiblu = widget_button(wd.roicolor,value='Blue')
wd.roicpy = widget_button(mroi,value='Copy ROI to Plns')
wd.roiplnplt= widget_button(mroi,value='Plot vs. Z')
wd.roiplnoplt= widget_button(mroi,value='Overplot vs. Z')
wd.nrmplt= widget_button(mroi,value='Plot norm vs. Z')
wd.nrmoplt= widget_button(mroi,value='Overplot norm vs. Z')

wdraw = widget_draw(drawrow,X_SCROLL_SIZE=!DRAW_SCROLL, $
        Y_SCROLL_SIZE=!DRAW_SCROLL,/BUTTON_EVENTS, $
	XSIZE=!DRAW_SIZE,YSIZE=!DRAW_SIZE,RETAIN=2)

wnextpln  = widget_button(butrow,value='Next Pln')
wlstpln   = widget_button(butrow,value='Last Pln')

wsld_pln  = widget_slider(filerow,Title='Plane Number')
werror    = widget_text(filerow,VALUE=' ')
wfiles    = widget_list(filerow,VALUE=file_onlies,SCR_YSIZE=150,SCR_XSIZE=420)
wroinum   = widget_label(butrow,value='ROI: -1')
wvol      = cw_field(butrow,TITLE='Vol: ',VALUE='-1',/RETURN_EVENTS,XSIZE=4)
wd.nxtvol = widget_button(butrow,value='Next vol #')

widget_control,wd.mroitype,SENSITIVE=0
widget_control,wd.roipcpy,SENSITIVE=0
widget_control,wd.roimov,SENSITIVE=0
widget_control,wdel,SENSITIVE=0
widget_control,wdisp,SENSITIVE=0
widget_control,wdreg,SENSITIVE=0
widget_control,wsreg,SENSITIVE=0
widget_control,wvol,SENSITIVE=0
widget_control,wsld_pln,SENSITIVE=0
widget_control,wsave,SENSITIVE=0
widget_control,wdisplay,SENSITIVE=0
widget_control,wd.dispsep,SENSITIVE=0
widget_control,whprof,SENSITIVE=0
widget_control,whprofsave,SENSITIVE=0
widget_control,wd.msmooth,SENSITIVE=0
widget_control,wslicer,SENSITIVE=0
widget_control,wd.wdispglob,SENSITIVE=0
widget_control,wd.wdisploc,SENSITIVE=0
widget_control,wsurf,SENSITIVE=0
widget_control,wd.mstats,SENSITIVE=0
widget_control,wzoomin,SENSITIVE=0
widget_control,wzoomout,SENSITIVE=0
widget_control,wmovie,SENSITIVE=0
widget_control,wnextpln,SENSITIVE=0
widget_control,wlstpln,SENSITIVE=0
widget_control,wd.nxtvol,SENSITIVE=0
widget_control,wd.repeat_action,SENSITIVE=0
widget_control,wdelfil,SENSITIVE=0
widget_control,wtiff,SENSITIVE=0
widget_control,wmask,SENSITIVE=0
widget_control,wldctiblt,SENSITIVE=0
widget_control,wanno,SENSITIVE=0
widget_control,wvprof,SENSITIVE=0
widget_control,wmath,SENSITIVE=0
widget_control,wd.build3x2,SENSITIVE=0
widget_control,wd.build2x3,SENSITIVE=0
widget_control,wldctiblt,SENSITIVE=0
widget_control,wprtplt,SENSITIVE=0
widget_control,wd.profsum,SENSITIVE=0
widget_control,mcustom,SENSITIVE=0
widget_control,mview,SENSITIVE=0
widget_control,wd.mscale,SENSITIVE=0
;widget_control,wd.wscatstat,SENSITIVE=0
widget_control,wd.msymbol,SENSITIVE=0
widget_control,wd.gaussfit,SENSITIVE=0
widget_control,wd.fftprof,SENSITIVE=0
widget_control,wd.profwidth,SENSITIVE=0
widget_control,wd.roicpy,SENSITIVE=0
widget_control,wd.roiplnplt,SENSITIVE=0
widget_control,wd.roiplnoplt,SENSITIVE=0
widget_control,wd.roicolor,SENSITIVE=0
widget_control,wd.calc,SENSITIVE=0
widget_control,wd.mhisto,SENSITIVE=0
widget_control,wd.wrtraw_flt,SENSITIVE=0

widget_control,base,/REALIZE

widget_control,GET_VALUE=image_index,wdraw
wset,image_index

xmanager,'roi',base

end

;********************
pro load_image,filter
;********************

common roi_comm

common evcomm

common images

if(nfiles ge !NUM_FILES) then begin 
    widget_control,werror,SET_VALUE=string('Too many images loaded.')
    error = !TRUE
    return
endif


case imgtype of

!CUBE: begin
;    File is in cube format.
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return

    ; Get number of planes.
    cmd = string('cubparam ',filnam,' 0 zdim')
    spawn,cmd,nz
    zdim = fix(nz)

    cmd = string('cubparam ',filnam,' 0 xdim')
    spawn,cmd,sxdim
    xdim = fix(sxdim)
    cmd = string('cubparam ',filnam,' 0 ydim')
    spawn,cmd,sydim
    ydim = fix(sydim)

    dim = intarr(7)
    dim(0) = xdim(0)
    dim(1) = ydim(0)
    dim(2) = zdim(0)
    dim(3) = 1
    dim(4) = 0
    dim(5) = 0
    plnfrm_sel = get_plnfrm(dim)
    widget_control,/HOURGLASS
    minpln = plnfrm_sel(0)
    maxpln = plnfrm_sel(1)
    rd_cub,filnam,image,MIN_PLANE=minpln(0),MAX_PLANE=maxpln(0)
    dxdy = 1.
    dz = 1.
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
end

!CTI: begin
;   Assume this is a CTI file.
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    dim = get_cti_dim(filnam)
    if(dim(0) lt 0) then begin
	widget_control,werror,SET_VALUE=string("*** Error reading file. ***")
	return
    endif
    plnfrm_sel = get_plnfrm(dim)
    widget_control,/HOURGLASS
    minpln = plnfrm_sel(0)
    maxpln = plnfrm_sel(1)
    frame  = plnfrm_sel(2)
    data   = plnfrm_sel(3)
    bed    = plnfrm_sel(4)
    rd_cti,filnam,stat,image,xdim,ydim,zdim,dxdy,dz, $
	MIN_PLANE=minpln(0),MAX_PLANE=maxpln(0),FRAME=frame,DATA=data,BED=bed
    plnfrm_sel(1) = zdim(0)
    maxpln = zdim(0)
    if(stat < 0) then begin
	widget_control,werror,SET_VALUE=string(' *** Could not read image. ***')
	return
    endif
    load_array,dxdy,dz(1),plnfrm_sel,frame,data,bed
end

!TIFF: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    widget_control,/HOURGLASS
    bimg = tiff_read(filnam)
    minpln = 1
    maxpln = 0
    xdim = n_elements(bimg(*,0))
    ydim = n_elements(bimg(0,*))
    image = fltarr(xdim,ydim,1)
    image(*,*,0) = bimg(*,*)
    dxdy = 1
    dz = 1
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 0
    plnfrm_sel(1) = 0
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
end

!RAW_LONG: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    cmd = string('ls -l ',filnam," | gawk '{print $5}'")
    spawn,cmd,csize
    size = long(csize)
    value = strarr(2)
    label = strarr(2)
    value(0) = string('128')
    value(1) = string('128')
    label(0) = string("X dimension: ")
    label(1) = string("Y dimension: ")
    dims = get_str(2,label,value)
    xdim = long(dims(0))
    ydim = long(dims(1))
    value = size/(xdim*ydim*4)
    label = string("z dimension: ")
    maxpln = fix(get_str(1,label,value))
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    img=lonarr(xdim(0),ydim(0),maxpln(0))
    readu, lu,img
    close,lu
    free_lun,lu
    image = float(img)
    minpln = 1
    dxdy = 1
    dz = 1
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!RAW_SHORT: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    cmd = string('ls -l ',filnam," | gawk '{print $5}'")
    spawn,cmd,csize
    size = long(csize)
    value = strarr(3)
    label = strarr(3)
    value(0) = string('0')
    value(1) = string('128')
    value(2) = string('128')
    label(0) = string("Offset in Bytes: ")
    label(1) = string("X dimension: ")
    label(2) = string("Y dimension: ")
    dims = get_str(3,label,value)
    offset = long(dims(0))
    xdim = long(dims(1))
    ydim = long(dims(2))
    value = size/(xdim*ydim*2)
    label = string("z dimension: ")
    maxpln = fix(get_str(1,label,value))
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    point_lun,lu,offset
    img=intarr(xdim(0),ydim(0),maxpln(0))
    readu, lu,img
    close,lu
    free_lun,lu
    image = float(img)
    minpln = 1
    dxdy = 1
    dz = 1
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!MRI_SHORT: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    cmd = string('ls -l ',filnam," | gawk '{print $5}'")
    spawn,cmd,csize
    size = long(csize)
    value = strarr(2)
    label = strarr(2)
    value(0) = string('128')
    value(1) = string('128')
    label(0) = string("X dimension: ")
    label(1) = string("Y dimension: ")
    dims = get_str(2,label,value)
    xdim = long(dims(0))
    ydim = long(dims(1))
    value = size/(xdim*ydim*2)
    label = string("z dimension: ")
    maxpln = fix(get_str(1,label,value))
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    img=intarr(xdim(0),ydim(0),maxpln(0))
    readu, lu,img
    close,lu
    free_lun,lu
    img = reverse(img,2)
    image = float(img)
    minpln = 1
    dxdy = 1
    dz = 1
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!RAW_FLT: begin
;    FILnam = pickfile(/read,FILTER=filter,PATH=path(0), $
;                                GET_PATH=path,/NOCONFIRM)
    filnam = pickfile(/read,FILTER='*',PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    cmd = string('ls -l ',filnam," | gawk '{print $5}'")
    spawn,cmd,csize
    size = long(csize)
    value = strarr(3)
    label = strarr(3)
    value(0) = string('0')
    value(1) = string('128')
    value(2) = string('128')
    label(0) = string("Offset in Bytes: ")
    label(1) = string("X dimension: ")
    label(2) = string("Y dimension: ")
    dims = get_str(3,label,value)
    offset = long(dims(0))
    xdim = long(dims(1))
    ydim = long(dims(2))
    value = size/(xdim*ydim*4)
    label = string("z dimension: ")
    maxpln = fix(get_str(1,label,value))
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    point_lun,lu,offset
    image=fltarr(xdim(0),ydim(0),maxpln(0))
    readu, lu,image
    close,lu
    free_lun,lu
    minpln = 1
    dxdy = .208
    dz = .3375
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!RAW_BYT: begin
;    FILnam = pickfile(/read,FILTER=filter,PATH=path(0), $
;                                GET_PATH=path,/NOCONFIRM)
    filnam = pickfile(/read,FILTER='*',PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    cmd = string('ls -l ',filnam," | gawk '{print $5}'")
    spawn,cmd,csize
    size = long(csize)
    value = strarr(3)
    label = strarr(3)
    value(0) = string('0')
    value(1) = string('128')
    value(2) = string('128')
    label(0) = string("Offset in Bytes: ")
    label(1) = string("X dimension: ")
    label(2) = string("Y dimension: ")
    dims = get_str(3,label,value)
    offset = long(dims(0))
    xdim = long(dims(1))
    ydim = long(dims(2))
    value = size/(xdim*ydim)
    label = string("z dimension: ")
    maxpln = fix(get_str(1,label,value))
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    point_lun,lu,offset
    image=bytarr(xdim(0),ydim(0),maxpln(0))
    readu, lu,image
    close,lu
    free_lun,lu
    minpln = 1
    dxdy = .208
    dz = .3375
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!INTERFILE: begin
    hdrfil = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(hdrfil eq '') then return
    ifh = read_ifh(hdrfil,scale_factors)
    xdim = ifh.matrix_size_1
    ydim = ifh.matrix_size_2
    value = string(ifh.matrix_size_3)
    label = string("Number of planes: ")
    dim3s = get_str(1,label,value)
    maxpln = long(dim3s(0))
    dxdy = ifh.zoom_factor_1
    dz = ifh.zoom_factor_3
    widget_control,/HOURGLASS
    image=fltarr(xdim(0),ydim(0),maxpln(0))
    if(strpos(ifh.number_format,"signed integer") > 0) then begin
        case ifh.bytes_per_pixel of
            1: imgin = bytarr(xdim(0),ydim(0),maxpln(0))
	    2: imgin = intarr(xdim(0),ydim(0),maxpln(0))
	    3: imgin = fltarr(xdim(0),ydim(0),maxpln(0))
            else: begin
	        print,"Invalid bytes per pixel in ",hdrfil
		return
	    endif
        endcase
    endif else begin
        print,"Invalid bytes per pixel in ",hdrfil
        return
    endelse
    get_lun,lu
    filnam = strcompress(string(path(0),ifh.data_file),/REMOVE_ALL)
    openr, lu,filnam
    readu, lu,imgin
    image(*,*,*) = imgin(*,*,*)
    for plane=0,maxpln(0)-1 do begin
        if(scale_factors(plane) gt 0) then $
	    image(*,*,plane) = scale_factors(plane)*image(*,*,plane) $
	else $
	    image(*,*,plane) = ifh.volume_sf*image(*,*,plane)
    endfor
    close,lu
    free_lun,lu
    minpln = 1
    frame = 0
    data = 0
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = maxpln(0)
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
    widget_control,HOURGLASS=0
end

!MONTE_ALL: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    file = filnam
    widget_control,/HOURGLASS
    get_lun,lu
    openr, lu,filnam
    sim=fltarr(!NANGMC,!NRAYMC,!NRINGMC,!NRINGMC,4,4)
    readu, lu, sim
    close,lu
    free_lun,lu
    true = fltarr(!NANGMC,!NRAYMC,!NRINGSQMC)
    single = fltarr(!NANGMC,!NRAYMC,!NRINGSQMC)
    mult = fltarr(!NANGMC,!NRAYMC,!NRINGMC,!NRINGMC)
    true(*,*,*) = reform(sim(*,*,*,*,0,0),!NANGMC,!NRAYMC,!NRINGSQMC)
    single(*,*,*) = reform(sim(*,*,*,*,0,1) + sim(*,*,*,*,1,0), $
						!NANGMC,!NRAYMC,!NRINGSQMC)
    mult(*,*,*,*) = sim(*,*,*,*,1,1)
    for i=0,!NSCATMC-1 do begin
        for j=2,!NSCATMC-1 do begin
	    mult(*,*,*,*) = mult(*,*,*,*) + sim(*,*,*,*,i,j)
	    if(i ne j) then mult(*,*,*,*) = mult(*,*,*,*) + sim(*,*,*,*,j,i)
        endfor
    endfor
    mult = reform(mult,!NANGMC,!NRAYMC,!NRINGSQMC)
    minpln = 1
    maxpln = !NRINGSQMC
    xdim = !NANGMC
    ydim = !NRAYMC
    dxdy = 1
    dz = 1
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = !NRINGSQMC
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0

    filnam = string(file,'_trues')
    image = fltarr(xdim,ydim,!NRINGSQMC+1)
    image(*,*,0:!NRINGSQMC-1) = true(*,*,*)
    image(*,*,!NRINGSQMC) = total(true,3)
    load_array,dxdy,dz,plnfrm_sel,frame,data,0

    filnam = string(file,'_single')
    image(*,*,0:!NRINGSQMC-1) = single(*,*,*)
    image(*,*,!NRINGSQMC) = total(single,3)
    load_array,dxdy,dz,plnfrm_sel,frame,data,0

    filnam = string(file,'_multiple')
    image(*,*,0:!NRINGSQMC-1) = mult(*,*,*)
    image(*,*,!NRINGSQMC) = total(mult,3)
    load_array,dxdy,dz,plnfrm_sel,frame,data,0
end

!MONTE_SCAT: begin
    filnam = pickfile(/read,FILTER=filter,PATH=path(0), $
                                GET_PATH=path,/NOCONFIRM)
    if(filnam eq '') then return
    file = filnam
    widget_control,/HOURGLASS
    mcdat = phgcvt(filnam)
    minpln = 1
    maxpln = !NSCATMC*!NSCATMC
    xdim = !NRAYCTI
    ydim = !NANGCTI
    dxdy = 1
    dz = 1
    plnfrm_sel = intarr(4)
    plnfrm_sel(0) = 1
    plnfrm_sel(1) = !NSCATMC*!NSCATMC
    plnfrm_sel(2) = 0
    plnfrm_sel(3) = 0
    image = mcdat(*,*,*,0) + mcdat(*,*,*,1) + mcdat(*,*,*,2)
    filnam = string(file,'_total')
    load_array,dxdy,dz,plnfrm_sel,frame,0,0
    image =  mcdat(*,*,*,1) + mcdat(*,*,*,2)
    filnam = string(file,'_scat')
    load_array,dxdy,dz,plnfrm_sel,frame,0,0
    image = mcdat(*,*,*,1)
    filnam = string(file,'_single')
    load_array,dxdy,dz,plnfrm_sel,frame,0,0
    image = mcdat(*,*,*,2)
    filnam = string(file,'_mult')
    load_array,dxdy,dz,plnfrm_sel,frame,0,0
end

endcase

widget_control,HOURGLASS=0
if(fl.first eq !TRUE) then begin
    ; Enable buttons.
    fl.first = !FALSE
    widget_control,wd.mroitype,/SENSITIVE
    widget_control,wd.roipcpy,/SENSITIVE
    widget_control,wd.roimov,/SENSITIVE
    widget_control,wd.calc,/SENSITIVE
    widget_control,wdel,/SENSITIVE
    widget_control,wdelall,/SENSITIVE
    widget_control,wdisp,/SENSITIVE
    widget_control,wdreg,/SENSITIVE
    widget_control,wsreg,/SENSITIVE
    widget_control,wdisplay,/SENSITIVE
    widget_control,wd.dispsep,/SENSITIVE
    widget_control,wsld_pln,/SENSITIVE
    widget_control,whprof,/SENSITIVE
    widget_control,wd.msmooth,/SENSITIVE
    widget_control,wslicer,/SENSITIVE
    widget_control,wd.wdispglob,/SENSITIVE
    widget_control,wsurf,/SENSITIVE
    widget_control,wd.mstats,/SENSITIVE
    widget_control,wzoomin,/SENSITIVE
    widget_control,wzoomout,/SENSITIVE
    widget_control,wmovie,/SENSITIVE
    widget_control,wnextpln,/SENSITIVE
    widget_control,wlstpln,/SENSITIVE
    widget_control,wd.nxtvol,/SENSITIVE
    widget_control,wd.repeat_action,/SENSITIVE
    widget_control,wdelfil,/SENSITIVE
    widget_control,wtiff,/SENSITIVE
    widget_control,wmask,/SENSITIVE
    widget_control,wvprof,/SENSITIVE
    widget_control,wd.zprof,/SENSITIVE
    widget_control,wmath,/SENSITIVE
    widget_control,wd.build3x2,/SENSITIVE
    widget_control,wd.build2x3,/SENSITIVE
    widget_control,wldctiblt,/SENSITIVE
    widget_control,wprtplt,/SENSITIVE
    widget_control,wd.profsum,/SENSITIVE
    widget_control,mcustom,/SENSITIVE
    widget_control,mview,/SENSITIVE
    widget_control,wd.mscale,/SENSITIVE
;    widget_control,wd.wscatstat,/SENSITIVE
    widget_control,wd.msymbol,/SENSITIVE
    widget_control,wd.gaussfit,/SENSITIVE
    widget_control,wd.fftprof,/SENSITIVE
    widget_control,wd.profwidth,/SENSITIVE
    widget_control,wd.roicpy,/SENSITIVE
    widget_control,wd.roiplnplt,/SENSITIVE
    widget_control,wd.roicolor,/SENSITIVE
    widget_control,wd.mhisto,/SENSITIVE
    widget_control,wd.wrtraw_flt,/SENSITIVE
endif


return
end

;***********************************************
pro load_array,dxdy,dz,plnfrm_sel,frame,data,bed
;***********************************************

common roi_comm

common evcomm

common images

;Setup path for next time file is loaded. 
wrtpath = path(0)
cd,path(0)
cmd = string('set y = ',filnam,'; echo $y:t')
spawn,cmd,fileonly

case imgtype of
    !CUBE: listname = fileonly 
    !CTI:  listname = strcompress(string(fileonly,frame,data,bed, $
				FORMAT='(a,"_f",i,"_d",i,"_b",i)'),/REMOVE_ALL)
    else:  listname = filnam
endcase

put_image,image,xdim(0),ydim(0),minpln(0),maxpln(0),listname,filnam,dxdy(0),dz

if(plnfrm_sel(0) eq plnfrm_sel(1)) then plnfrm_sel(1) = plnfrm_sel(1) - 1
widget_control,wsld_pln,SET_SLIDER_MIN=plnfrm_sel(0),SET_SLIDER_MAX=plnfrm_sel(1)
widget_control,wsld_pln,SET_VALUE=plnfrm_sel(0)
st.pln_sld_min = plnfrm_sel(0)
st.pln_sld_max = plnfrm_sel(1)
if(plane lt plnfrm_sel(0)-1) or (plane ge plnfrm_sel(1)-1) then $
	plane = plnfrm_sel(0) - 1 

widget_control,wsld_pln,SET_VALUE=plane + 1

; Save storage for regions if not yet declared.
if(n_elements(nroi) eq 0) then begin
    if(maxpln(0) < !MAX_ROI_PLN) then max = !MAX_ROI_PLN else max = maxpln(0)
    roi = intarr(max+1,!MAX_ROI,!MAX_ROI_VRTS,2)
    nverts = intarr(max+1,!MAX_ROI)
    nroi = intarr(max)
    vol = intarr(max+1,!MAX_ROI)
    vol(*,*) = -1
endif

if(xdim(0) eq ydim(0)) then begin
    zoom = !ZOOM_SIZE/xdim(0)
endif else begin
    if(xdim(0) gt ydim(0)) then zoom = !ZOOM_SIZE/xdim(0) $
    else zoom = !ZOOM_SIZE/ydim(0)
endelse
if(zoom eq 0) then zoom = 1
refresh_display,image_index,plane

return
end

;***************
pro delete_image
;***************

common roi_comm

common evcomm

common images

if((nfiles lt 1 ) or (current_file lt 0)) then begin
    widget_control,werror,SET_VALUE=string('No files opened.')
    return
endif

file_names(current_file) = file_names(nfiles-1)
file_onlies(current_file) = file_onlies(nfiles-1)
list_names(current_file) = list_names(nfiles-1)
paths(current_file) = paths(nfiles-1)
file_names(nfiles-1) = ' '
file_onlies(nfiles-1) = ' '
list_names(nfiles-1) = ' '
paths(nfiles-1) = ' '
widget_control,wfiles,SET_VALUE=list_names
erase

handle_free,handle_ids(current_file)
if(nfiles gt 1) then begin
    handle_ids(current_file) = handle_ids(nfiles-1)
    refresh_display,image_index,plane
endif

nfiles = nfiles - 1


return
end

;****************************
pro refresh_display,index,pln
;****************************

common roi_comm

common evcomm

wset,index
display,image(*,*,pln),min,max,ZOOM=zoom
if(fl.colscl eq !TRUE) then $
    showcolors,'ROI',image_minmax
image_minmax(0) = min
image_minmax(1) = max
if(view eq !TRANSVERSE) then begin
    if(pln lt !MAX_ROI_PLN) then begin
        for iroi=0,nroi(pln)-1 do begin
            draw_roi,roi(pln,iroi,*,*),nverts(pln,iroi)
        endfor
    endif
endif

return
end

;*************
pro set_colors
;*************

;Setup plotting colors and color table length.

red   = [1, 1, 1, 0, 0]
green = [1, 1, 0, 1, 0]
blue  = [1, 0, 0, 0, 1]
red = 255*red
green = 255*green
blue = 255*blue
tvlct,red,green,blue,!LEN_COLORTAB	 ;Load Plotting colors at end.

return
end

;********************************
pro showcolors,title,image_minmax
;********************************

common roi_comm

if(fl.colscl eq !TRUE) then $
    widget_control,GET_VALUE=scl_index,wd.colscl_draw
wset,scl_index
inc = (image_minmax(1) - image_minmax(0))/10
put_color_scale,!COLSCL_XOFF,!COLSCL_YOFF,image_minmax,inc, $
			ysize=!COLSCL_YSZ,TITLE=title
wset,image_index

return
end

;******************
pro custom_dead,arg
;******************

common roi_comm

fl.custom = !FALSE
widget_control,wldctiblt,SENSITIVE=0
widget_control,wanno,SENSITIVE=0

return
end

;******************
pro colscl_dead,arg
;******************

common roi_comm

fl.colscl = !FALSE

return
end

