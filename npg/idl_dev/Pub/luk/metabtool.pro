;
; MODULE NAME :
;	metabtool.pro
;
; PURPOSE:
;	IDL program for metabtool user interface
;
; CALLING SEQUENCE:
;	metabtool     (from IDL shell)
;
; INPUT
;	IDL widgets via user interface
;
; OUTPUT
;	Depending on the INPUT and run time options chosen. The normal result
;	are the Glucose Metabolic Image and an ASCII report file.
;
; Miscellaneous:
;
;	This file metabtool.pro contains 2 main modules : the main routine
;	'metabtool' sets up and widgets, initialize data and its event handler ;
;	The event handle is a big case statement that handles the response
;	of the different events. A collection of subroutines to handle error
;	checking are also included.
;
; HISTORY
;	15 Aug, 1994		P. Luk		v1
;

; -------------------------------
; routine to check the input file
; -------------------------------
function check_ifile,ifile,errmsg
  err=0			; 0 implies OK
  errmsg=''
  if ifile eq '/' then return,1
  q=findfile(ifile)
  ifile=ifile(0)
  if ifile eq '' then begin
     errmsg='file name not defined'
     err=1
  endif else if n_elements(q) gt 1 then return,1	$
  else begin
     q=findfile(ifile)

     if q(0) eq '' then begin
	errmsg='file ' + ifile + ' does not exists'
	err=1		; 1 implies error
     endif
  endelse
  return,err
end

; --------------------------------
; routine to check the output file
; --------------------------------
function check_ofile,ofile,errmsg
   err=0
   if ofile eq '/' or ofile eq '' then return,1
   q=findfile(ofile)
   if n_elements(q) gt 1 then return,1
   q=q(0)
   found=where(q eq ofile)
   if found(0) ne -1 then begin
	errmsg='file ' + ofile + ' already exists'
	err=2		; 2 implies warning
   endif
   return,err
end

; ---------------------------------------------------------------------------
; routine to check the bounds, inclusively or exclusively of a floating point
; number, when specified.
; ----------------------------------------------------------------------------
function check_number,x,errmsg,FMIN=fmin,FMAX=fmax,EFLAG=eflag

	lower=0
	upper=0
	exclusive=0
	err=0
	errmsg=''

;	if keyword_set(FMIN) then lower=1
;	if keyword_set(FMAX) then upper=1
;	if keyword_set(EFLAG) then exclusive=1
 	if n_elements(FMIN) ne 0 then lower=1
	if n_elements(FMAX) ne 0 then upper=1
	if n_elements(EFLAG) ne 0 then exclusive=1 

	if lower eq 0 and upper eq 0 then return,0	; no checking !
;;;	if fmin le 0.0001 then fmin=0.0
;;;	print,'lower, upper = ', lower, upper
;;;	print,'exclusive = ', exclusive
;;;	if n_elements(fmin) ne 0 then print,'fmin = ', fmin
;;;	if n_elements(fmax) ne 0 then print,'fmax = ', fmax

	if exclusive then begin
;;;	print,'exclusive bounds check  ..'
	  if upper and lower then begin
	    if x ge fmax or x le fmin then begin
		err=1
		errmsg=' has to be inside the bounds, exclusively of ' +  $
 			'[' + string(fmin) + ',' + string(fmax) + ']'
	    endif
	  endif else if upper then begin
	    if x ge fmax then begin
		err=1
		errmsg=' has to be less ' + string(fmax)
	    endif
	  endif else if lower then begin
	    if x le fmin then begin
		err=1
		errmsg=' has to be greater than ' + string(fmin)
	    endif
	  endif
	endif else begin
;;;	print,'inclusive bounds check ..'
	  if upper and lower then begin
	    if x gt fmax or x lt fmin then begin
		err=1
		errmsg=' has to be inside the bounds, inclusively of ' +  $
 			'[' + string(fmin) + ',' + string(fmax) + ']'
	    endif
	  endif else if upper then begin
	    if x gt fmax then begin
		err=1
		errmsg=' has to be less or equal to ' + string(fmax)
	    endif
	  endif else if lower then begin
	    if x lt fmin then begin
		err=1
		errmsg=' has to be greater than or equal to ' + string(fmin)
	    endif
	  endif
	endelse

	return,err
end

; -----------------------------------------------------------------------
; routine to place the cursor at the end of the text of the IDL compound
; widget CW_FIELD
; ------------------------------------------------------------------------
pro set_cursor, cwid

   widget_control,widget_info(cwid,/child),get_uvalue=x
   widget_control,cwid,get_value=y
   cpos=strlen(strcompress(string(y), /remove_all))
   widget_control,x.textid,set_text_select=[cpos,0],/input_focus

end

; ---------------------------------------------
; the main event handler routine for all events
; ---------------------------------------------

pro base_event, event
goto,godoit
catch,metab_ev_err
  if metab_ev_err ne 0 then begin
	print,'Trapped Error= ', metab_ev_err
	print,!ERROR
	print,!ERR_STRING
	err=1
	errmsg='Metabtool Fatal Error'
	goto,continue
endif

godoit:
widget_control,event.top,get_uvalue=uv
tags=tag_names(event)
p = where(tags eq 'VALUE')
if p(0) ne -1 then wval=event.value
OK=0
mess=''
errmsg=''

if event.id eq uv.wid.model_image then begin
	idlist = [uv.wid.ifile, uv.wid.ofile,				$
		uv.wid.injtime, uv.wid.scantime,  			$
		uv.wid.glucose, uv.wid.k(0), uv.wid.k(1), uv.wid.k(2),	$
		uv.wid.k(3), uv.wid.k(4), uv.wid.model_parm, event.id]
endif else if event.id eq uv.wid.model_parm then begin
	idlist = [uv.wid.ifile,				$
		uv.wid.injtime, uv.wid.scantime,			$
		uv.wid.k(0), uv.wid.k(1), uv.wid.k(2), uv.wid.k(3),	$
		uv.wid.k(4), event.id]
endif else begin
	idlist = [event.id]
endelse
for idx=0, n_elements(idlist)-1 do begin
;;   fmin=0.00001
   fmin=0.0
   idnum=idlist(idx)
   if n_elements(idlist) gt 1 then begin
	code = widget_info(idnum, /type)
	if code eq 0 then begin
	  get_widget_value,idnum,wval
;;;;	  print,'widget value = ', wval
;;;;	  help,/st,wval	  
	endif
   endif
   err=OK
   case idnum of
      uv.wid.ifile: begin	; - service the input file event
	ifile=wval(0)
	print,'input file name = ', ifile

;;	if ifile ne '' then begin
;;	  p=rstrpos(ifile,'/')
;;	  if p eq -1 then ifile='/ecat_data/' + ifile
;;	  widget_control,uv.wid.ifile,set_value=ifile
;;	endif

	s=strpos(ifile,'/')
	if s eq -1 then work_ifile='/home/data/' + ifile else work_ifile=ifile
;;	err=check_ifile(ifile, errmsg)
	err=check_ifile(work_ifile, errmsg)
	if err eq OK then begin
	   mess='Reading ' + ifile + ' ...  PLEASE WAIT'
	   widget_control,uv.wid.mess,set_value=mess
	   uv.values.ifile=ifile
	   s=df_open(work_ifile,dfptr)
	   if s ne 0 then begin
		err = 1
		goto,continue
	   endif
	   s=df_info(dfptr, study=study)
	   if s ne 0 then begin
		err = 1
		goto,continue
	   endif
	   ecf=study.calscale/37.0		; convert from Bq/cc to nCi/cc
	   uv.values.ecf = ecf
	   ;widget_control,uv.wid.ecf,set_value=uv.values.ecf

print,'inside ifile service :'
print,uv.values.scandura
print,uv.values.frame_selected
print,uv.values.frame_duras
print,uv.values.frame_starts

	   if uv.values.frame_duras(0) eq 0.0 then begin
	      uv.values.scandura = study.frame_duration(0)/1000.0/60.0
	   endif else begin
	      uv.values.scandura = $
		uv.values.frame_duras(uv.values.frame_selected-1)
	   endelse
	   widget_control,uv.wid.scandura,set_value=uv.values.scandura
	   uv.values.decay_fctr = study.decay_corr(0)
	   uv.values.halflife=study.iso_halflife/60.0
	   s=df_close(dfptr)

;;	   if n_elements(idlist) eq 1 then begin
;;	     p=strpos(uv.values.model,'Sok')
;;	     q=rstrpos(ifile, '.')	   
;;	     if p ne -1 then begin 
;;	       ofile=strmid(ifile, 0, q) + '.sok'
;;	     endif else ofile=strmid(ifile, 0, q) + '.hut'

	     s=assign_ofname(ifile, uv.values.model, ofile)
	     if s eq OK then begin
		widget_control,uv.wid.ofile,set_value=ofile
		uv.values.ofile=ofile
	     endif
	   set_cursor, uv.wid.ofile
	   mess='Input File   ' + ifile + '   OK'
	   frames = '(1-' + string(study.nframes) + ')'
	   frames=strcompress(frames)
	   frames = 'frames ' + frames
	   widget_control, uv.wid.frames_label,set_value=frames
	   uv.values.nframes=study.nframes
	   for i=0, study.nframes-1 do begin
		uv.values.frame_starts(i)=study.frame_start_time(i)/1000.0/60.0
		uv.values.frame_duras(i)=study.frame_duration(i)/1000.0/60.0
	   endfor
	   study=0
	endif else begin
	    set_cursor, uv.wid.ifile
	    errmsg = 'Emission  ' + errmsg
	endelse
	end
;     uv.wid.ecf: begin		; - service the ECF event
;	ecf = wval(0)
;	print,'ecf = ', ecf
;	err=check_number(ecf,errmsg,FMIN=fmin,/EFLAG)
;	if err eq OK then begin
;	   uv.values.ecf=ecf
;	   mess='ECF set to ' + string(ecf) + '   nCi/ml  /  ECAT cps'
;	endif else errmsg = 'ECF  ' + errmsg
;       end
      uv.wid.ofile: begin	; - service the output file event
	ofile = wval(0)
	print,'output file name = ', ofile
	err=check_ofile(ofile, errmsg)
	if err eq OK then begin
	   if ofile ne uv.values.ofile then mess='Output File set to  ' + ofile
	   uv.values.ofile=ofile
	   set_cursor, uv.wid.scantime
	endif else if err eq 2 then begin
	   errmsg='Metabolic  ' + errmsg + ' ; Please Delete or Rename'
	endif else errmsg='Metabolic File Name'
	end
      uv.wid.scandura: begin	; - service the scan duration event
	print,'scan duration = ', event.value
	err=check_number(event.value,errmsg,FMIN=fmin,/EFLAG)
	if err ne 1 then uv.values.scandura=event.value
	end
      uv.wid.frame_selected: begin
	widget_control, uv.wid.frame_selected, get_value=frame_selected
	frame_selected=fix(frame_selected(0))
	if frame_selected lt 1 OR frame_selected gt uv.values.nframes  $
	then begin
	  err=1
	  errmsg='selected frame ' + string(frame_selected) + ' does not exists'
	endif else begin
	  uv.values.frame_selected=frame_selected
	  uv.values.scandura=uv.values.frame_duras(frame_selected-1)
	  widget_control, uv.wid.frame_start_time,  $
		set_value=uv.values.frame_starts(frame_selected-1)
	  widget_control, uv.wid.scandura,  $
		 set_value=uv.values.frame_duras(frame_selected-1)
	endelse
	end
      uv.wid.injtime: begin	; - service the injection time event
	injtime=wval(0)
	print,'injection time = ', injtime
	err=check_number(injtime,errmsg,FMIN=fmin)
	if err eq OK then begin
	   uv.values.injtime=injtime
	   mess='Injection Time set to ' + string(injtime)
	endif else errmsg='Injectime ' + errmsg
	end
      uv.wid.scantime: begin	; - service the scan start time event
	scantime=wval(0)
	print,'scan start time = ', scantime
	injtime=uv.values.injtime
;;;;	if uv.values.injtime eq 0.0 then injtime=0.00001
	print,'injtime = ', injtime
	print,'scantime = ', scantime
	err=check_number(scantime,errmsg,FMIN=injtime)
	if err eq OK then begin
	   uv.values.scantime=scantime
	   mess='Scan Time set to ' + string(scantime)
	   set_cursor, uv.wid.glucose
	endif else begin
	   errmsg='Scantime has to be greater than injection time'
	endelse
	end
      uv.wid.unit: begin	; - service the LMRGLu event
	uv.values.unit=event.value
	print,'LMRGLu unit = ', uv.values.unit
	widget_control,uv.wid.unit_stat,set_value=event.value
	end
      uv.wid.tissue: begin	; - service the tissue push down button event
	index=event.value-1
	widget_control,uv.wid.tissue_dis,		$
		set_value=uv.values.tissue_choice(index)
	for i=0,4 do begin
	   widget_control,uv.wid.k(i), set_value=uv.values.cons(i, index)
	   uv.values.k(i)=uv.values.cons(i, index)
	endfor
	print,'tissue = ', uv.values.tissue
	uv.values.tissue=uv.values.tissue_choice(index)
	end

      uv.wid.tissue_dis: begin		; - service the tissue type event
	widget_control,uv.wid.tissue_dis,get_value=tissue
	uv.values.tissue=tissue(0)
	print,'tissue display set to : ', uv.values.tissue_dis
	set_cursor, uv.wid.k(0)
	end
      uv.wid.glucose: begin		; - service the glucose concentration
	glucose=wval(0)
	print,'glucose concentration = ', glucose
	err=check_number(glucose,errmsg,FMIN=fmin,/EFLAG)
	if err eq OK then begin
	   uv.values.glucose=glucose
	   mess='Glucose concentration set to  ' + string(glucose)
;;;;;;;	   if uv.values.input_ok then begin
;;;;;;;		widget_control,uv.wid.model_image, /input_focus
;;;;;;;	   endif else widget_control,uv.wid.inputf, /input_focus
;;;;;;;	   set_cursor,uv.wid.k(0)
	endif else errmsg='Glucose ' + errmsg
	end

      uv.wid.glucose_but: begin		; - service the average glucose event
	print,'Glucose sample button ..'
;;;;	print,'uv.wid.glucose_pd = ', uv.wid.glucose_pd
	n=n_elements(uv.values.gsamples)
	if uv.wid.glucose_pd eq 0L then begin
	   gsamp, event.top, 0L, n, ret=wid
	   uv.wid.glucose_pd=wid
	endif else begin
	   wid=uv.wid.glucose_pd
	   gsamp, event.top, wid, n
	endelse
;;;;	print,'glucose pd widget ID = ', uv.wid.glucose_pd
        end
      uv.wid.k(0): begin	; handles the lump constant button
	lc=wval(0)
	print,'lump constant = ', lc
	err=check_number(lc,errmsg,FMIN=fmin,/EFLAG)
	if err eq OK then begin
	   uv.values.k(0)=lc
	   mess='Lump Constant set to  ' + string(lc)
	   set_cursor, uv.wid.k(1)
	endif else errmsg='Lump Constant ' + errmsg
	end
      uv.wid.k(1): begin
	k1=wval(0)
	print,'k1 = ', k1
	err=check_number(k1,errmsg,FMIN=fmin,FMAX=1.0,/EFLAG)
	if err eq OK then begin
	   uv.values.k(1)=k1
	   mess='k1 set to  ' + string(k1)
	   set_cursor, uv.wid.k(2)
	endif else begin
	   set_cursor,uv.wid.k(1)
	   errmsg='k1 ' + errmsg
	endelse
	end
      uv.wid.k(2): begin
	k2=wval(0)
	print,'k2 = ', k2
	err=check_number(k2,errmsg,FMIN=fmin,FMAX=1.0,/EFLAG)
	if err eq OK then begin
	   uv.values.k(2)=k2
	   mess='k2 set to  ' + string(k2)
	   set_cursor, uv.wid.k(3)
	endif else begin
	   set_cursor,uv.wid.k(2)
	   errmsg='k2 ' + errmsg
	endelse
	end
      uv.wid.k(3): begin
	k3=wval(0)
	print,'k3 = ', k3
	err=check_number(k3,errmsg,FMIN=fmin,FMAX=1.0,/EFLAG)
	if err eq OK then begin
	   uv.values.k(3)=k3
	   mess='k3 set to  ' + string(k3)
	   set_cursor, uv.wid.k(4)
	endif else begin
	   set_cursor,uv.wid.k(3)
	   errmsg='k3 ' + errmsg
	endelse
	end
      uv.wid.k(4): begin
	k4=wval(0)
	print,'k4 = ', k4
	err=check_number(k4,errmsg,FMIN=fmin,FMAX=1.0)
	if err eq OK then begin
	   uv.values.k(4)=k4
	   mess='k4 set to  ' + string(k4)
;;;;;;;	   if uv.values.input_ok then begin
;;;;;;;		widget_control,uv.wid.model_image, /input_focus
;;;;;;;	   endif else widget_control,uv.wid.inputf, /input_focus
	endif else errmsg='k4 ' + errmsg
	end
      uv.wid.inputf: begin	; handles the input function button
;;	inputf has to return a status so that metabtool knows the status,
;;	either (OK or FAILURE) of the input function. Also, the array
;;	representating the input function has to be passed back.
	print,' building input function ...'
	ifile=uv.values.ifile
	path='/home/data/'
	cerr=check_ifile(ifile,errmsg)
	if cerr eq OK then begin
	   p=rstrpos(ifile,'/')
	   if p ne 0 then path=strmid(ifile,0,p+1)
	endif
	inputf, event.top, uv.values.halflife,path
	end
      uv.wid.model: begin		; handles the model choice button
	print,'model choice = ', event.value
	uv.values.model=event.value
	widget_control,uv.wid.model_dis,set_value=event.value
	p=strpos(uv.values.model,'Sok')
	widget_control,uv.wid.ofile,get_value=ofile
	ofile=ofile(0)

;;	if ofile ne '' then begin
;;	   q=rstrpos(ofile, '.')
;;	   if p ne -1 then begin
;;		ofile=strmid(ofile, 0, q) + '.sok'
;;	   endif else ofile=strmid(ofile, 0, q) + '.hut'

	widget_control,uv.wid.ifile,get_value=ifile
	ifile=ifile(0)
	s=assign_ofname(ifile,uv.values.model,ofile)
	if s eq OK then begin
	   widget_control,uv.wid.ofile,set_value=ofile
	   if ofile ne uv.values.ofile then begin
	     uv.values.ofile=ofile
	     mess='Model = ' + uv.values.model + ' ; LMRGLu File set to    '   $
		+ ofile
	   endif
	endif
	end

      uv.wid.done: begin
	widget_control,event.top,/destroy
	return
	end

      uv.wid.model_parm: begin		; handles the compute  button
;	call routines to compute Ce and Cm based on previous definitions
;	update Ce and Cm values in ??

	print,'computing ce and cm ...'
	if uv.values.input_ok ne 1 then begin
	   err=1
	   errmsg='input function not READY, cannot compute model parameters'
	   goto,continue
	endif
print,'servicing compute model parms: scandura = ', uv.values.scandura
	total_time=uv.values.scantime-uv.values.injtime	 $
		+uv.values.frame_starts(uv.values.frame_selected-1)  $
		+uv.values.scandura/2.0

;	total_time=uv.values.scantime-uv.values.injtime		$
;			+uv.values.scandura/2.0

	uv.values.total_time=total_time

print,'scan time = ', uv.values.scantime
print,'injectime time = ', uv.values.injtime
print,'frame start time = ', uv.values.frame_starts(uv.values.frame_selected-1)
print,'scan duration = ', uv.values.scandura
print,'total time = ', total_time

	handle_value,uv.values.inputfunc,inputfunc,/no_copy
	s=make_Cpstar(inputfunc.x,inputfunc.y,total_time, st, counts)
	handle_value,uv.values.inputfunc,inputfunc,/set,/no_copy
	if s ne OK then begin
	   err=1
	   errmsg='input function error'
	   goto,continue
	endif
;;;;	print,'return status after make_Cpstar = ', s
	k=fltarr(4)
	for i=1,4 do k(i-1)=uv.values.k(i)

; interpolate raw data points if 'No Fit' was selected

	if uv.values.fittype eq 'No Fit' then begin
	   nsize = 1024
	   dt = (max(st) - min(st)) / nsize
	   st_int = findgen(1024) * dt
	   counts_int = interpol(counts, st, st_int)
	   st = 0
	   st = st_int
	   counts = 0
	   counts = counts_int
	endif
	s=ce_cm_trapozoid(total_time,k,st,counts,ce,cm)
;;;;	print,'return status for Ce and Cm computation = ', s
	if s ne OK then begin
	   err=1
	   errmsg='error in computing Ce and Cm'
	   goto,continue
	endif
	uv.values.ce=ce
	uv.values.cm=cm
	mess='Free DG = ' + strcompress(ce, /remove_all) + ' nCi/ml  ;   ' +  $
	     ' Trapped DG = '+ strcompress(cm, /remove_all) + ' nCi/ml'
;;;;	print,'Ce = ', ce, ' Cm = ', cm
	st=0
	counts=0
	end
      uv.wid.model_image: begin		; handles the make LMRGLu button
;	use Ce, Cm and other parameters to compute the linear transformation,
;	apply to iput images and save output images to a matrix file

	print,'computing LMRGLu images ... '
;;	print,'emission data = ', uv.values.ifile
;;	print,'LMRGLu data = ', uv.values.ofile
;;	print,'ECAT ecf = ', uv.values.ecf
;;	print,'Decay Correction Factor = ', uv.values.decay_fctr
;;	print,'Scan duration = ', uv.values.scandura
;;	print,'Injection Time = ', uv.values.injtime
;;	print,'Scan Start Time = ', uv.values.scantime
;;	print,'Time of experiment = ', uv.values.total_time
;;	print,'Glucose concentration = ', uv.values.glucose
;;	print,'Lump and Rate constants = ', uv.values.k
;;	print,'Input Function Status = ', uv.values.input_ok
;;	print,'Model Choice = ', uv.values.model
;;	print,'Free DG in Precursor= ', uv.values.ce
;;	print,'Trapped DG-6-Phosphate = ', uv.values.cm
	if uv.values.input_ok ne 1 then begin
	   errmsg='Input Function not READY'
	   err=1
	   goto,continue
 	endif
	mess='Making LMRGLu Images ...'
	widget_control,uv.wid.mess,set_value=mess
	Cp=uv.values.glucose
	lc=uv.values.k(0)
	k1=uv.values.k(1)
	k2=uv.values.k(2)
	k3=uv.values.k(3)
	k4=uv.values.k(4)
	lmrglu=k1*k3*Cp / (lc*(k2+k3))
;;;;	print,'lmrglu = ', lmrglu
	uv.values.lmrglu=lmrglu
	s=make_lmrglu_image(uv.values, ce, cm, slope, intercept, cunit)	
;;;;	print,'return status from lmrglu = ', s
	if s eq OK then begin
;	   slope_unit='  ' + uv.values.unit + ' / nCi/ml '
;	   intercept_unit='  ' + uv.values.unit
	   if cunit ne 'nCi/cc' then	$
	      slope=slope*37.0		; convert from Bq/cc to nCi/ml for now
	   uv.values.slope=slope
	   uv.values.intercept=intercept
	   slope=strmid(strcompress(slope,/remove_all),0,7)
	   intercept=strmid(strcompress(intercept,/remove_all),0,7)
	   mess='slope = ' + strcompress(slope,/remove_all) +  $
		' ; ' + 'intercept = ' + strcompress(intercept,/remove_all)
;		'  ;  LMRGLu image made'
	   s=make_report(uv,rfname)
	   if s ne 0 then begin
		err=1
		errmsg='failed to make report file '
		goto, continue
	    endif else mess=mess + ' ; Report = ' + rfname
	endif else begin
	   err=1
	   errmsg='failed to make metabolic image'
	endelse
	end
      else: begin	; handle the return event from other widgets
	print,'the wild card case is reserved for input function and glucose'
;;;	print,'event.id = ', event.id
	tags=tag_names(event)
	p=where(tags eq 'GLUCOSE')
	if p(0) ne -1 then begin	; event from glucose samples
	   uv.values.glucose=event.glucose.mean
	   uv.values.gsamples=event.glucose.sample
	   widget_control,uv.wid.glucose,set_value=event.glucose.mean
	   mess='Average glucose samples set to '+string(event.glucose.mean)
	endif else	$
	  if event.err eq 0 then begin
	    uv.values.input_ok=1
;;;	    print,'inputfun x :'
;;;	    print,event.inputx
;;;	    print,'inputfun y :'
;;;	    print,event.inputy
;;;;	    print,'blood sample file name : ', event.bfile
	    inputfunc={ x:event.inputx, y:event.inputy }
	    handle_value,uv.values.inputfunc,inputfunc,/set,/no_copy
	    widget_control,uv.wid.inputf_stat,	$
		set_value='READY - ' + event.fittype
	    mess='Input Function OK - ' + event.fittype
	    uv.values.fittype=event.fittype
	    uv.values.bfile=event.bfile
	  endif else begin
	    err=1
	    errmsg='Input Function definition FAILED ; No Update'
	    goto, continue
	  endelse
	end
   endcase

continue:
   if err eq 1 then head = 'ERROR: '
   if err eq 2 then head = 'WARNING: '
   if err ne OK then begin
;	spawn,'beep > /dev/console'
	xbell
	mess = head + errmsg
   endif else begin
	if mess eq '' then mess='MESSAGE WINDOW: '
	widget_control,event.top,set_uvalue=uv
   endelse
   widget_control,uv.wid.mess,set_value=mess
   if err ne OK then goto,break
endfor 
break:

end

; -------------------------------
; clean up procedure before Exit
; -------------------------------
pro cleanup, id
;;print,'entering final clean up procedure ...'
widget_control,id,get_uvalue=uv
if uv.values.input_ok then handle_free,uv.values.inputfunc

end


;----------------------------------------------------
; Main program to set the widgets and initialize data
;----------------------------------------------------
pro metabtool,em_fname,FNAME=fname

; try to open tissue data file to set up data for tissue pop up widget

;;if NOT keyword_set(FNAME) then fname='tissue.dat'
if NOT keyword_set(FNAME) then fname=capp_file('data/tissue.dat')
maxset=4
tissue_choice=strarr(maxset)
cons=fltarr(5,maxset)
cons_err=0
initmess=''
openr,lun,/get_lun,fname,ERROR=err
if err ne 0 then begin
   print,!ERR_STRING
   cons_err=1
   initmess='ERROR: opening tissue file  ' + fname + $
		' ;  Tissue pop up widget NOT defined'
endif else begin
   str=''
   for i=0,maxset-1 do begin
	readf,lun,str
	decode_string,str,ss,n,'	'
	tissue_choice(i)=ss(0)
	for j=0,4 do cons(j, i)=float(ss(j+1))
   endfor
   close,lun
   free_lun,lun
   ftemp=0
endelse

;;; print,tissue_choice
;;; print,cons

if cons_err then begin
    cons(0,0)=0.42
    cons(1,0)=0.102
    cons(2,0)=0.1300
    cons(3,0)=0.0620
    cons(4,0)=0.0068
    tissue_choice(0)='FDG Gray Matter'
endif
kcons=fltarr(5)
for i=0,4 do kcons(i)=cons(i,0)
;;; print,'k = ', kcons

;
; Set up and Define all Widgets used in the metabtool main menu
;
base=widget_base(title='Metabtool-KOLN (v1.31) 15-Jan-1997', column=1)
messtext='MESSAGE WINDOW: '
mess=widget_text(base, value=messtext)

database=widget_base(base,row=2, frame=2)	;14-jan-97, PL
dbase1=widget_base(database,row=1)
ifiletext='Emission File Name:       '
IF n_elements(em_fname) ne 0 THEN fvalue=em_fname ELSE fvalue=''
ifile=cw_field(dbase1, title=ifiletext,value=fvalue, /return_events)
;ecf=cw_field(dbase1, title='ECF (nCi/ml / ECAT cps)', /return_event,	$
;			/floating, xsize=11)

dbase2=widget_base(database,row=1)
ofiletext='Metabolic Rate File Name: '
ofile=cw_field(dbase2, title=ofiletext, /return_events)
scandura=cw_field(dbase2, title='Frame Duration (min)', xsize=12)

;dbase3=widget_base(database, row=1)		;14-jan-97, PL		
frames_label = widget_label(dbase1, value='frames (1-1 )')
frame_selected = widget_text(dbase1, value = '1', xsize=2, /editable)
frame_start_time = cw_field(dbase1, title='frame start (min)', value=0.0)

tbase=widget_base(base,row=1,frame=2)
tx1='Time of Injection (min):'
injtime=cw_field(tbase, title=tx1, value=0.0, /return_events, /floating)
tx2='Scan Start (relative to injection (min)):'
scantime=cw_field(tbase, title=tx2, /return_events, /floating)

kbase=widget_base(base,frame=2,row=2)
kbase1=widget_base(kbase,row=1)
glucose_but=widget_button(kbase1, value='Glucose')
tx3='in (mg/100ml):'
glucose=cw_field(kbase1, title=tx3, /return_events, /floating)
ngsamp=4
gsamples=fltarr(ngsamp)

junk={PDMENU, flags:0, name:''}
tissue=[ {PDMENU, 1, 'Tissue'},			$
		{PDMENU, 0, tissue_choice(0)},	$
		{PDMENU, 0, tissue_choice(1)},	$
		{PDMENU, 0, tissue_choice(2)},	$
		{PDMENU, 0, tissue_choice(3)}	$
	]
tissue_w=CW_PDMENU(kbase1, tissue, /Return_index)
tissue_dis_w=widget_text(kbase1, value=tissue_choice(0), /editable)

kbase2=widget_base(kbase,row=1)
t=strarr(5)
t(0)='LC'
t(1)='k1'
t(2)='k2'
t(3)='k3'
t(4)='k4'
k=lonarr(5)
for i=0,4 do k(i)=cw_field(kbase2, title=t(i), value=cons(i), xsize=10, $
			  /return_events, /floating)

inputbase=widget_base(base,row=1,frame=2)
unit_choice=['umol/min/100g', 'mg/min/100g']
unit=[ {PDMENU, 1, 'LMRGLu Unit'},	$
	{PDMENU, 0, unit_choice(0)},	$
	{PDMENU, 0, unit_choice(1)}	$
     ]
unit_w=CW_PDMENU(inputbase, unit, /Return_name)
unit_dis_w=widget_text(inputbase, value=unit_choice(0))

tx2='Input Function'
inputf_but=widget_button(inputbase,value=tx2)
tx3='Status:'
inputf_stat=cw_field(inputbase, title=tx3, value='NOT Ready')

model_base=widget_base(base, row=1, frame=2)
model_choice=[ 'Sokoloff', 'Hutchins' ]
models=[ {PDMENU, 1, 'Model Choice'},  $
		{PDMENU, 0, model_choice(0)}, {PDMENU, 0, model_choice(1)}  $
	]
model_w=CW_PDMENU(model_base, models, /Return_name)
model_dis_w=widget_text(model_base,value=model_choice(0), xsize=15)

model_parm=widget_button(model_base,value='Compute Model Parameters')
model_image=widget_button(model_base,value='Make LMRGLu Images')
done=widget_button(model_base,value='Done')

inputfunc=handle_create()
wid = {					$
	mess:mess,			$
	ifile:ifile,			$
;	ecf:ecf,			$
	scandura:scandura,		$
	ofile:ofile,			$
	frames_label:frames_label,	$
	frame_selected:frame_selected,	$
	frame_start_time:frame_start_time, $
	injtime:injtime,		$
	scantime:scantime,		$
	tissue:tissue_w,		$
	tissue_dis:tissue_dis_w,	$
	unit:unit_w,			$
	unit_stat:unit_dis_w,		$
	glucose:glucose,		$
	glucose_pd:0L,			$
	glucose_but:glucose_but,	$
	k:k,				$
	inputf:inputf_but,		$
	inputf_stat:inputf_stat,	$
	model:model_w,			$
	model_dis:model_dis_w,		$
	model_parm:model_parm,		$
	model_image:model_image,	$
	done:done			$
      }

values = {			$
	ifile:'',		$
	ecf:0.0,		$
	scandura:0.0,		$
	decay_fctr:0.0,		$
	ofile:'',		$
	injtime:0.0,		$
	scantime:0.0,		$
	total_time:0.0,		$
	glucose:0.0,		$
	gsamples:gsamples,	$
	unit:unit_choice(0),	$
	tissue:tissue_choice(0),	$
	tissue_choice:tissue_choice,	$
	cons:cons,		$
	k:kcons,		$
	lmrglu:0.0,		$
	input_ok:0,		$
	inputfunc:inputfunc,	$
	bfile:'',		$
	fittype:'',		$
	halflife:0.0,		$
	model:model_choice(0),	$
	model_parm:-1,		$
	ce:0.0,			$
	cm:0.0,			$
	slope:0.0,		$
	intercept:0.0,		$
	frame_starts:fltarr(30), $
	frame_duras:fltarr(30), $
	nframes:0,		$
	frame_selected:1	$
	}

;;; print,'wids : '
;;; help,/st,wid
uv = { wid:wid, values:values }
widget_control,base,set_uvalue=uv
widget_control,base,/realize
if initmess ne '' then begin
;   spawn,'beep > /dev/console'
   xbell
   widget_control,uv.wid.mess,set_value=initmess
endif
set_cursor,uv.wid.ifile
xmanager,'metabtool',base,event='base_event',cleanup='cleanup'
end
