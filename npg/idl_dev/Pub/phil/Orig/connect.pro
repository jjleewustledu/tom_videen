;+
; NAME: CONNECT.PRO
;
; PURPOSE: An example to illustrate using XDISPLAY in your own widget
;          programs.  Specifically: 
;          1) sharing ROIs across xdisplays,
;          2) Defining ROIs on a slice-by-slice basis or not
;          3) Cleaning up after Xdisplay
;
; CATEGORY: Examples
;
; CALLING SEQUENCE: connect, data
; 
; INPUTS: data : 2D or 3D stack of data
;
; PROCEDURE:
;    IDL> .com xdisplay
;    IDL> data = getimage()
;    IDL> connect, data
;
; MODIFICATION HISTORY:
;    20 Jan 97 Initial Coding PMW
;-
pro connectEvent, ev

;--- Get out the info structure
widget_control, ev.top, get_uvalue = info, /no_copy

;--- Are we quitting?
if ev.id eq info.quit then begin

;--- Store the info structure
    widget_control, ev.top, set_uvalue=info,/no_copy

;--- Destroy the widget
    widget_control, ev.top, /destroy
    return
endif

;--- Which display button was pressed
foo = where(ev.id eq info.button)
if foo(0) ne -1 then begin
;--- Define a title
    title = 'Display '+strtrim(foo(0) + 1,2)

;--- Call xdisplay.  
;    For Display 1 ROIs defined by slice.           foo(0) = 0
;    For Display 2 ROIs defined across all slices.  foo(0) = 1
    roi = xdisplay(info.data, $
                   roiPTR   = info.roiptr, $
                   title    = title, $
                   allslice = foo(0), $
                   group    = info.base)

;--- Get the widget ID
    info.disps(foo(0)) = xregistered(title, /get_id)

;--- Print some information
    print,title,' ID = ',strtrim(info.disps(foo(0)),2)
endif

foo = where(ev.id eq info.cols)
if foo(0) ne -1 then begin
    newcolor = info.colors(ev.index)
    if widget_info(info.disps(foo(0)), /valid) then $
      widget_control, info.disps(foo(0)), set_value = 'ROICOLOR = ' + newcolor
endif

;--- Store the info structure
widget_control, ev.top, set_uvalue=info,/no_copy
end

;-----------------------------------------------------------------------------

pro connect_cleanup, tlb
widget_control, tlb, get_uvalue = info, /no_copy

if n_elements(info) ne 0 then begin
    handle_free, info.roiptr
    handle_free, info.xy
endif
end

;-----------------------------------------------------------------------------

pro connect, data

;--- Set up color array
colors = ['BLACK', 'WHITE', 'RED', 'GREEN', 'BLUE', 'CYAN', 'MAGENTA', 'YELLOW', $
           'CHARCOAL' ]

;--- Initialize the roiptr
roiptr = init_rois()

;--- Create a handle for the xy points
xy = handle_create()

;--- Get the roi struture out of storage
roi = get_handle_value(roiptr, /no_copy)

;--- Set roi.xy = xy
roi.xy = xy

;--- Store the roi to roiptr
set_handle_value, roiptr, roi, /no_copy

;--- Create a base widget
base = widget_base(title = 'connect', /column)

;--- Define some buttons
colorbase = widget_base(base, /row)
label = widget_label(colorbase, value = 'Display 1 Color:')
col  = widget_droplist(colorbase, value = colors)

colorbase = widget_base(base, /row)
label = widget_label(colorbase, value = 'Display 1 Color:')
col2  = widget_droplist(colorbase, value = colors)

but0 = widget_button(base, value = 'Display 1')
but1 = widget_button(base, value = 'Display 2')
quit = widget_button(base, value = 'quit')

;--- Set up the info structure.
info = {base   : base, $
        data   : data, $
        roiptr : roiptr, $
        xy     : xy, $
        quit   : quit, $
        disps  : lonarr(2), $
        cols   : [col, col2], $
        colors : colors, $
        button : [but0, but1]}

;--- Realize the widget
widget_control, base, /realize

;--- Store the info struture
widget_control, base, set_uvalue= info, /no_copy

;--- Register with the window manager and start processing events.
xmanager, "connect", base, event_handler="connectEvent", cleanup = "connect_cleanup"

end
