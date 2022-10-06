function readsignaheader, path, file

get_lun,head_lun
openr,head_lun,file
junk = fstat(head_lun)
data = bytarr(junk.size)
readu,head_lun,data
free_lun,head_lun

header = bytarr(156)
header(0:155) = data(0:155)

offset = long(header,4)
cols = long(header,8)
rows = long(header,12)
junk1 = long(header,16)

;Why I want to be consistent w/ bruker is beyond my comprehension
case junk1 of
    long(1) : datatype = '_1BIT'
    long(8) : datatype = '_8BIT_SGN_INT'
    long(16) : datatype = '_16BIT_SGN_INT'
    long(24) : datatype = '_24BIT_SGN_INT'
endcase

ExamHeadoff = long(header,132)
ExamHeadlen = long(header,136)
SeriesHeadoff = long(header,140)
SeriesHeadlen = long(header,136)
ImHeadoff = long(header,148)
ImHeadlen = long(header,152)

examheader = bytarr(ExamHeadlen)
examheader(0:ExamHeadlen-1) = data(ExamHeadoff:ExamHeadoff+ExamHeadlen-1)

seriesheader = bytarr(SeriesHeadlen)
seriesheader(0:SeriesHeadlen-1) = data(SeriesHeadoff:SeriesHeadoff+SeriesHeadlen-1)

imheader = bytarr(imheadlen)
imheader(0:imheadlen-1) = data(imheadoff:imheadoff+imheadlen-1)

slthick = float(imheader,26)
fovx = float(imheader,34)
fovy = float(imheader,38)
tr = long(imheader,194)/1000.
te = long(imheader,202)/1000.
navgs = float(imheader,218)

info = {info_s, $
        DIR: path, $            ;path to the data
        DIM: 2, $               ;not sure the best way to determine this yet
        OFFSET: offset, $       ;offset to the data
        SIZE: junk.size, $      ;size of data file
        MATRIX: [cols, rows, 0], $ ;get slices from readsigna
        FOV: [fovx, fovy, slthick], $
        DATATYPE: datatype, $
        TR: tr, $
        TE: te, $
        NAVGS: navgs}

return, info
end

