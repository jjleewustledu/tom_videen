;            File: $RCSfile: xload.pro,v $
;          Author: $Author: williams $
;         version: $Revision: 2.0 $
;   Last Modified: $Date: 1997/01/26 19:23:21 $
;+
; NAME: XLOAD.PRO
;
; PURPOSE: To load image data from bruker or GE Signa 5.x images
;
; CATEGORY: File I/O
;
; CALLING SEQUENCE:
;	result = XLOAD(path)
;
; INPUTS:
;      path : starting path to the image database
;
; OPTIONAL INPUTS: none
;      
;
; KEYWORD PARAMETERS:
;      group     = Group leader for calls from other widgets
;      data_info = A named variable which will hold the data_info
;                  structure. 
;         IDL> help,/st,data_info
;                  For more information.
;      SIGNA     = set keyword for reading Signa 5.x data
;      BRUKER    = set keyword for reading Bruker data. Default is to
;                  read Signa data.
;
; OUTPUTS:
;      The data.  If widget was dismissed then -1 is returned
;
; COMMON BLOCKS: none
;
; SIDE EFFECTS: none
;
; PROCEDURE:
;      result = xload(path, /Signa)
;
; MODIFICATION HISTORY:
;     $Log: xload.pro,v $
;   Revision 2.0  1997/01/26  19:23:21  williams
;   New calling procedure.
;
;   Revision 1.6  1997/01/16  23:18:34  williams
;   Fixed memory leak associated with listPTR
;
;   Revision 1.5  1997/01/15  14:20:05  williams
;   Fixed bug in when going back to list series from images.
;
;   Revision 1.4  1997/01/05  15:33:08  williams
;   Fixed bug in reading last file in a GE series.
;
;   Revision 1.3  1997/01/02  19:07:21  williams
;   Minor bug fixes. Filter file list to datatype.
;
;   Revision 1.2  1997/01/02  18:49:11  williams
;   Removed all common blocks.
;   Added keywords for data type.
;   Now called with path to database.
;
;   Revision 1.1  1996/12/31  00:08:56  williams
;   Initial revision
;
;
;-
;------------------
pro inform, id, message

if (widget_info(/active) and widget_info(id, /realized)) then $
  widget_control, id, set_value = message $
else $
  print, message
end

;-----------------------------------------------------------------------------
pro xload_erase, ids, id_start

foo = size(ids)
id_end = foo(1)-1
for i = id_start,id_end do $
  widget_control, ids(i), set_value=''

end

;--------------------------------------

pro update_xload, linfo

path = linfo.curpath
exist = direxist(path)
if exist(0) eq 1 then begin
    cd, current = curDir
    cd, path

    if linfo.findfile eq 1 then begin
        case linfo.datatype of
            'Bruker' : list = findfile('2dseq', count = count)
            'GE'     : list = findfile('I.*', count = count)
            else:
        endcase
    endif else begin
        list = findfile()
        dirs = direxist(list)
        found = where(dirs eq 1, count)
        if count ne 0 then list = list(found)
    endelse
    if count ne 0 then filelist = list $
    else filelist = ''
    
    widget_control,linfo.li_files, set_val=filelist
    
    set_handle_value, linfo.listPTR, filelist, /no_copy
    cd, curDir
endif
linfo.findfile = 0

end

;--------------------------------------

pro navigateEvent, ev

stash=ev.top
WIDGET_CONTROL, stash, GET_UVALUE=linfo

inform, linfo.l_error, ''

foo = where(ev.id eq linfo.buttons, count)
if count ne 0 then begin

; get the field corresponding to the button
    widget_control, linfo.fields(foo), get_val = temp
    
; Does the curpath contain that field?
    index = strpos(linfo.curpath,temp(0))
    
; Go back one level
    linfo.curpath = strmid(linfo.curpath,0,index-1)
    linfo.where = foo(0)
    
; Update the filelist
    update_xload,linfo
    
; Erase the old fields
    xload_erase, linfo.fields, foo(0)
    
endif

if linfo.where gt 4 then $
  widget_control,linfo.lseries, /sensitive $
else $ 
  widget_control,linfo.lseries, sensitive = 0
if linfo.where ge 5 then $
  widget_control,linfo.limage, /sensitive $
else begin
    linfo.file = ''
    widget_control,linfo.limage, sensitive = 0
endelse

for i=1,linfo.where-1 do $
  widget_control, linfo.buttons(i), sensitive = 1
for i=linfo.where,5 do $
  widget_control, linfo.buttons(i), sensitive = 0

WIDGET_CONTROL, stash, SET_UVALUE=linfo
end

pro xload_event, ev

  stash=ev.handler
  WIDGET_CONTROL, stash, GET_UVALUE=linfo

; Get the uval for the widget
  widget_control, ev.id, get_uvalue = uval
  case uval of
      "Database" : begin
          widget_control, linfo.fields(0), get_val = temp
          linfo.curpath = temp(0)
          update_xload, linfo
          linfo.where = 1
          xload_erase, linfo.fields,1
          widget_control,linfo.buttons(linfo.where), set_button=1
      end

      "FileList" : begin
          filelist = get_handle_value(linfo.listPTR, /no_copy)
          if (linfo.where eq 5) then begin
              linfo.file = filelist(ev.index)
              widget_control, linfo.fields(linfo.where), $
                set_value=filelist(ev.index)
          endif else begin
              linfo.curpath = linfo.curpath+'/'+filelist(ev.index)
              widget_control, linfo.fields(linfo.where), $
                set_value=filelist(ev.index)
              if linfo.where eq 4 then linfo.findfile = 1
              update_xload, linfo
              linfo.where = linfo.where+1
              widget_control,linfo.buttons(linfo.where), set_button=1
          endelse 
      end

      "LIMAGE" : begin          ; Load a single image
          if linfo.file eq '' then begin
              widget_control, linfo.l_error, set_val='Please Select an Image.'
              goto, CLEANUP
          endif else begin
              case linfo.datatype of
                  'GE'     : begin
                      widget_control, linfo.fields(5), get_value = temp
                      info = getsignaheader(linfo.curpath, temp(0))
                      info.matrix(2) = 1
                      cols = info.matrix(0)
                      rows = info.matrix(1)
                      slices = info.matrix(2)
                  
                      im = intarr(cols,rows)
                      data = intarr(cols,rows)
                      raw = bytarr(info.size)
                      widget_control, linfo.l_error, $
                        set_val='Reading file '+linfo.curpath+'/'+temp(0)

                      get_lun,signa_lun
                      openr,signa_lun,linfo.curpath+'/'+temp(0)
                      readu,signa_lun,raw
                      free_lun,signa_lun
                      
                      im=fix(raw,info.offset,cols,rows)
                      im=rotate(temporary(im),7)
                      data(*,*)=im(*,*)
                  end
                  'Bruker' : goto, BRUKER
                  else : ok = widget_message('Unsupported data type.')
              endcase
              set_handle_value,linfo.infoPTR, info, /no_copy
              set_handle_value, linfo.dataPTR, data, /no_copy
              widget_control, ev.top, /destroy
              return
          endelse
      end

      "LSERIES" : begin         ; Load the whole series
          case linfo.datatype of
              'GE' : begin
                  filelist = get_handle_value(linfo.listPTR, /no_copy)
                  if filelist(0) ne '' then begin
                      images = filelist(0:n_elements(filelist)-1)
                      info = getsignaheader(linfo.curpath+'/', $
                                            filelist(0))
                      info.matrix(2) = n_elements(images)
SIGNA:          
                      cols = info.matrix(0)
                      rows = info.matrix(1)
                      slices = info.matrix(2)
                      
                      im = intarr(cols,rows)
                      data = intarr(cols,rows,slices)
                      raw = bytarr(info.size)
                      for i = 0,slices-1 do begin
                          widget_control, linfo.l_error, $
                            set_val='Reading file '+filelist(i)
                          get_lun,signa_lun
                          openr,signa_lun,linfo.curpath+'/'+filelist(i)
                          readu,signa_lun,raw
                          free_lun,signa_lun
                          
                          im=fix(raw,info.offset,cols,rows)
                          im=rotate(temporary(im),7)
                          data(*,*,i)=im(*,*)
                      endfor
                  endif else begin
                      set_handle_value, linfo.listPTR, filelist, /no_copy
                      widget_control, linfo.l_error, $
                        set_val = 'No Image Files found.'
                      goto, CLEANUP
                  endelse
              end
              'Bruker' : begin
BRUKER:
                  data = readbruker(linfo.curpath+'/', $
                                    data_info = info, $
                                    id = linfo.l_error)
              end
              else :
          endcase
          set_handle_value,linfo.infoPTR, info, /no_copy
          set_handle_value, linfo.dataPTR, data, /no_copy
          widget_control, ev.top, /destroy
          return
      end

      "HELP" :  ok = widget_message('Help is on the way!',/info)

      "EXIT" : begin
          widget_control, ev.top, /destroy
          return
      end
      else:
  endcase

; Turn on the load series and load image at the appropriate place
CLEANUP:
  if linfo.where gt 4 then $
    widget_control,linfo.lseries, /sensitive $
  else $ 
    widget_control,linfo.lseries, sensitive = 0
  if linfo.where ge 5 then $
    widget_control,linfo.limage, /sensitive $
  else begin
      linfo.file = ''
      widget_control,linfo.limage, sensitive = 0
  endelse

  for i=1,linfo.where-1 do $
    widget_control, linfo.buttons(i), sensitive = 1
  for i=linfo.where,5 do $
    widget_control, linfo.buttons(i), sensitive = 0
        ; Restore the state structure
  WIDGET_CONTROL, stash, SET_UVALUE=linfo
END

;-----------------------------------------------------------------------------

function xload, path, $
                group     = group, $
                data_info = data_info, $
                SIGNA     = signa, $
                BRUKER    = BRUKER

;ON_ERROR, 2                     ;return to caller

if n_params() ne 1 then begin
    err_string = string(format = '("Path missing.", A, "path = root directory to image database.")', string(10B))
    ok = widget_message(err_string, /error)
    return, -1
end
brukerFlag = keyword_set(bruker)
if not(brukerFlag) then datatype = 'GE' else datatype = 'Bruker'
case datatype of
    'GE' : title = 'Load Signa 5.x Image'
    'Bruker' : title = 'Load Bruker Image'
    else: title = 'Load'
endcase
listPTR = handle_create()
infoPTR = handle_create()
dataPTR = handle_create()

;--- Get the current directory
cd, current = home
linfo = { mainbase : 0L, $      ; The mainbase widget id
          type     : 0L, $      ; Data type bgroup widget id
          fields   : lonarr(6), $ ; The directory field ids
          buttons  : lonarr(6), $ ; The direcotry button ids
          li_files : 0L, $      ; The file/dir listing widget id
          l_error  : 0L, $      ; The error window id
          limage   : 0L, $      ; The load image button id
          lseries  : 0L, $      ; The load series button id
          curpath  : path, $    ; The current path
          home     : home, $
          datatype : datatype, $; 'GE' or 'Bruker'
          file     : '', $      ; The name of an image file
          listPTR  : listPTR, $ ; Pointer to the list data
          infoPTR  : infoPTR, $ ; Pointer to the info structure
          dataPTR  : dataPTR, $ ; Pointer to the data
          findfile : 0, $       ; 1 == find files in the curpath
          where    : 1 }        ; What level of directory are we at?

mainbase = WIDGET_BASE(title = title)
panel_base = widget_base(mainbase, /column)

; ---- Data Type ----
base = widget_base(panel_base, /row, /align_center)

; --- Filepath ---
base = widget_base(panel_base, /column, /frame)
linfo.fields(0) = cw_field(base, title="Database ",   xsize=30, $
                           value=linfo.curpath, uvalue = "Database", $
                           /return_events)
linfo.fields(1) = cw_field(base, title="Site     ",   xsize=30, $
                           /noedit)
linfo.fields(2) = cw_field(base, title="Patient  ",   xsize=30, $
                           /noedit)
linfo.fields(3) = cw_field(base, title="Study    ",   xsize=30, $
                           /noedit)
linfo.fields(4) = cw_field(base, title="Series   ",   xsize=30, $
                           /noedit)
linfo.fields(5) = cw_field(base, title="Image    ",   xsize=30, $
                           /noedit)

; --- Browser ---
base      = widget_base(panel_base, /column, /frame)
sbase     = widget_base(base, /row)
foo       = widget_label(sbase, value="List: ")
mbase     = widget_base(sbase, /row, /exclusive, $
                        event_pro = "navigateEvent")
linfo.buttons(1) = widget_button(mbase, value=" Site ",    uval="Site",$
                                 /no_release)
linfo.buttons(2) = widget_button(mbase, value=" Patient ", uval="Patient", $
                                 /no_release)
linfo.buttons(3) = widget_button(mbase, value=" Study ",   uval="Study", $
                                 /no_release)
linfo.buttons(4) = widget_button(mbase, value=" Series ",  uval="Series", $
                                 /no_release)
linfo.buttons(5) = widget_button(mbase, value=" Images ",  uval="Images", $
                                 /no_release)
for i=1,5 do widget_control, linfo.buttons(i), sensitive = 0

linfo.li_files  = widget_list(base, ysize=8, uval="FileList")

; --- Error report ---
base    = widget_base(panel_base, /row)
linfo.l_error = widget_text(base, value="", xsize=30)

; --- Control Buttons ---
base = widget_base(panel_base, /row, /align_center)
linfo.lseries = widget_button(base,   value=" Load Series ", uval="LSERIES")
widget_control, linfo.lseries, sensitive = 0
linfo.limage = widget_button(base,   value=" Load Image  ", uval="LIMAGE")
widget_control, linfo.limage, sensitive = 0
help = widget_button(base,   value="    Help     ", uval="HELP")
cancel = widget_button(base, value="   Dismiss    ", uval="EXIT")

widget_control, mainbase, /realize
update_xload, linfo

WIDGET_CONTROL, mainbase, SET_UVALUE=linfo

Xmanager, "xload", mainbase, $
  event_handler= "xload_event", $
  group_leader= group, $
  /modal

handle_value, linfo.infoPTR, data_info
if n_elements(data_info) gt 0 then $
  data = get_handle_value(linfo.dataPTR, /no_copy) $
else begin
    data_info = -1
    data = -1
endelse
handle_free, linfo.listPTR
handle_free, linfo.infoPTR
handle_free, linfo.dataPTR
cd, home
RETURN, data

END





