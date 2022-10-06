F....T..T................................................................M......
                     OPROFILE Programs                      15-Jun-87

0.   Linking files
        OPROFILE.CMP  (compiling file)
        OPROFILE.LNK  (linking file)

I.   Main Program
        OPROFILE.CSS
        OPROFILE.FTN

II.  Plot related subroutines
        OAXES    - draw and label axes on terminal;
        OFILE    - write values to a disk file;
        OPLOT    - main plotting subroutine;
        OPLOTDAT - plot data values;
        OPLOTYPE - select plot type parameters (dots,lines);

III. Mathematical subroutines
        BETAI
        FCTOFP
        GAMMA
        QSF
        RTNI

IV.  Mathematical functions
        BETADENS
        EXPTRAP - computes exponent trapping over- and underflow error;
        POFSIGMA
        SIGMAOFP

V.   GRAPHON.LIB library of subroutines
        GOCHSIZE - select character size (1-4 where 1 is largest);
        GOCINT   - write an integer (centered);
        GOCREAL  - write a real no.(centered);
        GOEXIT   - exit graphics mode;
        GOINIT   - enter graphics mode;
        GOLINE   - plot a line (screen coordinates);
        GOLTYPE  - select the line type;
        GOPOINT  - plot a point (screen coordinates);
        GORESET  - reset the graphics window;
        GORINT   - write an integer (right-justified);
        GORREAL  - write a real no.(right-justified);
        GOSCALE  - set common block scaling variables;
        GOSCINT  - write an integer (centered; scaled);
        GOSCREAL - write a real no.(centered; scaled);
        GOSGIN   - scaled graphics cursor;
        GOSLINE  - plot a line (scaled coordinates);
        GOSPOINT - plot a point (automatically scaled);
        GOSPLOT  - plot a point (draws a line if called sequentially
           without leaving the graphics mode);
        GOSRINT  - write an integer (right-justified; scaled);
        GOSRREAL - write a real no.(right-justified; scaled);
        GOTEXT   - writes text in graphics mode;
        GOTEXTA  - writes text in graphics mode (string must end with
           a tilde;  used to erase text by writing blanks);
        GOWAITCR - wait for user to enter a key (while in graphics mode);
           (either exits or calls GOSGIN)
.PP=0001====================L/P=60/FF          .S   1.00 .C .~..........=======

VI.  TERMINAL.LIB library of subroutines
        TERMSET    (uses SYS:IDV/S and SYS:TPARAM/S)
        TCONTROL - terminal control functions;
        GETCHAR  - get a character;
        GETINT   - get an integer;
        GETREAL  - get a real no.;
        GTSTRING - get a string;
        YESNO    - get a yes/no response;
