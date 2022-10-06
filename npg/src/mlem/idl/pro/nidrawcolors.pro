;+
; NAME:
;    NIdrawcolors
;
; PURPOSE:
;    Called by NIset_ct. Do NOT use NIdrawcolors directly!
;    Use NIset_ct instead!
;
;    Modify the lookup table, inserting 8 colors at the bottom.
;    This enables plotting colored lines in overlay over gray-level images.
;    If the lookup table was already modified by a previous call, nothing
;    is done.
;    The routines NIimage and NIplot use this feature.
;
; CATEGORY:
;    Widgets
;
; CALLING SEQUENCE:
;    NIdrawcolors
;
; COMMON BLOCKS:
;    Uses the IDL-common block COLORS.
;    Uses also NIct_common, shared by NIimage, NIset_ct, NIget_ct and
;                                     NIdrawcolors.
;
; SIDE EFFECTS:
;    Loads the lookup table. Image on display will feel this.
;
; EXAMPLE:
;    NIdrawcolors
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, 17-jan-1995
;               Johan Nuyts, 1-nov-1996: adapted to be used together with
;                     NIset_ct.
;-

pro NIdrawcolors, nostretch=nostretch, standard=standard, $
              drawred=drawred, drawgreen=drawgreen, drawblue=drawblue

common COLORS, r_orig, g_orig, b_orig, r_curr, g_curr, b_curr
common NIct_common, number_ct, ct_low, ct_high, draw_low, $
       draw_high, current_ct
if n_elements(r_curr) eq 0 then loadct,0

IF n_elements(standard) EQ 0 AND n_elements(drawred) EQ 0 $
  THEN IF n_elements(draw_high) NE 0 THEN return

IF n_elements(draw_low)  EQ 0 THEN draw_low =  0
IF n_elements(draw_high) EQ 0 THEN draw_high = 7
IF keyword_set(standard) THEN draw_high =  7

new_r = [0, 1, 0, 1, 0, 1, 0, 1] * 255
new_g = [0, 0, 1, 1, 0, 0, 1, 1] * 255
new_b = [0, 0, 0, 0, 1, 1, 1, 1] * 255

index = [0,1,2,3,4,5,6,7]

IF n_elements(drawred) NE 0 THEN BEGIN
  new_r =  [new_r, drawred * 255]
  new_g =  [new_g, drawgreen * 255]
  new_b =  [new_b, drawblue * 255]
  index = indgen(n_elements(new_r))
  draw_high =  n_elements(new_r)-1
ENDIF

;
; Check whether the colors are already there.
;
if total((new_r(index) eq r_curr(index)) and $
         (new_g(index) eq g_curr(index)) and $
         (new_b(index) eq b_curr(index))) EQ n_elements(index) AND $
      NOT keyword_set(standard) then return

;print,'Stretching'

if not keyword_set(nostretch) then stretch, draw_high + 1
r_curr(index) = new_r
g_curr(index) = new_g
b_curr(index) = new_b

tvlct, r_curr, g_curr, b_curr

end
