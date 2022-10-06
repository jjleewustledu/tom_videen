pro NIkul_check, reset = reset

  common NIkul_common, nrriemann, nrkulproj

  if keyword_set(reset) then begin
    nrriemann = 0L
    nrkulproj = 0L
    return
  endif

  if n_elements(nrriemann) eq 0 then begin
    print, 'no calls to RIEMANN, no calls to NIkul_proj'
    return
  endif

  print, NIstring(nrriemann) + ' calls to RIEMANN, ' + $
         NIstring(nrkulproj) + ' calls to NIKUL_PROJ (or NIKUL_SPECTPROJ).'

end