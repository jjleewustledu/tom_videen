pro run
	str=' '
	fname=' '
	read,"Fit file (from peakseek): ",fname
	openr,1,fname
	readf,1,str
	readf,1,str
	x=fltarr(9,16)
	readf,1,x
	close,1
	a=moment(x(6,*))
	b=moment(x(7,*))
	print,a(0)*2.354*2.086,format='("Mean radial FWHM     = ",F6.2)'
	print,b(0)*2.354*2.086,format='("Mean tangential FWHM = ",F6.2)'
	c = (x(6,*)+x(7,*))*2.354*2.086*0.5
	m = moment(c,sdev=sd)
	print,m(0),sd,format='("Mean plns 11-37 FWHM = ",F6.2,"  SD =",F5.2)'
end
