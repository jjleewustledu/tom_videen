FUNCTION VOI_SIMPLE, img, proc, mcal
   CASE proc OF
      1: img = img * (mcal[1]*img + mcal[2])
      2: img = mcal[3]*img
      5: img = 0.001*img
      6: img = mcal[3]*img
      7: img = mcal[8] * img * (mcal[1]*img + mcal[2])
      ELSE:
   ENDCASE
   RETURN, img
END 

FUNCTION VOI_OXYGEN, img, cbf, cbv, proc, mcal
   slvratio = 0.85              ; ratio of cerebral small vessel to large vessel hematocrit
   volfactr = 0.835             ; (postcapillary vol + 0.5*capillary vol)/total blood volume
   brainden = 1.05              ; density of brain (g/ml)
   c1 = slvratio * mcal[9] / 100.0
   c2 = volfactr * c1
   c3 = mcal[0] / brainden
   a1 = c3 * img
   a2 = cbf * (mcal[6]*cbf + mcal[7])
   a3 = c1 * cbv
   num = a1-a2-a3
   a4 = cbf * (mcal[4]*cbf + mcal[5])
   a5 = c2 * cbv
   den = a4-a5
   n = where(den, count)
   img[*] = 0
   CASE proc OF
      3: BEGIN 
         IF (count GT 0) THEN img[n] = num[n]/den[n]
         n = where(img GT 1, count)
         IF (count GT 0) THEN img[n] = 1.
      ENDCASE 
      4: IF (count GT 0) THEN img[n] = cbf[n] * mcal[8] * num[n]/den[n]
      ELSE:
   ENDCASE
   RETURN, img
END 

FUNCTION VOI_SSOXYGEN, img, cbf, cbv, proc, mcal, scantime
   slvratio = 0.85              ; ratio of cerebral small vessel to large vessel hematocrit
   volfactr = 0.835             ; (postcapillary vol + 0.5*capillary vol)/total blood volume
   brainden = 1.05              ; density of brain (g/ml)
   pc = 1.0                     ; partition coefficient for water
   f = brainden * cbf / (60. * 100.)
   v = brainden * cbv / 100.
   kd = 0.005668
   c1 = f / (kd + f/pc)
   watercont = mcal[8] * mcal[4]
   a1 = img * mcal[0] / scantime
   a2 = c1 * watercont;
   a3 = v * slvratio * mcal[8]
   a4 = c1 * mcal[8]
   a5 = v * slvratio * volfactr * mcal[8]
   num = a1-a2-a3
   den = a4-a5
   n = where(den, count)
   img[*] = 0
   CASE proc OF
      3: BEGIN 
         IF (count GT 0) THEN img[n] = num[n]/den[n]
         n = where(img GT 1, count)
         IF (count GT 0) THEN img[n] = 1.
      ENDCASE 
      4: IF (count GT 0) THEN img[n] = cbf[n] * mcal[8] * num[n]/den[n]
      ELSE:
   ENDCASE
   RETURN, img
END 
       
FUNCTION VOI_PROCESS, proc, img, mtype, mcal, cbf, cbfcal, cbv, cbvcal, scantime
   
   IF (proc EQ 3 OR proc EQ 4) THEN BEGIN
      cbfc = VOI_SIMPLE (cbf, 1, cbfcal)
      cbvc = VOI_SIMPLE (cbv, 2, cbvcal)
      CASE mtype OF
         1: val = VOI_OXYGEN (img, cbfc, cbvc, proc, mcal)
         7: val = VOI_SSOXYGEN (img, cbfc, cbvc, proc, mcal, scantime)
         ELSE: val = -1
      ENDCASE
   ENDIF ELSE IF (proc EQ 6) THEN BEGIN
      cbvc = VOI_SIMPLE (img, 2, mcal)
      cbfc = VOI_SIMPLE (cbf, 1, cbfcal)
      n = where(cbfc GT 0.0, count)
      img[*] = 0
      IF (count GT 0) THEN img[n] = (60. * cbvc[n]) / cbfc[n]
      RETURN, img
   ENDIF ELSE val = VOI_SIMPLE (img, proc, mcal)
   RETURN, val
END
