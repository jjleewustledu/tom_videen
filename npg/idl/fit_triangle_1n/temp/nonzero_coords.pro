; $Id: nonzero_coords.pro,v 1.1 1996/01/08 15:20:13 tom Exp $
;
;+
; NAME:
;	NONZERO_COORDS
;
; PURPOSE:
;	Return coordinates x,y,z, of all points in a 3-D array which are non-zero.
;
; CATEGORY:
;	Image processing.
;
; CALLING SEQUENCE:
;	NONZERO_COORDS, Data, Points
;
; INPUTS:
;	Data:	3-dimensional array.
;
; OUTPUTS:
;	Points:	Coordinates of points (x,y,z) and the value of Data at that coord in a 2-D array.
;
; PROCEDURE:
;	For all elements of Data, if value is > 0, include the coordinates in Points.
;	Note that array data (x,y,z) is row major; x varies fastest.
;
; EXAMPLE:
;	NONZERO_COORDS, Data, Points
;
; MODIFICATION HISTORY:
; 	Written by:	Tom Videen, 08-Nov-94
;-

pro nonzero_coords, data, points
	sz = size(data)
	if (sz(0) ne 3) then begin
		print, "Error in NONZERO_COORDS.  Array dimension must be 3"
		return
	endif
	offset = where (data gt 0, count)
	print,count, " points"
	points = dblarr(4, count)
	slicesize = sz(1) * sz(2)
	rowsize = sz(1)
	for n = 0, count - 1 do begin
		index = offset(n)
		x0 = (index mod slicesize) mod rowsize
		y0 = (index mod slicesize) / rowsize
		z0 = index / slicesize
		points(0,n) = x0
		points(1,n) = y0
		points(2,n) = z0
		points(3,n) = data(x0, y0, z0)
	endfor
	return
end
