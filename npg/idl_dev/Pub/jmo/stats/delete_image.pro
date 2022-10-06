;*************************
pro delete_image,wd,fi,dsp
;*************************

common linear_fit,fit_name,slope,intcpt,trend
 
if((fi.nfiles lt 1 ) or (fi.n lt 0)) then begin
    stat = widget_message(string('No files opened.'),/ERROR)
    return
endif

; Get header data
handle_value,fi.hdr_handle(fi.n),hdr

if(hdr.array_type eq !ASSOC_ARRAY) then $
    free_lun,fi.lun(fi.n)
 
if(trend.slope_handle(fi.n) ne 0) then begin
    handle_free,trend.slope_handle(fi.n)
    handle_free,trend.intcpt_handle(fi.n)
endif
trend.slope_handle(fi.n) = 0
    
fi.names(fi.n) = fi.names(fi.nfiles-1)
fi.tails(fi.n) = fi.tails(fi.nfiles-1)
fi.list(fi.n) = fi.list(fi.nfiles-1)
fi.paths(fi.n) = fi.paths(fi.nfiles-1)
fi.zoom(fi.n) = fi.zoom(fi.nfiles-1)
fi.names(fi.nfiles-1) = ' '
fi.tails(fi.nfiles-1) = ' '
fi.list(fi.nfiles-1) = ' '
fi.paths(fi.nfiles-1) = ' '
widget_control,wd.files,SET_VALUE=fi.list
erase

if(hdr.array_type ne !STITCH_ARRAY) then $
    handle_free,fi.data_handle(fi.n)
handle_free,fi.hdr_handle(fi.n)

if(fi.nfiles gt 1 and fi.n ne fi.nfiles) then begin
    fi.data_handle(fi.n) = fi.data_handle(fi.nfiles-1)
    fi.hdr_handle(fi.n) = fi.hdr_handle(fi.nfiles-1)
endif
fi.nfiles = fi.nfiles - 1
if(fi.n gt fi.nfiles-1) then $
    fi.n = fi.nfiles - 1
widget_control,wd.files,SET_LIST_SELECT=fi.n
 
return
end
