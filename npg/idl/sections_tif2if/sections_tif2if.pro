; $Id: sections_tif2if.pro,v 1.2 2005/10/20 21:22:20 tom Exp $
;+
; NAME:
;   SECTIONS_TIF2IF
;
; PURPOSE:
;   Convert brain sections in TIF files to Interfile 
;
; CATEGORY:
;   Image processing
;
; CALLING SEQUENCE:
;   SECTIONS_TIF2IF 
;
; PROCEDURE:
;   Read coronal TIF files (B&W) and combine them to
;   create a zdim-slice coronal image in Interfile format.
;
; MODIFICATION HISTORY:
;   Tom Videen, Apr 2009
;-

PRO SECTIONS_TIF2IF
@IMAGE_UTILS.h

   on_error,2

   name = ''
   first = 0
   last  = 0
   name  = getenv('ROOT')
   first = fix(getenv('FIRST'))
   last  = fix(getenv('LAST'))
   xdim = 1280
   ydim = 1280
   zdim = last-first+1
   image =  intarr(xdim,ydim,zdim)
   dim = lonarr(4)
   dim[0] = xdim
   dim[1] = ydim
   dim[2] = zdim
   dim[3] = 1
   pdim = fltarr(4)
   pdim[0] = 0.0476
   pdim[1] = 0.0476
   pdim[2] = 0.5
   pdim[3] = 0

   ;; Gray scale TIF file

   IF (last LT 1000) THEN BEGIN
       inam = 'IMG_0'
   ENDIF ELSE BEGIN
       inam = 'IMG_'
   ENDELSE
   n = 0
   FOR i=first,last DO BEGIN
       file1 = strcompress(inam+string(i)+'.tif', /remove_all)
       img = read_tiff(file1)
       print, 'Reading '+file1, size(img)
       tmp = reform(img,xdim,ydim)
       image[*,*,n] = tmp
       n = n+1
    ENDFOR

   hname = strcompress(name+'.ifh', /remove_all)
   fname = strcompress(name+'.img', /remove_all)

   openw,lu1, hname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+hname
      RETURN
   ENDIF
   
   openw,lu2, fname, ERROR=err, APPEND=append, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+fname
      close, lu1
      free_lun, lu1
      free_lun, lu2
      RETURN
   ENDIF
   writeu,lu2,image
   close,lu2

   name = fname
   k = rstrpos(name,'/')
   IF (k GT 0) THEN name = strmid(name,k+1)
   maxval = MAX(image, MIN=minval)
   
   ;; File Identification

   printf, lu1, 'INTERFILE :='
   printf, lu1, 'version of keys    := 3.3'
   printf, lu1, 'date of keys       := 1992:01:01'
   printf, lu1, 'conversion program := sections_tif2if.pro'
   printf, lu1, 'program date       := 2009:04:02'
   printf, lu1, 'conversion date    := ' +systime(0)
   printf, lu1, ';'

   ;; General Data

   printf, lu1, 'GENERAL DATA :='
   printf, lu1, 'original institution := Washington University'
   printf, lu1, 'data starting block  := 0'
   printf, lu1, 'name of data file  := '+strtrim(name,2)
   printf, lu1, ';'

   ;; General Image Data

   printf, lu1, 'GENERAL IMAGE DATA :='
   printf, lu1, 'type of data := tomographic'
   printf, lu1, 'total number of images := '+strtrim(string(dim[2]),2)
   printf, lu1, 'imagedata byte order   := bigendian'
   printf, lu1, ';'

   ;; Study Data

   printf, lu1, 'number of dimensions   := 3'
   printf, lu1, 'number of slices := '+strtrim(string(dim[2]),2)
   printf, lu1, 'matrix size [1]    := '+strtrim(string(dim[0]),2)
   printf, lu1, 'matrix size [2]    := '+strtrim(string(dim[1]),2)
   printf, lu1, 'matrix size [3]    := '+strtrim(string(dim[2]),2)
   printf, lu1, 'matrix size [4]    := '+strtrim(string(dim[3]),2)
   printf, lu1, 'number format      := signed integer'
   printf, lu1, 'number of bytes per pixel  := 2'
   printf, lu1, 'scaling factor (mm/pixel) [1]  := '+strtrim(string(pdim[0]),2)
   printf, lu1, 'scaling factor (mm/pixel) [2]  := '+strtrim(string(pdim[1]),2)
   printf, lu1, 'scaling factor (mm/pixel) [3]  := '+strtrim(string(pdim[2]),2)
   printf, lu1, 'matrix initial element [1] := left'
   printf, lu1, 'matrix initial element [2] := anterior'
   printf, lu1, 'matrix initial element [3] := inferior'
   printf, lu1, 'maximum pixel count := '+strtrim(string(maxval),2)
   printf, lu1, 'minimum pixel count := '+strtrim(string(minval),2)
   printf, lu1, 'END OF INTERFILE :='
   close,lu1
   print, 'Created '+fname

   free_lun,lu1
   free_lun,lu2
   RETURN
END
