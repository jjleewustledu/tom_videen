#$Header$
#$Log$
#
#   Makefile: genreal makefile for 3D reconstruction programs.
#   Author:   Tom Yang
#   Date:     19-Mar-93
#
#   Directions for checking out code and building programs
#   1. cp this Makefile to a directory.
#   2. % make install
#   3. % make all
#
SRCPATH         = .
BINPATH         = ${SRCPATH}/bin
RELEASEPATH     = /usr/local/bin
MINELIB_SRCPATH = ${SRCPATH}/mine
EMULIB_SRCPATH  = ${SRCPATH}/emu
RECON3D_SRCPATH = ${SRCPATH}/3d
RECON3D_RCSPATH = /usr/local/src/3drecon
CHECK_SOURCE    = co Makefile; make checkout

MINE_LIBRARY    = $(LIBRARYPATH)/libmine.a
EMU_LIBRARY     = $(LIBRARYPATH)/libcsemu.a

all: $(MINE_LIBRARY) $(EMU_LIBRARY) 3d_programs

$(MINE_LIBRARY):
	cd $(MINELIB_SRCPATH); $(MAKE)

$(EMU_LIBRARY):
	cd $(EMULIB_SRCPATH); $(MAKE)

3d_programs:
	cd $(RECON3D_SRCPATH); $(MAKE)

release:
	cp ${BINPATH}/* ${RELEASEPATH}

install:
	mkdir mine; cd mine; ln -s ${RECON3D_RCSPATH}/mine RCS; ${CHECK_SOURCE}
	mkdir emu; cd emu; ln -s ${RECON3D_RCSPATH}/emu RCS; ${CHECK_SOURCE}
	mkdir 3d; cd 3d; ln -s ${RECON3D_RCSPATH}/3d RCS; ${CHECK_SOURCE}
	mkdir include; cd include; ln -s ${RECON3D_RCSPATH}/include RCS; ${CHECK_SOURCE}
	mkdir bin

clean: 
	cd $(MINELIB_SRCPATH); $(MAKE) clean
	cd $(EMULIB_SRCPATH); $(MAKE) clean
	cd $(RECON3D_SRCPATH); $(MAKE) clean
	yes | rm $(LIBRARYPATH)/libmine.a
	yes | rm $(LIBRARYPATH)/libcsemu.a

clear:
	/bin/rm -rf $(MINELIB_SRCPATH)
	/bin/rm -rf $(EMULIB_SRCPATH)
	/bin/rm -rf $(RECON3D_SRCPATH)
	/bin/rm -rf $(LIBRARYPATH)
	/bin/rm -rf $(BINPATH)
	/bin/rm -rf include
