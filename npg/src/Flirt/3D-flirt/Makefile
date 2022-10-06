FLIRT3D     = /home/npggw/tom/src/Flirt/3D-flirt
RELEASEPATH = ${FLIRT3D}/lib
SRCFLIRT    = ${FLIRT3D}/flirt
SRCFLIRT++  = ${FLIRT3D}/flirt++
LIBFLIRT    = ${RELEASEPATH}/libflirt.a
LIBFLIRT++  = ${RELEASEPATH}/libflirt++.a

all: ${LIBFLIRT} ${LIBFLIRT++}

${LIBFLIRT}: 
	cd ${SRCFLIRT};   make release

${LIBFLIRT++}: ${LIBFLIRT} 
	cd ${SRCFLIRT++}; make release

checkout:
	cd ${SRCFLIRT}; co Makefile; make checkout
	cd ${SRCFLIRT++}; co Makefile; make checkout

clean:
	cd ${SRCFLIRT};   make clean
	cd ${SRCFLIRT++}; make clean

cleanall:
	cd ${RELEASEPATH}; rm ${LIBFLIRT} ${LIBFLIRT++}

