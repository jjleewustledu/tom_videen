;+
; NAME:
;    NIget_ct
;
; PURPOSE:
;    Interrogate the color lookup table information set by NIset_ct,
;    and used by NIimage.
;
; CATEGORY:
;    widgets
;
; CALLING SEQUENCE:
;    result = NIget_ct(...)
;       Specify exactly one keyword.
;
; KEYWORD PARAMETERS:
;    NRCT: NIget_ct(/nrct) returns the current number of color lookup tables.
;
;    DRAWCOLORS: NIget_ct(/draw) equals 1 when drawcolors are used, 0 otherwise.
;
;    CTNUM: NIget_ct(ctnum = CTNUM) returns a two element array, containing the
;           lowest and highest color table index used by lookup table CTNUM.
;           If CTNUM is an array of N lookup table numbers, a 2 by N matrix is
;           returned.
;
;    CURRENT: NIget_ct(/cur) returns the number of the current lookup table.
;
;    LOW: NIget_ct(/low, ctnum=ctnum) returns the low index for CTNUM or 
;         the current table (default).
;
;    HIGH: NIget_ct(/low, ctnum=ctnum) returns the high index for CTNUM or 
;         the current table (default).
;
;    MINMAX: NIget_ct(/minmax, ctnum=ctnum) returns the relative min and max
;          in % for CTNUM or the current talble (default when CTNUM not given).
;
; OUTPUTS:
;    RESULT: returned value depends on the keyword.
;
; COMMON BLOCKS:
;    NIct_common
;    Shared by NIset_ct, NIget_ct, NIimage and NIdrawcolors.
;
; SEE ALSO:
;      NIset_ct, NIimage, NIdrawcolors
;
; MODIFICATION HISTORY:
; 	Written by:	Johan Nuyts, nov 1996
;-


function NIget_ct, nrct=nrct, drawcolors = drawcolors, ctnum=ctnum, $
      current=current, low=low, high=high, minmax=minmax

  common NIct_common, number_ct, ct_low, ct_high, draw_low, draw_high, $
            current_ct

  if n_elements(number_ct) eq 0 then NIset_ct
  if keyword_set(nrct) then return, number_ct
  if keyword_set(drawcolors) then return, draw_high ge 0
  if keyword_set(current) then return, current_ct
  if keyword_set(low) then $
     IF n_elements(ctnum) EQ 0 THEN return, ct_low(current_ct) $
                               ELSE return, ct_low(ctnum)
  if keyword_set(high) then $
     IF n_elements(ctnum) EQ 0 THEN return, ct_high(current_ct) $
                               ELSE return, ct_high(ctnum)
  IF keyword_set(minmax) THEN BEGIN
    bot = NIget_ct(ctnum=ctnum, /low)
    top = NIget_ct(ctnum=ctnum, /high)
    tvlct, r, g, b, /get
    r = r(bot:top)
    g = g(bot:top)
    b = b(bot:top)
    last = n_elements(r)-1
    daar = where((r NE r(0)) or (g NE g(0)) or (b NE b(0)))
    IF daar(0) EQ -1 THEN return, [0., 100.]
    minval = daar(0)
    daar = where((r NE r(last)) or (g NE g(last)) or (b NE b(last)))
    IF daar(0) EQ -1 THEN return, [0., 100.]
    maxval = daar(n_elements(daar)-1)
    return, [minval-1, maxval+1] * 100. / last
  ENDIF

  if n_elements(ctnum) gt 0 then $
     if max(ctnum) lt number_ct $
        then return, transpose([[ct_low(ctnum)], [ct_high(ctnum)]]) $
        else return, -1

end
