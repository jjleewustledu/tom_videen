
FUNCTION Pickfile, dialog_parent = dialog_parent,directory = directory, $
		display_name = display_name,multiple_files = multiple_files, $
		Group=GROUP, Path=PATH, Read=READ, Write=WRITE, $
                Filter=FILTER, Title=TITLE, Noconfirm=NOCONFIRM, $
                Must_exist=MUST_EXIST, Fix_filter=FIX_FILTER, $
                FILE=FILE, GET_PATH=GET_PATH

;------------------------------------------------------------------------
;For compatibility with Linux and lingering versions of doctored pickfiles
;Also need filter for IDL 5.4 version
;J. Maldjian
;------------------------------------------------------------------------
if not(keyword_set(filter)) then filter = '*'
response = dialog_pickfile(dialog_parent = dialog_parent,directory = directory, display_name = display_name,$
		multiple_files = multiple_files, $
		Group=GROUP, Path=PATH, Read=READ, Write=WRITE, $
                   Filter=FILTER, Title=TITLE, Noconfirm=NOCONFIRM, $
                   Must_exist=MUST_EXIST, Fix_filter=FIX_FILTER, $
                   FILE=FILE, GET_PATH=GET_PATH)
GET_PATH= stpath(GET_PATH)

return,stpath(response)
end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


FUNCTION Pickdir,  dialog_parent = dialog_parent,directory = directory, $
		display_name = display_name,multiple_files = multiple_files, $
		Group=GROUP, Path=PATH, Read=READ, Write=WRITE, $
                Filter=FILTER, Title=TITLE, Noconfirm=NOCONFIRM, $
                Must_exist=MUST_EXIST, Fix_filter=FIX_FILTER, $
                FILE=FILE, GET_PATH=GET_PATH

;------------------------------------------------------------------------
;For compatibility with Linux and lingering versions of doctored pickdirs
;Also need filter for IDL 5.4 version
;------------------------------------------------------------------------
if not(keyword_set(filter)) then filter = '*'

response = dialog_pickfile(dialog_parent = dialog_parent,/directory, $
		display_name = display_name, multiple_files = multiple_files, $
		Group=GROUP, Path=PATH, Read=READ, Write=WRITE, $
                Filter=FILTER, Title=TITLE, Noconfirm=NOCONFIRM, $
                Must_exist=MUST_EXIST, Fix_filter=FIX_FILTER, $
                FILE=FILE, GET_PATH=GET_PATH)
GET_PATH= stpath(GET_PATH)
return,stpath(response)
end

