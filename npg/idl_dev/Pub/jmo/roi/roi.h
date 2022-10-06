; Variables defined for roi program.


defsysv,'!NUM_FILES',20		; Maximum simultaneously opened files.
defsysv,'!FALSE',0
defsysv,'!TRUE',1
defsysv,'!NEVER',2

defsysv,'!DRAW_SIZE',512	; Size of image displayed.
defsysv,'!DRAW_SIZE_TOT',512	; Size of image displayed (3-pixel frame).
defsysv,'!DRAW_SCROLL',400
defsysv,'!ZOOM_SIZE',256

defsysv,'!MAX_ROI',10		; Maximum number of ROIs per plane.
defsysv,'!MAX_ROI_PLN',512	; Maximum number of planes.
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

defsysv,'!SHARE_LIB',string(getenv('IDL_LIB'),'/idl_share.so') ;Library containing
;							    shared object modules.

defsysv,'!BLACK_BODY',3		; Index of black body color scale.
defsysv,'!LEN_COLORTAB',!D.N_COLORS-15 ;Length of color table for gray scale.
loadct,!BLACK_BODY,NCOLORS=!LEN_COLORTAB ;Load Black Body color table.

;Now color table changed, redefine length
defsysv,'!LEN_COLORTAB',!D.N_COLORS-5 ;Length of color table for gray scale.
defsysv,'!WHITE_INDEX',!D.N_COLORS-5
defsysv,'!YELLOW_INDEX',!D.N_COLORS-4
defsysv,'!RED_INDEX',!D.N_COLORS-3
defsysv,'!GREEN_INDEX',!D.N_COLORS-2
defsysv,'!BLUE_INDEX',!D.N_COLORS-1

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
defsysv,'!MONTE_SCAT',4
defsysv,'!RAW_LONG',5
defsysv,'!RAW_FLT',6
defsysv,'!INTERFILE',7
defsysv,'!RAW_SHORT',8
defsysv,'!MRI_SHORT',9
defsysv,'!RAW_BYT',10

defsysv,'!NANGMC',96
defsysv,'!NRAYMC',64
defsysv,'!NRINGMC',4
defsysv,'!NRINGSQMC',16
defsysv,'!NSCATMC',4
defsysv,'!NPHGTOT',6291456

defsysv,'!NANGCTI',192
defsysv,'!NRAYCTI',128

defsysv,'!PRINTER',"P2015"

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
