; Variables defined for roi program.


defsysv,'!NUM_FILES',30		; Maximum simultaneously opened files.
defsysv,'!FALSE',0
defsysv,'!TRUE',1
defsysv,'!NEVER',2

defsysv,'!LITTLE_ENDIAN',0

defsysv,'!OK',-1

defsysv,'!ASSOC_ARRAY',1
defsysv,'!FLOAT_ARRAY',2
defsysv,'!STITCH_ARRAY',3
defsysv,'!OVERLAY_ARRAY',4

defsysv,'!PROF_NO',1
defsysv,'!PROF_HV',2
defsysv,'!PROF_T',3
defsysv,'!PROF_REG',4
defsysv,'!PROF_NWIN',500
defsysv,'!MAX_PROFILES',500

defsysv,'!SIGNIFICANCE',1
defsysv,'!PERCENT',2

defsysv,'!EDGE',1  ; Edge and Face connectivity for regions in multcomp.
defsysv,'!FACE',2  ; Face connectivity for regions in multcomp.

defsysv,'!PLAIN_TPROF',1	; Simple sum over time profile region.
defsysv,'!THRESH_TPROF',2	; Sum over threshold pixels in profile region.
defsysv,'!GANG_TPROF',3		; 3x3 ganged array of plots.
defsysv,'!HISTO_TPROF',4	; Compute histogram over time profile.

defsysv,'!LOCAL',0
defsysv,'!GLOBAL',1
defsysv,'!FIXED',2

defsysv,'!PARADIGM_HGHT',120
defsysv,'!PARADIGM_OFFSET',50
defsysv,'!PARADIGM_WIDTH',612
defsysv,'!PARADIGM_Y0',40
defsysv,'!PARADIGM_Y1',100

defsysv,'!DRAW_SIZE',512	; Size of image displayed.
defsysv,'!DRAW_SIZEX',512	; Size of image displayed.
defsysv,'!DRAW_SIZEY',512	; Size of image displayed.
defsysv,'!DRAW_SIZE_TOT',512	; Size of image displayed (3-pixel frame).
defsysv,'!DRAW_SCROLL',400
defsysv,'!ZOOM_SIZE',256

defsysv,'!MAX_ROI',10		; Maximum number of ROIs per plane.
defsysv,'!MAX_ROI_PLN',48	; Maximum number of planes.
defsysv,'!MAX_ROI_VRTS',300	; Maximum number of vertices per ROI.
defsysv,'!MAX_VOL',100		; Maximum nuber of volumes.

defsysv,'!SQUARE',+6		; Square plotting symbol.
defsysv,'!PLUS',+1		; Plus plotting symbol.
defsysv,'!TRIANGLE',+5		; Triangle plotting symbol.
defsysv,'!STAR',+2		; Asterisk plotting symbol.
defsysv,'!DOT',+3		; Period plotting symbol.

defsysv,'!TRANSVERSE',1	
defsysv,'!SAGITAL',2	
defsysv,'!CORONAL',3	

defsysv,'!MAX_BLTIMG',30	; Maximum number of panes in custom image.

defsysv,'!SHARE_LIB',string('/usr/local/lib/idl_stats.so') ;Library containing shared object modules.
defsysv,'!BIN',string('/usr/local/bin/stats/bin') ;Path to binary files.

defsysv,'!COLOR_TABLE',"/usr/local/lib/idl_colors1.tbl"	; Color table.
defsysv,'!GRAY_SCALE',0		; Index of gray color scale.
defsysv,'!COLOR_42',1		; Index of color 42 color scale.
defsysv,'!DIFF_SCALE',2		; Index of difference image scale.
defsysv,'!BLACK_BODY',3		; Index of black body color scale.
defsysv,'!OVERLAY_SCALE',4	; Activation overlay color scale.
defsysv,'!DEFAULT_SCALE',5	; Use current default scale.
defsysv,'!LEN_COLORTAB',202 ;Length of color table for gray scale.
defsysv,'!NUM_COLORS',207 ;Length of color table for gray scale.

;Now color table changed, redefine length
;defsysv,'!WHITE_INDEX',!LEN_COLORTAB
;defsysv,'!YELLOW_INDEX',!LEN_COLORTAB+1
;defsysv,'!RED_INDEX',!LEN_COLORTAB+2
;defsysv,'!GREEN_INDEX',!LEN_COLORTAB+3
;defsysv,'!BLUE_INDEX',!LEN_COLORTAB+4
defsysv,'!WHITE_INDEX',202
defsysv,'!YELLOW_INDEX',203
defsysv,'!RED_INDEX',204
defsysv,'!GREEN_INDEX',205
defsysv,'!BLUE_INDEX',206

defsysv,'!COLSCL_WIN_XSZ',90
defsysv,'!COLSCL_WIN_YSZ',350
defsysv,'!COLSCL_XOFF',15
defsysv,'!COLSCL_YOFF',30
defsysv,'!COLSCL_YSZ',300

defsysv,'!CUSTOM_XSZ',520
defsysv,'!CUSTOM_YSZ',776

defsysv,'!CTI',0
defsysv,'!CUBE',1
defsysv,'!TIFF',2
defsysv,'!MONTE_ALL',3
defsysv,'!STITCH',4
defsysv,'!RAW',5
defsysv,'!INTERFILE',7
defsysv,'!MRI_ANALYZE',9
defsysv,'!MRI_4DFP',10
defsysv,'!GIF',11
defsysv,'!T88',12

defsysv,'!BYTE',1
defsysv,'!SHORT',2
defsysv,'!FLOAT',4

defsysv,'!NANGMC',96
defsysv,'!NRAYMC',64
defsysv,'!NRINGMC',4
defsysv,'!NRINGSQMC',16
defsysv,'!NSCATMC',4
defsysv,'!NPHGTOT',6291456

defsysv,'!NANGCTI',192
defsysv,'!NRAYCTI',128

defsysv,'!PRINTER',"Pfudd"

defsysv,'!LINE',1
defsysv,'!SYMBOL',2
defsysv,'!SYMLINE',3

defsysv,'!PLOTFILE',"/home/fafner/jmo/idl.ps"

defsysv,'!IRREG_ROI',1
defsysv,'!SQUARE_ROI',2
defsysv,'!CIRC_ROI',3

defsysv,'!PRESS',0
defsysv,'!RELEASE',1
defsysv,'!MOTION',2

defsysv,'!DRAW_OFF',0	; Values for prof_mode.
defsysv,'!DRAWING',1
defsysv,'!DRAW_DONE',2

