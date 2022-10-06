# $Header: /export/home/npggw/tom/src/recon3d/RCS/Makefile,v 1.5 1995/01/25 15:35:31 ty7777 Exp $
# $Log: Makefile,v $
# Revision 1.5  1995/01/25  15:35:31  ty7777
# Use -O to build the emu library.
#
# Revision 1.4  1993/03/19  23:06:03  ty7777
# Add "yes" to rm.
#
# Revision 1.3  1993/03/19  22:24:13  ty7777
# added ranlib to make it work on Sun.
#
# Revision 1.2  1993/03/19  21:45:07  ty7777
# New Makefile.
#
# Revision 1.1  1993/03/18  15:49:08  ty7777
# Initial revision
#
#
#   Makefile: libcsemu.a (CSPI emulation library for HP)
#   Authors:  Tom Yang and Tom Videen
#   Date:     03-Feb-93
#	      Modified by Tom Yang on March 19, 1993 to make it work with genreal Makefile.
#

SRCPATH = ..
INCLUDE = ${SRCPATH}/include
CFLAGS = -O -I${INCLUDE}
BINPATH = ${SRCPATH}/include
VPATH = ${INCLUDE}
.SUFFIXES: .c .o

OBJS = sc_3dbkp.o \
       sc_bkp3d.o \
       sc_fp3d.o \
       sc_obview.o \
       xbp.o \
       xccmul.o \
       xccvml.o \
       xcjmul.o \
       xcrdot.o \
       xcrdtc.o \
       xcrma1.o \
       xcrma2.o \
       xcrmai.o \
       xcrml.o \
       xcrmul.o \
       xcsadd.o \
       xcssub.o \
       xcvadd.o \
       xcvcnj.o \
       xcvmls.o \
       xcvmov.o \
       xcvsbx.o \
       xcvsub.o \
       xdcor.o \
       xdcord.o \
       xdcori.o \
       xdcork.o \
       xdfl22.o \
       xdintg.o \
       xfcf.o \
       xfci.o \
       xfrf.o \
       xfri.o \
       xgints.o \
       xmovk.o \
       xrdot.o \
       xreqs.o \
       xrges.o \
       xrgts.o \
       xrles.o \
       xrlts.o \
       xrmn.o \
       xrmx.o \
       xrnes.o \
       xrsma.o \
       xrsmsq.o \
       xrsum.o \
       xsadd.o \
       xsdiv.o \
       xsfl4.o \
       xsfx4.o \
       xsmul.o \
       xssub.o \
       xvab.o \
       xvad.o \
       xvaln.o \
       xvatn2.o \
       xvclip.o \
       xvclpl.o \
       xvclpu.o \
       xvclr.o \
       xvcos.o \
       xvcosr.o \
       xvdiv.o \
       xvfl2.o \
       xvfl2c.o \
       xvfl4.o \
       xvfx2.o \
       xvfx2c.o \
       xvfx4.o \
       xvln.o \
       xvmgtf.o \
       xvmls.o \
       xvmov.o \
       xvmovi.o \
       xvmvgt.o \
       xvmvic.o \
       xvneg.o \
       xvpow.o \
       xvrcp.o \
       xvrmp.o \
       xvsad.o \
       xvsb.o \
       xvsbx.o \
       xvsin.o \
       xvsinp.o \
       xvsinr.o \
       xvsm2a.o \
       xvsma1.o \
       xvsma2.o \
       xvsml.o \
       xvsq.o \
       xvsqrt.o \
       xvtabi.o

CSRCS= ${OBJS:.o=.c} 

libcsemu.a : ${OBJS}
	ar rv ${BINPATH}/$@ $?
	ranlib ${BINPATH}/$@ 

.c.o :
	cc ${CFLAGS} -c $<

checkout :
	co ${CSRCS}

clean :
	yes | rm ${OBJS}
