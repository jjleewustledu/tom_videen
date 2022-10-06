CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      BETADTA.FTN
C
C  Author:       Avi Snyder
C  Date:         05-Sep-90
C  Written For:  Automated blood sampler data files
C
C  History:
C     Modified 20-Dec-90 by TOV to change all ACCEPT statements
C        to READ (IN).
C     Modified 09-Apr-91 AZS
C        Time of first bin defined as 0 in kernel calculation.
C     Modified 10-Apr-91 AZS
C        Calculates blood-bankpair delay by two methods and
C        optionally shifts blood curve into alignment.
C     Modified 20-May-91 AZS to extend measured data backwards
C        20 seconds in preparation for deconvolution.
C     Modified 08-Jul-91 AZS to call GETTKE for decovolution
C         parameters and implement extraction fraction shifting.
C     Modified 12-Feb-92 by Tom Yang to port to Sun Unix.
C	  Changed the logical units numbers to 5 and 6. Changed
C	  all character format. Modified the open statements.
C	  Changed the data format for hexidecimal numbers.
C     Modified 16-Mar-92 AZS to properly read non-1 sec bins.
C
C  Intent:
C     Version of CRVDCNV for generating .DTA files
C
C  Logical Units:
C     5 = User input (IN)
C     6 = User output and prompts
C     8 = Input .CRV file
C
C  Uses Subroutines contained in:
C     DECONV
C     FFTPE
C     GETTKE
C     POLFIT
C
C  Internal Subroutines:
C     CRVCOPY
C     EXPAND
C     CRVCOR
C     CRVMIN
C     MAXCRV
C     CRVDIF
C     CRVSMO
C     CRVPEAK
C     CRVTOFF
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      PROGRAM BETADTA
C
      INTEGER*4		IN
C
      CHARACTER*20	STR
      CHARACTER*24	FILSPC, ROOSPC
      CHARACTER*4	SCANID
      CHARACTER*1	ASCII
C
      LOGICAL*4		LBANK/.TRUE./
      LOGICAL*4		LDECOR/.TRUE./
      LOGICAL*4		LEXIST
C
      PARAMETER (NBINMAX = 4096)
C
      REAL*4		B(NBINMAX), Y(NBINMAX)
      REAL*4		H(NBINMAX), G(NBINMAX), X(NBINMAX)
      REAL*4		S(NBINMAX), D(NBINMAX)
      REAL*4		A(2 * NBINMAX + 4)
      REAL*4		AP(0:2), AA(3,3), AT(50)
C
C     A:	SCRATCH ARRAY
C     H:	BLOOD SUCKER IMPULSE RESPONSE FUNCTION
C     G:	RAW BLOOD CURVE
C     B:	RAW BANK PAIR CURVE
C     Y:	DECAY CORRECTED BANK PAIR CURVE
C     X:	DECAY CORRECTED BLOOD CURVE
C     S:	DECAY CORRECTED DECONVOLVED BLOOD CURVE
C     D:	DECAY CORRECTED 10 * (d/dt) BANK PAIR CURVE
C
      DATA	TIMPBIN/1./
      DATA	NBINFIT/3/
      DATA	IN /5/
C
8000  FORMAT(F5.0)
7900  FORMAT(F4.0)
8100  FORMAT(F)
8200  FORMAT(I1)
8300  FORMAT(A24)
8400  FORMAT(A4)
8500  FORMAT(A20)
C
      TYPE "(' ENTER RADIONUCLIDE')"
      READ (IN, "(A1)") ASCII
      IF (ASCII .EQ. 'O' .OR. ASCII .EQ. 'o')THEN
        THALF = 122.1
        TYPE
     &"(' *** NOTE INCORRECT T1/2 COMPATIBLE WITH METPROC ***')"
      ENDIF
      IF (ASCII .EQ. 'C' .OR. ASCII .EQ. 'c') THALF = 1218.
      AK0 = ALOG(2.) / THALF
      TYPE "(' HALF LIFE SET TO',F9.2,' SECONDS')", THALF
      AK0 = ALOG(2.) / THALF
      TYPE "(' ENTER WELL COUNTER CONVERSION MULTIPLIER <1.>')"
      READ (IN, 8000) WELLF
      IF (WELLF .EQ. 0.) WELLF = 1.
      TYPE "(' ENTER TIME PER BIN IN SEC <1.>')"
      READ (IN, 7900) TIMPBIN
      IF (TIMPBIN .LE. 0.) TIMPBIN = 1.
      TYPE "(' ENTER MAXIMUM NUMBER OF BINS IN A RUN')"
      READ (IN, *) NBIN
      CALL GETTKE (IN, T0, AK1, E)
C     20 second deconvolution buffer
      NBUF = IFIX (20. / TIMPBIN) + 1   
      NEXPAND = 7
   12 TIMPBINE = TIMPBIN * (.5) ** NEXPAND
      U = AK1 * (FLOAT (NBINMAX) * TIMPBINE - T0)
      IF (U .GT. 0. .AND. E * U ** 2 .GT. 20.) GOTO 15
      NEXPAND = NEXPAND - 1
      GOTO 12
   15 N = IFIX(ALOG(FLOAT(NBINMAX) / FLOAT(NBIN + NBUF)) / ALOG(2.))
      NEXPAND = MIN0(NEXPAND, N)
      TYPE "(' NEXPAND', I3)", NEXPAND
      TIMPBINE = TIMPBIN * (.5) ** NEXPAND
C     KERNEL CALCULATION
      DO 11 I = 1, NBINMAX
      U = AK1 * (FLOAT(I - 1) * TIMPBINE - T0)
      IF (U .LE. 0. .OR. E * U ** 2 .GT. 20.) THEN
      H(I) = 0.
      ELSE
      R = 1. / (1. + U)
      H(I) = AK1 * TIMPBINE * R * EXP(-E * U ** 2) * (2. * E * U + R)
      ENDIF
   11 CONTINUE
      I0 = IFIX(T0 / TIMPBINE)
      IF (H (NBINMAX) .NE. 0) 
     &	TYPE "(' H (HNBINMAX)', E12.6)", H (NBINMAX)
  131 FORMAT(' IMPULSE AREA', F10.4)
      T = 0.
      DO 13 I = 1, NBINMAX
   13 T = T + H(I)
      TYPE 131, T
      DO 14 I = 1, NBINMAX
   14 H(I) = H(I) / T
      TYPE "(' ENTER 0 FOR NO SHIFT')"
      TYPE "(' ENTER 1 TO SHIFT BY CURVE TAKEOFFS')"
      TYPE "(' ENTER 2 TO SHIFT BY UPTAKE MODELING PEAKS')"
      TYPE "(' ENTER 3 TO SHIFT BY USER GIVEN VALUE')"
      READ (IN, 8200) NSHIFT
      EF = .85
      IF (NSHIFT .EQ. 2) THEN
        TYPE "(' ENTER EXTRACTION FRACTION <0.85>')"
        READ (IN, 8500) STR
        IF (INANYR (STR, ' ') .GT. 0) DECODE (4, 8100, STR) EF
      ENDIF
      IF(NSHIFT .EQ. 3) TYPE
     &"(' YOU WILL BE ASKED TO ENTER SHIFTS FOR EACH CURVE')"
      TYPE "(' ENTER POST DECONVOLUTION NSMOOTH <2>')"
      READ (IN, 8500) STR
      IF (INANYR(STR, ' ') .LE. 0)THEN
        NSMO = 2
      ELSE
        DECODE (4, 8200, STR) NSMO
      ENDIF
   16 TYPE "(' ENTER SCAN FILE SPECICATION ROOT (e.g., P1916)')"
      READ (IN, 8300) ROOSPC
      LR = INANYR(ROOSPC, ' ')
      FILSPC = ROOSPC
      FILSPC(LR+1:LR+4) = '.DTA'
      INQUIRE(FILE = FILSPC, EXIST = LEXIST, ERR = 16)
      IF (LEXIST) THEN
        TYPE "(1X, A20,' ALREADY EXISTS')", FILSPC
        GOTO 16
      ENDIF
      OPEN (1, FILE = FILSPC, STATUS = 'NEW', ERR = 16)
      TYPE "(' OPENING ', A24)", FILSPC
      WRITE (1, 119) ROOSPC (1:10), WELLF, T0, AK1, E, NSMO
  119 FORMAT('@ @@ ', A10,
     &' WELLF=', F6.2, ' T0=', F5.2, ' K1=', 
     &F6.3, ' E= ', F4.3, ' NSMO=', I2)
      TYPE "(' ENTER TOTAL NUMBER OF SCANS')"
      READ (IN, *) NSCAN
      WRITE (1, "(I)") NSCAN
      DO 39 N = 1, NSCAN
   36 TYPE "(' ENTER SCAN ID (e.g., HO1)')"
      READ (IN, 8400) SCANID
      FILSPC = ROOSPC
      FILSPC (LR+1:LR+4) = SCANID
      L = INANYR(FILSPC,' ')
      FILSPC(L+1:L+4) = '.CRV'
      OPEN (8, FILE = FILSPC, STATUS = 'OLD', ERR = 38)
      GOTO 37
   38 TYPE "(' OPEN ERROR ', A24)", FILSPC
      GOTO 36
   37 WRITE (1, "(I1, 1X, A4)") 2, SCANID
      TYPE "(' ENTER POST INJECTION SCAN START TIME, LENGTH (SEC)')"
      READ (IN, *) START, SEC
      READ (8, 8500) STR
      TYPE 117, STR
  117 FORMAT (1X, A20)
      READ (8,*) NBIN
      TYPE "(' NBIN = ', I6)", NBIN
      IF (2 ** NEXPAND * NBIN .GT. NBINMAX) 
     &  STOP 'NBIN TOO LARGE'
      DO 31 I = 1, NBIN
        IF (LBANK) THEN
          READ (8, *) T, J, K
          M = NINT(T)
        ELSE
          READ (8, *) T, J
          M = NINT(T)
          K = 0
        ENDIF
C        IF (M .NE. I) STOP '.CRV FILE READ ERROR'
        G(I) = FLOAT(J) * WELLF
   31   B(I) = FLOAT(K)
      CLOSE (8)
      CALL CRVMAX (B, NBIN, BANKMAX)
      CALL CRVCOPY (G, NBIN, X)
      DO 41 I = NBIN + 1, NBINMAX
   41 X(I) = 0.
      TYPE "(' EXPANDED NBIN',I6)", NBIN * 2 ** NEXPAND
      NBINE = NBIN
      DO 42 K = 1, NEXPAND
   42 CALL EXPAND (X, NBINE)
      CALL CRVCOR(X,NBINE,TIMPBINE,AK0)
      NBUFE = NBUF * 2 ** NEXPAND
      DO 44 I = 1, NBUFE
   44   X (NBINMAX - I + 1) = X(1)
      CALL DECONV (X, H, S, NBINMAX, A)
      K = NBINE / NBIN
      DO 43 I = 1, NBIN
   43 S(I) = S(I * K)
      DO 47 I = 1, NSMO
   47 CALL CRVSMO (S, NBIN)
      CALL CRVTOFF (S, NBIN, BT)
      CALL CRVCOPY (B, NBIN, Y)
      CALL CRVCOR (Y, NBIN, TIMPBIN, AK0)
      CALL CRVTOFF (Y, NBIN, HT)
      J = NINT (HT + 30. / TIMPBIN)
      CALL CRVCOPY (Y, NBIN, D)
      CALL CRVDIF (D, NBIN, TIMPBIN)
      CALL CRVSMO (D, NBIN)
      CALL CRVSMO (D, NBIN)
      CALL CRVPEAK (D, NBIN, HP)
      DO 45 I = 1, J
   45 D(I) = 10. * D(I)
      DO 46 I = J + 1, NBIN
   46 D(I) = 0.
   49 CONTINUE
      CALL CRVCOPY (S, NBIN, A)
      CALL CRVDIF (A, NBIN, TIMPBIN)
      DO 52 I = 1, NBIN
   52 A(I) = EF * S(I) + (1. - EF) * A(I)
      CALL CRVPEAK (A, NBIN, BP)
      IB = IFIX(BP)
      IH = IFIX(HP)
      DO 51 I = 1, 2 * NBINFIT + 1
      AT(I) = FLOAT (I - NBINFIT - 1)
   51 TYPE "(I3, 3F12.0)", I, AT(I), 
     &A(IB - NBINFIT + I), D(IH - NBINFIT + I)
      CALL POLFIT(AT, A(IB + 1 - NBINFIT), 
     &2 * NBINFIT + 1, AP, AA, 3, CHISQR)
      BPFIT = +.5 + FLOAT(IB) - .5 * AP(1) / AP(2)
      CALL POLFIT (AT, D(IH + 1 - NBINFIT), 
     &2 * NBINFIT + 1, AP, AA, 3, CHISQR)
      HPFIT = +.5 + FLOAT (IH) - .5 * AP(1) / AP(2)
      TYPE "(' DELAY BY CURVE TAKEOFFS   (sec)', F8.2)"
     &, (BT-HT) * TIMPBIN
      TYPE "(' DELAY BY EXTRACTION MODEL (sec)', F8.2)"
     &, (BPFIT - HPFIT) * TIMPBIN
C
      IF (NSHIFT .EQ. 3) THEN
      TYPE
     &  "(' ENTER SHIFT TIME (sec) (NEGATIVE VALUES SHIFT TO LEFT)')"
        READ (IN, 8000) T
        SHIFT = T / TIMPBIN
      ENDIF
      IF (NSHIFT .EQ. 1) SHIFT = -BT + HT
      IF (NSHIFT .EQ. 2) SHIFT = -BPFIT + HPFIT
      IF (NSHIFT .NE. 0) THEN
      J = NINT(SHIFT - .5)
      W = SHIFT - FLOAT(J)
      DO 61 I = 1, NBIN
   61 A(I) = 0.
      DO 62 I = 1, NBIN
      IF (I + J .GT. 0 .AND. I + J .LE. NBIN)
     & A(I + J) = A(I + J) + (1. - W) * S(I)
      IF (I + J + 1 .GT. 0 .AND. I + J + 1 .LE. NBIN)
     & A(I + J + 1) = A (I + J + 1) + W * S(I)
   62 CONTINUE
      CALL CRVCOPY (A, NBIN, S)
      ENDIF
      Q = 0.
      CALL CRVMIN(S, NBIN, Q)
      CALL CRVMIN(D, NBIN, Q)
      IF (LDECOR) GOTO 54
      CALL CRVCOR(S, NBIN, TIMPBIN, -AK0)
      CALL CRVCOR(Y, NBIN, TIMPBIN, -AK0)
      CALL CRVCOR(D, NBIN, TIMPBIN, -AK0)
   54 CONTINUE
      WRITE(1, "(2F9.0)") START, SEC
      WRITE(1, "(F10.4)") BANKMAX / 1000.
      WRITE(1, "(2F10.4)") 0., 0.
      WRITE(1,"(I)")NBIN
      DO 53 I=1,NBIN
   53 WRITE(1, 101) FLOAT(I) * TIMPBIN, S(I), Y(I), D(I)
  101 FORMAT(F9.2, 3F9.0)
   39 CONTINUE
      CLOSE(1)
      STOP
      END
C
      SUBROUTINE CRVCOPY (A, N, B)
      REAL*4 A(N), B(N)
      DO 1 I = 1, N
    1 B(I) = A(I)
      RETURN
      END
C
      SUBROUTINE EXPAND (A, NBIN)
      REAL*4 A(NBIN)
      DO 1 I = 1, NBIN
      II = NBIN - I + 1
    1 A(2*II) = A(II)
      DO 2 I = 1, NBIN - 1
      II = 2 * I
    2 A(II+1) = .5 * (A(II) + A(II + 2))
      NBIN = 2 * NBIN
      RETURN
      END
C
      SUBROUTINE CRVCOR (A, N, TIMPBIN, AK0)
      REAL*4 A(N)
      DO 1 I = 1, N
      T = FLOAT(I) * TIMPBIN
    1 A(I) = A(I) * EXP(AK0 * T)
      RETURN
      END
C
      SUBROUTINE CRVMIN (X, N, A)
      REAL*4 X(N)
      DO 1 I = 1, N
    1 X(I) = AMAX1(X(I), A)
      RETURN
      END
C
      SUBROUTINE CRVMAX (X, N, A)
      REAL*4 X(N)
      A = 0.
      DO 1 I = 1, N
    1 A = AMAX1 (X(I), A)
      RETURN
      END
C
      SUBROUTINE CRVDIF (A, N, TIMPBIN)
      REAL*4 A(N)
      U = A(1)
      DO 1 I = 1, N - 2
      T = .5 * (A(I + 2) - U)
      U = A(I + 1)
    1 A(I + 1) = T / TIMPBIN
      A(1) = 0.
      A(N) = 0.
      RETURN
      END
C
      SUBROUTINE CRVSMO (A, N)
      REAL*4 A(N)
      T = A(1)
      DO 1 I = 2, N
      U = A(I - 1) + 2. * A(I) + A(I + 1)
      A(I - 1) = T
    1 T = .25 * U
      RETURN
      END
C
C     RETURNS FBIN CORRESPONDING TO PEAK
C     FBIN IS BIN COUNTING FROM 0 + FRACTION
      SUBROUTINE CRVPEAK (A, N, FBIN)
      REAL*4 A(N)
      M = 1
      DO 1 I = 1, N
    1 IF (A(I) .GT. A(M)) M = I
      FBIN = -.5 + FLOAT(M) + .5 * (A(M - 1) - A(M + 1)) 
     &/ (A(M + 1) + A(M - 1) - 2. * A(M))
      RETURN
      END
C
C     RETURNS FBIN CORRESPONDING TO TAKEOFF
C     FBIN IS BIN COUNTING FROM 0 + FRACTION
      SUBROUTINE CRVTOFF (A, N, FBIN)
      REAL*4 A(N)
      DATA CRIT/.1/
      M = 1
      DO 1 I = 1, N
    1 IF (A(I) .GT. A(M)) M = I
      V = (A(M) - A(1)) * CRIT + A(1)
      DO 2 I = 1, N
    2 IF (A(I) .GT. V) GOTO 3
    3 FBIN = -.5 + FLOAT(I) - (A(I) - V) / (A(I) - A(I - 1))
      RETURN
      END
