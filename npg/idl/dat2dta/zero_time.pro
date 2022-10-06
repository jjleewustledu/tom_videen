; $Id: zero_time.pro,v 1.6 2003/01/03 22:40:28 tom Exp $
;
;+
; NAME:
;   ZERO_TIME
;
; PURPOSE:
;   Shift curve to assign time=0.
;
; CATEGORY:
;   Metabolic processing.
;
; CALLING SEQUENCE:
;   zero_time, curve, label
;
; INPUTS:
;   curve:		time-activity curve with time = curve(0,*) and activity = curve(1,*)
;	label:		name of curve
;
; OUTPUTS:
;   curve:		shifted time-activity curve
;
; MODIFICATION HISTORY:
;   Written by: Tom Videen, Dec. 1995
;-

pro zero_time, curve, label, time0, frame

	a = ''
	if (label eq 'Head') then color = 9 $
	else if (label eq 'Artery') then color = 8 $
	else color = 200
	t0 = 0
	t1 = t0
	if (label eq 'Head') then t1 = time0
	indx = where (curve(1,*) gt 0)
	tlast = max(curve(0,indx))
	tmax = max(curve(0,*))
	time = curve(0,*)

l1:	plot, time, curve(1,*), /nodata, charsize=1.5, xrange=[t0,tmax]
	oplot, time, curve(1,*), color=color, thick=2
	xyouts, 0.8, 0.85, label, /normal, color=color, size=2.0

	str = 'Frame ' + strtrim(string(frame),2) + ' relative to zero time'
	if (label eq 'Head') then getnum, str, t1, -10000, 10000 $
	else getnum, 'Zero time', t1, -10000, 10000
	if	(t1 ne t0) then begin
		t0 = t1
		time = curve(0,*) + t0
		tmax = max(time)
		print,'Enter time again to confirm.'
		goto, l1
	endif
	curve(0,*) = time
	indx = where (time gt 0)
	curve = curve(*,indx)
end
