;+
; NAME:
;    NIset_ct
;
; PURPOSE:
;    Set the color lookup table(s). Most general call is 
;    "NIset_ct, /interactive".
;
; CATEGORY:
;    Widgets
;
; CALLING SEQUENCE:
;    NIset_ct, nrct=nrct, ctnum=ctnum, table=table, interact=interact, $
;             nodrawcolors = nodrawcolors, group=group, current=current, $
;             maxnrct=maxnrct
;
;    When called without parameters, only some initializations are carried out
;    and the drawcolors are set (set /nodrawcolors to avoid that).
;
; KEYWORD PARAMETERS:
;    NRCT: define the number of look up tables.
;
;    CTNUM: the number, or an array with numbers, of lookup tables. To be
;           combined with table.
;
;    TABEL: set the lookup table CTNUM to the predefined table TABLE, using
;           LOADCT.
;           CTNUM and TABLE are values or arrays of the same length. See LOADCT
;           and XLOADCT for predefined lookup tables.
;
;    INTERACTIVE:
;       If CTNUM is set, XLOADCT is called, but its operation is restricted
;       to lookup table CTNUM.
;       If CTNUM is not set, a widget is started, allowing the user to select a
;       lookup table to manipulate with XLOADCT, and to set the current lookup
;       table.
;       If group is set, the widget uses group as the group_leader.       
;
;    NODRAWCOLORS: When set, no drawingcolors are defined. Otherwise, 
;        NIdrawcolors is called to set the lowest values of the lookup table
;        to predefined colors.
;
;    GROUP: the group_leader (ignored when INTERACTIVE is not set).
;
;    CURRENT: define a lookup table as the current one. It becomes the default
;        lookup table used by NIimage.
;
;    MAXNRCT: the maximum number of lookup tables in the interactivewidget.
;       Default value is 5. Ignored when INTERACTIVE is not set.
; 
;
; COMMON BLOCKS:
;    NIct_common
;    (Shared with NIimage, NIget_ct, and NIdrawcolors).
;
;
; SEE ALSO:
;    NIget_ct, NIimage, NIdrawcolors.
;
; EXAMPLE:
;    NIset_ct, /inter
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, nov 1996
;-

pro NIset_ct_event, ev

  widget_control, ev.id, get_uvalue = uvalue
  if uvalue eq 'quit' then begin
     widget_control, ev.handler, /destroy
     return
  endif
    
  if uvalue eq 'nicolor' then begin
     NIcolor
     return
  endif
    
  if uvalue eq 'slider' then begin
    widget_control, ev.id, get_value = value
    if value ne NIget_ct(/nrct) then begin
      widget_control, ev.handler, get_uvalue = status
      widgetnum = status.widgetnum
      widget_control, widgetnum(1), /destroy
      widgetnum(1) = widget_base(widgetnum(0), /row)
      for i = 0, value -1 do $
        knop = widget_button(widgetnum(1), value = NIstring(i), $
               uvalue = NIstring(i))
      status.widgetnum = widgetnum
      widget_control, ev.handler, set_uvalue = status
      NIset_ct, nrct = value
      widget_control, widgetnum(2), get_value = win
      wset,win
      niimage, findgen(200) # (fltarr(1,20)+1), /use_all_colors
      if NIget_ct(/current) ge value then NIset_ct, current=value-1
    endif
    goto, show_current
  endif

  if uvalue eq 'current' then begin
    widget_control, ev.id, get_value = value
    NIset_ct, current = value
    widget_control, ev.id, set_value = NIget_ct(/current)
    goto, show_current
  endif

  ctnum = fix(uvalue)
  widget_control, ev.handler, get_uvalue = status
  if status.file ne '' then file = status.file
  NIset_ct, ctnum = ctnum, /int, group=ev.handler, file=file
  NIset_ct, current=ctnum
  
show_current:
    widget_control, ev.handler, get_uvalue = status
    widgetnum = status.widgetnum
    widget_control, widgetnum(4), set_value = NIget_ct(/current)
    widget_control, widgetnum(3), get_value = win
    wset,win
    niimage, findgen(70) # (fltarr(1,20)+1)

end

;==========
pro NIset_ct, nrct=nrct, ctnum=ctnum, tabel=tabel, interact=interact, $
              nodrawcolors = nodrawcolors, group=group, current=current, $
              maxnrct=maxnrct, file = file, kulcolor=kulcolor, $
              drawred=drawred, drawgreen=drawgreen, drawblue=drawblue, $
              drawstandard= drawstandard

common NIct_common, number_ct, ct_low, ct_high, draw_low, draw_high, current_ct
common NIfiles

if keyword_set(kulcolor) then file = nicolorfile

; If the common variables are not yet define, define them
;============
if keyword_set(nodrawcolors) then begin
  draw_low = -1
  draw_high = -1
endif else begin
  nidrawcolors, /nostretch, standard= drawstandard, $
              drawred=drawred, drawgreen=drawgreen, drawblue=drawblue
  if n_elements(ct_low) eq 0 then ct_low = draw_high + 1 $
                             else ct_low(0) = draw_high + 1
endelse
if n_elements(number_ct) eq 0  then number_ct = 1
if n_elements(ct_high) eq 0 then begin
  ct_low  = draw_high + 1
  ct_high = !d.table_size - 1
endif
if n_elements(current_ct) eq 0 then current_ct = 0

if n_elements(nrct) gt 0 then begin
  number_ct = nrct
  maxval = !d.table_size
  if draw_high ge 0 then nidrawcolors, /nostretch, standard=drawstandard,$
              drawred=drawred, drawgreen=drawgreen, drawblue=drawblue
  ct_low  = intarr(nrct)
  ct_high = intarr(nrct)
  for i = 0, nrct-1 do begin
    ct_low(i) = draw_high + 1 + (i*(!d.table_size - draw_high-1)) / nrct
    ct_high(i) = draw_high + 1 + ((i+1)*(!d.table_size - draw_high-1)) / nrct-1
    loadct, 0, bottom=ct_low(i), ncolors = ct_high(i)-ct_low(i)+1, file=file
  endfor
  current_ct = current_ct < (number_ct - 1)
endif

if n_elements(tabel) gt 0 then $
  if n_elements(ctnum) gt 0 then begin
    for i = 0, n_elements(ctnum) -1 do begin
      loadct, tabel(i), bottom=ct_low(ctnum(i)), $
              ncolors=ct_high(ctnum(i))-ct_low(ctnum(i))+1, file=file
    endfor
  endif else begin
    loadct, tabel(0), bottom=ct_low(current_ct), $
              ncolors=ct_high(current_ct)-ct_low(current_ct)+1, file=file
  endelse

if n_elements(current) gt 0 then begin
  if current lt number_ct then current_ct = current
endif

if keyword_set(interact) then begin
  if n_elements(ctnum) ne 0 then begin
    if n_elements(group) eq 0 $
      then nixloadct, bottom=ct_low(ctnum(0)), $
               ncolors=ct_high(ctnum(0))-ct_low(ctnum(0))+1, file=file $
      else nixloadct, bottom=ct_low(ctnum(0)), file=file, $
               ncolors=ct_high(ctnum(0))-ct_low(ctnum(0))+1, group=group
  endif else begin
    if n_elements(maxnrct) eq 0 then maxnrct = 5 > number_ct
    if n_elements(group) gt 0 $
        then base = widget_base(/col, group=group, title='NIset_ct') $
        else  base = widget_base(/col, title='NIset_ct')
    draw1  = widget_draw(base, xsize = 200, ysize = 20)
    slid1 = widget_slider(base, value = number_ct, uvalue='slider', min=1, $
                          max=maxnrct, title = 'nr tables')
    curwid = widget_base(base,/row)
    slid2 = widget_slider(curwid, value = current_ct, uvalue = 'current', $
                          min=0, max = maxnrct-1, title = 'current')
    draw2 = widget_draw(curwid,xsize=70, ysize=20)
    row   = widget_base(base, /row)
    label = widget_label(row, value = 'Edit CT')
    buttons = widget_base(row, /row)
    for i = 0, number_ct -1 do $
      knop = widget_button(buttons, value = NIstring(i), uvalue = NIstring(i))
    rij = widget_base(base, /row)
    dummy = widget_button(rij, value = 'NIcolor', uvalue = 'nicolor')
    quit  = widget_button(rij, value = 'quit', uvalue = 'quit')
    if n_elements(file) eq 0 then file = ''
    status = {widgetnum : [row, buttons, draw1, draw2,slid2], $
              file : file}
    widget_control, base, set_uvalue = status
    widget_control, base, /realize
    widget_control, draw1, get_value = win
    wset,win
    niimage, findgen(200) # (fltarr(1,20)+1), /use_all_colors
    widget_control, draw2, get_value = win
    wset,win
    niimage, findgen(70) # (fltarr(1,20)+1)
    xmanager, "NIset_ct", base
  endelse
endif
IF (!D.N_COLORS GT 256) THEN DEVICE, DECOMPOSED=0 

end
