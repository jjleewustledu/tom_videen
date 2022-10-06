; $Id: fiducial_transform2.pro,v 1.4 2008/11/17 21:58:19 tom Exp $
;
;+
; NAME:
;	FIDUCIAL_ANGLES
;
; PURPOSE:
;	Compute 3 angles from fiducial points.
;	These are passed to manualreslice to create a new image with principle image axes in x,y,z planes.
;
; MODIFICATION HISTORY:
;	Written by: Tom Videen, Mar 2009.
;-

PRO FIDUCIAL_ANGLES

	on_error,2
	fidptsfile = getenv('FIDPTS')

	str = ''
	str1 = ''
	x = 0.
	y = 0.
	z = 0.
	std = fltarr(4, 3)
	fidpts = fltarr(14, 3)

	;; FIDUCIAL POINTS
	;; Read reslice image fiducial points
	;; ----------------------------------
	openr,lu,fidptsfile,/get_lun
	readf,lu,format='(A)',str1
	readf,lu,format='(A)',str
	x = 0.
	y = 0.
	z = 0.
	for i=0,13 do begin
		readf,lu,format='(A3,3F8)',str,x,y,z
		fidpts[i,0] = x
		fidpts[i,1] = y
		fidpts[i,2] = z
	endfor
	close,lu
	free_lun,lu

	;; AC = (xac, yac, zac)	anterior commissure
	xac = fidpts[0,0]
	yac = fidpts[0,1]
	zac = fidpts[0,2]

	;; PC = (xpc, ypc, zpc)	posterior commissure
	xpc = fidpts[1,0]
	ypc = fidpts[1,1]
	zpc = fidpts[1,2]

	;; IPF = (xipf, yipf, zipf)	interpeduncular fissure
	xipf = fidpts[13,0]
	yipf = fidpts[13,1]
	zipf = fidpts[13,2]

	;; Compute 3 angles of rotation (ang_x = pitch; ang_y = roll; ang_z = yaw)
	;; all angles are presumed sufficiently small to compute from 2D projects
	;; ----------------------------------------------------------------------
	dxc = xpc - xac
	dyc = ypc - yac
	dzc = zpc - zac
	dxi = xipf - xac
	dzi = zipf - zac

	ang_x = ATAN(dzc/dyc)
	ang_y = ATAN(dxi/dzi)
	ang_z = ATAN(dxc/dyc)
	print, format='("dxc =",F6.1)', dxc
	print, format='("dyc =",F6.1)', dyc
	print, format='("dzc =",F6.1)', dzc
	print, format='("dxi =",F6.1)', dxi
	print, format='("dzi =",F6.1)', dzi

	print, format='("tanx =",F7.3)', dzc/dyc
	print, format='("tany =",F7.3)', dxi/dzi
	print, format='("tanz =",F7.3)', dxc/dyc

	Rad_to_Deg = 180./3.14159
	print, "Angles X Y Z"
	print, ang_x*Rad_to_Deg
	print, ang_y*Rad_to_Deg
	print, ang_z*Rad_to_Deg

END
