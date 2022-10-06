pro sbhdrdmp7,infile,plane,frame,gate

if (n_params() lt 1) then begin
  print,"Usage: sbhdrdmp7,infile,plane,frame,gate
endif

mh=rd_mhdr(infile)

sh=rd_shdr(infile,plane,frame,gate)

shbase=widget_base(/COLUMN,TITLE=string('Subheader, ',infile))

case !ECAT.file_types(mh.file_type) of

"S" : begin
    data_type=widget_label(shbase,value=string("Data type: ",sh.data_type),/ALIGN_LEFT)
    num_r_elements=widget_label(shbase,value=string("Number of rays: ",sh.num_r_elements),/ALIGN_LEFT)
    num_angles=widget_label(shbase,value=string("Number of angles: ",sh.num_angles),/ALIGN_LEFT)
    num_z_elements=widget_label(shbase,value=string("Number of planes: ",sh.num_z_elements),/ALIGN_LEFT)
    ring_difference=widget_label(shbase,value=string("Max ring difference: ",sh.ring_difference),/ALIGN_LEFT)
    scale_factor=widget_label(shbase,value=string("Scale factor: ",sh.scale_factor),/ALIGN_LEFT)
    scan_min=widget_label(shbase,value=string("Minimum: ",sh.scan_min),/ALIGN_LEFT)
    scan_max=widget_label(shbase,value=string("Maximum: ",sh.scan_max),/ALIGN_LEFT)
    prompts=widget_label(shbase,value=string("Prompts: ",float(sh.prompts)*1.e-6,' Mcts'),/ALIGN_LEFT)
    delayed=widget_label(shbase,value=string("Delayed: ",float(sh.delayed)*1.e-6,' Mcts'),/ALIGN_LEFT)
    multiples=widget_label(shbase,value=string("Multiples: ",float(sh.multiples)*1.e-6,' Mcts'),/ALIGN_LEFT)
    net_trues=widget_label(shbase,value=string("Net trues: ",float(sh.net_trues)*1.e-6,' Mcts'),/ALIGN_LEFT)
    total_coin_rate=widget_label(shbase,value=string("Total coincidence rate: ",$
	sh.total_coin_rate),/ALIGN_LEFT)
    frame_start_time=widget_label(shbase,value=string("Frame start time: ",$
	float(sh.frame_start_time),' sec'),/ALIGN_LEFT)
    frame_duration=widget_label(shbase,value=string("Frame duration: ",$
	float(sh.frame_duration)*.001,' sec'),/ALIGN_LEFT)
    dt_corr_fctr=widget_label(shbase,value=string("Deadtime Correction factor: ",$
	sh.dt_corr_fctr),/ALIGN_LEFT)
    xres=widget_label(shbase,value=string("X resolution: ",sh.x_resolution,' cm'),/ALIGN_LEFT)
    yres=widget_label(shbase,value=string("Y resolution: ",sh.y_resolution,' cm'),/ALIGN_LEFT)
    zres=widget_label(shbase,value=string("Z resolution: ",sh.z_resolution,' cm'),/ALIGN_LEFT)
end
"V" : begin
    data_type=widget_label(shbase,value=string("Data type: ",sh.data_type),/ALIGN_LEFT)
    xdim=widget_label(shbase,value=string("X dimension: ",sh.x_dimension),/ALIGN_LEFT)
    y_dimension=widget_label(shbase,value=string("Y dimension: ",sh.y_dimension),/ALIGN_LEFT)
    z_dimension=widget_label(shbase,value=string("Z dimension: ",sh.z_dimension),/ALIGN_LEFT)
    recon_zoom=widget_label(shbase,value=string("Zoom factor: ",sh.recon_zoom),/ALIGN_LEFT)
    scale_factor=widget_label(shbase,value=string("Scale factor: ",sh.scale_factor),/ALIGN_LEFT)
    image_min=widget_label(shbase,value=string("Minimum: ",sh.image_min),/ALIGN_LEFT)
    image_max=widget_label(shbase,value=string("Maximum: ",sh.image_max),/ALIGN_LEFT)
    x_pixel_size=widget_label(shbase,value=string("X voxel size: ",sh.x_pixel_size,' cm'),/ALIGN_LEFT)
    y_pixel_size=widget_label(shbase,value=string("Y voxel size: ",sh.y_pixel_size,' cm'),/ALIGN_LEFT)
    z_pixel_size=widget_label(shbase,value=string("Z voxel size: ",sh.z_pixel_size,' cm'),/ALIGN_LEFT)
    frame_duration=widget_label(shbase,value=string("Duration: ",.001*sh.frame_duration,' sec'),/ALIGN_LEFT)
    frame_start_time=widget_label(shbase,value=string("Start time: ",.001*sh.frame_start_time,' sec'),/ALIGN_LEFT)
    x_res=widget_label(shbase,value=string("X resolution: ",sh.x_res,' cm'),/ALIGN_LEFT)
    y_res=widget_label(shbase,value=string("Y resolution: ",sh.y_res,' cm'),/ALIGN_LEFT)
    z_res=widget_label(shbase,value=string("Z resolution: ",sh.z_res,' cm'),/ALIGN_LEFT)
    x_rot_angle=widget_label(shbase,value=string("X rotation: ",sh.x_rot_angle,' deg'),/ALIGN_LEFT)
    y_rot_angle=widget_label(shbase,value=string("Y rotation: ",sh.y_rot_angle,' deg'),/ALIGN_LEFT)
    z_rot_angle=widget_label(shbase,value=string("Z rotation: ",sh.z_rot_angle,' deg'),/ALIGN_LEFT)
    dec_corr_fctr=widget_label(shbase,value=string("Decay correction: ",sh.dec_corr_fctr),/ALIGN_LEFT)
    fil_cutoff_freq=widget_label(shbase,value=string("Filter cutoff: ",sh.fil_cutoff_freq),/ALIGN_LEFT)
    fil_dc_comp=widget_label(shbase,value=string("Filter DC comp.: ",sh.fil_dc_comp),/ALIGN_LEFT)
    fil_rs=widget_label(shbase,value=string("Filter resolution: ",sh.fil_rs),/ALIGN_LEFT)
    fil_order=widget_label(shbase,value=string("Filter order: ",sh.fil_order),/ALIGN_LEFT)
    fil_sf=widget_label(shbase,value=string("Filter scatter frac: ",sh.fil_sf),/ALIGN_LEFT)
    fil_scat_slope=widget_label(shbase,value=string("Filter scatter slope: ",sh.fil_scat_slope),/ALIGN_LEFT)
end
else: begin
    xerror=widget_label(shbase,value=string('File type not supported: ',infile),/ALIGN_LEFT)
end
endcase
    
widget_control,shbase,/REALIZE

return
end
