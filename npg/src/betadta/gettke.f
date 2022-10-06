      SUBROUTINE GETTKE (IN, T, AK, E)
C     Written 07-Jul-91 AZS
C     Returns deconvolution parameters T,AK,E
C     Reads LU IN for information
C     Parameters determined by KEHFIT and THFIT
      TYPE 111
  111 FORMAT(
     &' CATHETER                  NUMBER'/
     &' 35    cm @  5.00 cc/min        1'/
     &' 35+10 cm @  5.00 cc/min        2')
      TYPE "(' ENTER CATHETER NUMBER')"
      READ (IN, "(I1)") N
      IF (N .LE. 0 .OR. N .GT. 2) THEN
        TYPE "(' ENTER T0, AK1, E')"
        READ (IN, "(3F10.4)") T, AK, E
        RETURN
      ENDIF
      TYPE "(' ENTER HEMATOCRIT (AS PERCENT)')"
      READ (IN, "(F5.0)") T
      HCT = T / 100.
      GOTO (1, 2), N
    1 T = 3.4124 - 3.4306 * (HCT - .3552)
      AK = 0.2919 - 0.5463 * (HCT - .3552)
      E = 0.0753 - 0.1621 * (HCT - .3552)
      RETURN
    2 T = 5.8971 - 3.2983 * (HCT - .3523)
      AK = 0.2095 - 0.1476 * (HCT - .3523)
      E = 0.0302 - 0.0869 * (HCT - .3523)
      RETURN
      END
