      SUBROUTINE SYSIO (PBLK, FC, LU, START, NBYTES, RANADD)
    
      INTEGER*4    PBLK(6)    ! OS/32 SVC Parameter Block
      INTEGER*4    FC         ! I/O Function Code 
      INTEGER*4    LU         ! FORTRAN Logical Unit Number 
      CHARACTER*(*) START      ! Address of I/O Buffer
      INTEGER*4    NBYTES     ! Number of Bytes to Transfer
      INTEGER*4    RANADD     ! Logical Record Number for Random Access 

      INTEGER*4    recl       ! record length 
      INTEGER*4    fd         ! HPUX file descriptor
      INTEGER*4    result     ! result code for HPUX operations
      INTEGER*4    offset     ! HPUX file offset
      INTEGER*4    relative   
      INTEGER*4    IOREAD
      INTEGER*4    IOWRITE 
      INTEGER*4    IOREADR
      INTEGER*4    IOWRITER
      INTEGER*4    getfd 

      DATA  IOREAD   /88/
      DATA  IOWRITE  /56/
      DATA  IOREADR  /92/
      DATA  IOWRITER /60/


      INQUIRE (LU, RECL = recl)		! get the record length as opened
      IF (recl .EQ. 0) THEN		! if the record length is zero then
	recl	= 256			!   it was probably a contiguous file
      ENDIF

      fd	= getfd (LU)		! get the HPUX file descriptor

      relative	= 0			! set the relative position for the offset
					!  to the beginning of the file

      IF (FC .EQ. IOREADR) THEN 
	offset	= RANADD * recl
	result	= CLSEEK (fd, offset, relative)
	result	= CREAD (fd, START, NBYTES)
      ENDIF

      IF (FC .EQ. IOWRITER) THEN 
	offset	= RANADD * recl
	result	= CLSEEK (fd, offset, relative)
	result	= CWRITE (fd, START, NBYTES)
      ENDIF

      IF (FC .EQ. IOREAD) THEN 
	result	= CREAD (fd, START, NBYTES)
      ENDIF

      IF (FC .EQ. IOWRITE) THEN 
	result	= CWRITE (fd, START, NBYTES)
      ENDIF

      RETURN
      END
