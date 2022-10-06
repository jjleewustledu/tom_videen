PRO PETT6header_init, p6
   
   p6.file_flag = string(replicate(32b,2))
   p6.run_name = string(replicate(32b,6))
   p6.rotation = 0
   p6.wobble_rate = 0
   p6.nwobble_pts = 0
   p6.scan_time = 0
   p6.subject_id = string(replicate(32b,8))
   p6.date = string(replicate(32b,6))
   p6.resolution=0
   p6.isotope_code=0
   p6.slice_number = 0
   p6.decay_constant = 0L
   p6.image_orientation = 0
   p6.time_per_frame = 0
   p6.total_counts = 0L
   p6.compound = string(replicate(32b,10))
   FOR j=0,1 DO p6.unused_1[j] = 0
   p6.number_of_slices = 0
   FOR j=0,4 DO p6.unused_2[j] = 0
   p6.file_type = 0
   p6.ecat_orientation = 0
   p6.rotation_increment = 0
   p6.filter = string(replicate(32b,12))
   FOR j=0,1 DO p6.unused_4[j] = 0
   p6.un_norm_average = 0
   p6.norm_average = 0
   p6.total_raw_events = 0L
   p6.total_random_events = 0L
   p6.raw_random_events = 0L
   FOR j=0,12 DO p6.unused_5[j] = 0
   p6.norm_run_time = 0
   FOR j=0,7 DO p6.unused_6[j] = 0
   FOR j=0,15 DO p6.metabolic_data[j]=0
   FOR j=0,25 DO p6.unused_7[j] = 0
   p6.pett_number = 0
   p6.model_number = 0
   p6.pett_location = string(replicate(32b,12))

END
