CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Include:      BLOOD.INC
C  Author:       Tom O. Videen
C  Date:         21-Aug-86
C
C  History:
C     Halflife for no decay changed to 1.0E+9 by TOV 06-Oct-88.
C
C  Intent:
C     Common Block variables for BLOOD.FTN and subroutines.
C
C     USRIO - logical unit for user I/O
C     BLOODPRM - blood parameters
C     BLOODATA - data initializations for BLOODPRM
C
C  Variables:
C     ISOTOPE - isotope (1-6);
C     COUNTS - counts per COUNTIME seconds from well counter;
C     N - number of NEXT point in blood curve;
C        (the point following the last point entered in the curve)
C     DTAFILE - logical unit to write to .DTA file;
C     OLDDAT  - logical unit to read old .DTA file;
C     SCANNUM - scan number for each type of run;
C     SCANSTART - start time of scan in MIN.SEC;
C     BANKPAIRS - peak bank pair count during scan;
C     OXYCONT - oxygen content;
C     HEMATOCRIT - percent;
C     HALFLIFE - in seconds;
C     DRYWEIGHT - dry weight of syringe;
C     WETWEIGHT - weight of syringe with blood sample;
C     COUNTIME - seconds summed in well counter (usually 10);
C     TIMEDRAW - time blood sample was taken (MIN.SEC);
C     TIMESECS - time blood sample was taken in seconds;
C     TIMECNT - time blood sample was counted (MIN.SEC);
C     CORCNTS - decay corrected counts/(ml*sec);
C        (corrected to time of injection)
C     SC1 - correction factor for well counter;
C     SC2 - correction factor for well counter (related to volume);
C     BLOODDEN - density of whole blood (g/ml);
C     PLASMADN - density of plasma (g/ml);
C     SCANID - scan id (last 3-4 characters of scan file name);
C     NUCLIDE - name of nuclide;
C     RUNTYPE - type of run;
C     SAVE - true--> store results in a .DTA file;
C     VARYTIME - true--> COUNTIME is variable;
C                false-->COUNTIME = DEFTIME seconds;
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

**USRIO
      INTEGER*4    USERIN    ! log unit assigned for terminal input!
      INTEGER*4    USEROUT   ! log unit for terminal output!

**BLDIPARM
      INTEGER*4    SCANTYPE  ! type  of scan (1-9)
      INTEGER*4    ISOTOPE
      INTEGER*4    COUNTS(200)
      INTEGER*4    N
      INTEGER*4    DTAFILE
      INTEGER*4    SCANNUM(9)
      INTEGER*4    OLDDTA
      INTEGER*4    SCANNER    ! scanner (1=other, 2=953b, 3=961)
      INTEGER*4    DEFTIME    ! default count time
      LOGICAL*1    SAVE
      LOGICAL*1    VARYTIME

**BLDCPARM
      CHARACTER*4  SCANID
      CHARACTER*15 NUCLIDE(6)
      CHARACTER*41 RUNTYPE(9)

**BLDRPARM
      REAL*4       SCANSTART
      REAL*4       BANKPAIRS
      REAL*4       OXYCONT
      REAL*4       HEMATOCRIT
      REAL*4       HALFLIFE(6)
      REAL*4       DRYWEIGHT(200)
      REAL*4       WETWEIGHT(200)
      REAL*4       COUNTIME(200)
      REAL*4       TIMEDRAW(200)
      REAL*4       TIMECNT(200)
      REAL*4       CORCNTS(200)
      REAL*4       SC1
      REAL*4       SC2
      REAL*4       TIMESECS(200)
      REAL*4       BLOODDEN
      REAL*4       PLASMADN
      REAL*4       SCANLENGTH  ! length of scan in MIN.SEC

      COMMON /USRIO/ USERIN,USEROUT

      COMMON /BLDIPARM/ SCANTYPE,ISOTOPE,COUNTS,N,DTAFILE,
     &                  SCANNUM,OLDDTA,SCANNER,DEFTIME,SAVE,VARYTIME

      COMMON /BLDCPARM/ SCANID,NUCLIDE,RUNTYPE

      COMMON /BLDRPARM/ SCANSTART,BANKPAIRS,OXYCONT,
     &                  HEMATOCRIT,HALFLIFE,DRYWEIGHT,
     &                  WETWEIGHT,COUNTIME,TIMEDRAW,
     &                  TIMECNT,CORCNTS,SC1,SC2,TIMESECS,
     &                  BLOODDEN,PLASMADN,SCANLENGTH
