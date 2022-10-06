CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  GETCOLOR.FTN
C
C  This procedure gets an integer (I*2) from the user.
C     The value corresponds to a color when loaded into the
C     Ramtek's auxilliary memory map.
C          Z'0F00 = red
C          Z'00F0 = green
C          Z'000F = blue
C          Z'0FFF = white
C     Each of these 4-bit locations may be set with values of
C     0 to F indicating the intensity of that primary color.
C
C  Uses Common Block  /USRIO/ USERIN,USEROUT
C     USERIN - logical unit for terminal input;
C     USEROUT - logical unit for terminal output;
C
C  Variables:
C     Q = question (passed without a question mark)
C     COLOR = integer value (I*2)
C     REDPOS,BLUPOS,GRNPOS = 4 bit location of each color value;
C     REDNULL,BLUNULL,GRNNULL = complement of REDPOS,BLUPOS,GRNPOS;
C     RED,GREEN,BLUE = amount of each primary (1-15);
C     REDSHIFT = bit shift to move 4-bit red value to lowest bits;
C     GRNSHIFT = bi[Cshift to move 4-bit green value to lowest bits;
C
C  Uses Subroutines:
C     GETINT
C
C  Subroutine of TERMINAL.LIB
C
C  T.O. Videen -- 24-Jan-86
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C $Log: getcolor.f,v $
c Revision 1.1  1992/04/13  19:08:15  tom
c Initial revision
c
c Revision 1.1  89/11/21  12:40:55  12:40:55  todd (M. Todd Gamble)
c Initial revision
c 
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE GETCOLOR(Q,COLOR)
C
      INTEGER*2    COLOR,REDPOS,GRNPOS,BLUPOS,REDNULL,GRNNULL,BLUNULL
      INTEGER*4    RED,GREEN,BLUE,REDSHIFT,GRNSHIFT,USERIN,USEROUT
      CHARACTER*80 Q,Q1
C
      COMMON /USRIO/ USERIN,USEROUT
C
      DATA REDPOS   /Z'0F00'/
      DATA GRNPOS   /Z'00F0'/
      DATA BLUPOS   /Z'000F'/
      DATA REDNULL  /Z'F0FF'/
      DATA GRNNULL  /Z'FF0F'/
      DATA BLUNULL  /Z'FFF0'/
      DATA REDSHIFT /8/
      DATA GRNSHIFT /4/
C
      L = INANYR(Q,' ')
      WRITE(USEROUT,*) Q(1:L)
C
      Q1 = 'Amount of Red (0-15)'
      RED = ISHFT(IAND(COLOR,REDPOS),-REDSHIFT)
      CALL GETINT(Q1,RED,0,15)
      COLOR = IAND(COLOR,REDNULL)
      COLOR = IOR(COLOR,ISHFT(RED,REDSHIFT))
C
      Q1 = 'Amount of Green (0-15)'
      GREEN = ISHFT(IAND(COLOR,GRNPOS),-GRNSHIFT)
      CALL GETINT(Q1,GREEN,0,15)
      COLOR = IAND(COLOR,GRNNULL)
      COLOR = IOR(COLOR,ISHFT(GREEN,GRNSHIFT))
C
      Q1 = 'Amount of Blue (0-15)'
      BLUE = IAND(COLOR,BLUPOS)
      CALL GETINT(Q1,BLUE,0,15)
      COLOR = IAND(COLOR,BLUNULL)
      COLOR = IOR(COLOR,BLUE)
C
      RETURN
      END
