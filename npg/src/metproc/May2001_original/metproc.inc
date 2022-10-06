CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Common Block File:   METPROC.INC
C
C  Author:       Tom O. Videen
C  Date:         12-Jun-87
C  Written For:  METPROC
C
C  Common Blocks for METPROC program and subroutines.
C
C   **USRIO     - logical units for user I/O
C   **BLOOD     - blood points & constants;
C   **SCANS     - scan file variables;
C   **COMPUTE   - variables in integrations & convolutions;
C   **CONSTANT  - decay constants, physiological constants;
C   **VALUES    - values assigned to CONSTANT;
C
C  History:
C     TAUO15 changed from .005592 to .005568 31-Mar-88 by TOV.
C     TAUO15 changed from .005568 to .005668 10-Oct-88 by TOV,
C        as it should have been on 31-Mar-88.
C     Modified 23-Mar-89 by TOV to add SCANDATE and NORMMEAN.
C     Modified 12-May-92 by TOV for UNIX:
C        VOLUME changed to PATH
C        FNAME changed to IMGNAME
C        PATH, IMGNAME, PIENAME changed to character*256
C        EXTENSION changed to character*10
C     Modified May 2001 by TOV for steady-state oxygen:
C        HOB2P, OOB2P, OOPLASMA added
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
**USRIO
      INTEGER*4 USERIN,USEROUT

      COMMON /USRIO/ USERIN,USEROUT

C     USERIN = logical unit for user input;
C     USEROUT = logical unit for user output;

**BLOOD
      INTEGER*4  NUMPNTS,FILETYPE
      REAL*4     TIME(1000),ACTIVITY(1000),HEMATOCRIT,OXYCONT(2),
     &           PEAKTIME,FRACTIME,H2OFRAC,CORACTIV(1000)

      COMMON /BLOOD/ TIME,ACTIVITY,CORACTIV,HEMATOCRIT,OXYCONT,
     &               PEAKTIME,FRACTIME,H2OFRAC,NUMPNTS,FILETYPE

C     TIME = time of blood samples (sec)
C     ACTIVITY = activity of blood samples (counts/(sec*ml));
C        (with decay restored -- non-decay-corrected)
C     CORACTIV = activity of blood samples (counts/(sec*ml));
C        (decay-corrected)
C     HEMATOCRIT = hematocrit (percent);
C     OXYCONT = oxygen content of blood (ml/ml);
C        the first array element is for 7-slice images and for even
C        slices of 14-slice images; the second array element is for
C        odd slices of 14-slice images;
C     PEAKTIME = time of maximum activity in blood samples (sec);
C     FRACTIME = time at which blood was sampled for O2/H2O fraction (sec);
C     H2OFRAC = fraction of total O-15 counts attributed to H2O
C        at FRACTIME;
C     NUMPNTS = no. of points;
C     FILETYPE = version of DTA/DAT file (1-5);

**SCANS
      INTEGER*4    NTYPE,COUNT(7),SLICES,SCANTIME,RCONTYPE,RESOLUTION
      REAL*4       AWATER(2),BWATER(2),AOXYGEN(2),BOXYGEN(2),AFLOW(2),
     &             BFLOW(2),BVFACTOR(2),PETTCONV(2),O2CNTS(2),
     &             SCANST,SCANEND,SCANLEN,FRAMELEN,PEAKBANK,LAMBDA,
     &             TAU,TOTALCNTS,SCANCNTS,PIESLOPE,BPAIR,NORMMEAN,
     &             PIEMEAN, HOB2P, OOB2P, OOPLASMA
      CHARACTER*10 EXTENSION
      CHARACTER*4  INITIALS,SCANID
      CHARACTER*8  SUBJECT,SCANNAME,PIEDATE
      CHARACTER*10 COMPOUND, SCANDATE
      CHARACTER*12 FILTER
      CHARACTER*14 SCANNER
      CHARACTER*20 SCANTYPE(7)
      CHARACTER*50 HEADER
      CHARACTER*256 PATH,IMGNAME,PIENAME

      COMMON /SCANS/ NTYPE, COUNT, SLICES, SCANTIME, RCONTYPE,
     &    RESOLUTION, AWATER, BWATER, AOXYGEN, BOXYGEN, AFLOW,
     &    BFLOW, BVFACTOR, SCANST, SCANEND, SCANLEN, FRAMELEN,
     &    PEAKBANK, LAMBDA, TAU, TOTALCNTS, SCANCNTS, O2CNTS,
     &    PIESLOPE, BPAIR, NORMMEAN, PIEMEAN, PETTCONV,
     &    HOB2P, OOB2P, OOPLASMA,
     &    INITIALS, PATH, SCANID, SUBJECT, SCANNAME, SCANDATE,
     &    PIEDATE, COMPOUND, FILTER, SCANNER, IMGNAME, PIENAME,
     &    SCANTYPE, HEADER, EXTENSION

C     NTYPE = type of scan file;
C     COUNT = number of scan files processed of each type;
C     SLICES = number of slices in image file;
C     SCANTIME = length of scan (seconds) read from file header;
C     RCONTYPE = reconstruction type;
C     RESOLUTION: 0 = low; 1 = high resolution scan;

C  For the following, the first array element is for 7-slice images
C  and for even slices of 14-slice images.  The second array element
C  is for odd slices of 14-slice images.

C     AWATER,BWATER = coefficients of eqn:
C        Counts = AWATER*Flow*Flow + BWATER*Flow
C        Counts = tissue counts if 100% extraction of arterial O2;
C     AOXYGEN,BOXYGEN = coefficients of eqn:
C        Counts = AOXYGEN*Flow*Flow + BOXYGEN *Flow
C        Counts = tissue counts due to recirculating water of metabolism;
C     AFLOW,BFLOW = coefficients of eqn:
C        Flow = AFLOW*Counts*Counts + BFLOW*Counts
C     BVFACTOR = factor in eqn:
C        Regional CBV = BVFACTOR * Counts
C     O2CNTS = integral of blood oxygen counts from start of scan;
C     HOB2P = whole blood/plasma counts (following H2O bolus)
C     OOB2P = whole blood/plasma counts (following O2 inhalation)
C     OOPLASMA = plasma well counts/ml/sec following O2 inhalation)
C     PETTCONV = PETT conversion factor = PIESLOPE*60;
C        (well counts * pixel)/(PETT no * ml)

C     Units in the equations above:
C        Counts:  PETT counts/(pixel * scan period)
C        Flow = ml/(min*100 g)

C     SCANST = start time of scan (sec);
C     SCANEND = end time of scan (sec);
C     SCANLEN = scan length (sec);
C     FRAMELEN = frame length (non-zero for 14-slice scans);
C     PEAKBANK = peak bank pair observed during scan;
C     LAMBDA = partition coefficient (ml/ml);
C     TAU = decay constant used for specified file (1/sec);
C     TOTALCNTS = Integral of blood counts from time of injection;
C     SCANCNTS = Integral of blood counts from start of scan;
C        (non-decay-corrected counts)
C     PIESLOPE = pie slope used;
C        (well counts * pixel * min)/(PETT no * ml * sec)
C     BPAIR = bank pair used;
C     INITIALS = initials of user;
C     PATH = UNIX path to scan file;
C     EXTENSION = scan file extension;
C     SCANID = ID used at end of scan file name;
C     SUBJECT = subject ID;
C     SCANNAME = name of scan image file;
C     SCANDATE = date of scan (MM/DD/YYYY);
C     PIEDATE  = date of pie scans (MM/DD/YY);
C     COMPOUND = compound used in scan;
C     FILTER = reconstruction filter;
C     SCANNER = scanner name (PETT VI, SUPER PETT IIH, etc);
C     IMGNAME = scan file name;
C     PIENAME = name of PIE file;
C     SCANTYPE = type of scan;
C     NORMMEAN = mean NORM counts/bin/minute for scan
C     PIEMEAN  = mean NORM counts/bin/minute for pie scans
C     HEADER = header of DTA file;

**COMPUTE
      INTEGER*4 INUM
      REAL*4    IACTIV(1000),ITIME(1000),CONV(1000)

      COMMON /COMPUTE/ INUM,ITIME,IACTIV,CONV

C     INUM = number of values in ITIME and IACTIV;
C     ITIME = interpolated blood curve times (twice as many times);
C     IACTIV = interpolated blood curve activities (twice as many);
C     CONV = convolution integral of IACTIV * EXP(-A*ITIME)

**CONSTANT
      REAL*4 TAUO15,TAUC11,TAUGA68,TAUF18,HOLAMBDA,BULAMBDA,
     &       BRAINDEN,BLOODDEN,SLVRATIO,RBCFACTOR,TAUO1501,
     &       TAUO1502,OLDO15,TAUO1500

      COMMON /CONSTANT/ TAUO15,TAUC11,TAUGA68,TAUF18,HOLAMBDA,
     &        BULAMBDA,BRAINDEN,BLOODDEN,SLVRATIO,RBCFACTOR,
     &        TAUO1501,TAUO1502,OLDO15,TAUO1500

C     TAUO15  - decay constant for O15 (1/sec);
C     TAUO1500 - decay constant used in type '@ @@' DTA files for O15;
C     TAUO1501 - decay constant used in type '@01@' DTA files for O15;
C     TAUO1502 - decay constant used in type '@02@' DTA files for O15;
C     OLDO15  - decay constant used in BLOOD FOR O15;
C     TAUC11  - decay constant for C11 (1/sec);
C     TAUGA68 - decay constant for GA68(1/sec);
C     TAUF18  - decay constant for F18 (1/sec);
C     HOLAMBDA - partition coefficient for water (ml blood/g brain)
C     BULAMBDA - partition coefficient for butanol (ml blood/g brain)
C     BRAINDEN - density of brain (g/ml)
C     BLOODDEN - density of blood (g/ml)
C     SLVRATIO - ratio of cerebral small vessel hematocrit to
C        large vessel hematocrit;
C     RBCFACTOR = water content of rbc/water content of plasma;
C        (used by O2UTIL.FTN to compute water fraction of blood counts)

C                   66 g water         g rbc
C                  -----------  * 1.10 -----
C                   100 g rbc          ml rbc
C     RBCFACTOR = -------------------------------  = 0.766
C                   92 g water         g plasma
C                  -----------  * 1.03 --------
C                  100 g plasma        ml plasma

