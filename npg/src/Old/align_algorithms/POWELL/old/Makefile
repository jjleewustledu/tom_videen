#	Makefile:	alignbyfids
#	Author:		Tom Videen
#	Date		20-Jul-95

LIB = /usr/local/lib
INC = /usr/local/include
CC = cc -g -L${LIB} -I${INC}
CSRCS = alignbyfids.c matdist.c matcreate.c vector_dist.c
OBJS =  ${CSRCS:.c=.o}

alignbyfids.bin: ${OBJS}
	${CC} -o $@ ${OBJS} -lm -lmatrix -lnrc

.c.o:
	${CC} -c $<

checkout:
	co ${CSRCS} 

release:
	chmod 775 alignbyfids.bin
	mv alignbyfids.bin /usr/local/npg/alignbyfids.bin
	saversion /usr/local/npg/alignbyfids.bin Version

clean:
	rm ${OBJS}
