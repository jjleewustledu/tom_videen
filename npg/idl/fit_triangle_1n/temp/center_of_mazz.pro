; $Id: center_of_mazz.pro,v 1.1 1996/01/08 15:20:13 tom Exp $
;
;+
; NAME:
;	CENTER_OF_MAZZ
;
; PURPOSE:
;	Return array with only a single coordinate for each Plane.
;	The coordinate is the center of mass in the Plane and the value is the
;	sum of all input points in that plane.
;	Unlike center_of_mass, this routine uses ALL points.
;
; CATEGORY:
;	Image processing.
;
; CALLING SEQUENCE:
;	CENTER_OF_MAZZ, Points, Plane
;
; INPUTS:
;	Points:	Coordinates of points and their values.
;	Plane:	Dimension of Points to collapse.
;
; OUTPUTS:
;	COM_points:	Coordinates of center-of-mass points and their values.
;
; FUNCTION:
;	Create histogram of all points by plane number.
;	Stepping through planes:
;		1) set x and y equal mean values for plane z,
;		2) set z equal to plane
;		3) set val equal to sum of values for all points in this plane
;		4) concatenate this (4,1) array to com_points
;
; EXAMPLE:
;	CENTER_OF_MAZZ, Points, Plane
;
; MODIFICATION HISTORY:
; 	Written by:	Tom Videen, 05-Dec-94
;-

function center_of_mazz, points, plane

	hist = histogram(points(plane, *), REVERSE_INDICES = r, omin=pmin, omax=pmax)
	n = (size(hist))(1)
	first = 0
	last = n-1

	for j = first, last do begin
		if (r(j) ne r(j+1)) then begin
			for i = 0, 2 do begin
				if (i ne plane) then $
					val = double(total(points(i, r(r(j):r(j+1)-1)))) / double(hist(j)) $
				else $
					val = double(points(plane, r(r(j))))
				if (i eq 0) then arr = val else arr = [arr, val]
			endfor
			arr = [arr, total(points(3, r(r(j):r(j+1)-1)))]
			if (j eq first) then com_points = arr else com_points = [[com_points], [arr]]
		endif
	endfor
	return, com_points
end
