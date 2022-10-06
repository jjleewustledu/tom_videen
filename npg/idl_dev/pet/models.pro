PRO models, GROUP=GROUP


ON_ERROR, 2


genbase = WIDGET_BASE(TITLE = 'Models',/column)
studybase = WIDGET_BASE(genbase,/column)
studylabel = WIDGET_LABEL(studybase, VALUE = "Study Name : PA1089DY2")

curvebase = WIDGET_BASE(genbase,/row)
bcbase = WIDGET_BASE(curvebase,/COLUMN)
bcloadbase = WIDGET_BASE(bcbase,/row)
bcload = WIDGET_BUTTON(bcloadbase, VALUE = "Load Blood Curve", UVALUE = 'BCLOAD')
bcps = WIDGET_BUTTON(bcloadbase, VALUE = "Postscript", UVALUE = 'BCPS')
bclabel = WIDGET_LABEL(bcbase, VALUE = " Blood Curve ")
bcdraw = WIDGET_DRAW(bcbase, $
	/BUTTON_EVENTS, $	;generate events when buttons pressed
	/FRAME, $
	UVALUE = 'DRAW_WIN_EVENT1', $
	RETAIN = 2, $		;make sure draw is redrawn when covered
	XSIZE = 256, $
	YSIZE = 256)

interpbase = WIDGET_BASE(bcbase,/row)

XPdMenu, [	'"Interpolation method"		{',	$
		'"linear"		LINEAR',	$
		'"biexponential"	BIEXP',		$
		'}'					$
	 ], interpbase
method = WIDGET_LABEL(interpbase, VALUE = " Linear ")


modelbase = WIDGET_BASE(bcbase,/row)

XPdMenu, [	'"Equations"		{',	$
		'"H2O"		H2O',		$
		'"NH3"		NH3',		$
		'}'				$
	 ], modelbase

model = WIDGET_LABEL(modelbase, VALUE = "H2O")
viewmod = WIDGET_button(modelbase, VALUE = "View Equations",uvalue='VIEWMOD')

minuitbase = WIDGET_BASE(bcbase,/row)
minuitlabel = WIDGET_LABEL(minuitbase, VALUE = " Minuit Cmds ")
minuitcmds = WIDGET_button(minuitbase, VALUE = "Load set",uvalue='LOADCMDS')
viewcmds = WIDGET_button(minuitbase, VALUE = "Show set",uvalue='VIEWCMDS')

roibase = WIDGET_BASE(curvebase,/COLUMN)
rloadbase = WIDGET_BASE(roibase,/row)
roiload = WIDGET_BUTTON(rloadbase, VALUE = "Load ROI Curve", UVALUE = 'ROILOAD')
previous = WIDGET_BUTTON(rloadbase, VALUE = "<<", UVALUE = 'PREVIOUS')
next = WIDGET_BUTTON(rloadbase, VALUE = ">>", UVALUE = 'NEXT')
roips = WIDGET_BUTTON(rloadbase, VALUE = "Postscript", UVALUE = 'ROIPS')
roilabel = WIDGET_LABEL(roibase, VALUE = " ROI Curve ")
roidraw = WIDGET_DRAW(roibase, $
	/BUTTON_EVENTS, $	;generate events when buttons pressed
	/FRAME, $
	UVALUE = 'DRAW_WIN_EVENT1', $
	RETAIN = 2, $		;make sure draw is redrawn when covered
	XSIZE = 256, $
	YSIZE = 256)

rcorrbase = WIDGET_BASE(roibase,/row)
decay = WIDGET_BUTTON(rcorrbase, VALUE = "Decay on", UVALUE = 'DECAY')
deadt = WIDGET_BUTTON(rcorrbase, VALUE = "DeadTime on", UVALUE = 'DEADT')
spill = WIDGET_BUTTON(rcorrbase, VALUE = "Spillover on", UVALUE = 'SPILL')

fit = WIDGET_BUTTON(roibase, VALUE = "Start Fit", UVALUE = 'FIT')
label = WIDGET_LABEL(roibase, VALUE = 'Fit History')
history = WIDGET_TEXT(roibase, VALUE = 'Please load data ...', xsize=30,YSIZE = 5,/FRAME)

done = WIDGET_BUTTON(genbase, VALUE = "Exit", UVALUE = 'DONE')

WIDGET_CONTROL, genbase, /REALIZE

WIDGET_CONTROL,bcdraw, GET_VALUE = win_num1
WIDGET_CONTROL,roidraw, GET_VALUE = win_num2


XMANAGER, "models", genbase, GROUP_LEADER=GROUP

END
