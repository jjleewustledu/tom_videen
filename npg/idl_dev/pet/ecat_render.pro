;+
; NAME:
;	ECAT_RENDER
;
; PURPOSE:
;	Create a shaded surface display from an ecat data set.
;
; CATEGORY:
;	CTI utility.
;
; CALLING SEQUENCE:
;	ecat_render, win, img, threshold
;
; INPUTS:
;	WIN:	Specified window for image display.
;	
;	IMG:	Ecat data structure - data to be displayed.
;
;	THRESHOLD:	The threshold to be used when creating shaded volume.
;
;
; OPTIONAL INPUT PARAMETERS:
;	None.
;
; KEYWORD PARAMETERS:
;	None.
;
; OUTPUTS:
;
;	Displays the shaded surface image from the input data
;
; COMMON BLOCKS:
;	None.
;
; SIDE EFFECTS:
;	This procedure takes a relatively long time to execute for datasets
;	with more than 15 planes.
;
; RESTRICTIONS:
;	The shaded volume will occupy the entire window you have selected
;	for display. The larger the display window, the longer the procedure
;	will take.
;
; PROCEDURE:
;
;	The window is erased.
;	Shaded surface image is created
;	Shaded surface is displayed
;
;
; MODIFICATION HISTORY:
;	EMP, 10/92.
;-

;----------------------------------------
;	@(#)ecat_render.pro	1.1  10/30/92
;----------------------------------------
; Create a shaded surface display from ecat data 
;----------------------------------------

pro ecat_render, win, img, thresh

	; Set the window number 
	win_erase, win

	s = size(img.data)

	slice_width = img.sh.slice_width
	p_size = img.sh.pixel_size
	plane_sep = img.mh.scan.plane_separation
	zsclfct=plane_sep(0)*s(3)/(s(1)*p_size)

	volmax = max(img.data)

	!order=1

	thresh = .3
	surface,fltarr(2,2),/nodata,/save,xrange=[0,s(1)-1],$
		yrange=[0,s(2)-1],zrange=[0,s(3)-1],color=0

 	shade_volume,img.data,volmax*thresh,v,p,LOW=1
	
	surfr,az=0,ax=0
	!x.s=[0,1.]/(s(1)-1)
	!y.s=[0,1.]/(s(2)-1)
	!z.s=[0,1.]*zsclfct/(s(3)-1)

	t3d,/reset,tr=[-.5,-.5,-.5*zsclfct],rot=[-90,0.*15,0]
	t3d,tr=[.5,.5,0]

	; display the shaded volume
	tvscl, polyshade(v,p,/t3d)
END
