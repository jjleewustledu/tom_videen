;
; Conventions:
; Faces = faces of cube.  Numbered 0 to 5:
;		0 = X = c0(0)  (Corner 0)
;		1 = Y = c0(1)
;		2 = Z = c0(2)
;		3 = X = c1(0)  (Corner 1)
;		4 = Y = c1(1)
;		5 = Z = c1(2)
; Orientations:
;	6 = reset to default.
;	0 = xy exchange, 1 = xz exchange, 2 = yz exchange.
;	3 = x reverse, 4 = y reverse, 5 = z reverse.
; Vertex indices:	Faces
;	0	0,0,0   0,1,2
;	1	N,0,0	1,2,3
;	2	0,N,0	0,2,4
;	3	N,N,0	2,3,4
;	4	0,0,N	0,1,5
;	5	N,0,N	1,3,5
;	6	0,N,N	0,4,5
;	7	N,N,N	3,4,5
;
; Modes:
;	0 = Slices
;	1 = Cube
;	2 = Cut
;	3 = Isosurface
;	4 = Probe
;	5 = rotations


function p_inside_poly, poly, p
;	poly = (2,n) array of polygon vertices, either clockwise or
;		counter-clockwise order.
;	p = (2) point coordinates.
;	return 0 if point is outside polygon, 1 if inside.
;
x = float(p(0))
y = float(p(1))
np = n_elements(poly)/2		;# of vertices
x1 = poly(0,np-1)		;Start at last point
y1 = poly(1,np-1)

pos = [0,0]
for i=0,np-1 do BEGIN
	x2 = poly(0,i)
	y2 = poly(1,i)
	k = (x*(y1-y2) + y * (x2-x1) + x1*y2-y1*x2)  ;Side of line point is on
	if k eq 0 THEN BEGIN	;On line?
	    if (y lt (y1< y2)) or (y gt (y1 > y2)) or  $  ;Check more
		(x lt (x1 < x2)) or (x gt (x1 > x2)) THEN return,0
	ENDIF ELSE BEGIN		;Not on line
		k = k gt 0		;1 if on right side
		if pos(1-k) ne 0 THEN return,0
		pos(k) = 1
	ENDELSE
	x1=x2
	y1=y2
	ENDFOR
return,1
end



PRO SLICER_PLAYBACK, FILE = file, Commands
; Play back a journal.  Commands are either in the designated file
; or in the string array Commands".
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

if n_elements(file) gt 0 THEN BEGIN	;Read from file
	OPENR, unit, /GET_LUN, file, ERROR = i
	if i ne 0 then begin	;OK?
		widget_control, sl.file_text(1), set_value = !ERR_STRING
		return
		ENDIF
	commands = strarr(100)		;Read up to 100 lines
	on_ioerror, eof
	readf, unit, commands
eof:	i = fstat(unit)			;Get transfer count
	ncommands = i.transfer_count
	free_lun, unit
ENDIF ELSE ncommands = n_elements(commands)

IF mode ne 7 THEN BEGIN			;Not our mode?
	if sl.mode_bases(mode) ne 0 THEN $  ;Remove panel if mapped
		WIDGET_CONTROL, sl.mode_bases(mode), MAP=0
	mode = 7			;New mode = ours
	WIDGET_CONTROL, sl.mode_bases(mode), MAP=1
	ENDIF

pars = fltarr(10)

for i=0, ncommands-1 do begin		;Each command
	s = strtrim(strcompress(commands(i)))	;Parse it, extracting fields
	j = 0
	m = 0
	while j lt strlen(s) do begin	;While string left
		k = strpos(s, ' ', j)	;Find next blank
		if k le 0 then k = strlen(s)  ;if none, go to end of string
		if j eq 0 then cmd = strmid(s,0,k) $
		else begin pars(m) = strmid(s,j,k) & m=m+1 & endelse
		j = k+1
		endwhile
	WIDGET_CONTROL, sl.file_text(1), SET_VALUE = strmid(s,0,32)
	case strupcase(cmd) of		;Interpret commands.....
"UNDO": slicer_undo
"ORI":  BEGIN    ;AXIS(3), AXIS_REVERSE(3), ROTATIONS(2)
	sl.axex = pars(0:2)
	sl.axrev = pars(3:5)
	sl.rotation = pars(6:7)
	for j=0,1 do WIDGET_CONTROL, sl.rslide(j), SET_VALUE = sl.rotation(j)
	WIDGET_CONTROL, sl.rslide(2), SET_VALUE=string(pars(8))  ;Aspect
	SLICER_ORIENTATION
	ENDCASE
"TRANS": BEGIN   ;On/Off Threshold(%)
	sl.trans = pars(0)
	sl.threshold = pars(1) /100. * sl.nc3
	WIDGET_CONTROL, sl.threshold_base(3), SET_VALUE = pars(1)
	WIDGET_CONTROL, sl.threshold_base(1+sl.trans), SET_BUTTON=1
	WIDGET_CONTROL, sl.threshold_base(2-sl.trans), SET_BUTTON=0
	WIDGET_CONTROL, sl.threshold_base(0), MAP=sl.trans
	ENDCASE
"SLICE": BEGIN	  ;Axis, slice_value, interp
	sl.interp = pars(2)
	SLICER_DRAW, pars(0), pars(1)
	ENDCASE
"COLOR": BEGIN	 ;Table_num (-1 = present), low, high, shading
	sl.stretch = pars(1:2)
	sl.shading = pars(3)/100.
	for j=0,2 do WIDGET_CONTROL, sl.cslide(j), SET_VALUE = pars(j+1)
	slicer_colors, 	pars(0)
	ENDCASE
"ISO":  BEGIN		;value, hi/lo
	sl.isop.value = pars(0) / 100. * (sl.amax-sl.amin) + sl.amin
	sl.isop.hi_lo = pars(1)
	DO_ISOSURFACE
	ENDCASE
"ERASE":  slicer_erase
"SAVEPS" : slicer_postscript
"WAIT":  wait, pars(0)
"CUBE": BEGIN		;mode (1 = block, 0 = cutout), cut_ovr, interp,
			; start_coords(3), end_coords(3)
	mode = pars(0)
	sl.cut_ovr = pars(1)
	sl.interp = pars(2)
	sl.p0cube = reform(pars(3:8), 3,2)
	DO_CUBE	
	ENDCASE
    ENDCASE
  ENDFOR
mode=7
WIDGET_CONTROL, sl.file_text(1), SET_VALUE = 'Playback Done'
end

; Journal events if journal file is open.
PRO SLICER_JOURNAL, name, params
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

if sl.journal le 0 then return
if n_elements(params) le 0 then params = 0.
printf, sl.journal, name, params, format='(A, 1x, 10F10.3)'
end


PRO SLICER_UNDO		;Undo last operation
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

SLICER_JOURNAL, "UNDO"
if n_elements(zb_last) le 1 then return
set_plot,'Z'
tmp = tvrd(/WORDS, CHANNEL=1)  ;Read depth buffer & swap
tv, zb_last, CHANNEL=1, /WORDS
zb_last = temporary(tmp)
tmp = tvrd()		;Swap them
tv, z_last
slicer_show, z_last
z_last = tmp
end


PRO slicer_orientation, i	;i = orientation
;  Set the New Orientation
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed
if n_elements(i) gt 0 THEN BEGIN
	if i le 2 THEN BEGIN
		j = 2 * i
		ll = ([0,1,0,2,1,2])(j:j+1)  ;axes to swap
		t = sl.axex(ll(0)) & sl.axex(ll(0)) = sl.axex(ll(1)) & 
		sl.axex(ll(1)) = t
	ENDIF ELSE if i eq 6 THEN BEGIN
		sl.axex = [0,1,2]		;default transformation
		sl.axrev = intarr(3)
	ENDIF ELSE sl.axrev(i-3) = 1-sl.axrev(i-3)  ;reverse
ENDIF		

d = [ 0., dims(0), 0., dims(1), 0., dims(2)]
f = 1.0
WIDGET_CONTROL, sl.rslide(2), GET_VALUE=s
ON_IOERROR, bad_aspect
f = (float(s))(0)
if (f le 0.0) then goto, bad_aspect
IF f gt 1. THEN BEGIN
	x = (f-1)/2.
	d(0) = [-x * dims(0), (x+1) * dims(0), -x * dims(1), (x+1) * dims(1)]
ENDIF ELSE BEGIN
	x = (1-f)/2
	d(4) = [-x * dims(2), (x+1) * dims(2)]
ENDELSE
bad_aspect:
SLICER_JOURNAL, "ORI", [sl.axex, sl.axrev, sl.rotation, f ]

for i=0,2 do if sl.axrev(i) THEN BEGIN	;Swap endpoints for reversed axes
	j=i*2
	t = d(j) & d(j) = d(j+1) & d(j+1) = t
	ENDIF

!x.type = 0		;make sure its linear
scale3, xrange=d(0:1), yrange=d(2:3), zrange=d(4:5), ax = sl.rotation(0), $
	az = sl.rotation(1)

k = 1		;current y axis
if sl.axex(0) ne 0 THEN BEGIN	;swap x?
	if sl.axex(0) eq 1 THEN BEGIN &	t3d, /XYEXCH & k = 0
	ENDIF ELSE t3d,/XZEXCH
ENDIF

if k ne sl.axex(1) THEN t3d,/YZEXCH
slicer_erase
if mode le 2 THEN draw_orientation
end



PRO slicer_draw, ax, slice		;draw a slice.
;  ax = axis, 0 for x, 1 for Y, 2 for Z. slice = plane number.
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

SLICER_JOURNAL, "TRANS", [sl.trans, sl.threshold * 100. / sl.nc3]
SLICER_JOURNAL, "SLICE", [ax, slice, sl.interp]
s = replicate(slice,4)
set_plot,'Z'
z_last = tvrd()			;save previous image & z
zb_last = tvrd(CHANNEL=1, /WORDS)

d0 = [0,0,0]
d1 = dims -1
d0(ax) = slice
d1(ax) = slice

;	extract & scale the slice
offset = byte(ax*sl.nc3)	;bias for this slice
p = bytscl(a(d0(0):d1(0), d0(1):d1(1), d0(2):d1(2)), $
	max=sl.amax, min=sl.amin, top = sl.nc3-1)

if sl.trans THEN t = (sl.threshold > 1) + offset $  ;lower limit
ELSE t = 0
if ax ne 0 THEN p = p + offset	;add bias

case ax of
0:	polyfill, s, [0,dims(1)-1,dims(1)-1,0],[0,0,dims(2)-1,dims(2)-1],/T3D,$
		pat=reform(p, dims(1), dims(2), /OVER), $
		image_coord = [0,0, dims(1),0, dims(1),dims(2), 0,dims(2)], $
		image_interp= sl.interp, trans=t
1:	polyfill, [0,dims(0)-1,dims(0)-1,0],s,[0,0,dims(2)-1,dims(2)-1],/T3D,$
		pat=reform(p, dims(0), dims(2), /OVER), $
		image_coord = [0,0, dims(0),0, dims(0),dims(2), 0,dims(2)], $
		image_interp= sl.interp, trans=t
2:	polyfill,[0,dims(0)-1,dims(0)-1,0],[0,0,dims(1)-1,dims(1)-1],s,/T3D,$
		pat=reform(p, dims(0), dims(1), /OVER),$
		image_coord = [0,0, dims(0),0, dims(0),dims(1), 0,dims(1)], $
		image_interp= sl.interp, trans=t
ENDCASE
slicer_show
end

PRO slicer_colors, table	;load our color table
; Table = index of color table to load, -1 to retain present.
; The color palette is repeated 3 times, once for each of the possible face
; directions.  The colors indices:
;  3 * sl.nc3  = red
;  3 * sl.nc3 + 1 = green
;  3 * sl.nc3 + 2 = blue
;  3 * sl.nc3 + 3 = white

COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed
common colors, r_orig, g_orig, b_orig, r_curr, g_curr, b_curr

v = [0, .5, 1.]		;Shading of the planes.
if n_params() eq 0 then table = -1
if table ge 0 then loadct, /SILENT, table
SLICER_JOURNAL, "COLOR", [table, sl.stretch, 100. * sl.shading]

nc1 = !d.n_colors-1
if sl.stretch(0) eq sl.stretch(1) then $
	q = lindgen(sl.nc3) * nc1 / (sl.nc3-1) $
else BEGIN		;Use contrast max and min
	s = 100./(sl.stretch(1)-sl.stretch(0))
	int = -s * sl.stretch(0)/100.
	q = long(3*(findgen(sl.nc3) * s + int))
ENDELSE

r_curr = bytarr(3*sl.nc3)
g_curr = r_curr
b_curr = r_curr
s = 1.-sl.shading
for i=0,2 do begin		;3 faces
	v0 = sl.shading * v(i) * 255
	r_curr(i*sl.nc3) = s * r_orig(q) + v0
	g_curr(i*sl.nc3) = s * g_orig(q) + v0
	b_curr(i*sl.nc3) = s * b_orig(q) + v0
	endfor
tvlct, r_curr, g_curr, b_curr
	; load last 4 colors as red, green, blue, white
tvlct, [255,0,0,255],[0,255,0,255],[0,0,255,255],3*sl.nc3
end

PRO DO_ISOSURFACE		;Draw the isosurface
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed


SLICER_JOURNAL, "ISO", [(sl.isop.value-sl.amin)/(sl.amax-sl.amin)*100, $
		sl.isop.hi_lo]
set_plot,'Z'
widget_control, sl.pos_text, set_value='Computing Polygons'
shade_volume, a, sl.isop.value, verts, polys, $
	low = sl.isop.hi_lo
if n_elements(verts) eq 0 then begin
	widget_control, sl.pos_text, set_value = $
	'No surface at this value'
	set_plot,'X'
endif else begin
  widget_control, sl.pos_text, set_value = $
	strtrim((size(verts))(2),2)+' Vertices, ' + $
	strtrim((size(polys))(1)/4,2) + ' Polygons.'
  z_last = tvrd()			;Save old display
  zb_last = tvrd(CHANNEL=1, /WORDS)
  b = polyshade(verts,polys,/T3D, TOP = sl.nc3-1)
  verts = 0 & polys = 0		;Free space
  slicer_show
endelse
end



PRO draw_cube, c0, c1, faces, color
;	draw a cube whose opposite corners are [c0(0),c0(1),c0(2)],
;	and [c1(0), c1(1), p2(3)].
;	color = drawing color.
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed
p0 = intarr(3,8)
p1 = float(p0)
cc = [[c0],[c1]]
for i=0,7 do BEGIN
	p0(0,i) = [ cc(0, i and 1), cc(1, (i/2 and 1)), cc(2, (i/4 and 1))]
	p1(0,i) = convert_coord(p0(*,i), /T3D,/TO_DEVICE,/DATA)
	ENDFOR

if n_elements(color) le 0 THEN color = !d.n_colors-1
f = sl.facevs
flags = bytarr(8,8)	;line flags, dont draw same line twice
for i=0,n_elements(faces)-1 do BEGIN
    ff = [ f(*,faces(i)), f(0,faces(i))]  ;Vertex indices
    for j=0,3 do begin
	k = ff(j) < ff(j+1) & l = ff(j) > ff(j+1)
	if not flags(k,l) then plots, p1(*,[k,l]), color = color, /dev
	flags(k,l) = 1
	ENDFOR
    ENDFOR
end
 
PRO slicer_erase		;draw the background
;	call with no params to erase all.
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

SLICER_JOURNAL, "ERASE"
set_plot,'Z'
erase
sl.p1 = convert_coord(sl.p0, /T3D, /TO_DEVICE, /DATA)  ;save dev coords
s = strarr(8)
for i=0,7 do BEGIN
	s(i) = string(sl.p0(*,i),format ="(' (',i0,',',i0,',',i0,')')")
	ENDFOR
junk = max(sl.p1(2,*), j)
sl.v_close = j			;index of closest vertex
p = where(sl.facevs eq sl.v_close)/4   ;indices of closest verts
colors = [ !d.n_colors-1, sl.nc3*3+1]  ;white, green
for i=0,5 do BEGIN		;draw faces
	k= (where(p eq i))(0) lt 0 	;1 = not close, 0 = close
	draw_cube, [0,0,0], dims-1, i,colors(k)
	ENDFOR
for i=0,7 do xyouts, sl.p1(0,i),sl.p1(1,i),/DEVICE,s(i), $
	color=colors(i ne sl.v_close)
z_last = tvrd()
zb_last = tvrd(CHANNEL=1, /WORDS)
slicer_show
end

PRO slicer_postscript
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

wset, sl.window
d=tvrd(0,0,!d.x_size,!d.y_size)
set_plot,'PS'
device,/color,BITS_PER_PIXEL=8,filename='slicer.ps'
d(where(d eq 0B)) = 255B			; first (normally black) to last color (white?)
tv,d
device,/close
set_plot,getenv('IMSLIDL_DEVICE')
end


PRO slicer_show, image
;move the Z buffer to the X display.  leave device set to X.
; if parameter is present, show it rather than reading the Z buffer
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

if n_params() eq 0 then begin
	set_plot,'Z'
	image = tvrd()
	endif
set_plot,'X'
wset, sl.window
tv, image
sl.cube_on = 0
end

PRO draw_orientation	;draw the orientation cube in the small window
; Draw the outline of the 3 frontmost faces of the main cube.
; Draw the fixed axis plane in green.
; If the mode is cut or cube, draw the selected cube.  Draw the back faces
;	in blue, and label the selection points.

COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

i = sl.mode_bases(mode)
widget_control, i, get_uvalue = draw		;the widget id
widget_control, draw, get_value=window	;the window number
wset, window
erase

if mode le 0 then begin
	z0 = [0,0,0]
	d1 = dims-1
  endif else begin
	z0 = sl.p0cube(*,0) < sl.p0cube(*,1)
	d1 = sl.p0cube(*,0) > sl.p0cube(*,1)
  endelse
; draw fixed plane:
p = z0
nlines = 6
d = (d1-z0) / float(nlines-1)
p(fixed) = (d1(fixed) + z0(fixed))/2.
d(fixed) = 0.0
k = 3 * sl.nc3 + 1
for i=0,nlines-1 do BEGIN		;draw fixed direction
	xx = replicate(p(0),2)
	yy = replicate(p(1),2)
	zz = replicate(p(2),2)
	if fixed ne 0 THEN plots, [z0(0),d1(0)], yy,zz, /T3D, COLOR=k, /DATA
	if fixed ne 1 THEN plots, xx, [z0(1),d1(1)],zz, /T3D, COLOR=k, /DATA
	if fixed ne 2 THEN plots, xx, yy, [z0(2),d1(2)],/T3D, COLOR=k, /DATA
	p = p + d
	ENDFOR

if mode ne 0 then begin		;Do cube
	draw_cube, sl.p0cube(*,0), sl.p0cube(*,1), indgen(6), k+1  ;all faces
		;Close faces in white
	draw_cube, sl.p0cube(*,0), sl.p0cube(*,1), $
			where(sl.facevs eq sl.v_close)/4
	for i=0,1 do begin		;Label them
		p = convert_coord(sl.p0cube(*,i), /T3D, /TO_DEV, /DATA)
		xyouts, p(0), p(1), strtrim(i,2), /device
		endfor
	endif
draw_cube, [0,0,0], dims-1, where(sl.facevs eq sl.v_close)/4  ;draw 3 close faces
wset, sl.window
return
end


PRO mark_slice		;mark a horizontal or vertical slice

COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

;		loop until the mouse is released.

first = 1	;first time
d = ''
f = where(sl.facevs eq sl.v_close)/4  ;3 faces to check that are front
buttons = 1

again:
widget_control, sl.pos_text, set_value = d  ;label it
d = 'no data value'			;assume no data


if buttons eq 0 THEN BEGIN		;done?
done:	device,set_graphics=3		;restore normal graphics mode
	return
	ENDIF
cursor, /DEV, x, y, /NOWAIT
if x lt 0 then goto, done
buttons = !err
p = float([x, y])			;device
face = -1				;assume no face
for i=0,2 do BEGIN			;find the face
    j = f(i)				;face index
    p1 = sl.p1(0:1, sl.facevs(*,j))	;vertices
    if p_inside_poly(p1, p) THEN face = j
    ENDFOR

if first THEN BEGIN			;erase last one?
    first = 0
    device, set_graph =6 		;set XOR mode
    on = 0
ENDIF ELSE BEGIN
    if on THEN draw_cube, z1, d1, fixed, 255
    on = 0
ENDELSE

if face lt 0 THEN goto, again	; not on one of our faces

p = p / [ !d.x_size, !d.y_size] ;normalized
face_dim = [ 0, 1, 2, 0, 1, 2]
if face le 2 THEN k = 0 ELSE k = dims(face_dim(face))-1
p = COORD2to3(p(0), p(1), face_dim(face), k, junk)
ip = fix(p + 0.5)		;round
d = 'Position: ' + string(ip, $
	format="('(',i4,',',i4,',',i4,')')")
k = ip(fixed)			;slice number

if buttons ne 0 THEN BEGIN
	d1 = dims-1 & z1 = [0,0,0]
	d1(fixed) = k & z1(fixed) = k
	draw_cube, z1, d1, fixed, 255
	on = 1
ENDIF ELSE BEGIN
	device, set_graph=3    ;normal mode
	if (k ge 0) and (k lt dims(fixed)) THEN $
		slicer_draw, fixed, k
ENDELSE
goto, again
end


PRO do_cube				;Draw a cube or cut
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

SET_PLOT, 'Z'
z =  (z_last = tvrd())			;Save previous image & current cont
zb = (zb_last = tvrd(CHANNEL=1, /WORDS))


SLICER_JOURNAL, "TRANS", [sl.trans, sl.threshold * 100. / sl.nc3]
SLICER_JOURNAL, "CUBE", [ mode, sl.cut_ovr, sl.interp, reform(sl.p0cube, 6) ]

d0 = sl.p0cube(*,0) < sl.p0cube(*,1)		;Lower corner
d1 =  (sl.p0cube(*,0) > sl.p0cube(*,1)) -d0
coords = fltarr(3,8)
for i=0,7 do coords(0,i) = $		;Verts of our cube
   [ (i and 1) * d1(0), (i and 2)/2 *d1(1), (i and 4)/4 * d1(2)] + d0
faces = where(sl.facevs eq sl.v_close)/4   ;close faces
if mode eq 1 then relation = 1 else relation = 0

FOR face = 0, 5 do $			;Draw the faces of the cube or cut
  IF ((where(face eq faces))(0) ge 0) eq relation then begin  ;Do this face?
	v = 0 & ones = 0 & z0 = 0 & q = 0	;Clear things out
	erase					;Reset Z buffer
	verts = sl.facevs(*,face)		;Vertices
	polyfill, coords(*, verts), /T3D	;Draw polygon for face
	z0 = tvrd(CHANNEL=1, /WORDS)		;Now read the Z buffer
	if mode eq 1 then $			;Cube? or Cut?
		points = where(z0 gt zb) $	;New points for cube
	else if sl.cut_ovr then points = where(z0 ne z0(0,0)) $ ;Cut mode?
	else points = where((z0 ne z0(0,0) and (zb gt z0)))  ;Over mode

	widget_control, sl.pos_text, set_value = $
		'Creating face ' + strtrim(face,2)
	if points(0) ne -1 THEN	BEGIN		;Anything to do?
	  ones = replicate(1, n_elements(points))
		;Make 3D homogen array in  data array subscripts coords
	  v = [ [(points mod !d.x_size) / float(!d.x_size)], $ ;Norm X
		[(points / !d.x_size)   / float(!d.y_size)], $ ;Norm Y
		[ z(points)/65530. + 0.5], $		;Normalized Z coords
		[ ones ]]				;Homogeneous coord
		;Get subscripts in data cube (data coords)
	  v = ((v # invert(!p.t)) - $
			(ones # [!x.s(0), !y.s(0), !z.s(0)])) / $
			(ones # [ !x.s(1), !y.s(1), !z.s(1)])
;		Either interpolate or pick nearest neighbor
	  widget_control, sl.pos_text, set_value = $
		(['Sampling','Interpolating'])(sl.interp) + $
		' face ' + strtrim(face,2) + ',  ' + $
		strtrim(n_elements(v)/3,2) + ' Pixels'

	  if sl.interp THEN v = interpolate(a, v(*,0), v(*,1), v(*,2)) $
	  else v = a(long(temporary(v)) # [1, dims(0), dims(0) * dims(1)])
					;Update our points
	  offset = byte((face mod 3) * sl.nc3)   ;Offset
	  if sl.trans THEN BEGIN	;Transparency?
		good = where(v ge sl.threshold)
		v = v(good)
		points = points(good)
		endif
	  q = bytscl(v, max=sl.amax, min=sl.amin, top = sl.nc3-1) ;face data
				   ;Get subscripts in data cube
	  if offset ne 0 then q = q + offset
	  z(points) = q			;Store the new face
	  zb(points) = z0(points)	;in both buffers
	ENDIF				;Anything to do
  ENDIF					;Each face
tv, z					;Now show it
tv, zb, /WORDS, CHANNEL=1		;and update depth buffer
v = 0 & ones = 0 & z0 = 0 & q = 0	;Clear things out
z = 0 & zb = 0
widget_control, sl.pos_text, set_value = 'Done.'
slicer_show
end




PRO mark_cube1, p0, ip	;Draw the outline of the cube in the main drawable
; p0 = cube coordinates (3,2).
; ip = index of corner that is marked (0 or 1).
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

draw_cube, p0(*,0),p0(*,1), indgen(6), 255   ;Basic cube
p1 = [[p0(*,ip)], [p0(*,ip)]]
p = sl.p0(*,sl.v_close)
for i=0,2 do begin		;Lines to faces
	p1(i,0) = p(i)
	plots, p1, color=128, /T3D
	p1(i,0) = p1(i,1)
	endfor
end



PRO mark_cube, ev			;Mark a cube in the main window
;  Use the XOR graphics mode to avoid killing the display
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

;		loop until the mouse is released.
device, SET_GRAPHICS = 6	;Set XOR mode

d1 = dims -1

if n_params() eq 0 then	begin	;Erase it?
	if sl.cube_on then mark_cube1, sl.p0cube, sl.cube_ip
	sl.cube_on = 0
	device, SET_GRAPHICS=3
	return
	ENDIF

repeat begin
	cursor, x, y, /nowait, /norm		;Read the cursor
	if !err eq 0 then begin			;No buttons?
		device, SET_GRAPHICS=3		;Reset Normal writing mode
		draw_orientation
		return
		ENDIF
	if sl.cube_on then mark_cube1, sl.p0cube, sl.cube_ip  ;Erase prev
	sl.cube_ip = ev.press eq 2		;Corner index
	sl.cube_on = 1
	q = sl.p0cube(fixed, sl.cube_ip)	;Fixed axis/point
	p = fix(COORD2TO3(x, y, fixed, q, pti)+0.5) ;3D coords
	sl.p0cube(*,sl.cube_ip) = p < d1 > 0	;New corner value
	sl.p0cube(fixed, sl.cube_ip) = q
	mark_cube1, sl.p0cube, sl.cube_ip
	d = string(sl.p0cube, format="('(', 3i4, ') (', 3i4, ')')")
	widget_control, sl.pos_text, set_value = d  ;label it
	ENDREP UNTIL 0
end

PRO slicer_event, ev
COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed

wset, sl.window		;Our window
if ev.id eq sl.draw THEN BEGIN		;mouse press?
  if ev.press eq 0 THEN return	;ignore releases
  IF (mode le 2) and ((ev.press and 4) ne 0) THEN BEGIN
		fixed = (fixed + 1) mod 3	;bump plane
		draw_orientation
		return
		ENDIF	
  if mode eq 0 then BEGIN
	if ev.press and 1 then mark_slice $	;mark the slice
	else goto, probe_it	;SLice mode, middle button = probe
  ENDIF ELSE if mode le 2 then mark_cube, ev $	;Cubes?
  ELSE BEGIN			;Other modes
  probe_it: if (n_elements(zb_last) le 1) THEN return
    set_plot,'Z'
			 ;unsigned shorts in zb
    z = float(tvrd(ev.x, ev.y, /WORD, /CHANNEL))/65530. + .5
    set_plot,'X'
    p = [ ev.x, ev.y, z(0)] / [!d.x_size, !d.y_size, 1.] ;normalized coords
    d = 'No Data Value'
    if p(2) gt .01 THEN BEGIN	;anything there?
	  p = [p,1] # invert(!p.t)	;inverse transform to normalized
	  p = (p - [!x.s(0),!y.s(0),!z.s(0)])/[!x.s(1),!y.s(1),!z.s(1)]
	  p = fix(p + 0.5)		;round
	  p = p > 0 < (dims-1)		;to range
	  x = a(p(0), p(1), p(2))+0
	  y = fix(100.*(x - sl.amin)/(sl.amax - sl.amin))  ;To %
	  d = 'Position: '+string(p,format="('(',i0,',',i0,',',i0,')')") + $
		', Data= ' + strtrim(x,2) + ' (' + strtrim(y,2) + '%)'
	ENDIF			;Something there
	widget_control, sl.pos_text, set_value = d
  ENDELSE
  return
ENDIF			;Drawable window

if ev.id eq sl.isop.drawable then begin	;Isosurface threshold
	if ev.press eq 0 then return
	x = (ev.x - sl.isop.xs(0)) / sl.isop.xs(1)
	x = x > sl.amin < sl.amax
	WIDGET_CONTROL, sl.isop.slider, $
	   SET_VALUE = 100.*(x - sl.amin) / (sl.amax-sl.amin)
	sl.isop.value = x
	return
	ENDIF
if ev.id eq sl.file_text(0) then return	;Ignore return in file name widget
if ev.id eq sl.rslide(2) then BEGIN
	slicer_orientation
	return
	ENDIF
;		here, it must be a button or a slider:
widget_control, ev.id, get_uvalue = eventval
case eventval of
"CANCUBE" : mark_cube		;Undraw outline
"COLORS" : slicer_colors, (where(sl.color_button eq ev.id))(0)
"CUTINTO": sl.cut_ovr = 0
"CUTOVER": sl.cut_ovr = 1
"SAVEPS": slicer_postscript
"ERASE"  : slicer_erase
"EXIT"   : BEGIN
	widget_control, ev.top, /DESTROY
	z_last = 0
	zb_last = 0
	goto, close_journal
	ENDCASE
"GOCUBE" : do_cube
"HELP" : BEGIN
	xdisplayfile, filepath("slicer_demo.txt", subdir='help'), $
		title = "slicer demo help", $
		group = ev.top, $
		width = 72, height = 24
	return
	ENDCASE

"INTON": sl.interp = 1
"INTOFF" : sl.interp = 0

"ORIENTATION": SLICER_ORIENTATION, (where(sl.ori_butt eq ev.id))(0)
"PLAYBACK" : BEGIN
	WIDGET_CONTROL, sl.file_text(0), GET_VALUE=name
	name = strtrim(name(0),2)
	SLICER_PLAYBACK, FILE = name
	ENDCASE
"RECORD" : BEGIN
start_journal: if sl.journal ne 0 then free_lun, sl.journal  ;Close old
	sl.journal = 0
	WIDGET_CONTROL, sl.file_text(0), GET_VALUE=name
	name = strtrim(name(0))
	openw, i, name, ERROR=j, /GET_LUN
	if j ne 0 then begin		;OK?
		widget_control, sl.file_text(1), set_value = !ERR_STRING
		return
		ENDIF
	widget_control, sl.file_text(1), set_value = 'Journal Active'
	sl.journal = i
	ENDCASE
"RECORDOFF" : BEGIN
	widget_control, sl.file_text(1), set_value = 'Journal Closed'
close_journal: if sl.journal ne 0 then free_lun, sl.journal  ;Close old
	sl.journal = 0
	ENDCASE
"THRESHOLD" : sl.threshold = sl.nc3 * ev.value / 100.
"TRANS": BEGIN
	sl.trans = ev.id eq sl.threshold_base(2)  ;On or Off?
	widget_control, sl.threshold_base(0), MAP=sl.trans
	ENDCASE
"HIGH": sl.isop.hi_lo = 0
"LOW" : sl.isop.hi_lo = 1
"ISOSLIDE" : sl.isop.value = (ev.value / 100.)*(sl.amax-sl.amin) + sl.amin
"GO":   do_isosurface
"SHADING": BEGIN
	sl.shading = ev.value /100.
	slicer_colors
	ENDCASE
"STMAX": BEGIN
	sl.stretch(1) = ev.value
	slicer_colors
	ENDCASE
"STMIN": BEGIN
	sl.stretch(0) = ev.value
	slicer_colors
	ENDCASE
"UNDO" : slicer_undo
"XROTATION": BEGIN
	sl.rotation(0) = ev.value
	slicer_orientation
	ENDCASE
"ZROTATION": BEGIN
	sl.rotation(1) = ev.value
	slicer_orientation
	ENDCASE

ELSE :   BEGIN			;mode button?
	k = where(eventval eq sl.mode_names, count)  ;Match with mode name?
	if count eq 1 THEN BEGIN		;switch mode
		if sl.cube_on then mark_cube	;Remove the cube if vis
		fixed = 0			;Reset fixed direction
		if sl.mode_bases(mode) ne 0 THEN $  ;Remove panel if mapped
			WIDGET_CONTROL, sl.mode_bases(mode), MAP=0
		mode = k(0)			;New mode
		if sl.mode_bases(mode) ne 0 THEN $  ;Map new base
			WIDGET_CONTROL, sl.mode_bases(mode), MAP=1
		if mode le 2 THEN BEGIN		;Slice or cube?
			draw_orientation
			ENDIF
		if mode eq 3 THEN BEGIN		;Draw histogram
			WSET, sl.isop.window
			type = size(a)
			int = type(type(0) + 1) le 3  ;True if int type
			j = (sl.amax -sl.amin)/100. ;bin size
			if int then j = j > 1
			h = histogram(a, max=sl.amax, min = sl.amin, bin=j)
			if int THEN j = fix(j + .99)
			k = sort(h)
			n = n_elements(h)
			x = findgen(n) * j + sl.amin < sl.amax
			xsave = !x.s & ysave = !y.s
			PLOT,x,h, xst = 8, yst=8, ymargin=[2,0], $
				yrange= [0,h(k(n-8))], yticks=4, chars=.75, $
				xticks=4
			sl.isop.xs = !x.s * !d.x_size
			WSET, sl.window
			!x.s = xsave & !y.s = ysave
			ENDIF			;Isosurface
		RETURN
	ENDIF
	print,'Unknown event: ', eventval
	help, /STRUCT, ev
	ENDCASE
ENDCASE
end

PRO slicer, GROUP = group, RANGE = range, COMMANDS = commands, $
	CMD_FILE = cmd_file
;+
; NAME:
;	SLICER
;
; PURPOSE:
;	Widget based application to show 3D volume slices and isosurfaces.
;
; CATEGORY:
;	Volume display / rendering.
;
; CALLING SEQUENCE:
;	COMMON VOLUME_DATA, A
;	A = your_volume_data
;	SLICER
;
; INPUTS:
;	Variable A in VOLUME_DATA common contains volume data.  See EXAMPLE
;	section below.
;
; KEYWORD PARAMETERS:
;     COMMANDS:	An optional string array of commands to execute
;		before entering the interactive mode.  Commands are
;		in the form of a keyword optionally followed one or more 
;		numeric, blank-separated parameters.  For example:
;			"COMMAND P1 P2 P3 ... Pn"
;		Keywords and parameters are:
;		UNDO:	Undo previous operation.
;		ORI X_Axis Y_Axis Z_axis X_Rev Y_Rev Z_Rev X_Rot Z_Rot Asp
;			This command sets the orientation for the SLICER 
;			display.  X_Axis, Y_Axis, and Z_Axis should be 0 for 
;			data x, 1 for data y, and 2 for data z.  
;			X_Rev, Y_Rev, and Z_Rev should be 0 for normal, 1 for 
;			reversed.  Asp is the Z axis aspect ratio w/ respect 
;			to X, Y.  X_Rot and Z_Rot are the rotations of the 
;			X and Z axes in degrees (30 is the default).
;			For example, to interchange the X and Z axes and
;			reverse the Y use the string:
;				ORI 2 1 0 0 1 0 30 30
;		TRANS On_Off Threshold:  Use this command to turn transparency 
;			on or off and set the transparency threshold value.
;			1 means on, 0 means off.  Threshold is expressed in 
;			percent of data range (0 = min data value, 100 = max 
;			data value).
;		SLICE Axis Value Interp:  Draw a slice along the given
;			axis (0=x, 1=y, 2=z) at Value.  Set Interp
;			equal to 1 for interpolation, 0 for nearest neighbor.
;		COLOR Table_Index Low High Shading:  Set the color tables.
;			Table_Index is the pre-defined color table number (see
;			LOADCT), or -1 to retain the present table.  Low, High
;			and Shading are expressed in percent.
;		ISO Threshold Hi_Lo:  Draw an iso-surface.  Threshold is the 
;			isosurface threshold value.  Hi_Lo should be set to 1
;			to view the low side, 0 for the high side.
;		ERASE:	Erase the display.
;		CUBE Mode Cut_Ovr Interp X0 Y0 Z0 X1 Y1 Z1:  Draw cube 
;			(mode = 1) or cut-out (mode = 0).
;			Cut_Ovr should be set to  1 for cut-over, 0 for 
;			cut-thru.  Interp should be 1 for interpolation, 0 
;			for nearest neighbor.  (X0,Y0,Z0) is the lower corner 
;			of the cube.  (X1,Y1,Z1) is the upper corner. 
;			(X0 < X1, etc.)
;		WAIT Secs:  Wait the designated time (in seconds).
;
;		Postscript option added .... CM at UCL-Belgium
;
;     CMD_FILE:	A string that contains the name of a file containing SLICER
;		commands to execute as described above.
;
;	GROUP:	The base ID of the widget that calls SLICER.  When this 
;		keyword is specified, the death of the caller results in the
;		death of the SLICER.
;
;	RANGE:	A two-element array containing minimum and maximum data
;		values of interest.  If this keyword is omitted, the data is 
;		scanned for the minimum and maximum.
;
; OUTPUTS:
;	No explicit outputs.
;
; COMMON BLOCKS:
;	COMMON VOLUME_DATA, A   ;Used to pass in the volume data.
;	COMMON SLICER_COMMMON   ;Used internally.
;
; SIDE EFFECTS:
;	Widgets are created on the X window display.
;
; RESTRICTIONS:
;	Widgets are required.
;	The volume data must fit in memory.
;
; PROCEDURE:
;	The slicer program has the following modes:
;	Slices:         Displays orthogonal slices thru the data volume.
;	Block:          Displaces the surfaces of a selected block inside
;	                the volume. 
;	Cutout:         Cuts blocks from previously drawn objects.
;	Isosurface:     Draws an isosurface contour.
;	Probe:          Displays the position and value of objects
;	                using the mouse.
;	Colors:         Manipulates the color tables and contrast.
;	Rotations:      Sets the orientation of the display.
;
; EXAMPLE:
;	Data is transferred to the SLICER via the VOLUME_DATA common block
;	instead of as an argument.  This technique is used because volume
;	datasets can be very large and hence, the duplication that occurs when
;	passing values as arguments is a waste of memory.  Suppose that you 
;	want to read some data from the file "head.dat" into IMSL/IDL for use
;	in the SLICER.  Before you read the data, establish the VOLUME_DATA
;	common block with the command:
;
;		COMMON VOLUME_DATA, VOL
;
;	The VOLUME_DATA common block has just one variable in it.  The variable
;	can have any name.  Here, we're using the name "VOL".  Now read the
;	data from the file into VOL.  For example:
;
;		OPENR, 1, "head.dat"
;		VOL = FLTARR(20, 30, 42)
;		READU, 1, VOL
;		CLOSE, 1
;
;	Now you can run the SLICER widget application by entering:
;
;		SLICER
;
;	The data stored in VOL is the data being worked on by the SLICER.
;
; MODIFICATION HISTORY:
;	DMS - RSI, Oct, 1991.
;-



COMMON volume_data, a
COMMON slicer_commmon, dims, sl, z_last, zb_last, mode, fixed



mode = 0
fixed = 0
sl_width = 240		;Slider width
mode_names = [ 'Slices', 'Block', 'Cutout', 'Isosurface', 'Probe', 'Colors', $
	'Rotations', 'Journal' ]
nmodes = n_elements(mode_names)		;# of modes

isop = { ISOP, hi_lo : 1, value: 0.0, window : 0, drawable : 0L, slider : 0L, $
		xs : fltarr(2) }

;	Main data structure
sl = {  SLICER, base : 0L, $		;Main base
	draw:0L, $			;Big drawable
	window:0, $			;Big drawable window index
	trans:0, $			;Transparency flag
	threshold_base: lonarr(4), $	;Threshold slider base for transp + but
	threshold:0b, $			;Transp threshold in pixel values
	mode_names : mode_names, $	;Names of modes
	interp: 0, $			;Interpolation flag
	mode_bases : lonarr(nmodes), $	;Mode panel bases
	nc3: 0, $			;# of colors per partition (3 of them)
	amax : 0.0, $			;Data max, min
	amin : 0.0, $
	color_button:lonarr(16), $	;Color  table buttons
	axex: intarr(3), $		;TRUE to reverse axis
	axrev: intarr(3), $		;Axis permutations
	ori_butt: lonarr(7), $		;Orientation buttons
	pos_text : 0L, $		;Label widget at bottom
	rotation: [ 30., 30.], $	;Current rotations
	v_close: 0, $			;Index of closest vertex
	p0 : fltarr(3,8), $		;Data coords of cube corners
	p1 : fltarr(3,8), $		;Device coords of cube corners
	vfaces : intarr(3,8), $		;Face index vs vertex index
	facevs : intarr(4,6), $		;Vertex index vs faces
	isop: isop, $			;Isosurface parameters
	p0cube : intarr(3,2), $		;Corner coords of cube selection
	cut_ovr : 0, $			;Cut mode
	cube_on : 0, $			;If cube is on
	cube_ip : 0, $			;Corner of cube
	shading : 0.20, $		;Amount of differential shading
	file_text : LONARR(2), $	;File name text widgets
	cslide : LONARR(3), $		;Color sliders
	rslide : LONARR(3), $		;Rotation sliders, aspect text
	journal : 0, $			;Journal file
	stretch : [0,100] }		;Stretch params

;  Faces vs vertex index
sl.vfaces = [[0,1,2],[1,2,3],[0,2,4],[2,3,4],[0,1,5], [1,3,5], [0,4,5], $
		[3,4,5]]
;  Vertex indices vs faces  (clockwise order).
sl.facevs = [ [0,2,6,4], [0,4,5,1], [2,0,1,3], [1,5,7,3], [3,7,6,2], $
		[6,7,5,4]]
;

if XRegistered("slicer") THEN RETURN
resolution = [ 640, 512]

set_plot,'Z'
device, set_resolution = resolution
set_plot,'X'

z_last = 0
zb_last = 0

s = size(a)
if s(0) ne 3 THEN $
	MESSAGE,'Slicer: volume_data common block does not contain 3D data'
dims = s(1:3)
d1 = dims-1

sl.p0cube = [[dims/4], [3 * dims/4]]
for i=0,7 do sl.p0(*,i) = $		;Data coords of corners
	[ (i and 1) * d1(0), (i and 2)/2 * d1(1), (i and 4)/4 * d1(2)]

if n_elements(range) ge 2 THEN BEGIN	;Range specified?
	sl.amax = range(1)
	sl.amin = range(0)
ENDIF ELSE BEGIN
	sl.amax = max(a, min = q)
	sl.amin = q
ENDELSE



sl.base = WIDGET_BASE(TITLE='IMSL/IDL Slicer', /ROW)
lbase = WIDGET_BASE(sl.base, /COLUMN)
rbase = WIDGET_BASE(sl.base, /COLUMN)
sl.draw = WIDGET_DRAW(rbase, XSIZE=resolution(0), YSIZE=resolution(1),$
	RETAIN=2,/BUTTON_EVENTS)

XPdMenu, [	'" Done "			EXIT',		$
		'" Erase "			ERASE',		$
		'" Undo "			UNDO',		$
		'" Postscript "			SAVEPS',	$
		'" Help "			HELP'		$
	],  lbase



junk1 = WIDGET_BASE(lbase, /ROW)
junk = WIDGET_BUTTON(junk1, VALUE='Orientation',/MENU)
ori_names = [ 'X Y Exchange', 'X Z Exchange', 'Y Z Exchange',$
	'X Reverse','Y Reverse','Z Reverse', 'Reset']
for i=0,6 do sl.ori_butt(i) = WIDGET_BUTTON(junk, VALUE=ori_names(i),$
		UVALUE = 'ORIENTATION')
widget_control, sl.ori_butt(0), SET_BUTTON=1

junk1 = WIDGET_BUTTON(junk1, VALUE = 'Sampling', /MENU)
junk =  WIDGET_BUTTON(junk1, VALUE = 'Interpolation', UVALUE = 'INTON')
junk =  WIDGET_BUTTON(junk1, VALUE = 'Nearest Neighbor', UVALUE = 'INTOFF')

junk = WIDGET_BASE(lbase, /FRAME, /COLUMN)
junk1 = WIDGET_LABEL(junk, value='Modes:  ')
junk1 = WIDGET_BASE(junk, COLUMN=3, /EXCLUSIVE)

for i=0,nmodes-1 do $	; Mode buttons
    junk2 = WIDGET_BUTTON(junk1, value=sl.mode_names(i), $
		uvalue=sl.mode_names(i), /NO_RELEASE)

junk = WIDGET_BASE(lbase, /FRAME, /COLUMN)
mode_base = WIDGET_BASE(junk)		;For the mode dependent bases

for i=0,nmodes-1 do $
   if i ne 2 then sl.mode_bases(i) = WIDGET_BASE(mode_base, uvalue=0L, /COLUMN)


parent = sl.mode_bases(0)		; slices mode
junk1 = WIDGET_DRAW(parent, XSIZE = sl_width, $
	YSIZE = sl_width * float(resolution(1)) / resolution(0))
widget_control, parent, set_uvalue=junk1

parent = sl.mode_bases(1)		;Cube & Cut modes
junk = WIDGET_BASE(parent, /ROW)
junk1 = WIDGET_BUTTON(junk, value=' GO ', uvalue='GOCUBE', /NO_RELEASE)
junk1 = WIDGET_BUTTON(junk, value=' Cancel ', uvalue='CANCUBE', /NO_REL)
junk1 = WIDGET_BASE(junk, /EXCLUSIVE, /ROW)
junk = WIDGET_BUTTON(junk1, VALUE="Cut Into", UVALUE="CUTINTO", /NO_REL)
junk = WIDGET_BUTTON(junk1, VALUE="Cut Over", UVALUE="CUTOVER", /NO_REL)
junk = WIDGET_DRAW(parent, XSIZE = sl_width, $
	YSIZE = sl_width * float(resolution(1)) / resolution(0))
widget_control, parent, set_uvalue= junk

sl.mode_bases(2) = sl.mode_bases(1)	;Cut is copy of cube

parent = sl.mode_bases(3) 		 ; Isosurface mode
junk = widget_button(parent, value='GO', UVALUE='GO')
junk = widget_base(parent, /row)
junk1 = widget_label(junk, value='Display: ')
junk = widget_base(junk, /row, /exclusive)
junk1 = widget_button(junk, value='High Side', uvalue='HIGH', /NO_REL)
junk1 = widget_button(junk, value='Low Side', uvalue='LOW', /NO_REL)
widget_control, junk1, /set_button	;Set low value
sl.isop.slider = WIDGET_SLIDER(parent, xsize=sl_width, MINIMUM = 0, $
		UVALUE = "ISOSLIDE", $
		TITLE = 'Isosurface Threshold (%)', $
		MAXIMUM = 100)
isodraw = WIDGET_DRAW(parent, XSIZE=sl_width+50, YSIZE = 100, /BUTTON_EVENTS)

;	Color tables
parent = sl.mode_bases(5)
junk1 = widget_base(parent, /ROW)
junk = WIDGET_BUTTON(junk1, VALUE = 'Color Tables', /MENU)
OPENR,lun, FILEPATH('colors.tbl'), /BLOCK,/GET_LUN  ;Read color table file
aa=ASSOC(lun, BYTARR(32))       ;Get name

FOR i = 0, 15 DO sl.color_button(i) = $	;Make color pull down buttons
	WIDGET_BUTTON(junk, VALUE=STRTRIM(aa(i),2), uvalue='COLORS')
FREE_LUN, lun

sl.cslide(0) = WIDGET_SLIDER(parent, xsize = sl_width, MINIMUM=0, /DRAG, $
	MAXIMUM=100, UVALUE = "STMIN", Title="Contrast Minimum", VALUE=0)
sl.cslide(1) = WIDGET_SLIDER(parent, xsize = sl_width, MINIMUM=0, /DRAG, $
	MAXIMUM=100, UVALUE = "STMAX", Title="Contrast Maximum", VALUE=100)
sl.cslide(2) = WIDGET_SLIDER(parent, xsize = sl_width, MINIMUM=0, /DRAG, $
	MAXIMUM=100, UVALUE = "SHADING", Title="Differential Shading (%)", $
	VALUE=20)


parent = sl.mode_bases(6)		;Rotations mode
sl.rslide(0) = WIDGET_SLIDER(parent, xsize=sl_width, MINIMUM=-90, MAXIMUM=90, $
	UVALUE = "XROTATION", Title="X Axis Rotation", VALUE=30)
sl.rslide(1) = WIDGET_SLIDER(parent, xsize=sl_width, MINIMUM=-90, MAXIMUM=90, $
	UVALUE = "ZROTATION", Title="Z Axis Rotation", VALUE=30)
junk = WIDGET_BASE(parent, /frame, /row)
junk1 = WIDGET_LABEL(junk, VALUE='Z Aspect Ratio:')
sl.rslide(2) = WIDGET_TEXT(junk, VALUE='1.0     ', /EDIT, YSIZE=1, XSIZE=10)


parent = sl.mode_bases(7)		;Journal mode
junk = WIDGET_BASE(parent, /COLUMN)
junk1 = WIDGET_BUTTON(junk, VALUE='Start Recording', UVALUE='RECORD', /NO_REL)
junk1 = WIDGET_BUTTON(junk, VALUE='Stop Recording', UVALUE='RECORDOFF',$
		/NO_REL)
junk1 = WIDGET_BUTTON(junk, VALUE='Playback', UVALUE='PLAYBACK', /NO_REL)
junk = WIDGET_BASE(parent, /ROW)
junk1 = WIDGET_LABEL(junk, value='File Name:')
sl.file_text(0) = WIDGET_TEXT(junk, xsize=24, ysize=1, $
	value='slicer.jou'+string(replicate(32b,14)), /EDIT, /FRAME)
sl.file_text(1) = WIDGET_TEXT(parent, xsize=32, ysize=1, $
	value='Journal Closed', /FRAME)

;	Transparency buttons / slider
junk = WIDGET_BASE(lbase, /FRAME, /COLUMN)
junk1 = WIDGET_BASE(junk, /ROW)
junk2 = WIDGET_LABEL(junk1, value='Transparency: ')
junk2 = WIDGET_BASE(junk1, /ROW, /EXCLUSIVE)
sl.threshold_base(1) = WIDGET_BUTTON(junk2, VALUE='Off', $
	UVALUE='TRANS', /NO_REL)
widget_control, junk1, /SET_BUTTON
sl.threshold_base(2) = WIDGET_BUTTON(junk2, VALUE='On', $
	UVALUE='TRANS', /NO_REL)
sl.trans = 0
; We want to map this button, so we put it in a base of its own
junk = widget_base(junk)
sl.threshold_base(0) = WIDGET_BASE(junk)
sl.threshold_base(3) = WIDGET_SLIDER(sl.threshold_base(0), xsize=sl_width, $
	MINIMUM=0, MAXIMUM=100,$
	UVALUE="THRESHOLD", TITLE="Transparency Threshold (%)", VALUE=0)

junk1 = WIDGET_BASE(lbase, /FRAME)
sl.pos_text = WIDGET_TEXT(junk1, xsize=40, ysize=1)


WIDGET_CONTROL, sl.base, /REALIZE
; Unmap mode dependent widgets  (Leave journal mapped because of obscure bug)
for i=1, nmodes-1 do widget_control, sl.mode_bases(i), MAP=0

WIDGET_CONTROL, sl.draw, get_value = junk
sl.window = junk		;Main window
WIDGET_CONTROL, isodraw, get_value = junk
sl.isop.window = junk		;Isosurface drawable
sl.isop.drawable = isodraw
WIDGET_CONTROL, sl.threshold_base(0), MAP=0	;Threshold not active yet

sl.nc3 = (!d.n_colors-4)/3	;Colors per orientation

slicer_orientation,6		;Reset to default orientation, erase
slicer_colors, 0

if n_elements(commands) gt 0 then slicer_playback, commands  ;Execute cmds?
if n_elements(cmd_file) gt 0 then slicer_playback, file = cmd_file

XManager, "slicer", sl.base, EVENT_HANDLER = slicer_events, GROUP = group
end
