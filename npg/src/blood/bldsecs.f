C$Header: /home/npggw/tom/src/blood/RCS/bldsecs.f,v 2.0 2004/02/20 16:38:58 tom Exp $
C$Log: bldsecs.f,v $
c Revision 2.0  2004/02/20  16:38:58  tom
c Feb 2004
c
c Revision 1.2  1995/05/10  18:18:13  tom
c added RCSHEADER
c
c Revision 1.1  1992/10/12  16:17:30  ty7777
c Initial revision
c
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  MODULE:       BLDSECS.FTN
C  Functions:    SECS
C                MINUTES
C
C  Author:       Tom O. Videen
C  Date:         22-Jun-86
C  Written For:  BLOOD
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Function:     SECS
C
C  Intent:
C     Converts DTIME from the form "MIN.SEC" to seconds.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      FUNCTION SECS(DTIME)
C
      REAL      SECS,DTIME,TIME,MIN,SEC
      CHARACTER*256 RCSHEADER    ! RCS header information
      RCSHEADER = "$Header: /home/npggw/tom/src/blood/RCS/bldsecs.f,v 2.0 2004/02/20 16:38:58 tom Exp $"
C
      TIME = ABS(DTIME)
      MIN = FLOAT(INT(TIME))
      SEC = 100.*(TIME-FLOAT(INT(TIME)))
      SECS = 60.*MIN + SEC
C
      RETURN
      END
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Function:     MINUTES
C
C  Intent:
C     Converts DTIME from seconds to the form "MIN.SEC"
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      FUNCTION MINUTES(DTIME)
C
      REAL      MINUTES,DTIME,MIN,SEC
C
      MIN = FLOAT(INT(DTIME/60.))
      SEC = AMOD(DTIME,60.)
      MINUTES = MIN + 0.01*SEC
C
      RETURN
      END
