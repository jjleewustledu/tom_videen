;----------------------------
;Some useful file routines
;J. Maldjian
;---------------------------

pro close_lun,unit
close,unit
free_lun,unit
end


pro extract_fname, path,fname
;------------------------------------------		
;extract filename part of pathname
;-----------------------------------------

i=0
chrep = !separator
iver = !version
if (iver.os eq 'MacOS') then chrep = ':'
while (i ne -1) do begin
	first=i
	i=strpos(path, chrep, i)
	if (i ne -1) then i=i+1
	endwhile
;return, strmid(path,first,strlen(path)-i+1)
fname  = strmid(path,first,strlen(path)-i+1)

end
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


pro extract_pname,path,pname,parents = parents

;-----------------------------------------------------
;extracts the pathname
;parents = array of individual folders summing to path
;---------------------------------------------------
parents = ''
i=0
chrep = !separator
iver = !version
if (iver.os eq 'MacOS') then chrep = ':'
dstruct = intarr(30)
j = -1
while (i ne -1) do begin
	first=i
	i=strpos(path, chrep, i)
	if (i ne -1) then begin
		j = j + 1
		i = i + 1
		dstruct(j) = i
	endif
endwhile
pname = strmid(path,0,first-1)
pname = pname + chrep
if j ge 0  then begin
	parents = strarr(j + 1)
	for i = 1,j do begin
	parents(i - 1) = strmid(path,dstruct(i - 1),dstruct(i) - dstruct(i -1) - 1)
	endfor
	parents(j) = strmid(path,dstruct(j),strlen(path) - dstruct(j))

endif
end

;**************************************************************************


function var_set, x		;true if variable is set, even if 0
return, n_elements(x) gt 0
end

;**************************************************************************


function replace_string,line,char = char,rep=rep
return,line
end

function replace_string,inline,char = char,rep=rep
line=inline
if not(keyword_set(rep)) then rep = ''
q = strpos(line,char)
if q ge 0 then line = strmid(line,0,q) + rep + strmid(line,q + 1,strlen(line))
q = strpos(line,char)
if q ge 0 then line = replace_string(line,char = char,rep=rep)
return,line
end


;**************************************************************************

function idl_datatype, adt	;converts analyze datatype to idl datatype
case adt of
 2:return,1
 4:return,2
 8:return,3
16:return,4
32:return,5
64:return,5
132:return,2
else:return,0
endcase
end

function typecode, x		;returns the idl type code
s = size(x)
return, s(s(0)+1)
end

function an_datatype, idt	;extracts idl datatype and converts to analyze datatype
case typecode(idt) of
1:return, 2
2:return, 4
3:return, 8
4:return,16
5:return,64
else:return,0
endcase
end

function bytesize, tcode	;returns the size per element in bytes of an idl type code
case tcode of
1:return,1
2:return,2
3:return,4
4:return,4
5:return,8
else:return,0
endcase
end

function bitsize,image	;returns the size per element in bits for any variable (an.bits)
bits_size  = 8 * bytesize(idl_datatype(an_datatype(image)))
return,bits_size
end

function is_tag, struc, tag	;true if tag belongs to struc
return, total(where(struc eq strupcase(tag))) gt -1
end


pro helpstruc, s	; debugging tool to get help on all fields of a structure
t = tag_names(s)
w = max(strlen(t))
for i = 0,n_elements(t)-1 do begin
	print, format='($,a'+string(w)+'," ")', t(i)
	r=execute('help,s.'+t(i))
endfor
end

pro parse_filename, filename=filename, path=dirname, basename=basename, extension=extension
filename = strtrim(filename,2)
spawn,'dirname '+filename, dirname
spawn,'basename '+filename, basename
dirname   = dirname(0) + !separator
basename  = basename(0)
dotpos    = rstrpos(basename, '.')
if dotpos eq -1 then begin
	extension = ''
	return
endif
extension = strmid(basename, dotpos+1, 10)
basename  = strmid(basename, 0, strlen(basename)-strlen(extension)-1)
return
end



function read_txtfile, filename, nostrip = nostrip
nline = ''
line = ''
k = findfile(filename(0),count = fcount)
if fcount gt 0 then begin
	openr,unit,filename(0),/get_lun
	fstruc = fstat(unit)
	;print, fstruc.size - (32767.0)
	point_lun,unit,(fstruc.size - (32767.0) ) > 0
	while not(eof(unit)) do begin
		readf,unit,nline
		if (strlen(strtrim(nline(0),2)) gt 0) and (strmid(nline(0),0,1) ne ';') then line = [line,nline(0)] else $
			if keyword_set(nostrip) then line = [line,nline(0)]
	
	endwhile
	close_lun,unit
endif
if n_elements(line) gt 1 then line = line(1:*)
return,line
end 





FUNCTION valid_dir, dir
  spawn, ['test -d "'+dir +'" -a -x "'+dir+'" ; echo $?'], result, /sh
  return, (not fix(result(0)) )
END

;------------------------------------------------------------------------------
;       procedure GETDIR
;------------------------------------------------------------------------------
; This routine finds the files or directories at the current directory level.
; It must be called with either files or directories as a keyword.
;------------------------------------------------------------------------------

function getdirs

WIDGET_CONTROL, /HOUR

  retval = ['../']
  SPAWN, ["/bin/sh", "-c", "ls -laL 2> /dev/null"], /NOSHELL, results
  numfound = N_ELEMENTS(results)
  IF(KEYWORD_SET(results)) THEN BEGIN                   ;extension of ".dir"
    firsts = STRUPCASE(STRMID(results, 0, 1))
    dirs = (where(firsts EQ "D", found))
    IF (found GT 0) THEN BEGIN
      results = results(dirs)
      spaceinds = WHERE(BYTE(results(0)) EQ 32)
      spaceindex = spaceinds(N_ELEMENTS(spaceinds)-1)
      retval = [retval, STRMID(results, spaceindex + 1, 100)]
      retval = retval(WHERE( (retval ne '.')and(retval ne '..')) )
    ENDIF
  ENDIF
RETURN, retval
END ; function getdirs

;------------------------------------------------------------------------------

pro badboy, text,header,quiet = quiet,bell = bell

;This program displays errors in a widget window

modal = 1
if keyword_set(quiet) then modal = 0

IF N_PARAMS(0) lt 2 then begin
	bel = string(7B)
	print,bel
	header = 'ERROR'
endif

if keyword_set(bell) then begin
	bel = string(7B)
	print,bel
endif

minsize = 20
miny = 1
szm = size(text)
if szm(0) eq 0 then begin
	if strlen(text) gt minsize then minsize = strlen(text)
endif

if (szm(0) ge 1) then begin
	for i = 0,szm(1) - 1 do begin
		if strlen(text(i)) gt minsize then minsize = strlen(text(i))
	endfor
endif

sz2 = size(szm)
if (sz2(1) gt 3) then miny = szm(1)

base = widget_base(title = header, /row)
col1 = widget_base(base, /frame, /column, space = 40)
w1 = widget_text(col1, value = text, xsize = minsize,ysize = miny)
w1 = widget_button(col1, value = "OK")
widget_control, base, /realize
xmanager, 'badboy', base, MODAL = MODAL

end

;;----------------------------------------------------------------------------



pro badboy_event,ev

type = tag_names(ev, /structure)

if (type eq 'WIDGET_BUTTON') then begin
	widget_control, ev.id, get_value = value, /hourglass
	if (value eq 'OK') then  begin
		 widget_control, /destroy, ev.top
	endif
endif

end





