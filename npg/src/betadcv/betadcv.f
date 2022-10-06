C$Header: /home/npggw/tom/src/betadcv/RCS/betadcv.f,v 2.2 2008/11/14 20:40:31 tom Exp $
C$Log: betadcv.f,v $
c Revision 2.2  2008/11/14  20:40:31  tom
c added countrate correction for well counter nonlinearity
c
c Revision 2.1  2007/03/20  22:33:36  tom
c change prompt in bldenter
c
c Revision 2.0  2004/02/13  19:45:46  tom
c Feb 2004
c
c Revision 1.14  2002/11/27  22:01:56  tom
c *** empty log message ***
c
c Revision 1.13  2002/02/26  15:20:40  tom
c get starttime, binwidth, scantype, and new in calling shell
c
c Revision 1.12  2002/02/19  21:50:25  tom
c add prompt to set first 2 points equal to third
c
c Revision 1.11  2001/06/01  18:53:44  tom
c add ntype=7 for steady-state oxygen
c
c Revision 1.10  1998/07/27  17:51:43  tom
c automatically subtract 105 sec for added points to CO curve
c
c Revision 1.9  1996/10/31  19:13:03  tom
c fix loop in adding points
c
c Revision 1.8  1996/10/31  16:53:43  tom
c sync with Version
c
c Revision 1.5  1996/10/31  16:49:27  tom
c change array declarations to reflect actual space in betadcv.f and fftpe.f
c
c Revision 1.4  1996/10/29  22:29:24  tom
c permit >1 additional points at end of curve
c
c Revision 1.3  1996/01/25  21:17:35  tom
c prompt for hand drawn points for all curves
c
c Revision 1.2  1995/11/09  19:44:42  tom
c chnage format for output line 1
c
c Revision 1.1  1995/11/09  19:29:25  tom
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:	BETADCV.FTN
C  Author:	Avi Snyder (BETADTA)
C  Date:	05-Sep-90
C  Intent:	Automated blood sampler data files
C  History:
C	 09-Nov-95	TOV Cut out everything but deconvolution and 
C				decay-correction of blood curve.
C
C  Logical Units:
C		2 = Input blood curve (CRTFILE)
C		3 = Output blood curve (DCVFILE)
C		5 = User input (USERIN)
C		6 = User output (USEROUT)
C
C  Uses Subroutines contained in:
C	 DECONV
C	 FFTPE
C	 GETTKE
C
C  Internal Subroutines:
C	 CRVCOPY
C	 EXPAND
C	 CRVMIN
C	 MAXCRV
C	 CRVSMO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
	PROGRAM BETADCV
C
	INTEGER*4	CRTFILE		! input CRT file
	INTEGER*4	DCVFILE		! output DCV file
	INTEGER*4	USERIN		! log unit assigned for terminal input
	INTEGER*4	USEROUT		! log unit for terminal output!
	INTEGER*4	XBIN		! extra time bins for hand drawn samples
	INTEGER*4	SCANTYPE 	! type of scan (1-7)
	INTEGER*4	NBIN		! number of input data bins
	INTEGER*4	NBINE		! number of expanded data bins
	INTEGER*4	I, J, K, M

	CHARACTER*8		START, STRING
	CHARACTER*80	CRTNAME, DCVNAME
	CHARACTER*80	Q
	CHARACTER*15	NUCLIDE(7)
	CHARACTER*35	RUNTYPE(7)

	LOGICAL*1		ANSWER

	PARAMETER	(NBINMAX = 4096)
	REAL*4		BSRF(NBINMAX)	! Blood sucker impulse response function
	REAL*4		BLOOD(NBINMAX)	! Raw blood curve
	REAL*4		DCVBLD(NBINMAX)	! Decay corrected deconvolved blood curve
	REAL*4		TMP(2*NBINMAX + 4)	! Scratch array
	REAL*4		HEMATOCRIT
	REAL*4		OXYCONT
	REAL*4		COUNTS(100)
	REAL*4		TIME(100)
	REAL*4		COUNT1
	REAL*4		TIME1
	REAL*4		TIMPBIN
	REAL*4		HALFLIFE(6)
	REAL*4		LAMBDA

	CHARACTER*256	RCSHEADER
	COMMON /USRIO/ USERIN,USEROUT

	RCSHEADER = "$Id: betadcv.f,v 2.2 2008/11/14 20:40:31 tom Exp $"
	DATA	NBINFIT /3/
	DATA	CRTFILE, DCVFILE /2,3/
	DATA 	USERIN, USEROUT /5,6/

	DATA  RUNTYPE(1) /'O-15 Oxygen Metabolism Study   (OO)'/
	DATA  RUNTYPE(2) /'O-15 Water Blood Flow Study    (HO)'/
	DATA  RUNTYPE(3) /'O-15 Blood Volume Study        (CO)'/
	DATA  RUNTYPE(4) /'C-11 Butanol Blood Flow Study  (BU)'/
	DATA  RUNTYPE(5) /'F-18 Study                         '/
	DATA  RUNTYPE(6) /'Misc. Study                        '/
	DATA  RUNTYPE(7) /'O-15 Oxygen Steady-State Study (OO)'/

	DATA  NUCLIDE(1) /'O-15 '/
	DATA  NUCLIDE(2) /'N-13 '/
	DATA  NUCLIDE(3) /'C-11 '/
	DATA  NUCLIDE(4) /'Ga-68'/
	DATA  NUCLIDE(5) /'F-18 '/
	DATA  NUCLIDE(6) /'NONE (no decay)'/
	DATA  NUCLIDE(7) /'O-15 '/

C  Halflives are in seconds with same number of significant figures as
C  given in the following source:
C  Lederer & Shirley, Table of Isotopes, 1978 (7th edition), John Wiley.

	DATA  HALFLIFE(1) /122.1/  ! METEVAL compatible
	DATA  HALFLIFE(2) /597.8/
	DATA  HALFLIFE(3) /1223./
	DATA  HALFLIFE(4) /4100./
	DATA  HALFLIFE(5) /6583.2/
	DATA  HALFLIFE(6) /1.0E+9/

	CALL GETARG (1, CRTNAME)
	CALL GETARG (2, DCVNAME)
	CALL GETARG (3, START)
	CALL GETARG (4, STRING)
	READ (STRING,*) TIMPBIN
	CALL GETARG (5, STRING)
	READ (STRING,*) ANSWER
	CALL GETARG (6, STRING)
	READ (STRING,*) SCANTYPE

	Q = "Well Counter Conversion Multiplier"
	WELLF = 0.0
	CALL GETREAL(Q,WELLF,1.,100.)

	OPEN (CRTFILE, FILE=CRTNAME, STATUS='OLD', ERR=10)
	READ (CRTFILE,*, ERR=10) STRING
	GOTO 20
10	STOP 'ERROR Reading CRT File'
20	READ (CRTFILE,*) NBIN
	DO 30 I = 1, NBIN
		READ (CRTFILE, *) T, J
		M = NINT(T)
		BLOOD(I) = FLOAT(J) * WELLF
30	CONTINUE
	CLOSE (CRTFILE)

	WRITE (USEROUT, "('For 961 files >= p5999 (25 Feb 2002)')")
	Q = "Set first 2 points equal to third"
	CALL YESNO(Q,ANSWER)
	IF (ANSWER) THEN
		BLOOD(1) = BLOOD(3)
		BLOOD(2) = BLOOD(3)
	ENDIF
	ANSWER = .false.

	Q = "Sampling rate (sec/bin)"
	CALL GETREAL(Q,TIMPBIN,0.1,100.)

	CALL GETTKE (T0, AK1, E, HEMATOCRIT)
	NBUF = IFIX (20. / TIMPBIN) + 1   ! 20-sec deconvolution buffer
	NEXPAND = 7
100	TIMPBINE = TIMPBIN * (.5) ** NEXPAND
	U = AK1 * (FLOAT (NBINMAX) * TIMPBINE - T0)
	IF (U .GT. 0. .AND. E * U ** 2 .GT. 20.) GOTO 110
	NEXPAND = NEXPAND - 1
	GOTO 100
110	N = IFIX(ALOG(FLOAT(NBINMAX) / FLOAT(NBIN + NBUF)) / ALOG(2.))
	NEXPAND = MIN0(NEXPAND, N)
	TIMPBINE = TIMPBIN * (.5) ** NEXPAND
	IF (2 ** NEXPAND * NBIN .GT. NBINMAX) THEN
		WRITE (USEROUT, "('Too many time bins  ',I)"), NBIN
	 	STOP 'NBIN TOO LARGE'
	ENDIF

C	KERNEL CALCULATION

	DO 120 I = 1, NBINMAX
		U = AK1 * (FLOAT(I - 1) * TIMPBINE - T0)
		IF (U .LE. 0. .OR. E * U ** 2 .GT. 20.) THEN
			BSRF(I) = 0.
		ELSE
			R = 1. / (1. + U)
			BSRF(I) = AK1 * TIMPBINE * R * EXP(-E * U ** 2) * (2. * E * U + R)
		ENDIF
120	CONTINUE
	I0 = IFIX(T0 / TIMPBINE)
	T = 0.
	DO 130 I = 1, NBINMAX
		T = T + BSRF(I)
130	CONTINUE
	WRITE (USEROUT,8100) T
8100	FORMAT ("Impulse response fn area =", F8.4)
	DO 140 I = 1, NBINMAX
		BSRF(I) = BSRF(I) / T
140	CONTINUE

	Q = "Smoothing factor for deconvolved curve"
	NSMO = 2
	CALL GETINT(Q,NSMO,0,10)
			
C  Get Scan Type

8600	FORMAT (4X, I1, ' = ', A)
	DO 141 I = 1, 7
		WRITE (USEROUT, 8600) I, RUNTYPE (I)
141	CONTINUE
	WRITE (USEROUT, *)
	Q = "Scan Type"
	CALL GETINT (Q, SCANTYPE, 1, 7)

C  Get ISOTOPE if it is not specified by the scan type

	IF (SCANTYPE .EQ. 6) THEN
		WRITE (USEROUT, *)
		DO 142 I=1,6
			WRITE (USEROUT, 8600) I, NUCLIDE (I)
142   	CONTINUE
		WRITE (USEROUT, *)
		Q = 'Isotope Number'
		CALL GETINT (Q, ISOTOPE, 1, 6)
		WRITE (USEROUT, *) 'Isotope = ', NUCLIDE (ISOTOPE)
	ELSE
		IF (SCANTYPE .EQ. 4) THEN
			ISOTOPE = 3
		ELSE IF (SCANTYPE .EQ. 5) THEN
			ISOTOPE = 5
		ELSE 
			ISOTOPE = 1
		ENDIF
	ENDIF
	LAMBDA = ALOG(2.) / HALFLIFE(ISOTOPE)
	WRITE (USEROUT, "('Isotope = ',A,' Halflife =',F9.2,' seconds  Lambda = ', F)"),
     & NUCLIDE (ISOTOPE), HALFLIFE(ISOTOPE), LAMBDA

	OPEN (DCVFILE, FILE=DCVNAME, STATUS='NEW', ERR=150)
	GOTO 200
150	OPEN (DCVFILE, FILE=DCVNAME, STATUS='OLD', ERR=160)
	GOTO 200
160	WRITE (USEROUT, 8300) DCVFILE
	STOP
8300	FORMAT ("ERROR: Cannot open ", A20) 
200	CONTINUE

	DO 610 I = NBIN + 1, NBINMAX
		BLOOD(I) = 0.
610	CONTINUE
	NBINE = NBIN
	DO 620 K = 1, NEXPAND
		CALL EXPAND (BLOOD, NBINE)
620	CONTINUE

C	Decay-correct blood curve

	DO 625 I = 1, NBINE
		BLOOD(I) = BLOOD(I) * EXP(LAMBDA * I * TIMPBINE)
625	CONTINUE
	NBUFE = NBUF * 2 ** NEXPAND
	DO 630 I = 1, NBUFE
		BLOOD(NBINMAX-I+1) = BLOOD(1)
630	CONTINUE

C	Deconvolve

	CALL DECONV (BLOOD, BSRF, DCVBLD, NBINMAX, TMP)
	K = NBINE / NBIN
	DO 640 I = 1, NBIN
		DCVBLD(I) = DCVBLD(I * K)
640	CONTINUE
	DO 650 I = 1, NSMO
		CALL CRVSMO (DCVBLD, NBIN)
650	CONTINUE

C	Eliminate negative counts

	CALL CRVMIN(DCVBLD, NBIN, 0.0)

C	Two extra points for oxygen

	XBIN = 0
	TIME1 = FLOAT(NBIN) * TIMPBIN
	COUNT1 = 0.
	OXYCONT = 0.
	IF (SCANTYPE .EQ. 1) THEN
		Q = 'Oxygen Content (ml/ml)'
		CALL GETREAL (Q, OXYCONT, 0.01, 0.40)
		WRITE (USEROUT,*) "Add 2 blood points to end of curve for oxygen"
		WRITE (USEROUT,*) "for calculation of recirculating water fraction."
		WRITE (USEROUT,*) "Last point should be the plasma counts."
800		CALL BLDENTER(LAMBDA, TIME1, COUNT1, SCANTYPE)
		IF (COUNT1 .GT. 0.) THEN	
			XBIN = XBIN + 1
			TIME(XBIN) = TIME1
			COUNTS(XBIN) = COUNT1
		ENDIF
		Q = "Add another point"
		CALL YESNO(Q,ANSWER)
		IF (ANSWER) GO TO 800
	ELSE
		Q = "Add a point to the curve"
		ANSWER = .TRUE.
		CALL YESNO(Q,ANSWER)
		IF (ANSWER) THEN
810			CALL BLDENTER(LAMBDA, TIME1, COUNT1, SCANTYPE)
			IF (COUNT1 .GT. 0.) THEN	
				XBIN = XBIN + 1
				TIME(XBIN) = TIME1
				COUNTS(XBIN) = COUNT1
			ENDIF
			Q = "Add another point to the curve"
			CALL YESNO(Q,ANSWER)
			IF (ANSWER) GO TO 810
		ENDIF
	ENDIF

C  Write to DCV file:  Time, Decay-corrected deconvolved blood counts

8700	FORMAT(A8,2X,I4,F8.4,F6.1,2X,'WELLF=',F8.4,' T0=',F5.2,' K1=',
     & F6.3,' E=',F4.3,' NSMO=', I2,2X,A20)
8900	FORMAT (F9.1, F14.1)

	WRITE (DCVFILE, 8700) START, NBIN+XBIN, OXYCONT, HEMATOCRIT, WELLF, T0, AK1, E, NSMO, CRTNAME
	DO 900 I=1,NBIN
		WRITE (DCVFILE, 8900) FLOAT(I) * TIMPBIN, DCVBLD(I)
900	CONTINUE
	IF (XBIN .GT. 0) THEN
		DO 910 I=1,XBIN
			WRITE (DCVFILE, 8900) TIME(I), COUNTS(I)
910		CONTINUE
	ENDIF
	CLOSE(1)
	STOP
	END
C
	SUBROUTINE CRVCOPY (A, N, B)
	REAL*4 A(N), B(N)
	DO 10 I = 1, N
10	B(I) = A(I)
	RETURN
	END
C
	SUBROUTINE EXPAND (A, NBIN)
	REAL*4 A(NBIN*2)
	DO 10 I = 1, NBIN
		II = NBIN - I + 1
10		A(2*II) = A(II)
	DO 20 I = 1, NBIN - 1
		II = 2 * I
20		A(II+1) = .5 * (A(II) + A(II + 2))
	NBIN = 2 * NBIN
	RETURN
	END
C
	SUBROUTINE CRVMIN (X, N, A)
	REAL*4 X(N)
	DO 10 I = 1, N
10	X(I) = AMAX1(X(I), A)
	RETURN
	END
C
	SUBROUTINE CRVSMO (A, N)
	REAL*4 A(N*2)
	T = A(1)
	DO 10 I = 2, N
		U = A(I - 1) + 2. * A(I) + A(I + 1)
		A(I - 1) = T
10		T = .25 * U
	RETURN
	END
