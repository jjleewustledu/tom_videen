;*******************************
function normaln_jmo,length,seed
;*******************************


rv = fltarr(length)
passtype = bytarr(3)
passtype(1) = 1
stat=call_external(!SHARE_LIB,'_normal_vector',rv,length,seed,VALUE=passtype)

return,rv
end
