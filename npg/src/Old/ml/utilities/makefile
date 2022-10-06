#
pt6driver:	pt6driver.c
	cc -o pt6driver -O pt6driver.c -lm
	mv pt6driver /usr/local/pkg/ml/bin
#
em:	pt6driver
	ln -s pt6driver em
#
fbp:	pt6driver
	ln -s pt6driver fbp
#
pt6include.h:	../include/pt6include.f.h ftoc.awk ftoc.sed
#	expand -8 ../include/pt6include.f.h > expanded
#	nawk -f ftoc.awk expanded > awkward
#	sed -f ftoc.sed awkward >pt6include.h
#	rm expanded awkward
	expand -8 ../include/pt6include.f.h | \
	nawk -f ftoc.awk | \
	sed -f ftoc.sed > pt6include.h
#
pretty:	pt6driver.c
	cb <pt6driver.c >stuff
	expand -4 stuff >pt6driver.c
#
pt6intfloat:	pt6intfloat.c pt6include.h
	cc -o pt6intfloat -O pt6intfloat.c
	mv pt6intfloat ../bin
#
pt6gensurvprob:	pt6gensurvprob.c pt6include.h
	cc -o pt6gensurvprob -O pt6gensurvprob.c
	mv pt6gensurvprob ../bin
#
pt6mirimage_2.87mm:	pt6mirimage_2.87mm.c pt6include.h
	cc -o pt6mirimage_2.87mm -O pt6mirimage_2.87mm.c -lm
	mv pt6mirimage_2.87mm ../bin
#
pt6mirimage_2.00mm:	pt6mirimage_2.00mm.c pt6include.h
	cc -o pt6mirimage_2.00mm -O pt6mirimage_2.00mm.c -lm
	mv pt6mirimage_2.00mm ../bin
#
pt6precorfbp:	pt6precorfbp.c pt6include.h
	cc -o pt6precorfbp -O pt6precorfbp.c
	mv pt6precorfbp ../bin
#
pt6uniform:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_00_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6uniform pt6fio.o pt6stripspace.f \
	pt6uniform.f
	strip ../bin/pt6uniform
	rm -f pt6include.f pt6fio.o pt6stripspace.o pt6uniform.o
