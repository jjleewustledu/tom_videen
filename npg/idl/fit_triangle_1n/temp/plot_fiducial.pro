; $Id: plot_fiducial.pro,v 1.1 1996/01/08 15:20:13 tom Exp $
;
;+
; NAME:
;	PLOT_FIDUCIAL
;
; PURPOSE:
;	Plot fiducial N points.
;
; CATEGORY:
;	Image processing.
;
; CALLING SEQUENCE:
;	PLOT_FIDUCIAL, x, y
;
; INPUTS:
;	x, y:	coordinates
;	Plot_title:	plot title
;	X_title:	x-axis label
;	Y_title:	y-axis label
;
; OUTPUTS:
;	None.
;
; PROCEDURE:
;	Plot x vs. y
;
; EXAMPLE:
;	PLOT_FIDUCIAL, x, y
;
; MODIFICATION HISTORY:
; 	Written by:	Tom Videen, 15-Dec-94
;-

pro plot_fiducial, x, y, plot_title, x_title, y_title

	xmin = min(x) - 5 
	xmax = max(x) + 5
	ymin = min(y) - 10
	ymax = max(y)
	plot, x, y, psym=3, xrange=[xmin,xmax], yrange=[ymin,ymax], $
		XTITLE=x_title, YTITLE=y_title, /nodata, charthick=2.0
end
