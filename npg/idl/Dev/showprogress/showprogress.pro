FUNCTION ShowProgress
   tlb = Widget_Base(Column=1, Title='Progress', XOffset=450, YOffset=0)
   msg = Widget_Text(tlb, Xsize=20)
   cancel = Widget_Button(tlb, Value='Stop')
   Widget_Control, tlb, /Realize
   Return, {top:tlb, msg:msg, cancel:cancel}
END 

Pro Test_Event, event
   Widget_Control, event.id, Get_UValue=buttonUValue
   CASE buttonUValue OF
      'LOOP': BEGIN
         info = ShowProgress()
         cnt = 0L
         WHILE (1) DO BEGIN	; Loop until Stop
            widget_control, info.msg, set_value=string(cnt, format='("Count = ",I)')
            cnt = cnt+1
            progressEvent = Widget_Event(info.cancel, /NoWait)
            eventName = Tag_Names(progressEvent, /Structure_Name)
            IF eventName EQ 'WIDGET_BUTTON' THEN BEGIN
               Widget_Control, info.top, /Destroy
               GoTo, endLoop
            ENDIF
         ENDWHILE
         endLoop: 
      ENDCASE

      'QUIT': Widget_control, event.top, /Destroy
   ENDCASE
END 

Pro Test
   tlb = Widget_Base(Column=1, XOffset=300, YOffset=0)
   button = Widget_Button(tlb, Value='Start Loop', UValue='LOOP')
   quitter = Widget_Button(tlb, Value='Quit', UValue='QUIT')
   Widget_Control, tlb, /Realize
   XManager, 'Test', tlb
END 
