;********************
pro custom_dead,draw
;********************
 
common stats_comm,wd,fi,pr,dsp,roi,fl,st,stc,cstm,bltimg
 
cstm.init = !FALSE
widget_control,wd.build_addimg,SENSITIVE=0
widget_control,wd.build_annotate,SENSITIVE=0
for i=0,!PROF_NWIN-1 do begin
    if(cstm.draw(i) eq draw) then begin
        cstm.id(i) = -1
        cstm.draw(i) = -1
        cstm.base(i) = -1
    end
end
 
return
end

