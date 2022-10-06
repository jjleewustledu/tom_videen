PRO test_scale_event, ev
   widget_control, ev.top, get_uvalue=uv
   CASE ev.id OF
      uv.stype:BEGIN
         stype = ev.value
         widget_control, ev.top, set_uvalue=uv
      END 
      uv.sdone: BEGIN
         widget_control, ev.top, /destroy
         RETURN 
      END
   ENDCASE
END

FUNCTION test_scale, val
   handle_value, val, type
   print,"VAL=",type
   sbase = widget_base(title='Scaling Options', /COL)
   stype = cw_bgroup(sbase, ['Auto','Manual'], /EXCLUSIVE, set_value=type)
   sdone = widget_button(sbase, value='Done')
   uv1 = {stype:stype, sdone:sdone}
   widget_control, sbase, set_uvalue=uv1
   widget_control, sbase, /realize
   xmanager, "test_scale", sbase, EVENT_HANDLER='test_scale_event',/modal
   print,"top=",uv1.stype
   type = uv1.stype
   handle_value, val, type, /set
END 

PRO test_event, ev
   print,"TOP=",ev.top
   widget_control, ev.top, get_uvalue=uv
   print,"PTR=",uv.ptrtotype, "  WAIT=",uv.wait
   IF (uv.wait) THEN BEGIN
;      IF (stat) THEN BEGIN
         handle_value, newptr, newp
         handle_value, uv.ptrtotype, newp, /set
         print, "Type=",uv.ptrtotype
         uv.wait = 0
         widget_control, ev.top, set_uvalue=uv
 ;     ENDIF
   ENDIF ELSE BEGIN
      CASE ev.id OF
         uv.scale: BEGIN
            uv.wait = 1
            widget_control, ev.top, set_uvalue=uv
            stat = 0
            ;newptr = handle_create()
            zero = 0
            handle_value, ptr, zero, /set
            stat = test_scale (newptr)
         END
         uv.exit: BEGIN
            widget_control, ev.top, /destroy
         END
      ENDCASE
   ENDELSE
END

PRO test
   base = widget_base (/ROW)
   scale = widget_button(base, value='Scale')
   exit = widget_button(base, value='Exit')
   ptrtotype = handle_create()
   zero = 0
   ptrtotype = zero
   uv = {scale:scale, exit:exit, ptrtotype:ptrtotype, wait:0}
   widget_control, base, set_uvalue=uv
   widget_control, base, /realize
   xmanager, "test", base, EVENT_HANDLER='test_event'
END 
