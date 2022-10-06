;+
; NAME:
;	SLICE
;
; PURPOSE:
;
;	This is a routine to display a 3D data set and slice through it
;       at arbitrary orthographic angles. The display window (when run
;       with the GUI) is resizeable. The output can be sent either to
;       the display, to a GUI, or to a PostScript printer (as true
;       PostScript output, not a screen dump.) When run with the GUI,
;       slices can be animated and displays can be saved and restored
;       for later processing.
;
; CATEGORY:
;
;	3D Graphics, Widgets.
;
; CALLING SEQUENCE:
;
;       For display in a regular IDL graphics window:
;
;          SLICE, data
;
;       For display with a built-in GUI:
;
;          SLICE, data, /GUI
;
;       To send display to a PostScript printer:
;
;          deviceKeywords = PS_FORM(/INITIALIZE)
;          SLICE, data, POSTSCRIPT=deviceKeywords
;
; INPUTS:
;
;	data:	The 3D data set to be displayed. The data is assumed to be 
;          scaled into the number of colors. 
;
; KEYWORD PARAMETERS:
;
;       AX      The rotation of the 3D coordinate system about the X axis.
;               Must be between 0 and 90 degrees. Default is 30 degrees.
;
;       AZ      The rotation of the 3D coordinate system about the Z axis.
;               Must be between -30 and 210 degrees. Default is 30 degrees.
;
;       BACKGROUND   The color index of the plot background. Default is BOTTOM. 
;
;       BOTTOM   The starting index of the color table. Default is 0.
;
;       COLOR    The color index of the plot axes and annotation. Default is
;                (NCOLORS-1) + BOTTOM.
;
;       CHARSIZE   The size of the plot annotations. Default is 1.0.
;
;       _EXTRA   This keyword allows other undefined keywords to be passed
;                to the SURFACE command. For example: XTITLE, YTITLE, ZTITLE,
;                and TITLE keywords.
;
;       GROUP    The group leader of the GUI. Normally used with GUI.
;
;       GUI      Set this keyword if you want a graphical user interface to the
;                program functionality.
;
;       NCOLORS  The number of colors used in the color table. The default is
;                !D.N_COLORS.
;
;       NOAXES   Set this keyword to draw the plot without axes.
;
;       POSTSCRIPT   This will be a strucuture of the type returned by PS_FORM.
;                The fields of the structure are used to congfigue the PostScript device.
;
;       THISINSTANCE   Multiple versions of the GUI program can be running concurrently.
;                This keyword is used to make sure they can be managed by XMANAGER
;                concurrently. The keyword is NOT normally set by the user.
;
;       TRANSPARENT   This keyword sets the transparency threshold for the 3D volume.
;                allowed values go from 0 to 100 percent. The default is 0.
;
;       WID      The window index number of the window you wish the display to go into.
;                In non-GUI mode, the window does not have to currently be open. This
;                keyword is ignored in GUI mode.
;
;       WTITLE   The window title. Used in conjunction with THISINSTANCE to label
;                control and graphics windows. It can be set by the user.
;
;       XCOORD   The X coordinate of the 3D data set. The data will be sliced at this 
;                coordinate in the X direction. The default is XSIZE / 2.
; 
;       XSLICE   Set this keyword to draw an slice through the data in the X direction.
;
;       YCOORD   The Y coordinate of the 3D data set. The data will be sliced at this 
;                coordinate in the Y direction. The default is YSIZE / 2.
; 
;       YSLICE   Set this keyword to draw an slice through the data in the Y direction.
;
;       ZCOORD   The Z coordinate of the 3D data set. The data will be sliced at this 
;                coordinate in the Z direction. The default is ZSIZE / 2.
; 
;       ZSLICE   Set this keyword to draw an slice through the data in the Z direction.
;
;       
; OUTPUTS:
;
;	None.
;;
; COMMON BLOCKS:
;	
;       None.
;
; RESTRICTIONS:
;
;	Requires modified IDL library routines CW_ANIMATE and XINTERANIMATE.
;       To restore all program dependencies, restore the file "sliceadd.sav'
;       before compiling and running the SLICE program. 
;
; EXAMPLE:
;
;      To run this program with a GUI, a charcoal background color, axes
;      drawn in yellow and with the X and Y axes shown, type:
;	
;      filename = FILEPATH(SUBDIR=['examples', 'data'], 'abnorm.dat')
;      OPENR, lun, filename, /GET_LUN
;      heart = BYTARR(64,64,15)
;      READU, lun, heart
;      FREE_LUN, lun
;      LOADCT, 5, NCOLORS=150, BOTTOM=0
;      TVLCT, [70, 255], [70, 255], [70, 0], 150
;      RESTORE, 'sliceadd.sav'
;      SLICE, heart, /GUI, BACKGROUND=150, COLOR=151, NCOLORS=150, $
;         BOTTOM=0, /XSLICE, /YSLICE
;
; MODIFICATION HISTORY:
; 	Written by:	David Fanning, Sept 1996.
;       31 Oct 96: Made slight changes in how PostScript output is handled.
;-



FUNCTION WHAT_BUTTON_TYPE, event

   ; Checks event.type to find out what kind of button
   ; was clicked in a draw widget. This is NOT an event handler.

type = ['DOWN', 'UP', 'MOTION', 'SCROLL']
RETURN, type(event.type)
END ;*******************************************************************




FUNCTION WHAT_BUTTON_PRESSED, event

   ; Checks event.press to find out what kind of button
   ; was pressed in a draw widget.  This is NOT an event handler.

button = ['NONE', 'LEFT', 'MIDDLE', 'NONE', 'RIGHT']
RETURN, button(event.press)
END ;*******************************************************************



PRO SLICE_DRAW_MOTION_EVENTS, event

   ; Get info structure.

WIDGET_CONTROL, event.top, GET_UVALUE=tlb
WIDGET_CONTROL, tlb, GET_UVALUE=info, /NO_COPY

   ; What type of an event is this?

thisButtonType = WHAT_BUTTON_TYPE(event)

   ; Is this a button UP event?
   
IF thisButtonType EQ 'UP' THEN BEGIN

      ; If this is an UP event, you need to erase the slice, turn motion 
      ; events OFF, set thedraw widget's event handler back to 
      ; SLICE_DRAW_BUTTON_EVENTS, and redraw the display with the new
      ; slice.
      
      ; Erase the slice one final time by copying from the pixmap.
     
   WSET, info.wid
   DEVICE, COPY = [0, 0, info.pixXsize, info.pixYsize, 0, 0, info.pixmapID]
   
      ; Delete the pixmap.
      
   WDELETE, info.pixmapID
   info.pixmapID = -1

      ; Turn motion events off.
   
    WIDGET_CONTROL, event.id, DRAW_MOTION_EVENTS=0, $
       EVENT_PRO='SLICE_DRAW_BUTTON_EVENTS'
 
   ; Set correct transformation matrix.
   
!P.T = info.pt

   ; Get the current coodinates in 3D space.
     
coords = CONVERT_COORD(event.x, event.y, /DEVICE, /TO_DATA, /T3D)
x=coords(0)
y=coords(1)

   ; Which slice are we talking about. Set the coordinates.
   
CASE info.interactive OF

   'LEFT': BEGIN
      x = FIX(x)
      info.xcoord = x > 0
      info.xcoord = info.xcoord < info.maxx
      WIDGET_CONTROL, info.xcoordID, SET_VALUE=STRTRIM(x,2)
      END
            
   'RIGHT': BEGIN
      y = FIX(y)
      info.ycoord = y > 0
      info.ycoord = info.ycoord < info.maxy
      WIDGET_CONTROL, info.ycoordID, SET_VALUE=STRTRIM(y,2)
      END
      
   ELSE:  
ENDCASE

   ; Call drawing procedure.
   
   IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
      SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
   WSET, info.wid

   HANDLE_VALUE, info.dataptr, data, /NO_COPY
   SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
      ZCOORD=info.zcoord, XSLICE=info.xslice, $
      YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
      AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
      BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
      BACKGROUND=info.background, COLOR=info.color, $
      CHARSIZE=info.charscale, NOAXES=info.noaxes, $
      _EXTRA=info.extra
   HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY  

      ; Put the info structure back into its storage location and then, out of here!
      
   WIDGET_CONTROL, tlb, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF ; thisEvent = UP

   ; Most of the action in this event handler occurs here while we are waiting
   ; for an UP event to occur. As long as we don't get it, keep erasing the
   ; old slice and drawing a new one.
   
   ; Erase the old slice.
   
WSET, info.wid
DEVICE, COPY = [0, 0, info.pixXsize, info.pixYsize, 0, 0, info.pixmapID]

   ; Set correct transformation matrix.
   
!P.T = info.pt

   ; Get the current coodinates in 3D space.
   
coords = CONVERT_COORD(event.x, event.y, /DEVICE, /TO_DATA, /T3D)
x=coords(0)
y=coords(1)

   ; Which button was pushed down? Draw the proper slice. 
   ; Left button = X slice, Middle button = Z slice, 
   ; Right button = Y slice.
   
CASE info.interactive OF

   'LEFT': BEGIN
      x = FIX(x)
      x = x > 0
      x = x < info.maxx
      PLOTS, [x, x, x, x, x], [0, info.maxy, info.maxy, 0, 0], $
         [0, 0, info.maxz, info.maxz, 0], /T3D, COLOR=info.color
      XYOUTS, 0.05, 0.05, 'X Coordinate = ' + STRTRIM(x,2), /NORMAL, $
         SIZE=info.charscale, COLOR=info.color
      END
            
   'RIGHT': BEGIN
      y = FIX(y)
      y = y > 0
      y = y < info.maxy
      PLOTS, [0, info.maxx, info.maxx, 0, 0], [y,y,y,y,y], $
         [0, 0, info.maxz, info.maxz, 0], /T3D, COLOR=info.color
      XYOUTS, 0.05, 0.05, 'Y Coordinate = ' + STRTRIM(y, 2), /NORMAL, $
         SIZE=info.charscale, COLOR=info.color
      END
   
   ELSE: 
ENDCASE
       
   ; Put the info structure back into its storage location.
   
WIDGET_CONTROL, tlb, SET_UVALUE=info, /NO_COPY
END ; of SLICE_DRAW_MOTION_EVENTS event handler **************************************



PRO SLICE_DRAW_BUTTON_EVENTS, event
thisButtonType = WHAT_BUTTON_TYPE(event)

   ; Is this a button down event? If not, return.
   
IF thisButtonType NE 'DOWN' THEN RETURN

   ; Don't want to handle middle mouse buttons.
   
buttonPressed = WHAT_BUTTON_PRESSED(event)
IF buttonPressed EQ 'MIDDLE' THEN RETURN

   ; Get info structure.

WIDGET_CONTROL, event.top, GET_UVALUE=tlb
WIDGET_CONTROL, tlb, GET_UVALUE=info, /NO_COPY

   ; Create a pixmap for the window.

WIDGET_CONTROL, info.graphics_tlb, TLB_GET_SIZE=gsize   
WINDOW, /FREE, XSIZE=gsize(0), YSIZE=gsize(1), /PIXMAP
info.pixmapID = !D.WINDOW
info.pixXsize = gsize(0)
info.pixYsize = gsize(1)

   ; Draw the plot here with no axes.
   
HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
  ZCOORD=info.zcoord, XSLICE=info.xslice, $
  YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
  AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
  BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
  BACKGROUND=info.background, COLOR=info.color, $
  CHARSIZE=info.charscale, NOAXES=1, $
  _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY  

   ; Set correct transformation matrix.
   
info.pt = !P.T

   ; Draw 3D box.

oldDevice = !D.NAME   
SET_PLOT, 'Z'

PLOTS, [0, 0, info.maxx, info.maxx, 0], [0, info.maxy, info.maxy, 0, 0], $
   [0, 0, 0, 0, 0], /T3D, COLOR=info.color
PLOTS, [0, 0, info.maxx, info.maxx, 0], [0, info.maxy, info.maxy, 0, 0], $
   [info.maxz, info.maxz, info.maxz, info.maxz, info.maxz], /T3D, COLOR=info.color
PLOTS, [0, 0, 0, 0, 0], [0, info.maxy, info.maxy, 0, 0], $
   [0, 0, info.maxz, info.maxz, 0], /T3D, COLOR=info.color
PLOTS, [info.maxx, info.maxx, info.maxx, info.maxx, info.maxx], $
   [0, info.maxy, info.maxy, 0, 0], [0, 0, info.maxz, info.maxz, 0], $
   /T3D, COLOR=info.color
picture = TVRD()
SET_PLOT, oldDevice
WSET, info.pixmapID
TV, picture

   ; Do the same thing in the display window.
   
WSET, info.wid
TV, picture
   
   ; Get the current coodinates in 3D space.
   
coords = CONVERT_COORD(event.x, event.y, /DEVICE, /TO_DATA, /T3D)
x=coords(0)
y=coords(1)

   ; Which button was pushed down? Draw the proper slice. 
   ; Left button = X slice, Right button = Y slice.
   
info.interactive = WHAT_BUTTON_PRESSED(event)

CASE info.interactive OF

   'LEFT': BEGIN
      x=info.xcoord
      PLOTS, [x, x, x, x, x], [0, info.maxy, info.maxy, 0, 0], $
         [0, 0, info.maxz, info.maxz, 0], /T3D, COLOR=info.color
      END
      
   'RIGHT': BEGIN
      y = info.ycoord
      PLOTS, [0, info.maxx, info.maxx, 0, 0], [y,y,y,y,y], $
         [0, 0, info.maxz, info.maxz, 0], /T3D, COLOR=info.color
      END
      
   ELSE:
ENDCASE
      
WIDGET_CONTROL, info.drawID, EVENT_PRO='SLICE_DRAW_MOTION_EVENTS', $
   DRAW_MOTION_EVENTS=1
WIDGET_CONTROL, tlb, SET_UVALUE=info, /NO_COPY
END ; of SLICE_DRAW_BUTTON_EVENTS event handler ***************************      
      


PRO SLICE_GUI_SAVE_DISPLAY, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY

   ; Get a copy of the data.
   
HANDLE_VALUE, info.dataptr, data, /NO_COPY
oldData = data
oldInfo = info
HANDLE_VALUE, info.dataptr, data, /NO_COPY, /SET

   ; Open a file to save the data and the info structure.
   
filename = 'slice' + STRTRIM(info.thisInstance, 2) + '.slc'
filename = PICKFILE(/WRITE, FILE=filename)
IF filename EQ '' THEN BEGIN
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF

SAVE, oldData, oldinfo, FILE=filename

WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_SAVE_DISPLAY event handler *********************



PRO SLICE_GUI_RESTORE_DISPLAY, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY

   ; Open a file to get the data and the info structure.
   
filename = PICKFILE(FILTER='*.slc')
IF filename EQ '' THEN BEGIN
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF

CATCH, error
IF error NE 0 THEN BEGIN
   ok = WIDGET_MESSAGE(!ERR_STRING)
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF
   
RESTORE, filename

CATCH, /CANCEL

   ; Free up old pointer and set new pointer to old data.
   
HANDLE_FREE, info.dataptr   
info.dataptr = HANDLE_CREATE(VALUE=oldData, /NO_COPY)

   ; Set the proper display parameters.
   
WIDGET_CONTROL, info.xcoordID, SET_VALUE=oldinfo.xcoord
WIDGET_CONTROL, info.ycoordID, SET_VALUE=oldinfo.ycoord
WIDGET_CONTROL, info.zcoordID, SET_VALUE=oldinfo.zcoord

   ; Set the transparent factor.
   
transpercent = (oldinfo.transparent - (oldinfo.bottom -1))/(oldinfo.ncolors*0.01)
WIDGET_CONTROL, info.transparentID, SET_VALUE=transpercent

   ; Find current selection for the droplist.
   
thisSelection = 0
IF oldinfo.xslice THEN thisSelection = thisSelection + 1
IF oldinfo.yslice THEN thisSelection = thisSelection + 2
IF oldinfo.zslice THEN thisSelection = thisSelection + 4

CASE thisSelection OF
   1: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=0
   2: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=1
   3: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=3
   4: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=2
   5: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=4
   6: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=5
   7: WIDGET_CONTROL, info.droplistID, SET_DROPLIST_SELECT=6
ENDCASE

   ; Set the axes rotations.
   
WIDGET_CONTROL, info.axsliderID, SET_VALUE=oldinfo.ax
WIDGET_CONTROL, info.azsliderID, SET_VALUE=oldinfo.az

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid
oldinfo.wid = info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=oldinfo.xcoord, YCOORD=oldinfo.ycoord, $
   ZCOORD=oldinfo.zcoord, XSLICE=oldinfo.xslice, $
   YSLICE=oldinfo.yslice, ZSLICE=oldinfo.zslice, AX=oldinfo.ax, $
   AZ=oldinfo.az, WID=oldinfo.wid, NCOLORS=oldinfo.ncolors, $
   BOTTOM=oldinfo.bottom, TRANSPARENT=oldinfo.transparent, $
   BACKGROUND=oldinfo.background, COLOR=oldinfo.color, $
   CHARSIZE=1.0*oldinfo.charscale, NOAXES=oldinfo.noaxes, $
   _EXTRA=oldinfo.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      

   ; Set up the info structure.

info = { dataptr:info.dataptr, $
         xcoord:oldinfo.xcoord, $
         ycoord:oldinfo.ycoord, $
         zcoord:oldinfo.zcoord, $
         xslice:oldinfo.xslice, $
         yslice:oldinfo.yslice, $
         zslice:oldinfo.zslice, $
         az:oldinfo.az, $
         ax:oldinfo.ax, $
         wid:oldinfo.wid, $
         ncolors:oldinfo.ncolors, $
         bottom:oldinfo.bottom, $
         transparent:oldinfo.transparent, $
         charscale:oldinfo.charscale, $
         background:oldinfo.background, $
         color:oldinfo.color, $
         noaxes:oldinfo.noaxes, $
         extra:oldinfo.extra, $
         wtitle:oldinfo.wtitle, $
         thisInstance:info.thisInstance, $
         xcoordID:info.xcoordID, $
         ycoordID:info.ycoordID, $
         zcoordID:info.zcoordID, $
         transparentID:info.transparentID, $
         maxx:oldinfo.maxx, $
         maxy:oldinfo.maxy, $
         maxz:oldinfo.maxz, $
         interactive:oldinfo.interactive, $
         pt:oldinfo.pt, $
         pixmapID:oldinfo.pixmapID, $  
         pixXsize:oldinfo.pixXsize, $
         pixYsize:oldinfo.pixYsize, $       
         droplistID:info.droplistID, $
         axsliderID:info.axsliderID, $
         azsliderID:info.azsliderID, $
         graphics_tlb:info.graphics_tlb, $
         drawID:info.drawID, $
         tlb:info.tlb, $
         postscript:info.postscript}
 
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_RESTORE_DISPLAY event handler *********************



PRO SLICE_GUI_ANIMATE, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
WIDGET_CONTROL, event.id, GET_UVALUE=slice

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

CASE slice OF 
   'X': BEGIN
        IF info.xslice EQ 0 THEN BEGIN
           info.xslice = 1
           setBack = 1
        ENDIF ELSE setBack = 0
        XINTERANIMATE, SET = [300, 300, info.maxx+1], /SHOWLOAD, $
        /CYCLE, BOTTOM=info.bottom, NCOLORS=info.ncolors
        FOR j=0,info.maxx DO BEGIN
           HANDLE_VALUE, info.dataptr, data, /NO_COPY
           SLICE, data, XCOORD=j(0), YCOORD=info.ycoord, $
           ZCOORD=info.zcoord, XSLICE=info.xslice, $
           YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
           AZ=info.az, WID=!D.WINDOW, NCOLORS=info.ncolors, $
           BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
           BACKGROUND=info.background, COLOR=info.color, $
           CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
           _EXTRA=info.extra
           HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY  
           XINTERANIMATE, FRAME=j, WINDOW=!D.WINDOW
        ENDFOR
        XINTERANIMATE, 20, GROUP=event.top
        IF setBack THEN info.xslice = 0
        END
        
    'Y': BEGIN
        IF info.yslice EQ 0 THEN BEGIN
           info.yslice = 1
           setBack = 1
        ENDIF ELSE setBack = 0
        XINTERANIMATE, SET = [300, 300, info.maxy+1], /SHOWLOAD, $
        /CYCLE, BOTTOM=info.bottom, NCOLORS=info.ncolors
        FOR j=0,info.maxy DO BEGIN
           HANDLE_VALUE, info.dataptr, data, /NO_COPY
           SLICE, data, XCOORD=info.xcoord, YCOORD=j(0), $
           ZCOORD=info.zcoord, XSLICE=info.xslice, $
           YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
           AZ=info.az, WID=!D.WINDOW, NCOLORS=info.ncolors, $
           BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
           BACKGROUND=info.background, COLOR=info.color, $
           CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
           _EXTRA=info.extra
           HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY  
           XINTERANIMATE, FRAME=j, WINDOW=!D.WINDOW
        ENDFOR
        XINTERANIMATE, 20, GROUP=event.top
        IF setBack THEN info.yslice = 0
        END
        
   'Z': BEGIN
        IF info.zslice EQ 0 THEN BEGIN
           info.zslice = 1
           setBack = 1
        ENDIF ELSE setBack = 0
        XINTERANIMATE, SET = [300, 300, info.maxz+1], /SHOWLOAD,  $
        /CYCLE, BOTTOM=info.bottom, NCOLORS=info.ncolors
        FOR j=0,info.maxz DO BEGIN
           HANDLE_VALUE, info.dataptr, data, /NO_COPY
           SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
           ZCOORD=j(0), XSLICE=info.xslice, $
           YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
           AZ=info.az, WID=!D.WINDOW, NCOLORS=info.ncolors, $
           BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
           BACKGROUND=info.background, COLOR=info.color, $
           CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
           _EXTRA=info.extra
           HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY  
           XINTERANIMATE, FRAME=j, WINDOW=!D.WINDOW
        ENDFOR
        XINTERANIMATE, 20, GROUP=event.top
        IF setBack THEN info.zslice = 0
        END

ENDCASE

WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_ANIMATE event handler ***************************


PRO SLICE_GUI_CLEANUP, id
WIDGET_CONTROL, id, GET_VALUE=info
IF N_ELEMENTS(info) NE 0 THEN HANDLE_FREE, info.dataptr
END ; of SLICE_GUI_CLEANUP routine *******************************



PRO SLICE_GUI_AXES_OFF, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
IF info.noaxes EQ 1 THEN BEGIN
   WIDGET_CONTROL, event.id, SET_VALUE='Turn Axes Off'
   info.noaxes = 0
ENDIF ELSE BEGIN
   WIDGET_CONTROL, event.id, SET_VALUE='Turn Axes On'
   info.noaxes = 1
ENDELSE

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_AXES_OFF event handler *************************



PRO SLICE_GUI_CHARSIZE, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
 
   ; Get the new character size.
 
scale = CHARSIZE(info.charscale)

   ; Did user cancel?
   
IF scale LT 0 THEN BEGIN
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
 ENDIF
 
info.charscale = scale

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_CHARSIZE event handler ******************************



PRO SLICE_GUI_AXES_COLOR, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
CHGCOLOR, info.color, Title='Change Axes Color', $
   GROUP=event.top
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_AXES_COLOR event handl*******************************


PRO SLICE_GUI_BACKGROUND_COLOR, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
CHGCOLOR, info.background, TITLE='Change Background Color', $
   GROUP=event.top
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_BACKGROUND_COLOR event handl*************************


PRO SLICE_GUI_NEW_DISPLAY, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY

   ; Create another SQRSLIICE_GUI application.

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

HANDLE_VALUE, info.dataptr, data, /NO_COPY
newdata = data
SLICE_GUI, newdata, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=0, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra, WTITLE=info.wtitle, $
   THISINSTANCE=info.thisInstance + 1
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_NEW_DISPLAY event handler *********************************



PRO SLICE_GUI_MAKE_WINDOW, info, XSIZE=xsize, YSIZE=ysize, $
   XOFFSET=xoffset, YOFFSET=yoffset
   
ON_ERROR, 1   
np = N_PARAMS()
IF np NE 1 THEN MESSAGE, 'Info structure parameter required.'

IF N_ELEMENTS(xsize) EQ 0 THEN xsize = 400
IF N_ELEMENTS(ysize) EQ 0 THEN ysize = 400
DEVICE, GET_SCREEN_SIZE=screenSize
xoff = (screenSize(0) - xsize) / 2.0
yoff = (screenSize(1) - ysize) / 2.0
IF N_ELEMENTS(xoffset) EQ 0 THEN xoffset = xoff
IF N_ELEMENTS(yoffset) EQ 0 THEN yoffset = yoff
thisTitle = 'Slice Resizeable Display: ' + STRTRIM(info.thisInstance, 2)

graphics_tlb = WIDGET_BASE(TLB_SIZE_EVENT=1, $
   EVENT_PRO='SLICE_GUI_RESIZE', $
   UVALUE=info.tlb, XOFFSET=xoffset, YOFFSET=yoffset, $
   TITLE=thisTitle, GROUP=info.tlb)
drawID = WIDGET_DRAW(graphics_tlb, XSIZE=xsize, YSIZE=ysize, $
   EVENT_PRO='SLICE_DRAW_BUTTON_EVENTS')
WIDGET_CONTROL, graphics_tlb, /REALIZE
WIDGET_CONTROL, drawID, GET_VALUE=wid
info.wid = wid
info.drawID = drawID
info.graphics_tlb = graphics_tlb
WSET, info.wid
END ; of SLICE_GUI_MAKE_WINDOW utility routine *********************************



PRO SLICE_GUI_TRANSPARENT, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
 
   ; Get the new transparent factor.
   
WIDGET_CONTROL, event.id, GET_VALUE=transpercent

transvalue = ((info.bottom-1)+(transpercent*info.ncolors*0.01)) > 0

info.transparent = transvalue

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_TRANSPARENT event handler *********************************



PRO SLICE_GUI_ROTATE, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
 
   ; Get the new rotation.
   
WIDGET_CONTROL, event.id, GET_VALUE=thisRotation

   ; Branch appropriately.
   
CASE event.id OF
   info.axsliderID: info.ax = thisRotation
   info.azsliderID: info.az = thisRotation
ENDCASE

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_ROTATE event handler *********************************



PRO SLICE_GUI_COORDINATE_SELECT, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
 
   ; Get the new coordinate.
   
WIDGET_CONTROL, event.id, GET_VALUE=thisCoordinate

   ; Branch appropriately.
   
CASE event.id OF
   info.xcoordID: BEGIN
   
      IF thisCoordinate LT 0 OR thisCoordinate GT info.maxx THEN BEGIN
         ok = WIDGET_MESSAGE(['The value ' + STRTRIM(thisCoordinate, 2) + $
         ' is out of range.', 'Range: 0 - ' + STRTRIM(info.maxx,2)])
         WIDGET_CONTROL, info.xcoordID, SET_VALUE=info.xcoord
         WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
         RETURN
      ENDIF
      info.xcoord = thisCoordinate
      END
      
   info.ycoordID: BEGIN
      IF thisCoordinate LT 0 OR thisCoordinate GT info.maxy THEN BEGIN
         ok = WIDGET_MESSAGE(['The value ' + STRTRIM(thisCoordinate, 2) + $
         ' is out of range.', 'Range: 0 - ' + STRTRIM(info.maxy,2)])
         WIDGET_CONTROL, info.ycoordID, SET_VALUE=info.ycoord
         WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
         RETURN
      ENDIF
      info.ycoord = thisCoordinate
      END
      
   info.zcoordID: BEGIN
      IF thisCoordinate LT 0 OR thisCoordinate GT info.maxz THEN BEGIN
         ok = WIDGET_MESSAGE(['The value ' + STRTRIM(thisCoordinate, 2) + $
         ' is out of range.', 'Range: 0 - ' + STRTRIM(info.maxz, 2)])
         WIDGET_CONTROL, info.zcoordID, SET_VALUE=info.zcoord
         WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
         RETURN
      ENDIF
      info.zcoord = thisCoordinate
      END
      
ENDCASE

   ; User could type in bogus value. Check for it.

CATCH, error
IF error NE 0 THEN BEGIN
   ok = WIDGET_MESSAGE(['Problem with coordinate you', $
      'just typed in. Returning...'])
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
   
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_COORDINATE_SELECT event handler ***************************



PRO SLICE_GUI_SLICE_SELECT, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
WIDGET_CONTROL, event.id, GET_UVALUE=options
CASE options(event.index) OF
   'X Only': BEGIN
      info.xslice = 1
      info.yslice = 0
      info.zslice = 0
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'Y Only': BEGIN
      info.xslice = 0
      info.yslice = 1
      info.zslice = 0
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'Z Only': BEGIN
      info.xslice = 0
      info.yslice = 0
      info.zslice = 1
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'X and Y': BEGIN
      info.xslice = 1
      info.yslice = 1
      info.zslice = 0
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'X and Z': BEGIN
      info.xslice = 1
      info.yslice = 0
      info.zslice = 1
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'Y and Z': BEGIN
      info.xslice = 0
      info.yslice = 1
      info.zslice = 1
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'XYZ': BEGIN
      info.xslice = 1
      info.yslice = 1
      info.zslice = 1
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=0
      END
   'Interactive XY': BEGIN
      WIDGET_CONTROL, info.drawID, DRAW_BUTTON_EVENTS=1
      END
ENDCASE

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Call the drawing procedure.
   
IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid

HANDLE_VALUE, info.dataptr, data, /NO_COPY
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY      
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_SLICE_SELECT event handler ************************


PRO SLICE_GUI_LOAD_DATA, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY

CATCH, error
IF error NE 0 THEN BEGIN
   ok = WIDGET_MESSAGE(['You must download the file GETIMAGE.PRO', $
      'from the TRAINING directory at RSI. E-mail', $
      'davidf@fortnet.org for more information.'])
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF
      
data = GETIMAGE()

CATCH, /CANCEL

IF N_ELEMENTS(data) LE 1 THEN BEGIN
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF
s = SIZE(data)
IF s(0) NE 3 THEN BEGIN
   ok = WIDGET_MESSAGE('New data set must be 3D.')
   WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
   RETURN
ENDIF
xs = s(1)
ys = s(2)
zs = s(3)

   ; Use current coodinates if they fit, otherwise
   ; update them.

IF info.xcoord GE s(1) THEN BEGIN
   info.xcoord = s(1) / 2
   WIDGET_CONTROL, info.xcoordID, SET_VALUE=info.xcoord
ENDIF   
IF info.ycoord GE s(2) THEN BEGIN
   info.ycoord = s(2) / 2
   WIDGET_CONTROL, info.ycoordID, SET_VALUE=info.ycoord
ENDIF   
IF info.zcoord GE s(3) THEN BEGIN
   info.zcoord = s(3) / 2
   WIDGET_CONTROL, info.zcoordID, SET_VALUE=info.zcoord
ENDIF   

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Make sure requested slices are inside the data.
   
info.maxX = xs - 1
info.maxY = ys - 1
info.maxZ = zs - 1
info.xcoord = 0 > info.xcoord
info.xcoord = info.xcoord < info.maxX
info.ycoord = 0 > info.ycoord
info.ycoord = info.ycoord < info.maxY
info.zcoord = 0 > info.zcoord
info.zcoord = info.zcoord < info.maxZ

IF NOT WIDGET_INFO(info.drawID, /VALID_ID) THEN $
   SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400
WSET, info.wid
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, /SET, data, /NO_COPY
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_LOAD_DATA event handler ******************************


PRO SLICE_GUI_CHANGE_COLORS, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
XCOLORS, NCOLORS=info.ncolors, BOTTOM=info.bottom, GROUP=event.top
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_CHANGE_COLORS event handler ***************************


PRO SLICE_GUI_POSTSCRIPT, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

   ; Get the user value of the button.
  
WIDGET_CONTROL, event.id, GET_UVALUE=thisUValue
CASE thisUVALUE of

   'CREATE': BEGIN
      HANDLE_VALUE, info.dataptr, data, /NO_COPY
      SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
         ZCOORD=info.zcoord, XSLICE=info.xslice, $
         YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
         AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
         BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
         BACKGROUND=info.color, COLOR=info.background, $
         POSTSCRIPT=info.postscript, $
         CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
         _EXTRA=info.extra
      HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY
      END
      
   'CONFIGURE': BEGIN
      keywords = PS_FORM(CANCEL=cancelled)
      IF NOT cancelled THEN info.postscript = keywords
      END
ENDCASE
WIDGET_CONTROL, event.top, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_POSTSCRIPT event handler *******************************



PRO SLICE_GUI_RESIZE, event
WIDGET_CONTROL, event.top, GET_UVALUE=tlb
WIDGET_CONTROL, tlb, GET_UVALUE=info, /NO_COPY
WSET, info.wid
ERASE
WIDGET_CONTROL, info.drawID, XSIZE=event.x, YSIZE=event.y

   ; Get the slice coordinates, just in case someone changed
   ; them and didn't hit a CR.
   
WIDGET_CONTROL, info.xcoordID, GET_VALUE=xcoord
info.xcoord = xcoord
WIDGET_CONTROL, info.ycoordID, GET_VALUE=ycoord
info.ycoord = ycoord
WIDGET_CONTROL, info.zcoordID, GET_VALUE=zcoord
info.zcoord = zcoord

HANDLE_VALUE, info.dataptr, data, /NO_COPY
WSET, info.wid
SLICE, data, XCOORD=info.xcoord, YCOORD=info.ycoord, $
   ZCOORD=info.zcoord, XSLICE=info.xslice, $
   YSLICE=info.yslice, ZSLICE=info.zslice, AX=info.ax, $
   AZ=info.az, WID=info.wid, NCOLORS=info.ncolors, $
   BOTTOM=info.bottom, TRANSPARENT=info.transparent, $
   BACKGROUND=info.background, COLOR=info.color, $
   CHARSIZE=1.0*info.charscale, NOAXES=info.noaxes, $
   _EXTRA=info.extra
HANDLE_VALUE, info.dataptr, data, /SET, /NO_COPY
WIDGET_CONTROL, tlb, SET_UVALUE=info, /NO_COPY
END ; of SLICE_GUI_RESIZE event handler *********************************



PRO SLICE_GUI_QUITTER, event
WIDGET_CONTROL, event.top, GET_UVALUE=info, /NO_COPY
HANDLE_FREE, info.dataptr
WIDGET_CONTROL, event.top, /DESTROY
END ; of SLICE_GUI_QUITTER event handler ********************************



PRO SLICE_GUI, data, XSLICE=xslice, YSLICE=yslice, ZSLICE=zslice, $
   WID=wid, NOAXES=noaxes, XCOORD=xcoord, YCOORD=ycoord, ZCOORD=zcoord, $
   NCOLORS=ncolors, BOTTOM=bottom, BACKGROUND=background, GROUP=group, $
   COLOR=color, AZ=az, AX=ax, TRANSPARENT=transparent, _EXTRA=extra, $
   CHARSIZE=charscale, WTITLE=wtitle, THISINSTANCE=thisInstance

ON_ERROR, 1

   ; If no data passed in, make some up for demo purposes.
   
np = N_PARAMS()   
IF np EQ 0 THEN BEGIN
    MESSAGE, 'No data passed in. Using jet data...', /INFORMATIONAL
    filename = FILEPATH(SUBDIR=['examples', 'data'], 'jet.dat')
    CATCH, error
    IF error NE 0 THEN BEGIN
       ok = WIDGET_MESSAGE("Can't find JET.DAT. Returning...")
       RETURN
    ENDIF
    OPENR, lun, filename, /GET_LUN
    CATCH, /CANCEL
    data = BYTARR(81, 40, 101)
    READU, lun, data
    FREE_LUN, lun
ENDIF

   ; Make sure data is 3D. Pick appropriate row and column slices.
   
s = SIZE(data)
IF s(0) NE 3 THEN MESSAGE, 'Data set must be 3D.'
xs = s(1)
ys = s(2)
zs = s(3)
IF N_ELEMENTS(xcoord) EQ 0 THEN xcoord= xs / 2
IF N_ELEMENTS(ycoord) EQ 0 THEN ycoord= ys / 2
IF N_ELEMENTS(zcoord) EQ 0 THEN zcoord= zs / 2

   ; Make sure requested slices are inside the data.
   
maxX = xs - 1
maxY = ys - 1
maxZ = zs - 1
xcoord = 0 > xcoord
xcoord = xcoord < maxX
ycoord = 0 > ycoord
ycoord = ycoord < maxY
zcoord = 0 > zcoord
zcoord = zcoord < maxZ

   ; What slices do you want. Must have something! X Slice is default.
   
xslice = KEYWORD_SET(xslice)
yslice = KEYWORD_SET(yslice)
zslice = KEYWORD_SET(zslice)
IF NOT xslice AND NOT yslice AND NOT zslice THEN xslice = 1

   ; Set colors. 
   
IF N_ELEMENTS(ncolors) EQ 0 THEN ncolors=!D.N_COLORS
IF N_ELEMENTS(bottom) EQ 0 THEN bottom = 0
IF N_ELEMENTS(background) EQ 0 THEN background = bottom
IF N_ELEMENTS(color) EQ 0 THEN color = (ncolors-1) + bottom

   ; Default axes rotations. Keep in program range.
   
IF N_ELEMENTS(az) EQ 0 THEN az = 30
az = az > (-30)
az = az < 210
IF N_ELEMENTS(ax) EQ 0 THEN ax = 30
ax = ax > 0
ax = ax < 90

   ; Miscellaneous keywords.
   
IF N_ELEMENTS(noaxes) EQ 0 THEN noaxes = 0
IF N_ELEMENTS(transparent) EQ 0 THEN transparent = 0
transvalue = ((bottom-1)+(transparent*ncolors*0.01)) > 0
IF N_ELEMENTS(charscale) EQ 0 THEN charscale = 1.0
IF N_ELEMENTS(extra) EQ 0 THEN extra = CREATE_STRUCT('title', '')
IF N_ELEMENTS(wid) EQ 0 THEN wid = -1
IF N_ELEMENTS(thisInstance) EQ 0 THEN thisInstance = 1
IF N_ELEMENTS(wtitle) EQ 0 THEN wtitle = 'Slice Controls: '

   ; Only one program by this name, please!
   
registerName = 'xsection_gui' + STRTRIM(thisInstance, 2)
IF XREGISTERED(registerName) THEN BEGIN
   ok = WIDGET_MESSAGE(['Sorry, a new display cannot be opened', $
   'from this SLICE program. Try the SLICE', $
   'program that is now forward on the display.'])
   RETURN
ENDIF
   
   ; Create the control widgets.
   
tlb = WIDGET_BASE(COLUMN=1, TLB_FRAME_ATTR=1, $
   TITLE=wtitle + STRTRIM(thisInstance, 2))

coordbase = WIDGET_BASE(tlb, COLUMN=1, FRAME=1, $
   EVENT_PRO='SLICE_GUI_COORDINATE_SELECT')
label = WIDGET_LABEL(coordbase, Value='Slice Coordinates')
xcoordID = CW_FIELD(coordbase, /INTEGER, Value=xcoord, TITLE='X Coordinate: ', $
   RETURN_EVENTS=1)
ycoordID = CW_FIELD(coordbase, /INTEGER, Value=ycoord, TITLE='Y Coordinate: ', $
   RETURN_EVENTS=1)
zcoordID = CW_FIELD(coordbase, /INTEGER, Value=zcoord, TITLE='Z Coordinate: ', $
   RETURN_EVENTS=1)

slicebase = WIDGET_BASE(tlb, COLUMN=1, FRAME=1)
options = ['X Only', 'Y Only', 'Z Only', 'X and Y', 'X and Z', 'Y and Z', $
   'XYZ', 'Interactive XY']
droplistID = WIDGET_DROPLIST(slicebase, Value=options, $
   UValue=options, TITLE='Slice Selection: ', $
   EVENT_PRO='SLICE_GUI_SLICE_SELECT')
   
   ; Find current selection for the droplist.
   
thisSelection = 0
IF xslice THEN thisSelection = thisSelection + 1
IF yslice THEN thisSelection = thisSelection + 2
IF zslice THEN thisSelection = thisSelection + 4

CASE thisSelection OF
   1: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=0
   2: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=1
   3: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=3
   4: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=2
   5: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=4
   6: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=5
   7: WIDGET_CONTROL, droplistID, SET_DROPLIST_SELECT=6
ENDCASE

   ; Rotation and transparency.

rotbase = WIDGET_BASE(tlb, COLUMN=1, FRAME=1)
azsliderID = WIDGET_SLIDER(rotbase, TITLE='Rotate About Z', MIN=-30, $
   EVENT_PRO='SLICE_GUI_ROTATE', MAX=210, VALUE=az, SCR_XSIZE=200)
axsliderID = WIDGET_SLIDER(rotbase, TITLE='Rotate About X', MIN=0, $
   EVENT_PRO='SLICE_GUI_ROTATE', MAX=90, VALUE=ax, SCR_XSIZE=200)
transparentID = WIDGET_SLIDER(rotbase, TITLE='Transparency Threshold', $
   MIN=0, EVENT_PRO='SLICE_GUI_TRANSPARENT', MAX=100, $
   VALUE=transparent, SCR_XSIZE=200)
   
   ; Animate pull-down menu.
   
animateSlice = WIDGET_BUTTON(tlb, VALUE='Animate Slice', MENU=2,  $
   EVENT_PRO='SLICE_GUI_ANIMATE')
junk = WIDGET_BUTTON(animateSlice, VALUE='X Slice', UVALUE='X')
junk = WIDGET_BUTTON(animateSlice, VALUE='Y Slice', UVALUE='Y')
junk = WIDGET_BUTTON(animateSlice, VALUE='Z Slice', UVALUE='Z')

   ; Graphics display options.
   
character = WIDGET_BUTTON(tlb, VALUE='Graphics Display Properties', MENU=2)
changecolorID = WIDGET_BUTTON(character, VALUE='Change Data Colors', $
   EVENT_PRO='SLICE_GUI_CHANGE_COLORS')
backcolor = WIDGET_BUTTON(character, VALUE='Change Background Color', $
   EVENT_PRO='SLICE_GUI_BACKGROUND_COLOR')
axescolor = WIDGET_BUTTON(character, VALUE='Change Axes Color', $
   EVENT_PRO='SLICE_GUI_AXES_COLOR')
IF noaxes THEN noaxesText = ' Turn Axes On ' ELSE noaxesText = 'Turn Axes Off'
axesoff = WIDGET_BUTTON(character, VALUE=noaxesText, $
   EVENT_PRO='SLICE_GUI_AXES_OFF')
annotationSize = WIDGET_BUTTON(character, VALUE='Change Character Size', $
   EVENT_PRO='SLICE_GUI_CHARSIZE')
 saveDisplay = WIDGET_BUTTON(character, VALUE='SAVE Graphic Display', $
   EVENT_PRO='SLICE_GUI_SAVE_DISPLAY')
 restoreDisplay = WIDGET_BUTTON(character, VALUE='RESTORE Graphic Display', $
   EVENT_PRO='SLICE_GUI_RESTORE_DISPLAY')

   ; Hardcopy output.
   
hardcopyID = WIDGET_BUTTON(tlb, Value='PostScript Output', $
   EVENT_PRO='SLICE_GUI_POSTSCRIPT', /MENU)
makeFile = WIDGET_BUTTON(hardcopyID, VALUE='Create PostScript File', $
   UVALUE='CREATE')
configFile = WIDGET_BUTTON(hardcopyID, VALUE='Configure PostScript Device', $
   UVALUE='CONFIGURE')
   
   ; Load new data.
   
loaddataID = WIDGET_BUTTON(tlb, VALUE='Load New Data', $
   EVENT_PRO='SLICE_GUI_LOAD_DATA')
   
   ; New graphic display.
   
newdisplayID = WIDGET_BUTTON(tlb, Value='New Graphic Display', $
   EVENT_PRO='SLICE_GUI_NEW_DISPLAY')
   
   ; Quit button.
   
quitID = WIDGET_BUTTON(tlb, VALUE='QUIT', EVENT_PRO='SLICE_GUI_QUITTER')

   ; Get initial PostScript setups.
   
postscript = PS_FORM(/INITIALIZE)

   ; Show it.
   
WIDGET_CONTROL, tlb, /REALIZE

   ; Set up the info structure.

info = { dataptr:-1L, $
         xcoord:xcoord, $
         ycoord:ycoord, $
         zcoord:zcoord, $
         xslice:xslice, $
         yslice:yslice, $
         zslice:zslice, $
         az:az, $
         ax:ax, $
         wid:wid, $
         ncolors:ncolors, $
         bottom:bottom, $
         transparent:transvalue, $
         charscale:charscale, $
         background:background, $
         color:color, $
         noaxes:noaxes, $
         extra:extra, $
         wtitle:wtitle, $
         thisInstance:thisInstance, $
         xcoordID:xcoordID, $
         ycoordID:ycoordID, $
         zcoordID:zcoordID, $
         transparentID:transparentID, $
         interactive:'', $
         pt:!P.T, $
         pixmapID:-1, $
         pixXsize:-1, $
         pixYsize:-1, $       
         maxx:maxx, $
         maxy:maxy, $
         maxz:maxz, $
         droplistID:droplistID, $
         axsliderID:axsliderID, $
         azsliderID:azsliderID, $
         graphics_tlb:-1L, $
         drawID:-1L, $
         tlb:tlb, $
         postscript:postscript}
 
   ; Create the display window.
   
WIDGET_CONTROL, tlb, TLB_GET_OFFSET=tlb_offsets, TLB_GET_SIZE=tlb_sizes
thisXoff = tlb_offsets(0) + tlb_sizes(0) + 50
thisYoff = tlb_offsets(1) 
SLICE_GUI_MAKE_WINDOW, info, XSIZE=400, YSIZE=400, XOFFSET=thisXoff, YOFFSET=thisYoff

   ; Put initial display into the display window.
  
SLICE, data, XCOORD=xcoord, YCOORD=ycoord, ZCOORD=zcoord, XSLICE=xslice, $
   YSLICE=yslice, ZSLICE=zslice, AX=ax, AZ=az, WID=info.wid, NCOLORS=ncolors, $
   BOTTOM=bottom, TRANSPARENT=transparent, BACKGROUND=background, COLOR=color, $
   CHARSIZE=1.0*charscale, NOAXES=noaxes, _EXTRA=extra

   ; Get correct transformation in the info.pt field.
   
info.pt = !P.T
   
   ; Set up pointer to data so data can be easily changed.

info.dataptr = HANDLE_CREATE(VALUE=data, /NO_COPY)
   
   ; Store the info structure and set focus on the control panel.
   
WIDGET_CONTROL, tlb, SET_UVALUE=info, /NO_COPY, $
   EVENT_PRO='SLICE_GUI_RESIZE', /SHOW
XMANAGER, registerName, tlb, EVENT_HANDLER='SLICE_GUI_RESIZE', $
   GROUP_LEADER=group, CLEANUP = 'SLICE_GUI_CLEANUP'

END ; of SLICE_GUI PROGRAM *************************************************



FUNCTION DELAY_NOTICE, question

   ; Default question.
   
IF N_PARAMS() EQ 0 THEN question = 'There will be a short delay ...'

   ; Put up in the middle of the display. NOT a modal widget.
   
DEVICE, GET_SCREEN_SIZE=screenSize

tlb = WIDGET_BASE(XOFFSET=screenSize(0)/2.0 - 75, YOFFSET=screenSize(1)/ 3.0)
label = WIDGET_LABEL(tlb, VALUE=question)

WIDGET_CONTROL, tlb, /REALIZE
RETURN, tlb
END ; of DELAY_NOTICE utility routine ****************************************



PRO SLICE, data, XSLICE=xslice, YSLICE=yslice, ZSLICE=zslice, WID=wid, $
   NOAXES=noaxes, XCOORD=xcoord, YCOORD=ycoord, ZCOORD=zcoord, $
   NCOLORS=ncolors, BOTTOM=bottom, POSTSCRIPT=postscript, BACKGROUND=background, $
   COLOR=color, AZ=az, AX=ax, TRANSPARENT=transparent, _EXTRA=extra, $
   CHARSIZE=charscale, GUI=gui, WTITLE=wtitle, THISINSTANCE=thisInstance

   ; Error checking
   
ON_ERROR, 1
np = N_PARAMS()

   ; If no data passed in, make some up for demo purposes.
   
IF np EQ 0 THEN BEGIN
    MESSAGE, 'No data passed in. Using jet data...', /INFORMATIONAL
    filename = FILEPATH(SUBDIR=['examples', 'data'], 'jet.dat')
    CATCH, error
    IF error NE 0 THEN BEGIN
       ok = WIDGET_MESSAGE("Can't find JET.DAT. Returning...")
       RETURN
    ENDIF
    OPENR, lun, filename, /GET_LUN
    CATCH, /CANCEL
    data = BYTARR(81, 40, 101)
    READU, lun, data
    data = CONGRID(TEMPORARY(data), 20, 10, 25, /INTERP)
    FREE_LUN, lun
 ENDIF

   ; Make sure data is 3D. Pick appropriate row and column slices.
   
s = SIZE(data)
IF s(0) NE 3 THEN MESSAGE, 'Data set must be 3D.'
xs = s(1)
ys = s(2)
zs = s(3)
IF N_ELEMENTS(xcoord) EQ 0 THEN xcoord= xs / 2
IF N_ELEMENTS(ycoord) EQ 0 THEN ycoord= ys / 2
IF N_ELEMENTS(zcoord) EQ 0 THEN zcoord= zs / 2

   ; What slices do you want. Must have something! X Slice is default.
   
xslice = KEYWORD_SET(xslice)
yslice = KEYWORD_SET(yslice)
zslice = KEYWORD_SET(zslice)
IF NOT xslice AND NOT yslice AND NOT zslice THEN xslice = 1

   ; Set colors.
   
IF N_ELEMENTS(ncolors) EQ 0 THEN ncolors=!D.N_COLORS
IF N_ELEMENTS(bottom) EQ 0 THEN bottom = 0
IF N_ELEMENTS(transparent) EQ 0 THEN transparent = 0
transparent = transparent > 0
transparent = transparent < 100
IF N_ELEMENTS(background) EQ 0 THEN BEGIN
   IF KEYWORD_SET(postscript) THEN background = 255 ELSE background = bottom
ENDIF
IF N_ELEMENTS(color) EQ 0 THEN BEGIN
   IF KEYWORD_SET(postscript) THEN color = 0 ELSE color = (ncolors-1) + bottom
ENDIF

   ; Miscellaneous keywords.
   
IF N_ELEMENTS(charscale) EQ 0 THEN charscale = 1.0
IF N_ELEMENTS(extra) EQ 0 THEN extra = CREATE_STRUCT('title', '')
IF N_ELEMENTS(wid) EQ 0 THEN wid = !D.WINDOW
IF N_ELEMENTS(wtitle) EQ 0 THEN wtitle='Slice Graphics Window'
IF N_ELEMENTS(thisInstance) EQ 0 THEN thisInstance = 1
noaxes = KEYWORD_SET(noaxes)

   ; Default axes rotations.
   
IF N_ELEMENTS(az) EQ 0 THEN az = 30
IF N_ELEMENTS(ax) EQ 0 THEN ax = 30

   ; Does user want a widget program?
   
IF KEYWORD_SET(gui) THEN BEGIN
   SLICE_GUI, data, XSLICE=xslice, YSLICE=yslice, ZSLICE=zslice, $
   WID=wid, NOAXES=noaxes, XCOORD=xcoord, YCOORD=ycoord, ZCOORD=zcoord, $
   NCOLORS=ncolors, BOTTOM=bottom, BACKGROUND=background, GROUP=group, $
   COLOR=color, AZ=az, AX=ax, TRANSPARENT=transparent, _EXTRA=extra, $
   CHARSIZE=charscale, WTITLE='Slice Controls: ', THISINSTANCE=thisInstance
   RETURN
ENDIF

   ; Create a window if one is needed.

IF wid EQ -1 THEN BEGIN
   IF NOT KEYWORD_SET(postscript) THEN $
      WINDOW, /FREE, XSIZE=400, YSIZE=400, TITLE=wtitle
   wid = !D.WINDOW
ENDIF

   ; Make sure requested slices are inside the data.
   
maxX = xs - 1
maxY = ys - 1
maxZ = zs - 1
maxData = MAX(data, MIN=minData)
xcoord = 0 > xcoord
xcoord = xcoord < maxX
ycoord = 0 > ycoord
ycoord = ycoord < maxY
zcoord = 0 > zcoord
zcoord = zcoord < maxZ

   ; Obtain the slice planes.
   
xplane = [ [xcoord, 0, 0], [xcoord, 0, maxz], [xcoord, maxy, maxz], [xcoord, maxy, 0] ]
yplane = [ [0, ycoord, 0], [0, ycoord, maxz], [maxx, ycoord, maxz], [maxx, ycoord, 0] ]
zplane = [ [ 0, 0, zcoord], [maxx, 0, zcoord], [maxx, maxy, zcoord], [0, maxy, zcoord] ]

   ; Obtain the slice images.

ximage = data(xcoord, *, *)
yimage = data(*, ycoord, *)
zimage = data(*, *, zcoord)

   ; Make a 2D image out of the images.

ximage = REFORM(ximage)
yimage = REFORM(yimage)
zimage = REFORM(zimage)

   ; Scale images appropriately for number of colors.
     
ximage = BYTSCL(ximage, TOP=ncolors-1, MAX=maxData, MIN=minData) + BYTE(bottom)
yimage = BYTSCL(yimage, TOP=ncolors-1, MAX=maxData, MIN=minData) + BYTE(bottom)
zimage = BYTSCL(zimage, TOP=ncolors-1, MAX=maxData, MIN=minData) + BYTE(bottom)

   ; If the window index number doesn't exist, create it.

CATCH, error
IF error NE 0 THEN BEGIN
   wid = wid < 31
   WINDOW, wid, XSIZE=400, YSIZE=400, TITLE=wtitle
ENDIF
IF NOT KEYWORD_SET(postscript) THEN WSET, wid
CATCH, /CANCEL
 
   ; The slices will be rendered in the Z-Buffer. 
   ; Set Z-Buffer size.

IF KEYWORD_SET(postscript) THEN xsize=400 ELSE xsize = !D.X_SIZE 
IF KEYWORD_SET(postscript) THEN ysize=400 ELSE ysize = !D.Y_SIZE 

oldDevice = !D.NAME  
SET_PLOT, 'Z'
DEVICE, SET_RESOLUTION=[xsize, ysize], SET_COLORS=ncolors

   ; Set up 3D coordinate system.
   
SURFACE, dist(30), /NODATA, XRANGE=[0,maxx], YRANGE=[0,maxy], ZRANGE=[0,maxz], $
   XSTYLE=5, YSTYLE=5, ZSTYLE=5, POSITION=[0.15, 0.15, 0.95, 0.95, 0.1, 0.95], $
   /SAVE, AZ=az, AX=ax

   ; Set up background color.
   
POLYFILL, [0, 0, 1.01, 1.01, 0], [0, 1.01, 1.01, 0, 0], /NORMAL, COLOR=background
      
   ; Render the slices.

IF xslice THEN POLYFILL, xplane, /T3D, PATTERN=ximage, $
   IMAGE_COORD=[ [0,0], [0, zs-1], [ys-1, zs-1], [ys-1, 0] ], /IMAGE_INTERP, $
   TRANSPARENT=transparent
   
IF yslice THEN POLYFILL, yplane, /T3D, PATTERN=yimage, $
   IMAGE_COORD=[  [0,0], [0, zs-1], [xs-1, zs-1], [xs-1, 0] ], /IMAGE_INTERP, $
   TRANSPARENT=transparent
   
IF zslice THEN POLYFILL, zplane, /T3D, PATTERN=zimage, $
   IMAGE_COORD=[ [0,0], [xs-1, 0], [xs-1, ys-1], [0, ys-1] ], /IMAGE_INTERP, $
   TRANSPARENT=transparent
   
   ; Add axes if needed and not in PostScript mode.
   
IF NOT KEYWORD_SET(postscript) THEN $          
IF NOT noaxes THEN SURFACE, dist(30), /NODATA, XRANGE=[0,maxx], $
   YRANGE=[0,maxy], ZRANGE=[0,maxz], XSTYLE=1, YSTYLE=1, ZSTYLE=1, /NOERASE, $
   POSITION=[0.15, 0.15, 0.95, 0.95, 0.1, 0.95], XTITLE='X Coordinate', $
   YTITLE='Y Coordinate', ZTITLE='Z Coordinate', CHARSIZE=1.0*charscale, $
   COLOR=color, AZ=az, AX=ax, _EXTRA=extra
   
   ; Take a snap-shot.
   
picture = TVRD(0, 0, xsize-1, ysize-1)

   ; Display the data in the display window. 
   ; Make sure window is valid and whether the device is PostScript.
   
SET_PLOT, oldDevice

   ; Can ask for a window that is not available. Anticipate!!
   
CATCH, error
IF error NE 0 THEN BEGIN
   ok = WIDGET_MESSAGE(['Requested window index number '+ STRTRIM(wid,2) + ' is', $
      'not available. Using current window index.'])
   wid = !D.WINDOW
   IF wid EQ -1 THEN BEGIN
      ok = WIDGET_MESSAGE("Can't find valid display window. Returning...")
      RETURN
   ENDIF
ENDIF

IF NOT KEYWORD_SET(postscript) THEN WSET, wid
CATCH, /CANCEL

   ; Special handling if in PostScript.
   
IF KEYWORD_SET(postscript) THEN BEGIN

      ; Get instructions for configuring the device from the user.
   
   deviceKeywords = PS_FORM(INITIALIZE=1, LOCALDEFAULTS=postscript)
      
      ; Let user know something is going on.
      
   notice = DELAY_NOTICE('Writing PostScript file...')

      ; Configure PostScript device.
         
   oldDevice = !D.NAME
   IF oldDevice NE 'PS' THEN BEGIN
      SET_PLOT, 'PS'
      setBack = 1
   ENDIF ELSE setBack = 0    
   DEVICE, _EXTRA=deviceKeywords
   filename = deviceKeywords.filename
      
   ; Special PostScript processing for backgrounds and axes color.
   
   index = WHERE(picture EQ background, count)
   IF count GT 0 THEN picture(index) = 255
   TVLCT, r, g, b, /GET
   rThis = r(0)
   gThis = g(0)
   bThis = b(0)
   TVLCT, 0, 0, 0, 0
   color = 0
   
      ; Display the image.
      
   TV, picture, XSIZE=deviceKeywords.xsize, YSIZE=deviceKeywords.ysize, $
      Inches=deviceKeywords.inches
         
      ; Add axes if needed.
         
    IF NOT noaxes THEN SURFACE, DIST(30,30), /NODATA, XRANGE=[0,maxx], $
       YRANGE=[0,maxy], ZRANGE=[0,maxz], XSTYLE=1, YSTYLE=1, ZSTYLE=1, /NOERASE, $
       POSITION=[0.15, 0.15, 0.95, 0.95, 0.1, 0.95], XTITLE='X Coordinate', $
       YTITLE='Y Coordinate', ZTITLE='Z Coordinate', _EXTRA=extra, $
       COLOR=color, CHARSIZE=1.0*charscale, AZ=az, AX=ax
        
    TVLCT, rThis, gThis, bThis, 0   
    DEVICE, /CLOSE
    WIDGET_CONTROL, notice, /DESTROY
    CD, CURRENT=currentDirectory
    Print, 'File "' + filename + '" written to directory "', currentDirectory, '"'
    IF setBack THEN SET_PLOT, oldDevice
ENDIF ELSE TV, picture

END ; of SLICE PROGRAM **********************************************************



   ; EXAMPLE program to see how it works.

PRO EXAMPLE

   ; How many colors?
   
WINDOW, /FREE, /PIXMAP, XSIZE=10, YSIZE=10
WDELETE, !D.WINDOW
ncolors = !D.N_COLORS

   ; Load data and drawing colors
   
LOADCT, 5, NCOLORS=ncolors-2, BOTTOM=0
TVLCT, [70, 255], [70, 255], [70, 0], ncolors-2

   ; Restore SLICE auxilliary files.
   
RESTORE, 'sliceadd.sav'

   ; Run the SLICE program.
   
SLICE, /GUI, BACKGROUND=ncolors-2, COLOR=ncolors-1, NCOLORS=ncolors-2, $
   BOTTOM=0, /XSLICE, /YSLICE
END