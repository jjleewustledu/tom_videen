C$Header: /home/npggw/tom/src/blood/RCS/bldcalc.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldcalc.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.3  2001/06/06  17:59:32  tom
c add ntypes 7-9 for steady-state and steady-inhalation oxygen methods
c
c Revision 1.2  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLDCALC.FTN
C  Author:       Tom O. Videen
C  Date:         22-Aug-86
C  Written for:  BLOOD.FTN
C
C  Intent:
C     This subroutine calculates the decay-corrected counts,
C     CORCNTS, for all points in a blood curve.
C
C  History:
C     Modified 14-Jan-88 by TOV so decay correction is computed by
C        including the average decay during the well counting period.
C        This is now always computed through a single subroutine BLDDECOR.
C     Modified Jun 2001 TOV: last point of scantype=7 is handled like
C        last point of scantype=1 (plasma counts).
C
C  Uses Subroutine:
C     BLDDECOR  - decay correct a single point in a blood curve;
C
C  Called by BLDREPRO.FTN
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE BLDCALC

      INCLUDE 'blood.inc'

      INTEGER*4  I
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldcalc.f,v 2.0 2004/02/20 16:38:58 tom Exp $"

      DO 100 I=1,N-1
        CALL BLDDECOR(I)
100   CONTINUE

C  Points sampled from plasma should have densities adjusted 

      IF ((SCANTYPE.EQ.1 .OR. SCANTYPE.EQ.7) .AND. HEMATOCRIT.NE.0.0) THEN
        CORCNTS(N-1) = CORCNTS(N-1)*PLASMADN/BLOODDEN
      ELSE IF (SCANTYPE.EQ.8) THEN
        DO 400 I=1,N-1
          CORCNTS(I) = CORCNTS(I)*PLASMADN/BLOODDEN
400     CONTINUE
      END IF

      RETURN
      END
