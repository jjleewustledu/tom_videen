;**********************************************************/
pro read_header,filnam,xdim,ydim,zdim,tdim,dxdy,dz,paradigm
;**********************************************************/

    dot = rstrpos(filnam,'.')
    if(dot gt 0) then $
        stem = strmid(filnam,0,dot) $
    else $
        stem = filnam
    hfil = strcompress(string(stem,".ifh"),/REMOVE_ALL)
    lenfil = n_elements(hfil)
    if(lenfil gt 1) then begin
        hdrfil = hfil(0)
	for i=1,lenfil-1 do hdrfil = hdrfil+hfil(i)
    endif else begin
	hdrfil = hfil(0)
    endelse

;   Get header.
    test = strlen(findfile(hdrfil))
    if(test(0) gt 0) then begin
        ifh = read_mri_ifh(hdrfil)
        xdim = ifh.matrix_size_1
        ydim = ifh.matrix_size_2
        zdim = ifh.matrix_size_3
        tdim = ifh.matrix_size_4
        dxdy = ifh.scale_1
        dz   = ifh.scale_3
	paradigm = fltarr(tdim)
	n = strlen(ifh.mri_paradigm_format)
	if(n eq 0) then begin
	    paradigm(2) = -2
	    return
 	endif
        paradigm = convert_paradigm(ifh.mri_paradigm_format,tdim)
    endif else begin   
;	Look for .rec file.
	stat=widget_message('Could not find interfile (.ifh) header.  Will try to parse .rec file.')
        hfil = strcompress(string(filnam,".rec"),/REMOVE_ALL)
        lenfil = n_elements(hfil)
        if(lenfil gt 1) then begin
            hdrfil = hfil(0)
	    for i=1,lenfil-1 do hdrfil = hdrfil+hfil(i)
        endif else begin
	    hdrfil = hfil(0)
        endelse
        rec = read_4dfp_rec(hdrfil)
        xdim = rec.xdim
        ydim = rec.ydim
        zdim = rec.zdim
        tdim = rec.tdim
        dxdy = rec.xpix
        dz   = rec.zpix
	paradigm = fltarr(tdim) ; Flag not loaded by setting dimension to tdim+1
	paradigm(0) = -2
    endelse

return
end
