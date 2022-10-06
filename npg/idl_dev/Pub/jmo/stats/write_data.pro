;*********************************************************
function write_data,fi,st,dsp,wd,stc,img_type,help,cstm,pr
;*********************************************************

; Write data to disk.

if(fi.nfiles ge !NUM_FILES) then begin
    st.error_msg=string('Too many images loaded.')
    st.error = !TRUE
    return,-1
endif

case img_type of

!GIF: begin
    if(help.enable eq !TRUE) then begin
        widget_control,help.id,SET_VALUE=string('Writes either the main window (the image contained in the main window) or the currently selected custom window to a GIF file.')
    endif
;   Write current image to a GIF file.
    id = get_id(dsp,cstm,pr)
    if(id lt 0) then begin
       stat=widget_message('*** No valid windows.  ***',/ERROR)
        return,!ERROR
    endif
    wset,id
    gif_file = strcompress(string(id, $
    			FORMAT='("custom_",i,".gif")'),/REMOVE_ALL)
    gif_file = pickfile(/read,FILTER='*.gif',PATH='.', $
                    GET_PATH=wrtpath,FILE=gif_file)
    if(gif_file eq '') then return,!ERROR
    write_gif,gif_file(0),tvrd()
    wset,dsp.image_index
    widget_control,wd.error,SET_VALUE=string(id,gif_file(0), $
		FORMAT='("Window #",i," written to ",a)')
    error = !FALSE
end

!RAW: begin
;   Write current image to a RAW file.
    if(help.enable eq !TRUE) then begin
        widget_control,help.id,SET_VALUE=string('Writes the currently selected file to a 4DFP format file.  This function is only valid for processed data. It will not save input data.  File names are constructed by appending .img and .rec to the displayed name.')
    endif
    handle_value,fi.hdr_handle(fi.n),hdr
    if(hdr.tdim gt 1) then begin
 	stat=widget_message('Multiple frames cannot be stored as ifh files.',/ERROR)
	return,!ERROR
    endif
    hdr.tdim = 1
    raw_file = strcompress(string(fi.name,'_xxx.4dfp.img'),/REMOVE_ALL)
    raw_file = pickfile(/read,FILTER='*.img',PATH='.', $
                GET_PATH=wrtpath,FILE=raw_file)
    if(raw_file eq '') then return,!ERROR
    raw_file = strcompress(wrtpath+raw_file,/REMOVE_ALL)
    get_lun,lun
    img = fltarr(hdr.xdim,hdr.ydim,hdr.zdim)
    openw,lun,raw_file
    for z=0,hdr.zdim-1 do begin
        img(*,*,z) = get_image(z,fi,stc)
        writeu,lun,img(*,*,z)
    end
    close,lun
    free_lun,lun
    img_min = min(img)
    img_max = max(img)
    cmd1 = string(!BIN,'/write_hdr_ifh ')
    cmd3 = string(hdr.xdim,hdr.ydim,hdr.zdim)
    cmd4 = string(img_min,img_max,32,hdr.dxdy,hdr.dxdy,hdr.dz)
    cmd = cmd1 + raw_file + cmd3 + cmd4
    spawn,cmd,status
    if(status(0) ne '0') then begin
	string = string('Error while writing headers',raw_file)
	print,status
 	stat=widget_message(string,/ERROR)
    endif
    close,lun
    free_lun,lun
    widget_control,wd.error,SET_VALUE= string(raw_file(0), $
                FORMAT='("Image written to ",a)')
    error = !FALSE
end

else: begin
 	stat=widget_message('Unknown file type.',/ERROR)
        st.error_msg = string('Unknown file type.')
        st.error = !TRUE
end

endcase

return,error
end
