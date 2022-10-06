;
; $Id: tvmenu.pro,v 1.1 90/01/23 16:47:04 wave Exp $
;
; $Id: tvmenu.pro,v 1.1 90/01/23 16:47:04 wave Exp $
;
Function TVMENU1, strings, title, xpos, ypos, prev_ans = prev_ans

; NAME:		TVMENU
; PURPOSE:	Emulate the VAX/VMS PV-WAVE GPX TVMENU function.

if !d.name eq 'X' then begin
   old_window = !d.window
   device, /bypass_translation
   if (old_window eq -1) then wdelete,!d.window
endif

linespacing = fix(!d.y_ch_size * 1.5)
yoffset = 8
if n_elements(title) eq 0 then title = 'Menu'

; set default for previous answer.  If parameter doesn't exist, set to 0
if n_elements(prev_ans) eq 0 then prev_ans = 0 

; Compute width of the window.  The window must be wide enough to hold the
; longest string, whether that is a normal string, or the title.  The 
; larger font of the title, and the DECWindows icon/widgets in the window
; label area must be taken into effect.  Computations for systems other
; than X-Windows are straightforward.

   title_len      = strlen(title)          ; length of title
   max_str_len    = max(strlen(strings))   ; lenght of longest string

   if !D.NAME eq 'X' then begin
      if title_len ge max_str_len / 2 then $
         wid = 12 + title_len * (2 * !d.x_ch_size) $
      else $
         wid = 20 + max_str_len * !d.x_ch_size
   endif else begin
     wid = 10 + (title_len > max_str_len) * !d.x_ch_size 
   endelse

n = n_elements(strings)
if (prev_ans gt n or prev_ans lt 0) then prev_ans = 0  ; check for bogus values
ht = n * linespacing + yoffset - 8
if n_elements(ypos) ne 0 then $
	window,/free,xpos = xpos, ypos = ypos, $
		title=title, xsiz = wid, ysiz = ht ,colors=2 $
  else	window,/free, title=title, xsiz = wid, ysiz = ht ,colors=2

;nc1 = !d.n_colors-1	;Background color
nc1 = 4	;Background color
pcolor = !p.color
bcolor = !p.background
!p.color = nc1
!p.background = 0
erase, nc1		;Set background to white
for i=0,n-1 do $	;Output text
     xyouts, 5, (n-1-i) * linespacing + yoffset/2, $
	strings(i),/dev,/nocl,font=0, color=0

old_choice = -1		;Prev choice = none
;Position cursor at previous position.
tvcrs,!d.x_size/2, ht - (prev_ans+1/2.)*linespacing  
iflag = 0
x1 = wid-1

col = nc1
device,get_graphics=old_gr, set_gr = 6  ;Graph fcn = src XOR dst

repeat begin
	tvrdc,x,y,iflag,/dev	;Get cursor after movement
	iflag = 2		;Next time wait
	if (y ge 0) then begin	;Hit?
	  i = (y / linespacing) < (n-1) ;choice index
	  if (i ne old_choice) then begin
		if old_choice ge 0  then begin  ;Invert old
		   y0 = old_choice * linespacing + yoffset/2-4 ;rect bottom
		   y1 = y0 + linespacing-4 ;top
		   polyfill, [[0,y0],[0,y1],[x1,y1],[x1,y0]],/dev,color=col
		endif
		old_choice = i		;New choice, invert area
		y0 = old_choice * linespacing + yoffset/2-4 ;rect bottom
		y1 = y0 + linespacing-4 ;top
		polyfill, [[0,y0],[0,y1],[x1,y1],[x1,y0]],/dev ;Invert new
	  endif	;i ne old choice
	endif		;Y ge 0
endrep until !err ne 0

device,set_graphics=old_gr	;Restore old graphic fcn
if !d.name eq "X" then device, bypass_translation=0
!p.color = pcolor
!p.background = bcolor
wdelete
return,n-1-old_choice
end

