PRO BASE_EVENT, event

         uv.wid.ifile: begin	; Service the input file event
               ecf = study.calscale/37.0 ; convert from Bq/cc to nCi/cc
               uv.values.ecf = ecf
                  uv.values.scandura = study.frame_duration(0)/1000.0/60.0
               uv.values.decay_fctr = study.decay_corr(0)
               uv.values.halflife = study.iso_halflife/60.0
               uv.values.nframes = study.nframes
               for i=0, study.nframes-1 do begin
                  uv.values.frame_starts(i) = study.frame_start_time(i)/1000.0/60.0
                  uv.values.frame_duras(i) = study.frame_duration(i)/1000.0/60.0
               endfor
         
         uv.wid.ofile: begin	; Service the output file event
               uv.values.ofile = ofile
         
         uv.wid.scandura: begin ; Service the scan duration event
                  
         uv.wid.frame_selected: begin
               uv.values.frame_selected = frame_selected
               uv.values.scandura = uv.values.frame_duras(frame_selected-1)

         uv.wid.injtime: begin	; Service the injection time event
               uv.values.injtime = injtime

         uv.wid.scantime: begin	; Service the scan start time event
               uv.values.scantime = scantime
         
         uv.wid.unit: begin	; Service the LMRGLu event
            uv.values.unit = event.value
         
         uv.wid.tissue: begin	; Service the tissue push down button event
            for i=0,4 do begin
               uv.values.k(i) = uv.values.cons(i, index)

         uv.wid.tissue_dis: begin ; Service the tissue type event
            uv.values.tissue = tissue(0)
         
         uv.wid.glucose: begin  ; Service the glucose concentration
            uv.values.glucose = glucose

         uv.wid.glucose_but: begin ; Service the average glucose event
            n = n_elements(uv.values.gsamples)
            uv.wid.glucose_pd = wid
         
         uv.wid.k(0): begin	; handles the lump constant button
            uv.values.k(0) = lc

         uv.wid.k(1): begin
            uv.values.k(1) = k1
         
         uv.wid.k(2): begin
            uv.values.k(2) = k2
         
         uv.wid.k(3): begin
            uv.values.k(3) = k3

         uv.wid.k(4): begin
            uv.values.k(4) = k4
         
         uv.wid.inputf: begin	; handles the input function button
            ifile = uv.values.ifile
            path = '/home/data/'
            inputf, event.top, uv.values.halflife,path
         
         uv.wid.model: begin    ; handles the model choice button
            uv.values.model = event.value
         
         uv.wid.done: begin

         uv.wid.model_parm: begin ; handles the compute  button
            total_time = uv.values.scantime-uv.values.injtime	 $
               + uvalues.frame_starts(uv.values.frame_selected-1)  $
               + uv.values.scandura/2.0
            uv.values.total_time = total_time
            k = fltarr(4)
            for i=1,4 do k(i-1) = uv.values.k(i)

; Interpolate raw data points
               nsize = 1024
               dt  =  (max(st) - min(st)) / nsize
               st_int  =  findgen(1024) * dt
               counts_int = interpol(counts, st, st_int)
               st = 0
               st = st_int
               counts = 0
               counts = counts_int

            uv.values.ce = ce
            uv.values.cm = cm
            mess = 'Free DG = ' + strcompress(ce, /remove_all) + ' nCi/ml  ;   ' +  $
               ' Trapped DG = '+ strcompress(cm, /remove_all) + ' nCi/ml'
            st = 0
            counts = 0
         
         uv.wid.model_image: begin ; handles the make LMRGLu button
            
; Use Ce, Cm and other parameters to compute the linear transformation,
; Apply to iput images and save output images to a matrix file

            print,'computing LMRGLu images ... '
            
            Cp = uv.values.glucose
            lc = uv.values.k(0)
            k1 = uv.values.k(1)
            k2 = uv.values.k(2)
            k3 = uv.values.k(3)
            k4 = uv.values.k(4)
            lmrglu = k1*k3*Cp / (lc*(k2+k3))
            uv.values.lmrglu = lmrglu
            s = make_lmrglu_image(uv.values, ce, cm, slope, intercept, cunit)	
            if s eq OK then begin
               ;;   slope_unit='  ' + uv.values.unit + ' / nCi/ml '
               ;;   intercept_unit='  ' + uv.values.unit
               if cunit ne 'nCi/cc' then	$
                  slope = slope*37.0 ; convert from Bq/cc to nCi/ml for now
               uv.values.slope = slope
               uv.values.intercept = intercept
               slope = strmid(strcompress(slope,/remove_all),0,7)
               intercept = strmid(strcompress(intercept,/remove_all),0,7)
         
         else: begin            ; handle the return event from other widgets
            p = where(tags eq 'GLUCOSE')
            if p(0) ne -1 then begin ; event from glucose samples
               uv.values.glucose = event.glucose.mean
               uv.values.gsamples = event.glucose.sample
      endcase

;----------------------------------------------------
; Main program to set the widgets and initialize data
;----------------------------------------------------

PRO METABTOOL, m_fname,FNAME=fname

   if cons_err then begin
      cons(0,0) = 0.42
      cons(1,0) = 0.102
      cons(2,0) = 0.1300
      cons(3,0) = 0.0620
      cons(4,0) = 0.0068
      tissue_choice(0) = 'FDG Gray Matter'
   endif
   kcons = fltarr(5)
   for i=0,4 do kcons(i) = cons(i,0)

   values = {				$
               ifile:'',		$
               ecf:0.0,			$
               scandura:0.0,		$
               decay_fctr:0.0,		$
               ofile:'',		$
               injtime:0.0,		$
               scantime:0.0,		$
               total_time:0.0,		$
               glucose:0.0,		$ ; Glucose concentration
               gsamples:gsamples,	$
               unit:unit_choice(0),	$
               tissue:tissue_choice(0),	$
               tissue_choice:tissue_choice,	$
               cons:cons,		$
               k:kcons,			$
               lmrglu:0.0,		$
               input_ok:0,		$
               inputfunc:inputfunc,	$
               bfile:'',		$
               fittype:'',		$
               halflife:0.0,		$
               model:model_choice(0),	$
               model_parm:-1,		$
               ce:0.0,			$ ; Free DG in Precursor
               cm:0.0,			$ ; Trapped DG-6-Phosphate
               slope:0.0,		$
               intercept:0.0,		$
               frame_starts:fltarr(30), $
               frame_duras:fltarr(30),	$
               nframes:0,		$
               frame_selected:1	$
            }

   uv = { wid:wid, values:values }
            
