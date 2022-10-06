FUNCTION VIDI_DISPLAY_SCALE, image, type, i, xsize, ysize, sample, stype, mini, maxi
   COMMON VIDI
   
   IF (dsp[i].type EQ 1) THEN BEGIN ; Manual scaling
      mini = dsp[i].min
      maxi = dsp[i].max
   ENDIF ELSE BEGIN             ; Auto scaling
      IF (dsp[i].range EQ 0) THEN BEGIN
         mini = 0
         maxi = max(image)
      ENDIF ELSE IF (dsp[i].range EQ 1) THEN BEGIN
         mini = 0
         maxi = -min(image)
      ENDIF ELSE BEGIN
         tmp1 = max(image, min=tmp2)
         maxi = max([abs(tmp1), abs(tmp2)])
         mini = -maxi 
      ENDELSE
   ENDELSE
   dsp[i].amin = mini
   dsp[i].amax = maxi
   nc = dsp[i].nc
   IF (stype EQ 0) THEN BEGIN
      IF (dsp[i].range EQ 1) THEN $
         temp1 = bytscl(rebin(-(image), xsize, ysize, SAMPLE=sample), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE temp1 = bytscl(rebin(image, xsize, ysize, SAMPLE=sample), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDIF ELSE BEGIN 
      IF (dsp[i].range EQ 1) THEN $
         temp1 = bytscl(congrid(-(image), xsize, ysize, /INTERP), $
                        MIN=mini, MAX=maxi, TOP=(nc-1)) $
      ELSE temp1 = bytscl(congrid(image, xsize, ysize, /INTERP), $
                          MIN=mini, MAX=maxi, TOP=(nc-1))
   ENDELSE
   RETURN, temp1
END
      
PRO VIDI_DISPLAY, uv, SAMPLE=sample
   ;; Scale and display image i
   ;; uv = the info structure for this display
   ;; sample = use nearest neighbor sampling for rebin
   COMMON IMAGE_UTILS
   COMMON VIDI
   
   IF (NOT (keyword_set(sample))) THEN SAMPLE=0
   nodata = 1

   MAI = [10,11,11,12,12,13,13,14,14,15,15,16,17,18,20,22,23,24,25,26,27,28,29,30, $
          31,32,33,34,35,36,37,38,39,40,41,42,42,43,44,45,46,47,48,49]

   FOR i= 0, 1 DO BEGIN
      proc = uv.img[i].proc
      IF (dsp[i].set) THEN BEGIN 
         IF (uv.img[i].data NE ptr_new()) THEN BEGIN
            nodata = 0
            slice = uv.img[i].p - uv.img[i].fp
            frame = uv.img[i].f - uv.img[i].ff
            CASE orient OF
               0: BEGIN
                  x0 = 0
                  y0 = 1
                  image = reform((*uv.img[i].data)[*,*,slice,frame])
               END
               1: BEGIN
                  x0 = 0
                  y0 = 2
                  image = reform((*uv.img[i].data)[*,slice,*,frame])
               END
               2: BEGIN
                  x0 = 1
                  y0 = 2
                  image = reform((*uv.img[i].data)[slice,*,*,frame])
               END
            ENDCASE
            zoom = dsp[i].zoom
            xsize = uv.img[i].dim[x0] * zoom
            ysize = uv.img[i].dim[y0] * zoom
            stype = 1
            fact = 1
            IF (uv.img[i].pdim[y0] GT uv.img[i].pdim[x0]) THEN BEGIN 
               fact = (uv.img[i].pdim[y0])/(uv.img[i].pdim[x0])
               ysize = ROUND(fact * uv.img[i].dim[y0] * zoom)
            ENDIF ELSE IF (uv.img[i].pdim[x0] GT uv.img[i].pdim[y0]) THEN BEGIN 
               fact = (uv.img[i].pdim[x0])/(uv.img[i].pdim[y0])
               xsize = ROUND(fact * uv.img[i].dim[x0] * zoom)
            ENDIF ELSE stype = 0

            cbf = 0
            cbv = 0
            IF (proc EQ 3 OR proc EQ 4 or proc EQ 6) THEN BEGIN
               CASE orient OF
                  0: cbf = reform((*uv.cbf[i].data)[*,*,slice,frame])
                  1: cbf = reform((*uv.cbf[i].data)[*,slice,*,frame])
                  2: cbf = reform((*uv.cbf[i].data)[slice,*,*,frame])
               ENDCASE
            ENDIF
            IF (proc EQ 3 OR proc EQ 4) THEN BEGIN
               CASE orient OF
                  0: cbv = reform((*uv.cbv[i].data)[*,*,slice,frame])
                  1: cbv = reform((*uv.cbv[i].data)[*,slice,*,frame])
                  2: cbv = reform((*uv.cbv[i].data)[slice,*,*,frame])
               ENDCASE
            ENDIF
            scantime = uv.img[i].len * 1000
            IF (proc EQ 3 OR proc EQ 4) THEN BEGIN
               pix = where (image LE 0, count)
               IF (count GT 0) THEN BEGIN
                  cbf(pix) = 0
                  cbv(pix) = 0
               ENDIF
            ENDIF
            temp1 = VOI_PROCESS (proc, image, uv.img[i].mtype, uv.img[i].mcal, $
                                 cbf, uv.cbf[i].mcal, cbv, uv.cbv[i].mcal, scantime)
            
            temp1 = VIDI_DISPLAY_SCALE (temp1, uv.img[i].itype, i, xsize, ysize, sample, stype, mini, maxi)
            IF (atlas[4] GT 0) THEN BEGIN ; Color Bar
               nc = dsp[i].nc
               wid = ysize/20
               len = ysize*9/10
               off = ysize*1/20
               temp = bytscl(indgen(len), TOP=(nc-1))
               FOR j=1, wid DO FOR k=1, len DO temp1[xsize-j, ysize-k-off] = temp[k-1]
            ENDIF
            widget_control, uv.wid.win[i], draw_xsize=xsize, draw_ysize=ysize
            wset, dsp[i].id
            temp1 = temp1 + dsp[i].bot
            tv, temp1
            IF (atlas[3] AND NOT atlas[i]) THEN BEGIN ; Slice Label with no Atlas
               IF ((rstrpos(uv.img[i].fname,'MaiThalamus') GT 0 OR $
                    rstrpos(uv.img[i].fname,'MaiMicrosections') GT 0) AND orient EQ 1) THEN BEGIN
                  str = strcompress(string(MAI[slice]),/remove_all)
                  xyouts,zoom*20,zoom*280,str,charsize=2*zoom,charthick=2,/device,color=130
               ENDIF ELSE BEGIN
                  str = strcompress(string(slice+1),/remove_all)
                  xyouts,zoom*5,zoom*5,str,charsize=zoom,charthick=2,/device,color=label
               ENDELSE
            ENDIF

            VIDI_ATLAS, uv, i, temp1
            IF (atlas[4] GT 0) THEN BEGIN ; Color Bar
               IF (mini EQ 0) THEN BEGIN
                  minstr = '0'
               ENDIF ELSE IF (abs(mini) GE 10) THEN BEGIN
                  minstr = string(round(mini))
               ENDIF ELSE IF (abs(mini) LT 10 AND abs(mini) GE 0.1) THEN BEGIN
                  minstr = string(format='(F4.1)',mini)
               ENDIF ELSE IF (abs(mini) LT 0.1 AND abs(mini) GE 0.01) THEN BEGIN
                  minstr = string(format='(F5.2)',mini)
               ENDIF ELSE IF (abs(mini) LT 0.01 AND abs(mini) GE 0.001) THEN BEGIN
                  minstr = string(format='(F6.3)',mini)
               ENDIF ELSE BEGIN
                  minstr = string(mini)
               ENDELSE

               IF (maxi EQ 0) THEN BEGIN
                  maxstr = '0'
               ENDIF ELSE IF (abs(maxi) GE 10) THEN BEGIN
                  maxstr = string(round(maxi))
               ENDIF ELSE IF (abs(maxi) LT 10 AND abs(maxi) GE 0.1) THEN BEGIN
                  maxstr = string(format='(F3.1)',maxi)
               ENDIF ELSE IF (abs(maxi) LT 0.1 AND abs(maxi) GE 0.01) THEN BEGIN
                  maxstr = string(format='(F4.2)',maxi)
               ENDIF ELSE IF (abs(maxi) LT 0.01 AND abs(maxi) GE 0.001) THEN BEGIN
                  maxstr = string(format='(F5.3)',maxi)
               ENDIF ELSE BEGIN
                  maxstr = string(maxi)
               ENDELSE

               xloc = xsize-wid-2
               IF (ysize LT 200) THEN BEGIN
                  csiz=1
                  cthk=1
                  yoff=ysize-off*2
               ENDIF ELSE IF (ysize LT 400) THEN BEGIN
                  csiz=2
                  cthk=2
                  yoff=ysize-off*2
               ENDIF ELSE BEGIN
                  csiz=2
                  cthk=2
                  yoff=ysize-off*3/2
               ENDELSE
               wset, dsp[i].id
               xyouts,xloc, off, minstr,alignment=1.0,charsize=csiz,charthick=cthk,color=label,/device
               xyouts,xloc, yoff, maxstr,alignment=1.0,charsize=csiz,charthick=cthk,color=label,/device
            ENDIF
            widget_control,uv.wid.max[i],set_value=dsp[i].amax
         ENDIF 
      ENDIF
   ENDFOR
   
   IF (dsp[2].set) THEN BEGIN
      IF (uv.img[0].data NE ptr_new() AND uv.img[1].data NE ptr_new()) THEN BEGIN
         IF ((uv.img[0].dim[0] NE uv.img[1].dim[0]) OR $
             (uv.img[0].dim[1] NE uv.img[1].dim[1]) OR $
             (uv.img[0].dim[2] NE uv.img[1].dim[2])) THEN BEGIN
            widget_control, uv.wid.msg, set_value='Cannot display composite of different sized images'
         ENDIF ELSE BEGIN 
            nodata = 0
            i = 0
            slice = uv.img[i].p - uv.img[i].fp
            frame = uv.img[i].f - uv.img[i].ff
            CASE orient OF
               0: BEGIN
                  x0 = 0
                  y0 = 1
                  image = reform((*uv.img[i].data)[*,*,slice,frame])
               END
               1: BEGIN
                  x0 = 0
                  y0 = 2
                  image = reform((*uv.img[i].data)[*,slice,*,frame])
               END
               2: BEGIN
                  x0 = 1
                  y0 = 2
                  image = reform((*uv.img[i].data)[slice,*,*,frame])
               END
            ENDCASE
            zoom = dsp[i].zoom
            xsize = uv.img[i].dim[x0] * zoom
            ysize = uv.img[i].dim[y0] * zoom
            stype = 1

            IF (uv.img[i].pdim[y0] GT uv.img[i].pdim[x0]) THEN BEGIN 
               fact = (uv.img[i].pdim[y0])/(uv.img[i].pdim[x0])
               ysize = ROUND(fact * uv.img[i].dim[y0] * zoom)
            ENDIF ELSE IF (uv.img[i].pdim[x0] GT uv.img[i].pdim[y0]) THEN BEGIN 
               fact = (uv.img[i].pdim[x0])/(uv.img[i].pdim[y0])
               xsize = ROUND(fact * uv.img[i].dim[x0] * zoom)
            ENDIF ELSE stype = 0

            widget_control, uv.wid.win[2], draw_xsize=xsize, draw_ysize=ysize
            temp1 = VIDI_DISPLAY_SCALE (image, uv.img[i].itype, i, xsize, ysize, sample, stype, mini, maxi)
            IF (atlas[4] GT 0) THEN BEGIN ; Color Bar
               nc = dsp[i].nc
               wid = ysize/20
               len = ysize*9/10
               off = ysize*1/20
               temp = bytscl(indgen(len), TOP=(nc-1))
               FOR j=1, wid DO FOR k=1, len DO temp1[j-1, ysize-k-off] = temp[k-1]
            ENDIF

            i = 1
            CASE orient OF
               0:  image = reform((*uv.img[i].data)[*,*,slice,frame])
               1:  image = reform((*uv.img[i].data)[*,slice,*,frame])
               2:  image = reform((*uv.img[i].data)[slice,*,*,frame])
            ENDCASE
            temp2 = VIDI_DISPLAY_SCALE (image, uv.img[i].itype, i, xsize, ysize, sample, stype, mini, maxi)
            IF (atlas[4] GT 0) THEN BEGIN ; Color Bar
               nc = dsp[i].nc
               wid = ysize/20
               len = ysize*9/10
               off = ysize*1/20
               temp = bytscl(indgen(len), TOP=(nc-1))
               FOR j=1, wid DO FOR k=1, len DO temp2[xsize-j, ysize-k-off] = temp[k-1]
            ENDIF
            
            IF (dsp[i].range EQ 2) THEN BEGIN
               uthresh =  (0.5 + (0.005 * dsp[0].cut)) * dsp[1].nc
               lthresh =  (0.5 - (0.005 * dsp[0].cut)) * dsp[1].nc
               idx1 = where((temp2 LT uthresh) AND (temp2 GT lthresh), count1)
               idx2 = where((temp2 GE uthresh) OR (temp2 LE lthresh), count2)
            ENDIF ELSE BEGIN
               thresh =  0.01 * dsp[0].cut * dsp[1].nc
               idx1 = where(temp2 LT thresh, count1)
               idx2 = where(temp2 GE thresh, count2)
            ENDELSE
            IF (count1 GT 0) THEN temp2[idx1] = 0 ; temp = temp1
            IF (count2 GT 0) THEN temp1[idx2] = dsp[0].nc ; temp = nc + temp2
            temp = temp1 + temp2 ; combine
            wset, dsp[2].id
            tv, temp
            IF (atlas[3] AND NOT atlas[2]) THEN BEGIN ; Slice Label with no Atlas
               IF ((rstrpos(uv.img[i].fname,'MaiThalamus') GT 0 OR $
                    rstrpos(uv.img[i].fname,'MaiMicrosections') GT 0) AND orient EQ 1) THEN BEGIN
                  str = strcompress(string(MAI[slice]),/remove_all)
                  xyouts,zoom*20,zoom*280,str,charsize=2*zoom,charthick=2,/device,color=130
               ENDIF ELSE BEGIN
                  str = strcompress(string(slice+1),/remove_all)
                  xyouts,zoom*5,zoom*5,str,charsize=zoom,charthick=2,/device,color=label
               ENDELSE
            ENDIF


            VIDI_ATLAS, uv, 2, temp
            IF (atlas[4] GT 0) THEN BEGIN ; Color Bar
               IF (mini EQ 0) THEN BEGIN
                  minstr = '0'
               ENDIF ELSE IF (abs(mini) GE 10) THEN BEGIN
                  minstr = string(round(mini))
               ENDIF ELSE IF (abs(mini) LT 10 AND abs(mini) GE 0.1) THEN BEGIN
                  minstr = string(format='(F3.1)',mini)
               ENDIF ELSE IF (abs(mini) LT 0.1 AND abs(mini) GE 0.01) THEN BEGIN
                  minstr = string(format='(F4.2)',mini)
               ENDIF ELSE IF (abs(mini) LT 0.01 AND abs(mini) GE 0.001) THEN BEGIN
                  minstr = string(format='(F5.3)',mini)
               ENDIF ELSE BEGIN
                  minstr = string(mini)
               ENDELSE

               IF (maxi EQ 0) THEN BEGIN
                  maxstr = '0'
               ENDIF ELSE IF (abs(maxi) GE 10) THEN BEGIN
                  maxstr = string(round(maxi))
               ENDIF ELSE IF (abs(maxi) LT 10 AND abs(maxi) GE 0.1) THEN BEGIN
                  maxstr = string(format='(F3.1)',maxi)
               ENDIF ELSE IF (abs(maxi) LT 0.1 AND abs(maxi) GE 0.01) THEN BEGIN
                  maxstr = string(format='(F4.2)',maxi)
               ENDIF ELSE IF (abs(maxi) LT 0.01 AND abs(maxi) GE 0.001) THEN BEGIN
                  maxstr = string(format='(F5.3)',maxi)
               ENDIF ELSE BEGIN
                  maxstr = string(maxi)
               ENDELSE

               xloc = xsize-wid-2
               IF (ysize LT 200) THEN BEGIN
                  csiz=1
                  cthk=1
                  yoff=ysize-off*2
               ENDIF ELSE IF (ysize LT 400) THEN BEGIN
                  csiz=2
                  cthk=2
                  yoff=ysize-off*2
               ENDIF ELSE BEGIN
                  csiz=2
                  cthk=2
                  yoff=ysize-off*3/2
               ENDELSE
               wset, dsp[2].id
               xyouts,xloc, off, minstr,alignment=1.0,charsize=csiz,charthick=cthk,color=label,/device
               xyouts,xloc, yoff, maxstr,alignment=1.0,charsize=csiz,charthick=cthk,color=label,/device
            ENDIF
         ENDELSE
      ENDIF 
   ENDIF
   IF (nodata EQ 1) THEN widget_control, uv.wid.msg, set_value='No data'
END
