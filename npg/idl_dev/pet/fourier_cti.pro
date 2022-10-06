;-----------------------------------------------------------------------------
;
;	 Adapted from Fourierdemo
;	 ------------------------
;	 Read and filters images in cti format 
;	 As long as the load button is not pushed, next/previous scan the file
;	 directory sequentially.
;
;	 Filters have been adapted toinclude cti ones (Hann, Hamm, Parzen ) 
;	 Donner SW conventions have been followed (max cutoff at .5)
;
;	 Care has been taken to sensitize widgets as follows
;
;	 lowpass and highpass options are active for all filters
;	 bandpass and bandreject are inactive for Hann, Hamming and Parzen
;	 order is only active for butterworth and exponential filters
;	 in "bandmode", the meaning of "cutoff" is changed into "band center" 
;
;	 C. Michel July,13 1992
;--------------------------------------------------------------------------------
;
; Copyright (c) 1991, Research Systems, Inc.  All rights reserved.
;	Unauthorized reproduction prohibited.

pro load_image, newdata, DESCRIPTION = file, DIMENSIONS = dimensions, GROUP=group, NFILE= nfile

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot, specs

if(keyword_set(nfile)) then nfile=1 else nfile = 0

if (nfile eq 1) then begin
	data_dir = '/topo6b/mich/data/cti'
	cd, data_dir, current=curdir
	file = pickfile(GROUP=group,TITLE='Select CTI image',FILTER=['img','scn','wbp','imp'])
	cd, curdir
	filename=file
	if (filename eq '') then return
	openr,unit,file,/get_lun
	stat=rmh(unit,mh)
	if(stat eq !ECAT.ERROR) then print, !ECAT.ERROR
	nf = mh.file.nframes
	np = mh.file.nplanes
	ng = mh.file.ngates
	nb = mh.file.nbeds
	f = 0 & p = 0 & g = 0 & b = 0
	if (nf gt 0) then f='*'
	if (np gt 0) then p='*'
	if (ng gt 0) then g='*'
	if (nb gt 0) then b='*'
	expand,mh,f,p,g,b
	f=fix(f) & p=fix(p) & g=fix(g) & b=fix(b)
	mf = n_elements(f)
	mp = n_elements(p)
	mg = n_elements(g)
	mb = n_elements(b)
	ntot  = mf*mp*mg*mb
	mats=lonarr(5,ntot)
	specs=strarr(ntot)
	stat = rdir(unit,dir)
	free_lun,unit
	;print,ntot,'Matrixes'
	for i =0, ntot-1 do begin
		mats(*,i) = matval(dir(0,i))
		f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
		specs(i) = ' frame '+ string(f) +' plane '+string(p)+' gate '+string(g)+ $
			' data '+string(d)+' bed '+string(b)
	end
	;print,mats
	index=0
	maxindex = ntot-1
endif

f= mats(0,index) & p= mats(1,index) & g= mats(2,index) & d= mats(3,index) & b= mats(4,index)
;print, filename, 'contains ',ntot,' matrixes'
;print,' frame ',f,' plane ',p,' gate ',g,' data ',d,' bed ',b
stat=recat(filename,f,p,g,0,b,data)
if(stat eq !ECAT.ERROR) then begin
	print,!ECAT.ERROR
	return
end

dimensions = [data.xdim,data.ydim]
;
;	Fourier needs multiple of 2 data
;
if (data.xdim eq data.ydim) then begin	; normal images are always 64 or 128 or 256
	newdata=bytscl(rebin(data.data,256,256))
endif else begin			; scan or nonsquare data
	sc = max([data.xdim,data.ydim])/256.
	newdata=bytscl(poly_2d(data.data,[0,0,sc,0],[0,sc,0,0],1,256,256))
endelse
end

	;------------------------------------------------------
	;   procedure setsliders
	;------------------------------------------------------
	; this routine sets the sliders that are data dependent.
	; The keyword NEW tells whether to label the new sliders
	; or destroy old ones before making replacements.
	;------------------------------------------------------

PRO setsliders, new = new

COMMON fourier, image, imagetrans, view, cutofflabel, bandwidthlabel, cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN, HAMM, PARZ, EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS, NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs

dimens = SIZE(image)
IF(KEYWORD_SET(NEW)) THEN BEGIN
  sliderwidth = (NVIEWS*VIEWSIZE) + ((NVIEWS+1)*20) - 670
  cutoffslider = WIDGET_SLIDER(sliderbase, XSIZE = sliderwidth, $
		VALUE =  20, MINIMUM = 1, MAXIMUM = 20, UVALUE = FILTERCUTOFF, /SUPPRESS_VALUE)
  cutoff = dimens(1) / 2. 
  cutofflabel = WIDGET_LABEL(sliderbase, VALUE = "Cutoff at " + STRING(cutoff/dimens(1)))
  bandwidth = dimens(1)/4.
  bandwidthslider = WIDGET_SLIDER(sliderbase, XSIZE = sliderwidth, $
		VALUE = 10, MINIMUM = 0, MAXIMUM = 20, UVALUE = FILTERBANDWIDTH, /SUPPRESS_VALUE)
  WIDGET_CONTROL, bandwidthslider, SENSITIVE = 0
  bandwidthlabel = WIDGET_LABEL(sliderbase, VALUE = "Bandwidth is " + STRING(bandwidth/dimens(1)))
  WIDGET_CONTROL, bandwidthlabel, SENSITIVE = 0
  matlabel = WIDGET_LABEL(fourierbase, VALUE = "Select matrix or use << and >> buttons")
  matlist = WIDGET_LIST(fourierbase,VALUE = specs, UVALUE = SETMATRIX, YSIZE = 5)
ENDIF ELSE BEGIN
  WIDGET_CONTROL, cutoffslider, SET_VALUE = 20.
  cutoff = dimens(1) / 2.
  WIDGET_CONTROL, cutofflabel, SET_VALUE = "Cutoff at " + STRING(cutoff/dimens(1))
  WIDGET_CONTROL, bandwidthslider, SET_VALUE = 5.
  bandwidth = dimens(1)/4.
  WIDGET_CONTROL, bandwidthlabel, SET_VALUE = "Bandwidth is " + STRING(bandwidth/dimens(1))
  widget_control,matlist,set_value=specs
ENDELSE
END
	;------------ end of procedure setsliders -------------


	;------------------------------------------------------
	;   procedure loaddata
	;------------------------------------------------------
	; This routine gets the name of the new data file to be
	; loaded. It loads the
	; data, resets the controls, and computes the new FFT of
	; the new data.  It also erases the old filtered image
	; and the old filtered power spectrum.
	;
	; KEYWORDS:
	;	NAME - if name is specified, the user is not
	;	  prompted with a choice and name is used in its
	;	  place.
	;------------------------------------------------------

PRO loaddata, NEW = NEW, NFILE = NFILE

COMMON fourier, image, imagetrans, view, cutofflabel, bandwidthlabel, cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN,  HAMM, PARZ,EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS, NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs

load_image,newdata,description=datadesc,dimensions=datadim,group=fouriercontrol, nfile=nfile

IF(KEYWORD_SET(newdata)) THEN BEGIN
  IF(datadim(1) NE 1) THEN BEGIN
    IF(datadim(0) LT (VIEWSIZE / 2)) THEN BEGIN
      datadim(0) = datadim(0) * (VIEWSIZE / datadim(0))
      datadim(1) = datadim(1) * (VIEWSIZE / datadim(1))
      image = REBIN(newdata, datadim(0), datadim(1))
    ENDIF ELSE IF (datadim(0) GT VIEWSIZE) THEN BEGIN
      sx = datadim(0) / (1 + datadim(0)/VIEWSIZE)
      sy = datadim(1) / (1 + datadim(1)/VIEWSIZE)
      if (datadim(0) mod sx ne 0) or (datadim(1) mod sy ne 0) then $
	image = CONGRID(newdata, VIEWSIZE, VIEWSIZE) $
      	else image = REBIN(newdata, sx, sy)
      datadim = (SIZE(image))(1:2)
    ENDIF ELSE image = newdata
  ENDIF ELSE image = newdata	;datadim(1)
ENDIF				;keyword_set

IF(NOT(KEYWORD_SET(NEW))) THEN setsliders

IF(KEYWORD_SET(image)) THEN imagetrans = FFT(image, -1)
filteredimage = 0
filteredspectrum = 0

END
	;------------- end of procedure loaddata --------------


	;------------------------------------------------------
	;   procedure resetfilter
	;------------------------------------------------------
	; This procedure computes the correct filter depending
	; on the bandpass type.  Here a high pass filter is 
	; one minus the low pass filter.
	;------------------------------------------------------

PRO resetfilter, NEW = NEW

COMMON fourier, image, imagetrans, view, cutofflabel, bandwidthlabel,cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN, HAMM, PARZ, EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS,NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs

COMMON fourier_dist, distfun

dimens = SIZE(image)

filteredimage = 0				;reset the filtered image and
						;the filtered spectrum since
filteredspectrum = 0				;changing the filter affects
						;them
imagesize = SIZE(image)
IF N_ELEMENTS(distfun) ne N_ELEMENTS(image) then begin
	IF(imagesize(0) EQ 1) THEN BEGIN	;compute either a single 
	  distfun = FINDGEN(imagesize(1))	;dimensional or two dimensional
	  distfun = distfun < (imagesize(1) - distfun)	;euclidean dist fcn
	ENDIF ELSE BEGIN		;2D case
	  distfun = DIST(imagesize(1))		;depending on the data dim.
	ENDELSE
	distfun(0) = 1e-4			;Avoid division by 0
ENDIF						;New dist fcn
WIDGET_CONTROL, waitlabel, $			;Display wait message while
		SET_VALUE = "PLEASE WAIT"	;computing filter

CASE FILTERTYPE OF

   BUTTER: BEGIN			;Butterworth filter
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 1
	   WIDGET_CONTROL, filtset(0), sensitive=1
	   WIDGET_CONTROL, filtset(1), sensitive=1
	   WIDGET_CONTROL, filtset(2), sensitive=1
	   WIDGET_CONTROL, filtset(3), sensitive=1
	    cutofftext ='Cutoff at'
  	    x = distfun /cutoff	
	    IF(bandtype EQ LOWPASS) THEN $
	      filter = 1. / (1. + x^(2 * order)) $
	    ELSE IF(bandtype EQ HIGHPASS) THEN $
	      filter = 1.-(1. / (1. + x^(2 * order))) $
	    ELSE BEGIN
	      cutofftext ='Band center at'
	      filter = distfun * distfun - cutoff^2  ;Dist squared
	      zeroes = where(filter eq 0.0, count)
	      if count ne 0 then filter(zeroes) = 1e-6  ;Avoid divide by 0
	      filter = 1.0 / (1.0 + ((distfun*bandwidth)/ filter) ^ (2 * order))
	      IF bandtype eq BANDPASS then filter = 1.0 - filter
	    ENDELSE
           END

  HANN: BEGIN				; Hann Filter
	   cutofftext ='Cutoff at'
           IF(bandtype eq BANDPASS) or (bandtype eq BANDREJECT) then begin	  
               WIDGET_CONTROL, cutofflabel, SET_VALUE = cutofftext + STRING(cutoff/dimens(1))
	       bandtype = LOWPASS
	       WIDGET_CONTROL, filtset(0), /SET_BUTTON		; Low pass
	   endif
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 0
	   WIDGET_CONTROL, filtset(2), sensitive=0
	   WIDGET_CONTROL, filtset(3), sensitive=0
	   x = distfun /cutoff	
	   IF(bandtype EQ LOWPASS) THEN $	
	     filter = (x le 1.)*(0.5+0.5*cos(!pi*x))$
           ELSE IF(bandtype EQ HIGHPASS) THEN $
	     filter = 1.0 - ((x le 1.)*(0.5+0.5*cos(!pi*x)))
         END

  HAMM: BEGIN				; Hamm Filter
	   cutofftext ='Cutoff at'
          IF(bandtype eq BANDPASS) or (bandtype eq BANDREJECT) then begin	  
               WIDGET_CONTROL, cutofflabel, SET_VALUE = cutofftext + STRING(cutoff/dimens(1))
	       bandtype = LOWPASS
	       WIDGET_CONTROL, filtset(0), /SET_BUTTON		; Low pass
	   endif
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 0
	   WIDGET_CONTROL, filtset(2), sensitive=0
	   WIDGET_CONTROL, filtset(3), sensitive=0
	   x = distfun /cutoff	
	   IF(bandtype EQ LOWPASS) THEN $	
	     filter = (x le 1.)*(0.54+0.46*cos(!pi*x))$
           ELSE IF(bandtype EQ HIGHPASS) THEN $
	     filter = 1.0 - ((x le 1.)*(0.54+0.46*cos(!pi*x)))
         END
  
  PARZ: BEGIN				; Parzen Filter
	  cutofftext ='Cutoff at'
          IF(bandtype eq BANDPASS) or (bandtype eq BANDREJECT) then begin	  
               WIDGET_CONTROL, cutofflabel, SET_VALUE = cutofftext + STRING(cutoff/dimens(1))
	       bandtype = LOWPASS
	       WIDGET_CONTROL, filtset(0), /SET_BUTTON		; Low pass
	   endif
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 0
	   WIDGET_CONTROL, filtset(2), sensitive=0
	   WIDGET_CONTROL, filtset(3), sensitive=0
	   x = distfun /cutoff	
	   filter = (x le .5)*(1.-6.*x*x*(1-x)) + (x gt .5 and x le 1.)*(2.*(1.-x)^3)
	   IF(bandtype EQ HIGHPASS) THEN filter = 1 - filter
         END

  EXPON: BEGIN				;Exponential filter
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 1
	   WIDGET_CONTROL, filtset(0), sensitive=1
	   WIDGET_CONTROL, filtset(1), sensitive=1
	   WIDGET_CONTROL, filtset(2), sensitive=1
	   WIDGET_CONTROL, filtset(3), sensitive=1
	   cutofftext ='Cutoff at'
	   x = distfun /cutoff	
	   IF(bandtype EQ LOWPASS) THEN $
	     filter = exp(-x^order) $
	   ELSE IF(bandtype EQ HIGHPASS) THEN $
	     filter = 1.0 - exp(-x^order) $
	   ELSE BEGIN	;Bandpass / reject,  avoid underflow in exp fcn
	     cutofftext ='Band Center at'
	     filter = (distfun * distfun - cutoff^2) / (distfun * bandwidth)
	     filter = exp(-(filter ^ (2 * order) < 25))
	     IF(bandtype EQ BANDREJECT) THEN filter = 1.0 - filter
	   ENDELSE
         END

  IDEAL: BEGIN				;Ideal filter
     	   WIDGET_CONTROL, ORDERSLIDER, SENSITIVE = 0
	   WIDGET_CONTROL, filtset(0), sensitive=1
	   WIDGET_CONTROL, filtset(1), sensitive=1
	   WIDGET_CONTROL, filtset(2), sensitive=1
	   WIDGET_CONTROL, filtset(3), sensitive=1
	   x = distfun /cutoff	
	   IF(bandtype EQ LOWPASS) THEN $	
	     filter = (x le 1) $
           ELSE IF(bandtype EQ HIGHPASS) THEN $
	     filter = 1.0 - (x le 1.) $
	   ELSE IF(bandtype EQ BANDREJECT) THEN $
	     filter = ((distfun lt (cutoff - bandwidth / 2.0)) or $
		     (distfun gt (cutoff + bandwidth / 2.0))) $
	   ELSE $
	     filter = ((distfun ge (cutoff - bandwidth / 2.0)) and $
		     (distfun le (cutoff + bandwidth / 2.0)))
	   cutofftext ='Cutoff at'
	   IF(bandtype EQ HIGHPASS or bandtype EQ BANDREJECT) then cutofftext ='Band center at'
	  END

  ELSE: MESSAGE, "resetfilter: Bad Filter Type"
ENDCASE

FOR i = 0, NVIEWS - 1 DO $			;since the filter has changed
  IF(view(i).viewtype GT 1) THEN resetview, $	;redraw all the windows that
		i, UPDATE_LABEL = NEW		;depend on the filter

WIDGET_CONTROL, waitlabel, $
		SET_VALUE = "   READY   "	;clear the wait flag

END
	;---------------- end of resetfilter ------------------


	;------------------------------------------------------
	;   procedure resetview
	;------------------------------------------------------
	; This function resets the view number passed in.  It 
	; is assumed that the structure associated with the 
	; passed in view has been altered in view type so it
	; needs to be redrawn.  In the case of one dimensional
	; data, there is no filter intensity image and cross -
	; sections of the filter are the same as filter plots
	; so they just call those routines.  Once the view has
	; been updated, the button that controls that view is
	; renamed to reflect the new view type.
	;------------------------------------------------------

pro resetview, windowindex, UPDATE_LABEL = UPDATE_LABEL

COMMON fourier, image, imagetrans, view, cutofflabel, bandwidthlabel,cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN,  HAMM, PARZ,EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS, NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs


wset, view(windowindex).windownum		;set to draw in passed in view
if(view(windowindex).viewtype ne 8) then $	;if not a saved view then 
  erase, color = 100				;erase the view

imagesize = size(image)

case view(windowindex).viewtype of 
		
		;Original Image Drawing
  0: begin 
       if(imagesize(0) GT 1) then begin
         tvscl, image, (VIEWSIZE - imagesize(1)) / 2, $
		(VIEWSIZE - imagesize(2)) / 2 
         if(keyword_set(UPDATE_LABEL)) then $
           widget_control, view(windowindex).controlid, $
				set_value = "Original Image View"
       endif else begin
         plot, image, /xst, /yst
         if(keyword_set(UPDATE_LABEL)) then $
           widget_control, view(windowindex).controlid, $
				set_value = "Original Signal Plot"
       endelse
     end

		;Image Spectrum Drawing
  1: begin 
       if(imagesize(0) GT 1) then $
         tvscl, shift(alog(abs(imagetrans)), $
		imagesize(1) / 2, imagesize(2) / 2),$
		(VIEWSIZE - imagesize(1)) / 2, $
		(VIEWSIZE - imagesize(2)) / 2 $
       else begin
         plot_io, shift(abs(imagetrans), imagesize(1)/2), /xst
       endelse
       if(keyword_set(UPDATE_LABEL)) then $
         widget_control, view(windowindex).controlid, $
				set_value = "Log Power Spectrum"
     end

		;Filter Plot Drawing
  2: begin 
       grid_size = 32
       if(imagesize(0) GT 1) then begin
         thinfilter = congrid(filter, grid_size, grid_size)
         surface, shift(thinfilter, grid_size/2, grid_size/2),$
		xstyle = 4, ystyle = 4, zstyle = 4
       endif else begin
         plot, shift(filter, imagesize(1) / 2), /xst
       endelse
       if(keyword_set(UPDATE_LABEL)) then $
         widget_control, view(windowindex).controlid, set_value = "Plot of Filter"
       empty
     end

		;Filter Intensity Drawing
  3: begin 
       if(imagesize(0) GT 1) then begin
         tvscl, shift(filter, imagesize(1) / 2, imagesize(2) / 2), $
		(VIEWSIZE - imagesize(1)) / 2, $
		(VIEWSIZE - imagesize(2)) / 2
         if(keyword_set(UPDATE_LABEL)) then $
           widget_control, view(windowindex).controlid, $
				set_value = "Filter Intensity Image"
       endif else begin
         view(windowindex).viewtype = 2
	 resetview, windowindex, UPDATE_LABEL = UPDATE_LABEL
       endelse
     end

		;Filter Cross Section Drawing
  4: begin 
      if(imagesize(0) GT 1) then begin
         plot, shift(filter(*, 0), imagesize(1) / 2), /xst
         if(keyword_set(UPDATE_LABEL)) then $
	   widget_control, view(windowindex).controlid, $
				set_value = "Filter Cross Section"
         empty
       endif else begin
	 view(windowindex).viewtype = 2
	 resetview, windowindex, UPDATE_LABEL = UPDATE_LABEL
       endelse
     end

		;Filter Shade Surface Drawing
  5: begin 
	grid_size = 48
       if(imagesize(0) GT 1) then begin
         thinfilter = congrid(filter, grid_size, grid_size)
         shade_surf, shift(thinfilter, grid_size/2, grid_size/2), $
		     xstyle = 4, ystyle = 4, zstyle = 4
       endif else begin
         plot, shift(filter, imagesize(1) / 2), /xst
       endelse
       if(keyword_set(UPDATE_LABEL)) then $
         widget_control, view(windowindex).controlid, $
				set_value = "Shaded Surface of Filter"
       empty
     end

		;Filtered Spectrum drawing
  6: begin 
       if(n_elements(filteredspectrum) le 1) then $
	 filteredspectrum = imagetrans * filter
       if(imagesize(0) GT 1) then $
         tvscl, shift(alog(abs(filteredspectrum)> 1e-10), $
		imagesize(1) / 2, imagesize(2) / 2), $
		(VIEWSIZE - imagesize(1)) / 2, $
		(VIEWSIZE - imagesize(2)) / 2 $
       else begin
         plot_io, shift(abs(filteredspectrum), imagesize(1)/2), $
		XSTYLE = 4, $
		YSTYLE = 4
         axis, imagesize(1) / 2, 0, /yaxis, /noerase, ycharsize = 0.001, $
		yticklen = .02
         axis, 0, 0, /xaxis, /noerase, xcharsize = .001, xticklen = .02
       endelse
       if(keyword_set(UPDATE_LABEL)) then $
         widget_control, view(windowindex).controlid, $
				set_value = "Log Filtered Power Spectrum"
     end

		;Filtered Image drawing
  7: begin 
       if(n_elements(filteredimage) le 1) then $
	 filteredimage = float(FFT(imagetrans * filter, 1))
       if(imagesize(0) GT 1) then begin
         tvscl, filteredimage, $
		(VIEWSIZE - imagesize(1)) / 2, $
		(VIEWSIZE - imagesize(2)) / 2
       if(keyword_set(UPDATE_LABEL)) then $
         widget_control, view(windowindex).controlid, $
				set_value = "Filtered Image"
       endif else begin
         plot, filteredimage
         if(keyword_set(UPDATE_LABEL)) then $
           widget_control, view(windowindex).controlid, $
				set_value = "Filtered Signal"
       endelse
     end

		;Saved View Drawing
   8: if(keyword_set(UPDATE_LABEL)) then $
        widget_control, view(windowindex).controlid, $
				set_value = "Saved View"

  else: widget_control, view(windowindex).controlid, $
				set_value = "Unsupported View"
endcase
end
	;------------------- end of resetview -----------------

	;------------------------------------------------------
	;   procedure four_event
	;------------------------------------------------------
	; This is the main event handler for the fourier_cti procedure.
	;------------------------------------------------------

pro four_event, event

common fourier, image, imagetrans, view, cutofflabel, bandwidthlabel,cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN, HAMM, PARZ, EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS, NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs

dimens = SIZE(image)

widget_control, get_uvalue = retval, event.id

	;check if view change was selected and if so, reset the affected view

  if(retval ge VIEWOPTBASE and retval lt viewopttop) then begin
    viewindex = fix((retval - VIEWOPTBASE)/NVIEWOPTS)
    setting = ((retval - VIEWOPTBASE) mod NVIEWOPTS)
    if(view(viewindex).viewtype ne setting) then begin
      view(viewindex).viewtype = setting
      resetview, viewindex, /UPDATE_LABEL
    endif

	;next see if the band type was changed and if so, update filter

  endif else if (retval ge FIRSTBANDTYPE and retval le LASTBANDTYPE) then begin
    if(event.select eq 1 and bandtype ne retval) then begin
      bandtype = retval
      resetfilter
    endif
    IF(bandtype NE BANDPASS) AND (bandtype NE BANDREJECT) THEN BEGIN
      WIDGET_CONTROL, bandwidthslider, SENSITIVE = 0
      WIDGET_CONTROL, bandwidthlabel, SENSITIVE = 0

    ENDIF else BEGIN
      WIDGET_CONTROL, cutofflabel, SET_VALUE = "Band Center at " + STRING(cutoff/dimens(1))
      WIDGET_CONTROL, bandwidthslider, /SENSITIVE
      WIDGET_CONTROL, bandwidthlabel, /SENSITIVE
    ENDELSE

	;reset the filter if the filter type was changed

  endif else if (retval ge FIRSTFILTER and retval le LASTFILTER) then begin
    if(event.select eq 1 and filtertype ne retval) then begin
      filtertype = retval
      resetfilter
    endif
  endif else case retval of

	;the other choices are unique so deal with each in the case statement

    HELP : XDisplayFile, FILEPATH("fourierdemo.txt", subdir='help'),	$
		TITLE = "Fourier_CTI Help", $
		GROUP = event.top, $
		WIDTH = 55, $
		HEIGHT = 16

    LOAD : begin					;load new data file
	     WIDGET_CONTROL, waitlabel, SET_VALUE = "   Loading data   "
	     widget_control, fourierbase, sensitive = 0
	     oldindex = index
	     oldfile = filename
	     index=1
	     loaddata,/NFILE
	     if(filename eq '') then begin
		filename = oldfile
		index = oldindex
		return
	     endif
	     widget_control, control_previous,sensitive=0
	     widget_control, control_next,/sensitive
	     msg1 = 'File               :'+ filename + ' contains '+ string(maxindex+1) + ' matrixes'
	     widget_control, matrixfile,set_value=msg1
	     msg2 = 'Specifications :'+ specs(index)
	     widget_control, matrixspecs,set_value=msg2
	     for i = 0, NVIEWS - 1 do $
	 	if(view(i).viewtype lt 2) then resetview, i
	     widget_control, fourierbase, /sensitive
	     resetfilter
	   end

     NEXT : begin
	     WIDGET_CONTROL, waitlabel, SET_VALUE = "   Loading data   "
	     index=index+1
	     loaddata
	     if(index eq maxindex) then widget_control, control_next,sensitive=0
	     if(index eq 1) then widget_control, control_previous,/sensitive
	     msg2 = 'Specifications :'+specs(index)
	     widget_control, matrixspecs,set_value=msg2
	     for i = 0, NVIEWS - 1 do $
	 	if(view(i).viewtype lt 2) then resetview, i
	     widget_control, fourierbase, /sensitive
	     resetfilter
	   end

    PREVIOUS : begin
	     WIDGET_CONTROL, waitlabel, SET_VALUE = "   Loading data   "
	     index=index-1
	     loaddata
	     if(index eq maxindex-1) then widget_control, control_next,/sensitive
	     if(index eq 0) then widget_control, control_previous,sensitive=0
	     msg2 = 'Specifications :'+specs(index)
	     widget_control, matrixspecs,set_value=msg2
	     for i = 0, NVIEWS - 1 do $
	 	if(view(i).viewtype lt 2) then resetview, i
	     widget_control, fourierbase, /sensitive
	     resetfilter
	   end

    SETMATRIX: begin
	     previndex = index
	     index=event.index
	     if (index eq previndex) then return
	     WIDGET_CONTROL, waitlabel, SET_VALUE = "   Loading data   "
	     loaddata
	     widget_control, control_next,sensitive=1
	     widget_control, control_previous,sensitive=1
	     if(index eq maxindex) then widget_control, control_next,sensitive=0
	     if(index eq 0) then widget_control, control_previous,sensitive=0
	     msg2 = 'Specifications :'+specs(index)
	     widget_control, matrixspecs,set_value=msg2
	     for i = 0, NVIEWS - 1 do $
	 	if(view(i).viewtype lt 2) then resetview, i
	     widget_control, fourierbase, /sensitive
	     resetfilter
	     end

    COLADJUST : xloadct, /SILENT, GROUP = event.top	;adjust the colors

    FILTERORDER : begin
		  widget_control, ORDERSLIDER, get_value = temp
		  if(temp ne order) then begin
		    order = temp
		    resetfilter
		  endif
  		end

    FILTERCUTOFF : begin		
		  widget_control, cutoffslider, get_value = temp
		  cutoff = temp/20. * (dimens(1) / 2.)
  		  WIDGET_CONTROL, cutofflabel, SET_VALUE = cutofftext + STRING(cutoff/dimens(1))
 		  resetfilter
  		end

    FILTERBANDWIDTH : begin		
			if((bandtype eq BANDPASS) or (bandtype eq BANDREJECT)) then begin
		          widget_control, bandwidthslider, get_value = temp
			  bandwidth = temp/20. * (dimens(1) / 4.)
			  WIDGET_CONTROL, bandwidthlabel, $
			  SET_VALUE = "Bandwidth is " + STRING(bandwidth/dimens(1))
			  resetfilter
		  	endif
  		      end

    EXIT : begin
 	     widget_control, fourierbase, /destroy  ;free up the widgets used
	     !X.margin = oldxmargins		    ;restore previous margins
	     !Y.margin = oldymargins
	   end
    else: 
  endcase
end


	;------------------------------------------------------
	;   procedure fourier_cti
	;------------------------------------------------------
	; This is the main procedure for fourier cti.
	;------------------------------------------------------

PRO fourier_cti, GROUP = GROUP

COMMON fourier, image, imagetrans, view, cutofflabel, bandwidthlabel,cutofftext, $
	        VIEWSIZE, order, cutoff, bandwidth, filter, filteredimage, filtset, $
		filteredspectrum, NVIEWS, fourierbase, waitlabel, bandtype, $
		BUTTER, HANN, HAMM, PARZ, EXPON, IDEAL, LOWPASS, HIGHPASS, BANDPASS, $
		BANDREJECT, cutoffslider, bandwidthslider, FILTERCUTOFF, SETMATRIX, $
		FILTERBANDWIDTH, oldxmargins, oldymargins, VIEWOPTBASE, $
		VIEWOPTTOP, NVIEWOPTS, FIRSTBANDTYPE, LASTBANDTYPE, $
		FIRSTFILTER, LASTFILTER, HELP, LOAD, COLADJUST, FILTERORDER, $
		FILTERTYPE, ORDERSLIDER, EXIT, sliderbase, statusbase, $
		PREVIOUS, NEXT, matrixfile,matrixspecs,matlist

common matrix,	control_next,control_previous, index, maxindex
common filinfo, mats, filename,ntot,specs

IF (XRegistered("fourier_cti") NE 0) THEN GOTO, FINISH

oldxmargins = !X.MARGIN
oldymargins = !Y.MARGIN
!X.MARGIN = [6, 2]
!Y.MARGIN = [2, 2]

filteredimage = 0				;initialization
filteredspectrum = 0
waitlabel = 0

HELP		= 0				;constants for user choices
LOAD		= 1
FILTERORDER 	= 2
FILTERCUTOFF	= 3
FILTERBANDWIDTH	= 4
COLADJUST	= 5	
EXIT		= 6
PREVIOUS	= 100
NEXT		= 200
SETMATRIX	= 300


FIRSTBANDTYPE	= 7				;constants for the types of
  LOWPASS	= 7				;filtering passes
  HIGHPASS	= 8
  BANDPASS	= 9
  BANDREJECT	= 10
LASTBANDTYPE	= 10

FIRSTFILTER	= 11				;constants for the different
  BUTTER	= 11				;filter types
  EXPON		= 12
  IDEAL		= 13
  HANN		= 14
  HAMM		= 15
  PARZ		= 16
LASTFILTER	= 16

ORDER = 2					;default filter order, type
FILTERTYPE = HANN				;pass and width
cutoff = .5
bandwidth = .25
bandtype = LOWPASS

DEVICE, GET_SCREEN = screendims			;determine the device size in
						;pixels
IF(screendims(0) LT 1152) THEN $
  VIEWSIZE = 200 $				;number of views and view
ELSE $						;size in pixels (device)
  VIEWSIZE = 256
NVIEWS = 3	

loaddata, /NEW, /NFILE			;load in the initial data
if(filename eq '') then return
IF(NOT(KEYWORD_SET(image))) THEN GOTO, FINISH	;if no data was selected, drop
						;out of routine

viewinstance = {vinst, 	viewtype:0b, 	$	;structure for each view
			controlid:0L, 	$
			windowid:0L, 	$
			windownum:0}

view = REPLICATE(viewinstance, NVIEWS)		;array of view structures

view(0).viewtype = 0				;the default view settings
IF(NVIEWS GE 2) THEN view(1).viewtype = 4	;that are used for differing
IF(NVIEWS GE 3) THEN view(2).viewtype = 7	;numbers of views
IF(NVIEWS GE 4) THEN view(3).viewtype = 1
IF(NVIEWS GE 5) THEN view(4).viewtype = 6
IF(NVIEWS GE 6) THEN view(5).viewtype = 0

instance = {vopt, viewname:'', $
		  viewid:0}			;structures describing each
NVIEWOPTS = 9					;type of view are kept in 
VIEWOPTBASE = 20				;an array and they have user
viewopttop = VIEWOPTBASE + (NVIEWOPTS*NVIEWS)	;values ranging from viewbase
viewoptions = REPLICATE(instance, NVIEWOPTS)	;to NVIEWS * NVIEWOPTS

viewoptions(0).viewname = "Original Signal"	
viewoptions(0).viewid = 0
viewoptions(1).viewname = "Log Power Spectrum"
viewoptions(1).viewid = 1
viewoptions(2).viewname = "Filter Plot"
viewoptions(2).viewid = 2
viewoptions(3).viewname = "Filter Intensity Image"
viewoptions(3).viewid = 3
viewoptions(4).viewname = "Filter Cross Section"
viewoptions(4).viewid = 4
viewoptions(5).viewname = "Shaded Surface of Filter"
viewoptions(5).viewid = 5
viewoptions(6).viewname = "Log Filtered Power Spectrum"
viewoptions(6).viewid = 6
viewoptions(7).viewname = "Filtered Signal"
viewoptions(7).viewid = 7
viewoptions(8).viewname = "Save View"
viewoptions(8).viewid = 8

fourierbase = WIDGET_BASE(TITLE = "Fourier Filtering for CTI images", $
		/COLUMN, $
		XPAD =  5, $
		YPAD = 5, $
		SPACE = 5)

fouriercontrol = WIDGET_BASE(fourierbase, /ROW)
control_exit = WIDGET_BUTTON(fouriercontrol, $
		VALUE = "Exit", $
		UVALUE = EXIT)
control_data = WIDGET_BUTTON(fouriercontrol, $
		VALUE = "Load New Data", $
		UVALUE = LOAD)
control_previous = WIDGET_BUTTON(fouriercontrol, $
		VALUE = "<<", $
		UVALUE = PREVIOUS)
control_next = WIDGET_BUTTON(fouriercontrol, $
		VALUE = ">>", $
		UVALUE = NEXT)
control_color = WIDGET_BUTTON(fouriercontrol, $
		VALUE = "Adjust Color Table", $
		UVALUE = COLADJUST)
control_help = WIDGET_BUTTON(fouriercontrol, $
		VALUE = "Help", $
		UVALUE = HELP)

matrixbase = WIDGET_BASE(fourierbase, /COLUMN,/FRAME,XPAD = 5, SPACE = 5)
matrixfile =  WIDGET_TEXT(matrixbase, VALUE = 'File               :', XSIZE=80, YSIZE = 1)
matrixspecs = WIDGET_TEXT(matrixbase, VALUE = 'Specifications :', XSIZE=80, YSIZE = 1)


;------------------ the view windows are defined ---------------------

IF(!VERSION.OS EQ 'vms') THEN $			;account for differences
  initialname = "Log Filtered Power Spectrum" $	;between fonts with vms
ELSE $						;and the rest of the world
  initialname = "                          "

viewbase = WIDGET_BASE(fourierbase, $
		/ROW, $
		SPACE = 5)

FOR viewindex = 0, NVIEWS - 1 DO BEGIN
  tempbase = WIDGET_BASE(viewbase, /FRAME, $
		/COLUMN)
  view(viewindex).controlid = WIDGET_BUTTON(tempbase, $
		VALUE = initialname, $
		MENU = 2)
  view(viewindex).windowid = WIDGET_DRAW(tempbase, $
		XSIZE = VIEWSIZE, $
		YSIZE = VIEWSIZE, $
		RETAIN = 2)
  FOR i = 0, NVIEWOPTS - 1 DO BEGIN
     dummy = WIDGET_BUTTON(view(viewindex).controlid, $
		VALUE = viewoptions(i).viewname, $
		UVALUE = viewoptions(i).viewid + $ 
			 VIEWOPTBASE + (NVIEWOPTS*viewindex))  
  ENDFOR
ENDFOR

;----------------- the filter controls are defined ------------------

controlbase = WIDGET_BASE(fourierbase, $
		/ROW, $
		SPACE = 5)

funtypes = ["Butterworth", "Hann", "Hamming","Parzen","Exponential", "Ideal"]
funusers = [BUTTER, HANN, HAMM, PARZ, EXPON, IDEAL]
XMenu, funtypes, $
		controlbase, $
		UVALUE = funusers, $
		/EXCLUSIVE, $
		/COLUMN, $
		BUTTONS = bandset, $
		TITLE = "Filter Function"

;---------------- the bandpass controls are defined -----------------

filttypes = ["Low Pass", "High Pass", "Band Pass", "Band Reject"]
filtusers = [LOWPASS, HIGHPASS, BANDPASS, BANDREJECT]
XMenu, filttypes, $
		controlbase, $
		UVALUE = filtusers, $
		/EXCLUSIVE, $
		/COLUMN, $
		BUTTONS = filtset, $
		TITLE = "Filter Type"

;-------------------- the order control is defined -------------------

orderbase = WIDGET_BASE(controlbase, $
		/COLUMN)
orderlabel = WIDGET_LABEL(orderbase, $
		VALUE = "Filter Order")
ORDERSLIDER = WIDGET_SLIDER(orderbase, $
		YSIZE = 140, $
		/VERTICAL, $
		VALUE = order, $
		MINIMUM = 1, $
		MAXIMUM = 10, $
		UVALUE = FILTERORDER)

;--------- the sliders for filter width and cutoff are defined -------

sliderbase = WIDGET_BASE(controlbase, $
		/COLUMN)

setsliders, /NEW				;builds the cutoff and 
						;bandwidth liders

;--------------- the computation status label is defined --------------

statusbase = WIDGET_BASE(controlbase, $
		/FRAME, $
		/COLUMN, $
		XPAD = 5, $
		SPACE = 5)
statuslabel = WIDGET_LABEL(statusbase, $
		VALUE = "Status")
waitlabel = WIDGET_LABEL(statusbase, $
		VALUE = "PLEASE WAIT")

WIDGET_CONTROL, fourierbase, /REALIZE		;register all the widget things
						;with Xwindows

widget_control, control_previous,sensitive=0
msg1 = 'File               : '+ filename + ' contains '+ string(maxindex+1) + ' matrixes'
widget_control, matrixfile,set_value=msg1
msg2 = 'Specifications :'+specs(index)
widget_control, matrixspecs,set_value=msg2

WIDGET_CONTROL, bandset(1), /SET_BUTTON		; Hann
WIDGET_CONTROL, filtset(0), /SET_BUTTON		; Low pass

FOR i = 0, NVIEWS - 1 DO BEGIN			;now that they exist, get the
  WIDGET_CONTROL, view(i).windowid, $		;drawing area numbers and set
		GET_VALUE = temp		;the view structure array so 
  view(i).windownum = temp			;these values can be referenced
ENDFOR						;when drawing

						;display wait while seting up

resetview, 0, /UPDATE_LABEL			;draws the default views that
;resetview, 1, /UPDATE_LABEL			;don't rely on the filter and
resetfilter, /NEW				;then reset the filter which
						;automatically draws the 
						;filter's dependent views

WIDGET_CONTROL, waitlabel, $
		SET_VALUE = "   READY   "	;remove wait, ready to go!


XManager, "fouriercti", $
		fourierbase, $
		EVENT_HANDLER = "four_event", $
	  	GROUP_LEADER = GROUP

FINISH:

END
	;================ procedure fourier_cti ====================

