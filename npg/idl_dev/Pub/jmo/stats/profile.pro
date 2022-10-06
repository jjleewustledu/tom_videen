;************************************************************************
pro profile,prof,row,pr,dsp,file,dummy,XMIN=xmin,XMAX=xmax,TITLE=xtitle,YLOG=ylog
;************************************************************************

pr.cur_window = pr.nwindows
pr.nwindows = pr.nwindows + 1
plot_title = string(pr.cur_window,FORMAT='("Profile #",i2)')
pr.base(pr.cur_window) = widget_base(TITLE=plot_title,/COLUMN)
pr.draw(pr.cur_window) = widget_draw(pr.base(pr.cur_window),xsize=400,ysize=500,COLORS=2,RETAIN=2,KILL_NOTIFY=string('prof_dead'))
widget_control,pr.base(pr.cur_window),/REALIZE
widget_control,GET_VALUE=i,pr.draw(pr.cur_window)
pr.id(pr.cur_window)=i
wset,pr.id(pr.cur_window)
xr=fltarr(2)
if(keyword_set(YLOG)) then ylog = !TRUE else ylog = !FALSE
if(keyword_set(XMIN)) then xr(0) = xmin else xr(0) = 0
if(keyword_set(XMAX)) then xr(1) = xmax else xr(1) = n_elements(prof) + xr(0)
if(keyword_set(TITLE)) then pr.plot_title = xtitle(0) else pr.plot_title=file
profdat = fltarr(n_elements(prof),20)
del = (xr(1) - xr(0))/float(n_elements(prof))
profdat(0,0) = xr(0)
for i=1,n_elements(prof)-1 do $
    profdat(i,0) = profdat(i-1) + del
profdat(*,1) = prof
yr=fltarr(2)
yr(0) = min(prof)
if(yr(0) gt 0) then yr(0) = 0
yr(1) = 1.25*max(prof)
pr.profcmd = strarr(10)
legcol = intarr(10)
subtitle = string(FORMAT='("Row/Column: ",i3)',row)
case pr.symtype of
    !LINE: pr.sym(0) = 0
    !SYMBOL: pr.sym(0) = !SQUARE
    !SYMLINE: pr.sym(0) = -!SQUARE
    else: pr.sym(0) = 0
endcase
xxx = fltarr(n_elements(prof))
if(ylog eq !FALSE) then begin
    plot,profdat(*,0),prof,BACKGROUND=0,COLOR=!WHITE_INDEX,XRANGE=xr,FONT=0,SYMSIZE=.4, $
    XSTYLE=1,SUBTITLE=subtitle,XTITLE=string('Index'),PSYM=pr.sym(0), $
    YTITLE=string('Image value'),TITLE=pr.plot_title(0),YRANGE=yr
    oplot,xxx,COLOR=!WHITE_INDEX
endif else begin
    plot,profdat(*,0),prof,BACKGROUND=0,COLOR=!WHITE_INDEX,XRANGE=xr,FONT=0,SYMSIZE=.4, $
    XSTYLE=1,SUBTITLE=subtitle,XTITLE=string('Index'),PSYM=pr.sym(0), $
    YTITLE=string('Image value'),TITLE=pr.plot_title(0),/YLOG
endelse

if(ylog eq !FALSE) then begin
    pr.profcmd(0) = strcompress(string('plot,prd.profdat(*,0),prd.profdat(*,1),BACKGROUND=!WHITE_INDEX,COLOR=0,XRANGE=xr,FONT=0,XSTYLE=1,XTITLE=string("Row/Column index"),YTITLE=string("Image value"),TITLE=pr.plot_title(0),YRANGE=yr,PSYM=pr.sym(0),SYMSIZE=.4,SUBTITLE=prd.subtitle'))
endif else begin
    pr.profcmd(0) = strcompress(string('plot,prd.profdat(*,0),prd.profdat(*,1),BACKGROUND=!WHITE_INDEX,COLOR=0,XRANGE=xr,FONT=0,XSTYLE=1,XTITLE=string("Row/Column index"),YTITLE=string("Image value"),TITLE=pr.plot_title(0),PSYM=pr.sym(0),SYMSIZE=.4,SUBTITLE=prd.subtitle,/YLOG'))
endelse

pr.profcmd(1) = string('oplot,prof,COLOR=0')
pr.plotfiles = strarr(10)
pr.plotfiles(0) = strcompress(string(file,dsp.plane,dsp.frame, $
			FORMAT='(a,"_p",i,"_f",i)'),/REMOVE_ALL)
col  = !WHITE_INDEX
legend,pr.plotfiles(0),PSYM=pr.sym(0),COLORS=col,TEXTCOLORS=col,BOX=0

pr.nprof = 1
xx = {name:'',profcmd:pr.profcmd,profdat:profdat,plotfiles:pr.plotfiles,subtitle:subtitle,nprof:pr.nprof}
pr.profcmds(pr.cur_window) = handle_create(pr.tophand,VALUE=xx)

wset,dsp.image_index

return
end
