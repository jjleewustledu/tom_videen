;            File: $RCSfile: xdisplay.pro,v $
;          Author: $Author: williams $
;         version: $Revision: 2.2 $
;   Last Modified: $Date: 1997/01/30 15:20:48 $   
;+
; Name: XDISPALY.PRO
;
; PURPOSE: 
;   A multipurpose widget program that allows 2D or 3D images to be
;   displayed and define ROIs on them.  3 types of ROIs can be
;   defined.  Square, Circular and Freehand.  There is also a 'Magic
;   Wand' tool which allows automatic ROI drawing when in Threshold or
;   Density slice mode.  Can display images in greyscale, color, or
;   both.
;
; CATEGORY: Graphical Display's
;
; CALLING SEQUENCE:
;	widget = XDISPLAY(data[, mask])
;
; INPUTS: 
;       data = Can be either a handle to, or 2D or 3D arrays.
;
; OPTIONAL INPUTS:
;       mask = Can be either a handle to, or 2D mask for the data.
;
; KEYWORD PARAMETERS:
;       DELAY   = Time delay to begin scrolling. Default = 0.5s
;       MIN     = Will override the min of the data.  Allows for
;                 different scaling of image. Default = min(data)
;       MAX     = Will override the max of the data.  Allows for
;                 different scaling of image. Default = max(data)
;       ZOOM    = Initial zoom factor. Default = 1
;       BOTTOM  = The bottom of the greyscale LUT. Default = 0
;       NGREYS  = Number of entries in LUT for greyscale. Default is
;                 to split the colors equally between greyscale and
;                 color.
;       NCOLORS = Number of entries in LUT for color.
;       ROIPTR  = A valid handle in which to place the rois.  This
;                 allows you to share ROIs across any number of
;                 xdisplays.
;       TITLE   = The title. Default = 'Xdisplay'
;     ALLSLICE  = Apply all ROIs to all slices. Default ROIs only
;                 appear on slice that they were defined upon.
;       MODAL   = Make this a blocking widget.  Default is nonblocking.
;     JUST_REG  = Just register with the xmanager.
;       GROUP   = The group leader.  Killing the group leader also
;                 kills XDISPLAY
;
; OUTPUTS:
;       The handle to the top ROI handle
;
; COMMON BLOCKS: None.
;
; SIDE EFFECTS: 
;     XDISPLAY uses the top 9 colors of th available colors for
;     drawing.  Presently it is possible to zoom the window larger
;     than the screen.
;
; PROCEDURE:
;   Get some data
;     IDL> image = getimage()
;   Display it
;     IDL> rois = xdisplay(image)
;   To get info on any ROIS defined see help for ROI_INFO
;
;  To use xdisplay in your own widget program simply do the following:
;     roi = xdisplay(image, group = group_leader)
;
;  To share ROIs between multiple xdisplays takes a little setup
;    1) Be sure to .com xdisplay
;    2) must first setup the parent roi pointer
;          roiPTR = init_rois()   ; This is a pointer to an ROI_S structure
;    3) create a handle to hold the xy coords and assign it to roi.xy
;          roi = get_handle_value(roiPTR, /no_copy)
;          xy  = handle_create()
;          roi.xy = xy
;          set_handle_value, roiPTR, roi, /no_copy
;    4) Be sure in your cleanup / quit procedure to free the handle to
;       the roiPTR and the xy pointer.
;          roi = get_handle_value(roiPTR)
;          handle_free, roi.xy
;          handle_free, roiPTR
;
; RESTRICTIONS:
;   Only works on 8-bit displays
;
; MODIFICATION HISTORY:
;    $Log: xdisplay.pro,v $
;   Revision 2.2  1997/01/30  15:20:48  williams
;   Changed filter and filename for ROI files.
;
;   Revision 2.1  1997/01/26  19:07:40  williams
;   Added 3D support and label support to ROI_INFO.
;
;   Revision 2.0  1997/01/26  17:09:39  williams
;-
FUNCTION GETCOLORS, getColor, $
                 LOAD   = LOAD, $
                 BOTTOM = BOTTOM, $
                 INDEX  = INDEX
;--- Adapted from David Fanning's getcolor routine

;--- Always assume that the colors are starting at !d.n_colors - 9
if n_elements(BOTTOM) eq 0 then bottom = !d.n_colors - 9

;--- Set up the color vectors.
names   = ['BLACK', 'WHITE', 'RED', 'GREEN', 'BLUE', 'CYAN', 'MAGENTA', 'YELLOW']
rvalue  = [   0,      255,    255,     0,        0,     0,     255,       255   ]
gvalue  = [   0,      255,      0,   255,        0,   255,       0,       255   ]
bvalue  = [   0,      255,      0,     0,      255,   255,     255,         0   ]

names   = [ names, 'CHARCOAL' ]
rvalue  = [ rvalue,    70     ]
gvalue  = [ gvalue,    70     ]
bvalue  = [ bvalue,    70     ]

;--- Did the user ask for a specific color? If not, return all the
;    colors. If the user asked for a specific color, find out if a
;    24-bit value is required. Return to main IDL level if an error
;    occurs.
   
ON_ERROR, 1
np = N_PARAMS()
IF np EQ 1 THEN BEGIN

;--- Make sure the parameter is an uppercase string.
   
    varInfo = SIZE(getColor)
    IF varInfo(varInfo(0) + 1) NE 7 THEN $
      MESSAGE, 'The color name must be a string.'
    getColor = STRUPCASE(getColor)
    
;--- Get the color triple for this color.
    
    colorIndex = WHERE(names EQ getColor)
    
;--- If you can't find it. Issue an infomational message,
;    set the index to a RED color, and continue.
      
    IF colorIndex(0) LT 0 THEN BEGIN
        MESSAGE, "Can't find color. Returning yellow.", /INFORMATIONAL
        colorIndex = 7
    ENDIF
    
;--- Get the color triple.
    
    r = rvalue(colorIndex)
    g = gvalue(colorIndex)
    b = bvalue(colorIndex)
    returnColor = REFORM([r, g, b], 1, 3)
    idx = colorIndex(0) + bottom
    if keyword_set(INDEX) then return, idx $
    else RETURN, returnColor
ENDIF

;--- If you got here. Return all the colors.
if keyword_set(LOAD) then $
  tvlct, rvalue, gvalue, bvalue, bottom
RETURN, REFORM([rvalue, gvalue, bvalue], 9, 3)
END

;-----------------------------------------------------------------------

pro ROI_save, Ptr, group = group, title = title

if n_elements(group) eq 0 then group = 0
if n_elements(title) eq 0 then title = 'File for saving rois'

ans = 'YES'
repeat begin
    roiFile = pickfile(/write, $
                        title = title, $
                        FILE = 'xdisplay.roi', $
                        get_path = path, $
                        FILTER = '*.roi', $
                        group = group)
    if roiFile eq '' then begin
        ok = widget_message('Save aborted.')
        return
    endif
    
    pathPos = strpos(roiFile, path)
    if pathPos ne -1 then $
      roiFile = strmid(roiFile,strlen(path),strlen(roiFile))
    if fileExists(roiFile, path) then begin
        string = roiFile + ' exists. Overwrite it?'
        ans = widget_message(string, /question, /default_no, dialog_parent = group)
    endif
endrep until strupcase(ans) eq 'YES'

rois = replicate({ROI_S}, 1)
child = handle_info(Ptr, /first_child)
repeat begin
    handle_value, child, roi
    rois = [rois, roi]
    child = handle_info(child, /sibling)
endrep until child eq 0
rois = rois(1:n_elements(rois)-1)
;--- Build the save command
cmd = 'save, filename =  "' + path + roiFile + '", rois'

ok = execute(cmd)

end

;-------------------------------------------------------------------------

pro ROI_restore, dispInfo

repeat begin
    roiFile = pickfile(/read, $
                       FILE = 'xdisplay.roi', $
                       get_path = path, $
                       FILTER = '*.roi', $
                       GROUP = dispInfo.base)

    if roiFile eq '' then begin
        ok = widget_message('Load aborted.', $
                            /info, $
                            dialog_parent = dispInfo.base)
        return
    endif
    
    pathPos = strpos(roiFile, path)
    if pathPos ne -1 then $
      roiFile = strmid(roiFile,strlen(path),strlen(roiFile))
    if not(fileExists(roiFile, path)) then begin
        string = roiFile + ' not found. Try again.'
        ans = widget_message(string, dialog_parent = dispInfo.base)
    endif
endrep until fileExists(roiFile, path)

ans = 'YES'
if (dispInfo.nrois ne 0) then $
  ans = widget_message('This will overwrite current ROIs. Conitnue?', $
                       dialog_parent = dispInfo.base, $
                       /question, $
                       /default_no)

if strupcase(ans) eq 'YES' then begin
    if dispInfo.nrois ne 0 then begin
        child = get_child(dispInfo.roiPTR, 1, 1)
        repeat begin
            handle_free,child
            child = get_child(dispInfo.roiPTR, 1, 1)
        endrep until child eq 0
    endif

    restore, path+roiFile
    if n_elements(rois) ne 0 then begin
        for i = 0, n_elements(rois)-1 do begin
            rois.xy = dispInfo.xy
            child = handle_create(dispInfo.roiPTR, $
                                  /first_child, $
                                  value = rois(i), $
                                  /no_copy)
        endfor
        dispInfo.nrois = handle_info(dispInfo.roiptr, /num_child)
        device, copy = [0,0, !d.x_size, !d.y_size, $
                        0, 0, dispInfo.backing(0)]
        showImage, dispInfo
        widget_control, dispInfo.editIDs(1), /sensitive
    endif
endif else ok = widget_message('No ROIs restored.', $
                               /info, $
                               dialog_parent = dispInfo.base)
    
end
;----------------------------------------------------------
pro print_label, r
end

;-------------------------------------------------------------------

function init_rois
; this procedure initializes the roi data structures

rois = 0 & blank_roi = 0

blank_roi = {ROI_S, $
             label    : '', $   ; A label associated with the ROI
             roi_type : '', $   ; Polygon, Point, Rectangle, Circle
             p      : [0.0,0.0,0.0,0.0], $ ; x1, y1, x2, y2
             handle   : fltarr(2,4), $
             x        : fltarr(512), $
             y        : fltarr(512), $
             xy       : 0, $    ; Widget id for irregular xy holder
             npts     : 0, $
             slice    : 0, $
             userval  : '', $   ; Place holder for auxiallary stuff 
             ROIcolor : ''}     ; The color for the roi
rois = replicate({ROI_S}, 1)
ptr = handle_create(value = rois)
return,ptr
end

;--------------------------------------------------------------------

function roi_load_obj, dispInfo, n
; load the nth roi from the roi handles into the r structure

if handle_info(dispInfo.roiPtr, /num_child) lt n then return, 0
child = get_child(dispInfo.roiPtr, n, 1)
roi = get_handle_value(child)

dispInfo.roi_type = roi.roi_type
dispInfo.p = roi.p
dispInfo.handle = roi.handle
dispInfo.label = roi.label
dispInfo.npts = roi.npts
dispInfo.x = roi.x
dispInfo.y = roi.y
xy = fltarr(2,dispInfo.npts)
xy(0,*) = dispInfo.x(0:dispInfo.npts-1)
xy(1,*) = dispInfo.y(0:dispInfo.npts-1)
set_handle_value, dispInfo.xy, xy, /no_copy

return,1
end

;-------------------------------------------------------------------

pro roi_move_resize, dispInfo, first
; if first=1 then look for a handle, otherwise there already is one
; if initial=1 then this roi hasn't been saved yet.

; if nothing is open then return
if dispInfo.open eq -1 then return

pos = dispInfo.pos
h = dispInfo.handle
if dispInfo.roi_type eq 'Point' then np = 1 else np = 4

; if first then determine which handle was selected.
if (first or dispInfo.selected) then begin
    d = [[h], [total(h,2)/np]] - (pos # replicate(1.,np+1)); pos to handles dist
    d = min(total(d^2, 1), i)   ; Dist squared, get closest
    if i eq np then i = long(5) ; always use 5 for center handle
    if d lt 0.01 then begin     ; Close enough?
        dispInfo.ihandle = i
        if dispInfo.roi_type eq 'Circle' then begin
            c = dispInfo.p(2:3)
            dispInfo.temp = [dispInfo.p(0:1) - c, total((pos - c)^2)] ;dx,dy, len
        endif
        if i ne 5 then dispInfo.orig = h(*,np-1-i) $ ; Anchor point
        else dispInfo.orig = pos
    endif else begin
        dispInfo.ihandle = -1
    endelse
endif

ihandle = dispInfo.ihandle
if ihandle lt 0 then return     ; Didn't find a handle

d = pos - dispInfo.orig

;--- Center handle move roi
if ihandle eq 5 then begin
    dispInfo.orig = pos
    if dispInfo.roi_type eq 'Irregular' then begin
        if dispInfo.npts gt 1 then begin
            xy = get_handle_value(dispInfo.xy, /no_copy)
            xy = xy + (d # replicate(1, dispInfo.npts)) ;Translate
            set_handle_value, dispInfo.xy, xy, /no_copy
        endif
    endif else dispInfo.p(0:3) = dispInfo.p(0:3) + [d,d] ; Just move origin
;---- Other handle resize roi
endif else begin
    s = h(*,ihandle) - dispInfo.orig                       ; stretch factor
    if min(abs(s)) lt 0.001 then goto, no_move
    s = (pos - dispInfo.orig) / s                          ; Scale factor
    if min(abs(s)) lt 0.001 then goto, no_move
    case dispInfo.roi_type of
        'Circle'    : begin
            c = dispInfo.p(2:3)		             ; Center
            rad = sqrt(total((pos - c)^2)/dispInfo.temp(2)) ; Dist from ctr
            dispInfo.p(0:1) = dispInfo.temp(0:1) * rad + c
        end
        'Rectangle' : begin
	    x0 = [dispInfo.orig, dispInfo.orig]
	    dispInfo.p(0:3) = (dispInfo.p(0:3) - x0) * s([0,1,0,1]) + x0
        end
        'Point'     : 
        'Irregular' : begin
            xy = get_handle_value(dispInfo.xy, /no_copy)
            xy0 = dispInfo.orig # replicate(1.0,dispInfo.npts)
            xy = fltarr(2, dispInfo.npts)
            xy(0,*) = dispInfo.x(0:dispInfo.npts-1)
            xy(1,*) = dispInfo.y(0:dispInfo.npts-1)
            xy = (xy - xy0) * (s # replicate(1.0,dispInfo.npts)) + xy0
            set_handle_value, dispInfo.xy, xy, /no_copy
        end
        else:
    endcase
endelse

no_move:
roi_draw_object, dispInfo, h, refresh = dispInfo.backing(1), /resize
dispInfo.handle = h
return
end

;--------------------------------------------------------------------------

function roi_closest_roi, dispInfo

dmin = 1e6
pos = dispInfo.pos

if handle_info(dispInfo.roiPtr, /num_child) lt 1 then return, -1

child = handle_info(dispInfo.roiPtr, /first_child)
roi = get_handle_value(child)

for i=1,dispInfo.nrois do begin
    k = i
    if roi.slice eq dispInfo.img_num then begin
        case roi.roi_type of
            'Circle'    : q = roi.p(2:3)
            'Rectangle' : q = [roi.p(0)+roi.p(2), $
                               roi.p(1)+roi.p(3)]/2.0
            'Point'     : q = roi.p(2:3)
            'Irregular' : begin
                foo = roi.npts
                xy = fltarr(2, foo)
                xy(0,*) = roi.x(0:foo-1)
                xy(1,*) = roi.y(0:foo-1)
                xmin=min(xy(0,*), max=xmax)
                ymin=min(xy(1,*), max=ymax)
                q = [xmin+xmax, ymin+ymax]/2.
            end
            else        : q = pos + 10
        endcase
        d = total((pos - q)^2)
        if d lt dmin then begin 
            dmin = d
            imin = k
        endif
    endif
    child = handle_info(child, /sibling)
    if child ne 0 then handle_value, child, roi

endfor

if dmin lt 0.04 then return, imin else return, -1
end

;--------------------------------------------------------------------------
pro roi_draw_object, dispInfo, handle, REFRESH = refresh, RESIZE = resize
; REFRESH will first copy the contents of refresh to the screen

if not(keyword_set(resize)) then resize = 0
if n_elements(refresh) gt 0 then $  ;Refresh?
	device, copy = [0,0, !d.x_size, !d.y_size, 0, 0, refresh]

case dispInfo.roi_type of
    'Circle'    : begin
        a = !d.x_size / float(!d.y_size)    ; Aspect ratio for ellipse (Future)
        dx = dispInfo.p(0) - dispInfo.p(2)
        dy = dispInfo.p(1) - dispInfo.p(3)
        rad = sqrt(dx^2 + dy^2)
        if rad eq 0.0 then return
        dx = dx/rad
        dy = dy/rad
        n = 128                 ; # of points in the circle
        t = findgen(n) * (( 2 * !pi)/ (n-1)) ; Angles in radians
	xy = [[cos(t) * rad], [sin(t) * rad]] # [[dx, -dy],[dy, dx]]
	xy = reform([dispInfo.p(2) + xy(*,0), dispInfo.p(3) + a*xy(*,1)], n,2,/OVER)
	xy = transpose(xy)
plot_it:
        if n gt 1 then begin
            dispInfo.x(*) = 0
            dispInfo.y(*) = 0
            dispInfo.x = xy(0,0:n-1)
            dispInfo.y = xy(1,0:n-1)
            dispInfo.npts = n

            plots, dispInfo.x(0:n-1), dispInfo.y(0:n-1), $
              color = getcolors(dispInfo.ROIcolor, /index), thick=1, $
              linestyle=0, /NORM
            
            if keyword_set(handle) then begin ;Draw resizing handles
                xmin=min(dispInfo.x(0:dispInfo.npts-1), max=xmax)
                ymin=min(dispInfo.y(0:dispInfo.npts-1), max=ymax)
                n = 4
                handle = [[xmin, ymin], [xmin, ymax], [xmax,ymin],[xmax, ymax]]
do_handle:  
                PLOTS, handle, /NORM, $
                  color = getcolors(dispInfo.ROIcolor, /index), $
                  PSYM=6
                PLOTS, total(handle,2)/n, /NORM, $
                  color = getcolors(dispInfo.ROIcolor, /index), PSYM=6 ;The center

	   ENDIF

        endif
    end    

    'Irregular' : begin
        if resize eq 0 then begin
            dispInfo.npts = dispInfo.npts + 1
            n = dispInfo.npts
            if dispInfo.npts eq 1 then begin
                xy = dispInfo.p(2:3) 
                dispInfo.x(0) = dispInfo.p(2)
                dispInfo.y(0) = dispInfo.p(3)
            endif else begin
                if n le 511 then begin
                    xy = fltarr(2,n)
                    xy(0,0:n-2) = dispInfo.x(0:n-2)
                    xy(1,0:n-2) = dispInfo.y(0:n-2)
                    xy(*,n-1)   = dispInfo.p(0:1)
                endif else begin
;--- Close the contour if max pts exceeded
                    n = 512
                    xy = fltarr(2,512)
                    xy(0,0:510) = dispInfo.x(0:510)
                    xy(1,0:510) = dispInfo.y(0:510)
                    xy(0,511) = dispInfo.x(0)
                    xy(1,511) = dispInfo.y(0)
                endelse
            endelse
        endif else begin
            xy = get_handle_value(dispInfo.xy)
            xySize = size(xy)
            n = xySize(2)
;            n = dispInfo.npts
        endelse
		set_handle_value, dispInfo.xy, xy
        goto, plot_it
    end

    'Rectangle' : begin
	xy = dispInfo.p([[0,1],[0,3],[2,3],[2,1],[0,1]])
        n = 5
        goto, plot_it
    end

    'Point'     : begin
        dx = 0.01
        dy = 0.01
        xy = [[dispInfo.p(0)-dx, dispInfo.p(1)-dy], $
              [dispInfo.p(0)-dx, dispInfo.p(1)+dy], $
              [dispInfo.p(0)+dx, dispInfo.p(1)+dy], $
              [dispInfo.p(0)+dx, dispInfo.p(1)-dy], $
              [dispInfo.p(0)-dx, dispInfo.p(1)-dy]]
        
        dx = 0.001
        dy = 0.001
        xyp = [[dispInfo.p(0)-dx, dispInfo.p(1)-dy], $
              [dispInfo.p(0)-dx, dispInfo.p(1)+dy], $
              [dispInfo.p(0)+dx, dispInfo.p(1)+dy], $
              [dispInfo.p(0)+dx, dispInfo.p(1)-dy], $
              [dispInfo.p(0)-dx, dispInfo.p(1)-dy]]
        n = 5
        x = xyp(0,0:n-1)
        y = xyp(1,0:n-1)
        plots, x, y, thick=1, $
          linestyle=0, color = getcolors(dispInfo.ROIcolor, /index), /NORM

        x = xy(0,0:n-1)
        y = xy(1,0:n-1)
        plots, x, y, thick=1, $
          linestyle=0, color = getcolors(dispInfo.ROIcolor, /index), /NORM
    end
    else: print,'Missing type'
endcase
end 

;-----------------------------------------------------------------------------
function roi_redraw_rois, dispInfo, n, SCRATCH = SCRATCH, WINDOW=window
; SCRATCH will first copy the original window dispInfo.backing(0) to the
; window before drawing
; WINDOW will use window instead of the default window.
; n which one to draw handles on

wset, dispInfo.thedraw

if KEYWORD_SET(SCRATCH) then $
  device, copy = [0, 0, !d.x_size, !d.y_size, 0, 0, dispinfo.backing(0)]

first = 1
last = handle_info(dispInfo.roiPtr, /num_child)

;--- Get the first child roi id
child = handle_info(dispInfo.roiPtr, /first_child)
if child ne 0 then roi = get_handle_value(child, /no_copy) $
else return, -1

for i=first,last do begin
    if roi.roi_type eq 'Irregular' then begin
        resize = 1 
        foo = roi.npts
        xy = fltarr(2, foo)
        xy(0,*) = roi.x(0:foo-1)
        xy(1,*) = roi.y(0:foo-1)
        set_handle_value, dispInfo.xy, xy, /no_copy
    endif else resize = 0

    if i ne n then begin
        if (roi.slice eq dispInfo.img_num) or not(dispInfo.allFlag) then $
          roi_draw_object, roi, resize = resize
    endif

    set_handle_value, child, roi, /no_copy

    child = handle_info(child, /sibling)
    if child ne 0 then roi = get_handle_value(child, /no_copy)
endfor

if KEYWORD_SET(SCRATCH) then begin
    wset, dispinfo.backing(1)
    device, copy = [0,0,!d.x_size, !d.y_size,0,0,dispinfo.thedraw]
endif

wset, dispinfo.thedraw
h = 1
;--- Draw the selected ROI with handles
if n ne 0 then begin
    child = get_child(dispInfo.roiPtr, n, 1)
    roi = get_handle_value(child)

    foo = roi.npts
    xy = fltarr(2, foo)
    xy(0,*) = roi.x(0:foo-1)
    xy(1,*) = roi.y(0:foo-1)
    set_handle_value, dispInfo.xy, xy, /no_copy

    roi_draw_object, roi, h, /resize

endif
return, h

end                             ; of roi_redraw_rois


;---------------------------------------------------------------
pro roi_save_roi, dispInfo

if dispInfo.npts ne 0 then begin       ; Make sure that there is something to save

;--- First time for this roi?
    if dispInfo.initial eq 1 then begin
        nroi = replicate({ROI_S}, 1)
        nroi.roi_type = dispInfo.roi_type
        nroi.slice = dispInfo.img_num
    endif else begin
        child = get_child(dispInfo.roiPtr, dispInfo.open, 1)
        nroi = get_handle_value(child)
    endelse

;--- These are the only vars that have changed during an edit
    widget_control, dispInfo.labelID, get_value = junk
    nroi.label    = junk(0)
    nroi.p        = dispInfo.p
    nroi.handle   = dispInfo.handle
    nroi.x        = dispInfo.x
    nroi.y        = dispInfo.y
    nroi.npts     = dispInfo.npts
    nroi.xy       = dispInfo.xy              ; Irregular support
    nroi.ROIcolor = dispInfo.ROIcolor
    nroi.uvalue   = dispInfo.uvalue
    if dispInfo.initial eq 1 then $
      child = handle_create(dispInfo.roiPtr, value = nroi) $
    else set_handle_value, child, nroi
    dispInfo.nrois = handle_info(dispInfo.roiPtr, /num_child)
    h = roi_redraw_rois(dispInfo, 0, /scratch)
endif else print, 'Nothing to save.'

widget_control, dispInfo.labelID, set_value = 'ROI'+strcompress(dispInfo.nrois+1)

;--- Sensitize and desensitize widgets
widget_control, dispInfo.roiTypeBase, sensitive = 1
widget_control, dispInfo.label_base, sensitive = 0
widget_control, dispInfo.editIDs(1), sensitive = 1
widget_control, dispInfo.editIDs(2), sensitive = 0
widget_control, dispInfo.editIDs(3), sensitive = 0
widget_control, dispInfo.editIDs(4), sensitive = 0

dispInfo.roi_type = dispInfo.p_types(dispInfo.ROImode)

dispInfo.open = -1
dispInfo.edit = 0
dispInfo.initial = 0
dispInfo.npts = 0
dispInfo.roicolor = 'RED'
end                             ; roi_save_roi

;-----------------------------------------------------------------------------

pro delete_roi, parent, whichChild

goner = get_child(parent, whichChild, 1)
handle_free, goner

end

;-----------------------------------------------------------------------------

function roi_magicwand, xseed, yseed, dispInfo
;--- Finds all points connected to given seed point

widget_control, /hourglass

;--- Perform find on user scaled image
data = get_handle_value(dispInfo.dataptr, /no_copy)

temp = bytscl(data(*,*,dispInfo.img_num), $
              min = dispInfo.min, $
              max = dispInfo.max, $
              top = dispInfo.ngreys - 1) + byte(dispInfo.greyBottom)

;--- image now a binary mask
if dispInfo.threshold then image = float(temp gt dispInfo.lev)
if dispInfo.density   then begin
    low  = temp le dispInfo.drange(1)
    high = temp ge dispInfo.drange(0)
    image = float(low and high)
endif

if image(xseed,yseed) lt 1.0 then begin
	upper = 0.9
	lower = 0.0
endif else begin
	upper = 1.0
	lower = 0.1
endelse

connected = search2d(image, xseed, yseed, lower, upper, /diagonal)

found = bytarr(dispInfo.xsize, dispInfo.ysize)
found(connected) = 1B

set_handle_value, dispInfo.dataptr, data, /no_copy

;--- Fill in the holes
kernel = replicate(1,5,5)
for i = 0,2 do image = erode(dilate(temporary(image), kernel), kernel)

;--- Find the outline using contour function

oldDisplay = !d.name
set_plot,'NULL'
contour, found, nlevels = 1, path_xy = xy, path_info = info, /follow, $
  pos = [0.0, 0.0, float(dispInfo.xsize - 1), $
         float(dispInfo.ysize - 1)], $
  xrange = [0.0, float(dispInfo.xsize - 1)], $
  yrange = [0.0, float(dispInfo.ysize - 1)], $
  xstyle = 1, ystyle = 1
set_plot,oldDisplay

n_contours = n_elements(info)

if n_contours eq 0 then return, {xy:0, npts:0} else begin
    num_pts = info(0).N
    
;Make sure there's no more than 512 points in the ROI
    inc = 1

    while num_pts gt 512 do begin
        num_pts = num_pts/2
        inc = inc * 2
    end
    
    roi_type = 'Irregular'
    j=0
    rxy = fltarr(2,num_pts)
    for i=0,num_pts-1 do begin
        rxy(0,i) = xy(0,j)
        rxy(1,i) = xy(1,j)
        j=j+inc
    end
;--- don't need to close the contour.  roidrawevent does that.    
;    rxy(0,num_pts) = xy(0,0)
;    rxy(1,num_pts) = xy(1,0)
    return, {xy:rxy, npts:num_pts}
endelse 
end

;-----------------------------------------------------------------------------

FUNCTION WhatTypeVariable, variable
;--- From David Fanning's XWINDOW program.
;    See:  http://www.dfanning.com/documents/programs.html

; Use SIZE function to get variable info.
   
varInfo = Size(variable)

; The next to last element in varInfo has the data type.
   
typeIndex = varInfo(varInfo(0) + 1)
dataTypes = ['UNDEFINED', 'BYTE', 'INTEGER', 'LONG', 'FLOATING', $
             'DOUBLE', 'COMPLEX', 'STRING', 'STRUCTURE', 'DCOMPLEX']
thisType = dataTypes(typeIndex)

RETURN, thisType
END

;-----------------------------------------------------------------------------

function roi_info, parent, whichROI, data, $
            arg1, arg2, arg3, $
            INDEX = INDEX, $
            USER_FUNC = USER_FUNC, $
            _EXTRA = extra

;+
; NAME: ROI_INFO
;
; PURPOSE: 
;     This routine is for processing ROIs returned from xdisplay.  It
;     can return the indices of the pixels contained in the ROI, an
;     ROI_INFO structure (described below), or *any* IDL or user
;     written FUNCTION that is defined and expects a vector
;     (data(indices)) as the first positional parametedispInfo.  This
;     USER_FUNC can also have up to three additional parameters and an
;     unlimited number of keywords that are appropriate.
;
; CATEGORY: ROI_UTILS
;
; CALLING SEQUENCE: info = roi_info(parent, whichROI, data[, /INDEX, $
;                                   USER_FUNC = USER_FUNC, $
;                                   arg1, arg2, arg3])
; 
; INPUTS:
;     parent   : handle to parent roi
;     whichROI : Get info for this ROI # or ROI label ( if multiple
;                ROIs have the same label ROI_INFO will return info
;                for the first occurence.
;     data     : The image to process. (Can be 2D or 3D data.) 
;
; OPTIONAL INPUTS: 
;     arg1, arg2, arg3 : Named parameters for user processing
;
; KEYWORD PARAMETERS:
;   INDEX     : If called with INDEX keyword then only the indices are
;               returned. Over rides USER_FUNC keyword.
;   USER_FUNC : Set USER_FUNC to any valid IDL command to perform user
;               specified statistics on the ROI.  Supports any
;               keywords that the USER_FUNC allows.  In this case the
;               result of USER_FUNC is returned. 
;
; OUTPUTS:
;    Out put can be any of the following depending on the calling
;    sequence: 
;    1)  ROI_INFO structure {ROI_INFO, $
;                            npixels: npixles, $
;                            mean : mean, $
;                            sdev: sdev}
;    2)  An array of indices within the ROI.
;    3)  Output from USER_FUNC.
;
; OPTIONAL OUTPUTS: none
;
; RESTRICTIONS: 
;    1)  Parent must have children of ROI_S type
;    2)  USER_FUNC must expect image(index) as the first positional
;        parameter.
;    3)  Keywords to be passed to USER_FUNC can not be for return
;        purposes, i.e. 
;           min = ROI_INFO(parent, 2, image, user_func='min', max = max)
;        will not work.
;
; PROCEDURE:
;    use xdisplay to define some ROIs
;      IDL> roi = xdisplay(image)
;
;    Display info about ROI #2.  
;***********************************************
; Note that ROI numbers are not 0 offset.  
;         ROI #1 <> 0
;         ROI #1 = 1
;***********************************************
;    IDL> info = roi_info(roi, 2, image)
;    IDL> print, info.mean
;
; MODIFICATION HISTORY:
;     27 Dec 96 PMW Initial Coding
;     26 Jan 96 PMW Added ability to handle 3D data and ROI slice
;                   definition.
;                   Can call with ROI number or ROI label.
;-

if not(keyword_set(index)) then index = 0
extraFlag = keyword_set(extra)
userFlag = keyword_set(user_func)

CATCH, error
if error ne 0 then begin
    ok = widget_message(!err_string, /error)
    return, 'error'
endif

np = n_params()
if np lt 3 then $
  message, 'Wrong number of arguments.'

if not(handle_info(parent, /valid)) then $
  message, 'Invalid pointer.'

num_children = handle_info(parent, /num_child)
if  num_children eq 0 then $
  message, 'Pointer has no children.'

datasize = size(data)

first = get_child(parent, 1)
firstValue = get_handle_value(first)
if whatTypeVariable(firstValue) ne 'STRUCTURE' then $
  message,'Parent does not point to a structure.'
if tag_names(firstValue, /structure) ne 'ROI_S' then $
  message, 'Parent does not point to a ROI_S structure.'

if whatTypeVariable(whichROI) ne 'STRING' then begin
    if whichROI gt num_children then begin
        string = $
          'whichROI is out of range.  Must be >= 0 < '+strtrim(num_children,2)
        message, string
    endif
    child = get_child(parent, whichROI)
endif else begin
    nrois = handle_info(parent, /num)
    i = 0
    repeat begin
        i = i + 1
        child = get_child(parent, i)
        if child ne 0 then begin
            roi   = get_handle_value(child, /no_copy)
            label = roi.label
            set_handle_value, child, roi, /no_copy
        endif
    endrep until (label eq whichROI) or (i eq nrois+1)
    if i eq nrois+1 then $
      message, 'No ROIs with label: '+whichROI
endelse

roi = get_handle_value(child)

if datasize(0) eq 3 then begin
    if roi.slice gt datasize(3) then message,'ROI slice outside region.'
    image = data(*,*,roi.slice)
endif else image = data
CATCH, /cancel

indices = polyfillv(roi.x(0:roi.npts-1)*datasize(1), $
                    roi.y(0:roi.npts-1)*datasize(2), $
                    datasize(1), datasize(2))

if index then return, indices

if not(keyword_set(USER_FUNC)) then begin
    result = moment(image(indices), sdev = s)
    
    return, {ROI_INFO, npixels: n_elements(indices), $
             mean : result(0), $
             sdev: s}
endif else begin
    if extraflag then help,/st,extra
    case np of
        3: begin
            if extraFlag then $
              result = CALL_FUNCTION(user_func, image(indices), $
                                     _EXTRA = extra) $
            else result = CALL_FUNCTION(user_func, image(indices))
        end
        4 : begin
            if extraFlag then $
              result = CALL_FUNCTION(user_func, image(indices), $
                                     arg1, $
                                     _EXTRA = extra) $
            else result = CALL_FUNCTION(user_func, image(indices), $
                                        arg1)
        end
        5 : begin
            if extraFlag then $
              result = CALL_FUNCTION(user_func, image(indices), $
                                     arg1, $
                                     arg2, $
                                     _EXTRA = extra) $
             else result = CALL_FUNCTION(user_func, image(indices), $
                                         arg1, $
                                         arg2)
        end
        6 : begin
            if extraFlag then $
              result = CALL_FUNCTION(user_func, image(indices), $
                                     arg1, $
                                     arg2, $
                                     arg3, $
                                     _EXTRA = extra) $
            else result = CALL_FUNCTION(user_func, image(indices), $
                                        arg1, $
                                        arg2, $
                                        arg3)
        end
        else:
    endcase
    return, result
endelse

end

;----------------------------------------------------------------------

pro wscale, win, lev, lut, $
            bottom = bottom, $ 
            ncolors = ncolors, $
            density = density, $
            threshold = threshold, $
            range = range
   
;--- Turn off reporting 
!quiet = 1

if not(keyword_set(bottom)) then bottom = 0
if not(keyword_set(ncolors)) then ncolors = !d.n_colors
if keyword_set(density) and not keyword_set(range) then $
  MESSAGE, 'RANGE needed for DENSITY keyword.'
if keyword_set(range) then begin
    range(0) = (range(0) > 0) < (ncolors - 1)
    range(1) = (range(1) < (ncolors - 1)) > range(0)
endif

t = findgen(ncolors)

if keyword_set(threshold) then $
  gs = byte((t gt lev) * 255) $
else $
  gs = byte(255.0*(1.-1./(exp((t-lev)/win)+1)))
r = gs
g = gs
b = gs

if keyword_set(density) then begin
    r(range(0):range(1)) = 0B
    g(range(0):range(1)) = 0B
    b(range(0):range(1)) = 255B
endif

; Load the color table
tvlct,r,g,b,bottom

;--- Turn on reporting 
!quiet = 0
END

;------------------------------------------------------------

function get_pixmaps, xsize, ysize, display, $
            n_maps = n_maps, $
            oldids = oldids

if n_elements(n_maps) eq 0 then n_maps = 2

;--- Should I delete the old pixmaps
if n_elements(oldids) gt 0 then $
  for i=0,n_elements(oldids)-1 do begin
  	if oldids(i) gt 30 then wdelete, oldids(i)
  endfor
  	
maps = lonarr(n_maps)
for i = 0,n_maps-1 do begin
    window, /free, /pixmap, xsize=xsize, ysize=ysize
    maps(i) = !d.window
    device, copy=[0, 0, xsize, ysize, 0, 0, display]
endfor
wset, display
return, maps
end

;------------------------------------------------------------

pro printInfo, dispInfo, scratch = scratch
;--- This procedure prints display info on the window
; dispInfo == the info structure for this display
; scratch == start with the fresh window from backing store

if not(keyword_set(scratch)) then scratch = 0

if scratch then begin
    wset,dispInfo.thedraw
    device, copy=[0, 0,dispInfo.xsize*dispInfo.zoom,dispInfo.ysize*dispInfo.zoom,$
                  0, 0,dispInfo.backing(1)]
endif else begin
    wset,dispInfo.backing(1)
    device, copy=[0, 0,dispInfo.xsize*dispInfo.zoom,dispInfo.ysize*dispInfo.zoom,$
                  0, 0,dispInfo.thedraw]
endelse
wset,dispInfo.thedraw

;--- Get the correct charsize for window, level and image #
size = 1.0
xyouts, 0.1, 0.2,  'w: 555', $
  /norm, color = getcolors(dispInfo.textColor,/index), width = thisWidth, charsize = -1
while thisWidth gt 0.2 do begin
    size = size*0.9
    xyouts, 0.05, 0.15,  'w: 555', $
      /norm, color = getcolors(dispInfo.textColor,/index), $
      width = thisWidth, charsize = -size
endwhile


xyouts, 0.05, 0.15,  'w: ' + strtrim(dispInfo.win, 2), $
  /norm, color = getcolors(dispInfo.textColor, /index), charsize = size
xyouts, 0.05, 0.1, 'l: '+ strtrim(dispInfo.lev, 2), $
  /norm, color = getcolors(dispInfo.textColor, /index), charsize = size
xyouts, 0.05, 0.05, '#: ' + strtrim(dispInfo.img_num, 2), $
  /norm, color = getcolors(dispInfo.textColor, /index), charsize = size

end

;-----------------------------------------------------------------------

pro showImage, dispInfo, sample = sample
;--- Displays the desired image
; dispInfo == the info structure for this display
; sample == use nearest neighbor sampling for rebin

if not(keyword_set(sample)) then sample = 0

widget_control, /hourglass

data = get_handle_value(dispInfo.dataPtr, /no_copy)

;--- Scale the image for display
if dispInfo.colormode eq 0 then begin
    top    = dispInfo.ngreys
    bottom = dispInfo.greyBottom
endif else begin
    top    = dispInfo.ncolors
    bottom = dispInfo.colorBottom
endelse

image = bytscl(data(*,*,dispInfo.img_num), $
               min = dispInfo.min, $
               max = dispInfo.max, $
               top = top - 1) + byte(bottom)

;--- Mask it if wanted
if handle_info(dispInfo.maskPTR) then begin
    mask = get_handle_value(dispInfo.maskPTR, /no_copy)
    image = temporary(image) * mask
    set_handle_value, dispInfo.maskPTR, mask, /no_copy
endif

;--- Display the image at the correct size
wset, dispInfo.thedraw
tv, rebin(image, $
          dispInfo.datasize(1)*dispInfo.zoom, $
          dispInfo.datasize(2)*dispInfo.zoom, sample = sample)

dispInfo.backing = get_pixmaps(dispInfo.xsize*dispInfo.zoom, $
                      dispInfo.ysize*dispInfo.zoom, $
                      dispInfo.theDraw, $
                      oldids = dispInfo.backing)

;--- check for rois
if handle_info(dispInfo.roiptr, /num_child) gt 0 then $
  h = roi_redraw_rois(dispInfo, 0, /scratch)

set_handle_value, dispInfo.dataptr, data, /no_copy

printInfo, dispInfo

end                             ;-- of showimage

;-------------------------------------------

PRO xdisplay_set_value, id, value
;--- This routine should be used to interact with the xdisplay.  It
;    value should be a string which contains a command of the type:
;             VAR = value
;    VAR can be one of the following:
;       ROICOLOR : used to change the current ROI drawing color.
;       UVALUE   : used to change the current user value for ROI_S
stash = id
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

foo = str_sep(value, '=', /trim)
if n_elements(foo) ne 2 then $
  ok = widget_message('Incorrect command format sent to XDISPLAY.', /error)

command = strupcase(foo(0))
value   = strupcase(foo(1))

case command of
    'ROICOLOR' : dispInfo.roicolor = value
    'UVALUE'   : dispInfo.uvalue   = value
    else : ok = widget_message(command+' is an unsupported XDISPLAY command.', $
                               /error)
endcase

WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY

END

;-----------------------------------

FUNCTION xdisplay_get_value, id
;--- Get the display window id for this widget
  ;ON_ERROR, 2

  stash = id
  WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

;-- Get the value here
  display = dispInfo.theDraw

	; Restore the dispInfo.
  WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY

        ; Return the value here.
  return, display
END


;----------------------------------------------------------------------------

pro roiModeEvent, ev
;--- Event handler for roi types

stash = ev.top
widget_control, stash, get_uvalue=dispInfo, /no_copy

onButton = where(ev.id eq dispInfo.roiButtons)
offButtons = where(ev.id ne dispInfo.roiButtons)

;--- If the mode didn't change do nothing
if dispInfo.p_types(onButton(0)) eq dispInfo.roi_type then goto, cleanup

widget_control, dispInfo.roiButtons(onButton), get_uvalue = states
widget_control, dispInfo.roiButtons(onButton), set_value = reform(states(0,*,*))
dispInfo.roi_type = dispInfo.p_types(onButton(0))
;--- magicwand is always irregular!
dispInfo.ROImode = onButton(0)

for i=0,n_elements(offButtons)-1 do begin
    widget_control, dispInfo.roiButtons(offButtons(i)), get_uvalue = states
    widget_control, dispInfo.roiButtons(offButtons(i)), $
    	set_value = reform(states(1,*,*))
endfor

CLEANUP:
widget_control, stash, set_uvalue = dispInfo, /no_copy
end

;----------------------------------------------------------------------------

pro modeEvent, ev
;--- Eventhandler for the mode buttons (window control, zoom control,
;    roi control)

stash = ev.top
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

onButton = where(ev.id eq dispInfo.modeButtons)
offButtons = where(ev.id ne dispInfo.modeButtons)

;--- If the mode didn't change do nothing
if dispInfo.mode eq onButton(0) then goto, cleanup

widget_control, dispInfo.draw, draw_motion_events = 0

;--- going out of ROI mode?
if dispInfo.mode eq 2 then begin

;--- Check to see if there is an open roi and save it
    if dispInfo.open ne -1 then $
      roi_save_roi, dispInfo

    widget_control, dispInfo.roiBase, sensitive = 0
    widget_control, dispInfo.roiEditBase, sensitive = 0
endif

widget_control, dispInfo.modeButtons(onButton), get_uvalue = states
widget_control, dispInfo.modeButtons(onButton), set_value = reform(states(0,*,*))
dispInfo.mode = onButton(0)

for i=0,n_elements(offButtons)-1 do begin
    widget_control, dispInfo.modeButtons(offButtons(i)), get_uvalue = states
    widget_control, dispInfo.modeButtons(offButtons(i)), $
    	set_value = reform(states(1,*,*))
endfor

;--- Check to see if it's roi mode or not and map / unmap roi base
;    widget

if dispInfo.mode eq 2 then begin
    widget_control, dispInfo.roiBase, /sensitive
    widget_control, dispInfo.roiEditBase, /sensitive
;--- turn on the motion events
    widget_control, dispInfo.draw, /draw_motion_events
;--- update the backing store
endif else widget_control, dispInfo.draw, draw_motion_events = 0

widget_control, dispInfo.draw, event_pro = dispInfo.modeHandlers(dispInfo.mode)

CLEANUP:
WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY
end

;----------------------------------------------------------------------------

pro scrollEvent, ev
;--- Event handler for scrolling through a 3D data set

widget_control, ev.top, get_uvalue=dispInfo, /no_copy

name = tag_names(ev, /structure)
case name of
    'WIDGET_DRAW' : begin
        showimage, dispInfo
        
        widget_control, ev.id, event_pro = "drawEvent"
        dispInfo.sdown = 0
    end

    'WIDGET_TIMER' : begin
;--- change the image        
        dispInfo.img_num = $
              (dispInfo.img_num + dispInfo.sdown) < (dispInfo.datasize(3) - 1) > 0
        showimage, dispInfo, /sample
;--- set the next timer event
        widget_control, ev.id, timer = 0.0001
    end
endcase

widget_control, ev.top, set_uvalue=dispInfo, /no_copy
end


;---------------------------------------------------------------------------

pro zoomEvent, ev
;--- event handler for zoom mode

stash = ev.top
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

wset,dispInfo.thedraw

case ev.press of
    0: ; release do nothing
    1: dispInfo.zoom = (dispInfo.zoom + 1) < dispInfo.zoomMax
    2: dispInfo.zoom = 1
    4: dispInfo.zoom = (dispInfo.zoom - 1) > dispInfo.zoomMin
    else:
endcase

widget_control, dispInfo.base, $
  tlb_set_title = dispInfo.title+' (x'+strtrim(dispInfo.zoom,2)+')'
widget_control, dispInfo.draw, $
  xsize = dispInfo.xsize*dispInfo.zoom, $
  ysize = dispInfo.ysize*dispInfo.zoom
widget_control, dispInfo.lut, $
  xsize = 20, $
  ysize = dispInfo.ysize*dispInfo.zoom

if dispInfo.colormode eq 0 then begin
    junk = indgen(dispInfo.ngreys)
    ct = bytarr(2,dispInfo.ngreys)
    ct(0, *) = byte(junk)
    ct(1, *) = ct(0, *)
    dispInfo.ctscale = float(dispInfo.ngreys)/float(dispInfo.ysize*dispInfo.zoom)
endif else begin
    junk = indgen(dispInfo.ncolors)
    ct = bytarr(2,dispInfo.ncolors)
    ct(0, *) = byte(junk)
    ct(1, *) = ct(0, *)
endelse
wset, dispInfo.theLUT
tv, congrid(ct,20,dispInfo.ysize*dispInfo.zoom)

showImage, dispInfo

WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY
end

;----------------------------------------------------------------------------

pro LUTEvent, ev
;--- event handler for LUT events

stash = ev.top
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

if dispInfo.density eq 0 and dispInfo.threshold eq 0 then begin
    goto,cleanup
endif

ctval = round(ev.y * dispInfo.ctscale)
if ev.type eq 2 then begin
    diff = ctval - dispInfo.iy
    if dispInfo.sdown eq 1 then begin
        if dispInfo.density then begin
            dispInfo.drange(0) = dispInfo.irange(0) + diff
            dispInfo.drange(1) = dispInfo.irange(1) + diff
        endif else $
          dispInfo.lev = $
          ((dispInfo.oldlev+diff) < dispInfo.lev_max) > dispInfo.lev_min
    endif else if dispInfo.d_evnt eq 1 then begin
        dispInfo.drange(1) = dispInfo.irange(1) + diff
    endif else if dispInfo.d_evnt eq -1 then begin
        dispInfo.drange(0) = dispInfo.irange(0) + diff
    endif
    
    wscale, dispInfo.win, dispInfo.lev, $
      density   = dispInfo.density, $
      threshold = dispInfo.threshold, $
      range     = dispInfo.drange, $
      bottom    = dispInfo.greyBottom, $
      ncolors   = dispInfo.ngreys
    goto, CLEANUP
endif

case ev.press of
    0 : begin                   ; button was released
        widget_control, dispInfo.lut, draw_motion_events = 0
        dispInfo.sdown = 0
        dispInfo.d_evnt = 0
        dispInfo.irange = dispInfo.drange
    end
    1 : begin                   ; left button pressed
        widget_control, dispInfo.lut, /draw_motion_events
        dispInfo.iy = ctval
        dispInfo.irange = dispInfo.drange
        dispInfo.oldlev = dispInfo.lev
        if dispInfo.density then $
          center = total(dispInfo.irange)/2.0 $
        else $
          center = dispInfo.lev

        if ctval lt center + center*0.10 and $
          ctval gt center - center*0.10 then begin
            dispInfo.sdown = 1
        endif else begin
            if ctval gt (total(dispInfo.irange)/2.0) then begin
                dispInfo.d_evnt = 1
            endif else begin
                dispInfo.d_evnt = -1
            endelse
        endelse
    end
    else:
endcase
CLEANUP:
WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY
end

;----------------------------------------------------------------------------

pro drawEvent, ev
;--- event handler for the default display mode window/level/scroll

name = tag_names(ev, /structure)
if name eq 'WIDGET_TIMER' then return

stash = ev.top
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

wset,dispInfo.thedraw

if ev.type eq 2 then begin      ; Only get drag events from the middle
    dispInfo.win = $
      ((dispInfo.oldwin-(dispInfo.iy-ev.y)) < dispInfo.win_max) > $
      dispInfo.win_min
    dispInfo.lev = $
      ((dispInfo.oldlev-(dispInfo.ix-ev.x)) < dispInfo.lev_max) > $
      dispInfo.lev_min
    if dispInfo.colorMode eq 0 then $
      wscale, dispInfo.win, dispInfo.lev, $
      density   = dispInfo.density, $
      threshold = dispInfo.threshold, $
      range     = dispInfo.drange, $
      bottom = dispInfo.greyBottom, $
      ncolors = dispInfo.ngreys
    printInfo, dispInfo, /scratch
endif else begin
    widget_control, dispInfo.draw, draw_motion_events = 0 
    dispInfo.d_evnt = 0
endelse

case ev.press of
;button release
    0 :
    
; Left button
    1 : begin
        if dispInfo.datasize(0) eq 3 then begin
            dispInfo.img_num = (dispInfo.img_num - 1) > 0
            dispInfo.sdown = -1
            showimage, dispInfo
            widget_control, ev.id, event_pro = "scrollEvent", timer = dispInfo.delay
        endif
    end
    
    2 : begin                   ; Middle button pressed
        if dispInfo.colorMode eq 0 then begin
            if dispInfo.d_evnt eq 0 then begin
                widget_control, dispInfo.draw, /draw_motion_events
                dispInfo.ix = ev.x & dispInfo.iy = ev.y
                dispInfo.oldlev = dispInfo.lev
                dispInfo.oldwin = dispInfo.win
                dispInfo.d_evnt = 1
            endif
        endif
    end
    
; right button
    4 : begin
        if dispInfo.datasize(0) eq 3 then begin
            dispInfo.img_num = $
              (dispInfo.img_num + 1) < (dispInfo.datasize(3) - 1)
            dispInfo.sDown = 1
            showimage, dispInfo
            widget_control, ev.id, event_pro = "scrollEvent", timer = dispInfo.delay
        endif
    end

    else:
endcase


;--- Restore the dispInfo structure
WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY
end

;-----------------------------------------------------------------------------

pro roiDrawEvent, ev
;--- Event handler for draw when in roi mode

stash = ev.top
widget_control, stash, get_uvalue = dispInfo, /no_copy

select = 0
if ev.press eq 4 then begin     ; Right mouse saves the current ROI
    if dispInfo.open ne -1 then roi_save_roi, dispInfo
    goto,cleanup
endif else if ev.press eq 2 then begin ; Middle selects
    if dispInfo.open eq -1 then dispInfo.selected = 1
endif

;Initialize some stuff
pos = [(0 > ev.x < dispInfo.xsize*dispInfo.zoom), $
       (0 > ev.y < dispInfo.ysize*dispInfo.zoom)]

first = 0
last = 0 
drag = 0

if dispInfo.first eq 0 then begin
    if ev.press ne 1 then begin ; It's a move event
        if dispInfo.selected eq 0 then goto, cleanup
    endif else begin
        if dispInfo.roi_type eq 'MagicWand' then begin
            string = 'Magic Wand tool '
            if dispInfo.colorMode eq 1 then begin
                string = string + 'is not available in colormode.'
                ok = widget_message(string, dialog_parent = dispInfo.base)
                goto, cleanup
            endif
            
            if not(dispInfo.threshold or dispInfo.density) then begin
                string = string + 'only works in Threshold or Density mode.'
                ok = widget_message(string, dialog_parent = dispInfo.base)
                goto, cleanup
            endif
        endif
        first = 1
        dispInfo.first = 1
    endelse
;--- Not first.  It was either a release or a drag
endif else begin
    if ev.release ne 1 then drag = 1 $
    else begin                  ; button released
        if dispInfo.npts le 1 then begin
            ok = widget_message('ROI has no points.', /error, $
                               dialog_parent = dispInfo.base)
            widget_control, dispInfo.label_base, sensitive = 0
            widget_control, dispInfo.editIDs(2), sensitive = 0
            widget_control, dispInfo.editIDs(3), sensitive = 0
            widget_control, dispInfo.editIDs(4), sensitive = 0
            widget_control, dispInfo.roitypebase, sensitive = 1
            dispInfo.open = -1
            dispInfo.first = 0
            dispInfo.selected = 0
            dispInfo.initial = 0
            dispInfo.npts = 0
            goto, cleanup
        endif
        dispInfo.first = 0
        last = 1
        if dispInfo.open gt dispInfo.nrois then dispInfo.initial = 1
        widget_control, dispInfo.labelID, /input_focus
    endelse
endelse

g = dispInfo.grain
pos = 0.0 > (round(pos/g)*g / float([!d.x_size, !d.y_size])) < 1.0

; If the drag event didn't go anywhere return
if drag and $
  (dispInfo.pos(0) eq pos(0)) and (dispInfo.pos(1) eq pos(1)) then goto, CLEANUP

dispInfo.pos = pos
if dispInfo.edit eq 1 then begin       ; Move/resize
    roi_move_resize, dispInfo, first
    goto, CLEANUP
endif else if dispInfo.selected then begin ; Selects closest ROI
    x = roi_closest_roi(dispInfo)
;--- Found one
    if x ne -1 then begin       
        if roi_load_obj(dispInfo, x) then begin
            dispInfo.edit = 1          ; Now in edit mode
            dispInfo.open = x          ; Roi found is open
            child = get_child(dispInfo.roiPTR, x)
            roi = get_handle_value(child, /no_copy)
            dispInfo.roicolor = roi.roicolor
            set_handle_value, child, roi, /no_copy
            h = roi_redraw_rois(dispInfo, x, /scratch)
            dispInfo.handle = h

;--- sensitize widgets
            widget_control, dispInfo.label_base, /sensitive
            widget_control, dispInfo.editIDs(2), /sensitive
            widget_control, dispInfo.editIDs(3), /sensitive
            widget_control, dispInfo.editIDs(4), /sensitive
            widget_control, dispInfo.roiTypeBase, sensitive = 0
            widget_control, dispInfo.labelID, set_value = dispInfo.label

            roi_move_resize, dispInfo, first
        endif
    endif
    dispInfo.selected = 0
    goto, CLEANUP
endif                           ; end of editing/moving

if dispInfo.open eq -1 then begin      ; New ROI
    widget_control, dispInfo.label_base, sensitive = 0
    widget_control, dispInfo.editIDs(2), sensitive = 0
    widget_control, dispInfo.editIDs(3), sensitive = 0
    widget_control, dispInfo.editIDs(4), sensitive = 0
    widget_control, dispInfo.roitypebase, sensitive = 1
    dispInfo.open = dispInfo.nrois + 1
endif else begin
    widget_control, dispInfo.label_base, /sensitive
    widget_control, dispInfo.editIDs(2), /sensitive
    widget_control, dispInfo.editIDs(3), /sensitive
    widget_control, dispInfo.editIDs(4), /sensitive
    widget_control, dispInfo.roitypebase, sensitive = 0
endelse

case dispInfo.roi_type of
    'Circle'    : begin
DRAWIT:
        if first then dispInfo.p(2:3) = pos
        dispInfo.p(0:1) = pos
        h = last
        resize = 0
        if last then begin
            if (dispInfo.roi_type eq 'Irregular') $
              and (dispInfo.npts lt 512) then begin
                n = dispInfo.npts
                dispInfo.npts = n + 1
                xy = fltarr(2,dispInfo.npts)
                xy(0,0:n-1) = dispInfo.x(0:n-1)
                xy(1,0:n-1) = dispInfo.y(0:n-1)
                xy(0,dispInfo.npts-1) = dispInfo.x(0)
                xy(1,dispInfo.npts-1) = dispInfo.y(0)
                set_handle_value, dispInfo.xy, xy, /no_copy
                resize = 1
            endif            
            dispInfo.edit = 1
        endif
        roi_draw_object, dispInfo, h, refresh = dispInfo.backing(1), resize = resize
        dispInfo.handle = h
    end
    
    'Rectangle' : goto, DRAWIT
    'Point'     : goto, DRAWIT
    'Irregular' : goto, DRAWIT
    'MagicWand' : begin
        dispInfo.magic = 1
        wandInfo = roi_magicwand(ev.x/dispInfo.zoom, $
                                 ev.y/dispInfo.zoom, $
                                 dispInfo)
        if wandInfo.npts eq 0 then begin
            ok = widget_message('No regions found.',/info)
            goto, CLEANUP
        endif
        dispInfo.p(0:1) = [wandInfo.xy(0,0),wandInfo.xy(1,0)]
        dispInfo.npts = wandInfo.npts
        dispInfo.x = wandInfo.xy(0,*)
        dispInfo.y = wandInfo.xy(1,*)
        xy = wandInfo.xy
        set_handle_value, dispInfo.xy, xy, /no_copy

;--- Generate a fake event to switch the buttons and modes
        top = dispInfo.base
        id  = dispInfo.roiButtons(0)
        widget_control, stash, set_uvalue = dispInfo, /no_copy
        roiModeEvent, {top : top, id : id}
        widget_control, stash, get_uvalue = dispInfo, /no_copy

        widget_control, dispInfo.label_base, /sensitive
        widget_control, dispInfo.editIDs(2), /sensitive
        widget_control, dispInfo.editIDs(3), /sensitive
        widget_control, dispInfo.editIDs(4), /sensitive
        widget_control, dispInfo.roitypebase, sensitive = 0
        
;--- Draw it!
        dispInfo.first = 0
        last = 1
        if dispInfo.open gt dispInfo.nrois then dispInfo.initial = 1
        widget_control, dispInfo.labelID, /input_focus
        goto, DRAWIT
    end
        
    else: 
endcase


CLEANUP:
widget_control, stash, set_uvalue = dispInfo, /no_copy

END                             ; roiDrawEvent

;--------------------------------------------------------------------

pro colorToolEvent, ev
;--- Event handler for color button if xdisplay is all color display

widget_control, ev.top, get_uvalue = dispInfo, /no_copy

XColors, ncolors=dispInfo.ncolors, $
  Bottom=dispInfo.colorBottom, $
  Group=dispInfo.base, Title=dispInfo.title + ' Colors'

widget_control, ev.top, set_uvalue = dispInfo, /no_copy
end

;--------------------------------------------------------------------

pro colorDropEvent, ev
;--- Event handler for color / greyscale droplist

widget_control, ev.top, get_uvalue = dispInfo, /no_copy

case dispInfo.colorModes(ev.index) of
    'Greyscale' : begin
        dispInfo.colorMode = 0
        widget_control, dispInfo.options, sensitive = 1
        id = xregistered('XColors:' + dispInfo.title + ' Colors', $
                         /noshow, /nomap, /get_id)
        if id then begin
            widget_control, id, map = 0
        endif
    end

    'Color'     : begin
        dispInfo.colorMode = 1
        widget_control, dispInfo.options, sensitive = 0
        XColors, ncolors=dispInfo.ncolors, $
          Bottom=dispInfo.colorBottom, $
          Group=ev.top, Title=dispInfo.title + ' Colors'
    end 
    else :
endcase

showimage, dispInfo

widget_control, ev.top, set_uvalue = dispInfo, /no_Copy
end

;--------------------------------------------------------------------

pro optionDropEvent, ev
;--- Event handler for density / threshold / normal drop list

widget_control, ev.top, get_uvalue = dispInfo, /no_copy

case dispInfo.optionModes(ev.index) of
    'Normal' :begin
        dispInfo.density = 0
        dispInfo.threshold = 0
    end

    'Density' : begin
        dispInfo.density = 1
        dispInfo.threshold = 0
    end
    'Threshold' :begin
        dispInfo.density = 0
        dispInfo.threshold = 1
    end

endcase

wscale, dispInfo.win, dispInfo.lev, $
  density   = dispInfo.density, $
  threshold = dispInfo.threshold, $
  range     = dispInfo.drange, $
  bottom    = dispInfo.greyBottom, $
  ncolors   = dispInfo.ngreys

widget_control, ev.top, set_uvalue = dispInfo, /no_copy
end

;-----------------------------------------------------------------------------

pro menuEvent, ev
;--- Event handler for main menu buttons

WIDGET_CONTROL, ev.top, GET_UVALUE=dispInfo, /NO_COPY
widget_control, ev.id, get_uvalue = eventval

case eventval of
    'OPEN'   : ROI_restore, dispInfo    
    'SAVE'   : ROI_save, dispInfo.roiptr, group = dispInfo.base
    'DELETE' : begin
DELETE:
        if dispInfo.nrois eq 0 then $  ;  Have any rois been saved yet?
          device, copy = [0, 0, !d.x_size, !d.y_size, 0, 0, dispInfo.backing(0)] $
        else begin
            if dispInfo.initial ne 1 then $ ; Has THIS roi been saved yet?
              delete_roi, dispInfo.roiptr, dispInfo.open

;--- redraw the rest of them
            if handle_info(dispInfo.roiptr, /num_child) ne 0 then begin
                child = handle_info(dispInfo.roiptr, /first_child)
                repeat begin
                    handle_value, child, roi
                    roi_draw_object, roi
                    child = handle_info(child, /sibling)
                endrep until child eq 0
            endif
            device, copy = [0, 0, !d.x_size, !d.y_size, 0, 0, dispInfo.backing(1)]
        endelse
        dispInfo.nrois = handle_info(dispInfo.roiptr, /num_child)
        if dispInfo.nrois eq 0 then $
          widget_control, dispInfo.editIDs(1), sensitive = 0
        dispInfo.edit = 0
        dispInfo.open = -1
        dispInfo.selected = 0
        dispInfo.initial = 0
        dispInfo.first = 0
        dispInfo.npts = 0
        if dispInfo.ROImode eq 3 then dispInfo.roi_type = dispInfo.p_types(0) $
        else dispInfo.roi_type = dispInfo.p_types(dispInfo.ROImode)
        widget_control, dispInfo.roiTypeBase, /sensitive
        widget_control, dispInfo.label_base, sensitive = 0
        widget_control, dispInfo.editIDs(2), sensitive = 0
        widget_control, dispInfo.editIDs(3), sensitive = 0
        widget_control, dispInfo.editIDs(4), sensitive = 0
    end

    'CUT'    : begin
        if dispInfo.initial eq 1 then begin
            dispInfo.clipboard.roi_type = dispInfo.roi_type
            dispInfo.clipboard.p        = dispInfo.p
            dispInfo.clipboard.handle   = dispInfo.handle
            dispInfo.clipboard.npts     = dispInfo.npts
            dispInfo.clipboard.x        = dispInfo.x
            dispInfo.clipboard.y        = dispInfo.y
        endif else begin
            child = get_child(dispInfo.roiptr, dispInfo.open, 1)
            dispInfo.clipboard = get_handle_value(child)
        endelse

        widget_control, dispInfo.editIDs(5), /sensitive
        goto, DELETE
    end

    'COPY'   : begin
        if dispInfo.initial eq 1 then begin
            dispInfo.clipboard.roi_type = dispInfo.roi_type
            dispInfo.clipboard.p        = dispInfo.p
            dispInfo.clipboard.handle   = dispInfo.handle
            dispInfo.clipboard.npts     = dispInfo.npts
            dispInfo.clipboard.x        = dispInfo.x
            dispInfo.clipboard.y        = dispInfo.y
        endif else begin
            child = get_child(dispInfo.roiptr, dispInfo.open, 1)
            dispInfo.clipboard = get_handle_value(child)
        endelse
        widget_control, dispInfo.editIDs(5), /sensitive
    end

    'PASTE'  : begin
; Is there an open ROI? If so save it
        if dispInfo.open ne -1 then roi_save_roi, dispInfo
; First redraw the exisiting rois
        h = roi_redraw_rois(dispinfo, 0, /scratch)
; Put it in edit mode
        dispInfo.initial = 1
        dispInfo.edit = 1
; The new one is active
        dispInfo.open = dispInfo.nrois+1
; paste the clipboard to r
        dispInfo.roi_type = dispInfo.clipboard.roi_type
        dispInfo.p        = dispInfo.clipboard.p
        dispInfo.handle   = dispInfo.clipboard.handle
        dispInfo.npts     = dispInfo.clipboard.npts
        dispInfo.x        = dispInfo.clipboard.x
        dispInfo.y        = dispInfo.clipboard.y

; draw it with handles
        roi_draw_object, dispInfo, 1, refresh = dispInfo.backing(1)
        widget_control, dispInfo.label_base, /sensitive
    end
    else:
endcase

CLEANUP:
;--- Restore the dispInfo structure
WIDGET_CONTROL, ev.top, SET_UVALUE=dispInfo, /NO_COPY

end

;-----------------------------------------------------------------------------

PRO xdisplay_event, ev
;--- Event handler for all button presses except for the mode controls

stash = ev.handler
WIDGET_CONTROL, stash, GET_UVALUE=dispInfo, /NO_COPY

;--- See if it was a tracking event
name = tag_names(ev, /structure)
if name eq 'WIDGET_TRACKING' then begin
;--- If it was the base that generated it then change the input focus
    if (ev.id eq dispInfo.base) then begin
        if (ev.enter eq 1) then begin
;--- Sync the number of rois 
            dispInfo.nrois = handle_info(dispInfo.roiPtr, /num_child)
            widget_control, dispInfo.labelID, $
              set_value = 'ROI'+strcompress(dispInfo.nrois+1)

            showImage, dispInfo
            
;--- Change the greyscale to the appropriate value.
            if dispInfo.colorMode eq 0 then $
              wscale, dispInfo.win, dispInfo.lev, $
              density   = dispInfo.density, $
              threshold = dispInfo.threshold, $
              range     = dispInfo.drange, $
              bottom = dispInfo.greyBottom, $
              ncolors = dispInfo.ngreys
        endif

;--- When leaving Check to see if there is an open roi and save it
        if (ev.enter eq 0) then begin
            if (dispInfo.open ne -1) then $
              roi_save_roi, dispInfo
        endif
        goto, CLEANUP
    endif
endif

CLEANUP:
;--- Restore the dispInfo structure
WIDGET_CONTROL, stash, SET_UVALUE=dispInfo, /NO_COPY

END

;-----------------------------------------------------------------------------

pro quitter, ev
   widget_control, ev.top, /destroy
end

;-----------------------------------------------------------------------------

PRO draw_zoom, state, newx, newy
;--- This was taken from cw_zoom.  Currently unused

; compute size of rectangle in original image
; round up to make sure image fills zoom window

  rect_x = long(state.x_zm_sz / float(state.scale) + 0.999)
  rect_y = long(state.y_zm_sz / float(state.scale) + 0.999)

  ; compute location of origin of rect (user specified center)
  x0 = newx - rect_x/2
  y0 = newy - rect_y/2

  ; make sure rectangle fits into original image
  ;left edge from center
  x0 = x0 > 0
  ; limit right position
  x0 = x0 < (state.x_im_sz - rect_x)

  ;bottom
  y0 = y0 > 0
  y0 = y0 < (state.y_im_sz - rect_y)

  IF (state.scale EQ 1) THEN BEGIN
    ;Save window number
    save_win = !D.WINDOW
    WSET, state.zoom_win
    IF (rect_x GT state.x_im_sz OR rect_y GT state.y_im_sz) THEN BEGIN
      ERASE
      IF (rect_x GT state.x_im_sz) THEN x0 = 0 & rect_x = state.x_im_sz
      IF (rect_y GT state.x_im_sz) THEN y0 = 0 & rect_y = state.y_im_sz
    ENDIF
    ; don't use tvscl here, to preserve same range as in unzoomed image
    TV, state.orig_image(x0:x0+rect_x-1,y0:y0+rect_y-1)
    ;Restore window
    IF (save_win NE -1) THEN WSET, save_win
  ENDIF ELSE BEGIN
    ;Make integer rebin factors.  These may be larger than the zoom image
    dim_x = rect_x * state.scale
    dim_y = rect_y * state.scale

    x1 = x0 + rect_x - 1
    y1 = y0 + rect_y - 1

    temp_image = rebin(state.orig_image(x0:x1,y0:y1), $
                       dim_x, dim_y, $
                       sample=state.sample)

    ;Save the zoomed image
    state.zoom_image = $
                     temp_image(0:state.x_zm_sz-1,0:state.y_zm_sz-1)

    ;Save the corners in original image
    state.x0 = x0
    state.y0 = y0
    state.x1 = x1
    state.y1 = y1

    ;Display the new zoomed image
    ;Save window number
    save_win = !D.WINDOW
    WSET, state.zoom_win
    ; don't use tvscl here, to preserve same range as in unzoomed image
    TV, state.zoom_image
    ;Restore window
    IF (save_win NE -1) THEN WSET, save_win

 ENDELSE

END

;-------------------------------------------------------------

function zoomWindow, xsize, ysize, zoom
;--- Create a popup window for zoomed images.  Currently unused

base = widget_base(title = 'Zoomed Image (x',strtrim(zoom,2)+')')
draw = widget_draw(base, xsize = xsize, ysize = ysize, /button_events)

widget_control, base, /realize
widget_Control, draw, get_value = temp
zoomInfo = { base: base, $
             draw: draw, $
             theDraw: temp(0), $
             xsize : xsize, $
             ysize : ysize, $
             zoom  : zoom}
widget_control, base, set_uvalue = zoomInfo, /no_copy
end

;-----------------------------------------------------------------------------

pro xdisplay_cleanup, tlb
;--- Cleanup procedure for the widget   
Widget_Control, tlb, Get_UValue=info

;--- Only perform cleanup if info structure is valid
IF N_Elements(info) NE 0 then begin

    if not(info.wasPtr) THEN $
        handle_free, info.dataptr
    if not(info.maskwasPtr) and handle_info(info.maskPTR) THEN $
        handle_free, info.maskptr

;--- Only free the xy value if it wasn't called by another widget
    if info.callingbase eq 0 then $
      handle_free, info.xy

;--- Delete the backing store pixmaps
    for i = 0,n_elements(info.backing)-1 do wdelete, info.backing(i)
endif

end 

;-----------------------------------------------------------------------------


function xdisplay, argv, argv2, $
                   DELAY          = delay, $ ; The delay to start scrolling
                   MIN            = min, $ ; The min for bytscl
                   MAX            = max, $ ; The max for bytscl
                   ZOOM           = zoom, $ ; The zoom factor
                   BOTTOM         = bottom, $
                   NGREYS         = ngreys, $
                   NCOLORS        = ncolors, $
                   ROIPTR         = roiPtr, $
                   TITLE          = title, $ ; The title for the widget
                   ALLSLICE       = allslice, $ ; Keep ROIs same for all slices
                   JUST_REG       = just_reg, $
                   MODAL          = modal, $
                   UVALUE         = uvalue, $
                   GROUP          = group  ; Group leader

;ON_ERROR, 1					;return to main

;--- Check to make sure this is running on an 8bit display
window,/free, /pixmap,xs=1,ys=1
wdelete,!d.window
if !d.table_size gt 256 then begin
    ok = widget_message('xdisplay only runs on 8 bit displays.', /error)
    return,0
endif

if Xregistered(title) then return, 0

;--- Set up the menu buttons
buttons, magbuttons, winbuttons, roibuttons, irrbuttons, cirbuttons, $
	squbuttons, magicbuttons, folder, disk, eraser, scissors, copy, paste

if not(keyword_set(ZOOM))     then zoom = 1
IF not(keyword_set(TITLE))    then title = 'XDisplay'
IF NOT(KEYWORD_SET(GROUP))    THEN group = 0
if n_elements(delay)  eq 0    then delay = 0.5
if n_elements(uvalue) eq 0    then uvalue = '' else uvalue = strtrim(uvalue,2)
allFlag = not(keyword_set(allslice))

if n_elements(roiPtr) eq 0 then begin
    roiPtr = init_rois()
    xy = handle_create()
endif else begin
    if handle_info(roiPtr, /valid) then begin
        roi = get_handle_value(roiPTR, /no_copy)
        xy = roi.xy
        set_handle_value, roiPTR, roi, /no_copy

        nchild = handle_info(roiPtr, /num)
        if nchild gt 0 then begin
            for i=1,nchild do begin
                child = get_child(roiPtr, i)
                roi = get_handle_value(child, /no_copy)
                roi.xy = xy
                set_handle_value, child, roi, /no_copy
            endfor
        endif
    endif
endelse

modalFlag    = keyword_set(modal)
registerFlag = keyword_set(just_reg)
greyFlag     = keyword_set(ngreys)
colorFlag    = keyword_set(ncolors)

junk = getcolors(/load)
colorsize = size(junk)
nauxColors = colorSize(1)
;--- if ngreys or ncolors not specified setup color table
  if not(greyFlag or colorFlag) then begin
      ngreys      = (!d.n_colors - nauxColors) / 2
      ncolors     = ngreys
  endif else begin
;--- Make sure not too many greys wanted
      if greyFlag then begin
          if (ngreys gt (!d.n_colors-nauxColors)) then begin
              string = 'Too many grey levels. ngreys < '+strtrim(!d.n_colors - nauxColors,2)
              ok = widget_message(string, /error)
              return, 0
          endif
      endif
;--- Make sure not too many colors wanted
      if colorFlag then begin
          if (ncolors gt (!d.n_colors-nauxColors)) then begin
              string = 'Too many colors. ncolors < '+strtrim(!d.n_colors - nauxColors,2)
              ok = widget_message(string, /error)
              return, 0
          endif
      endif
;--- Make sure not too many colors+greys wanted
      if (colorFlag and greyFlag) then begin
          if (ncolors+ngreys) gt (!d.n_colors-nauxColors) then begin
              string = 'Too many colors and grey levels. ' + $
                '(ncolors + ngreys) < '+strtrim(!d.n_colors - nauxColors,2)
              ok = widget_message(string, /error)
              return, 0
          endif
      endif else begin
          if greyFlag then ncolors = (!d.n_colors - nauxColors) - ngreys $
          else ngreys = (!d.n_colors - nauxColors) - ncolors
      endelse
  endelse
  greyBottom  = 0
  colorBottom = ngreys


  colorMode = 0
;--- set the default window and leveling for greyscale images and colors
  if ncolors ne 0 then  loadct, 11, ncolors = ncolors, bottom=ngreys, /silent
  if ngreys ne 0 then  wscale, 20, ngreys/2, bottom = 0, ncolors = ngreys $
  else colorMode = 1
  
  if n_elements(argv) lt 1 then begin
      ok = widget_message('Need data',/error)
      return, -1
  endif
  
  if (whatTypeVariable(argv) eq 'LONG') and (n_elements(argv) eq 1) then begin
;--- Make sure it's a valid pointer
      if not(handle_info(argv, /valid)) then message, 'Invalid pointer.'
      data = get_handle_value(argv, /no_copy)
      dataptr = argv
      wasPtr = 1
  endif else begin
      dataptr = handle_create()
      data = argv
      wasPtr = 0
  endelse


;--- Get the datasize
  datasize = size(data)
  m1 = min(data, max=m2)
  if n_elements(MIN) eq 0 then min = m1
  if n_elements(MAX) eq 0 then max = m2
  set_handle_value,dataptr, data, no_copy = wasPtr

  if n_elements(argv2) gt 0 then begin
      if (whatTypeVariable(argv2) eq 'LONG') and (n_elements(argv2) eq 1) then begin
;--- Make sure it's a valid pointer
          if not(handle_info(argv2, /valid)) then message, 'Invalid pointer.'
          mask = get_handle_value(argv2, /no_copy)
          maskptr = argv2
          maskwasPtr = 1
      endif else begin
          maskptr = handle_create()
          mask = argv2
          maskwasPtr = 0
      endelse
;--- Make sure the mask is of the right size
      s = datasize(1)*datasize(2)
      if s ne n_elements(mask) then begin
          if wasPTR     ne 1 then handle_free, dataptr
          if maskwasPTR ne 1 then handle_free, maskptr
          ok = widget_message('Mask and data incompatible.', /error)
          return, -1
      endif
      set_handle_value, maskPTR, mask, no_copy = maskwasPTR
  endif else begin
      maskPTR = 0L
      maskwasPTR = 0
  endelse

;--- Initialization
  modeHandlers = ['drawEvent', 'zoomEvent', 'roiDrawEvent']
  
  junk = {CW_PDMENU_S, flags:0, name:''}

  colorModeMenu = ['Greyscale', 'Color']
  
  optionmenu = ['Normal', 'Density', 'Threshold']

  roiTypes = ['Irregular','Rectangle','Circle', 'MagicWand']

  clipboard = replicate({ROI_S}, 1)

  dispInfo = {callingBase    : group, $ ; calling base widget ID
              base           : 0L, $ ; base widget ID
              draw           : 0L, $ ; draw widget ID
              lut            : 0L, $ ; LUT display
              zoomPopBase    : 0L, $ ; ID for popup droplist (unused)
              dismiss        : 0L, $ ; dismiss button ID
              options        : 0L, $ ; option pdmenu
              roiEditBase    : 0L, $ ; ROI edit button base
              roiBase        : 0L, $ ; base for ROI widgets (types and labels)
              roiTypeBase    : 0L, $ ; base for ROI buttons
              rInfo          : 0L, $ ; pointer to the rinfo structure
              modeButtons    : lonarr(3), $ ; Mode button IDs
              type           : 0L, $ ; Widget id for type button group      aaa
              label_base     : 0L, $ ; Base widget for the label
              labelID        : 0L, $ ; Widget id for label widget
              editIDs        : lonarr(6), $ ; IDs for edit buttons
              roiButtons     : lonarr(4), $ ; ROI button IDs                aaa
              modeHandlers   : modeHandlers, $ ;  mode event handler names
              ROImode        : 0, $ ; ROI Type Index. redundant, fix later
              mode           : 0, $ ; 0 == winlev 1==zoom
              optionMenu     : optionMenu, $
              title          : title, $ ; title of the widget
              d_evnt         : 0, $ ; draw events are on (for middle mouse)
              magic          : 0, $ ; drawing a magic wand
              theDraw        : 0, $ ; The draw window ID
              thelut         : 0, $ ; the lut draw window ID
              backing        : lonarr(2), $ ; Backing pixmaps for display
              xsize          : datasize(1), $ ; xsize of the draw widget
              ysize          : datasize(2), $ ; ysize of the draw widget
              ngreys         : ngreys, $ ; Number greys used
              greybottom     : 0, $ ; bottom of the color table
              ncolors        : ncolors, $ ; Number of colors used
              colorbottom    : colorbottom, $
              aux_colors     : nauxColors, $ ; number of drawing colors
              textColor      : 'RED', $ ; Color for text display
              ROIcolor       : 'RED', $ ; The color to draw the rois
              colorMode      : colorMode, $ 0 == grey 1 == color
              colorModes     : colorModeMenu, $
              optionModes    : optionmenu, $
              delay          : delay, $ ; time delay for scrolling
              sdown          : 0, $ ; Scrolling is on/off
              dataptr        : dataptr, $ ; Pointer to the data
              wasPtr         : wasPtr, $ ; Called with a pointer to the data
              datasize       : datasize, $ ; size of the data
              maskPTR        : maskPTR, $
              maskwasPTR     : maskwasPTR, $
              min            : min, $ ; min of data
              max            : max, $ ; max of data
              zoom           : zoom, $ ; Zoom factor
              zoomMax        : 5, $  ; max zoom factor
              zoomMin        : 1,$ ; min zoom factor
              allFlag        : allFlag, $ ; Keep ROIs same for all slices 
              density        : 0, $ ; density slicing on?
              drange         : [50,100], $ ; min max for density
              irange         : [50,100], $ ; temp min max for density for changes
              ctscale        : 0.0, $ ; Scaling for LUT
              threshold      : 0, $ ; thresholding on?
              img_num        : 0, $ ; Current image number for stack
              win            : 20, $ ; Current window level
              oldwin         : 20, $ ; Old window level
              win_max        : 512, $ ; Max windowing
              win_min        : 1, $ ; Min windowing
              win_scale      : 1, $ ; Scaling for window
              lev            : ngreys/2, $ ; Current level level
              oldlev         : ngreys/2, $ ; Old level level
              lev_max        : 1024, $ ; Max leveling
              lev_min        : -1024, $ ; Min leveling
              lev_scale      : 1, $ ; Scaling for level
              ix             : 0, $ ; Where mouse was pressed
              iy             : 0, $ ; Where mouse was pressed
              roiPtr      : roiPtr, $ ; Pointer to the ROI structure  ***ROI INFO***
              xy          : xy, $ ; Pointer to xy values for Irregular type
              clipboard   : clipboard, $ ; For copy/paste ROIs
              p_types     : roitypes, $ ; Possible roi types
              roi_type    : 'Irregular', $ ; ROI Type
              label       : 'ROI 1', $ ; ROI label
              uvalue      : uvalue, $ ; Auxiallary label for ROI_S
              open        : -1, $ ; Which ROI is open for editing
              edit        : 0,  $ ; Flag indicating a previous ROI is open
              initial     : 0,  $ ; Flag indicating roi has yet to be saved
              selected    : 0,  $ ; Has an ROI been selected
              nrois       : 0,  $ ; How many ROIs have been saved
              first       : 0,  $ ; Is this the first time for this ROI
              grain       : 2.0, $ ; The grid granularity
              pos         : [0.0, 0.0], $ ; The current position
              p           : [0.0,0.0,0.0,0.0], $ ; x1, y1, x2, y2
              handle      : fltarr(2,4), $ ; 4 pairs of xy points the handles
              ihandle     : 0, $ ; Which handle was clicked?
              orig        : fltarr(2), $ ; For use in move_resize
              temp        : fltarr(3), $ ; temp storage for move_resize
              npts        : 0,$ ; Num pts contained in ROI
              x           : fltarr(512), $ ; The x points
              y           : fltarr(512)} ; the y points

  base = WIDGET_BASE(title = title, $
                     /tracking, $
                     FUNC_GET_VALUE = "xdisplay_get_value", $
                     PRO_SET_VALUE = "xdisplay_set_value", $
                     GROUP_LEADER = group)

  dispInfo.base = base

  colBase = widget_base(base, /column)

  drawBase = widget_base(colBase, /colum)   
  
  cntlBase = widget_base(drawBase, /column, /align_top)

;--- Top row menu base
  dummyBase = widget_base(cntlBase, /row, /frame)
  menuBase = widget_base(dummyBase, /row, event_pro = 'menuEvent')
  open = widget_button(menuBase, value = reform(folder), uvalue = 'OPEN')
  save = widget_button(menuBase, value = disk, uvalue = 'SAVE')

  dispInfo.roiEditBase = widget_base(dummyBase, /row, event_pro = 'menuEvent')
  delete = widget_button(dispInfo.roiEditBase, value = eraser, uvalue = 'DELETE')
  cut = widget_button(dispInfo.roiEditBase, value = scissors, uvalue = 'CUT')
  copy = widget_button(dispInfo.roiEditBase, value = copy, uvalue = 'COPY')
  paste = widget_button(dispInfo.roiEditBase, value = paste, uvalue = 'PASTE')
  dispInfo.editIDS = [open, save, delete, cut, copy, paste]
  for i = 1,5 do widget_control, dispInfo.editIDs(i), sensitive = 0
  widget_control, dispInfo.roiEditBase, sensitive = 0

;--- Sub base for modes
  modeBase = widget_base(dummyBase, /row, $
                         event_pro = "modeEvent", $
                         func_get_value = "modeEvent_get", $
                         pro_set_value = "modeEvent_set")

  levButton = widget_button(modeBase, $
                            value = reform(winbuttons(0,*,*)), $
                            uvalue = winbuttons, /no_rel)
  zoomButton = widget_button(modeBase, $
                             value = reform(magbuttons(1,*,*)), $
                             uvalue = magbuttons, /no_rel)
  roiButton = widget_button(modeBase, $
                            value = reform(roibuttons(1,*,*)), $
                            uvalue = roibuttons, /no_rel)
  dispInfo.modeButtons = [levButton, zoomButton, roiButton]
  dispInfo.mode = 0

;--- ROI Base 
  dispInfo.roiBase = widget_base(cntlBase, /row, /frame)
  dispInfo.roiTypeBase = widget_base(dispInfo.roiBase, /row, $
                                 event_pro = "roiModeEvent")
  irrButton = widget_button(dispInfo.roiTypeBase, $
                            value = reform(irrbuttons(0,*,*)), $
                            uvalue = irrbuttons, /no_rel)
  squButton = widget_button(dispInfo.roiTypeBase, $
                             value = reform(squbuttons(1,*,*)), $
                             uvalue = squbuttons, /no_rel)
  cirButton = widget_button(dispInfo.roiTypeBase, $
                            value = reform(cirbuttons(1,*,*)), $
                            uvalue = cirbuttons, /no_rel)
  magButton = widget_button(dispInfo.roiTypeBase, $
                            value = reform(magicbuttons(1,*,*)), $
                            uvalue = magicbuttons, /no_rel)
  dispInfo.roiButtons = [irrButton, squButton, cirButton, magButton]
  dispInfo.roi_type = 'Irregular'
  dispInfo.ROImode = 0

  dispInfo.label_base = widget_base(dispInfo.roiBase, /row)
  junk = widget_label(dispInfo.label_base, value = 'Label: ')
  dispInfo.labelID = widget_text(dispInfo.label_base, $
                              xsize=13, $
                              ysize = 1, $
                              /editable)

  n = handle_info(roiPTR, /num)
  widget_control, dispInfo.labelID, set_value = 'ROI '+strtrim(n+1,2)
  dispInfo.label = 'ROI '+strtrim(n+1,2)
  widget_control, dispInfo.roiBase, sensitive = 0

;--- Window Base
  windowBase = widget_base(drawBase, /row)
  dispInfo.draw = widget_draw(windowBase, $
                              event_pro = "drawEvent", $
                              /button_events, $
                              xsize = dispInfo.xsize*dispInfo.zoom, $
                              ysize = dispInfo.ysize*dispInfo.zoom, $
                              retain=2)
  dispInfo.lut = widget_draw(windowBase, $
                             event_pro = "LUTEvent", $
                             /button_events, $
                             xsize = 20, $
                             ysize = dispInfo.ysize*dispInfo.zoom, $
                             retain = 2)

  bottomBase = widget_base(colBase, /row, /align_center)
  dispInfo.dismiss = widget_button(bottomBase, value = 'Dismiss', $
                                   /no_rel, $
                                   event_pro = 'quitter')

  if ngreys eq 0 then $ 
    junk = widget_button(bottomBase, value = 'Xcolors', $
                         event_pro = 'colorToolEvent') $
  else $
    dispInfo.options = widget_droplist(bottomBase, value = optionMenu, $
                                       event_pro = 'optionDropEvent')

  if (ngreys ne 0) and (ncolors ne 0) then begin
      junk = widget_droplist(bottomBase, value = colorModeMenu, $
                             event_pro = 'colorDropEvent', $
                             uvalue = colorModeMenu)
  endif
  
  
  widget_control, base, /realize
  
  widget_control, dispInfo.draw, get_value = temp
  dispInfo.theDraw = temp
  widget_control, dispInfo.lut, get_value = temp
  dispInfo.theLUT = temp
  wset, temp


;--- display the LUT for greyscale
  if dispInfo.colormode eq 0 then begin
      junk = indgen(dispInfo.ngreys)
      ct = bytarr(2,dispInfo.ngreys)
      ct(0, *) = byte(junk)
      ct(1, *) = ct(0, *)
      dispInfo.ctscale = float(dispInfo.ngreys)/float(dispInfo.ysize*dispInfo.zoom)
  endif else begin
      junk = indgen(dispInfo.ncolors)
      ct = bytarr(2,dispInfo.ncolors)
      ct(0, *) = byte(junk)
      ct(1, *) = ct(0, *)
  endelse
  wset, dispInfo.theLUT
  tv, congrid(ct,20,dispInfo.ysize*dispInfo.zoom)

;--- Sync the number of rois 
  dispInfo.nrois = handle_info(roiPtr, /num_child)

  showImage, dispInfo

  WIDGET_CONTROL, base, SET_UVALUE=dispInfo, /NO_COPY

  Xmanager, title, base, event_handler = "xdisplay_event", $
    cleanup = "xdisplay_cleanup", just_reg = registerFlag, modal = modalFlag
  return, roiPtr
  help,roiPtr
END
