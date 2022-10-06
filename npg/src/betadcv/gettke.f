C	Subroutine:   GETTKE
C	Author:       Avi Snyder
C	Date:         07-Jul-91
C	Intent:
C		Returns deconvolution parameters T,AK,E
C		Parameters determined by KEHFIT and THFIT
C	History:
C		02-Oct-95 TOV Clean up code.

	SUBROUTINE GETTKE (T, AK, E, HEMATOCRIT)

	REAL*4		T, AK, E, HEMATOCRIT
	INTEGER*4   USERIN, USEROUT
	CHARACTER*80	Q
	CHARACTER*256	RCSHEADER

	COMMON /USRIO/ USERIN,USEROUT

	RCSHEADER = "$Id: gettke.f,v 2.2 2008/11/14 20:40:31 tom Exp $"

	WRITE (USEROUT,8000)
8000	FORMAT(' CATHETER                  NUMBER'/
     &' 35    cm @  5.00 cc/min        1  (standard)'/
     &' 35+10 cm @  5.00 cc/min        2  (extension)')

	Q = "Catheter Number"
	N = 1
	CALL GETINT(Q,N,0,10)

	IF (N.NE.1 .AND. N.NE.2) THEN
		Q = "T0"
		CALL GETREAL(Q,T0,0.,100.)
		Q = "AK1"
		CALL GETREAL(Q,AK1,0.,100.)
		Q = "E"
		CALL GETREAL(Q,T0,0.,100.)
		RETURN
	ELSE
		Q = "Hematocrit (%)"
		CALL GETREAL(Q,HEMATOCRIT,20.,60.)
		HCT = HEMATOCRIT / 100.
		IF (N .EQ. 1) THEN
			T = 3.4124 - 3.4306 * (HCT - .3552)
			AK = 0.2919 - 0.5463 * (HCT - .3552)
			E = 0.0753 - 0.1621 * (HCT - .3552)
		ELSE 
			T = 5.8971 - 3.2983 * (HCT - .3523)
			AK = 0.2095 - 0.1476 * (HCT - .3523)
			E = 0.0302 - 0.0869 * (HCT - .3523)
		ENDIF
	ENDIF
	RETURN
	END
