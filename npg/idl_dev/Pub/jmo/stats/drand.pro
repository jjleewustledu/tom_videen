;**************************
function drand,length,seed
;**************************


if(n_elements(seed) eq 0) then $
    seed = intarr(3)
rv = dblarr(length)
passtype = bytarr(3)
passtype(1) = 1
passtype(2) = 0
stat=call_external(!SHARE_LIB,'_drand',rv,length,seed,VALUE=passtype)

return,rv
end
