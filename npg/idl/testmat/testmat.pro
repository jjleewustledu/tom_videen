PRO TESTMAT

   on_error,2
   fidptsfile = getenv('FIDPTS')
   refptsfile = getenv('STDPTS')
   airfile = getenv('AIRFILE')

	str = ''
	x = 0.
	y = 0.
	z = 0.
	fidnam = strarr(13)
	fidpts = fltarr(13, 3)
	refnam = strarr(13)
	refpts = fltarr(13, 3)

	;; Read fiducial points

    openr,lu,fidptsfile,/get_lun
    readf,lu,format='(A)',str
    print,str
    readf,lu,format='(A)',str
	for i=0,12 do begin
		readf,lu,format='(A3,3F8)',str,x,y,z
		fidnam[i] = str
		fidpts[i,0] = x
		fidpts[i,1] = y
		fidpts[i,2] = z
	endfor
    close,lu
	free_lun,lu
;	for i=0,12 do begin
;		print,format='(I2,2X,A3,F6.1,F6.1,F6.1)',i+1,fidnam[i],fidpts[i,*]
;	endfor

	AFID = [[fidpts[0,0],fidpts[1,0],fidpts[2,0],fidpts[9,0]], $
			[fidpts[0,1],fidpts[1,1],fidpts[2,1],fidpts[9,1]], $
			[fidpts[0,2],fidpts[1,2],fidpts[2,2],fidpts[9,2]], $
			[1,1,1,1]]
	print,AFID
	xdim = 256
	ydim = 256
	zdim = 128
	FIDOFF = [[xdim, xdim, xdim, xdim],[ydim, ydim, ydim, ydim],[zdim, zdim, zdim, zdim],[2,2,2,2]]
	OFID = FIDOFF - AFID
	;print,"with Analyze offset"
	;print,OFID

	;; Read standard reference points

    openr,lu,refptsfile,/get_lun
    readf,lu,format='(A)',str
    ;print,str
    readf,lu,format='(A)',str
	for i=0,12 do begin
		readf,lu,format='(A3,3F8)',str,x,y,z
		refnam[i] = str
		refpts[i,0] = x
		refpts[i,1] = y
		refpts[i,2] = z
	endfor
    close,lu
	free_lun,lu
;	for i=0,12 do begin
;		print,format='(I2,2X,A3,F6.1,F6.1,F6.1)',i+1,refnam[i],refpts[i,*]
;	endfor

	AREF = [[refpts[0,0],refpts[1,0],refpts[2,0],refpts[9,0]], $
			[refpts[0,1],refpts[1,1],refpts[2,1],refpts[9,1]], $
			[refpts[0,2],refpts[1,2],refpts[2,2],refpts[9,2]], $
			[1,1,1,1]]
	;print,AREF
	xdim = 1120
	ydim = 39
	zdim = 936
	REFOFF = [[xdim, xdim, xdim, xdim],[ydim, ydim, ydim, ydim],[zdim, zdim, zdim, zdim],[2,2,2,2]]
	AREF = REFOFF - AREF
	;print,"with Analyze offset"
	;print,AREF

	;; Read AIR file

   a = {air}
   openr,lu,airfile,/get_lun
   readu,lu,a
   close,lu
   free_lun,lu

	minvox = MIN([a.std.x_size, a.std.y_size, a.std.z_size])
	sxoom = a.std.x_size/minvox
	syoom = a.std.y_size/minvox
	szoom = a.std.z_size/minvox
	SZOOM = [[sxoom,0,0,0],[0,syoom,0,0],[0,0,szoom,0],[0,0,0,1]]

	; reverse col/rows
	MAT = ROTATE(a.e, 4)
	;print,'MAT'
	;print,MAT
    ATRANS = MAT ## SZOOM
	;print,'Scaled MAT'
	;print,ATRANS

	ACOMP = ATRANS ## AREF
	;print,'Computed Points'
	;print,ACOMP
	
	xdim = 256
	ydim = 256
	zdim = 128
	FIDOFF = [[xdim, xdim, xdim, xdim],[ydim, ydim, ydim, ydim],[zdim, zdim, zdim, zdim],[2,2,2,2]]
	ACOMP = FIDOFF - ACOMP
	print,"Conventional offset"
	print,ACOMP

END
