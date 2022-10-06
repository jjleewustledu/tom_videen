; idlutils.pro

function index, string, delimiter, i
p=-1
for j=1,i do begin
    p=strpos(string,delimiter,p+1)
    if (p eq -1) then goto, continue
end

continue:
if ( (p lt 0) and (i gt 0)) then p=strlen(string)+1
return, p
end

function element, instr, delimiter, i
str=string(instr)
p1=index(str,delimiter,i)
p2=index(str,delimiter,i+1)
length=p2-p1-1
start=p1+1
;print,p1,p2,start,length
if (start gt strlen(str)) then return, ''
return,strmid(str,start,length)
end

function idlstr_to_cstr, in
length=strlen(in)
a=bytarr(length+1)
for i=0, length do begin
   a(i)=byte(strmid(in, i, 1))
end
return, a
end

function mspec, fname, f, p, g, d, b

if (n_params() NE 6) then return,0
str=fname+','+strcompress(f,/remove_all)+','+strcompress(p,/remove_all)+','+strcompress(g,/remove_all)+','+strcompress(d,/remove_all)+','+strcompress(b,/remove_all)
return,str
end

