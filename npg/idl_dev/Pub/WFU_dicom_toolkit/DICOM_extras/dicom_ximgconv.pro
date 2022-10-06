pro dicom_ximgconv,DIR=inpath,examdir = examdir

; D. Alsop (9/29/94)
;modified J. Maldjian (8-20-97)

;;;This program has been modified to convert and sort DICOM images into directories
;;;gunzip, and tar are built-in
;;;program uses mbu get_pixels to create square images
;;;all extraneous files are removed
;Note that with the group_length problem, pixel 0 is getting a very high value, screwing up the display


;Program to sort the images created by the ximg extraction tool on the
;GE SIGNA scanners.  Images created are of the form:
;
;	E"exam #"S"series #"I"image #".MR      ex. E1020S2I3.MR
;
;This routine creates a directory hierarchy of exams with series subdirs
;and moves the files to the appropriate subdir.  The files are renamed
;
;	I."im #"         ex. I.002
;
;The directory where the images are located can be specified with the string
;keyword DIR or chosen with pickdir(), a widget interface.


;Use a widget interface to choose inpath if not given as keyword
;dicompath = '/data/DICOM'
dicompath = '/data/'
cd,current = startdir
if path_of('hostname') eq 'kyle' then cd,dicompath,current = startdir
if (not keyword_set(inpath)) then begin
   inpath=pickdir(title="Please select the dir where images are located")
endif
cd, inpath


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;unzip files using gunzip
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
filespec = 'MR*'

filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
swap = 0
if fcount le 0 then begin
	filespec = 'GE*'
	filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
	swap = 1
endif
if fcount le 0 then begin
	filespec = 'CT*'
	filels = make_filelist(path = inpath,filespec = filespec +  '.Z',filecount = fcount)
	swap = 1
endif

cmd = 'gunzip '
cmd = 'uncompress '
if fcount gt 0 then for i = 0,fcount -1 do spawn,cmd + inpath + filels(i)

;;;;;;;;;;;;;;;;;;;;;;
;;;;untar files
;;;;;;;;;;;;;;;;;;;;;;;
filels = make_filelist(path = inpath,filespec = filespec,filecount = fcount)
cmd = 'tar -xvf '
for i = 0,fcount -1 do begin
	s = strlen(filels(i))
	mid = strmid(filels(i),s-3,3)
	if mid eq 'tar' then begin
		print,'untarring and removing ' + filels(i) + ' ...'
		 spawn,cmd + filels(i),result
		spawn, 'rm ' + inpath + filels(i)
	endif
endfor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;make square images
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
filespec = 'MR*.*'
filels = make_filelist(path = inpath,filespec = filespec,filecount = fcount)
if fcount eq 0 then filels = make_filelist(path = inpath,filespec = '*.*',filecount = fcount)
;----------------------------------------------------
;;Swap BIG endian for GEPACS images from new archive
;;Apparently, all the images now need swapping
;;Problem with group_length too?
;----------------------------------------------------
swap = 1
group_length = 1
cmd_swap = 'mbu swap ' + inpath
cmd_group = 'mbu group_length ' + inpath
cmd_mbu = 'mbu get_pixels ' + inpath

;-----------------------------------------------
;Save a representative file from each series
;For Dicom Header Info
;-----------------------------------------------
dcmheaders = findfile(inpath + '*.001',count = hcount)
if hcount gt 0 then begin
	for i = 0,hcount - 1 do begin
		snum = strmid(dcmheaders(i),strlen(dcmheaders(i)) - 7,3)
		hfile = inpath + 'IMG_S' + strtrim(snum,2) + '_I' + '001.dcm'
		spawn,'cp ' + dcmheaders(i) + ' ' + hfile
	endfor
endif


print,'Executing mbu get_pixels on ' + inpath
if fcount ge 1 then begin
		for i = 0,fcount - 1 do begin
		nextfile = filels(i)
		if strmid(nextfile,strlen(nextfile) - 6,6) ne 'pixels' then begin
			if swap then spawn,cmd_swap + filels(i)
			if group_length then spawn,cmd_group + filels(i)
			spawn,cmd_mbu +  filels(i)
			spawn,'rm ' + inpath + filels(i)
		endif
	endfor
endif

filels = make_filelist(path = inpath,filespec = '*.pixels',filecount = nfiles)
;nfiles=n_elements(filels)
if (filels(0) eq "No match") then nfiles=0	;no files are present

;Move the files one at a time
for filenum=0,nfiles-1 do begin

   ;First strip out the exam, series and image info
   fname=filels(filenum)
	fparts = str_sep(fname,'.',/trim,/remove_all)
	si = 1
	notdone = 1
	while notdone do begin
		examdir = fparts(si)
		if strlen(examdir) gt 3 and long(examdir) gt 0 then notdone = 0
		si = si + 1
		if si ge n_elements(fparts) then notdone = 0
	endwhile
	





  	; epos = 7
	;mid = strmid(fname,epos + 1,strlen(fname) - (epos + 1))
	;spos = epos + strpos(mid,'.') + 1
	;mid2 = strmid(fname,spos + 1,strlen(fname) - (spos + 1))
	;ipos = spos + strpos(mid2,'.') + 1
	;perpos = strpos(fname,'.pixels')
	;if perpos lt 0 then perpos = strlen(fname)

		
   ;epos=STRPOS(fname,"E")	;position of "E" in filename
   ;spos=STRPOS(fname,"S")	;position of "S" in filename
   ;ipos=STRPOS(fname,"I")	;position of "I" in filename
   ;perpos=STRPOS(fname,".")	;position of "." in filename
  
 ;if( (epos eq -1) or (spos eq -1) or (ipos eq -1) or (perpos eq -1)) $
  ;                                                            then continue
   ;if( (epos ge spos) or (spos ge ipos) or (ipos ge perpos) ) then continue


   ;exam=fix(strmid(fname,epos+1,spos-epos-1))
   ;series=fix(strmid(fname,spos+1,ipos-spos-1))
   ;image=fix(strmid(fname,ipos+1,perpos-ipos-1))

 	;examdir=string(strmid(fname,epos+1,spos-epos-1),FORMAT = '(I5.5)')
	;examdir=strtrim(strmid(fname,epos+1,spos-epos-1),2)

   	;seriesdir=string(strmid(fname,spos+1,ipos-spos-1),FORMAT = '(I3.3)')
   	;image=string(strmid(fname,ipos+1,(perpos - ipos - 1)),FORMAT = '(I3.3)')

;----------------------------------------
;added for new GEPACS format
;Should also be compatible with old format
;-----------------------------------------
	beg_ipos = 10
	beg_spos = 14
	seriesdir = strtrim(strmid(fname,strlen(fname) - beg_spos,3))
	image = strtrim(strmid(fname,strlen(fname) - beg_ipos,3))


   ;Create a dir for the exam if not already present
   ;examdir=string(exam,FORMAT='(I5.5)')
   exexist=valid_dir(examdir)
   if (exexist lt -1 or exexist eq 0) then spawn, 'mkdir '+examdir

   ;Create a dir for the series if not already present
   ;seriesdir=string(series,FORMAT='(I3.3)')
   serexist=valid_dir(examdir + "/" + seriesdir)
   if (serexist eq 0 or serexist lt - 1) then spawn, 'mkdir ' + examdir+ "/" + seriesdir

   ;Move and rename the file
   imfile=string(image,FORMAT='(I3.3)')
  ; dcmfile =  inpath + strmid(filels(filenum),0,strlen(filels(filenum)) - 7)
  ; if imfile eq '001' then begin
 ;	dcmheaderfile = inpath + 'IMG' + '_S' + seriesdir + '_I' + imfile + '.dcm'
;	cmd_save_dicom_header = 'cp ' + dcmfile + ' ' + dcmheaderfile
;	;print,cmd_save_dicom_header
;	spawn,cmd_save_dicom_header
;   endif
;   spawn,'rm ' + dcmfile		

;bdir = startdir + '/' + 
;original = startdir + '/tmp/' + fname
;moveto = startdir + '/tmp/' + examdir+'/'+seriesdir+'/I.'+imfile
;print, 'original = ',original
;print, 'moveto = ', moveto
 ;spawn, 'mv ' + original + " " + moveto
 
spawn, 'mv '+fname+' '+examdir+'/'+seriesdir+'/I.'+imfile

endfor

;Go back to the original directory
cd,startdir

return
end

