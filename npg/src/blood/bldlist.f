C$Header: /home/npggw/tom/src/blood/RCS/bldlist.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldlist.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.5  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.4  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.3  1995/05/10  16:13:20  tom
c default count time differs for different scanners
c
c Revision 1.2  1992/10/13  19:06:56  ty7777
c blood working version.
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDLIST.FTN
C  Author:       Tom O. Videen
C  Date:         22-Aug-86
C
C  Intent:
C     This subroutine writes parameters for the scan to the terminal.
C     (scan type, id, start time, length, peak bank pairs, oxygen
C      content, hematocrit, isotope and halflife)
C
C     Subroutine of BLOOD.FTN
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDLIST

      INCLUDE 'blood.inc'

      REAL SECS
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldlist.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      WRITE(USEROUT,8610) RUNTYPE(SCANTYPE),SCANID
      WRITE(USEROUT,8620) NUCLIDE(ISOTOPE),HALFLIFE(ISOTOPE)
      WRITE(USEROUT,8630) SCANSTART,NINT(SECS(SCANSTART))
      WRITE(USEROUT,8640) SCANLENGTH,NINT(SECS(SCANLENGTH))
      WRITE(USEROUT,8650) BANKPAIRS
      IF (SCANTYPE.EQ.1 .OR. SCANTYPE.GE.7) THEN
        WRITE(USEROUT,8660) OXYCONT
        WRITE(USEROUT,8670) HEMATOCRIT
      END IF
      WRITE(USEROUT,*)

      RETURN

8610  FORMAT(1X,A41,'  Scan ID = ',A)
8620  FORMAT(1X,'Isotope     = ',A,3X,'Halflife =',F10.1,' seconds')
8630  FORMAT(1X,'Scan Start  = ',F10.2,'  or ',I6,' seconds')
8640  FORMAT(1X,'Scan Length = ',F10.2,'  or ',I6,' seconds')
8650  FORMAT(1X,'Peak Bank Pairs =',F8.2)
8660  FORMAT(1X,'Oxygen Content  =',F8.2)
8670  FORMAT(1X,'Hematocrit      =',F8.2,'%')

      END
