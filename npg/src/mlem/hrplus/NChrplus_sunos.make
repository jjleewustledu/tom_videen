# Program and library
LIBRARY_NAME = NChrplus_sunos
LIBRARY_DIR = .
LIBRARY = $(LIBRARY_DIR)/lib$(LIBRARY_NAME).a

# Include files and linked libraries 
INCLUDE_DIRS = -I. -I../ucl_matrix
LIBS_DIRS = 

# Compile command 
COMPILE_COMMAND = gcc

# Compile and link options
COMPILE_FLAGS = -O3
#COMPILE_FLAGS = -O3 -DSUN4
#COMPILE_FLAGS = -g -DSUN4
COMPILER_OPTIONS = $(INCLUDE_DIRS) $(COMPILE_FLAGS) -c 

# Sources 

LIBRARY_SOURCES =  NCmake_empty_imagev7.c NCwrite_imagev7.c NCreadnormv7.c \
               NCmatrix_write_plane.c NCread_mat_float.c NCwrite_scanv7.c \
               NCmake_empty_scanv7.c NCmake_empty_attenv7.c NCwrite_attenv7.c

# Dependency
LIBRARY_DEPENDENCY = 	$(LIBRARY)($(LIBRARY_SOURCES:.c=.o))

install:	all
	@echo done

all:	lib

lib:	$(LIBRARY)

- $(LIBRARY):	$(LIBRARY_DEPENDENCY)
	cat $(LIBRARY_SOURCES:.c=.h) > ./NChrplus.h
	ranlib $(LIBRARY)
	@echo Library $(LIBRARY) is now up to date.
	@echo


.c.a:
	@echo Compiling and archiving $*.c
	$(COMPILE_COMMAND) $(COMPILER_OPTIONS) $*.c -o $*.o
	ar rv $(LIBRARY) $*.o
	/bin/rm $*.o
	/usr/local/src/luci/install/LC_parser -h $*.c
	@echo


.c.o:
	@echo Compiling $*.c
	$(COMPILE_COMMAND) $(COMPILER_OPTIONS) $*.c -o $@
	@echo











