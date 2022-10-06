all: pt6ml_2.87mm pt6ml_2.00mm pt6sieve_2.87mm pt6sieve_2.00mm \
	pt6sievecirc_2.87mm pt6sievecirc_2.00mm


pt6ml_2.87mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_87_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6ml_2.87mm pt6adddarr.f pt6backproj.f \
	pt6circzero.f pt6conv1d.f pt6divdarr.f pt6fio.o pt6forwproj.f \
	pt6genkern.f pt6maskml.f pt6ml.f pt6mult3img.f pt6multdarr.f \
	pt6normarea.f pt6paddata.f pt6padimg.f pt6stripspace.f \
	pt6unpadimgorig.f
	strip ../bin/pt6ml_2.87mm
	rm -f pt6include.f *.u *.o

pt6ml_2.00mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_00_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6ml_2.00mm pt6adddarr.f pt6backproj.f \
	pt6circzero.f pt6conv1d.f pt6divdarr.f pt6fio.o pt6forwproj.f \
	pt6genkern.f pt6maskml.f pt6ml.f pt6mult3img.f pt6multdarr.f \
	pt6normarea.f pt6paddata.f pt6padimg.f pt6stripspace.f \
	pt6unpadimgorig.f
	strip ../bin/pt6ml_2.00mm
	rm -f pt6include.f *.u *.o

pt6sieve_2.87mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_87_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6sieve_2.87mm pt6fio.o pt6genkern.f \
	pt6normarea.f pt6padimg.f pt6sieve.f pt6stripspace.f \
	pt6unpadimgfin.f
	strip ../bin/pt6sieve_2.87mm
	rm -f pt6include.f *.u *.o

pt6sieve_2.00mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_00_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6sieve_2.00mm pt6fio.o pt6genkern.f \
	pt6normarea.f pt6padimg.f pt6sieve.f pt6stripspace.f \
	pt6unpadimgfin.f
	strip ../bin/pt6sieve_2.00mm
	rm -f pt6include.f *.u *.o

pt6sievecirc_2.87mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_87_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6sievecirc_2.87mm pt6fio.o \
	pt6genkern.f pt6normarea.f pt6padimg.f pt6sievecirc.f pt6stripspace.f \
	pt6unpadimgfin.f
	strip ../bin/pt6sievecirc_2.87mm
	rm -f pt6include.f *.u *.o

pt6sievecirc_2.00mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_00_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6sievecirc_2.00mm pt6fio.o \
	pt6genkern.f pt6normarea.f pt6padimg.f pt6sievecirc.f pt6stripspace.f \
	pt6unpadimgfin.f
	strip ../bin/pt6sievecirc_2.00mm
	rm -f pt6include.f *.u *.o
