C$Header: /home/npggw/tom/src/blood/RCS/bldparm.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldparm.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.6  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.5  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.4  1994/01/03  20:30:11  tom
c correctly numbers scans > 10
c uses lower case for scanid
c
c Revision 1.3  1992/10/13  19:27:41  ty7777
c Deleted error messages.
c
c Revision 1.2  1992/10/13  19:07:50  ty7777
c blood working version.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   bldparm.f
C  Author:       Tom O. Videen
C  Date:         03-Oct-86
C
C  Intent:
C     This subroutine gets information from the user on parameters
C     in each blood curve
C
C     Subroutine of BLOOD.FTN
C
C  History:
C     Modified 02-May-88 by TOV to allow oxygen content to be
C        as low as 0.01.
C
C  Common Variables whose values are set in this subroutine:
C     SCANTYPE - type of scan (1-9)
C     SCANLENGTH - length of scan in seconds;
C     ISOTOPE - isotope (1-6);
C     SCANSTART - start time of scan in MIN.SEC;
C     BANKPAIRS - peak bank pair count during scan;
C     OXYCONT - oxygen content;
C     HEMATOCRIT - percent;
C     SCANID - scan id (last 3-4 characters of scan file name);
C
C  Uses TERMINAL.LIB
C     GETINT
C     GETREAL
C     GTSTRING
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDPARM (ICURVE)

      INCLUDE 'blood.inc'

      INTEGER*4    ICURVE, I
      CHARACTER*2  ID(9)
      CHARACTER*80 Q, STR
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldparm.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      ID(1) = 'oo'
      ID(2) = 'ho'
      ID(3) = 'oc'
      ID(4) = 'bu'
      ID(5) = 'sp'
      ID(6) = 'xx'

C  Get Scan Type

8500  FORMAT (4X, I1, ' = ', A)
      WRITE (USEROUT, *)
      DO 100 I = 1, 9
        WRITE (USEROUT, 8500) I, RUNTYPE(I)
100   CONTINUE
      WRITE (USEROUT, *)
      IF (ICURVE .LT. 10) THEN
        Q = 'Scan Type for Blood Curve #'//CHAR(48 + ICURVE)
      ELSE
        Q = 'Scan Type for Blood Curve #'//CHAR(48 + (ICURVE/10))//CHAR(48 + MOD(ICURVE,10))
      ENDIF
      CALL GETINT (Q, SCANTYPE, 1, 9)

C  Get ISOTOPE if it is not specified by the scan type

      IF (.NOT. SAVE .OR. SCANTYPE .EQ. 6) THEN
        WRITE (USEROUT, *)
        DO 200 I=1,6
          WRITE (USEROUT, 8500) I, NUCLIDE(I)
200     CONTINUE
        WRITE (USEROUT, *)
        Q = 'Isotope Number'
        CALL GETINT (Q, ISOTOPE, 1, 6)
        WRITE (USEROUT, *) 'Isotope = ', NUCLIDE(ISOTOPE)
      ELSE
        IF (SCANTYPE .EQ. 4) THEN
          ISOTOPE = 3
        ELSE IF (SCANTYPE .EQ. 5) THEN
          ISOTOPE = 5
        ELSE
          ISOTOPE = 1
        ENDIF
      END IF

      IF (.NOT. SAVE) RETURN

C  Additional parameters only if a .DTA file is being created

      WRITE (USEROUT, *) 'Scan File ID is the last 3-4 characters of the scan file name.'
      IF (ICURVE .LT. 10) THEN
        SCANID = ID(SCANTYPE) // CHAR(48 + SCANNUM(SCANTYPE))
      ELSE
        SCANID = ID(SCANTYPE) // CHAR(48 + (SCANNUM(SCANTYPE)/10)) // 
     &     CHAR(48 + MOD(SCANNUM(SCANTYPE),10))
      ENDIF
      STR = SCANID
      Q = 'Scan File ID'
      CALL GTSTRING(Q,STR)
      SCANID = STR(1:4)

      Q = 'Time delay until scan start (MIN.SEC)'
      IF (ISOTOPE .EQ. 1) THEN
        CALL GETREAL (Q, SCANSTART, 0.0, 10.0)
      ELSE
        CALL GETREAL (Q, SCANSTART, 0.0, 60.0)
      END IF
      Q = 'Length of Scan (MIN.SEC)'
      IF (ISOTOPE.EQ.1) THEN
        CALL GETREAL (Q, SCANLENGTH, 0.10, 20.0)
      ELSE
        CALL GETREAL (Q, SCANLENGTH, 0.10, 1000.0)
      END IF
      Q = 'Peak Bank Pairs for this scan (in thousands)'
      CALL GETREAL (Q, BANKPAIRS, 0.0, 100.0)
      IF (SCANTYPE.EQ.1 .OR. SCANTYPE.EQ.7) THEN
        Q = 'Oxygen Content (ml/ml)'
        CALL GETREAL (Q, OXYCONT, 0.01, 0.40)
        WRITE (USEROUT, *) 'If the last blood point is NOT the well counts for plasma,'
        WRITE (USEROUT, *) '  then enter 0 for hematocrit.'
        Q = 'Hematocrit (percent)'
        CALL GETREAL (Q, HEMATOCRIT, 0.0, 60.0)
      ELSE IF (SCANTYPE.EQ.8 .OR. SCANTYPE.EQ.9) THEN
        Q = 'Oxygen Content (ml/ml)'
        CALL GETREAL (Q, OXYCONT, 0.01, 0.40)
        Q = 'Hematocrit (percent)'
        CALL GETREAL (Q, HEMATOCRIT, 0.0, 60.0)
      ELSE
        OXYCONT = 0.
        HEMATOCRIT = 0.
      END IF

      RETURN
      END
