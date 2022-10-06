;*******************
pro adjust_colors,fi
;*******************

COMMON colors, r_orig, g_orig, b_orig, r_curr, g_curr, b_curr

vbot = fi.color_min(fi.n)
vtop = fi.color_max(fi.n)
gamma = fi.color_gamma(fi.n)
chop = 0
cbot = 0
ncolors = !d.table_size - cbot - 1
ncolors = ncolors < !LEN_COLORTAB
;ncolors = !LEN_COLORTAB  ; JMO
n = ncolors - 1
use_values = 0

if use_values then nc = 256 else nc = ncolors
s = (nc-1)/100.
x0 = vbot * s
x1 = vtop * s
if x0 ne x1 then s = (nc-1.0)/(x1 - x0) else s = 1.0
int = -s * x0
if gamma eq 1.0 then s = round(findgen(nc) * s + int > 0.0) $
else s = ((findgen(nc) * (s/nc) + (int/nc) > 0.0) ^ gamma) * nc
if chop ne 0 then begin
    too_high = where(s ge nc, n)
    if n gt 0 then s(too_high) = 0L
    endif
if use_values then begin
    s = s < 255L
    l = lindgen(ncolors) + cbot
    s = s + cbot
    r_curr(cbot) = (r = s(r_orig(l)))
    g_curr(cbot) = (g = s(g_orig(l)))
    b_curr(cbot) = (b = s(b_orig(l)))
endif else begin
    s = s + cbot
    r_curr(cbot) = (r = r_orig(s))
    g_curr(cbot) = (g = g_orig(s))
    b_curr(cbot) = (b = b_orig(s))
endelse
tvlct, r,g,b, cbot

return
end
