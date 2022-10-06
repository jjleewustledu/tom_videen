c $Log
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Subroutine:   BLOODVOL.FTN
C
C  Author:       Tom O. Videen
C  Date:         02-Jun-87
C  Written For:  METPROC
C
C  Intent:
C     Decay is treated explicitly in this model.
C     This subroutine computes the relationship between regional
C     PET numbers and regional cerebral blood volume (CBV) so that
C     we can compute CBV directly from regional tissue counts
C     in SPIDERAM:
C
C       CBV = BVFACTOR * PETT
C
C     where:
C
C                            PETTCONV*100.
C        BVFACTOR = ------------------------------------
C                   Total Blood Counts*SLVRATIO*BRAINDEN
C
C     Factor 100 to convert from ml/g to ml/100g;
C
C     Units:
C       CBV                   ml/100g
C       PETT                  PET counts/(pixel*scan period)
C       Total Blood Counts    well counts/(ml*scan period)
C       SLVRATIO              (none)
C       BRAINDEN              g/ml
C       PETTCONV             (well counts*pixel)/(PET counts*ml)
C       BVFACTOR              (ml*pixel*scan period)/(100g*PET counts)
C
C  References:
C     Grubb, Raichle, Higgins & Eichling (1978)
C        Annals of Neurology, 4:322
C     Mintun, Raichle, Martin & Herscovitch (1984)
C        J. Nuclear Medicine, 25:177
C     Videen, Perlmutter, Herscovitch & Raichle (JCBFM - 1987)
C
C  Variables:
C     INTEGRAL = integral of Blood Counts over period of scan;
C
C  Common Block Variable:
C     BVFACTOR = see above;
C
C  Subroutines:
C     INTEGRAT
C
C  Called by:  METPROC
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
      SUBROUTINE BLOODVOL
C
      INCLUDE 'metproc.inc'
C
      REAL   INTEGRAL
			CHARACTER*256 RCSHEADER
C
			RCSHEADER = "$Id: bloodvol.f,v 2.1 1993/07/15 20:25:17 tom Exp $"
C
      CALL INTEGRAT(TIME,ACTIVITY,SCANST,SCANEND,INTEGRAL)
      BVFACTOR(1) = (PETTCONV(1)*100.)/(INTEGRAL*BRAINDEN*SLVRATIO)
C
      RETURN
      END
