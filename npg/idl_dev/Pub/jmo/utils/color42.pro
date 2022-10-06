pro color42,file_name

red = intarr(256)
blue = intarr(256)
green = intarr(256)

red(0:12) = 0
red(13:25) = 102
red(26:37) = 51
red(51:63) = 85
red(64:76) = 102
red(77:88) = 119
red(89:101) = 136
red(102:114) = 51
red(128:139) = 85
red(140:152) = 68
red(153:165) = 204
red(166:177) = 238
red(178:190) = 221
red(217:228) = 204
red(229:241) = 187
red(242:254) = 255
green(0:12) = 0
green(26:37) = 51
green(51:63) = 85
green(64:76) = 102
green(77:88) = 119
green(89:101) = 136
green(102:114) = 102
green(115:127) = 153
green(128:139) = 204
green(140:152) = 238
green(153:165) = 255
green(166:177) = 238
green(178:190) = 221
green(191:203) = 187
green(204:216) = 136
green(217:228) = 102
green(229:241) = 68
green(242:254) = 0
green(255) = 88
blue(0:12) = 0
blue(13:25) = 85
blue(38:50) = 119
blue(51:63) = 136
blue(64:76) = 187
blue(77:88) = 204
blue(89:101) = 238
blue(102:114) = 51
blue(128:139) = 85
blue(140:152) = 68
blue(153:165) = 34
blue(166:177) = 102
blue(178:190) = 85
blue(191:203) = 68
blue(204:216) = 34
blue(217:228) = 17
blue(229:255) = 0

modifyct,12,'Color 42',red,green,blue,FILE=file_name

return
end
