; $Id: print_bloodcrv.pro,v 1.1 2004/05/20 16:10:55 tom Exp $
;
;+
; NAME:
;   PRINT_BLOODCRV
;
; PURPOSE:
;   Print blood curve and head curves.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   print_bloodcrv, bcrv, hcrv, t1, t2
;
; INPUTS:
;   bcrv:		time-activity curve with time = bcrv(0,*) and activity = bcrv(1,*)
;   hcrv:		time-activity curve with time = hcrv(0,*) and activity = hcrv(1,*)
;	t1, t2:		range (sec)
;
; OUTPUTS:
;	None.
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, May 2004
;-

pro print_bloodcrv, bcrv, hcrv, t1, t2, name, start, length, shift

    printname = '/tmp/cos_plot.ps'
    set_plot,'ps'
    device,filename=printname,xsize=7,ysize=6,xoffset=0.8,yoffset=3,/inches
	plot, hcrv(0,*), hcrv(1,*), /nodata, charsize=1.5, xrange=[t1,t2]
	oplot, hcrv(0,*), hcrv(1,*), color=9, thick=2, linestyle=2
	oplot, bcrv(0,*), bcrv(1,*), color=8, thick=2, linestyle=0
	xyouts, 0.8, 0.65, "Head", /normal, color=9, size=1.5
	xyouts, 0.8, 0.60, "Artery", /normal, color=8, size=1.5
	str = string(format='(A,"   Start:",I3,"  Length:", I3,"  Shift:",I3)', name, start, length, shift)
	xyouts, 0.20, -0.10, str, /normal, color=9, size=1.5
    device,/close
    cmd = 'cat '+printname+' | lpr'
    spawn, cmd
    cmd = '/usr/bin/rm '+printname
    spawn, cmd
    set_plot,'x'
end
