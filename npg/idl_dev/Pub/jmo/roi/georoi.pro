;*********************************************
function circle_plot,center,radius,color=color
;*********************************************

if not (keyword_set(color)) then color = 175

x = dblarr(60)
y = dblarr(60)

anglestep = !dpi / 30
startangle = 0.0

for i = 0, 59 do begin

   x(i) = center(0) + (sin(startangle) * radius)
   y(i) = center(1) + (cos(startangle) * radius)
   startangle = startangle + anglestep

endfor

plots,x,y,color=color,/dev,/noclip

verts = intarr(60,2)
verts(*,0) = x
verts(*,1) = y

return,verts
end

;*******************************************
function square_plot,corner,edge,color=color
;*******************************************

if not (keyword_set(color)) then color = 175

x = intarr(5)
y = intarr(5)

x(0) = corner(0)
y(0) = corner(1)
x(1) = x(0) + edge
y(1) = y(0)
x(2) = x(1)
y(2) = y(0) + edge
x(3) = x(0)
y(3) = y(2)
x(4) = x(0)
y(4) = y(0)

plots,x,y,color=color,/dev,/noclip

verts = intarr(5,2)
verts(*,0) = x
verts(*,1) = y

return,verts
end

;**********************************************
function defroi_jwu,Sx,Sy,X0=x0,Y0=y0,ZOOM=zoom
;**********************************************

common define_comm,circle,square,done,radius,edge,rset,eset,x,y,z, $
       xverts,yverts,verts,sx1,sy1,nc1     

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path, $
       regpath,wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel, $ 
       wsave,wload,wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype, $
       image_index,plot_index,woprof,lcol,wdone,wslicer,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       wtiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd

on_error,2
rset = !FALSE
eset = !FALSE

if n_elements(x0) le 0 then x = 0 else x = x0 
if n_elements(y0) le 0 then y = 0 else y = y0 
if n_elements(zoom) le 0 then z = 1 else z = zoom 
sx1 = sx
sy1 = sy

background = widget_base(title = 'Define ROI',/COLUMN)
worktop = widget_base(background,/ROW)
circle = widget_button(worktop,value = '     Circle     ')
square = widget_button(worktop,value = '     Square     ')
done = widget_button(worktop,value = '      Done      ')
radius = widget_slider(background,title='Radius of Circle',MIN=1,MAX=128) 
edge = widget_slider(background,title='Edge of Square',MIN=1,MAX=256)

widget_control,background,/REALIZE
xmanager,'defroi_jwu',background,/MODAL

return,verts 
end

;**********************
pro defroi_jwu_event,ev
;**********************

common define_comm,circle,square,done,radius,edge,rset,eset,x,y,z, $
       xverts,yverts,verts,sx1,sy1,nc1    

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path, $
       regpath,wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel, $ 
       wsave,wload,wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype, $
       image_index,plot_index,woprof,lcol,wdone,wslicer,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       wtiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd

case ev.id of

   circle: begin
           widget_control,square,SENSITIVE = 0
           widget_control,edge,SENSITIVE = 0
           end

   square: begin
           widget_control,circle,SENSITIVE = 0
           widget_control,radius,SENSITIVE = 0
           end

   radius: rset = !TRUE

   edge: eset = !TRUE

   done: begin
         wset,image_index

         if (rset eq !TRUE) then begin
            widget_control,radius,get_value = rad 
            Cursor, xx, yy, /WAIT, /DEV
            repeat begin
               xx = (xx - x) / z
               yy = (yy - y) / z
               if (xx lt sx1) and (yy lt sy1) and (!err eq 1) then $ 
                  verts = circle_plot([xx,yy],rad) 
               Cursor, xx, yy, /WAIT, /DEV
            endrep until !err eq 4
         endif 

         if (eset eq !TRUE) then begin
            widget_control,edge,get_value = side
            Cursor, xx, yy, /WAIT, /DEV
            repeat begin
               xx = (xx - x) / z
               yy = (yy - y) / z
               if (xx lt sx1) and (yy lt sy1) and (!err eq 1) then $ 
                  verts = square_plot([xx,yy],side) 
               Cursor, xx, yy, /WAIT, /DEV
            endrep until !err eq 4
         endif 

         if ((rset eq !FALSE) and (eset eq !FALSE)) then $
            message,'You must set either the radius or the edge size.'

         print,ev.top
         widget_control,ev.top,/DESTROY
         end
   endcase
end

