pro test

@roi.h

i = intarr(5)
x = fltarr(5)
i(0) = 1
x(0) = 1
str = "fromidl"
print,'IDL: ',i(0),' ',x(0),' ',str

passtype = bytarr(3)
passtype(0) = 0
passtype(1) = 0
stat=call_external(!SHARE_LIB,'_test',i,x,str,/F_VALUE,VALUE=passtype)

print,'Back_in_IDL ',i(0),' ',x(0),' ',str

return
end
