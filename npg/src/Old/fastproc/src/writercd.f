CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   WRITERCD.FTN
C
C  Author:       Tom O. Videen
C  Date:         12-Jun-87
C  Written For:  METPROC
C
C  Intent:
C     Write information to .RCD file.
C
C  History:
C     Typo in units of PETTCONV fixed by TOV 02-May-88.
C     Add version date to output 10-Oct-88 TOV.
C     Modified 24-Mar-89 by TOV to write scaling factor, EFACTOR.
C			Modified for UNIX 16-Jan-92 TOV:
C				INTDATE replaced by IDATE
C
C  Variables Passed:
C     N - scan number;
C     RCDFILE - logical unit assigned to .RCD file;
C     VERSION - date of current program version
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE WRITERCD(N,RCDFILE,EFACTOR,VERSION)
C
      INCLUDE 'metproc.inc'
C
      INTEGER*4    N,RCDFILE,DATE(3)
      REAL*4       EFACTOR
      CHARACTER*2  MM,DD,YY
      CHARACTER*8  PROCDATE
      CHARACTER*9  VERSION
C
8000  FORMAT(I2)
			CALL IDATE(DATE)
      WRITE(DD,8000) DATE(1)
      WRITE(MM,8000) DATE(2)
      WRITE(YY,8000) DATE(3) - ((DATE(3)/100)*100)
      PROCDATE = MM//'/'//DD//'/'//YY
C
      IF (N.EQ.1) WRITE(RCDFILE,8005) HEADER,INITIALS,PROCDATE,VERSION
8005  FORMAT(1X,A50,/1X,'Processed by ',A4,' on ',A8,' with ',
     2  'METPROC {',A9,'}')
C
      WRITE(RCDFILE,8010)
      WRITE(RCDFILE,8020)
      WRITE(RCDFILE,8030) N
      WRITE(RCDFILE,8010)
8010  FORMAT(1X,/,80('*'),/)
8020  FORMAT(21X,'METABOLIC INTERPRETATION INFORMATION',/)
8030  FORMAT(31X,'>>> SCAN ',I1,' <<<')
C
      WRITE(RCDFILE,8100) SCANTYPE(NTYPE)
      WRITE(RCDFILE,8110) SCANST
      WRITE(RCDFILE,8115) SCANLEN
      WRITE(RCDFILE,8117) FRAMELEN
8100  FORMAT(5X,'Scan Type = ',A)
8110  FORMAT(5X,'Scan Began',F6.0,' seconds after administration')
8115  FORMAT(5X,'Scan Time was',F7.0,' seconds')
8117  FORMAT(5X,'Frame Length was',F7.2,' seconds')
C
      IF (NTYPE.EQ.1) THEN
        WRITE(RCDFILE,8120) OXYCONT(1)
        WRITE(RCDFILE,8121) HEMATOCRIT
        WRITE(RCDFILE,8122)
        WRITE(RCDFILE,8123) PEAKTIME
        WRITE(RCDFILE,8124) FRACTIME
        WRITE(RCDFILE,8125) H2OFRAC
      END IF
8120  FORMAT(5X,'Blood Oxygen Fraction = ',F5.3)
8121  FORMAT(5X,'Blood Hematocrit = ',F5.1)
8122  FORMAT(5X,'Recirculating Water Parameters:')
8123  FORMAT(10X,'Peak Time = ',F6.1)
8124  FORMAT(10X,'Frac Time = ',F6.1)
8125  FORMAT(10X,'Fraction H2O= ',F6.3)
C
      WRITE(RCDFILE,8130) NUMPNTS
      WRITE(RCDFILE,8135)
      DO 100 I=1,NUMPNTS
        WRITE(RCDFILE,8140) TIME(I),ACTIVITY(I)
100   CONTINUE
      WRITE(RCDFILE,*)
8130  FORMAT(5X,'There were ',I3,' Points in the Blood Curve',/)
8135  FORMAT(10X,'TIME (SECS)',10X,'ACTIVITY (counts/(sec*ml blood))')
8140  FORMAT(10X,F8.1,20X,F9.0)
C
      WRITE(RCDFILE,8150) BPAIR
8150  FORMAT(5X,'Bank Pairs Used ',F7.2)
C
      IF (LNBLNK(PIENAME).GT.0) THEN
        WRITE(RCDFILE,8160) PIENAME( : LNBLNK(PIENAME))
      ELSE
        WRITE(RCDFILE,8160) 'NONE'
      END IF
      WRITE(RCDFILE,8162) PIESLOPE
      WRITE(RCDFILE,8164) PETTCONV(1)
      WRITE(RCDFILE,8166)
      WRITE(RCDFILE,8168) EFACTOR
      IF (EFACTOR.NE.1.0) WRITE(RCDFILE,8169)
8160  FORMAT(5X,'Pie File used = ',A)
8162  FORMAT(5X,'Pie Slope =',F12.4)
8164  FORMAT(5X,'PETT Conversion Factor =',F12.4)
8166  FORMAT(10X,'PETT Counts/(pixel*scan) to Well Counts/(ml*sec)')
8168  FORMAT(5X,'Efficiency Factor included in PETT Conversion ',
     &  'Factor = ',F)
8169  FORMAT(10X,'(not included in the Pie Slope)')
C
      IF (NTYPE.EQ.1) THEN
        WRITE(RCDFILE,8180) 'Water',AWATER(1)
        WRITE(RCDFILE,8190) 'Water',BWATER(1)
        WRITE(RCDFILE,8180) 'Oxygen',AOXYGEN(1)
        WRITE(RCDFILE,8190) 'Oxygen',BOXYGEN(1)
      ELSE IF (NTYPE.EQ.2 .OR. NTYPE.EQ.4) THEN
        WRITE(RCDFILE,8170)
        WRITE(RCDFILE,8172) LAMBDA
        WRITE(RCDFILE,8174) TAU
        WRITE(RCDFILE,8180) 'Flow',AFLOW(1)
        WRITE(RCDFILE,8190) 'Flow',BFLOW(1)
      ELSE IF (NTYPE.EQ.3) THEN
        WRITE(RCDFILE,8200) BVFACTOR(1)
      END IF
8170  FORMAT(5X,'Constants used in Blood Flow Algorithm:')
8172  FORMAT(10X,'Partition Coef = ',F6.3,' (ml/ml)')
8174  FORMAT(10X,'Decay Constant = ',F10.8,' (1/sec)')
8180  FORMAT(5X,'2nd Order Coefficient for ',A,'=',F20.8,)
8190  FORMAT(5X,'1st Order Coefficient for ',A,'=',F20.8,)
8200  FORMAT(5X,'Blood Volume Factor =',F20.8,)
C
      WRITE(RCDFILE,*)
      WRITE(RCDFILE,8210) TOTALCNTS
      WRITE(RCDFILE,8220) SCANCNTS
      WRITE(RCDFILE,8230)
8210  FORMAT(5X,'Integrated Blood Curve (from injection) =',F15.0)
8220  FORMAT(5X,'                   (from start of scan) =',F15.0)
8230  FORMAT(5X,'    (Non-Decay-Corrected Counts)')
C
      RETURN
      END
