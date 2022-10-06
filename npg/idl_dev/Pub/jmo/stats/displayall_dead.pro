;***********************
pro displayall_dead,draw
;***********************
 
common stats_comm,wd,fi,pr,dsp,roi,fl,st,stc,cstm,bltimg
 
dsp.cur_window = -1
for i=0,!PROF_NWIN-1 do begin
    if(dsp.draw(i) eq draw) then begin
        dsp.id(i) = -1
        dsp.draw(i) = -1
        dsp.base(i) = -1
    end
end

 
return
end
