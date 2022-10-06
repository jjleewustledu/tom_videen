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

