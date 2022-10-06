CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 
C
C  Program:      SLEPAL.FTN
C  Author:       Tom O. Videen
C  Date:         Dec-99
C  Written For:  PETT VI Image Reconstruction                                 
C  History/Revisions:                                                         
C	Attempt to recreate original SLEPAL.FTN written for PETT VI 3/23/81
C		based on SLICESEP.FTN.
C     Original SLEPAL.FTN written for PETT VI 3/23/81 by Mark Mintun.         
C  Logical Units:                                                             
C     0 = User output (USEROUT);                                               
C     1 = Raw data (NORM, ATTENn, SCANn) input file (IN);                     
C     2 = Raw data separated into slices (SLICES) output file (OUT);        
C                                                                             
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC             
C
      PROGRAM SLEPAL

      INTEGER*2   ARRAY(8,8,16,18,7) ! data sorted by slice (for output)      
C                     8 = detectors in 2nd bank of pair                       
C                     8 = detectors in 1st bank of pair                       
C                    16 = wobble positions                                     
C                    18 = bank pair no.                                        
C                     7 = slice no.                                            
      INTEGER*2   PAIR(8,8,16,16) ! raw data for 1 bank pair of 16 slices    
C                     8 = detectors in 2nd bank of pair                       
C                     8 = detectors in 1st bank of pair                        
C                    16 = wobble positions                                     
C                    16 = slice no. (only first 7 are significant)            
      INTEGER*2   HEADER(128)  ! file header record (read from raw file,       
C                              !   altered and written to output file)        
      INTEGER*2   RAND         ! temporary value for random/scan/bin           
      REAL*4      TEMP         ! temporary value for random/scan/bin         

      INTEGER*4   ETOTAL       ! total raw events                             
      INTEGER*4   RTOTAL       ! total random events                          
      INTEGER*4   TTOTAL       ! total true events (raw - random)             
      INTEGER*4   COUNTS       ! temporary value for ARRAY()                  
      INTEGER*4   PB(6)        ! parameter block in SYSIO                     
      INTEGER*4   IOREAD       ! SYSIO read sequential binary wait            
      INTEGER*4   IOWRITE      ! SYSIO write sequential binary wait           
      INTEGER*4   IN           ! log unit for input file (raw data)           
      INTEGER*4   OUT          ! log unit for output file (slices separated)   
      INTEGER*4   USEROUT      ! log unit for output to user                   
      INTEGER*4   NBYTES       ! no. of bytes per slice                        

      INTEGER*4   NUMEVENTS(7) ! total events for each slice (raw data)       
      INTEGER*4   SUMEVENTS    ! total events ALL slices                      

C      (det-det bin includes all 16 wobble positions)                          

      REAL*4      OUTLIERS(7)  ! mean outliers per det-det bin for each slice
      INTEGER*4   NOUTLIERS(7) ! total outlying coincidences for each slice  

      REAL*4      RANDOMS(7)   ! mean randoms per det-det bin for each slice 
      INTEGER*4   NRANDOMS(7)  ! total randoms for each slice               
      INTEGER*4   SUMRANDOM    ! total randoms ALL slices               

      REAL*4      SINGLES(7)   ! mean singles per det-det bin for each slice 

      INTEGER*4   TRUES(7)     ! total events - total randoms              
      INTEGER*4   SUMTRUES     ! total events-randoms ALL slices              

      INTEGER*4   RESOLUTION   ! image resolution (0=low; 1=high)             
      INTEGER*4   BP           ! bank pair number (1-18)                   
      INTEGER*4   D1           ! detector number of 1st bank (1-8)         
      INTEGER*4   D2           ! detector number of 2nd bank (1-8)            
      INTEGER*4   WOB          ! wobble position (1-16)                   
      INTEGER*4   SLICE        ! slice number (1-7)                        
      INTEGER*4   I,J          ! local integers                               

      REAL*4      TIME         ! scan time (seconds)                         
      REAL*4      TAU          ! PET system coincidence window (nsec)       

      CHARACTER*1 BELL         ! used to ring terminal bell                  
      CHARACTER*3 RUNNAME      ! first 3 characters of run name
      CHARACTER*20 SCANNAME,SLICESNAME,ATENNAME,DETNAME  ! file names input as command line args
      EQUIVALENCE (RUNNAME, HEADER(2))                                    
      EQUIVALENCE (ETOTAL, HEADER(51))                                       
      EQUIVALENCE (RTOTAL, HEADER(53))                                      
      EQUIVALENCE (TTOTAL, HEADER(55))                                      

C  Initializations                                                         

      DATA IOREAD, IOWRITE  /88,56/                                         
      DATA USEROUT, IN, OUT /6,0,1/       
      DATA TAU /6.0E-09/                                               

      WRITE(USEROUT,*) '*** SLEPAL  Dec-99  {slice separation & randoms subtraction}'
      CALL GETARG (1, SCANNAME)
      CALL GETARG (2, SLICESNAME)

      PB(6) = 0                                                      
      BELL = CHAR(7)                                                    
      ADDRESS = 0            ! avoid compiler warning                    
      DO 20 SLICE=1,7                                                      
        TRUES(SLICE)     = 0                                               
        NOUTLIERS(SLICE) = 0                                              
        NRANDOMS(SLICE)  = 0                                         
        NUMEVENTS(SLICE) = 0                                                
        OUTLIERS(SLICE)  = 0.0                                         
        RANDOMS(SLICE)   = 0.0                                            
        SINGLES(SLICE)   = 0.0                                              
20    CONTINUE

C  Added the following open statements when porting to HP 4-29-91 KMG
      OPEN(IN,FILE=SCANNAME)
      OPEN(OUT,FILE=SLICESNAME)
      GOTO 22
21    WRITE(6,*) 'error'

C  Read header and check for rotation (which should never occur)          

22    CALL SYSIO(PB,IOREAD,IN,HEADER,256,0)                                 
      I = INT(HEADER(5))                                                     
      IF (I.NE.0) THEN                                                       
        WRITE(USEROUT,*) '********************************************'   
        WRITE(USEROUT,*) '*  File header indicates rotation was used.'         
        WRITE(USEROUT,*) '*  This program will NOT process raw data'      
        WRITE(USEROUT,*) '*    files in which PETT VI was rotated.'          
        WRITE(USEROUT,*) '********************************************'  
        STOP                                                                  
      END IF                                                                 

C  Check image resolution  (0=low, 1=high)                                   

      RESOLUTION = HEADER(16)      ! get resolution (0 or 1)             
      TIME = FLOAT(HEADER(8))      ! get scan time (sec)                   

C -----------------------------------------------------------          
C  Sort and count randoms in each bank pair                             

      NBYTES = 2*16*16*8*8   ! bytes per bank pair               
      DO 400 BP=1,18         ! 18 bank pairs                              

C  Read all coincidence pairs for this pair of banks                    

        CALL SYSIO(PB,IOREAD,IN,PAIR,NBYTES,0)
        DO 300 SLICE=1,7                                               

C  FLAG=1: outermost pair is D1=8, D2=1                                    

          IF (BP.EQ.1 .or. BP.EQ.5 .or. BP.EQ.9 .or. BP.EQ.13                
     &     .or. BP.EQ.16 .or. BP.EQ.18) THEN                                  
            DO 140 D1=5,8                                                   
              DO 130 D2=1,4                                                  
                DO 120 WOB=1,16                                             
                  NOUTLIERS(SLICE) =                                          
     &              NOUTLIERS(SLICE) + PAIR(D2,D1,WOB,SLICE)                 
120             CONTINUE                                                   
130           CONTINUE                                                     
140         CONTINUE                                                       

C  FLAG=2: outermost pair is D1=1, D2=8                                      

          ELSE IF (BP.EQ.4 .or. BP.EQ.8 .or. BP.EQ.12) THEN                 
            DO 180 D1=1,4                                                  
              DO 170 D2=5,8                                                 
                DO 160 WOB=1,16                                            
                  NOUTLIERS(SLICE) =                                           
     &              NOUTLIERS(SLICE) + PAIR(D2,D1,WOB,SLICE)               
160             CONTINUE                                                    
170           CONTINUE                                                      
180         CONTINUE                                                        
          END IF                                                              

C  Sum total coincidences detected for this slice and                     
C  Move raw data from bank pair array to slice-sorted array               

          DO 230 WOB=1,16                                                
            DO 220 D1=1,8                                                    
              DO 210 D2=1,8                                                  
                NUMEVENTS(SLICE) =                                             
     &            NUMEVENTS(SLICE) + PAIR(D2,D1,WOB,SLICE)                  
                ARRAY(D2,D1,WOB,BP,SLICE) = PAIR(D2,D1,WOB,SLICE)          
210           CONTINUE                                                    
220         CONTINUE                                                   
230       CONTINUE                                                      
300     CONTINUE       ! end of slice loop                               
400   CONTINUE         ! end of bank pair loop                     

C -----------------------------------------------------------                  
C  Compute singles and randoms for each slice.                             
C     144 =  9 Bank Pairs * 16 Det-Det Pairs (for outliers)              
C    1152 = 18 Bank Pairs * 64 Det-Det Pairs (for entire slice)            

      DO 420 I=1,7                                                         
        NRANDOMS(I) = JNINT(NOUTLIERS(I)/432)
420   CONTINUE                                                             

C -----------------------------------------------------------              
C  Subtract randoms from each detector-detector pair.                   

      DO 500 SLICE=1,7                                                    
        TEMP = RANDOMS(SLICE)*TIME/16.0                                     
        DO 470 BP=1,18                                                        
          DO 460 D1=1,8                                                    
            DO 450 D2=1,8
C Changed from INT2 to IINT when porting to HP  KMG 4-16-91           
C              RAND = INT2(NINT(TEMP*SENSITIVITY(D2,D1,BP,SLICE)))         
              RAND = ININT(TEMP*SENSITIVITY(D2,D1,BP,SLICE))           
              DO 440 WOB=1,16                                              
                COUNTS = ARRAY(D2,D1,WOB,BP,SLICE) - RAND                   
                IF (COUNTS.LT.0) COUNTS=0         ! no negative counts
                ARRAY(D2,D1,WOB,BP,SLICE) = (COUNTS)
                TRUES(SLICE) = TRUES(SLICE) + COUNTS                  
440           CONTINUE                                                        
450         CONTINUE                                                     
460       CONTINUE                                                        
470     CONTINUE                                                           
500   CONTINUE                                                             

C -----------------------------------------------------------               
C  Write separated slices (1-7) to output file 1 slice at a time        

      NBYTES = 2*18*16*8*8        ! bytes per slice                        
      HEADER(38) = 20             ! file type                  
      DO 600 SLICE=1,7                                                       
        HEADER(18) = SLICE                                                  
        ETOTAL = NUMEVENTS(SLICE)                                          
        RTOTAL = NRANDOMS(SLICE)                                          
        TTOTAL = TRUES(SLICE)                                             
        CALL SYSIO(PB,IOWRITE,OUT,HEADER,256,0)                          
        CALL SYSIO(PB,IOWRITE,OUT,ARRAY(1,1,1,1,SLICE),NBYTES,0)        
600   CONTINUE                                                            

C  Write out total events and total minus randoms               

      WRITE(USEROUT,8300)                                                     
      WRITE(USEROUT,8310)                                                   
      SUMEVENTS = 0                                                       
      SUMRANDOM = 0                                                      
      SUMTRUES  = 0                                                        
      DO 700 I=1,7                                                        
        WRITE(USEROUT,8320) I, NUMEVENTS(I), NRANDOMS(I), TRUES(I),        
     &    100.0*FLOAT(NRANDOMS(I))/FLOAT(NUMEVENTS(I)),                   
     &    NINT(RANDOMS(I)*TIME/16.0),                                     
     &    NINT(SINGLES(I)), NINT(OUTLIERS(I)*TIME),                      
     &    RANDOMS(I)/OUTLIERS(I)                                              
        SUMEVENTS = SUMEVENTS + NUMEVENTS(I)                                  
        SUMRANDOM = SUMRANDOM + NRANDOMS(I)                                 
        SUMTRUES  = SUMTRUES + TRUES(I)                                   
700   CONTINUE                                                            
      WRITE(USEROUT,*) ' '                                             
      WRITE(USEROUT,8330) SUMEVENTS, SUMRANDOM, SUMTRUES,                
     &    100.0*FLOAT(SUMRANDOM)/FLOAT(SUMEVENTS)                          

8300  FORMAT(1X,'Slice',4X,'Total ',4X,' Total ',5X,'Events-',3X,            
     &   ' %  ',2X,'Randoms',2X,'Singles',2X,'Outliers',2X,'Frac')      
8310  FORMAT(1X,' No. ',4X,'Events',4X,'Randoms',5X,'Randoms',3X,              
     &   'Rand',2X,'Per Bin',2X,'Per Sec',2X,'Per Scan',2X,'Subt')             
8320  FORMAT(1X,I3,I12,I11,I12,F7.1,I7,I10,I9,F8.2)                         
8330  FORMAT(1X,'SUM',I12,I11,I12,F7.1)                                      

      CLOSE(ALPHAFILE)
      CLOSE(ATENSIZE)
      CLOSE(OUT)
      STOP                                                                
      END                                                                  
