;;; Variables for vidi 

COMMON VIDI, dsp, thresh, default_path, local_path, atlas, atlas_type, mask_type, connect_type, $
   erod_type, segshow_type, seed, nseeds, npix_removed, vdsp, maskimg, loglun, ptlist, npt, $
   label, xloc0, yloc0, colortable, percent

dsp = [{VDISPLAY},{VDISPLAY},{VDISPLAY}]
thresh = [0.0,0.0]
default_path = 0
local_path = ''
atlas = [0,0,0,0,0]
atlas_type = 0
mask_type = 0
connect_type = 0
erod_type = 0
segshow_type = 0
seed = intarr(3,11)
nseeds = 0
npix_removed = 0
vdsp = {VDISPLAY}
maskimg = 0L
logfile = 0
ptlist = intarr(3,1000)
npt = 0
label = 0
xloc = 0
yloc = 0
colortable = '/usr/local/npg/idl/color_tables/videen.tbl'
percent = 10
