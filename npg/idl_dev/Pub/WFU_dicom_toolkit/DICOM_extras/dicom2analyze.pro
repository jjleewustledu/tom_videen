;-----------------------------------------------------------------------------------------------
;This suite of programs will allow you to take
;the images transferred over the net (ie, dicom ftp, or dicom_ximgconv)
;and create locs for normalization using matlab
;This should also work with any directory of images, regardless of header type
;as long as they are square
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;The best way to run this on the images that come from the PACS archive is as follows:

; 			dicom_ximgconv,dir = dicompath,examdir = examdir
;			dicom_locsort,path = dicompath
;
;   (where dicompath is the path of DICOM.tar.z file   (straight from the PACS DICOM ftper)
;The output is csmemp.loc type files
;you can run the autonorm routine directly on these files to perform spm normalization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 

;------------------------------------------------------------------------------------
;some of the routines used here:
;    dicom2loc,outdir = outdir, path = path
;		where outdir is where you want to write the files
;		and path = the path where the individual series containing
;		the images are located (note:not the actual directory of images, but 1 up) 
;to normalize:
;	dicom2talloc,outdir = outdir
;		where outdir is the directory where the locs you created above are located
;		the program will find the T1 file, convert it to analyze format
;		and invoke spmsn3d to normalize it
;		once this is done, any of the loc files in this directory
;		can be used with the fmri viewer (as long as they stay together in a subdirectory)
;notabente:  don't be fooled by the name, these programs will not read dicom images directly (ie 12 bit)
;		they are for the images transferred using HUP's dicom ftper
;		JAM 10-8-98
;		true dicom support to be added shortly
;-----------------------------------------------------------------------------------------------




function dicom2cube,path = path,image = image
;--------------------------------------------------------------------
;converts a directory of square images to a stack, regardless of header type
;--------------------------------------------------------------------
	
	if not(keyword_set(path)) then begin
		filename = pickfile(title = 'Choose an image file')
		extract_pname,filename,path
	endif
	flist = make_filelist(filecount = fcount,path = path)
	image = open_square_image(filename = path + flist(fcount/2))
	s = size(image)
	stack = intarr(s(1),s(2),fcount)
	;Don't rotate since we will create a csmemp type file
	for i = 0,fcount - 1 do stack(*,*,i) = open_square_image(filename = path + flist(i))
;------------------------------------------------------
;Quick fix for group_length problem with first pixel
;image is rotated relative to stack
;------------------------------------------------------
	image(0,0) = 0
	stack(0,0,*) = 0
	;stack(0,s(2) - 1,*) = 0
	return,stack
end




pro dicom_series2loc,path = path,fn = fn,outdir = outdir

;------------------------------------------------------------------------------------------
;the meat of these routines
;takes a stack and creates a loc file with a locheader
;note that some critical image header info must be supplied by the user (ie plane, and image type (T1,T2,etc)
;since this suite assumes we can't read the image headers directly
;---------------------------------------------------------------------------------------
	stack = fix(dicom2cube(path = path,image = image))
	tvscl,rotate(image,7)
	image_type = ok2('Image Type?','   T1   ','   T2   ','    FLAIR  ', '  OTHER  ',/bell)
	image_plane = ok2('Image Plane?','   Axial   ','   Sagittal   ', '   Coronal   ',/bell)
	
	CASE image_type of
	 	1: begin
			sequence = 'T1'
			tr = 600
			te = 14
		    end
		2: begin
			sequence = 'T2'
			tr = 2000.0
			te = 100.0
		    end
		3: begin
			sequence = 'FLAIR'
			tr = 2000.0
			te = 100.0
		   end
		4: begin
			sequence = 'OTHER'
			tr = 2000.0
			te = 100.0
		   end		
	ELSE:	begin
		sequence = 'T2'
		tr = 2000.0
		te = 200.0
		end	
	ENDCASE


	CASE image_plane of
	 	1: begin
			plane = 2
			plane_name = '     axial'
		end
		2:  begin
			plane = 4
			plane_name = '  sagittal'	
		    end
		3:  begin
			plane = 6
			plane_name = '   coronal'
		end
	ELSE: Begin
			plane = 4
			plane_name = ' who knows'
		end
	ENDCASE
	
	template = findfile(path_of('my_templates') + '*.loc')
	th = read_header(filename = template(0),/nocompute)
	;-----------------------------------------
	;construct and load analyze header
	;-----------------------------------------
	;orig = [69,109,64,0]
	msize = size(stack)
	fov = 240.0			;assumption
	an = make_analyze_header()
	;an.regular = 'n'			;default r for radiologic, ?n
	an.z_size 	= 2.0	
	an.x_dim	= msize(1)
	an.y_dim 	= msize(2)
	an.z_dim 	= msize(3)

	an.x_size	= fov/msize(1)
	an.y_size	= fov/msize(2)
	an.z_size 	= 5.0			;big assumption
	if msize(3) gt 40 then an.z_size = 3.0	;?thins
	if msize(3) gt 80 then an.z_size = 1.5	;?SMN

	an.t_dim = 1
	an.dims	= 4
	datatype = 4			;integer
	an.datatype	= datatype
	case datatype of
		 2:an.glmax	= 255
		 4:an.glmax	= 32767
		8:an.glmax	= (2L^31)-1
		16:an.glmax	= 1 ; because glmax is a long integer, not a float
		64:an.glmax	= 1
	endcase
	an.glmin = 0
	;an.bits  = 8 * bytesize(idl_datatype(datatype))	;16 for integer
	an.bits = 16

	;-------------------------------------------------
	;swap the new analyze header for the old one
	;-------------------------------------------------
	th = swap_analyze_header(th,an)
	th.x	= msize(1)
	th.y	= msize(2)
	th.z	= msize(3)
	th.magnet_transform = an.magnet_transform
	th.landmark =float(long(1000.0*(randomu(seed))) > 2)
	th.tr = 2000.0
	th.te = 200.0

	th.tr = tr
	th.te = te
	th.plane_name = plane_name
	th.plane = plane
	f_ext = '.locs'
	outfilename = outdir + sequence + '_' + strtrim(plane_name,2) + $
			'_' + fn + '_' + strtrim(string(msize(3)),2)
	;---------------------------------------------------------
	;Creates a .locs type file (labheader)
	;--------------------------------------------------------
	;write_header, filename=outfilename + f_ext, header=th, cube=stack
	;---------------------------------------------------------------
	;create a csmemp.loc type file with a real genesis header
	;---------------------------------------------------------------
	pix = [an.x_size,an.y_size,an.z_size]
	k = map2loc(stack,outfile = outfilename,pix = pix,/nopicktal,lh = th)

	;if keyword_set(make_analyze) then begin
	;	hdrname = outdir + fn + '.hdr'
	;	imgname = outdir + fn + '.img'
	;	openw,/swap_if_little_endian,unit,hdrname,/get_lun				
	;	writeu,unit,an			
	;	close_lun, unit
	;	openw,/swap_if_little_endian, unit,imgname, /get_lun				
	;	writeu,unit,stack
	;	close_lun,unit
	;endif
end





pro dicom2loc,path = path,outdir = outdir
;--------------------------------------------------------------------------------------------------
;the meat of these routines
;takes a stack and creates a loc file with a locheader
;note that some critical image header info must be supplied by the user (ie plane, and image type (T1,T2,etc)
;since this suite assumes we can't read the image headers directly
;this program will cycle through each series subdirectory and create a corresponding .locs file
;--------------------------------------------------------------------------------------------------
cd,current = here
;xloadct
error = 1
if keyword_set(outdir) then cd,outdir
if keyword_set(path) then cd,path
if not(keyword_set(path)) then path = pickdir(title = 'Choose directory of series')
repeatx = 0
while error do begin
	if repeatx gt 0 then path = pickdir(title = 'Choose directory of series')
	cd,path
	subdirs = find_subdir(path = path,error = error)
	txt1 = 'Bad path!'
	txt2 = 'Choose SERIES directory'
	txt3 = 'It should list 001 002 003 etc..'
	txt4 = 'Try Again'
	txt = [txt1,txt2,txt3,txt4]
	if error then badboy,txt
	repeatx = repeatx + 1
endwhile
	if not(keyword_set(outdir)) then outdir = pickdir(title = 'Choose output directory')
	extract_pname,outdir,outdir,parents = parents
	extract_pname,path,path,parents = parents

	fn = 'dc:' + parents(n_elements(parents) - 2) + ':' 
	
	;xloadct
	for i = 0,n_elements(subdirs) - 1 do begin
		;print, outdir
		;print,subdirs(i)
		print,'Creating locfile for ' + path +  fn + subdirs(i)
		dicom_series2loc,path = path + subdirs(i) + '/', fn = fn + subdirs(i),outdir = outdir
	endfor

cd,here
end
	
			


pro run_dicom2talloc,outdir = outdir

;---------------------------------------------------------------------------------
;this program will find the T1 file and normalize it
;note that the files don't have to be created using the above routines
;this program will work with any .loc or .locs directories
;outdir = the directory containing the .locs to be normalized
;the .sn3dmat file and normalized n*.loc will be written to the same directory
;------------------------------------------------------------------------------------

	if not(keyword_set(outdir)) then outdir = pickdir(title = 'Choose directory of locs for normalization')		
	loclist = findfile(outdir + '*.locs',count = count)
	locfile = find_t1(loclist = loclist,sequence_type = sequence_type,tal = search4talloc)
	locs2analyze,filename = locfile,outfilename = analyzelocfile,header = loch
	ax_loc = locfile
	loch = read_header(filename = ax_loc,/nocompute)
	sequence_type = 'T' + strtrim(string(sequence_type),2)
	SMN = 0
	testing = 0
	noplastic = 1
	repair = 0
	nonormalize = 0
	startim = 0
	noprompt = 1
	make_talloc = 1

	analyzelocfile = analyzelocfile + '.hdr'


;----------------------------------------------------
;You must normalize T1's to talairach now using matlab
;---------------------------------------------------_
;print,'performing talairach normalization...'
if not(keyword_set(SMN)) and not(keyword_set(testing)) then begin
	p = change_ext(analyzelocfile,'.img')
	print, p
	params = fltarr(6)
	params(0:2) = 2			;basis functions  (in x,y and z)
	params(0) = 8
	params(1) = 8
	params(2) = 8	
	params(3) = 16			;elastic deformation
	if keyword_set(noplastic) then params(3) = 0
	params(4) = 8			;smoothing for image
	params(5) = 0.02		;smoothness of deformation field
	aff_parms = [0, 0, 0, 0, 0, 0, -1, 1, 1, 0, 0, 0]
	matname = change_ext(P, '_sn3d.mat')
	locmatname = matname
	if keyword_set(repair) and not(keyword_set(noplastic)) then locmatname =  change_ext(P, '_acpc_sn3d.mat')
;----------------------------------------------------------------------------
;check if the scans were inverted, and if this is a pre-normalized loc
;---------------------------------------------------------------------------
; 	'Custom Starting Estimates'
; 	Enter starting estimates in the following order:
; 		x translation (mm)
; 		y translation (mm)
; 		z translation (mm)
; 		x rotation about - {pitch} (radians)
; 		y rotation about - {roll}  (radians)
; 		z rotation about - {yaw}   (radians)
; 		x scaling
; 		y scaling
; 		z scaling
; 		x affine
; 		y affine
; 		z affine
	free = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
	free(0:2) = 1		;translation
	free(3:5) = 1		;rotation
	free(6:8) = 1		;scaling
	free(9:11) = 1 		;affine	

	if loch.direction lt 0 then aff_parms(8) = (-1)
	

	
;-----------------------------------------------------------------------
;can fix any aff_params using free here as well
;ie fix rotations about x and z, but allow translations and scaling:  free(3) = 0
;								      free(5) = 0	
;------------------------------------------------------------------------
	if (strtrim(loch.type,2) eq 'TALLOCS') or (strpos(locfile,'tallocs.loc') ge 0)  then begin
			aff_parms(6) = 1
			free(3) = 0
			free(5) = 0
	endif
	;if loch.direction lt 0 then aff_parms = [0, 0, 0, 0, 0, 0, -1, 1, -1, 0, 0, 0]
	;spms='/appl/SPM96/templates/' + sequence_type + '.img'
	spms = path_of('my_templates') + sequence_type + '.img'

	cd,outdir
	bb=[[-90, -126, -72], [91, 91, 109]]
	Vox = fltarr(3)
	Vox(*) = 2
	;print,'outdir'
	;print,outdir
	;print,p
	;print,matname
	;print,bb
	;print,vox
	;print,'params'
	;print,params
	;print,spms
	;print,'aff'
	;print,aff_parms
	;print,'free'
	;print,free
	;if not(keyword_set(nonormalize)) and (startim eq 0) then spm_sn3d,P,matname,bb,Vox,params,spms,aff_parms,free,/display
	extract_pname,P(0),pname
	cd,pname
	spm_sn3d,P,matname,bb,Vox,params,spms,aff_parms,free,/display
	imagename = prepend(change_ext(P,'.hdr'),'n')
	templatename = change_ext(spms, '.hdr')
	Hold = 3		;Sinc
;---------------------------------------------------------
;generate .loc type normalized loc file for overlays if desired
;----------------------------------------------------------
	if keyword_set(make_talloc) and (startim eq 0) then begin
		vox(*) = 2
		print,'normalizing locfile to .img and converting to .loc'
		spm_write_sn,P,matname,bb,Vox,Hold
		an_talloc2loc,matname = matname,an_fname = imagename,object = locfile,/quiet
		vox(*) = 2
	endif
	spawn,'imagetool ' +  outdir + 'spm.ps &'
	if keyword_set(noprompt) then begin
		good = 1
		;spm_write_sn,P,matname,bb,Vox,Hold
	endif

	if not(keyword_set(noprompt)) then begin
		good = OK('Is This Adequate','Yes','No')
		;spm_spatial_display_results,imagename,templatename,'Normalization' ,replyback = good
		if not(good) then picktal
	endif
endif


end



pro dicom2talloc,outdir = outdir,batch = batch

;---------------------------------------------------------------------------------
;this program will find the T1 file and normalize it
;note that the files don't have to be created using the above routines
;this program will work with any .loc or .locs directories
;outdir = the directory containing the .locs to be normalized
;the .sn3dmat file and normalized n*.loc will be written to the same directory
;set batch keyword to select a directory of subdirectories to run the normalization
;------------------------------------------------------------------------------------
	count = 1
	dirs = ''
	if not(keyword_set(outdir)) then outdir = pickdir(title = 'Choose directory of locs for normalization')
	if not(keyword_set(batch)) then run_dicom2talloc,outdir = outdir
	if keyword_set(batch) then begin
		dirs = list_dir(path = outdir,count = count)
		if count ne 0 then begin
			for i = 1,count do begin
				nextdir = outdir + dirs(i - 1) + '/'
				print,'Running dicom2talloc on ' + nextdir
				print, 'Remaining = ', count - i
				run_dicom2talloc,outdir = outdir + dirs(i - 1) + '/'
			endfor
		endif
	endif
end

				


pro dicom_locsort,path = path
if not(keyword_set(path)) then path = pickdir(title = 'Choose folder of Dicom series images')
subdirs = find_subdir(path = path,error = error)
if error then begin
	badboy,'badpath : ' + path
	return
endif
dirs = n_elements(subdirs)
extract_pname,path,pn,parents = parents
outdir = '/'
for i = 0,n_elements(parents) - 3 do outdir = outdir + parents(i) + '/'
dicom2loc,outdir = outdir,path = path
end
