#	Make file for getpaths
#	Fred Kuhns
#	02/10/93

SYBLIBDIR = $(SYBTOOLS)/lib
INCDIR   = $(SYBTOOLS)/include 
HEADERS  = $(INCDIR)/syberror.h $(INCDIR)/sybfront.h $(INCDIR)/sybdb.h /usr/local/include/arc.h /usr/local/include/riis/syb.h /usr/local/include/riis/riisarc.h /usr/local/include/riis/riisd.h /usr/local/npg/include/arcfind.h

INCDIRS  = -I$(INCDIR) -I/usr/local/npg/include -I/usr/local/include 
LIBDIRS = -L$(SYBLIBDIR) -L/usr/local/lib

SRC = allfiles.c getpaths.c util.c
OBJ = $(SRC:.c=.o)
LIBS = -lriis -larc $(XLIBS) -lsybdb

CC = cc $(CFLAGS) $(INCDIRS) $(LIBDIRS)

.c.o: $(HEADERS) 
	$(CC) -c $<

svr4:
	@$(MAKE) getpaths XFLAGS="-DSVR4" SYBTOOLS="/usr/local/pkg/sybtools" \
	RANLIB="echo" XLIBS="-lsocket -lnsl"

bsd:
	@$(MAKE) getpaths XFLAGS="-DSOS4" SYBTOOLS="/usr/local/pkg/sybtools.413"
	RANLIB="ranlib" XLIBS=""

getpaths : $(OBJ) $(HEADERS)
	$(CC) -o $@ $(OBJ) $(LIBS)

clean:
	rm -f *.o getpaths
