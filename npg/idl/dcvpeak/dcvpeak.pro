; $Id: dcvpeak.pro,v 1.2 2010/05/04 16:35:12 tom Exp tom $
;
;+
; NAME:
;   DCVPEAK
;
; PURPOSE:
;   Get peak time, amplitude, and width of a blood curve (read from dcv file).
;
; CATEGORY:
;   PET processing.
;
; CALLING SEQUENCE:
;   dcvpeak FNAME THRESH INPUT
;
; INPUT PARAMETERS:
;	FNAME  = dcv file name for deconvolved arterial bloodcurve
;	THRESH = threshold based on peak (fraction between 0 and 1)
;	INPUT  = 0 (default) for no interactive input with plot; 1 to show plot
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, May 2010.
;-

PRO dcvpeak

;	Initializations

   on_error,2
   str=''
   str1=''
   fname=''
   thresh=0.0
   takeoff=0.0
   num = 0
   n1 = 0
   n2 = 0
   nmax = 0
   nlo = 0
   nhi = 0
   t1 = 0.0
   t2 = 0.0
   max = 0.0
   input = 0
   fname = getenv('FNAME')
   thresh = getenv('THRESH')
   input = getenv('INPUT')
   takeoff = getenv('TAKEOFF')

   if (fname eq '') then exit
   openr,lu,fname,/get_lun
   command = 'wc ' + fname
   spawn, command, result
   reads, result, num
   if (num eq 0) then begin
      print, fname, ' is empty'
   endif

   readf,lu,str
   crv = fltarr(2,num-1)
   readf,lu,crv
   close,lu
   free_lun,lu

   time = crv(0,*)
   cnts = crv(1,*)

   n1 = 3
   n2 = num-2
   if (cnts[n2] gt 0) then n2 = n2-1
   if (cnts[n2] gt 0) then n2 = n2-1 else n2 = num-2
   if (cnts[n2] gt 0) then n2 = num-2		;; hand-sampled curve
   ;; if more than 2 points at end of curve are non-zero, this is a hand-sampled curve

   while (cnts[n2] eq 0) do n2 = n2-1
   for n=n1,n2 do begin
      if (cnts[n] gt max) then begin
         nmax = n
         max = cnts[nmax]
      endif
   endfor

   n = nmax
   while ((cnts[n] gt max*thresh) AND (n lt n2)) do n = n+1
   nhi = n

   n = nmax
   while ((cnts[n] gt max*thresh) AND (n gt 2)) do n = n-1
   nlo = n

   n = nmax
   while ((cnts[n] gt max*takeoff) AND (n gt 2)) do n = n-1
   n0 = n

   ;; Check if the upper limit is at the end of the curve 
   ;; Note that the deconvolved curve will be > 0 at the end; hand-sampled curve will have no 0's
   char = " "
   val1 = cnts[nhi]
   val2 = cnts[nhi+1]
   val3 = cnts[num-4]
   if (nhi eq n2) then char = "*" $
   else if (val3 ne 0) then char = " " $
   else if (val2 lt 0.8*val1) then char = "*"

   if (input gt 0) then begin
      scrnsize = get_screen_size()
      xloc = scrnsize[0]/2
      IF (xloc GT 1150) THEN xloc = 1150
      loadct,5
      window, xpos=xloc, colors=10, xsize=750, ysize=600
      loadct,39
      title = strcompress(fname,/remove_all)
      if (input gt 0) then plot, time, cnts, charsize=1.5, xtitle='sec', ytitle='counts', title=title
      print, time[n0], time[nmax]-time[n0], time[nmax], cnts[nmax], time[nhi]-time[nlo], $
         FORMAT='("Takeoff:", F5.1,"  Rise:",F5.1,"  Peak:",F5.1,"  Amplitude:",F11.1,"  Width:",F5.1," ",A1)'
      val = max*thresh
      oplot, [time[nlo],time[nhi]],[val,val], color=240, thick=2
      oplot, [time[n0],time[n0]],[0,max/5], color=240, thick=1
      oplot, [time[nmax],time[nmax]],[0.95*max, 1.05*max], color=240, thick=1
      read, prompt='Continue ', str
   endif else print, fname, time[n0], time[nmax]-time[n0], time[nmax], cnts[nmax], time[nhi]-time[nlo], char,$
         FORMAT='(A,F8.1,F8.1,F8.1,F12.1,F8.1,X,A1)'

end
