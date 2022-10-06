PRO slicer_cti, GROUP = group
COMMON volume_data, a
loop:
if XRegistered("slicer") THEN RETURN
a = 0
i = 0
base = WIDGET_BASE(TITLE = 'Slicer CTI', /column)
junk = WIDGET_LABEL(base, VALUE = 'Select CTI study:')
choices =  [	'NMR-Christian Michel',$
		'NMR-Muriel Chiang',$
		'NMR-Unknown Soldier',$
		'NMR-Chest',$
		'PET-Ron Nutt',$
		'PET-Michel Maury',$
		'NMR-Atlas',$
		'Other...',$
		'Exit']
junk = WIDGET_LIST(base, FONT = '*times-bold-r-*180*', $
	VALUE = choices, YSIZE = n_elements(choices))

WIDGET_CONTROL, base, /realize
e = WIDGET_EVENT(base, BAD_ID = i)
widget_control, base, /destroy

case e.index of		;select data and parameters
0: BEGIN
	data_dir = '/topo6b/mich/data/cti/nmr'
	study='michel128'
	lth = 0
	hth = 2000
	isoth = '20'
	transth = '15'
	col_num = 15
	file = data_dir + '/' + study + '.img'
   ENDCASE
1: BEGIN
	data_dir = '/topo6b/mich/data/cti/nmr'
	study='muriel128'
	lth = 0
	hth = 2000
	isoth = '20'
	transth = '15'
	col_num = 15
	file = data_dir + '/' + study + '.img'
   ENDCASE
2: BEGIN
	data_dir = '/topo6b/mich/data/cti/nmr'
	study='byars128'
	lth = 60
	hth = 256
	isoth = '30'
	transth = '15'
	col_num = 15
	file = data_dir + '/' + study + '.img'
   ENDCASE
3: BEGIN
	data_dir = '/topo6b/mich/data/cti/nmr'
	study='deplamnmr'
	lth = 0
	hth = 2000
	isoth = '20'
	transth = '15'
	col_num = 15
	file = data_dir + '/' + study + '.img'
   ENDCASE
4: BEGIN
	data_dir = '/topo6b/mich/data/cti/931'
	study='ronnutfdg'
	lth = 0
	hth = 1024
	isoth = '30'
	transth = '25'
	col_num = 14
	file = data_dir + '/' + study + '.img'
   ENDCASE
5: BEGIN
	data_dir = '/topo6b/mich/data/cti/911'
	study='pat993fdg'
	lth = 0
	hth = 10000
	isoth = '40'
	transth = '25'
	col_num = 14
	file = data_dir + '/' + study + '.img'
   ENDCASE

6: BEGIN
	data_dir = '/topo6b/mich/data/cti/nmr'
	study='michelatl'
	lth = 221
	hth = 256
	isoth = '0'
	transth = '0'
	col_num = 15
	file = data_dir + '/' + study + '.img'
   ENDCASE
7: BEGIN
	data_dir = '/topo6b/mich/data/cti/'
	cd, data_dir, current=curdir
	file = pickfile(GROUP=base,TITLE='Select CTI image',FILTER=['img'])
	if file eq "" then return
	cd, curdir
	lth = 0
	hth = 1000000
	isoth = '0'
	transth = '0'
	col_num = 15
   ENDCASE
8: return
ENDCASE

openr,unit,file,/get_lun
stat=rmh(unit,mh)
if(stat eq !ECAT.ERROR) then begin 
  print, !ECAT.ERR
  return
endif
nf = mh.file.nframes
np = mh.file.nplanes
ng = mh.file.ngates
nb = mh.file.nbeds
f = 0 & p = 0 & g = 0 & b = 0
if (nf gt 0) then f='*'
if (np gt 0) then p='*'
if (ng gt 0) then g='*'
if (nb gt 0) then b='*'
expand,mh,f,p,g,b
f=fix(f) & p=fix(p) & g=fix(g) & b=fix(b)
mf = n_elements(f)
mp = n_elements(p)
mg = n_elements(g)
mb = n_elements(b)
ntot  = mf*mp*mg*mb
mats=lonarr(5,ntot)
stat = rdir(unit,dir)
free_lun,unit
print,'File contains ',ntot,' matrixes'
a=bytarr(128,128,ntot)
for i =0, ntot-1 do begin
  mats(*,i) = matval(dir(0,i))
  f= mats(0,i) & p= mats(1,i) & g= mats(2,i) & d= mats(3,i) & b= mats(4,i)
  stat=recat(file,f,p,g,d,b,data)
  if(stat eq !ECAT.ERROR) then begin
    print, !ECAT.ERROR
    return
  endif
  tmp = bytscl(rebin(data.data<hth>lth,128,128))
  a(*,*,i) = tmp
end
print,mats
ftype	= data.ftype
dim_x	= data.xdim
dim_y	= data.ydim
slice_width = data.sh.slice_width
pixel_size = data.sh.pixel_size
plane_sep = data.mh.scan.plane_separation
data=0
erase
loadct,col_num
for i=0,ntot-1 do begin
  tv, a(*,*,i), i 
endfor
print," Plane separation  ",plane_sep
print," Pixel_size        ",pixel_size
zr = 0
if plane_sep ne 0 then zr = pixel_size / plane_sep
if zr eq 0 then zr = 0.5
print," z ratio           ",zr
z_r = string(zr)

command1 = [ $
	'ORI  0 1 2 0 0 1 30. 30.' + z_r, $
	'COLOR ' +  string(col_num) +  ' 0  100   0', $
	'ISO ' +  isoth + ' 0', $
	'TRANS 1 ' + transth, $
	'SLICE 2 1 0',	$
	'WAIT 5',	$
	'ERASE 0',	$
	'ORI  0 1 2 0 0 1 10. 60.' + z_r, $
	'ISO ' +  isoth + ' 0', $
	'TRANS 1 ' + transth, $
	'SLICE 2 1 0',	$
	'WAIT 5',	$
	'ERASE 0',	$
	'ORI  0 1 2 0 0 1 10. -60.' + z_r, $
	'ISO ' +  isoth + ' 0', $
	'TRANS 1 ' + transth, $
	'SLICE 2 1 0']

command2 = [ $
	'ORI  0 2 1 0 1 0 10. 60.'  + z_r, $
	'COLOR ' +  string(col_num) +  ' 0  100   0', $
	'ISO ' +  isoth + ' 0' ,$ 
	'WAIT 5']

if(e.index eq 2) then begin
	print,'Second command set selected'
	slicer, GROUP = GROUP, COMMANDS = command2 
endif else begin
	print,'First command set selected'
	slicer, GROUP = GROUP, COMMANDS = command1
endelse

loadct,col_num
goto,loop
end
