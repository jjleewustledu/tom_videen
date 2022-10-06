;***********************************************************************
pro overplot,prof,row,label,profdat,pr,dsp,XMIN=xmin,XMAX=xmax,YLOG=ylog
;***********************************************************************

; Plot profile over existing profile created by "profile".

wset,pr.id(pr.cur_window)
handle_value,pr.profcmds(pr.cur_window),prd

xr = fltarr(2)
if(keyword_set(YLOG)) then ylog = !TRUE else ylog = !FALSE
if(keyword_set(XMIN)) then xmin = xmin else xmin = 0
if(keyword_set(XMAX)) then xmax = xmax else xmax = n_elements(prof) + xmin
x = fltarr(n_elements(prof))
for i=0,n_elements(prof)-1 do $
    x(i) = xmin + float(i)*(xmax-xmin)/n_elements(prof)

case prd.nprof of
    1: pr.sym(prd.nprof) = !TRIANGLE
    2: pr.sym(prd.nprof) = !PLUS
    3: pr.sym(prd.nprof) = !STAR
    4: pr.sym(prd.nprof) = !DOT
    5: pr.sym(prd.nprof) = !TRIANGLE
    6: pr.sym(prd.nprof) = !PLUS
    7: pr.sym(prd.nprof) = !STAR
    8: pr.sym(prd.nprof) = !DOT
    else: begin
	widget_control,wd.error,SET_VALUE=string('*** Too many profiles.  ***')
	wset,dsp.image_index
	pr.prof_on = !PROF_NO
 	return
    end
endcase
case pr.symtype of
    !LINE: pr.sym(prd.nprof) = 0
    !SYMLINE: pr.sym(prd.nprof) = -pr.sym(prd.nprof)
    else: pr.sym(prd.nprof) = pr.sym(prd.nprof)
endcase

prd.nprof = prd.nprof + 1
if(n_elements(prof) gt n_elements(prd.profdat(*,prd.nprof))) then begin
    off = (n_elements(prof) - n_elements(prd.profdat(*,prd.nprof)))/2
    prd.profdat(*,prd.nprof) = prof(off:n_elements(prd.profdat(*,prd.nprof))+off-1) 
endif else begin
    off = (n_elements(prd.profdat(*,prd.nprof)) - n_elements(prof))/2
    prd.profdat(off:n_elements(prof)+off-1,prd.nprof) = prof 
endelse
color = !LEN_COLORTAB+prd.nprof-1
if(color gt !D.N_COLORS-1) then color = !LEN_COLORTAB+prd.nprof-5
oplot,x,prd.profdat(*,prd.nprof),PSYM=pr.sym(prd.nprof-1),COLOR=color,SYMSIZE=.4
prd.profcmd(prd.nprof) = string('oplot,prd.profdat(*,i),COLOR=0,PSYM=',pr.sym(prd.nprof-1),',SYMSIZE=.4')
col = intarr(prd.nprof)
col(*) = !WHITE_INDEX
if(label ne '') then begin
    prd.plotfiles(prd.nprof-1) = strcompress(string(label,dsp.plane,dsp.frame, $
			FORMAT='(a,"_p",i,"_f",i)'),/REMOVE_ALL)
    legend,prd.plotfiles(0:prd.nprof-1),PSYM=pr.sym(0:prd.nprof-1),COLORS=col,TEXTCOLORS=col,BOX=0
endif
wset,dsp.image_index

xx = {name:'',profcmd:prd.profcmd,profdat:prd.profdat,plotfiles:prd.plotfiles,subtitle:prd.subtitle,nprof:prd.nprof}
handle_free,pr.profcmds(pr.cur_window)
pr.profcmds(pr.cur_window) = handle_create(pr.tophand,VALUE=xx)

return
end
