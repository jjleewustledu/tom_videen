PRO plotmannitol
    
   str = " "
	rows = 0
	cols = 0
   pchar = ''

   rd = findgen(32) * (!pi*2/32.)
   cmag = 1.0
   usersym, cmag*cos(rd), cmag*sin(rd), /fill
   files = ['patient1.dat','patient2.dat','patient3.dat','patient4.dat','patient6.dat','patient7.dat']
   set_plot,'X'

   openr,1,files[0]
   readf,1,str
   readf,1,rows,cols
   y = fltarr(cols,rows)
   readf,1,y
   close,1
   time = y[1,*]
   val = y[5,*]
   plot, time, val, xr=[0,70], yr=[-1.2, 0.3], psym=-8, linestyle=0
   oplot,[0,80],[0,0]

   for i=1,5 do begin
   openr,1,files[i]
   readf,1,str
   readf,1,rows,cols
   y = fltarr(cols,rows)
   readf,1,y
   close,1
   time = y[1,*]
   val = y[5,*]
   oplot, time, val, psym=-8, linestyle=0
   endfor
   read,prompt='exit? ',pchar

END 
