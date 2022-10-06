pro put_color_scale_jmo,x1,y1,range,nval,yval,labels,ysize=ysize,charsize=charsize,TITLE=title
;+
; ROUTINE:    put_color_scale
;
; USEAGE:     PUT_COLOR_SCALE,x1,y1,range[,nval,yval,label,ysize=ysize,charsize=charsize,TITLE=title]
;
; PURPOSE:   Draws a numbered color scale
;
;   INPUT:   
;
;  x1,y1        device coordinates of lower left hand corner of color bar
;
;  range        array which contains full range of physical values,
;               The number scale limits are computed fron min(range) and
;               max(range)
;  nval		Number of tic marks.
;  yval		Y values where tic marks occur.
;  labels	Labels for each tic mark.
;
;   OPTIONAL KEYWORD INPUT:
;
;  charsize     character size on number scale
;  ysize        vertical size of color bar in device units. 
;
; AUTHOR:       Paul Ricchiazzi    oct92 
;               Earth Space Research Group, UCSB
;-
;
if keyword_set(charsize) eq 0 then charsize=1
if keyword_set(TITLE) eq 0 then title=string(" ")

max_color=!LEN_COLORTAB-1 ;JMO 1/27/94
if keyword_set(ysize) eq 0 then ysize=max_color
amin=min(range)
amax=max(range)
s0=float(amin)
s1=float(amax)
;
frmt='(e9.2)'
mg=6
smax=string(amax,form=frmt)
smax=strcompress(smax,/remove_all)
smin=string(amin,form=frmt)
smin=strcompress(smin,/remove_all)
lablen=strlen(smax) > strlen(smin)
if !d.name eq 'X' then begin
  dx=20                       ; width of color bar
  x2=x1+dx
  x3=x2+2
  mg=6                        ; black out margin
  dy=ysize                    ; height of color bar
  y2=y1+dy
  bw=dx+2*mg+charsize*lablen*!d.x_ch_size
  bh=dy+2*mg+charsize*!d.y_ch_size
  tv,replicate(0,bw,bh),x1-mg,y1-mg,/device   ;  black out background 
  tv,bytscl(replicate(1,dx) # indgen(y2-y1),top=max_color),x1,y1,/device
endif else begin
  xs=!d.x_vsize/700.          ; about 100 pixels per inch on screen
  ys=!d.y_vsize/700.
  dx=20*xs                    ; width of color bar
  x2=x1+dx
  x3=x2+2*xs
  mg=6*xs                     ; black out margin
  dy=ysize                    ; height of color bar
  y2=y1+dy
;  bw=dx+2*mg+charsize*lablen*!d.x_ch_size
;  bh=dy+2*mg+charsize*!d.y_ch_size
;  tv,replicate(0,2,2),x1-mg,y1-mg,xsize=bw,ysize=bh   ;  black out background 
  tv,bytscl(replicate(1,2) # indgen(y2-y1),top=max_color), $
     x1,y1,xsize=dx,ysize=dy
endelse
;
boxx=[x1,x2,x2,x1,x1]
boxy=[y1,y1,y2,y2,y1]
plots,boxx,boxy,/device,COLOR=!WHITE_INDEX ;JMO 1/27/94
denom=amax-amin
;
for ival=0,nval-1 do begin
  ss=(yval(ival)-amin)/denom
  if ss ge 0 and ss le 1 then begin
    y=y1+(y2-y1)*ss
    plots,[x1,x2],[y,y],/device,COLOR=!WHITE_INDEX ; JMO 1/27/94
    xyouts,x3,y,labels(ival),/device,charsize=charsize,COLOR=!WHITE_INDEX ;JMO 1/27/94
  endif
endfor
xyouts,x1,y1/2,title,/DEVICE,CHARSIZE=1.25,COLOR=!WHITE_INDEX ;JMO 1/27/94

;
end



