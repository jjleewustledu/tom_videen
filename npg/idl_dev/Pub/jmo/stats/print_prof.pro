;*******************
pro print_prof,pr,fi
;*******************


;wset,pr.id(pr.cur_window)
handle_value,pr.profcmds(pr.cur_window),prd
set_plot,'ps'
plot_file = strcompress(string(getenv("HOME"),"/idl.ps"),/REMOVE_ALL)
device,YSIZE=20,YOFFSET=4,FILENAME=fi.printfile
yr = fltarr(2)
xr = fltarr(2)
xr(0) = prd.profdat(0,0)
xr(1) = prd.profdat(n_elements(prd.profdat(*,0))-1,0)
yr(0) = min(prd.profdat(*,1:prd.nprof))
if(yr(0) gt 0) then yr(0) = 0
yr(1) = 1.25*max(prd.profdat(*,1:prd.nprof))
prof = fltarr(n_elements(prd.profdat(*,0)))
help,prd
for i=0,prd.nprof do begin
    case i of
        1: symbol = !SQUARE
        2: symbol = !TRIANGLE
        3: symbol = !PLUS
        4: symbol = !STAR
        else: symbol = !DOT
    endcase
;print,prd.profcmd(i),total(prd.profdat(*,i))
;oplot,prd.profdat(*,i),COLOR=!LEN_COLORTAB,PSYM=       symbol,SYMSIZE=.4
    stat = execute(prd.profcmd(i))
endfor
legend,prd.plotfiles(0:prd.nprof-1),PSYM=sym,BOX=0
device,/CLOSE
cmd = string('lpr -',!PRINTER,' ',fi.printfile)
spawn,cmd
set_plot,'X'

return
end
