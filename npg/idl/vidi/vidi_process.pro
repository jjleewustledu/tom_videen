FUNCTION VIDI_PROCESS, pixels, img, cbf, cbv, log

   proc = img.proc
   n = n_elements(pixels)
   IF (n LE 0) THEN RETURN, 'Voxels = 0'
   vol = n * img.pdim[0] * img.pdim[1] * img.pdim[2] * 0.001
   
   val = (*img.data)[pixels]
   IF (n LE 2) THEN BEGIN
      ival = val
      sd = 0
   ENDIF ELSE ival = (moment(val,sdev=sd))[0]
   vmax = max(val,min=vmin)
   scantime = img.len * 1000
   CASE proc OF
      0: BEGIN
         IF (abs(ival) GT 1) THEN BEGIN
            str1 = string(format='("Mean = ",F17.3,"  (total cnts)",10X,"Min =",F14.3,"   Max=",F14.3)',ival, vmin, vmax)
		    str2 = string(format='(/"SD     = ",F17.3,/,"Voxels = ",I,10X,"Volume =",F10.3," cc")', sd, n, vol)
            log = string(format='("cnts ",F14.3, I8, F14.3)', ival, n, sd)
         ENDIF ELSE IF (abs(ival) GT 0.001) THEN BEGIN
            str1 = string(format='("Mean = ",F17.7,"  (total cnts)",10X,"Min =",F14.7,"   Max=",F14.7)',ival, vmin, vmax)
		    str2 = string(format='(/"SD     = ",F17.7,/,"Voxels = ",I,10X,"Volume =",F10.3," cc")', sd, n, vol)
            log = string(format='("cnts ",F14.7, I8, F14.7)', ival, n, sd)
         ENDIF ELSE BEGIN
            str1 = string(format='("Mean = ",F17.12,"  (total cnts)",10X,"Min =",F14.10,"   Max=",F14.10)',ival, vmin, vmax)
		    str2 = string(format='(/"SD     = ",F17.12,/,"Voxels = ",I,10X,"Volume =",F10.3," cc")', sd, n, vol)
            log = string(format='("cnts ",F14.9, I8, F14.9)', ival, n, sd)
         ENDELSE
         msg = string(format='(A,/,A)',str1, str2)
      ENDCASE
      1: BEGIN
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal)
         msg = string(format='("CBF  = ",F12.3,"  (ml/100g/min)",/,"Voxels = ",I)', val,n)
         log = string(format='("cbf  ",F12.3, I8)', val,n)
      ENDCASE
      2: BEGIN
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal)
         msg = string(format='("CBV  = ",F12.3,"  (ml/100g)",/,"Voxels = ",I)', val,n)
         log = string(format='("cbv  ",F12.3, I8)', val,n)
      ENDCASE
      3: BEGIN
         val = (*cbf.data)[pixels]
         fval = (moment(val))[0]
         val = (*cbv.data)[pixels]
         vval = (moment(val))[0]
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal, fval, cbf.mcal, vval, cbv.mcal, scantime)
         msg = string(format='("OEF  = ",F12.3,/,"Voxels = ",I)', val,n)
         log = string(format='("oef  ",F12.3, I8)', val,n)
      ENDCASE
      4: BEGIN
         val = (*cbf.data)[pixels]
         fval = (moment(val))[0]
         val = (*cbv.data)[pixels]
         vval = (moment(val))[0]
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal, fval, cbf.mcal, vval, cbv.mcal, scantime)
         msg = string(format='("CMRO2 = ",F12.3,"  (ml/100g/min)",/,"Voxels = ",I)', val,n)
         log = string(format='("cmro2",F12.3, I8)', val,n)
      ENDCASE
      5: BEGIN
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal)
         msg = string(format='("Mean  = ",F15.6,/,"Voxels = ",I)', val,n)  
         log = string(format='("cmrglu",F12.3, I8)', val,n)
      ENDCASE
      6: BEGIN
         val = (*cbf.data)[pixels]
         fval = (moment(val))[0]
         val = VOI_PROCESS(proc, ival, img.mtype, img.mcal, fval, cbf.mcal, vval, cbv.mcal, scantime)
         msg = string(format='("MTT ",F12.3,"  (sec)",/,"Voxels = ",I)', val,n)
         log = string(format='("mtt  ",F12.3, I8)', val,n)
      ENDCASE
   ENDCASE
   RETURN, msg
END   
