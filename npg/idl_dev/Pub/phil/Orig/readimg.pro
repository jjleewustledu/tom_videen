function readimg
	image = intarr(128,128,47)
	openr,lu,'test.img',/get_lun
	readu,lu,image
	close,lu
	return,image
end
