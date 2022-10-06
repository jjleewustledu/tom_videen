;****************************************************************************************
function load_stitch,fi,st,dsp,wd,stc,img_type,help,filnam,xdim,ydim,zdim,tdim_all,num_type,dxdy,dz, $
        scl,type,paradigm
;****************************************************************************************

; Stitch together several runs.

    if(help.enable eq !TRUE) then begin
        widget_control,help.id,SET_VALUE=string('Stitches together an arbitrary number of 4DFP stacks.  All images are loaded via associated variables (i.e., images are loaded as they are accessed by the software).'+'  Data sets are stitched together by using a data structure that maps specific frames to the correct files.')
    endif

    num = 0
    fi.n = fi.nfiles
    labels = strarr(2)
    value  = strarr(2)
    labels(0)  = 'Name of entry.'
    labels(1)  = 'Number of files'
    value(0) = strcompress(string('Conc_',fi.nfiles),/REMOVE_ALL)
    value(1) = '2'
    str = get_str(2,labels,value)
    filnam = str(0)
    num = fix(str(1))
    tail = ' '
    for nn=0,num-1 do begin
        file = pickfile(/read,FILTER='*.img',/MUST_EXIST, $
			PATH=fi.path,GET_PATH=fi.path,/NOCONFIRM)
        if(file eq '') then return,!ERROR
        if(n_elements(file) gt 1) then $
	    stc(fi.n).filnam(nn) = file(0)+file(1) $
        else $
	    stc(fi.n).filnam(nn) = file
	file = stc(fi.n).filnam(nn)
        pos = rstrpos(file,'/')
        len = strlen(file)
        if(pos ge 0) then $
            tail = strmid(file,pos+1,len-pos-1) $
        else $
            tail = file
	widget_control,wd.error,SET_VALUE=strcompress(string("Last file: ",tail))
        read_header,stc(fi.n).filnam(nn),xdim,ydim,zdim,tdim,dxdy,dz,paradigm_f
        if(paradigm_f(0) lt 0) then $
	    widget_control,wd.error,SET_VALUE=string('Header contains invalid paradigm code.')
        if(nn eq 0) then $
            paradigm = fltarr(num*tdim+1)
        paradigm(nn*tdim:(nn+1)*tdim-1) = paradigm_f(0:tdim-1)
        fi.n = fi.nfiles
        fi.lun(fi.n) = -1
	get_lun,lun
    	openr,lun,stc(fi.n).filnam(nn)
        image = assoc(lun,fltarr(xdim,ydim,/NOZERO),0)
	if(nn eq 0) then begin
                tot1 = total(image(*,*,zdim/2))
		scl = 1.
		tdim_all = tdim
	    endif else begin
                tot = total(image(*,*,zdim/2))
		scl = tot1/tot
		tdim_all = tdim_all + tdim
	    endelse
	    close,lun
	    free_lun,lun
	    xx = {name:'',xdim:xdim,ydim:ydim,zdim:zdim, $
		          tdim:tdim,type:!FLOAT,dxdy:dxdy, $
			   dz:dz,scl:scl,array_type:!ASSOC_ARRAY}
	    stc(fi.n).hdr_handle(nn) = handle_create(fi.tophand,VALUE=xx)
    end
    num_type = !FLOAT
    type = !STITCH_ARRAY
    stc(fi.n).tdim = tdim
    stc(fi.n).n = num

return,!OK
end
