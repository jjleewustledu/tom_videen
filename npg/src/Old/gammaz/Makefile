#		Makefile:	af3d_par
#		Author:		Tom Yang
#		Date:		27-Mar-95
#
SRC = gammaz.c 
OBJ = gammaz.o 
CC = cc -O -D_NO_PROTO

gammaz.bin: $(OBJ)
	$(CC) -o gammaz.bin $(OBJ) -lm

gammaz.o: gammaz.c
	$(CC) -c gammaz.c

release:
	cp gammaz.bin /usr/local/npg
	saversion /usr/local/npg/gammaz.bin Version

clean:
	rm $(OBJ)

checkout:
	co $(SRC)
