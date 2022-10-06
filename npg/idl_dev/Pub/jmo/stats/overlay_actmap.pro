;***************************************
pro overlay_actmap,fi,dsp,wd,st,stc,help
;***************************************

;Purpose: Overlay activation map on anatomical image.

handle_value,fi.hdr_handle(fi.n),hdr_act
handle_value,fi.hdr_handle(hdr_act.mother),hdr_img

zoomx = dsp.zoom*hdr_act.xdim
zoomy = dsp.zoom*hdr_act.ydim
anat_img    = fltarr(hdr_img.xdim,hdr_img.ydim,hdr_img.zdim)
act_img     = fltarr(hdr_img.xdim,hdr_img.ydim,hdr_img.zdim)
anat_img_zoom    = fltarr(zoomx,zoomy,hdr_img.zdim)
act_img_zoom     = fltarr(zoomx,zoomy,hdr_img.zdim)
overlay_img = fltarr(dsp.zoom*hdr_img.xdim,dsp.zoom*hdr_img.ydim,hdr_img.zdim)
mask        = fltarr(hdr_img.xdim,hdr_img.ydim,hdr_img.zdim)

value = strarr(3)
label = strarr(3)
value(0) = string('0')
value(1) = string('1')
value(2) = string(hdr_img.zdim)
label(0) = string("FWHM of Gaussian filter (pixels): ")
label(1) = string('First plane')
label(2) = string('Last plane')
dims = get_str(3,label,value)
fwhm = float(dims(0))
first_pln = fix(dims(1)) - 1
last_pln = fix(dims(2)) - 1

if(hdr_img.tdim le 1) then begin
    labels = strarr(fi.nfiles)
    labels(0:fi.nfiles-1) = fi.tails(0:fi.nfiles-1)
    idx_anat = get_button(labels,TITLE='Anatomy image.')
endif else begin
    idx_anat = hdr_act.mother
endelse

for z=0,hdr_img.zdim-1 do begin
    anat_img(*,*,z) = get_image(z,fi,stc,FILNUM=idx_anat+1)
    mask(*,*,z) = anat_img(*,*,z) gt .05*max(anat_img(*,*,z))
    mean = total(anat_img(*,*,z)*mask(*,*,z))/total(mask(*,*,z))
    mask(*,*,z) = anat_img(*,*,z) gt .05*mean
endfor

labels = strarr(fi.nfiles)
labels(0:fi.nfiles-1) = fi.tails(0:fi.nfiles-1)
idx = get_button(labels,TITLE='Multiple comparisons-corrected image.')

label = strarr(2)
label(0) = string('Default')
label(1) = string('User-specified map')
title = string('Display default map or user-specified map?')
lclogp = get_boolean(title,label)

if(lclogp eq !FALSE) then begin
    labels = strarr(fi.nfiles)
    labels(0:fi.nfiles-1) = fi.tails(0:fi.nfiles-1)
    idx1 = get_button(labels,TITLE='Activation map to be overlayed.')
    widget_control,/HOURGLASS
    for z=first_pln,last_pln do begin
        act_img(*,*,z)  = get_image(z,fi,stc,FILNUM=idx+1)
    endfor
    mask_act = (act_img gt 0) + (act_img lt 0)
    for z=first_pln,last_pln do begin
        act_img(*,*,z)  = get_image(z,fi,stc,FILNUM=idx1+1)
    endfor
    act_img = act_img*mask_act
    name = strcompress(string(fi.tails(idx1),FORMAT='(a,"_overlay")'),/REMOVE_ALL)
endif else begin
    widget_control,/HOURGLASS
    for z=first_pln,last_pln do begin
        act_img(*,*,z)  = get_image(z,fi,stc,FILNUM=idx+1)
    endfor
    name = strcompress(string(fi.tails(idx),FORMAT='(a,"_overlay")'),/REMOVE_ALL)
endelse

;mask = anat_img gt 100
act_img = act_img * mask

max_anat = max(anat_img)
if(dsp.scale eq !FIXED) then begin
    min_act = dsp.fixed_min
    max_act = dsp.fixed_max
endif else begin
    min_act = min(act_img)
    max_act = max(act_img)
endelse
if(abs(min_act) gt abs(max_act)) then $
    max_act = abs(min_act) $
else $
    min_act = -max_act
act_img = 20.*act_img/(max_act - min_act)

act_img_smoth = gauss_smoth(act_img,fwhm)

mask = (act_img_smoth lt  1) * (act_img_smoth ge 0) + $
       (act_img_smoth gt -1) * (act_img_smoth lt 0)
act_img = act_img + 10
act_img = (1-mask)*act_img

for z=0,dsp.zdim-1 do begin
    anat_img_zoom(*,*,z) = rebin(anat_img(*,*,z),zoomx,zoomy)
    act_img_zoom(*,*,z)  = rebin((act_img(*,*,z) +  $
				!LEN_COLORTAB-20), zoomx,zoomy,/SAMPLE)
endfor
mask = act_img_zoom eq 0
	
overlay_img = (mask*(!LEN_COLORTAB-20)/max_anat)*anat_img_zoom + act_img_zoom

widget_control,HOURGLASS=0

paradigm = fltarr(5)
paradigm(0) = -10
put_image,overlay_img,fi,wd,dsp,name,zoomx,zoomy,hdr_img.zdim,1,!FLOAT,hdr_img.dxdy,hdr_img.dz,1,!OVERLAY_ARRAY,hdr_img.mother,paradigm
fi.color_scale(fi.n) = !OVERLAY_SCALE

;name = strcompress(string(fi.tails(idx),FORMAT='(a,"_actmap")'),/REMOVE_ALL)
;put_image,act_img,fi,wd,dsp,name,hdr_act.xdim,hdr_act.ydim,hdr_act.zdim,1,!FLOAT,hdr_img.dxdy,hdr_img.dz,1,!FLOAT_ARRAY,hdr_img.mother,paradigm

return
end
