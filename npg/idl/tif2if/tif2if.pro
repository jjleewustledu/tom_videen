; $Id: tif2if.pro,v 1.2 2005/03/01 21:53:40 tom Exp $
;+
; NAME:
;   TIF2IF
;
; PURPOSE:
;   Convert TIF to Interfile 
;
; CATEGORY:
;   Image processing
;
; CALLING SEQUENCE:
;   TIF2IF Infile Outfile
;
; INPUTS: passed from Environment variables
;   Infile:  input filename
;   Outfile: output filename
;
; OUTPUTS:
;   Outfile: filtered image file
;
; PROCEDURE:
;   Read an image in TIF format 
;   and write to output file in Interfile format
;
; MODIFICATION HISTORY:
;   Tom Videen, May, 2001
;-

PRO TIF2IF
@IMAGE_UTILS.h

   on_error,2
   root = getenv('ROOT')
   ext =  getenv('EXT')
   first = FIX(getenv('FIRST'))
   last = FIX(getenv('LAST'))
   outname = getenv('OUTNAME')
   xdim = getenv('XDIM')
   hname = strcompress(outname+'.ifh', /remove_all)
   fname = strcompress(outname+'.img', /remove_all)
   planes =  last-first+1
   image =  intarr(xdim,xdim,planes)
   dim = lonarr(4)
   dim[0] = xdim
   dim[1] = xdim
   dim[2] = planes
   dim[3] = 1
   pdim = fltarr(4)
   pdim[0] = 0.443276
   pdim[1] = 0.443276
   pdim[2] = 5.0
   pdim[3] = 0

   ;; Reorientation assumes film was scanned so that TIF images
   ;; appear in normal radiological orientation (with text correct)

   FOR j=first,last DO BEGIN
      infile =  strcompress(root+string(j)+ext, /remove_all)
      print, 'Reading '+infile
      img = read_tiff(infile)
      img = rotate(img,7)
      x = (size(img))[1] - 1
      FOR i=0,x DO image[i,*,j-first] = fix(img[i,*])
   ENDFOR

   ;; New ifh (lu1)

   openw,lu1, hname, ERROR=err, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+hname
      RETURN
   ENDIF
   
   ;; New img (lu2)

   openw,lu2, fname, ERROR=err, APPEND=append, /GET_LUN
   IF (err NE 0) THEN BEGIN
      msg = 'Cannot open '+fname
      close, lu1
      free_lun, lu1
      RETURN
   ENDIF
   writeu,lu2,image
   close,lu2
   free_lun,lu2

   name = fname
   k = rstrpos(name,'/')
   IF (k GT 0) THEN name = strmid(name,k+1)
   maxval = MAX(image, MIN=minval)
   
   ;; File Identification

   printf, lu1, 'INTERFILE :='
   printf, lu1, 'version of keys    := 3.3'
   printf, lu1, 'date of keys       := 1992:01:01'
   printf, lu1, 'conversion program := tif2if.pro'
   printf, lu1, 'program date       := 2001:05:23'
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
   printf, lu1, 'matrix initial element [1] := right'
   printf, lu1, 'matrix initial element [2] := posterior'
   printf, lu1, 'matrix initial element [3] := inferior'
   printf, lu1, 'maximum pixel count := '+strtrim(string(maxval),2)
   printf, lu1, 'minimum pixel count := '+strtrim(string(minval),2)
   printf, lu1, 'END OF INTERFILE :='
   close,lu1
   free_lun,lu1
   print, 'Converted to '+fname
   RETURN
END
