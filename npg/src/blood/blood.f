C$Header: /home/npggw/tom/src/blood/RCS/blood.f,v 2.2 2009/04/22 19:48:29 tom Exp $
C$Log: blood.f,v $
c Revision 2.2  2009/04/22  19:48:29  tom
c default scanner = 3
c
c Revision 2.1  2009/01/09  15:07:20  tom
c changed prompt for SCANNER to include 962
c
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.7  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.6  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.5  1995/05/10  15:44:32  tom
c prompts for scanner and uses different default times for 953 and 961
c
c Revision 1.4  1994/01/03  20:32:04  tom
c altered bldparam
c
c Revision 1.3  1992/10/14  20:52:06  ty7777
c Release version of blood.
c
c Revision 1.2  1992/10/13  19:08:09  ty7777
c blood working version.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Program:      blood.f
C  Author:       Tom O. Videen
C  Date:         27-Oct-86
C  Written for:  Creating and modifying blood curve files (.dta)
C
C  Revision & Reorganization of NS8:METENTRY.FTN/191 11/20/85
C             by Mark Mintun
C  History:
C     Revised 26-Feb-87 by TOV with following changes:
C        1) Correction factors introduced for TB syringes;
C        2) 5 decimal places for syringe weights (instead of 3);
C     Revised 19-Aug-87 by TOV so that SAVE=FALSE really deletes the
C        DTA file instead of saving one with 0 records.
C     Revised 24-Aug-87 by TOV so that blood counts are always reprocessed
C        upon reading a blood curve whenever an existing DTA file is read.
C        Previously, counts were reprocessed only if a curve was altered
C        or if the user requested reprocessing.
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C        including the average decay during the well counting period.
C        This is now always computed through a single subroutine BLDDECOR.
C     Modified BLDPARM 02-May-88 by TOV to allow lower values of
C        oxygen content to be entered (requested by DIA).
C        Modified message in BLOOD.FTN indicating the beige caps are
C        identical to blue syringe caps.
C     Modified BLOOD.INC 06-Oct-88 by TOV changing halflife for no decay
C        to 1.0E+9 and altering BLDDECOR for processing with no decay.
C     Modified blood.inc 12-Oct-92 by Tom Yang moving all the data 
C        declarations to blood.c and ported the program to the from 
C        PE2 to the Sun Unix environment.
C     Modified blood.inc 09-May-95 by TOV to add SCANNER.
C
C  Intent:
C     This program is used for entering blood curve data from PET scans.
C
C     The program computes the counts per second for each blood sample
C     corrected for decay from the time of sampling, the volume of the
C     sample used in the well counter, and the position of the sample
C     in the well counter.
C     The user enters syringe dry & wet weights, times drawn & counted,
C     and the counts per n seconds, as well as pertinent data concerning
C     the PET scan.
C     The program is intended for creating .DTA files (used by METEVAL)
C     but may be used to display corrected counts without saving values.
C     See Subroutine BLDSAVE.FTN for .DTA file structure.
C
C  Uses BLOOD Subroutines:
C     BLDNEW   - create a new DTA file;
C     BLDREPRO - reprocess an existing DTA file;
C
C  Uses libpe
C     YESNO
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      PROGRAM BLOOD

      INTEGER*4    AUXLU
      REAL         SC1NEW, SC2NEW, SC1OLD, SC2OLD, SC1TB, SC2TB
      CHARACTER*80 Q, DTAOUTPUT, TEMPDTAOUT
      CHARACTER*1  REPROCESTR
      CHARACTER*256 RCSHEADER    ! RCS header information

      LOGICAL*1    BLUECAPS, REPROCES, A

      INCLUDE 'blood.inc'
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/blood.f,v 2.2 2009/04/22 19:48:29 tom Exp $"

      DATA  RUNTYPE(1) /'O-15 Oxygen Metabolism Study  (OO)       '/
      DATA  RUNTYPE(2) /'O-15 Water Blood Flow Study   (HO)       '/
      DATA  RUNTYPE(3) /'O-15 Blood Volume Study       (OC)       '/
      DATA  RUNTYPE(4) /'C-11 Butanol Blood Flow Study (BU)       '/
      DATA  RUNTYPE(5) /'F-18 Study                               '/
      DATA  RUNTYPE(6) /'Misc. Study                              '/
      DATA  RUNTYPE(7) /'O-15 Oxygen Steady-State Study           '/
      DATA  RUNTYPE(8) /'O-15 Oxygen Steady-Inhalation Plasma     '/
      DATA  RUNTYPE(9) /'O-15 Oxygen Steady-Inhalation Whole Blood'/

      DATA  NUCLIDE(1) /'O-15           '/
      DATA  NUCLIDE(2) /'N-13           '/
      DATA  NUCLIDE(3) /'C-11           '/
      DATA  NUCLIDE(4) /'Ga-68          '/
      DATA  NUCLIDE(5) /'F-18           '/
      DATA  NUCLIDE(6) /'NONE (no decay)'/

C  Halflives are in seconds with same number of significant figures as
C  given in the following source:
C  Lederer & Shirley, Table of Isotopes, 1978 (7th edition), John Wiley.

      DATA  HALFLIFE(1) /122.1/  ! METEVAL compatible
      DATA  HALFLIFE(2) /597.8/
      DATA  HALFLIFE(3) /1223./
      DATA  HALFLIFE(4) /4100./
      DATA  HALFLIFE(5) /6583.2/
      DATA  HALFLIFE(6) /1.0E+9/

C  Densities of whole blood and plasma taken from:
C  Herscovitch & Raichle (1985) J. Cereb Blood Flow Metabol, 5:65

      DATA BLOODDEN,PLASMADN /1.05,1.03/

      DATA AUXLU, DTAFILE, OLDDTA, USERIN, USEROUT /1, 2, 3, 5, 6/
      DATA SC1NEW, SC2NEW /1.026, -0.0522/
      DATA SC1OLD, SC2OLD /1.098, -0.1969/
      DATA SC1TB, SC2TB   /1.063, -0.300/

C  Get command line arguments.

      CALL GETARG (1, DTAOUTPUT)
      CALL GETARG (2, REPROCESTR)
      IF (REPROCESTR .EQ. '1') THEN
        REPROCES = .TRUE.
      ELSE
        REPROCES = .FALSE.
      END IF
      CALL GETARG (3, TEMPDTAOUT)
      SCANNER = 3

C  Open the .dta file

      IF (REPROCES) THEN
        OPEN (OLDDTA, FILE = DTAOUTPUT, STATUS = 'OLD', ERR = 55)
        OPEN (DTAFILE, FILE = TEMPDTAOUT, STATUS = 'NEW', ERR = 55)
      ELSE
        OPEN (DTAFILE, FILE = DTAOUTPUT, STATUS = 'NEW', ERR = 55)
      END IF
      GOTO 77
55    WRITE (USEROUT, *)'**** ERROR opening .dta file ****'
      WRITE (USEROUT, *)'****     Program Aborted     ****'
      STOP

77    CONTINUE
      WRITE (USEROUT, *) RCSHEADER(40:64)
      WRITE (USEROUT, *)
      WRITE (USEROUT, *) 'NOTE: The default values for scanner, syringe caps and syringe type are NOT'
      WRITE (USEROUT, *) 'read from the dta file. You must answer them properly as the blood curve will'
      WRITE (USEROUT, *) 'ALWAYS be REPROCESSED according to your answers'
      WRITE (USEROUT, 8520)
8520  FORMAT (77 (1H-))

C  Different syringes were used prior to 11/17/86
C  The old syringes had WHITE caps and fit held samples
C  in the well counter in a different position than the
C  new syringes with BLUE caps, as well as containing a
C  different volume.

C  Note: BEIGE caps (which appeared in April 1988) are identical
C  to BLUE caps.

C  Factor SC1 corrects for the position, and factor
C  SC2 corrects for the volume.

C  These correction factors convert corrected counts to
C  those which woulc have been obtained with 0.5 gm in a
C  3 cc syringe (or 0.21 gm in a TB syringe).

      Q = 'Scanner model (1=other, 2=953, 3=961 or 962)'
      CALL GETINT(Q,SCANNER,1,3)
      IF (SCANNER .EQ. 3) THEN
        DEFTIME = 12
      ELSE
        DEFTIME = 10
      ENDIF
      BLUECAPS = .TRUE.
      IF (SCANNER .EQ. 1) THEN
        Q = 'Blood Samples Taken after 11/17/85 (blue or beige syringe caps)'
        CALL YESNO(Q,BLUECAPS)
      ENDIF
      IF (BLUECAPS) THEN
        Q = 'Was a TB syringe used'
        A = .FALSE.
        CALL YESNO(Q,A)
        IF (A) THEN
          SC1 = SC1TB
          SC2 = SC2TB
        ELSE
          SC1 = SC1NEW
          SC2 = SC2NEW
        END IF
      ELSE
        SC1 = SC1OLD
        SC2 = SC2OLD
      ENDIF

      IF (REPROCES) THEN
        CALL BLDREPRO
      ELSE
        CALL BLDNEW
      END IF

      STOP
      END
