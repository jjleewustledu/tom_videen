;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   ECAT.PRO   **********
;
;	THIS FILE CONTAINS IDL ROUTINES FOR HANDLING	
;	ECAT MATRIX FILES. THERE ARE THREE PARTS TO 
;	THE FILE :
;	
;	   (1) GENERAL UTILITIES
;	   (2) HEADER AND DATA ROUTINES
;	   (3) READING AND WRITING ROUTINES
;
;	CREATED  
;	   AUGUST 26,1992	: WILL TRIBBEY
;
;	Modifications:
;	  2/93:	emp : Added support for reading Polar Map files.
;	  5/93: wpt : Added intrinsic tilt to rsh_img.
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   GENERAL UTILITIES   **********
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; ECAT_SET_ERROR
;
;	THIS IS THE BASIC ERROR ROUTINE FOR ALL ROUTINES
;
pro   set_ecat_error,code,module,FILE=file,UNIT=unit

	on_error,2
	m = ""
	c = 0
	f = ""
	if(n_params() GT 0) then c = code
	if(n_params() GT 1) then m = module + ": "
	if(keyword_set(file)) then f = " -> " + file
	if(keyword_set(unit)) then begin
	   f = fstat(unit)
	   file = f.name
	   f = " -> " + file
	   free_lun,unit
	endif
	  
	!ECAT.ECAT_ERROR = !MSG_PREFIX + m + !ECAT.ERROR_CODES(c) + f
	if(NOT !quiet) then print,!ECAT.ECAT_ERROR
	return
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; VTOSF
;	VTOSF CHANGES VAX FLOATS TO SUN FLOATS
;
;
function   vtosf,bits

	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"VTOSF"
	   return,0.0
	endif

	byteorder,bits,/lswap
	sign      = where((bits AND '8000'xl) NE 0, scount)
	exponent  = (ishft(bits,-7) AND 'ff'x) - 128 ; exponent - bias
	fraction  = ishft(bits AND '7f'x,16)	     + $ ; high 7 bits of fraction
	            ishft(bits AND 'ffff0000'xl,-16) + $ ; low 16 bits of fraction
		    '800000'xl				 ; hidden bit = 1
	z         = where(bits EQ 0, count)
	if(count GT 0) then begin			 ; set the zeros
	   fraction(z) = 0.0
	   exponent(z) = 0
	endif
	x = fraction*(2.0^(exponent-24.0))
	if(scount GT 0) then x(sign) = -x(sign)
	return,x
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; STOVF
;	STOVF CHANGES SUN FLOATS TO VAX FLOATS
;
;
function stovf,f

	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"STOVF"
	   return,0.0
	endif

	bits = long(f,0,n_elements(f))
	sign = where((bits and '80000000'xl) ne 0, scount)    ;Sign bit
	expon = ishft(bits, -23) and 'ff'x                    ;Exponent
	fraction = bits and '7fffff'xl                        ;Coefficient
	vax = ishft((expon+2), 7) + 		  $           ;Exponent
              (ishft(fraction, -16) and '7f'xl) + $           ;Most sig fraction
               ishft(fraction and 'ffff'xl, 16)
	if scount gt 0 then vax(sign) = vax(sign) or '8000'xl ;Add sign bit
        byteorder,vax,/lswap
        return, vax
end 
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;
; MATNUM
;	MATNUM RETURNS A MATRIX NUMBER FOR FRAME,PLANE,GATE,DATA,BED
;
function   matnum,frame,plane,gate,data,bed
	
	on_error,2
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"MATNUM"
	   return,!ECAT.ERROR
	endif

	hi_plane  = (long(plane) AND '300'XL)
	lo_plane  = (long(plane) AND 'FF'XL)
	hi_data	  = (long(data) AND '4'XL)
	lo_data   = (long(data) AND '3'XL)

	mat = ((long(frame) AND '1FF'XL) 	    OR $ 
	       (ishft((long(bed) AND 'F'XL), 12))   OR $
	       (ishft(lo_plane,16))		    OR $
	       (ishft(hi_plane,1))		    OR $
	       (ishft((long(gate) AND '3F'XL), 24)) OR $
	       (ishft(lo_data,30))		    OR $
	       (ishft(hi_data,9)))
	
	return,mat
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; MATVAL
;	MATVAL RETURNS AN ARRAY WITH THE FRAME, PLANE, GATE, DATA, AND
;	BED CORRESPONDING TO THE MATRIX NUMBER GIVEN BY MATNUM
;
function   matval,matnum	

	on_error,2
	if(n_params() NE 1) then begin
	   ecat_set_error,2,"MATVAL"
	   return,!ECAT.ERROR
	endif
	hi_plane  = (ishft(long(matnum),-1)  AND '300'XL)
	lo_plane  = (ishft(long(matnum),-16) AND 'FF'XL)
	hi_data	  = (ishft(long(matnum),-9)  AND '4'XL)
	lo_data   = (ishft(long(matnum),-30) AND '3'XL)
	mval0 = (matnum AND '1FF'XL)
	mval1 = (lo_plane OR hi_plane)
	mval2 = (ishft(long(matnum),-24) AND '3F'XL)
	mval3 = (lo_data OR hi_data)
	mval4 = (ishft(long(matnum),-12) AND 'F'XL)
	mval  = [mval0,mval1,mval2,mval3, mval4]
	return,mval
end 
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; MKMATS
;	MKMATS RETURNS THE MATRIX NUMBERS CORRESPONDING TO F,P,G,D,B
;	(FRAME,PLANE,GATE,DATA,BED). MATS IS AN OPTIONAL PARAMETER THAT
;	IS A LIST OF THE FRAMES,PLANES,GATES,DATA, AND BEDS GIVEN BY
;	F,P,G,D,B. 
;
function   mkmats,f,p,g,d,b,dir,NEW=new

	on_error,2
	if(n_params() NE 6) then begin
	   set_ecat_error,2,"MKMATS"
	   return,[!ECAT.ERROR]
	endif
	   	   
	nf = n_elements(f)
	np = n_elements(p)
	ng = n_elements(g)
	nd = n_elements(d)
	nb = n_elements(b)
	n  = nf*np*ng*nd*nb
	if(n LE 0) then begin
	   set_ecat_error,3,"MKMATS"
	   return,!ECAT.ERROR
	end

	mats = lonarr(5, n)

;	FILL FRAME INDICES

	ind = replicate(f(0), np)
	for i=1,nf-1 do ind = [ind,replicate(f(i), np)]
	lim = ng*nb*nd & ctr = 0 & inc = nf*np
	for i=1,lim do begin
	   mats(0,ctr:ctr+inc-1) = ind
	   ctr = ctr+inc
	end

;	FILL PLANE INDICES

	lim = nf*ng*nb*nd
	ind = p
	for i=2,lim do ind = [ind,p]
	mats(1,*) = ind

;	FILL GATE INDICES

	lim = np*nf
	ind = replicate(g(0),lim)
	for i=1,ng-1 do ind = [ind, replicate(g(i),lim)]
	lim = nb*nd & ctr = 0 & inc = np*nf*ng
	for i=1,lim do begin
	   mats(2,ctr:ctr+inc-1) = ind
	   ctr = ctr + inc
	end

;	FILL DATA INDICES

	lim = np*nf*ng*nb
	ind = replicate(d(0), lim) 
	for i=1,nd-1 do ind = [ind, replicate(d(i),lim)]
	mats(3,*) = ind

;	FILL BED INDICES

	lim = np*nf*ng
	ind = replicate(b(0), lim)
	for i=1,nb-1 do ind = [ind, replicate(b(i),lim)]
	lim = nd & ctr = 0 & inc = np*nf*ng*nb
	for i=1,lim do begin
	   mats(4,ctr:ctr+inc-1) = ind
	   ctr = ctr + inc
	end

;	REMOVE MISSING MATRICES

	matnums = matnum(mats(0,*),mats(1,*),mats(2,*),mats(3,*),mats(4,*))
	if(keyword_set(NEW)) then begin
	   list = lonarr(7,n_elements(matnums))
	   for i=0,n_elements(matnums)-1 do begin
	      list(*,i) = [matnums(i), 0L, mats(*,i)]
	   endfor
	endif else begin
	   matchnums,dir,matnums,mats
	   list = lonarr(7, n_elements(matnums))
	   for i=0,n_elements(matnums)-1 do begin
	      off = dir(4,where(dir(0,*) EQ matnums(i)))
	      list(*,i) = [matnums(i), off, mats(*,i)]
	   endfor
	endelse
	return,list
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RDIR
;	RDIR READS THE DIRECTORY OF THE FILE INDICATED EITHER BY THE OPEN
;	FILE,UNIT, OR THE FILE INDICATED BY THE KEYWORD FILE.
;	RDIR RETURNS !ECAT.OK ON SUCESSFUL RETURN AND !ECAT.ERROR OTHERWISE.
;
function   rdir,unit,dir,dirinfo,FILE=file

	on_error,2
	on_ioerror,io_error
	if(n_params() LT 2) then begin
	   if(keyword_set(FILE)) then            $
	      set_ecat_error,2,"RDIR",FILE=file  $
	   else                                  $
	      set_ecat_error,2,"RDIR",UNIT=unit
	   return,!ECAT.ERROR
	endif

	BLKSIZE = !ECAT.BLKSIZE
	DIRBLK1 = !ECAT.DIRBLK1
	dirblk   = lonarr(4,32)
	if(keyword_set(FILE)) then openr,unit,file,/get_lun
	point_lun,unit,(DIRBLK1-1)*BLKSIZE
	readu,unit,dirblk
	byteorder,dirblk,/lswap
	dir = dirblk(*,1:*)
	dirinfo = dirblk(*,0)
	
	while(dirblk(1,0) NE DIRBLK1) do begin
	   point_lun,unit,long(BLKSIZE*(dirblk(1,0)-1))
	   readu,unit,dirblk
	   byteorder,dirblk,/lswap
	   dirinfo = [[dirinfo],[dirblk(*,0)]]
	   dir =[[dir],[dirblk(*,1:*)]]
	end
	dir = [dir,(((dir(1,*) GT 0)*dir(1,*))-1)*!ECAT.blksize >0]
	if(keyword_set(FILE)) then free_lun,unit
	dirblk = 0
	return,!ECAT.OK

io_error : set_ecat_error,1,"RDIR",UNIT=unit
	   return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; MATCHNUMS
;	MATCHNUMS CHANGES MATNUMS AND MATS SO THAT ONLY MATRICES ACTUALLY
;	IN THE DIRECTORY DIR ARE REPRESENTED IN MATNUMS AND MATS.
;
pro matchnums,dir,matnums,mats

	on_error,2
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"MATCHNUMS"
	   return
	endif
	for i=0,n_elements(matnums)-1 do begin
	   ind = where(matnums(i) EQ dir(0,*))
	   if(ind(0) EQ -1) then begin
	      matnums(i) = 0
	      mats(*,i) = 0
	    endif
	endfor
	ind = where(matnums GT 0)
	if(ind(0) EQ -1) then begin
	   matnums = !ECAT.ERROR
	   mats = !ECAT.ERROR
	   set_ecat_error,14,"MATCHNUMS"
	   return
	endif
	matnums = matnums(where(matnums GT 0))
	ind = where((mats(0,*) GT 0) GT 0)
	if(ind(0) EQ -1) then return
	mats = mats(*, ind)
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; SECAT
;	SECAT WITH NO PARAMETERS PRINTS !ECAT TO THE SCREEN.
;	SECAT WITH ONE PARAMETER PRINTS HELP,/STRUCT TO THE SCREEN.
;	SECAT WITH THE KEYWORD ECAT ALSO PRINTS !ECAT TO THE SCREEN.
;
pro	secat,struct,ECAT=ecat

	if((n_params() EQ 0) OR keyword_set(ECAT)) then help,/struct,!ECAT
	if(n_params() GT 0) then help,/struct,struct
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; EXPAND
;	EXPAND HANDLES THE CASE WHERE A "*" IS INPUT FOR FRAMES,PLANES,
;	GATES, OR BEDS
;
pro	expand,f,p,g,d,b,dvals

	on_error,2
	if(n_params() NE 6) then begin
	   set_ecat_error,2,"EXPAND"
	   dvals = !ECAT.ERROR
	   return
	endif
	   
	maxf = dvals(0)
	maxp = dvals(1)
	maxg = dvals(2)
	maxd = dvals(3)
	maxb = dvals(4)
	sf = string(f)
	sp = string(p)
	sg = string(g)
	sd = string(d)
	sb = string(b)
	ind = where(sf EQ "*",cnt) & if(cnt GT 0) then f = lindgen(maxf)+1
	ind = where(sp EQ "*",cnt) & if(cnt GT 0) then p = lindgen(maxp)+1
	ind = where(sg EQ "*",cnt) & if(cnt GT 0) then g = lindgen(maxg)+1
	ind = where(sd EQ "*",cnt) & if(cnt GT 0) then d = lindgen(maxd)
	ind = where(sb EQ "*",cnt) & if(cnt GT 0) then b = lindgen(maxb)
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; FILL_ARR
;	FILL_ARR TAKES ARRAY A AND MAKES IT HAVE A LENGTH EQUAL TO LEN
;
pro	fill_arr,a,len

	on_error,2
	n = n_elements(a)
	if(n LT len) then a=[a, make_array(len-n,size=size(a))]
	return
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; INCR
;	INCR ADDS VAL TO I
;
pro	incr,i,val

	on_error,2
	i = i + long(val)
	return
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; DATA_SIZE
;	DATA_SIZE RETURNS THE DATA SIZE IN NUMBER OF BLOCKS.
;
function data_size,sh,dimsize

	on_error,2
	if(n_params() NE 2) then begin
	   set_ecat_error,2,"DATA_SIZE"
	   return,!ECAT.ERROR
	endif

	case !ECAT.data_types(sh.data_type) of
	   "VAX_I2" : size = 2L*dimsize
	   "SUN_I2" : size = 2L*dimsize
	   "VAX_R4" : size = 4L*dimsize
	   "SUN_R4" : size = 4L*dimsize
	   else : begin
	      set_ecat_data,0,"DATA_SIZE",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase
	
	if(size LT !ECAT.BLKSIZE) then return,1L
	if((size MOD !ECAT.BLKSIZE) EQ 0) then return,long(size/!ECAT.BLKSIZE)
	return,long(float(size)/!ECAT.BLKSIZE)+1L

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; DIFFVAL
;	DIFFVAL RETURNS AN ARRAY WITH THE TOTAL NUMBER OF DIFFERENT
;	FRAMES, PLANES,GATES,DATA, AND BEDS FOR THE DIRECTORY GIVEN.
;	DIFFVAL WILL RETURN THE TOTAL NUMBER OF DIFFERENT VALUES IN ANY
;	VECTOR, OR IN EACH COLUMN OF AN ARRAY
;
function diffval,dir,NOT_DIR=not_dir

	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"DIFFVAL"
	   return,!ECAT.ERROR
	endif

	if(NOT keyword_set(NOT_DIR)) then begin
	   ind = where(dir(0,*) GT 0)
	   if(ind(0) LT 0) then ind = 0
	   matnums = dir(0,ind)
	   fpgdb = matval(matnums)
	   fpgdb = reform(fpgdb,5,n_elements(fpgdb)/5)
	   s=size(fpgdb)
	   count=s(1)
	endif else 				$
	   fpgdb=dir
	if(keyword_set(NOT_DIR)) then begin
	   s=size(fpgdb)
	   if(s(0) EQ 0) then return,fpgdb
	   if(s(0) EQ 1) then begin		$
	      if(s(1) EQ 1) then return,fpgdb	$
	      else count=1 & fpgdb=transpose(fpgdb)
	   endif else count=s(1)
	endif
	dvals = lonarr(count)
	for j=0,count-1 do begin
	   n = fpgdb(j,*)
	   n = n(sort(n))
	   count = 1
	   for i=0,n_elements(n)-2 do begin
	      if(n(i) NE n(i+1)) then count = count +1
	   endfor
	   dvals(j) = count
	endfor
	return,dvals
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; ANAT_SORT 
;	ANAT_SORT SORTS THE GIVEN DIRECTORY INTO
;	ANATOMICAL ORDER.
;
function anat_sort,dir,mh

	on_error,2
	if(n_params() NE 2) then begin
	   set_ecat_error,2,"ANAT_SORT"
	   return,!ECAT.ERROR
	endif

	init_pos = mh.scan.init_bed_position
	offsets = mh.scan.bed_offset
	plane_separation = mh.scan.plane_separation
	dir = dir(*,where(dir(0,*) GT 0))
	matnums = dir(0,*)
	nbeds = mh.file.nbeds
	nplanes=mh.file.nplanes

	slice_loc=0.0
	bo=[0,offsets]
	for i=0,nbeds do begin
	   init_pos = init_pos + bo(i)
	   for j=0,nplanes-1 do begin
	      slice_loc=[slice_loc, init_pos + j*plane_separation]
	   endfor
	endfor
	index=sort(slice_loc(1:*))
	dir=dir(*,index)
	return,!ECAT.OK
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; MKNEWDIRBLK
;	MKNEWDIRBLK CREATES A NEW DIRECTORY BLOCK IN THE FILE 
; 	REPRESENTED BY UNIT. THE NEXT FREE BLOCK IS RETURNED ON
;	SUCCESS AND !ECAT.ERROR IS RETURNED ON FAILURE.
;
function mknewdirblk,unit,dir,dirinfo,NEW=new
	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"MKNEWDIRBLK",UNIT=unit
	   return,!ECAT.ERROR
	endif

	if(keyword_set(NEW)) then begin
	   dirblk = lonarr(4,32)
	   dirblk(*,0) = [31,!ECAT.DIRBLK1,0,0]
	   dir = [dirblk(*,1:*),transpose(lonarr(31))]
	   dirinfo = dirblk(*,0)
	   point_lun,unit,!ECAT.BLKSIZE
	   byteorder,dirblk,/lswap
	   writeu,unit,dirblk
	   return, !ECAT.OK
	endif

	dirblk = lonarr(4,32)
	ndirblks = n_elements(dirinfo(0,*))
	if(ndirblks EQ 1) then 			$
	   prevblk = !ECAT.DIRBLK1		$
	else					$
	   prevblk = dirinfo(1,ndirblks-2)
	ind = where(dir(2,*) GT 0, nmats)
	if(nmats EQ 0) then begin
	   set_ecat_error,5,"MKNEWDIRBLK",UNIT=unit
	   return,!ECAT.ERROR
	endif
	lastblk = dir(2, ind(nmats-1))

	dirblk(*,0)=[31L,0L,0L,0L]
	dirblk(1,0) = !ECAT.DIRBLK1
	dirblk(2,0) = prevblk
	dirblk(3,0) = 0L
	dir = [[dir],[lonarr(5,31)]]
	dirinfo = [[dirinfo],[dirblk(*,0)]]
	point_lun,unit,lastblk*!ECAT.BLKSIZE
	wdir = dirblk
	byteorder,wdir,/lswap
	writeu,unit,wdir
	point_lun,unit,(dirblk(2,0)-1)*!ECAT.BLKSIZE 
	prevdir = lonarr(4)
	readu,unit,prevdir 
	byteorder,prevdir,/lswap
	newdirblk = lastblk+1L
	prevdir(1) = newdirblk
	point_lun,unit,(dirblk(2,0)-1)*!ECAT.BLKSIZE 
	byteorder,prevdir,/lswap
	writeu,unit,prevdir 
	dirinfo(1,n_elements(dirinfo(0,*))-2) = newdirblk

	return,!ECAT.OK

io_error : set_ecat_error,1,"MKNEWDIRBLK",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; ENTER_MAT
;	ENTER_MAT UPDATES THE DIRECTORY WITH A NEW ENTRY.
;	ENTER_MAT RETURNS !ECAT.OK ON SUCCESS AND !ECAT.ERROR
;	ON FAILURE
;
function enter_mat,unit,matnum,dir,dirinfo,dirblk,nextblk,nblks,direntry

	on_error,2
	on_ioerror,io_error

	if(n_params() NE 8) then begin
	   set_ecat_error,2,"ENTER_MAT",UNIT=unit
	   return,!ECAT.ERROR
	endif
	point_lun,unit,(dirblk-1)*!ECAT.BLKSIZE
	d = lonarr(4,32)
	readu,unit,d & byteorder,d,/lswap
	ind = where(d(0,*) EQ 0, free)
	if(free EQ 0) then begin
	   set_ecat_error,5,"ENTER_MAT",UNIT=unit
	   return,!ECAT.ERROR
	endif
	direntry = [long(matnum),nextblk,nextblk+nblks-1L,1L]
	d(0,0) = d(0,0)-1
	d(3,0) = d(3,0)+1
	d(*,ind(0)) = direntry
	point_lun,unit,(dirblk-1)*!ECAT.BLKSIZE
	byteorder,d,/lswap & writeu,unit,d
	return,!ECAT.OK

io_error : set_ecat_error,1,"ENTER_MAT",UNIT=unit
	   return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; NEXTFREE
;	NEXTFREE TAKES A DIRECTORY AND DIRECTORY INFORMATION FROM THE RDIR
;	ROUTINE, CHECKS FOR THE NEXT FREE DIRECTORY POSITION OR CREATES A
;	NEW DIRECTORY BLOCK IF NECESSARY, AND RETURNS THE BLOCK NUMBER
;	FOR THE NEXT FREE BLOCK. NEXTFREE RETURNS !ECAT.ERROR WHEN AN 
;	ERROR IS ENCOUNTERED.
;
function nextfree,unit,matnum,nblks,dir,dirinfo

	on_error,2
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"NEXTFREE",UNIT=unit
	   return,!ECAT.ERROR
	endif

	ind = where(dirinfo(0,*) GT 0, anyfree)
	if(anyfree EQ 0) then begin
	   stat = mknewdirblk(unit,dir,dirinfo)
	   if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	endif

	ndir = n_elements(dirinfo(0,*))
	if(ndir EQ 1) then 			$
	   dirblk = !ECAT.DIRBLK1		$
	else 					$			
	   dirblk = dirinfo(1,ndir-2)
	ind = where(dir(2,*) GT 0, nmats)
	if(nmats EQ 0) then begin
	   nextblk = !ECAT.DIRBLK1+1L
	endif else begin
       	   nextblk = dir(2,nmats-1)+1L
	   if(nextblk Eq dirblk) then nextblk = dirblk+1L
	endelse
   
	stat = enter_mat(unit,matnum,dir,dirinfo,dirblk,nextblk,nblks,direntry)
	if(stat EQ !ECAT.ERROR) then return, !ECAT.ERROR
	dirinfo(0,ndir-1) = dirinfo(0,ndir-1)-1
	dirinfo(3,ndir-1) = dirinfo(3,ndir-1)+1
	ind = where(dir(0,*) EQ 0)
	dir(*,ind(0)) = [direntry,(direntry(1)-1)*!ECAT.BLKSIZE]
	
	return,nextblk


end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; DSCALE
;	DSCALE COMPUTES THE SCALE FACTOR NECESSARY FOR SINOGRAM AND
;	IMAGE DATA TO BE IN THE RANGE -32768 TO 32767.
;
function dscale,data

	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"DSCALE"
	   return,-1.0
	endif
	s = size(data)
	if(s(0) EQ 2) then zdim = 1 else zdim = s(3)
	sf = fltarr(zdim)
	for i=1,zdim do begin
	   dmin = 0.0
	   dmax = abs(max(data(*,*,i-1),min=dmin))
	   dmin = abs(dmin)
	   svec =[dmin,dmax,(1.0e-7)]
	   abmax = max(svec)
	   sf(i-1) = abmax/32767.0
	   data(*,*,i-1) = data(*,*,i-1)*(32767.0/abmax)
	endfor
	data = fix(data)
	return,sf

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   HEADER UTILITIES   **********
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RMH
;	RMH READS THE MAIN HEADER OF THE OPEN FILE, UNIT, OR THE 
;	FILE INDICATED BY THE KEYWORD FILE. RMH RETURNS !ECAT.OK ON SUCCESFUL
;	EXIT AND !ECAT.ERROR OTHERWISE.
;	
function rmh,unit,mh,FILE=file

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 2) then begin
	   if(keyword_set(FILE)) then  $
	      set_ecat_error,2,"RMH"   $
	   else                        $
	      set_ecat_error,2,"RMH",UNIT=unit
	   return,!ECAT.ERROR
	endif

	if(keyword_set(FILE)) then openr,unit,file,/get_lun
	point_lun,unit,28
	mh = {Main_Header}
	in=bytarr(20) & readu,unit,in & in=string(in)
	mh.file.original_file_name = in
	in=intarr(4) & readu,unit,in & byteorder,in
	mh.file.sw_version = in(0)
	mh.file.data_type = in(1)
	mh.file.system_type = in(2)
	mh.file.file_type = in(3)
	in=bytarr(10) & readu,unit,in & in=string(in)
	mh.file.node_id = in
	in=intarr(3) & readu,unit,in & byteorder,in
	mh.file.scan_start(*,0) = in
	readu,unit,in & byteorder,in
	mh.file.scan_start(*,1) = in
	in = bytarr(8) & readu,unit,in & in=string(in)
	mh.file.isotope_code = in
	in=1L & readu,unit,in & in=vtosf(in)
	mh.file.isotope_halflife = in
	in=bytarr(32) & readu,unit,in & in=string(in)
	mh.file.radiopharmaceutical = in
	in=lonarr(3) & readu,unit,in & in=vtosf(in)
	mh.scan.gantry = in(0:1)
	mh.scan.bed_elevation = in(2)
	in=intarr(3) & readu,unit,in & byteorder,in
	mh.scan.rot_source_speed = in(0)
	mh.scan.wobble_speed = in(1)
	mh.scan.transm_source_type = in(2)
	in=lonarr(2) & readu,unit,in & in=vtosf(in)
	mh.scan.axial_fov = in(0)
	mh.scan.transaxial_fov = in(1)
	in=intarr(3) & readu,unit,in & byteorder,in
        mh.scan.transaxial_samp_mode = in(0)
	mh.scan.coin_samp_mode = in(1)
	mh.scan.axial_samp_mode = in(2)
	in=1L & readu,unit,in & in=vtosf(in)
	mh.scan.calibration_factor = in
	in=intarr(2) & readu,unit,in & byteorder,in
	mh.scan.calibration_units = in(0)
	mh.scan.compression_code = in(0)
	in=bytarr(188) & readu,unit,in
	mh.pat(0) = string(in(0:11))
	mh.pat(1) = string(in(12:27))
	mh.pat(2) = string(in(28:59))
	mh.pat(3) = string(in(60))
	mh.pat(4) = string(in(61:70))
	mh.pat(5) = string(in(71:80))
	mh.pat(6) = string(in(81:90))
	mh.pat(7) = string(in(91))
	mh.pat(8) = string(in(92:123))
	mh.pat(9) = string(in(124:155))
	mh.pat(10) = string(in(156:187))
	in=intarr(3) & readu,unit,in & byteorder,in
	mh.scan.acquisition_type = in(0)
	mh.scan.bed_type = in(1)
	mh.scan.septa_type = in(2)
	in=bytarr(20) & readu,unit,in & in=string(in)
	mh.file.facility_name = in
	in=intarr(4) & readu,unit,in & byteorder,in
	mh.file.nplanes = in(0)
	mh.file.nframes = in(1)
	mh.file.ngates = in(2)
	mh.file.nbeds = in(3)
	in=lonarr(17) & readu,unit,in & in=vtosf(in)
	mh.scan.init_bed_position = in(0)
	mh.scan.bed_offset = in(1:15)
	mh.scan.plane_separation = in(16)
	in=intarr(3) & readu,unit,in & byteorder,in
	mh.scan.lwr_sctr_thresh = in(0)
	mh.scan.true_thresh = in(1:2)
	in=1L & readu,unit,in & in=vtosf(in)
	mh.scan.collimator = in
	in=bytarr(10) & readu,unit,in & in=string(in)
	mh.file.user_process_code = in
	in=1 & readu,unit,in & byteorder,in
	mh.scan.acquisition_mode = in

	if(keyword_set(FILE)) then free_lun,unit
	return,!ECAT.OK

io_error : set_ecat_error,1,"RMH",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WMH
;	WMH WRITES THE MAIN HEADER OF THE OPEN FILE, UNIT, OR THE 
;	FILE INDICATED BY THE KEYWORD FILE. RWMH RETURNS !ECAT.OK ON SUCCESFUL
;	EXIT AND !ECAT.ERROR OTHERWISE.
;	
function wmh,unit,mh,FILE=file

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 2) then begin
	   if(keyword_set(FILE)) then  $
	      set_ecat_error,2,"WMH"   $
	   else                        $
	      set_ecat_error,2,"WMH",UNIT=unit
	   return,!ECAT.ERROR
	endif

	wb = 0L
	if(keyword_set(FILE)) then openu,unit,file,/get_lun
	point_lun,unit,0
	out = bytarr(28) & writeu,unit,out & incr,wb,28
	out = byte(mh.file.original_file_name)
	fill_arr,out,20 & writeu,unit,out & incr,wb,20 
	out = [mh.file.sw_version, mh.file.data_type, $
	       mh.file.system_type, mh.file.file_type]
	byteorder,out & writeu,unit,out & incr,wb,8
	out = byte(mh.file.node_id)
	fill_arr,out,10 & writeu,unit,out & incr,wb,10
	out = mh.file.scan_start(*,0)
	byteorder,out & writeu,unit,out & incr,wb,6
	out = mh.file.scan_start(*,1)
	byteorder,out & writeu,unit,out & incr,wb,6
	out = byte(mh.file.isotope_code)
	fill_arr,out,8 & writeu,unit,out & incr,wb,8
	out = stovf(mh.file.isotope_halflife)
	writeu,unit,out & incr,wb,4
	out = byte(mh.file.radiopharmaceutical)
	fill_arr,out,32 & writeu,unit,out & incr,wb,32
	out = [mh.scan.gantry, mh.scan.bed_elevation]
	out = stovf(out) & writeu,unit,out & incr,wb,12
	out = [mh.scan.rot_source_speed, mh.scan.wobble_speed, 	$
	       mh.scan.transm_source_type]
	byteorder,out & writeu,unit,out & incr,wb,6
	out = [mh.scan.axial_fov, mh.scan.transaxial_fov]
	out = stovf(out) & writeu,unit,out & incr,wb,8
	out = [mh.scan.transaxial_samp_mode, mh.scan.coin_samp_mode, $
	       mh.scan.axial_samp_mode]
	byteorder,out & writeu,unit,out & incr,wb,6
	out = stovf(mh.scan.calibration_factor)
	writeu,unit,out & incr,wb,4
	out = [mh.scan.calibration_units, mh.scan.compression_code]
	byteorder,out & writeu,unit,out & incr,wb,4
	out = bytarr(188)
	out(0) =byte(mh.pat(0))
	out(12) = byte(mh.pat(1))
	out(28) = byte(mh.pat(2))
	out(60) = byte(mh.pat(3))
	out(61) = byte(mh.pat(4))
	out(71) = byte(mh.pat(5))
	out(81) = byte(mh.pat(6))
	out(91) = byte(mh.pat(7))
	out(92) = byte(mh.pat(8))
	out(124) = byte(mh.pat(9))
	out(156) = byte(mh.pat(10))
	fill_arr,out,188 & writeu,unit,out & incr,wb,188
	out = [mh.scan.acquisition_type, mh.scan.bed_type, $
	       mh.scan.septa_type]
	byteorder,out & writeu,unit,out & incr,wb,6
	out = byte(mh.file.facility_name)
	fill_arr,out,20 & writeu,unit,out & incr,wb,20
	out = [mh.file.nplanes, mh.file.nframes, 	$
	       mh.file.ngates, mh.file.nbeds]
	byteorder,out & writeu,unit,out & incr,wb,8
	out = stovf(mh.scan.init_bed_position)
	writeu,unit,out & incr,wb,4
	out = stovf(mh.scan.bed_offset(*))
	writeu,unit,out & incr,wb,60
	out = stovf(mh.scan.plane_separation)
	writeu,unit,out & incr,wb,4
	out = [mh.scan.lwr_sctr_thresh, mh.scan.true_thresh]
	byteorder,out & writeu,unit,out & incr,wb,6
	out = stovf(mh.scan.collimator)
	writeu,unit,out & incr,wb,4
	out = byte(mh.file.user_process_code)
	fill_arr,out,10 & writeu,unit,out & incr,wb,10
	out = (mh.scan.acquisition_mode)
	byteorder,out & writeu,unit,out & incr,wb,2
	out = bytarr(38) & writeu,unit,out & incr,wb,38
		
	if(keyword_set(FILE)) then free_lun,unit
	return,!ECAT.OK

io_error : set_ecat_error,1,"WMH",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RSH_IMG
;	RSH_IMG READS THE IMAGE SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT. RSH WRITES THE SUBHEADER INFO INTO SH AND RETURNS !ECAT.OK
;	ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function   rsh_img,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_IMG",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_img
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)+126L
	   in=1 & readu,unit,in & byteorder,in
	   shead.data_type = in
	   readu,unit,in & byteorder,in
	   shead.num_dimensions = in 
	   readu,unit,in & in=intarr(2)
	   readu,unit,in & byteorder,in
	   shead.dimension_1 = in(0)
	   shead.dimension_2 = in(1)
	   in=intarr(12) & readu,unit,in 
	   in=lonarr(4) & readu,unit,in & in=vtosf(in)
	   shead.x_origin = in(0)
	   shead.y_origin = in(1)
	   shead.recon.recon_scale = in(2)
	   shead.quant_scale = in(3)
	   in=intarr(2) & readu,unit,in & byteorder,in
	   shead.image_min = in(0)
	   shead.image_max = in(1)
	   in=intarr(2) & readu,unit,in & in=lonarr(2)
	   readu,unit,in & in=vtosf(in)
	   shead.pixel_size = in(0)
	   shead.slice_width = in(1)
	   in=lonarr(2) & readu,unit,in & byteorder,in,/lswap
	   shead.frame_duration = in(0)
	   shead.frame_start_time = in(1)
	   in=1 & readu,unit,in & byteorder,in
	   shead.slice_location = in
	   in=intarr(4) & readu,unit,in & byteorder,in
	   shead.recon.recon_start(*,0) = in(0:2)
	   shead.recon.recon_duration = in(3)
	   in=intarr(13) & readu,unit,in
	   in=1 & readu,unit,in & byteorder,in
	   shead.recon.filter_code = in
	   in=lonarr(3) & readu,unit,in & byteorder,in,/lswap
	   shead.recon.scan_matrix_num = in(0)
	   shead.recon.norm_matrix_num = in(1)
	   shead.recon.atten_cor_matrix_num = in(2)
	   in=intarr(23) & readu,unit,in
	   in=lonarr(5) & readu,unit,in & in=vtosf(in)
	   shead.image_rotation = in(0)
	   shead.plane_eff_corr_fctr = in(1)
	   shead.decay_corr_fctr = in(2)
	   shead.loss_corr_fctr = in(3)
	   shead.intrinsic_tilt = in(4)
	   in=intarr(32) & readu,unit,in
	   in=1 & readu,unit,in & byteorder,in
	   shead.processing_code = in
	   readu,unit,in & in=intarr(4) & readu,unit,in & byteorder,in
	   shead.quant_units = in(0)
	   shead.recon.recon_start(*,1) = in(1:3)
	   in=lonarr(8) & readu,unit,in & in=vtosf(in)
	   shead.ecat_calibration_fctr = in(0)
	   shead.well_counter_cal_fctr = in(1)
	   shead.recon.filter_params = in(2:7)
	   in=bytarr(40) & readu,unit,in & in=string(in)
	   shead.annotation = in	
	   sh=[sh,shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_IMG",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WSH_IMG
;	WSH_IMG WRITES THE IMAGE SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND !ECAT.ERROR
;	OTHERWISE.
;
function  wsh_img,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"WSH_IMG",UNIT=unit
	   return,!ECAT.ERROR
	endif
	
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)
	   wb = 0L
	   out = bytarr(126) & writeu,unit,out & incr,wb,126
	   out = [sh(i).data_type, sh(i).num_dimensions]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(2) & writeu, unit,out & incr,wb,2
	   out = [sh(i).dimension_1, sh(i).dimension_2]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(24) & writeu,unit,out & incr,wb,24
	   out = [sh(i).x_origin, sh(i).y_origin, $
	          sh(i).recon.recon_scale, sh(i).quant_scale]
	   out = stovf(out) & writeu,unit,out & incr,wb,16
	   out = [sh(i).image_min, sh(i).image_max]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(4) & writeu,unit,out & incr,wb,4
	   out = [sh(i).pixel_size, sh(i).slice_width]
	   out = stovf(out) & writeu,unit,out & incr,wb,8
	   out = [sh(i).frame_duration, sh(i).frame_start_time]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,8
	   out = [sh(i).slice_location, sh(i).recon.recon_start(*,0)]
	   byteorder,out & writeu,unit,out & incr,wb,8
	   out = sh(i).recon.recon_duration
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,4
	   out = bytarr(24) & writeu,unit,out & incr,wb,24
	   out = sh(i).recon.filter_code
	   byteorder,out & writeu,unit,out & incr,wb,2
	   out = [sh(i).recon.scan_matrix_num, $
		  sh(i).recon.norm_matrix_num, $
	          sh(i).recon.atten_cor_matrix_num]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,12
	   out = bytarr(46) & writeu,unit,out & incr,wb,46
	   out = [sh(i).image_rotation, sh(i).plane_eff_corr_fctr, $
		  sh(i).decay_corr_fctr, sh(i).loss_corr_fctr,     $
		  sh(i).intrinsic_tilt]
	   out = stovf(out) & writeu,unit,out & incr,wb,20
	   out = bytarr(64) & writeu,unit,out & incr,wb,64
	   out = sh(i).processing_code
	   byteorder,out & writeu,unit,out & incr,wb,2
	   out = bytarr(2) & writeu,unit,out & incr,wb,2
	   out = [sh(i).quant_units, sh(i).recon.recon_start(*,1)]
	   byteorder,out & writeu,unit,out & incr,wb,8
	   out = [sh(i).ecat_calibration_fctr, sh(i).well_counter_cal_fctr]
	   out = stovf(out) & writeu,unit,out & incr,wb,8
	   out = sh(i).recon.filter_params(*)
	   out = stovf(out) & writeu,unit,out & incr,wb,24
	   out = byte(sh(i).annotation)
	   fill_arr,out,40 & writeu,unit,out & incr,wb,40
	   out = bytarr(52) & writeu,unit,out & incr,wb,52

	endfor 
	return,!ECAT.OK
 io_error : set_ecat_error,1,"WSH_IMG",UNIT=unit
	    return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RSH_SCN
;	RSH_SCN READS THE SCAN SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT. RSH WRITES THE SUBHEADER INFO INTO SH AND RETURNS !ECAT.OK
;	ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function   rsh_scn,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_SCN",UNIT=unit
	   return,!ECAT.ERROR
	endif

	stat = fstat(unit)
	if(stat.open EQ 0) then begin
           set_ecat_error,6,"RSH_SCN"
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_scn
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)+126L
	   in=intarr(10) & readu,unit,in & byteorder,in
	   shead.data_type = in(0)
	   shead.dimension_1 = in(3)
	   shead.dimension_2 = in(4)
	   shead.smoothing = in(5)
	   shead.processing_code = in(6)
	   in=1L & readu,unit,in & in=vtosf(in)
	   shead.sample_distance = in
	   in=intarr(8) & readu,unit,in
	   in=1L & readu,unit,in & in=vtosf(in)
	   shead.isotope_halflife = in
	   in=1 & readu,unit,in & byteorder,in
	   shead.frame_duration_sec = in
	   in=lonarr(2) & readu,unit,in & byteorder,in,/lswap
	   shead.gate_duration = in(0)
	   shead.r_wave_offset = in(1)
	   in=1 & readu,unit,in
	   in=1L & readu,unit,in & in=vtosf(in)
	   shead.scale_factor = in
	   in=intarr(5) & readu,unit,in & byteorder,in
	   shead.scan_min = in(3)
	   shead.scan_max = in(4)
	   in=lonarr(4) & readu,unit,in & byteorder,in,/lswap
	   shead.prompts = in(0)
	   shead.delayed = in(1)
	   shead.multiples = in(2)
	   shead.net_trues = in(3)
	   in=intarr(52) & readu,unit,in
	   in=lonarr(38) & readu,unit,in
	   shead.cor_singles = vtosf(in(0:15))
	   shead.uncor_singles = vtosf(in(16:31))
	   shead.tot_avg_cor = vtosf(in(32))
	   shead.tot_avg_uncor = vtosf(in(33))
	   hold = in(34:36)
	   byteorder,hold,/lswap
	   shead.total_coin_rate = hold(0)
	   shead.frame_start_time = hold(1)
	   shead.frame_duration = hold(2)
	   shead.loss_correction_fctr = vtosf(in(37))
	   in=lonarr(8) & readu,unit,in & byteorder,in
	   shead.phy_planes = in
	   sh = [sh,shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_SCN",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;WSH_SCN
;	WSH_SCN WRITES THE SCAN SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT.  WSH_SCN RETURNS !ECAT.OK ON SUCCESFUL EXIT AND !ECAT.ERROR
;	OTHERWISE.
;
function   wsh_scn,unit,offset,sh
	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"WSH_SCN",UNIT=unit
	   return,!ECAT.ERROR
	endif

	for i=0,n_elements(offset)-1 do begin
	   wb = 0L
	   point_lun,unit,offset(i)
	   out = bytarr(126) & writeu,unit,out & incr,wb,126
	   out = sh(i).data_type
	   byteorder,out & writeu,unit,out & incr,wb,2
	   out = bytarr(4) & writeu,unit,out & incr,wb,4
	   out = [sh(i).dimension_1, sh(i).dimension_2, $
		       sh(i).smoothing, sh(i).processing_code]
	   byteorder,out & writeu,unit,out & incr,wb,8
	   out = bytarr(6) & writeu,unit,out & incr,wb,6
	   out = stovf(sh(i).sample_distance)
	   writeu,unit,out & incr,wb,4
	   out = bytarr(16) & writeu,unit,out & incr,wb,16
	   out = stovf(sh(i).isotope_halflife)
	   writeu,unit,out & incr,wb,4
	   out = sh(i).frame_duration_sec
	   byteorder,out & writeu,unit,out & incr,wb,2
	   out = [sh(i).gate_duration, sh(i).r_wave_offset]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,8
	   out = bytarr(2) & writeu,unit,out & incr,wb,2
	   out = stovf(sh(i).scale_factor)
	   writeu,unit,out & incr,wb,4
	   out = intarr(5)
	   out(3:4) = [sh(i).scan_min, sh(i).scan_max]
	   byteorder,out & writeu,unit,out & incr,wb,10
	   out = [sh(i).prompts, sh(i).delayed, $
		  sh(i).multiples, sh(i).net_trues]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,16
	   out = bytarr(104) & writeu,unit,out & incr,wb,104
	   out = lonarr(34)
	   out(0:15) = sh(i).cor_singles
	   out(16:31) = sh(i).uncor_singles
	   out(32) = sh(i).tot_avg_cor
	   out(33) = sh(i).tot_avg_uncor
	   out = stovf(out) & writeu,unit,out & incr,wb,136
	   out = [sh(i).total_coin_rate, sh(i).frame_start_time, $
		  sh(i).frame_duration]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,12
	   out = stovf(sh(i).loss_correction_fctr)
	   writeu,unit,out & incr,wb,4
	   out = bytarr(44) & writeu,unit,out & incr,wb,44
	endfor

	return,!ECAT.OK

io_error : set_ecat_error,1,"WSH_SCN",UNIT=unit
	   return,!ECAT.ERROR

end
;	   	
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RSH_NRM
;	RSH_NRM READS THE NORM SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT. RSH WRITES THE SUBHEADER INFO INTO SH AND RETURNS !ECAT.OK
;	ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function   rsh_nrm,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_NRM",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_nrm
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)+126L
	   in=intarr(5) & readu,unit,in & byteorder,in
	   shead.data_type = in(0)
	   shead.dimension_1 = in(3)
	   shead.dimension_2 = in(4)
	   in=intarr(23) & readu,unit,in
	   in=1L & readu,unit,in & in=vtosf(in)
	   shead.scale_factor = in
	   in=intarr(6) & readu,unit,in & byteorder,in
	   shead.norm_hour = in(0)
	   shead.norm_minute = in(1)
	   shead.norm_second = in(2)
	   shead.norm_day = in(3)
	   shead.norm_month = in(4)
	   shead.norm_year = in(5)
	   in=lonarr(2) & readu,unit,in & in=vtosf(in)
	   shead.fov_source_width = in(0)
	   sh = [sh, shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_NRM",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WSH_NRM
;	WSH_RNM WRITES THE NORM SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND !ECAT.ERROR
;	OTHERWISE.
;
;
function   wsh_nrm,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"WSH_NRM",UNIT=unit
	   return,!ECAT.ERROR
	endif

	for i=0,n_elements(offset)-1 do begin
	   wb = 0L
	   point_lun,unit,offset(i)
	   out = bytarr(126) & writeu,unit,out & incr,wb,126
	   out = sh(i).data_type
	   byteorder,out & writeu,unit,out & incr,wb,2
	   out = bytarr(4) & writeu,unit,out & incr,wb,4
	   out = [sh(i).dimension_1, sh(i).dimension_2]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(46) & writeu,unit,out & incr,wb,46
	   out = stovf(sh(i).scale_factor)
	   writeu,unit,out & incr,wb,4
	   out = [sh(i).norm_hour, sh(i).norm_minute, $
		  sh(i).norm_second, sh(i).norm_day, $
		  sh(i).norm_month, sh(i).norm_year]
	   byteorder,out & writeu,unit,out & incr,wb,12
	   out = stovf(sh(i).fov_source_width)
	   writeu,unit,out & incr,wb,4
	   out = bytarr(310) & writeu,unit,out & incr,wb,310
	endfor
	return,!ECAT.OK

io_error : set_ecat_error,1,"WSH_NRM",UNIT=unit
	   return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RSH_ATN
;	RSH_ATN READS THE SCAN SUBHEADER AT OFFSET FROM THE OPEN FILE,
;	UNIT. RSH WRITES THE SUBHEADER INFO INTO SH AND RETURNS !ECAT.OK
;	ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function   rsh_atn,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_ATN",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_atn
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)+126L
	   in=intarr(5) & readu,unit,in & byteorder,in
	   shead.data_type = in(0)
	   shead.attenuation_type = in(1)
	   shead.dimension_1 = in(3)
	   shead.dimension_2 = in(4)
	   in=intarr(23) & readu,unit,in
	   in=lonarr(8) & readu,unit,in & in=vtosf(in)
	   shead.scale_factor = in(0)
	   shead.x_origin = in(1)
	   shead.y_origin = in(2)
	   shead.x_radius = in(3)
	   shead.y_radius = in(4)
	   shead.tilt_angle = in(5)
	   shead.attenuation_coeff = in(6)
	   shead.sample_distance = in(7)
	   sh = [sh, shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_ATN",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WSH_ATN
;	WSH_ATN WRITES THE ATTENUATION SUBHEADER AT OFFSET FROM THE OPEN
; 	FILE, UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND
; 	!ECAT.ERROR OTHERWISE.
;
function   wsh_atn,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"WSH_ATN",UNIT =unit
	   return,!ECAT.ERROR
	endif

	for i=0,n_elements(offset)-1 do begin
	   wb=0L
	   point_lun,unit,offset(i)
	   out = bytarr(126) & writeu,unit,out & incr,wb,126
	   out = [sh(i).data_type, sh(i).attenuation_type]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(2) & writeu,unit,out & incr,wb,2
	   out = [sh(i).dimension_1, sh(i).dimension_2]
	   byteorder,out & writeu,unit,out & incr,wb,4
	   out = bytarr(46) & writeu,unit,out & incr,wb,46
	   out = [sh(i).scale_factor, sh(i).x_origin, $
		  sh(i).y_origin, sh(i).x_radius, sh(i).y_radius, $
		  sh(i).tilt_angle, sh(i).attenuation_coeff, $
		  sh(i).sample_distance]
	   out = stovf(out) & writeu,unit,out & incr,wb,32
	   out = bytarr(298) & writeu,unit,out & incr,wb,298
	endfor

	return,!ECAT.OK

io_error : set_ecat_error,1,"WSH_ATN",UNIT=unit
	   return,!ECAT.ERROR
end


;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; rsh_pm
;	rsh_pm reads THE polar map SUBHEADER AT OFFSET FROM THE OPEN
; 	FILE, UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND
; 	!ECAT.ERROR OTHERWISE.
;
function   rsh_pm,unit,offset,sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"RSH_PM",UNIT=unit
	   return,!ECAT.ERROR
	endif

	shead = !ECAT.sh_pm
	sh = [shead]
	for i=0,n_elements(offset)-1 do begin
	   point_lun,unit,offset(i)
	   in=1 & readu,unit,in & byteorder,in
	   shead.type = in
	   readu,unit,in & byteorder,in
	   shead.num_rings = in 

	   in=intarr(32) & readu,unit,in & byteorder,in
	   shead.sect_per_ring = in

	   in=lonarr(32) & readu,unit,in & in = vtosf(in)
	   shead.ring_pos = in

	   in=intarr(42) & readu,unit,in & byteorder,in
	   shead.ring_angle = in(0:31)
	   shead.start_angle = in(32)
	   shead.long_axis_left = in(33:35)
	   shead.long_axis_right = in(36:38)
	   shead.position_data = in(39)
	   shead.image_min = in(40)
	   shead.image_max = in(41)

	   in=lonarr(5) & readu,unit,in & in = vtosf(in)
	   shead.scale_factor = in(0)
	   shead.x_origin = in(1)
	   shead.y_origin = in(2)
	   shead.rotation = in(3)
	   shead.pixel_size = in(4)

	   in=lonarr(2) & readu,unit,in & byteorder,in,/lswap
	   shead.frame_duration = in(0)
	   shead.frame_start_time = in(1)
	   
	   in=intarr(2) & readu,unit,in & byteorder,in
	   shead.proc_code = in(0)
	   shead.quant_units = in(1)

	   in=bytarr(40) & readu,unit,in & in=string(in)
	   shead.annotation = in	
	  
	   sh=[sh,shead]
	endfor

	sh = sh(1:*)
	return,!ECAT.OK

io_error : set_ecat_error,1,"RSH_PM",UNIT=unit
           return,!ECAT.ERROR
end

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WSH_PM
;	WSH_PM WRITES THE polar map SUBHEADER AT OFFSET FROM THE OPEN
; 	FILE, UNIT.  WSH RETURNS !ECAT.OK ON SUCCESFUL EXIT AND
; 	!ECAT.ERROR OTHERWISE.
;
function   wsh_pm,unit,offset,sh

; written but not really tested yet.

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 3) then begin
	   set_ecat_error,2,"WSH_PM",UNIT =unit
	   return,!ECAT.ERROR
	endif

	for i=0,n_elements(offset)-1 do begin
	   wb=0L
	   point_lun,unit,offset(i)

	   out = [sh(i).type, sh(i).num_rings, sh(i).sect_per_ring]
	   byteorder,out & writeu,unit,out & incr,wb, 68

	   out = [sh(i).ring_pos]
	   out = stovf(out) & writeu,unit,out & incr,wb,128

	   out = [sh(i).ring_angle, sh(i).start_angle, sh(i).long_axis_left,$
			sh(i).long_axis_right, sh(i).position_data, $
			sh(i).image_min, sh(i).image_max]
	   byteorder,out & writeu,unit,out & incr,wb, 84

	   out = [sh(i).scale_factor, sh(i).x_origin, sh(i).y_origin, $
			sh(i).rotation, sh(i).pixel_size]
	   out = stovf(out) & writeu,unit,out & incr,wb,20

	   out = [sh(i).frame_duration, sh(i).frame_start_time]
	   byteorder,out,/lswap & writeu,unit,out & incr,wb,8

	   out = [sh(i).proc_code, sh(i).quant_units]
	   byteorder,out & writeu,unit,out & incr,wb,4

	   out = byte(sh(i).annotation)
	   fill_arr,out,40 & writeu,unit,out & incr,wb,40

	   out = bytarr(160) & writeu,unit,out & incr,wb,160

	endfor

	return,!ECAT.OK

io_error : set_ecat_error,1,"WSH_PM",UNIT=unit
	   return,!ECAT.ERROR
end

;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; PMH
;	PMH PRINTS THE DIFFERENT STRUCTURES THAT COMPOSE THE MAIN
;	HEADER STRUCTURE TO THE SCREEN.
;		   
pro	pmh,mh,FILE=file,PAT=pat,SCAN=scan

	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"PMH"
	   return
	endif

	help,/struct,mh
	print
	if(keyword_set(FILE)) then begin
	   format='("SCAN START -> ",I2,":",I2,":",I2," on ",I2,"/",I2,"/",I4)'
	   print,FORMAT=format,mh.file.scan_start(*,1),mh.file.scan_start([1,0,2],0)
	   print
	   help,/struct,mh.file
	endif
	if(keyword_set(PAT)) then begin
	   print
	   print,"PATIENT INFO"
	   print,"------------"
	   print,"   STUDY NAME        : ",mh.pat(0)
	   print,"   PATIENT ID        : ",mh.pat(1)
	   print,"   PATIENT NAME      : ",mh.pat(2)
	   print,"   PATIENT SEX       : ",mh.pat(3)
	   print,"   PATIENT AGE       : ",mh.pat(4)
	   print,"   PATIENT HEIGHT    : ",mh.pat(5)
	   print,"   PATIENT WEIGHT    : ",mh.pat(6)
	   print,"   PATIENT DEXTERITY : ",mh.pat(7)
	   print,"   PHYSICIAN NAME    : ",mh.pat(8)
	   print,"   OPERATOR NAME     : ",mh.pat(9)
	   print,"   STUDY DESCRIPTION : ",mh.pat(10)
	endif
	if(keyword_set(SCAN)) then begin
	   print
	   help,/struct,mh.scan
	   print
	   format='("GANTRY",10X,":  TILT = ",F9.4,"  ROTATION = ",F9.4)'
	   print,FORMAT=format,mh.scan.gantry
	   format='("TRUES THRESHOLD : LOWER = ",I4,10X,"UPPER = ",I4)'
	   print,FORMAT=format,mh.scan.true_thresh
	   format='("INITIAL BED POSITION    = ",F9.4)'
	   print,FORMAT=format,mh.scan.init_bed_position
	   format='("BED POSITION ",I2,6X,"   = ",F9.4)'
	   for i=1,mh.file.nbeds do begin
	      print,FORMAT=format,i,mh.scan.bed_offset(i-1)
	   end
	endif
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; PSH
;	PSH PRINTS SUBHEADERS TO THE SCREEN
;
pro   psh,sh
		   
	on_error,2
	if(n_params() NE 1) then begin
	   set_ecat_error,2,"PSH"
	   return
	endif

	ind = where(tag_names(sh) EQ "RECON", cnt)
	help,/struct,sh
	if(cnt GT 0) then help,/struct,sh.recon

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RSUBHEADERS
;	RSUBHEADERS READS THE APPROPRIATE TYPE SUBHEADERS IN THE OPEN FILE,
;	UNIT, AT THE GIVEN OFFSETS INTO THE PARAMETER SH. RSUBHEADERS RETURNS  
;	!ECAT.OK ON SUCCESSFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function rsubheaders,ftype,unit,offsets,sh,scales

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"RSUBHEADERS",UNIT=unit
	   return,!ECAT.ERROR
	endif

	case !ECAT.file_types(ftype) of

	   "IMG" : $
	      begin
		stat = rsh_img(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).quant_scale
	      end

	   "SCN" : $
	      begin
		stat = rsh_scn(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "NRM" : $
	      begin
		stat = rsh_nrm(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "ATN" : $
	      begin
		stat = rsh_atn(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   "PM" : $
	      begin
		stat = rsh_pm(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	        scales = sh(0:n_elements(sh)-1).scale_factor
	      end

	   else  : begin
	      set_ecat_error,15,"RSUBHEADERS",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	return,!ECAT.OK

io_error : set_ecat_error,1,"RSUBHEADERS",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WSUBHEADERS
;	WSUBHEADERS WRITES THE APPROPRIATE TYPE SUBHEADERS IN THE OPEN FILE,
;	UNIT, AT THE GIVEN OFFSETS FROM THE PARAMETER SH. WSUBHEADERS RETURNS  
;	!ECAT.OK ON SUCCESSFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function wsubheader,unit,ftype,offsets,sh,SCALE=scale

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 4) then begin
	   set_ecat_error,2,"WSUBHEADER",UNIT=unit
	   return,!ECAT.ERROR
	endif
	sf = 0
	nscales = n_elements(sh)
	if(keyword_set(SCALE)) then sf = 1
	if(NOT sf) then scale = 1.0
	nsubs = n_elements(sh)
	if(sf) then nscales = n_elements(scale)
	if(nscales NE nsubs) then begin
	   set_ecat_error,16,"WSUBHEADER",UNIT=unit
	   return,!ECAT.ERROR
	endif

	case !ECAT.file_types(ftype) of

	   "IMG" : $
	      begin
	        if(sf) then sh(*).quant_scale = scale
		stat = wsh_img(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	      end

	   "SCN" : $
	      begin
	        if(sf) then sh(*).scale_factor = scale
		stat = wsh_scn(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	      end

	   "NRM" : $
	      begin
	        if(sf) then sh(*).scale_factor = scale
		stat = wsh_nrm(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	      end

	   "ATN" : $
	      begin
	        if(sf) then sh(*).scale_factor = scale
		stat = wsh_atn(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	      end

	   "PM" : $
	      begin
		stat = wsh_pm(unit,offsets,sh)
	        if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	      end

	   else  : begin
	      set_ecat_error,15,"WSUBHEADER",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	return,!ECAT.OK

io_error : set_ecat_error,1,"WSUBHEADERS",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;	**********   READING/WRITING UTILITIES   **********
;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RDATA
;	RDATA READS INTO THE PARAMETER DATA THE MATRIX DATA AT THE 
; 	POSITIONS GIVEN BY OFFSETS IN THE OPEN FILE REPRESENTED BY
;	UNIT. RDATA RETURNS !ECAT.OK ON SUCCESSFUL EXIT AND 
;	!ECAT.ERROR OTHERWISE
;
function   rdata,dtype,unit,offsets,xd,yd,zd,data,SCALE=scale

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"RDATA",UNIT=unit
	   return,!ECAT.ERROR
	endif

	sf = keyword_set(SCALE)
	if(NOT sf) then $
	   scale = replicate(1, zd)

	case !ECAT.data_types(dtype) of
	   "VAX_I2" :  $
	   begin
	      if(sf) then 		 $
	         data = fltarr(xd,yd,zd) $
	      else $
	         data = intarr(xd,yd,zd)
	      d = intarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         byteorder,d
	         data(*,*,i) = d*scale(i)
	      endfor
	   end

	   "VAX_R4" : $
	   begin
	      data = fltarr(xd,yd,zd)
	      d = lonarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         data(*,*,i) = vtosf(d)*scale(i)
	      endfor
	   end

	   "SUN_I2" : $
	   begin
	      if(sf) then 		 $
	         data = fltarr(xd,yd,zd) $
	      else $
	         data = intarr(xd,yd,zd)
	      d = intarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         data(*,*,i) = d*scale(i) 
	      endfor
	   end

	   "SUN_R4" : $
	   begin
	      data = fltarr(xd,yd,zd)
	      d = fltarr(xd,yd)
	      for i=0,n_elements(offsets)-1 do begin
	         point_lun,unit,offsets(i)+!ECAT.blksize
	         readu,unit,d
	         data(*,*,i) = d*scale(i)
	      endfor
	   end

	   else : begin
	      set_ecat_error,4,"RDATA",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	d = 0
	return,!ECAT.OK

io_error : set_ecat_error,1,"RDATA",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WDATA
;	WDATA WRITES DATA INTO THE FILE REPRESENTED BY UNIT.
;	WDATA RETURNS !ECAT.OK ON SUCCESSFUL EXIT AND !ECAT.ERROR OTHERWISE
;
function wdata,unit,ftype,dtype,offsets,data

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 5) then begin
	   set_ecat_error,2,"WDATA",UNIT=unit
	   return,!ECAT.ERROR
	endif

	s = size(data(*,*,0))
	size = s(1)*s(2)
	pad = 0

	case !ECAT.data_types(dtype) of
	   "VAX_I2" :  $
	   begin
	      pad = 0L  
	      if(size*2L LT !ECAT.BLKSIZE) then begin
	         leftover = (!ECAT.BLKSIZE/2L)-size
	         pad = intarr(s(1), leftover/s(1))
	      endif else begin
	         r = (size*2L) MOD !ECAT.BLKSIZE
	         if(r GT 0) then begin
	            leftover = (!ECAT.BLKSIZE/2L)-r
	            pad = intarr(s(1), leftover/s(1))
	         endif
	      endelse
	      byteorder,data
	      for i=0,n_elements(offsets)-1 do begin
	         d = data(*,*,i)
	         if(pad) then d = [[data(*,*,i)],[pad]]
	         a = assoc(unit,d,offsets(i))
	         a(0) = d
	      endfor             
	   end

	   "VAX_R4" : $
	   begin
	      pad = 0L  
	      if(size*4L LT !ECAT.BLKSIZE) then begin
	         leftover = (!ECAT.BLKSIZE/4L)-size
	         pad = fltarr(s(1), leftover/s(1))
	      endif else begin
	         r = (size*4L) MOD !ECAT.BLKSIZE
	         if(r GT 0) then begin
	            leftover = (!ECAT.BLKSIZE/2L)-r
	            pad = fltarr(s(1), leftover/s(1))
	         endif
	      endelse
	      for i=0,n_elements(offsets)-1 do begin
	         d = stovf(data(*,*,i))
	         if(pad) then d = [[d],[pad]]
	         a = assoc(unit,d,offsets(i))
	         a(0) = d
	      endfor             
	   end

	   "SUN_I2" : $
	   begin
	      pad = 0L  
	      if(size*2L LT !ECAT.BLKSIZE) then begin
	         leftover = (!ECAT.BLKSIZE/2L)-size
	         pad = intarr(s(1), leftover/s(1))
	      endif else begin
	         r = (size*2L) MOD !ECAT.BLKSIZE
	         if(r GT 0) then begin
	            leftover = (!ECAT.BLKSIZE/2L)-r
	            pad = intarr(s(1), leftover/s(1))
	         endif
	      endelse
	      for i=0,n_elements(offsets)-1 do begin
	         d = data(*,*,i)
	         if(pad) then d = [[data(*,*,i)],[pad]]
	         a = assoc(unit,d,offsets(i))
	         a(0) = d
	      endfor        
	   end

	   "SUN_R4" : $
	   begin
	      pad = 0L  
	      if(size*4L LT !ECAT.BLKSIZE) then begin
	         leftover = (!ECAT.BLKSIZE/4L)-size
	         pad = fltarr(s(1), leftover/s(1))
	      endif else begin
	         r = (size*4L) MOD !ECAT.BLKSIZE
	         if(r GT 0) then begin
	            leftover = (!ECAT.BLKSIZE/2L)-r
	            pad = fltarr(s(1), leftover/s(1))
	         endif
	      endelse
	      for i=0,n_elements(offsets)-1 do begin
	         d = data(*,*,i)
	         if(pad) then d = [[data(*,*,i)],[pad]]
	         a = assoc(unit,d,offsets(i))
	         a(0) = d
	      endfor        
	   end

	   else : begin
	      set_ecat_data,0,"WDATA",UNIT=unit
	      return,!ECAT.ERROR
	   end
	endcase

	return,!ECAT.OK

io_error : set_ecat_error,1,"WDATA",UNIT=unit
           return,!ECAT.ERROR

end

;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RPMDATA
;	RPMDATA READS THE POLAR MAP DATA FROM THE FILE REPRESENTED BY UNIT.
;	RPMDATA RETURNS !ECAT.OK ON SUCCESSFUL EXIT AND !ECAT.ERROR OTHERWISE
;
function rpmdata, unit, sh, data

    print, "In rpmdata"
    size = sh(0).num_rings + total(sh(0).sect_per_ring)
    d = intarr(size)
    readu, unit, d
    data = d * sh(0).scale_factor

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WMATS
;	WMATS WRITES SUBHEADER AND DATA INTO THE FILE REPRESENTED BY UNIT.
;	WMATS RETURNS !ECAT.OK ON SUCCESSFUL EXIT AND !ECAT.ERROR OTHERWISE
;
function wmats,unit,ftype,sh,data,mats,dir,dirinfo,SH_ONLY=sh_only

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"WMATS",UNIT=unit
	   return,!ECAT.ERROR
	endif
	
	dtype = sh(0).data_type
	s = size(data(*,*,0))
	scale = 0
	
	nblks = data_size(sh(0),s(n_elements(s)-1))+1L
	if((!ECAT.file_types(ftype) EQ "IMG") OR 		$
	   (!ECAT.file_types(ftype) EQ "SCN")) then		$
	   scale = dscale(data)					$
	else							$
	   scale = replicate(1.0,n_elements(mats))

	for i=0,n_elements(mats)-1 do begin
	   ind = where(dir(0,*) EQ mats(i), cnt)
	   if(cnt EQ 0) then begin
	      nextblk = nextfree(unit,mats(i),nblks,dir,dirinfo)
	      if(nextblk EQ !ECAT.ERROR) then begin
	         set_ecat_error,0,"WMATS"
	         return,!ECAT.ERROR
	      endif
	      off = (nextblk-1)*!ECAT.BLKSIZE
	   endif else begin
	      off = dir(4,ind)
	   endelse
	   if(NOT keyword_set(SH_ONLY)) then begin
	      if(!ECAT.file_types(ftype) EQ "IMG") then begin
	         imin = 0
	   	 sh(i).image_max = max(data(*,*,i), min=imin)
	         sh(i).image_min = imin
	      endif
	      if(!ECAT.file_types(ftype) EQ "SCN") then begin
	         smin = 0
	         sh(i).scan_max = max(data(*,*,i), min=smin)
	         sh(i).scan_min = smin
	      endif
	   endif
	   stat = wsubheader(unit,ftype,off,sh(i),SCALE=scale(i))
	   if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	   if(NOT keyword_set(SH_ONLY)) then begin
	      stat = wdata(unit,ftype,dtype,off+!ECAT.BLKSIZE,data(*,*,i))
	      if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	   endif
	endfor

	return,!ECAT.OK

io_error : set_ecat_error,1,"WMATS",UNIT=unit
           return,!ECAT.ERROR

end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; RECAT
;	RECAT READS THE GIVEN F,P,G,D,B (FRAME, PLANES, GATES, DATA, BEDS)
;	DATA FROM THE FILE IN THE FILE PARAMETER. A STRUCTURE CONTAINING
;	INFORMATION ABOUT THE FILE AND DATA ARE RETURNED IN THE PARAMTER
;	ECAT_DATA. THE KEYWORD NO_SCALE PREVENTS THE SCALING OF THE DATA.
;	THE KEYWORD NO_DATA RETURNS NO DATA, BUT JUST HEADER INFO. RECAT
;	RETURNS !ECAT.OK ON SUCCESFUL EXIT AND !ECAT.ERROR OTHERWISE.
;
function   recat,file,f,p,g,d,b,ecat_data, 			$
	         NO_SCALE=no_scale,NO_DATA=no_data,ZOOM=zoom,	$
		 MAIN=main,DIR=dir,DIRINFO=dirinfo,		$
		 ANAT_SORT=anat_sort			

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 7) then begin
	   set_ecat_error,2,"RECAT",FILE=file
	   return,!ECAT.ERROR
	endif
	ecat_data = 0

	openr,unit,file,/get_lun
	stat = rmh(unit,mh)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(MAIN)) then begin
	   ecat_data = {mh:mh}
	   return,ecat_data
	endif
	stat = rdir(unit,directory,dinfo)
	if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	if(keyword_set(DIR)) then dir = directory
	if(keyword_set(DIRINFO)) then dirinfo = dirinfo
	if(keyword_set(ANAT_SORT)) then begin
	   if(anat_sort(directory,mh) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	endif
	dvals = diffval(directory(0,*))
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	expand,f,p,g,d,b,dvals
	if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	f=fix(f) & p=fix(p) & g=fix(g) & d=fix(d) & b=fix(b)
	mats = mkmats(f,p,g,d,b,directory)
	if((mats(0) EQ !ECAT.ERROR) ) then begin
	   set_ecat_error,3,"RECAT",UNIT=unit
	   return,!ECAT.ERROR
	endif
	n=n_elements(mats(0,*))
	name = file
	ftype = mh.file.file_type
	dtype = mh.file.data_type
	if(rsubheaders(ftype,unit,mats(1,*),sh,scales) EQ !ECAT.ERROR) then $
	   return,!ECAT.ERROR
	data = 0
	xd = 0
	yd = 0
	zd = n
	zf = 1

	; set the data size values for types other than the polar map
	if (ftype lt 5) then begin
	  xd = sh(0).dimension_1
	  yd = sh(0).dimension_2
	endif

	if(NOT keyword_set(NO_DATA)) then begin

	   ; handle special case of reading polar map data
	   if (ftype eq 5) then begin
	      stat = rpmdata(unit, sh, data)
	   endif else begin

	     if(keyword_set(NO_SCALE)) then 			  $
	        stat = rdata(dtype,unit,mats(1,*),xd,yd,zd,data)  $
	     else 						  $ 
	        stat = rdata(dtype,unit,mats(1,*),xd,yd,zd,data,SCALE=scales)
	     if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR
	     if(keyword_set(ZOOM)) then begin
	        z = [zoom*xd, zoom*yd]
	        if(zoom LT 1) then 			$
	           m = [xd mod z(0), yd mod z(1)]	$
	        else					$
	           m = [zoom*xd mod xd, zoom*yd mod yd]
	        if(total(m) GT 0) then begin
	           z = [xd,yd]
	           zoom = 1
	        endif
	        d = make_array(z(0),z(1),zd,SIZE=size(data(*,*,0)))
	        xd = z(0) & yd = z(1)
	        for i=0,zd-1 do d(*,*,i) = rebin(data(*,*,i),z(0),z(1))
	        data = temporary(d)
	        zf = zoom
	     endif
	   endelse
	endif
	ecat_data = {fname:name, 		$
		     ftype:ftype,		$
		     dtype:dtype,		$
		     nmats:n,			$
		     mats:mats,			$
		     xdim:xd,			$
		     ydim:yd,			$
		     zdim:n_elements(sh),	$
	             zoom:zf,			$
		     mh:mh,			$
		     sh:sh,			$
		     data:data               	$
		    }

	free_lun,unit
	return,!ECAT.OK

io_error : set_ecat_error,0,"RECAT",UNIT=unit
           return,!ECAT.ERROR
end
;
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; WECAT
;	WECAT WRITES THE MAIN HEADER, SUBHEADERS, AND DATA INTO FILE.
; 	WECAT RETURNS !ECAT.OK ON SUCCESS AND !ECAT.ERROR ON FAILURE.
;
function wecat,file,f,p,g,d,b,sh,data,MH=mh,NEW=new,ONLY_MH=only_mh,ONLY_SH=only_sh

	on_error,2
	on_ioerror,io_error
	if(n_params() NE 8) then begin
	   set_ecat_error,1,"WECAT",FILE=file
	   return,!ECAT.ERROR
	endif

	mhkey = keyword_set(MH)
	newkey = keyword_set(NEW)
	shonlykey = keyword_set(SH_ONLY)
	if((NOT mhkey) AND newkey) then begin
	   set_ecat_error,1,"WECAT",FILE=file
	   return,!ECAT.ERROR
	endif
	if(newkey AND shonlykey) then begin
	   set_ecat_error,1,"WECAT",FILE=file
	   return,!ECAT.ERROR
	endif


	if(newkey) then begin
	   openw,unit,file,/get_lun
	   if(wmh(unit,mh) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	   if(mknewdirblk(unit,dir,dirinfo,/NEW) EQ !ECAT.ERROR) then $
	      return,!ECAT.ERROR
	endif else begin
	   openu,unit,file,/get_lun
	   if(rdir(unit,dir,dirinfo) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	   if(mhkey) then 							$
	      if(wmh(unit,mh) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	endelse
	
	if(NOT keyword_set(ONLY_MH)) then begin

	   dvals = [n_elements(f), n_elements(p), n_elements(g),	$
	            n_elements(d), n_elements(b)]
	   expand,f,p,g,d,b,dvals
	   if(dvals(0) EQ !ECAT.ERROR) then return,!ECAT.ERROR
	   if(newkey) then						$
	      mats = mkmats(f,p,g,d,b,dir,/NEW)				$
	   else								$
	      mats = mkmats(f,p,g,d,b,dir)
	   if(mats(0) EQ !ECAT.ERROR) then begin
	      set_ecat_error,0,"WECAT",UNIT=unit
	      return,!ECAT.ERROR
	   endif
	   mats = mats(0,*)
	   if(n_elements(mats) NE n_elements(sh)) then begin
	      set_ecat_error,9,"WECAT",UNIT=unit
	      return,!ECAT.ERROR
	   endif

	   ndatablks = 0
	   scales = 0
	   if(NOT shonlykey) then begin
	      s = size(data)
	      if((s(0) LT 2) OR (s(0) GT 3)) then begin
	         set_ecat_error,10,"WECAT",UNIT=unit
	         return,!ECAT.ERROR
	      endif
	      if(s(0) EQ 2) then begin
	         if(n_elements(mats) NE 1) then begin	      
	            set_ecat_error,11,"WECAT",UNIT=unit
	            return,!ECAT.ERROR
	         endif
	      endif else begin
	         if(n_elements(mats) NE s(3)) then begin
	            set_ecat_error,11,"WECAT",UNIT=unit
	            return,!ECAT.ERROR
	         endif
	      endelse
	      if(newkey) then begin
	         ftype = mh.file.file_type
	      endif else begin
	         m = matval(dir(0,0))
	         if(m(0) EQ !ECAT.ERROR) then begin
	            set_ecat_error,12,"WECAT",UNIT=unit
	            return,!ECAT.ERROR
	         endif
	         stat = recat(file,m(0),m(1),m(2),m(3),m(4),d,/NO_DATA)
	         if(stat EQ !ECAT.ERROR) then begin
	            set_ecat_error,0,"WECAT",UNIT=unit
	            return,!ECAT.ERROR
	         endif
	         if((s(1)*s(2) NE d.xdim*d.ydim) OR		$
	            (sh(0).data_type NE d.mh.file.data_type)) then begin
	            set_ecat_error,13,"WECAT",UNIT=unit
	            return,!ECAT.ERROR
	         endif
	         ftype = d.mh.file.file_type
	      endelse
	      stat = wmats(unit,ftype,sh,data,mats(0,*),dir,dirinfo,SH_ONLY=shonlykey)
	      if(stat EQ !ECAT.ERROR) then return,!ECAT.ERROR

	   endif
	endif

	free_lun,unit
	return,!ECAT.OK

io_error : set_ecat_error,1,"WECAT",UNIT=unit
           return,!ECAT.ERROR
	
end

