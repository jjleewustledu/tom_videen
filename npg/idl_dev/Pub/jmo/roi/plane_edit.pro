pro plane_edit, inpic, pic_change
common win, szx,szy
common pndat,pix_point,size,value
sze=size(inpic)
szx=sze(1)
szy=sze(2)
pic_change=byte(inpic) 
window,xpos=500,ypos=300, title='EDIT IMAGE',xsize=szx,ysize=szy
pix_point=bytarr(1,1)+byte(200)
choice=0
p_choice=1
value=200
size_choice=0
size=0 
buf=pic_change

    tv, rebin(pic_change,szx,szy,sample=1)
    choice=tvmenu1(['Edit','EXIT/SAVE','EXIT/NOSAVE'],'EDITOR',350,500)
while ( (choice ne 1) and (choice ne 2)) do begin
    tv, rebin(pic_change,szx,szy,sample=1)
      tvcrs,256,256


       !err=1  ;  inintialize  !err.  It is what is returned from a mouse
                ; button call.  1=left,2=middle,4=right.  In fact, !err is
                ; a number containing three bits so  a combinations of the
                ; mouse buttons will return the binary addition of the
                ; constituent bits
       lerr=3   ;  init.  "last mouse button value"  in the future we are
                ; interested in changes in the values, 
        line_hold=intarr(5000,2)     ;  to hold a line of data
        lcnt=0                       ; start counter
        xlast=fix(1000)              ; last x value
        ylast=fix(1000)              ; last y value
           
          dr_choice=tvmenu1(['MODIFY PEN','Line Mode','Segment Mode','Shapes','Interpolated curves','EXIT'],' ',350,500)
;****************************************************************************
                if(dr_choice eq 3 ) then shapes,pic_change ; call shapes pro
                if(dr_choice eq 4 ) then interp,pic_change ; call interpoler
                
                if (dr_choice eq 0 ) then begin   ; change pen characteristics
                     mod_choice=tvmenu1(['PEN SIZE','NATURE OF PEN','NO CHANGE'],'PEN SETTINGS',350,500)
                     if mod_choice eq 0 then begin
                        size_edit,size
                        pix_point=rebin(bytarr(1,1),size+1,size+1,sample=1)+value
                      endif
                     if mod_choice eq 1 then begin
                      p_choice=tvmenu1(['ADD','REPLACE','AND','OR','XOR'],$
                         'PEN TYPE',350,500)
                       value_edit,value
                     endif
          dr_choice2=tvmenu1(['Line Mode','Segment Mode','EXIT'],' ',350,500)
          dr_choice=dr_choice2+1
                endif
;****************************************************************************
          if (dr_choice eq 3) then choice=2
           device, /cursor_original   
       while (!err ne 4 and dr_choice eq 1) do begin
                cursor, xtemp,ytemp,2,device=1
                xtemp=xtemp
                ytemp=ytemp
             if ((xtemp ne xlast or ytemp ne ylast)and(lerr eq !err and !err gt 0)) then begin
                if( (abs(ytemp-ylast) gt abs(xtemp-xlast))) then begin      
                     dir=fix((ytemp-ylast)/abs(ytemp-ylast))
                for yy=ylast+dir,ytemp,dir*fix(size+1) do begin
                    xx=xlast+ fix((yy-ylast)*(xtemp-xlast)/(ytemp-ylast))
                     if ((xx+size lt szx-1) and (yy+size lt szy-1)) then begin
                      if p_choice eq 0 then  begin
                        signn=fix(2*(1.5-!ERR))
                        pic_change(xx:xx+size,yy:yy+size)=signn*value+pic_change(xx:xx+size,yy:yy+size)
                      endif 
                      if p_choice eq 1 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value*(2-!ERR)
                      endif
                      if p_choice eq 2 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value and pic_change(xx:xx+size,yy:yy+size)
                      endif
                    
                      if p_choice eq 3 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value or pic_change(xx:xx+size,yy:yy+size)
                      endif
                      
                      if p_choice eq 4 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value xor pic_change(xx:xx+size,yy:yy+size)
                      endif
                    p_point=pix_point*0 + pic_change(xx:xx+size,yy:yy+size)
                    tv, p_point,xx,yy
                    endif
                endfor
                endif
                if( (abs(ytemp-ylast) le abs(xtemp-xlast))) then begin      
                    dir=fix((xtemp-xlast)/abs(xtemp-xlast))
                for xx=xlast+dir,xtemp,dir*fix(size+1) do begin
                    yy=ylast+ fix((xx-xlast)*(ytemp-ylast)/(xtemp-xlast))
                     if ((xx+size lt szx-1) and (yy+size lt szy-1)) then begin
                      if p_choice eq 0 then  begin
                        signn=fix(2*(!ERR-1.5))
                        pic_change(xx:xx+size,yy:yy+size)=signn*value+pic_change(xx:xx+size,yy:yy+size)
                      endif 
                      if p_choice eq 1 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value*(2-!ERR)
                      endif
                      if p_choice eq 2 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value and pic_change(xx:xx+size,yy:yy+size)
                      endif
                    
                      if p_choice eq 3 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value or pic_change(xx:xx+size,yy:yy+size)
                      endif
                      
                      if p_choice eq 4 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value xor pic_change(xx:xx+size,yy:yy+size)
                      endif
                    p_point=pix_point*0 + pic_change(xx:xx+size,yy:yy+size)
                    tv, p_point,xx,yy
                  endif 
                endfor
                endif
             endif
                 xlast=xtemp
                 ylast=ytemp
                 lerr=!err
       endwhile
        device, /cursor_original
           if (dr_choice eq 2)then begin
             print, 'Start segment with Left button'
             print,'Connect segments with middle button'
             print, 'Clip Segment with Left button. Exit with Right'
            endif
       str='  '
                device,/cursor_original
       while (!err ne 4 and dr_choice eq 2) do begin
                cursor, xtemp,ytemp,0,device=1
                str=get_kbrd(0)
               if (str eq 'A') then begin
                  ytemp=ytemp+1
                  tvcrs,xtemp,ytemp
               endif
               if (str eq 'B') then begin
                  ytemp=ytemp-1
                  tvcrs,xtemp,ytemp
               endif
               if (str eq 'C') then begin
                  xtemp=xtemp+1
                  tvcrs,xtemp,ytemp
               endif
               if (str eq 'D') then begin
                  xtemp=xtemp-1
                  tvcrs,xtemp,ytemp
               endif
               if( !err ne 1 and !err ne 2) then begin
                xtemp=xlast
                ytemp=ylast
               endif
             if ((xtemp ne xlast or ytemp ne ylast)and(!err eq 2)) then begin
                if( (abs(ytemp-ylast) gt abs(xtemp-xlast))) then begin      
                     dir=fix((ytemp-ylast)/abs(ytemp-ylast))
                for yy=ylast+dir,ytemp,dir*fix(size+1) do begin
                    xx=xlast+ fix((yy-ylast)*(xtemp-xlast)/(ytemp-ylast))
                     if ((xx+size lt szx-1) and (yy+size lt szy-1)) then begin
                      if p_choice eq 0 then  begin
                        signn=1
                        pic_change(xx:xx+size,yy:yy+size)=signn*value+pic_change(xx:xx+size,yy:yy+size)
                      endif 
                      if p_choice eq 1 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value
                      endif
                      if p_choice eq 2 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value and pic_change(xx:xx+size,yy:yy+size)
                      endif
                      if p_choice eq 3 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value or pic_change(xx:xx+size,yy:yy+size)
                      endif
                      
                      if p_choice eq 4 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value xor pic_change(xx:xx+size,yy:yy+size)
                      endif
                    p_point=pix_point*0 + pic_change(xx:xx+size,yy:yy+size)
                    tv, p_point,xx,yy
                    endif
                endfor
                endif
                if( (abs(ytemp-ylast) le abs(xtemp-xlast))) then begin      
                    dir=fix((xtemp-xlast)/abs(xtemp-xlast))
                for xx=xlast+dir,xtemp,dir*fix(size+1) do begin
                    yy=ylast+ fix((xx-xlast)*(ytemp-ylast)/(xtemp-xlast))
                     if ((xx+size lt szx-1) and (yy+size lt szy-1)) then begin
                      if p_choice eq 0 then  begin
                        signn=1
                        pic_change(xx:xx+size,yy:yy+size)=signn*value+pic_change(xx:xx+size,yy:yy+size)
                      endif 
                      if p_choice eq 1 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value
                      endif
                      if p_choice eq 2 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value and pic_change(xx:xx+size,yy:yy+size)
                      endif
                    
                      if p_choice eq 3 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value or pic_change(xx:xx+size,yy:yy+size)
                      endif
                      
                      if p_choice eq 4 then begin
                        pic_change(xx:xx+size,yy:yy+size)=value xor pic_change(xx:xx+size,yy:yy+size)
                      endif
                    p_point=pix_point*0 + pic_change(xx:xx+size,yy:yy+size)
                    tv, p_point,xx,yy
                  endif 
                endfor
                endif
             endif
                 xlast=xtemp
                 ylast=ytemp
                 lerr=!err
        endwhile
        device,/cursor_crosshair
    tv, rebin(pic_change,szx,szy,sample=1)
    choice=tvmenu1(['Edit','EXIT/SAVE','EXIT/NOSAVE','REFRESH'],'EDITOR',350,500)

    if choice eq 3 then pic_change=buf
endwhile
if(choice eq 2) then begin
  pic_change=buf
endif
end


pro size_edit,size     ; this allows the user to select size of pen point
common win, szx,szy
       size=tvmenu1(['    1','    2','    3','    4','    5','ENTER VALUE'],'PEN SIZE',350,500)   
      if (size eq 5) then begin
         print,'Enter Cursor Size'
         read, size
         if size gt  100 then size=100
         if size  lt  1 then size=1
         size=fix(size)
      endif
       tvcrs, 256,256
end


pro value_edit, value     ;  user slects value of pen point
common win, szx,szy
      ; input_choice=tvmenu1(['ENTER FROM KEYBOARD','ENTER WITH MOUSE'],' ',350,500)
         print, 'Enter the value which is added to or placed in each pixel'
         read, value
         value=fix(value)
end



pro shapes, pic     ; this function incorpotes shapes to editing abilities
common pndat,pix_point,size,value
  common win, szx,szy
          pix=bytarr(1,1)+127
sz=size(pic)
sy=sz(2)
sx=sz(1)
buf=pic
shape_choice=tvmenu1(['EXIT','CIRCLES','BOX'],'SHAPE CHOICES',350,500)
while shape_choice gt 0 do begin
device, /cursor_original
tv,pic 
  if shape_choice eq 1 then begin
     print,'SNAP on to center of circle'
          cursor,x,y,device=1
          for i=x-1,x+1 do begin
          for j=y-1,y+1 do begin
            if (i gt  0 and i lt sx and j gt 0 and j lt sy) then begin
                  tv,[[pix]],i,y           
             endif
           endfor
           endfor
      print,  'Press left button to view.  Right button to save'
          !err=0
              hold=intarr(40,2)
       while (!err ne 4) do begin
          cursor,xt,yt,device=1
           if (!err eq 1) then begin
              r= fix( sqrt( (xt-x)^2+(yt-y)^2))
              xc=x-r
              xcb=x+r
              yc=y-r
              ycb=y+r
              for i=0,39 do begin
                  point=pic(hold(i,0):hold(i,0)+size,hold(i,1):hold(i,1)+size)
                 tv,point,hold(i,0),hold(i,1)
                 angle=float(i)*2.0*!pi/(40.0)
                 xx=x+fix(r*cos(angle))
                 yy=y+fix(r*sin(angle))
                 if((xx gt 0 and xx lt sx-size) and  (yy gt 0 and yy lt sy-size)) then begin
                 hold(i,0)=xx
                 hold(i,1)=yy
                 ;tv, [[pix]], hold(i,0),hold(i,1)
                 tv, pix_point, hold(i,0),hold(i,1)
                 endif  
              endfor 
            endif  
           if  (!err eq 4) then begin
              r= fix( sqrt( (xt-x)^2+(yt-y)^2))
               for i=0,39 do begin
                ;  tv,[[ pic(hold(i,0),hold(i,1))]],hold(i,0),hold(i,1)
                tv,pic(hold(i,0):hold(i,0)+size,hold(i,1):hold(i,1)+size),hold(i,0),hold(i,1)
               endfor 

              for i=0,999 do begin
                  angle=float(i)*2.0*!pi/(1000.0)
                  xloc=x+ fix(r*cos(angle))
                  yloc=y+ fix(r*sin(angle))
                if(xloc gt 0 and xloc lt sx-size-1 and yloc gt 0 and yloc lt sy-size-1) $
                        then begin
                  tv, pix_point,xloc,yloc
                 pic(xloc:xloc+size,yloc:yloc+size)=value
                 endif
               endfor
               tv,pic
               !err = 4
             endif
       endwhile
     endif

   if shape_choice eq 2 then begin 
    print,'Snap mouse onto on corner of your box'
        mem=[0,2,0,2]
        cursor, x1,y1, device=1
      print,'Snap right button to fix your box'
       xline=bytarr(sx)+value
       yline=bytarr(sy)+value
      while(!err ne 4 ) do begin
          cursor,x2,y2,/change,device=1
          l=fix(min([x1,x2]))
          b=fix(min([y1,y2]))
          r=fix(max([x1,x2]))
          t=fix(max([y1,y2]))
          if (( t lt szy-size) and ( r lt szy-size) and( x2 ne -1) ) then begin
          tv, pic(mem(0):mem(1),mem(2)),mem(0),mem(2)
          tv, pic(mem(0):mem(1),mem(3)),mem(0),mem(3)
          tv, pic(mem(0),mem(2):mem(3)),mem(0),mem(2)
          tv, pic(mem(1),mem(2):mem(3)),mem(1),mem(2)
          mem(0)=l
          mem(1)=r
          mem(2)=b
          mem(3)=t
          tv, xline(l:r),l,b
          tv, xline(l:r),l,t
          tv, transpose(yline(b:t)),l,b
          tv, transpose(yline(b:t)),r,b
          if(!err eq 4 ) then begin
            pic(l:r,b)=xline(l:r)
            pic(l:r,t)=xline(l:r)
            pic(l,b:t)=yline(b:t)
            pic(r,b:t)=yline(b:t)
          pic(l:r,b:b+size)=rebin(xline(l:r),r-l+1,size+1,sample=1)
          pic(l:r,t:t+size)=rebin(xline(l:r),r-l+1,size+1,sample=1)
          pic(l:l+size,b:t)=rebin(transpose(yline(b:t)),size+1,t-b+1,sample=1)
          pic(r:r+size,b:t)=rebin(transpose(yline(b:t)),size+1,t-b+1,sample=1)
            tv,pic
          endif
          endif
      endwhile
           tv,pic
   endif
shape_choice=tvmenu1(['EXIT','CIRCLES','BOX','REFRESH'],'SHAPE CHOICES',350,500)
  if(shape_choice eq 3) then pic=buf
        device,/cursor_crosshair
endwhile
end



         
pro interp, pic
	common win, szx,szy
	buf=pic
	sh=size(pic)
	sx=sh(1)
	sy=sh(2)
	tv, pic
		print,'    '
		print,'    '
		print,'    '
		print,'    '
		print,'Enter The Value for the curve to assume'
		read,value
		print,'Enter The Value for the control points to assume'
		read,pvalue
                print, 'Thanks--  go draw!'
		print, 'Press the left button for every point you wish to '
		print,'interpolate a continuous line through '
		print, 'End this process with the right button'
	flag=1
	stptr=1
	while( flag eq 1 ) do begin
	!err=0
	       if(stptr eq 1) then begin
	       holder=intarr(1000,2)
	       n=0.0
		xlast=0
		ylast=0
		 device,/cursor_original
	       while( !err ne 4) do begin
		    cursor,x,y,device=1
		      d= sqrt( (x-xlast)^2+(y-ylast)^2)
		   if (!err eq 1 and d gt 4  ) then begin
		      n=n+1
		      holder(n-1,0)=x
		      holder(n-1,1)=y
			 xlast=x          
			 ylast=y
		      tv, [[pvalue]],x,y
		    endif
	       endwhile
		endif
		 x=holder(0:n-1,0)
		 y=holder(0:n-1,1)
		 d=findgen(n)
		  str=fltarr(20000)
		   l=0
		   for i=0,n-2 do begin
		     b=l
		     r=2*sqrt((holder(i,0)-holder(i+1,0))^2+(holder(i,1)-holder(i+1,1))^2)
		     r=max([1.0,r])
		     l=l+fix(r)
		     str(b:l-1)=findgen(fix(r))/float(r)
		     str(b:l-1)=str(b:l-1)+float(i)
		   endfor
		      str=str(0:l-1)
		 nx=spline(d,x,str)
		 ny=spline(d,y,str)
		
		 for i=0,l-1 do begin
			xp=fix(nx(i)) 
			yp=fix(ny(i))
		      if( xp gt 0 and xp lt sx and yp gt 0 and yp lt sy) then begin
		      pic(xp,yp)=value
		      tv,[[value]],xp,yp
		      endif
		 endfor
		 for i=0,n-1 do begin
		      xp=holder(i,0)
		      yp=holder(i,1)
		      pic(xp,yp)=pvalue
		      tv,[[pvalue]],xp,yp
		 endfor
		 tv,pic
	   flag=tvmenu1(['EXIT','CHANGE CONTROL POINTS'],' ',350,500)
	     if(flag eq 1) then begin
		  pic=buf
		  tv, pic
		  for i=0,n-1 do begin
		      tv,[[pvalue]],holder(i,0),holder(i,1)
		      pic(holder(i,0),holder(i,1))=pvalue
		      stptr=0
		  endfor
		   cursor,xp,yp,device=1,/nowait 
		    print,'Move cursor to desired point'
		    print,'When cursor shows match, drag point with left button down'
		    print, ' '
		    print,'Click middle button  to add or'
		    print, ' eliminate a control point'
		    print, ' '
		    print,'Exit mode  with right button' 
		   cursor,xp,yp,device=1,/nowait 
                    xpl=0 
                    ypl=0 
		   while( !err ne 4) do begin
		    cursor,xp,yp,device=1,/nowait
		    cnt=0
		    cl=0
		    for i=0,n-1 do begin
		      if(abs( xp-holder(i,0)) lt 3  and abs(yp-holder(i,1)) lt 3) then begin
			  cl=1
			  cnt=i
		      endif
		    endfor
		      if (cl eq 1) then begin
			 device,cursor_standard=38
			 while(!err eq 1) do begin
			   cursor,xp,yp,device=1,/nowait
                 if xp ne -1 then begin
			 tv,[[buf(holder(cnt,0),holder(cnt,1))]],holder(cnt,0),holder(cnt,1)
			pic(holder(cnt,0),holder(cnt,1))=buf(holder(cnt,0),holder(cnt,1))
                 holder(cnt,0)=xp
                 holder(cnt,1)=yp
                 tv,[[pvalue]],xp,yp
                  pic(xp,yp)=pvalue
                 endif
                 endwhile
                   while(!err eq 2) do begin
                     print,"press 'a' to add a point; 'r' to remove"
                     answer=get_kbrd(1.0)
                     if answer eq 'a' then begin
                      print,'snap on to new point'
                       cursor,xp,yp,device=1
                      dum=holder
                      holder=intarr(n+1,2)
                       holder(0:cnt,*)=dum(0:cnt,*)
                      if (n-cnt-2) gt 0 then holder(cnt+2:n,*)=dum(cnt+1:n-1,*) 
                      holder(cnt+1,*)=[xp,yp]
                      n=n+1
                      tv, [[pvalue]],xp,yp
                      pic(xp,yp)=pvalue
                     endif
                     if answer eq 'r' then begin
                      dum=holder
                      xd=holder(cnt,0)
                      yd=holder(cnt,1)
                      holder=intarr(n-1,2)
                      ; cursor,xd,yd,device=1,/nowait
                      if (cnt-1 gt 0) then holder(0:cnt-1,*)=dum(0:cnt-1,*)
                      if ((n-2-cnt)gt 0) then holder(cnt:n-2,*)=dum(cnt+1:n-1,*) 
                      n=n-1
                      pic(xd,yd)=buf(xd,yd)
                      tv, [[pic(xd,yd)]],xd,yd
                     endif
                      !err=0 
                     endwhile
              endif else begin
                 device,/cursor_original
              endelse
           endwhile
       endif  
endwhile
   sc=tvmenu1(['SAVE EDITS','DISCARD EDITS'],' ',350,500)
   if sc eq 1 then pic=buf
end
