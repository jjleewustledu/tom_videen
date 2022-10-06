CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   CURSOFF.FTN
C
C  Author:       Tom O. Videen
C  Date:         02-Oct-86
C  Written For:  FIELDS.LIB
C
C  Intent:
C     Turn off the cursor.
C
C  Uses:
C     SYSIO
C
C  Porting Information:
C     Rewritten for HPUX from Concurrent OS32.
C
C  Author:       M. Todd Gamble
C  Date:         11/28/89
C  Written For:  fields.o
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE CURSOFF

      INTEGER*4 TLENGTH(120), ROWLOC, COLLOC, USEROUT, USERIN
      CHARACTER TPARMS(120)*40
      LOGICAL*1 CURSOR

      COMMON   /TPARAMS/ TPARMS, TLENGTH, ROWLOC, COLLOC, CURSOR
      COMMON   /USRIO/   USERIN, USEROUT

      INTEGER*4 PBLK (6), IOWRITE

      DATA  IOWRITE /Z'00000029'/
      PBLK (6)	= 0

C  Cursor off

      IF (TLENGTH (10) .NE. 0) THEN
	CALL SYSIO (PBLK, IOWRITE, USEROUT, TPARMS (10), TLENGTH (10), 0) 
      END IF

C  Cursor invisible

      IF (TLENGTH (15) .NE. 0) THEN
	CALL SYSIO (PBLK, IOWRITE, USEROUT, TPARMS (15), TLENGTH (15), 0) 
      END IF
      CURSOR	= .FALSE.

      RETURN
      END
