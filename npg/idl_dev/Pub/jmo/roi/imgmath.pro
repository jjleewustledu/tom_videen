;**********
pro imgmath 
;**********

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path, $ 
       regpath,wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel, $
       wsave,wload,wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype, $
       image_index,plot_index,woprof,lcol,wdone,wslicer,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       stiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd
 
common calc_comm,wsum,wdif,wmlt,wdiv,wabs,wsmo,wsmo1,wsmo2,wlog,wexp, $
       wscl,wpad,wlp,wrp,wmdone,expression,dimage1,dimage2, $
       dimage3,dimage4,mathbase,wcan,wval,mathval,wmfile,fil1,fil2, $
       fil3,fil4,get1,get2,get3,get4,stack,wlab1,wlab2,wlab3,wlab4, $
       wimage1,wimage2,wimage3,wimage4,intexp,open,listname  

common images,a,b,c,d,e,f,g,h,ii,j,file_names,file_onlies,paths,nfiles, $
       current_file,nblt,bltimg,list_names 

common getpln_comm,wpln_min,wpln_max,wok,plnfrm_sel,wfrm,maxfrm,wdat, $
       maxdata,mindata

get1 = !FALSE
get2 = !FALSE
get3 = !FALSE
get4 = !FALSE
fil1 = 0
fil2 = 0
fil3 = 0
fil4 = 0
intexp = intarr(100)
open = 0

mathbase = widget_base(title = 'Image Math',/COLUMN)
mathtop = widget_base(mathbase,/COLUMN)
mathtoplcol = widget_base(mathtop,/ROW)
expression = widget_text(mathtop,value = 'Expression:') 

mathmid = widget_base(mathtop,/ROW)
mathmid1 = widget_base(mathtop,/ROW)
mathmid2 = widget_base(mathtop,/ROw)
mathmid3 = widget_base(mathtop,/ROW)
mathmid4 = widget_base(mathtop,/ROW) 
mathmid5 = widget_base(mathtop,/ROW)
mathmid6 = widget_base(mathtop,/ROW)
mathmid7 = widget_base(mathtop,/ROW)
mathmid8 = widget_base(mathtop,/ROW)
mathmid9 = widget_base(mathtop,/ROW)
mathbot = widget_base(mathtop,/ROW)

wval = cw_field(mathmid5,title = 'Scale Factor: ',value = '1', $
       /RETURN_EVENTS,XSIZE = 35)
widget_control,wval,SENSITIVE = 0
dimage1 = widget_button(mathmid6,value = 'Image 1: ')
dimage2 = widget_button(mathmid7,value = 'Image 2: ') 
dimage3 = widget_button(mathmid8,value = 'Image 3: ')
dimage4 = widget_button(mathmid9,value = 'Image 4: ')
wlab1 = widget_label(mathmid6,value = '')
wlab2 = widget_label(mathmid7,value = '')
wlab3 = widget_label(mathmid8,value = '')
wlab4 = widget_label(mathmid9,value = '')

wsum = widget_button(mathmid, value = '     +     ')
wdif = widget_button(mathmid, value = '     -     ')
wmlt = widget_button(mathmid, value = '     *     ')
wdiv = widget_button(mathmid, value = '     /     ')
wabs = widget_button(mathmid1, value = '    Abs    ')
wsmo = widget_button(mathmid1, value = '  Smooth3  ')
wsmo1 = widget_button(mathmid1, value = '  Smooth5  ')
wsmo2 = widget_button(mathmid1, value = '  Smooth7  ')
wlog = widget_button(mathmid2, value = '    Log    ')
wexp = widget_button(mathmid2, value = '    Exp    ')
wscl = widget_button(mathmid2, value = '   Scale   ')
wpad = widget_button(mathmid2, value = '    Pad    ')
wlp = widget_button(mathmid3, value = '     (     ')
wrp = widget_button(mathmid3, value = '     )     ')
wimage1 = widget_button(mathmid3, value = '  Image 1  ')
wimage2 = widget_button(mathmid3, value = '  Image 2  ')
wimage3 = widget_button(mathmid4, value = '  Image 3  ')
wimage4 = widget_button(mathmid4, value = '  Image 4  ')
wmdone = widget_button(mathmid4, value = '   Done    ')
wcan = widget_button(mathmid4, value = '   Cancel  ')

wmfile = widget_list(mathbot,VALUE = list_names,YSIZE=5,XSIZE=35)

widget_control,mathbase,/REALIZE 
xmanager,'imgmath',mathbase,/MODAL 

return
end

;*******************
pro imgmath_event,ev
;*******************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path, $
       regpath,wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel, $
       wsave,sload,wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype, $
       image_index,plot_index,woprof,lcol,wdone,wsliver,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       wtiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $ 
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd
       
common calc_comm,wsum,wdif,wmlt,wdiv,wabs,wsmo,wsmo1,wsmo2,wlog,wexp, $
       wscl,wpad,wlp,wrp,wmdone,expression,dimage1,dimage2, $
       dimage3,dimage4,mathbase,wcan,wval,mathval,wmfile,fil1,fil2, $
       fil3,fil4,get1,get2,get3,get4,stack,wlab1,wlab2,wlab3,wlab4, $
       wimage1,wimage2,wimage3,wimage4,intexp,open,listname       

common images,a,b,c,d,e,f,g,h,ii,j,file_names,file_onlies,paths,nfiles, $
       current_file,nblt,bltimg,list_names
        

case ev.id of

   wsum: begin 
         widget_control,expression,set_value = '+',/APPEND        
         intexp(open) = 13
         open = open + 1
         end

   wdif: begin
         widget_control,expression,set_value = '-',/APPEND
         intexp(open) = 14
         open = open + 1
         end

   wmlt: begin 
         widget_control,expression,set_value = '*',/APPEND
         intexp(open) = 15
         open = open + 1
         end

   wdiv: begin 
         widget_control,expression,set_value = '/',/APPEND
         intexp = 16
         open = open + 1
         end

   wabs: begin 
         widget_control,expression,set_value = 'Abs',/APPEND
         intexp(open) = 5
         open = open + 1
         end

   wsmo: begin
         widget_control,expression,set_value = 'Smo3',/APPEND
         intexp(open) = 10
         open = open + 1
         end

   wsmo1: begin 
          widget_control,expression,set_value = 'Smo5',/APPEND
          intexp(open) = 11
          open = open + 1
          end

   wsmo2: begin 
          widget_control,expression,set_value = 'Smo7',/APPEND
          intexp(open) = 12
          open = open + 1
          end

   wlog: begin 
         widget_control,expression,set_value = 'Log',/APPEND
         intexp(open) = 7
         open = open + 1
         end

   wexp: begin 
         widget_control,expression,set_value = 'Exp',/APPEND
         intexp(open) = 6
         open = open + 1
         end

   wscl: begin 
         widget_control,expression,set_value = 'Scl',/APPEND
         widget_control,wval,/SENSITIVE
         intexp(open) = 9
         open = open + 1
         end

   wpad: begin
         widget_control,expression,set_value = 'Pad',/APPEND
         intexp(open) = 8
         open = open + 1
         end
 
   wcan: widget_control,ev.top,/DESTROY 

   wlp: begin
        widget_control,expression,set_value = '(',/APPEND
        intexp(open) = 17
        open = open + 1
        end

   wrp: begin
        widget_control,expression,set_value = ')',/APPEND
        intexp(open) = 18
        open = open + 1
        end

   wval: begin
         widget_control,wval,GET_VALUE = smathval
         mathval = float(smathval)
         end

   wimage1: begin
            widget_control,expression,set_value = 'Img1',/APPEND
            intexp(open) = 1
            open = open + 1
            end

   wimage2: begin
            widget_control,expression,set_value = 'Img2',/APPEND
            intexp(open) = 2
            open = open + 1 
            end
  
   wimage3: begin
            widget_control,expression,set_value = 'Img3',/APPEND
            intexp(open) = 3
            open = open + 1
            end

   wimage4: begin
            widget_control,expression,set_value = 'Img4',/APPEND
            intexp(open) = 4
            open = open + 1
            end

   dimage1: begin
            get1 = !TRUE
            widget_control,dimage1,SENSITIVE = 0
            end

   dimage2: begin
            get2 = !TRUE
            widget_control,dimage2,SENSITIVE = 0
            end

   dimage3: begin
            get3 = !TRUE
            widget_control,dimage2,SENSITIVE = 0
            end

   dimage4: begin
            get4 = !TRUE
            widget_control,dimage2,SENSITIVE = 0
            end

   wmfile: begin
           if (get1 eq !TRUE) then begin
              get1 = !FALSE
              fil1 = ev.index
              widget_control,wlab1,set_value=list_names(fil1)
              widget_control,dimage1,/SENSITIVE
              widget_control,wimage1,/SENSITIVE
           endif else begin 
              if (get2 = !TRUE) then begin
                 get2 = !FALSE
                 fil2 = ev.index
                 widget_control,wlab2,set_value=list_names(fil2)
                 widget_control,dimage2,/SENSITIVE
                 widget_control,wimage2,/SENSITIVE
              endif else begin 
                 if (get3 = !TRUE) then begin
                    get3 = !FALSE
                    fil3 = ev.index
                    widget_control,wlab3,set_value=list_names(fil3)
                    widget_control,dimage3,/SENSITIVE
                    widget_control,wimage3,/SENSITIVE
                 endif else begin 
                    if (get4 = !TRUE) then begin
                       get4 = !FALSE
                       fil4 = ev.index
                       widget_control,wlab4,set_value=list_names(fil4)
                       widget_control,dimage4,/SENSITIVE
                       widget_control,wimage4,/SENSITIVE
                    endif
                 endelse
              endelse
           endelse
           end
 
   wmdone: begin
           widget_control,expression,get_value=entered 
           print,entered 
           stat = call_external(!SHARE_LIB,'_calc', intexp, /F_VALUE) 
           widget_control,ev.top,/DESTROY
           eval,intexp
           end

endcase
return
end

;******************
pro eval,reverseexp
;******************

common roi_comm,base,mcol,rcol,files,wsld_pln,wpln_file,wpln_path,path, $
       regpath,wroinum,wplane,wvol,werror,wdisp,wdreg,wsreg,wdel, $
       wsave,wload,wldcub,wdelall,wdisplay,wdraw,wfiles,whprof,imgtype, $
       image_index,plot_index,woprof,lcol,wdone,wslicer,wsurf,fileonly, $
       wzoomin,wzoomout,wmovie,wnextpln,wlstpln,wdelfil,wcolor,wldcti, $
       stiff,whprofsave,wmath,wnorm,wmask,wtrans,view,wldctiblt,wanno, $
       wxwd,displayglob,wvprof,horzprof,profcmd,profdat,roimode,fl,wd

common calc_comm,wsum,wdif,wmlt,wdiv,wabs,wsmo,wsmo1,wsmo2,wlog,wexp, $
       wscl,wpad,wlp,wrp,wmdone,expression,dimage1,dimage2, $
       dimage3,dimage4,mathbase,wcan,wval,mathval,wmfile,fil1,fil2, $
       fil3,fil4,get1,get2,get3,get4,stack,wlab1,wlab2,wlab3,wlab4, $
       wimage1,wimage2,wimage3,wimage4,intexp,open,listname    

common images,a,b,c,d,e,f,g,h,ii,j,file_names,file_onlies,paths,nfiles, $
       current_file,nblt,bltimg,list_names

common evcomm,filnam,plane,image,xdim,ydim,maxpln,roi,nverts,nroi,vol, $
       roinum,zoom,nprof,wprtplt,wcolscl,image_minmax,blt_minmax, $
       error,wsag,wcor,mcustom,mview,filindex,st

stack = replicate({node}, 4)
avail = 0
index = 0

while reverseexp(index) ne 20 do begin
   case reverseexp(index) of

   1: begin
         stack(avail) = get_image(list_names(fil1))
         avail = avail + 1
      end

   2: begin 
         stack(avail) = get_image(list_names(fil2))
         avail = avail + 1
      end

   3: begin
         stack(avail) = get_image(list_names(fil3))
         avail = avail + 1
      end

   4: begin
         stack(avail) = get_image(list_names(fil4))
         avail = avail + 1
      end

   5: begin
         stack(avail - 1).img = abs(stack(avail - 1).img)
         stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
         stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
         stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                 (0,0,*)) - 1
         listname = strcompress(string(list_names(fil1),'.abs'), $
                    /REMOVE_ALL)
         fileonly = listname
      end

   6: begin
         stack(avail - 1).img = exp(stack(avail - 1).img)
         stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
         stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
         stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                 (0,0,*)) - 1
         listname = strcompress(string(list_names(fil1),'.exp'), $
                    /REMOVE_ALL)
         fileonly = listname 
      end

   7: begin
         stack(avail - 1).img = alog(stack(avail - 1).img)
         stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
         stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
         stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                 (0,0,*)) - 1
         listname = strcompress(string(list_names(fil1),'.log'), $
                    /REMOVE_ALL)
         fileonly = listname
      end

   8: begin
         zdim = n_elements(stack(avail - 1).img(0,0,*))
         xdima = n_elements(stack(avail - 1).img(*,0,0))
         ydima = n_elements(stack(avail - 1).img(0,*,0))
         label = string("New x dimension: ")
         xdimb = fix(get_str(1,label,xdima))
         widget_control,/HOURGLASS
         bimage = fltarr(xdimb(0),ydima,zdim) 
         pad = (xdimb(0) - xdima) / 2
         if (pad gt 0) then begin
            bimage(pad:pad + xdima - 1,*,*) = stack(avail - 1).img(*,*,*)
         endif else begin
            bimage(*,*,*) = stack(avail - 1).img(*,*,*)
         endelse
         stack(avail - 1).xdim = xdimb(0)
         stack(avail - 1).img(*,*,*) = bimage(*,*,*) 
         listname = strcompress(string(list_names(fil1),'_pad', $
                    xdimb(0)),/REMOVE_ALL)
         fileonly = listname
      end 

   9: begin
         if (n_elements(mathval) eq 0) then begin
            widget_control,werror,SET_VALUE=string('*** Scale' + $ 
                'Factor is undefined. ***') 
            return
         endif
         stack(avail - 1).img = mathval(0) * stack(avail - 1).img  
         stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
         stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
         stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                 (0,0,*)) - 1
         listname = strcompress(string(list_names(fil1),'.scl_', $
                    mathval),/REMOVE_ALL)
         fileonly = listname
      end

   10: begin
          stack(avail - 1).img = smooth(stack(avail - 1).img,3)
          stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
          stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
          stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                  (0,0,*)) - 1
          listname = strcompress(string(list_names(fil1),'.smo3'), $
                     /REMOVE_ALL)
          fileonly = listname
       end

   11: begin
          stack(avail - 1).img = smooth(stack(avail - 1).img,5)
          stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
          stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
          stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                  (0,0,*)) - 1
          listname = strcompress(string(list_names(fil1),'.smo5'), $
                     /REMOVE_ALL)
          fileonly = listname
       end

   12: begin
          stack(avail - 1).img = smooth(stack(avail - 1).img,7)
          stack(avail - 1).xdim = n_elements(stack(avail - 1).img(*,0,0))
          stack(avail - 1).ydim = n_elements(stack(avail - 1).img(0,*,*))
          stack(avail - 1).zmax = n_elements(stack(avail - 1).img $
                                  (0,0,*)) - 1
          listname = strcompress(string(list_names(fil1),'.smo7'), $
                     /REMOVE_ALL)
          fileonly = listname
       end

   13: begin
          stack(avail - 2).img = stack(avail - 2).img + stack(avail - 1).img 
          stack(avail - 2).xdim = n_elements(stack(avail - 2).img(*,0,0))
          stack(avail - 2).ydim = n_elements(stack(avail - 2).img(0,*,*))
          stack(avail - 2).zmax = n_elements(stack(avail - 2).img $
                                  (0,0,*)) - 1
          stack(avail - 1).img(*,*,*) = 0
          avail = avail - 1
          listname = strcompress(string(list_names(fil1),'+', $
                     list_names(fil2)),/REMOVE_ALL)
          fileonly = listname
       end

   14: begin
          stack(avail - 2).img = stack(avail - 2).img - stack(avail - 1).img 
          stack(avail - 2).xdim = n_elements(stack(avail - 2).img(*,0,0))
          stack(avail - 2).ydim = n_elements(stack(avail - 2).img(0,*,*))
          stack(avail - 2).zmax = n_elements(stack(avail - 2).img $
                                  (0,0,*)) - 1
          stack(avail - 1).img(*,*,*) = 0
          avail = avail - 1
          listname = strcompress(string(list_names(fil1),'-', $
                     list_names(fil2)),/REMOVE_ALL)
          fileonly = listname
        end

   15: begin
          stack(avail - 2).img = stack(avail - 2).img * stack(avail - 1).img 
          stack(avail - 2).xdim = n_elements(stack(avail - 2).img(*,0,0))
          stack(avail - 2).ydim = n_elements(stack(avail - 2).img(0,*,*))
          stack(avail - 2).zmax = n_elements(stack(avail - 2).img $
                                  (0,0,*)) - 1
          stack(avail - 1).img(*,*,*) = 0
          avail = avail - 1
          listname = strcompress(string(list_names(fil1),'_mlt_', $
                     list_names(fil2)),/REMOVE_ALL)
          fileonly = listname
        end

   16: begin
          stack(avail - 1).img = stack(avail - 1).img > 1
          stack(avail - 2).img = stack(avail - 2).img / stack(avail - 1).img 
          stack(avail - 2).xdim = n_elements(stack(avail - 2).img(*,0,0))
          stack(avail - 2).ydim = n_elements(stack(avail - 2).img(0,*,*))
          stack(avail - 2).zmax = n_elements(stack(avail - 2).img $
                                  (0,0,*)) - 1
          stack(avail - 1).img(*,*,*) = 0
          avail = avail - 1
          listname = strcompress(string(list_names(fil1),'_div_', $
                     list_names(fil2)),/REMOVE_ALL)
          fileonly = listname
        end

   else: print,'Eureka!'

   endcase
   index = index + 1
endwhile

put_image,stack(0).img,stack(0).xdim,stack(0).ydim,stack(0).zmin,$
       stack(0).zmax,listname,listname,stack(0).dxdy,stack(0).dz 

end 
