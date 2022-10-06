;*************
pro set_colors
;*************

;Setup plotting colors and color table length.

red   = [1, 1, 1, 0, 0]
green = [1, 1, 0, 1, 0]
blue  = [1, 0, 0, 0, 1]
red = 255*red
green = 255*green
blue = 255*blue
tvlct,red,green,blue,!LEN_COLORTAB	 ;Load Plotting colors at end.

return
end
