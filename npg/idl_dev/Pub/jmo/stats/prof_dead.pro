;*****************
pro prof_dead,draw
;*****************
 
common stats_comm,wd,fi,pr,dsp,roi,fl,st,stc,cstm,bltimg
 
pr.prof_on = !PROF_NO

for i=0,!PROF_NWIN-1 do begin
    if(pr.draw(i) eq draw) then begin
        pr.id(i) = -1
        pr.draw(i) = -1
        pr.base(i) = -1
    end
end

 
return
end
