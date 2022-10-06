;*******************
pro get_time_prof,ev
;*******************

common profiles,x1,x2,y1,y2,x1m1,x2m1,y1m1,y2m1,row,col,profdat

common stats_comm,wd,fi,pr,dsp,roi,fl,st,stc,cstm,bltimg,dsp_image,help

common linear_fit,fit_name,slope,intcpt,trend


if(help.enable eq !TRUE) then begin
    value='Left button: Select row, Middle button: Exit profile mode, Right button: Plot profile.'+'  After the profile is plotted, the pixels used in the calculation are outlined in green.'+'  Since they are converted to image coordinates from the zoomed display image, they will be shifted slightly from those specified.'+'  The mean value of the profile is subtracted from each point prior to plotting it.'
    widget_control,help.id,SET_VALUE=value
endif

if(ev.press eq 4) then $
    pr.mode = !DRAW_DONE

if(pr.time_type eq !GANG_TPROF) then begin
    height = pr.gang_dim*pr.gang_subdim
    width = height
endif else begin
    height = pr.height
    width = pr.width
endelse

case pr.mode of

!DRAW_OFF: begin
    widget_control,wd.draw,/DRAW_MOTION_EVENTS
    xxx = intarr(16)
    device,set_graphics=6,cursor_image=xxx
    widget_control,wd.error,SET_VALUE=string('Left: Select row, Right: Plot profile.')
    y = ev.y
    x = ev.x
    row = fix((!DRAW_SIZEY - y -1)/dsp.zoom)
    y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - height/2) - dsp.zoom/2)
    y2 = !DRAW_SIZEY - 1 - (dsp.zoom*(row + height/2) + dsp.zoom/2)
    col = x/dsp.zoom
    x1 = dsp.zoom*(col - width/2) - dsp.zoom/2
    x2 = dsp.zoom*(col + width/2) + dsp.zoom/2
    plots,[x1,x2],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(height gt 1) then $
        plots,[x1,x2],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
    plots,[x1,x1],[y1,y2],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(width gt 1) then $
        plots,[x2,x2],[y1,y2],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    x1m1 = x1
    x2m1 = x2
    y1m1 = y1
    y2m1 = y2
    pr.mode = !DRAWING
end

!DRAWING: begin
    x = ev.x
    y = ev.y
    row = fix((!DRAW_SIZEY - y -1)/dsp.zoom)
    y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - height/2) - dsp.zoom/2)
    y2 = !DRAW_SIZEY - 1 - (dsp.zoom*(row + height/2) + dsp.zoom/2)
    col = x/dsp.zoom
    x1 = dsp.zoom*(col - width/2) - dsp.zoom/2
    x2 = dsp.zoom*(col + width/2) + dsp.zoom/2
    if(y1 ne y1m1) or (y2 ne y2m1) or (x1 ne x1m1) or (x2 ne x2m1) then begin
        plots,[x1m1,x2m1],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	if(height gt 1) then $
            plots,[x1m1,x2m1],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
        plots,[x1,x2],[y1,y1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    	if(height gt 1) then $
            plots,[x1,x2],[y2,y2],COLOR=!d.table_size-1,/DEV,/NOCLIP
        plots,[x1m1,x1m1],[y1m1,y2m1],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	if(width gt 1) then $
            plots,[x2m1,x2m1],[y1m1,y2m1],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
        plots,[x1,x1],[y1,y2],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
	if(width gt 1) then $
            plots,[x2,x2],[y1,y2],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    endif
    if(x1 ne x1m1) or (x2 ne x2m1) or (y1 ne y1m1) or (y2 ne y2m1) then $
        widget_control,wd.error,SET_VALUE=strcompress(string("(",col,",",row,")"),/REMOVE_ALL)
    x1m1 = x1
    x2m1 = x2
    y1m1 = y1
    y2m1 = y2
end

!DRAW_DONE: begin
    pr.mode = !DRAW_OFF
    plots,[x1m1,x1m1],[y1m1,y2m1],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    if(width gt 1) then $
        plots,[x2m1,x2m1],[y1m1,y2m1],COLOR=!GREEN_INDEX,/DEV,/NOCLIP
    plots,[x1m1,x2m1],[y1m1,y1m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    if(height gt 1) then $
         plots,[x1m1,x2m1],[y2m1,y2m1],COLOR=!d.table_size-1,/DEV,/NOCLIP
    device,SET_GRAPHICS=3,/cursor_crosshair	;Resume normal graphics.
    x1 = dsp.zoom*(col - width/2) - dsp.zoom/2
    x2 = dsp.zoom*(col + width/2) + dsp.zoom/2
    y1 = !DRAW_SIZEY - 1 - (dsp.zoom*(row - height/2) - dsp.zoom/2)
    y2 = !DRAW_SIZEY - 1 - (dsp.zoom*(row + height/2) + dsp.zoom/2)
    if(pr.time_type ne !GANG_TPROF) then $
        rect_plot,x1,x2,y1,y2,COLOR=pr.color
    x1 = col - width/2
    x2 = col + width/2
    if(x1 lt 0) then x1 = 0
    if(x2 gt dsp.xdim-1) then x2 = dsp.xdim-1
    y1 = row - height/2
    y2 = row + height/2
    if(y1 lt 0) then y1 = 0
    if(y1 gt dsp.ydim-1) then y2 = dsp.xdim-1
    pr.coord(0) = row
    pr.coord(1) = col

    handle_value,fi.hdr_handle(fi.n),hdr
    handle_value,fi.hdr_handle(hdr.mother),hd

    if(pr.subtract_trend eq !TRUE) then  $
        statimg = abs(get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc,/SUBTRACT_TREND)) $
    else $
        statimg = abs(get_image(dsp.zdim*(dsp.frame-1) + dsp.plane-1,fi,stc))
    case pr.time_type of
;     Plot boxes around voxels or regions used in profiles.
      !THRESH_TPROF: begin
	if(pr.time_thresh_type eq !PERCENT) then begin
	    max = max(statimg(x1:x2,y1:y2))
	    thresh = pr.time_thresh*max
	    mask = statimg gt thresh
        endif else begin  ; Threshold based on log significance prob
	    mask = statimg gt pr.time_thresh
        endelse
	mask(0:x1-1,*) = 0
        mask(x2+1:dsp.xdim-1,*) = 0
        mask(*,0:y1-1) = 0
	mask(*,y2+1:dsp.ydim-1) = 0
	indices = where(mask,nind)
        for i=0,nind-1 do begin
	    y = indices(i)/dsp.xdim
            x = indices(i) - long(y)*long(dsp.xdim)
	    xx = x*dsp.zoom
	    yy = !DRAW_SIZEY - 1 - long(y)*long(dsp.zoom)
	    side = dsp.zoom/2
    	    rect_plot,xx-side,xx+side-1,yy-side,yy+side-1,COLOR=pr.color
	end
      end
      !GANG_TPROF: begin
        mask = statimg gt 0
	x = (x1+x2)/2
	y = (y1+y2)/2
	xx1 = dsp.zoom*(x - float(pr.gang_dim*pr.gang_subdim)/2)
	xx2 = dsp.zoom*(x + float(pr.gang_dim*pr.gang_subdim)/2)
	yy2 = y - float(pr.gang_dim*pr.gang_subdim)/2
	yy1 = y + float(pr.gang_dim*pr.gang_subdim)/2
	yy2 = !DRAW_SIZEY - 1 - long(yy2)*long(dsp.zoom)
	yy1 = !DRAW_SIZEY - 1 - long(yy1)*long(dsp.zoom)
    	rect_plot,xx1,xx2,yy1,yy2,COLOR=pr.color
	yx = yy1 + pr.gang_subdim*dsp.zoom
	for i=0,pr.gang_dim-2 do begin
	   plots,[xx1,xx2],[yx,yx],/DEV,/NOCLIP,COLOR=pr.color
	   yx = yx + pr.gang_subdim*dsp.zoom
	endfor
	xx = xx1 + pr.gang_subdim*dsp.zoom
	for i=0,pr.gang_dim-2 do begin
	   plots,[xx,xx],[yy1,yy2],/DEV,/NOCLIP,COLOR=pr.color
	   xx = xx + pr.gang_subdim*dsp.zoom
	endfor
      end
    !PLAIN_TPROF: begin
    end
    !HISTO_TPROF: begin
    end
    else: print,'Invalid value of pr.time_type
    endcase

    case pr.time_type of
	!GANG_TPROF: begin
    	    prof = fltarr(pr.gang_dim^2,hd.tdim)
	end
	!HISTO_TPROF: begin
    	    prof = fltarr(x2-x1+1,y2-y1+1,hd.tdim)
	    n_histo = long(x2-x1+1)*long(y2-y1+1)*long(hd.tdim)
	    nx = long(x2-x1+1)
	    ny = long(y2-y1+1)
	    n_pix = long(x2-x1+1)*long(y2-y1+1)
	end
	else: begin
            prof = fltarr(hd.tdim)
	end
    endcase
    if((hd.tdim gt 1) and (pr.roiprof eq !FALSE)) then begin
	if(x1 lt 0) then x1 = 0
        if(x2 gt hdr.xdim-1) then x2 = hdr.xdim-1
	if(y1 lt 0) then y1 = 0
        if(y2 gt hdr.ydim-1) then y2 = hdr.ydim-1
	t1 = 0
 	for t=0,hd.tdim-1 do begin
	    if(pr.subtract_trend eq !TRUE) then  $
                image = get_image(t*dsp.zdim+dsp.plane-1,fi,stc, $
					FILNUM=hdr.mother+1,/SUBTRACT_TREND) $
	    else $
                image = get_image(t*dsp.zdim+dsp.plane-1,fi,stc,FILNUM=hdr.mother+1)
	    case pr.time_type of
	    !THRESH_TPROF: begin
	        image = mask*image
	        prof(t) = total(image(x1:x2,y1:y2))/float(nind)
    	    end
	    !PLAIN_TPROF: begin
	        prof(t) = total(image(x1:x2,y1:y2))/(width*height)
	    end
	    !HISTO_TPROF: begin
	        if(fi.paradigm(hdr.mother,t) ne 0) then begin
	            prof(*,*,t1) = image(x1:x2,y1:y2)
		    t1 = t1 + 1
		endif
	    end
	    !GANG_TPROF: begin
		x = (x1 + x2)/2
		y = (y1 + y2)/2
		if(x+(pr.gang_dim*pr.gang_subdim)/2 gt hdr.xdim-1) then $
			x = hdr.xdim -1 - (pr.gang_dim*pr.gang_subdim)/2
		if(x-(pr.gang_dim*pr.gang_subdim)/2 lt 0) then $
			x = (pr.gang_dim*pr.gang_subdim)/2
		if(y+(pr.gang_dim*pr.gang_subdim)/2 gt hdr.ydim-1) then $
			y = hdr.ydim -1 - (pr.gang_dim*pr.gang_subdim)/2
		if(y-(pr.gang_dim*pr.gang_subdim)/2 lt 0) then $
			y = (pr.gang_dim*pr.gang_subdim)/2
		y0 = y - (pr.gang_dim*pr.gang_subdim)/2
		for j=0,pr.gang_dim-1 do begin
		    x0 = x - (pr.gang_dim*pr.gang_subdim)/2
		    for ii=0,pr.gang_dim-1 do begin
			val = total(image(x0:x0+pr.gang_subdim-1,y0:y0+pr.gang_subdim-1))/pr.gang_subdim^2
	                prof(ii+pr.gang_dim*j,t) = val
		        x0 = x0 + pr.gang_subdim
                     endfor
		    y0 = y0 + pr.gang_subdim
                 endfor
             end
	     else: print,'Invalid value of pr.time_type
 	     endcase
        endfor
        if(pr.time_type eq !HISTO_TPROF) then begin
	    prof1 = fltarr(nx,ny,t1)
	    for i=0,nx-1 do begin
		for j=0,ny-1 do begin
	            m = total(prof(i,j,0:t1-1))/float(t1)
		    prof1(i,j,0:t1-1) = prof(i,j,0:t1-1) - m
		endfor
	    endfor
	    prof = reform(prof1,long(nx)*long(ny)*long(t1))
prof = fltarr(nx,ny)
prof = total(prof1,3)
;tmp = fltarr(ny,t1)
;tmp = total(prof1,1)
;prof = reform(tmp,long(ny)*long(t1))
	endif

        ; Smooth profile with boxcar filter.
;        if(pr.boxcar_width gt 0 and pr.time_type ne !HISTO_TPROF) then begin
        if(pr.boxcar_width gt 0) then begin
	    if(pr.time_type eq !GANG_TPROF) then begin
		profile = fltarr(pr.gang_dim^2,hd.tdim)
		for z=0,pr.gang_dim^2-1 do $
                    profile(z,*) = smooth(prof(z,*),pr.boxcar_width)
	    endif else begin
                profile = smooth(prof,pr.boxcar_width)
            endelse
        endif else begin
            profile = prof
	endelse

;	Get rid of discarded frames.
	lenprof = n_elements(profile)
        mask = fltarr(lenprof)
	ones = fltarr(lenprof)
        if(fi.paradigm_loaded(hdr.mother) eq !TRUE  and  $
				pr.time_type ne !HISTO_TPROF) then begin
	    ones(*) = 1
	    mask(0:hd.tdim-1) = abs(fi.paradigm(hdr.mother,0:hd.tdim-1))
	    ones = ones*mask
	endif else begin
	    mask(*) = 1
	    ones(*) = 1
        endelse

	if(pr.time_type eq !GANG_TPROF) then begin
	    zero_mean = fltarr(pr.gang_dim^2,hd.tdim)
	    for z=0,pr.gang_dim^2-1 do begin
                mean = total(profile(z,*)*mask)/total(ones)
                zero_mean(z,*) = (profile(z,*) - mean)*mask
	        for i=0,hd.tdim-1 do $
	            zero_mean(z,i) = 100.*zero_mean(z,i)/mean
	    endfor
	 endif else begin
             mean = total(profile*mask)/total(ones)
	     if( ((hd.array_type eq !ASSOC_ARRAY) or $
		  (hd.array_type eq !STITCH_ARRAY)) and  $
		     pr.time_type ne !HISTO_TPROF) then begin
	         zero_mean = (profile - mean)*mask
	         for i=0,lenprof-1 do $
	             zero_mean(i) = 100.*zero_mean(i)/mean
	     endif else begin
		zero_mean = profile
	     endelse
	 endelse

        name = strcompress(string(fi.tail,"_(",col,",",row,")"),/REMOVE_ALL)
        if(pr.prof_on eq !PROF_NO) then begin
	    case pr.time_type of
	        !GANG_TPROF: begin
                    ganged_time_profile,zero_mean,row,pr,dsp,name,profdat 
		    pr.prof_on = !PROF_NO
		end
	        !HISTO_TPROF: begin
		    nsig = 10
		    pts_per_sig = 8
		    nh = nsig*pts_per_sig
		    smean = total(profile)
		    N = n_elements(profile)
		    sig = sqrt((N*total(profile^2) - smean^2)/N/(N-1))
		    binsize = sig/pts_per_sig
		    smean = float(smean/N)
		    profile(*) = profile(*) - smean
		    min = -nsig*sig/2
		    max =  nsig*sig/2
		    histo = histogram(profile,OMIN=hmin,OMAX=hmax,MIN=min, $
				MAX=max,BINSIZE=binsize)
		    title = 'Histogram of time profile'
		    label = string(nx*ny*t1)
		    dum = 0
		    sym = pr.symtype
		    pr.symtype = !SYMBOL
		    profile,float(histo),dum,pr,dsp,label,profdat, $
				TITLE=title,XMIN=hmin/sig,XMAX=hmax/sig
		    pr.symtype = sym
		    lenh = n_elements(histo)
		    x = fltarr(lenh)
 		    x(0) = min + binsize/(2.*sig)
 		    for i=1,lenh-1 do $
			x(i) = x(i-1) + binsize/sig
		    params = fltarr(6)
		    fit = gaussfit1(x,histo(0:lenh-1),params)
		    mean = params(1)
 		    label = strcompress(string(params(0),mean,params(2), $
            FORMAT='(f10.0,"exp(-.5(x - ",f8.3,")**2/2*",f8.3,"**2)")'))
 		    text = strcompress(string(params(0),mean,params(2),smean,sig, $
            FORMAT='("Amplitude: ",f10.0,", Mean: ",f8.3,", std dev: ",f8.3," Sample mean: ",f8.3," Sample std. dev: ",f8.3)'))
                    widget_control,wd.error,SET_VALUE=text
		    print,text
		    overplot,float(fit),row,label,profdat,pr,dsp,XMIN=hmin/sig, $
						XMAX=hmax/sig
		end
	        else: begin
                    time_profile,zero_mean,row,pr,dsp,name,profdat 
	            if(fi.paradigm_loaded(hdr.mother) eq !TRUE) then begin
	                max = max(zero_mean)
	                min = min(zero_mean)
	                if(abs(max) gt abs(min)) then $
		            max = max $
	                else $
		            max = -min
	                prof = max*fi.paradigm(hdr.mother,0:hd.tdim-1)
		        name = ''
                        overplot,prof,row,name,profdat,pr,dsp
		    endif
	        end 
	    endcase
            pr.prof_on = !PROF_T
        endif else begin
            overplot,zero_mean,row,name,profdat,pr,dsp
            pr.prof_on = !PROF_T
	endelse
    endif else begin
;	Print out value for the region defined by the user.
	if(pr.subtract_trend eq !TRUE) then  $
            image = get_image((dsp.frame-1)*dsp.zdim+dsp.plane-1,fi,stc) $
	else $
            image = get_image((dsp.frame-1)*dsp.zdim+dsp.plane-1,fi,stc,/SUBTRACT_TREND)
        if(pr.time_type eq !THRESH_TPROF) then begin
           image = mask*image
	   val = total(image(x1:x2,y1:y2))/float(nind)
        endif else begin
           val = total(image(x1:x2,y1:y2))/(width*height)
        endelse
        val=string((x1+x2)/2,(y1+y2)/2,dsp.plane,val, $
 	  FORMAT='("Region centered at (",i2,",",i2,",",i2,"), Value: ",f9.4)')
        widget_control,wd.error,SET_VALUE=val
	print,val
	pr.prof_on = !PROF_REG
    endelse

    if(pr.time_type eq !HISTO_TPROF) then $
	pr.prof_on = !PROF_NO
    pr.x = x1
    pr.y = y1
    pr.mode = !DRAW_OFF
    widget_control,wd.draw,DRAW_MOTION_EVENTS=0
    widget_control,wd.draw,EVENT_PRO='stats_exec_event'
end


endcase

end

