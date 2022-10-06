;+
; NAME: NIimage
;	
;
; PURPOSE:
;    Display a 2D image. This may be the standard IDL-output or a draw widget.
;    Use wset to change between widgets.
;    Use window and wdelete to create and delete windows.
;    NIimage interpolates the image to the full size of the output window
;    and rescales the graylevels according to the maximum and minimum values
;    specified.
;    Calls NIimage3d for 3D images.
;
; CATEGORY:
;	Widgets.
;
; CALLING SEQUENCE:
;	NIimage, image
;
; INPUTS:
;       image: the 2D image to be displayed.
;
; OPTIONAL INPUT PARAMETERS:
;
; KEYWORD PARAMETERS:
;        MINIMUM: This value corresponds to the lowest graylevel. All lower
;                 values are clipped. Default is the image minimum.
;
;        MAXIMUM: This value corresponds to the highest graylevel. All higher
;                 values are clipped. Default is the image maximum.
;
;        DRAWCOLORS: Obsolete and ignored. See NIset_ct and NIget_ct.
;
;        INTERP: When set, linear interpolation is used for zooming
;
;        CUBIC: When set, cubic b-spline interpolation is used for zooming
;
;        XSIZE, YSIZE: specify the x-and y-size in pixels. When not set, the
;               image is interpolated in order to fit in the display.
;
;        NOERASE: when set, the image is not erased before the new one is
;                 displayed. Setting this keyword speeds up the display, but
;                 it results in a messy output when the images that are
;                 displayed have not the same x and y dimensions.
;
;        CTNUM: The number of the lookup table to be used. See NIset_ct and
;               NIget_ct. Default is the current lookup table, as set by
;               NIset_ct.
;
;        USE_ALL_COLORS: when set, the image is directly applied to the lookup
;               table, so colors and grayvalues are selected directly.
;               Otherwise, the image intensities are adapted to accomodate
;               the look up table indices of table CTNUM.
;
;        XPOS, YPOS: x- and y-position in device coordinates in the widget.
;                    These coordinates are directly passed to TV.
;
;        NUMBER: set it to the color that you wish to use for the plane numbers
;                in 3D images. When not provided, images are not numbered.
;
;        FIRSTPLANE: numbering starts from this plane. Default is 1.
;
;        GLOBAL: Turns on global colorscale and numbering for 3D images.
;
; OUTPUTS:
;
; OPTIONAL OUTPUT PARAMETERS:
;
; COMMON BLOCKS:
;     reads COLORS (IDL provided common block).
;     NIct_common: common block shared by NIimage, NIset_ct and NIget_ct.
;
; SIDE EFFECTS:
;
; RESTRICTIONS:
;
; PROCEDURE:
;
; MODIFICATION HISTORY:
;	Johan Nuyts, jan-1995
; Johan Nuyts, nov-1996: adapted to support multiple look up tables.
;-
;-----------------------------------------------------------
pro NIimage, image, minimum=minimum, maximum=maximum, DRAWCOLORS=drawcolors, $
             INTERP = interp, CUBIC = cubic, XSIZE = xsize, YSIZE = ysize, $
             noerase = noerase, ctnum = ctnum, use_all_colors=use_all_colors,$
             xpos=xpos, ypos=ypos, number=number, firstplane=firstplane, $
             global=global

COMMON colors, r_orig, g_orig, b_orig, r_curr, g_curr, b_curr
common NIct_common, number_ct, ct_low, ct_high, draw_low, draw_high, $
                    current_ct

if n_elements(number_ct) eq 0 then niset_ct, ct=0, ta=0
if n_elements(ctnum) eq 0 $
   then coltable = current_ct $
   else coltable = ctnum < (number_ct - 1)

computemax = n_elements(maximum) eq 0
computemin = n_elements(minimum) eq 0
if not (computemax or computemin) then begin
  if maximum eq minimum then begin
    computemax = 1
    computemin = 1
  endif
endif

if keyword_set(use_all_colors) then begin
  mincol = 0
  maxcol = !d.table_size - 1
endif else begin
  mincol = ct_low(coltable)
  maxcol = ct_high(coltable)
endelse

dims = size(image)
IF dims(0) EQ 3 THEN BEGIN
  IF keyword_set(global) THEN BEGIN
    IF computemax THEN maximum = max(image)
    IF computemin THEN minimum = min(image)
    IF n_elements(number)  EQ 0 THEN number =  1
  ENDIF
  NIimage3d, image, minimum=minimum, maximum=maximum, $
             INTERP = interp, CUBIC = cubic, number=number, $
             firstplane=firstplane, noerase = noerase, $
             xpos=xpos, ypos=ypos, xsize=xsize, ysize=ysize
  return
ENDIF

if not KEYWORD_SET(xsize) then xsize = !d.x_size
if not KEYWORD_SET(ysize) then ysize = !d.y_size
IF n_elements(xpos) EQ 0 THEN xpos =  0
IF n_elements(ypos) EQ 0 THEN ypos =  0

if not keyword_set(noerase) then erase
if !d.name eq 'PS' then begin
  factor = 1
endif else begin
  factor1 = float(xsize) / dims(1)
  factor2 = float(ysize) / dims(2)
  factor = min([factor1,factor2])
endelse

if computemin then curminimum = min(image) $
              ELSE curminimum = minimum
if computemax then curmaximum = max(image) $
              ELSE curmaximum = maximum

if curmaximum eq curminimum then noemer = 1 $
                      else noemer = curmaximum - curminimum

if keyword_set(use_all_colors) then $

  tv,congrid(image, $
     round(dims(1) * factor), round(dims(2) * factor), INTERP=interp, $
     minus_one=interp), xpos, ypos, xsize=xsize, ysize=ysize $
else $
  tv,congrid( (((image > float(curminimum)) < curmaximum) - curminimum) $
     * (maxcol - mincol) / noemer + mincol, $
     round(dims(1) * factor), round(dims(2) * factor), INTERP=interp, $
     minus_one=interp), xpos, ypos, xsize=xsize, ysize=ysize

end
