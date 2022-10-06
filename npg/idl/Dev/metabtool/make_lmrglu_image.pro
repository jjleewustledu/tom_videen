;
; make_lmrglu_image.pro
;
; IDL program to compute LMRGLu images from Emission Data

FUNCTION MAKE_LMRGLU_IMAGE, u, ce, cm, slope, intercept, unit_of_compute

   ERROR = 1
   OK = 0
   
   ifile = u.ifile
   ofile = u.ofile
   ecf = u.ecf
   decay_fctr = u.decay_fctr
   start_time = u.scantime-u.injtime
   duration = u.scandura
   total_time = u.total_time    ; total_time = start_time+u.frame_start+duration/2.0
   model = u.model
   lmrglu = u.lmrglu
   frame = u.frame_selected
   halflife = u.halflife
   lamda = alog(2.0)/halflife
   
   print,'ifile = ', ifile
   print,'ofile = ', ofile
   print,'decay correction factor in image subheader = ', decay_fctr
   print,'Time scan started = ', u.scantime
   print,'time of injection = ', u.injtime
   print,'absolute start time = ', start_time
   print,'frame start time = ', u.frame_starts(u.frame_selected-1)
   print,'scan duration = ', duration
   print,'total time = ', total_time

   ;; Read infile
   
   s = df_open(ifile,infp)
   if s ne OK then begin
      print,'error opening ', ifile, ' for reading'
      return,ERROR
   endif
   matstr = string(frame)+',*,1,0,0'
   matstr = strcompress(matstr, /remove_all)
   help,matstr
   s = df_read(infp,matstr,datap,hdr,/get_hdr)
   if s ne OK then begin
      print,'error reading data from ', ifile
      s = df_close(infp)
      return, ERROR
   endif
   handle_value,datap.data_ptr,data,/no_copy
   if decay_fctr gt 1.0 then begin
      print,'emission data was decay corrected'
      decay = exp(lamda*start_time)
   endif else begin
      print,'emission data was NOT decay corrected'
      decay = exp(lamda*total_time)
   endelse

   print,'data max and min : ',max(data), min(data)
   print,'total = ', total(data)
   
   ;;   sf=1.0/37.0			; convert from Bq/cc to nCi/cc
   sf = 1.0
   unit_of_compute = 'Bq/cc'
   cm2 = cm * 37.0
   ce2 = ce * 37.0
   p = strpos(u.unit,'umol')
   if p ne -1 then sf = 5.5*sf	; convert from mg/min/100g to umol/min/100g
   
   ;; Select model
   
   if model eq 'Sokoloff' then begin
      print,'Sokoloff model ..'
      slope = sf*lmrglu/cm2
      intercept = -sf*lmrglu*ce2/cm2
      data = temporary(data)*slope*decay+intercept  
   endif else if model eq 'Hutchins' then begin
      print,'Hutchins model ..'
      slope = sf*lmrglu/(ce2+cm2)
      intercept = 0.0
      data = temporary(data)*slope*decay
   endif else begin
      print,'Unknown model type, model = ', model
   endelse

   data = temporary(data) > 0.0

   print,'ecf = ', ecf
   print,'slope = ',slope
   print,'intercept = ', intercept
   print,'data max and min : ',max(data), min(data)
   print,'total = ', total(data)

   handle_value,datap.data_ptr,data,/set,/no_copy

   s = do_info(datap, study=study)
   study.calunits = 2
   study.callabel = u.unit
   study.calscale = 1.0
   s = do_info_set(datap, study = study)
   s = df_open(ofile,outfp,/NEW,dataptr=datap, dtype=7)
   if s ne OK then begin
      print,'error opening file ', ofile, ' for writing'
      err = 1
      s = df_close(infp)
      s = df_free_data(datap)
      return,ERROR
   endif

   s = df_write(outfp, datap)
   if s ne OK then begin
      print,'error writing data to ', ofile
      err = 1
      s = df_close(infp)
      s = df_close(outfp)
      s = df_free_data(datap)
      return,ERROR
   endif

   s = df_close(infp)
   s = df_close(outfp)
   data = 0
   s = df_free_data(datap)
   return,OK
end


