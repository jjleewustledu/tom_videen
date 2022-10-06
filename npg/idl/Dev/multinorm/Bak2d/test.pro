PRO TEST

	n = 3
	k = 1
	wa=fltarr(n,n,n)
	for z=0,n-1 do begin
	for y=0,n-1 do begin
	for x=0,n-1 do begin
		wa[x,y,z] = k
		k = k+1
	endfor
	endfor
	endfor
	;wa = [[5.,-1.,3.],[2.,0.,1.],[3.,2.,1.]]
	print,"wa"
	print,wa
	wb = invert(wa,/double,status)
	if (status eq 0) then begin
		print,"wb"
		print,wb
		print,"wb#wa"
		print,wa#wb
    endif else print,status
END

