#	Makefile:	testfmat
#	Author:		Tom Videen
#	Date		20-Jul-95

LIB = /usr/local/lib
INC = /usr/local/include
CC = cc -g -L${LIB} -I${INC}
CSRCS = testf.c
OBJS =  ${CSRCS:.c=.o}

testf: ${OBJS}
	${CC} -o $@ ${OBJS} -lm -lnrc -lmatrix

.c.o:
	${CC} -c $<

clean:
	rm ${OBJS}
