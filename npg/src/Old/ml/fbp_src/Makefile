all: pt6fbp_2.87mm pt6fbp_2.00mm


pt6fbp_2.87mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_87_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6fbp_2.87mm pt6backproj.f pt6fbp.f \
	pt6filgen1d.f pt6filt1d.f pt6fio.o pt6maskfbp.f pt6paddata.f \
	pt6stripspace.f pt6unpadimgfin.f
	strip ../bin/pt6fbp_2.87mm
	rm -f pt6include.f *.u *.o

pt6fbp_2.00mm:
	cc -c -O pt6fio.c
	/lib/cpp -P -DTWO_00_MM ../include/pt6include.f.h > pt6include.f
	f77 -u -O -o ../bin/pt6fbp_2.00mm pt6backproj.f pt6fbp.f \
	pt6filgen1d.f pt6filt1d.f pt6fio.o pt6maskfbp.f pt6paddata.f \
	pt6stripspace.f pt6unpadimgfin.f
	strip ../bin/pt6fbp_2.00mm
	rm -f pt6include.f *.u *.o
