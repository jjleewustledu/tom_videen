; $Id: plot_bloodcrv.pro,v 1.3 2004/05/20 16:10:55 tom Exp $
;
;+
; NAME:
;   PLOT_BLOODCRV
;
; PURPOSE:
;   Plot blood curve and head curves.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   plot_bloodcrv, bcrv, hcrv, t1, t2
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
;   Written by: Tom Videen, Nov. 1995
;-

pro plot_bloodcrv, bcrv, hcrv, t1, t2

	set_plot,'x'
	plot, hcrv(0,*), hcrv(1,*), /nodata, charsize=1.5, xrange=[t1,t2]
	oplot, hcrv(0,*), hcrv(1,*), color=9, thick=2
	oplot, bcrv(0,*), bcrv(1,*), color=8, thick=2
	xyouts, 0.8, 0.85, "Head", /normal, color=9, size=2.0
	xyouts, 0.8, 0.80, "Artery", /normal, color=8, size=2.0
end
