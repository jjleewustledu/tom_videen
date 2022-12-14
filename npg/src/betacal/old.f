      PROGRAM BETACAL

C     AZS 28-JUL-90
C     CALCULATES BETA PROBE TO WELL COUNTER CONVERSION FACTOR
C     USES SUBROUTINE POLFIT(X,Y,NPTS,A,ARRAY,NTERM,CHISQR)

C     Modified 12/05/91 to read floating T(I)
C     29-Apr-93 port to SUN
C               OUTPUT LISTED ON stdout
C     31-Oct-94 final conversion factor output format F6.2 => F10.4
C     25-Feb-2002 skip first 2 points for new PC

      CHARACTER*20 STR
      CHARACTER*256 FNAME
      CHARACTER*1 ASCII
      PARAMETER (NBINMAX = 1024)
      PARAMETER (NTERM = 2)
      REAL*4	T(NBINMAX), Y(NBINMAX)
      REAL*4	A(NTERM), ARRAY(NTERM, NTERM)
      INTEGER*4 BLDCRV
      INTEGER*4 USERIN, USEROUT

      COMMON /USRIO/    USERIN,USEROUT

      DATA BLDCRV /8/
      DATA USERIN, USEROUT /5,6/

100   TYPE "(' ENTER BETA PROBE CALIBRATION FILENAME (.crt or .crv)')"
      ACCEPT "(A256)", FNAME
      L = lnblnk(FNAME)
      IF (L.EQ.0) STOP 'No File Entered'
      OPEN (BLDCRV, FILE=FNAME, STATUS='OLD', ERR=100)
      TYPE "(' ENTER SEC/BIN IN CRV FILE')"
      ACCEPT *,BINTIME

      WRITE (*,"(A80)") FNAME(1:80)
      READ  (BLDCRV,"(A20)") STR
      WRITE (*,"(A20)") STR
      READ  (BLDCRV,*) NBIN
      IF (NBIN.GT.NBINMAX) STOP 'ERROR: Too many time bins'

      TYPE "(' Skip first 2 points (y/n)')"
      ACCEPT "(A1)", ASCII
      IF (ASCII.EQ.'Y'.or.ASCII.EQ.'y') THEN
         READ (BLDCRV,*) S, J
         READ (BLDCRV,*) S, J
         NBIN = NBIN-2
      ENDIF

      DO 110 I=1,NBIN
         READ (BLDCRV,*) S, J
         M = NINT(S)
         Y(I) = ALOG(FLOAT(J))
         T(I) = FLOAT(S)*BINTIME
110   CONTINUE
      CLOSE (BLDCRV)

      CALL POLFIT(T, Y, NBIN, A, ARRAY, NTERM, CHISQR)
      BETA = EXP(A(1))/BINTIME
      WRITE (*,8102) A, CHISQR
      WRITE (*,8103) BETA
      WRITE (*,8104) -ALOG(2.)/A(2)
      TYPE "(' ENTER RATIONUCLIDE (O/C)')"
      ACCEPT "(A1)", ASCII
      IF (ASCII.EQ.'O'.or.ASCII.EQ.'o') THALF = 123.
      IF (ASCII.EQ.'C'.or.ASCII.EQ.'c') THALF = 1218.
      TYPE "(' Halflife set to', F10.0,' sec')", THALF
      TYPE "(' ENTER WELL COUNTER COUNTING PERIOD IN SEC')"
      ACCEPT *,BINLEN
      CALL WELLCAL(THALF, BINLEN, WELL)
      WRITE (*,8106) WELL/BETA

8102  FORMAT(' A(1), A(2), CHISQR', 3F10.4)
8103  FORMAT(' COUNTS/SEC EXTRAPOLATED TO TIME 0', F9.0)
8104  FORMAT(' CALCULATED T1/2 IN SECONDS', F8.2)
8106  FORMAT(' BETA PROBE TO WELL COUNTER CONVERSION FACTOR', F10.4)
      END

      SUBROUTINE WELLCAL(THALF, BINLEN, RATE)
C     input THALF = half life of tracer in sec
C     input BINLEN = counting period of well counter (usually 10 sec)
C     returns RATE = COUNTS/SEC*GRAM
C     READS ASCII FILE ON LU 0 WITH WELL COUNTER CALIBRATION DATA
C     dry, wet, min, sec, counts/bin
C     EACH DUPLICATE RUN MUST APPEAR ON A SEPARATE LINE
C     DATA IS READ UNTIL FILE END OR FIRST NONNUMERIC
C     WELLCAL AVERAGES OVER DUPLICATES
C     CALCULATION SUMMARY IS SENT TO LU6

      PARAMETER (NMAX = 20)
      REAL*4    DRY(NMAX), WET(NMAX), R(NMAX)
      INTEGER*4 MINUTE(NMAX), SEC(NMAX), COUNTS(NMAX)

      AKT = ALOG(2.)*BINLEN/THALF
      BINFAC = AKT/(1.-EXP(-AKT))
      BINFAC = BINFAC/BINLEN
      CALL BETAGET(NMAX, DRY, WET, MINUTE, SEC, COUNTS, N)
      S = 0.
      V = 0.
      DO 200 I=1,N
         F = 2.**(FLOAT(SEC(I)+60*MINUTE(I))/THALF)
         R(I) = BINFAC*F*COUNTS(I)/(WET(I)-DRY(I))
         S = S+R(I)
         V = V+R(I)**2
200   CONTINUE
      S = S/FLOAT(N)
      V = V-FLOAT(N)*S**2
      V = V/FLOAT(N-1)
      WRITE (*,8113) S, SQRT(V), SQRT(V)/S
      RATE = S
      WRITE (*,8114)
      DO 210 I=1,N
         WRITE (*,8115) I, DRY(I), WET(I), MINUTE(I), SEC(I), COUNTS(I), R(I)
210   CONTINUE
      WRITE (*,8113) S, SQRT(V), SQRT(V)/S
8113  FORMAT (' mean cts/(g*sec)', F10.0,'  s.d.', F10.0,'  s.d./mean', F10.4)
8114  FORMAT(' LINE     DRY   WET  MIN  SEC  COUNTS cts/(g*sec)')
8115  FORMAT(I5, F8.3, F6.3, 2I5, I8, F12.0)
      RETURN
      END

      SUBROUTINE BETAGET(NMAX, DRY, WET, MINUTE, SEC, COUNTS, N)

      REAL*4    DRY(NMAX), WET(NMAX)
      INTEGER*4 MINUTE(NMAX), SEC(NMAX), COUNTS(NMAX)
      CHARACTER*20 FNAME
      CHARACTER*80 STRING
      INTEGER*4 WELLFILE

      DATA WELLFILE /8/

      I = 0
      N = 0
      TYPE "(' ENTER WELL COUNTER DATA FILE NAME')"
      FNAME = ' '
      ACCEPT "(A20)", FNAME
      OPEN (WELLFILE, FILE=FNAME, STATUS='OLD', ERR=350)
300   I = I+1
      READ (WELLFILE,*,END=310, ERR=310) DRY(I), WET(I), MINUTE(I), SEC(I), COUNTS(I)
      N = I
      IF (N.LT.NMAX) GOTO 300
310   CLOSE (WELLFILE)
320   TYPE "(' LINE     DRY   WET  MIN  SEC  COUNTS')"
      DO 330 I=1,N
         WRITE (*,8200) I, DRY(I), WET(I), MINUTE(I), SEC(I), COUNTS(I)
330   CONTINUE
      TYPE "(' EDIT A DATA LINE OR')"
      TYPE "(' DELETE A LINE BY ENTERING THE LINE NUMBER')"
      TYPE "(' ADD DATA BY ENTERING LINE ', I2)", N+1
      TYPE "(/' ENTER line, dry, wet, minutes, seconds, counts')"
      STRING = ' '
      ACCEPT "(A80)", STRING
      L = lnblnk(STRING)
      IF (L.LE.0) GOTO 360
      DECODE(L, "(I)", STRING(1:L))I
      IF (I.GT.N+1) GOTO 320
      IF (I.EQ.N+1)N = I
      DECODE(L, 8200, STRING(1:L)) I, DRY(I), WET(I), MINUTE(I), SEC(I), COUNTS(I)
8200  FORMAT(I5, F8.3, F6.3,2I5, I8)
      IF (COUNTS(I).EQ.0)THEN
      DO 340 J = I+1,N
         DRY(J-1) = DRY(J)
         WET(J-1) = WET(J)
         MINUTE(J-1) = MINUTE(J)
         SEC(J-1) = SEC(J)
         COUNTS(J-1) = COUNTS(J)
340   CONTINUE
      N = N-1
      ENDIF
      GOTO 320

350   TYPE "(' FILE NOT FOUND')"
      GOTO 320

360   TYPE "(' ENTER WELL COUNTER FILENAME TO BE SAVED (.wel)')"
      FNAME = ' '
      ACCEPT "(A20)", FNAME
      L = lnblnk(FNAME)
      IF (L.LE.0) GOTO 380
      OPEN (8, FILE=FNAME, ERR=380)
      DO 370 I=1,N
         WRITE (8,8210) DRY(I), WET(I), MINUTE(I), SEC(I), COUNTS(I)
370   CONTINUE
8210  FORMAT(2F7.3, 2I5, I8)
      CLOSE (8, STATUS='KEEP')
      TYPE "(' SAVE WELL COUNTER DATA FILE ', A20)", FNAME
      GOTO 390
380   TYPE "(' ERROR OPENING', A20)", FNAME
390   RETURN
      END
