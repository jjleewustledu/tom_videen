pro plotmott
   ifile = "p5067seg3_a.dat"
   ofile = "p5067seg3_a2.dat"
   nz = 46
   f0 = 15
   f1 = 40
   
   nf = f1-f0+1
   dat=intarr(nz,100)
   openr,1,ifile
   readf,1,dat
   close,1
   
   x=indgen(100)
   val1=fltarr(nz)
   val2=fltarr(nz)
   weights=fltarr(nf)
   weights[*]=1
   i = 0
   ch = ''

   for i=0,nz-1 do begin
      plot,x,dat[i,*]
      yy = rotate(dat[i,f0:f1],4)
      xx = rotate(indgen(nf)+f0,4)
      z=regress(xx,yy,weights,/RELATIVE_WEIGHT,yfit,y0)
      xf = [0,f1]
      yf = [y0,z*f1+y0]
      oplot,xf,yf
      ;;print,i+1, z, yf[1],format='(I4,F8.4,F8.1)'
      val1[i]=z
      val2[i]=yf[1]
      read,prompt=string(i+1)+' Continue ',ch
   endfor
   
   openw,1,ofile
   for i=0,nz-1 do printf,1,i+1,val1[i],val2[i],format='(I4,F8.4,F8.1)'
   close,1
end
