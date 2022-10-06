PRO PLOT_ALIGNDYN2
   nm = 8                       ; number of values/line (frm, mean, x, y, z, x, y, z)
   nc = 1                       ; number of data lines
   str = ''
   pchar = ''
   fname = getenv('FILENAME')
   IF (fname EQ ' ') THEN read,prompt='Log filename: ',fname
   str = getenv('DMAX')
   IF (str NE '') THEN dmax = fix(str) ELSE dmax = 0
   str = getenv('AMAX')
   IF (str NE '') THEN amax = fix(str) ELSE amax = 0

   len = rstrpos(fname,'.')
   fid = strmid(fname,0,len)
   cmd = 'wc '+fname+' | cut -c1-8'
   spawn, cmd, result
   reads, result, nc            ; number of lines
   nc = nc-1
   x = fltarr(nm, nc)
   openr,1,fname
   readf,1,str
   str = ''
   val = fltarr(8)
   FOR n=1,nc DO BEGIN
      readf, 1, val
      x[*,n-1] = val
   ENDFOR
   close, 1
   free_lun, 1

distances:
   IF (dmax LE 0) THEN BEGIN
      dmax =  max(x[1,*], min=dmin)
      print,dmax,dmin
      read,prompt='Plot Maximum: ',dmax
   ENDIF
   dmin = -dmax

save_ps:
   IF (pchar EQ 'p') THEN BEGIN
      set_plot,'ps'
      fname = fid+'.ps'
      device,filename=fname,xsize=7,ysize=10,xoffset=0.8,yoffset=0.5,/inches
   ENDIF ELSE BEGIN
      set_plot,'X'
      window,0,xsize=630,ysize=900
      loadct,0
   ENDELSE
   !P.Multi = [2,1,2,0,0]
   
   cmag = 1.0
   rd = findgen(32) * (!pi*2/32.)
   usersym, cmag*cos(rd), cmag*sin(rd), /fill

   nx = (nc/10 + 1) * 10
   plot,x[0,*],x[1,*],yr=[dmin,dmax],xr=[0,nx], psym=-8, $
      charsize=1.4, charthick=2, xthick=2, ythick=2, $
      xtitle="Frame",ytitle="Difference (mm)", $
      title=fid
   oplot,x[0,*],x[2,*],psym=-1,linestyle=1
   oplot,x[0,*],x[3,*],psym=-2,linestyle=1
   oplot,x[0,*],x[4,*],psym=-5,linestyle=1
   oplot,[0,nx],[0,0],linestyle=0  
   
   inc = float(dmax)/8.0
   oplot,[3],[dmin+4*inc],psym=8
   oplot,[3],[dmin+3*inc],psym=1
   oplot,[3],[dmin+2*inc],psym=2
   oplot,[3],[dmin+1*inc],psym=5
   xyouts,4,dmin+4*inc,'Mean',charsize=1.5
   xyouts,4,dmin+3*inc,'X',charsize=1.5
   xyouts,4,dmin+2*inc,'Y',charsize=1.5
   xyouts,4,dmin+1*inc,'Z',charsize=1.5

angles:
   IF (amax LE 0) THEN BEGIN
      amax =  max(x[5:7,*], min=amin)
      print,amax,amin
      read,prompt='Plot Maximum: ',amax
   ENDIF
   amin = -amax

save_ps2:
   nx = (nc/10 + 1) * 10
   plot,x[0,*],x[5,*],yr=[amin,amax],xr=[0,nx], psym=-8, $
      charsize=1.4, charthick=2, xthick=2, ythick=2, $
      xtitle="Frame",ytitle="Angle (degrees)"
   oplot,x[0,*],x[6,*],psym=-2,linestyle=1
   oplot,x[0,*],x[7,*],psym=-5,linestyle=1
   oplot,[0,nx],[0,0],linestyle=0  
   
   inc = float(amax)/8.0
   oplot,[3],[amin+3*inc],psym=8
   oplot,[3],[amin+2*inc],psym=2
   oplot,[3],[amin+1*inc],psym=5
   xyouts,4,amin+3*inc,'X',charsize=1.5
   xyouts,4,amin+2*inc,'Y',charsize=1.5
   xyouts,4,amin+1*inc,'Z',charsize=1.5
   IF (pchar EQ 'p') THEN BEGIN
      device,/close
      cmd = 'cat '+fname+' | lpr'
      spawn, cmd
      cmd = '/usr/bin/rm '+fname
      spawn, cmd
   ENDIF
   read,prompt='print or exit [p,x]: x',pchar
   IF (pchar EQ 'p') THEN goto, save_ps
END 
