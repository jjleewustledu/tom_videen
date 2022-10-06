;***********************************
pro linfit,idx,fi,pr,dsp,wd,stc,help
;***********************************

common profiles,x1,x2,y1,y2,x1m1,x2m1,y1m1,y2m1,row,col,profdat

common linear_fit,fit_name,slope,intcpt,trend

if(help.enable eq !TRUE) then begin
    widget_control,help.id,SET_VALUE=string('Perform a least square fit to first, third, fifth etc. phases of the activation paradigm.  The results are stored in the file list for viewing and can be optionally used by other processing routines.')
endif

handle_value,fi.hdr_handle(fi.n),hd
handle_value,fi.hdr_handle(hd.mother),hdr
if(fi.paradigm_loaded(hd.mother) ne !TRUE) then begin
    stat=widget_message('*** No stimulus time-course defined.  ***',/ERROR)
    return
endif

widget_control,/HOURGLASS

handle_value,fi.hdr_handle(idx),hd
handle_value,fi.hdr_handle(hd.mother),hdr

paradigm = fltarr(hdr.tdim)
paradigm(*) = fi.paradigm(hd.mother,0:hdr.tdim-1)

if(hd.array_type eq !STITCH_ARRAY) then begin
    slope = fltarr(hdr.xdim,hdr.ydim,hdr.zdim,stc(hd.mother).n)
    intcpt = fltarr(hdr.xdim,hdr.ydim,hdr.zdim,stc(hd.mother).n)
    n_slope = stc(hd.mother).n
    t_rest1 = intarr(stc(hd.mother).n)
    for file=0,stc(hd.mother).n-1 do begin
	t0 = file*stc(hd.mother).tdim
        t1 = (file+1)*stc(hd.mother).tdim - 1
        get_trend,hd.mother,t0,t1,wd,fi,stc,hdr,paradigm,slp,int,t_rest1_tmp
	t_rest1(file) = t_rest1_tmp
	slope(*,*,*,file) = slp
	intcpt(*,*,*,file) = int
    endfor
    xx = {name:'',tdim:stc(hd.mother).tdim,tdim_all:hdr.tdim}
endif else begin
    n_slope = 1
    get_trend,hd.mother,0,hdr.tdim-1,wd,fi,stc,hdr,paradigm,slope,intcpt,t_rest1
    xx = {name:'',tdim:hdr.tdim,tdim_all:hdr.tdim}
endelse

if(hdr.array_type eq !STITCH_ARRAY) then begin
    trend.t_rest1(hd.mother,0:stc(hd.mother).n-1) = t_rest1 
endif else begin 
    trend.t_rest1(hd.mother,0) = t_rest1 
endelse
trend.slope_handle(hd.mother) = handle_create(fi.tophand,VALUE=slope)
trend.intcpt_handle(hd.mother) = handle_create(fi.tophand,VALUE=intcpt)
trend.hdr_handle(hd.mother) = handle_create(fi.tophand,VALUE=xx)

fit_name = fi.tails(hd.mother)
filnum = hd.mother
name = strcompress(string(fi.name,FORMAT='(a,"_slope")'),/REMOVE_ALL)
put_image,slope,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,n_slope,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,filnum,paradigm

name = strcompress(string(fit_name,FORMAT='(a,"_intcpt")'),/REMOVE_ALL)
put_image,intcpt,fi,wd,dsp,name,hdr.xdim,hdr.ydim,hdr.zdim,n_slope,!FLOAT,hdr.dxdy,hdr.dz,hdr.scl,!FLOAT_ARRAY,filnum,paradigm

widget_control,HOURGLASS=0

return
end
