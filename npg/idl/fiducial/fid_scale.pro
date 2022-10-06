PRO FID_SCALE_EVENT, ev
   COMMON FID
   
   catch, error_status
   IF (error_status NE 0) THEN BEGIN
      ok = widget_message(!err_string)
      RETURN
   END
   
   done = 0
   widget_control, ev.top, get_uvalue=scl
   
   CASE ev.id OF
      
      scl.stype: BEGIN
         dsp[0].type = ev.value
      END
      
      scl.smax: BEGIN
         dsp[0].max = ev.value
      END
      
      scl.smin: BEGIN
         dsp[0].min = ev.value
      END
      
      scl.sdone: BEGIN
         widget_control, ev.top, /destroy
         done = 1
      END
      
   ENDCASE
   IF (done EQ 1) THEN RETURN
END

PRO FID_SCALE, tlb
   COMMON FID
   
   sbase = widget_base(title='Scaling Options', group_leader=tlb, /COLUMN, /MODAL)
   stype = cw_bgroup(sbase, ['Auto','Manual'],/ROW, /EXCLUSIVE, set_value=dsp[0].type)
   smax = cw_field(sbase, title='Max', /FLOATING, value=dsp[0].max, /RETURN_EVENTS)
   smin = cw_field(sbase, title='Min', /FLOATING, value=dsp[0].min, /RETURN_EVENTS)
   sdone = widget_button(sbase, value='Done')

   scl = {SCALE,		$
          sbase   : sbase,	$ ; scaling menu id
          stype   : stype,	$ ; scale type id for image
          smax    : smax,	$ ; scale max id for image 
          smin    : smin,	$ ; scale min id for image
          sdone   : sdone	$ ; scale done id
         }
   
   widget_control, sbase, set_uvalue=scl
   widget_control, sbase, /realize
   xmanager, "FID_SCALE", sbase, EVENT_HANDLER='FID_SCALE_EVENT'
   RETURN
END 
