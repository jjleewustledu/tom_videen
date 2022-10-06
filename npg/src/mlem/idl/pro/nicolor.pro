;===============================================================================
pro NIcolor_event, ev

common NIct_common, number_ct, ct_low, ct_high, draw_low, draw_high, current_ct
common NIfiles
COMMON colors, r_orig, g_orig, b_orig, r_curr, g_curr, b_curr

widget_control, ev.id, get_uvalue = uvalue
widget_control, widget_info(ev.handler, /child), get_uvalue=state
state.ctnum = state.ctnum < (number_ct-1)
case uvalue of
  'quit' : begin
           widget_control, ev.handler, /destroy
           return
           end
  
  'nrtab' : begin
            nrtab = NIstring(NIget_ct(/nrct))
            nrtab = NIhey_user('Number of color tables', /beep, ask=nrtab)
            nrtab = fix(nrtab)
            NIset_ct, nrct = nrtab
            widget_control, state.draw1, get_value = win
            wset,win
            NIimage, (fltarr(20,1)+1) # (149-findgen(150))
            end

  'current' : begin
    widget_control, ev.id, get_value = value
    if value ge number_ct then begin
      value = number_ct - 1
      widget_control, ev.id, set_value = value
    endif else begin
      NIset_ct, current=value
      state.ctnum = value
      widget_control, state.draw1, get_value = win
      wset,win
      NIimage, (fltarr(20,1)+1) # (149-findgen(150)), ctnum=state.ctnum
      widget_control, widget_info(ev.handler, /child), set_uvalue=state
    endelse
    end
    
    'gamma' : begin
      WIDGET_CONTROL, state.g_slider, GET_VALUE = gamma
      gamma = 10.0^((gamma/50.) - 1.0)
      WIDGET_CONTROL, state.g_lbl, SET_VALUE = $
			   STRING(gamma, format='(f6.3)')
      state.gamma = gamma
      widget_control, widget_info(ev.handler, /child), set_uvalue=state
      goto, set_gamma
      end
    
    'color' : begin
      kleur = ev.value - 1
      NIset_ct, t = kleur, file=nicolorfile, ctnum=state.ctnum
      widget_control, widget_info(ev.handler, /child), set_uvalue=state
      goto, set_gamma
      end
    
    'min' : begin
      widget_control, ev.id, get_value = bot
      widget_control, state.l_min, set_value = NIstring(bot)
      state.vbot = bot
      widget_control, widget_info(ev.handler, /child), set_uvalue=state
      goto, set_gamma
      end
    
    'max' : begin
      widget_control, ev.id, get_value = top
      widget_control, state.l_max, set_value = NIstring(top)
      state.vtop = top
      widget_control, widget_info(ev.handler, /child), set_uvalue=state
      goto, set_gamma
      end
    
   else : print, 'NIcolor: contacteer J Nuyts! ' + uvalue
endcase
return

set_gamma:
  gamma = state.gamma
	cbot = niget_ct(/low, ctnum=state.ctnum)
	ctop = niget_ct(/high, ctnum=state.ctnum)
	nc   = ctop - cbot + 1
	s = (nc-1)/100.
	x0 = state.vbot * s
	x1 = state.vtop * s
	if x0 ne x1 then s = (nc-1.0)/(x1 - x0) else s = 1.0
	int = -s * x0
	if gamma eq 1.0 then s = round(findgen(nc) * s + int > 0.0) $
	else s = ((findgen(nc) * (s/nc) + (int/nc) > 0.0) ^ gamma) * nc

  too_high = where(s ge nc, n)
  if n gt 0 then s(too_high) = nc-1

  s = s + cbot
	r_curr(cbot) = (r = r_orig(s))
	g_curr(cbot) = (g = g_orig(s))
	b_curr(cbot) = (b = b_orig(s))
	tvlct, r,g,b, cbot

end
;==============================================================================
pro NIcolor, maxnrct=maxnrct, just_reg=just_reg, group_leader = group_leader,$
             table=table, ctnum=ctnum, additional=additional, widget=base, $
             destroy = destroy, parent = parent


common NIct_common, number_ct, ct_low, ct_high, draw_low, draw_high, current_ct
COMMON NIcolor_COMMON, actief
common NIfiles

IF n_elements(actief) NE 0 THEN BEGIN
  if keyword_set(destroy) then begin
    daar = where(widget_info(actief,/valid) EQ 1)
    if daar(0) eq -1 then return
    for i = 0, n_elements(daar)-1 do begin
      widget_control, actief(daar(i)), /destroy
    endfor
    return
  endif
  IF NOT keyword_set(additional) $
    THEN IF max(widget_info(actief,/valid)) NE 0 THEN return
  daar = where(widget_info(actief,/valid) EQ 1)
  IF daar(0) NE -1 $
    THEN actief = actief(daar) $
    ELSE actief =  0L
ENDIF ELSE BEGIN
  actief =  0L
ENDELSE

IF n_elements(ctnum) EQ 0 THEN ctnum = niget_ct(/cur)
niset_ct, ctnum=ctnum
if n_elements(maxnrct) eq 0 then maxnrct = 5 > number_ct

if n_elements(parent) ne 0 then NImapupdate, parent, /off

if n_elements(parent) ne 0 $
  then base = widget_base(parent, /col, title='NIcolor') $
  else base = widget_base(/col, title='NIcolor', group_leader = group_leader)

actief = [actief, base]

g_lbl = WIDGET_LABEL(base, VALUE = STRING(1.0))
g_slider = WIDGET_slider(base, TITLE = "Gamma Correction", $
      MINIMUM = 0, MAXIMUM = 100, VALUE = 50, UVALUE = "gamma", $
      /SUPPRESS_VALUE, /DRAG)
row  = widget_base(base, /row)
col1 = widget_base(row, /col)
l_max = widget_label(col1, value = '100')
draw1  = widget_draw(col1, xsize = 20, ysize = 150)
l_min = widget_label(col1, value = '0')
col2 = widget_base(row,/col)

minmax = widget_base(col2, /row)
slidmin = widget_slider(minmax, value = 0, uvalue = 'min', min=0,$
                      max = 100, title = 'v', /drag, /vert,/sup, ys=100)
slidmax = widget_slider(minmax, value = 100, uvalue = 'max', min=0,$
                      max = 100, title = '^', /drag, /vert,/sup,ys=100)

junk = {cw_pdmenu_s, flags:0, name:''}
desc = [{cw_pdmenu_s, 1, 'Color'}]
colornames = 1
loadct, get_names = colornames, file = nicolorfile
daar = where(colornames eq '-')
if daar(0) ne -1 then colornames = colornames(0:daar(0)-1)

for i = 0, n_elements(colornames)-1 do $
  desc = [desc, {cw_pdmenu_s, 0, colornames(i)}]
desc(n_elements(colornames)).flags = 2

drop = cw_pdmenu(col2, desc, /return_index, uvalue = 'color')


slidnr = widget_slider(col2, value = ctnum, uvalue = 'current',$
              min=0, max = maxnrct-1, title = 'Table nr', /drag, xs=60, $
              scroll=1)
onder = widget_base(base, /row)
nrtab = widget_button(onder, value = 'Nr tab', uvalue = 'nrtab')
quit  = widget_button(onder, value = 'quit', uvalue = 'quit')

device, get_screen_size= screensize
geom = widget_info(base, /geom)

if n_elements(parent) eq 0 $
  then widget_control, base, /realize, $
         tlb_set_x=screensize(0) - geom.scr_xsize - geom.margin, $
         tlb_set_y=(screensize(1) - (geom.scr_ysize - geom.margin) * $
           (number_ct - ctnum)) < (screensize(1)-geom.scr_ysize+geom.margin) $
  else NImapupdate, parent, /on


widget_control, draw1, get_value = win
wset,win
niimage, (fltarr(20,1)+1) # (149-findgen(150)), ctnum=ctnum

state = { base:      base, $
          draw1 :    draw1, $
          g_slider:  g_slider, $
          g_lbl:     g_lbl, $
          vtop:      100, $
          gamma:     1.0, $
          vbot:       0, $
          l_max:     l_max, $
          l_min:     l_min, $
          ctnum:     ctnum}

if n_elements(table) eq 0 then table = 1
NIset_ct, t=table, file=nicolorfile, ctnum=ctnum

widget_control, widget_info(base,/child), set_uvalue=state


XManager, "nicolor", base, just_reg=just_reg

end
