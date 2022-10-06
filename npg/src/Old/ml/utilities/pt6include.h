/************************************************************************ */
/* */
/*    PROGRAM TITLE              pt6include.f.h */
/* */
/*    WRITTEN BY                 David G. Politte */
/*    DATE WRITTEN               July 16, 1991 */
/*    WRITTEN FOR                PETT VI image reconstruction */
/* */
/* */
/*    PROGRAM INTENT */
/*            This include file contains constants that describe */
/* */
/*       a) the sampling and array sizes of the images, */
/*       b) the sampling and array sizes of the data, */
/*       c) the transverse measurement error of the scanner, */
/*       d) the size in bytes of a real or integer datum, */
/*       e) the size of the masks used to minimize the number of */
/*          computations, */
/*       f) the number of bytes in the initial informational header. */
/* */
/************************************************************************ */




/*    x, the horizontal coordinate of an image, increases from left to */
/*    right and is assigned an index from xindexmin to xindexmax, */
/*    inclusive.  The geometric center of the scanner, xindexcent, is at */
/*    the boundary between two pixels.  The first four parameters below */
/*    describe the padded image in which most of the computations are */
/*    done.  The "orig" and "fin" parameters describe images that are */
/*    read or written by various programs. */

#ifdef TWO_87_MM
#define xindexmin         0  
#define xindexmax       127  
#define numx            128  
#define xindexcent       63.5

#define xindexminorig     0  
#define xindexmaxorig    99  
#define numxorig        100  
#define xindexcentorig   49.5

#define xindexminfin      0  
#define xindexmaxfin     99  
#define numxfin         100  
#define xindexcentfin    49.5
#endif

#ifdef TWO_00_MM
#define xindexmin         0  
#define xindexmax       183  
#define numx            184  
#define xindexcent       91.5

#define xindexminorig     0  
#define xindexmaxorig   143  
#define numxorig        144  
#define xindexcentorig   71.5

#define xindexminfin      0  
#define xindexmaxfin    127  
#define numxfin         128  
#define xindexcentfin    63.5
#endif

/*    y, the vertical coordinate of an image, increases from top to */
/*    bottom and is assigned an index from yindexmin to yindexmax, */
/*    inclusive.  The geometric center of the scanner, yindexcent, is at */
/*    the boundary between two pixels.  The first four parameters below */
/*    describe the padded image in which most of the computations are */
/*    done.  The "orig" and "fin" parameters describe images that are */
/*    read or written by various programs. */

#ifdef TWO_87_MM
#define yindexmin         0  
#define yindexmax       127  
#define numy            128  
#define yindexcent       63.5

#define yindexminorig     0  
#define yindexmaxorig    99  
#define numyorig        100  
#define yindexcentorig   49.5

#define yindexminfin      0  
#define yindexmaxfin     99  
#define numyfin         100  
#define yindexcentfin    49.5
#endif

#ifdef TWO_00_MM
#define yindexmin         0  
#define yindexmax       183  
#define numy            184  
#define yindexcent       91.5

#define yindexminorig     0  
#define yindexmaxorig   143  
#define numyorig        144  
#define yindexcentorig   71.5

#define yindexminfin      0  
#define yindexmaxfin    127  
#define numyfin         128  
#define yindexcentfin    63.5
#endif

/*    The following parameters are used to access an (x,y) array that is */
/*    treated as a one-dimensional array.  The (x) index changes fastest. */

#define xyindexmin  (yindexmin * numx) + xindexmin
#define xyindexmax  (yindexmax * numx) + xindexmax
#define numxy  numx * numy

#define xyindexminorig        (yindexminorig * numxorig) + xindexminorig
#define xyindexmaxorig        (yindexmaxorig * numxorig) + xindexmaxorig
#define numxyorig  numxorig * numyorig

#define xyindexminfin        (yindexminfin * numxfin) + xindexminfin
#define xyindexmaxfin        (yindexmaxfin * numxfin) + xindexmaxfin
#define numxyfin  numxfin * numyfin

/*    t is the time-of-flight coordinate of the data.  PETT VI does not */
/*    measure differential time-of-flight, so the parameter given below */
/*    is only used to calculate a "pseudo" time-of-flight coordinate when */
/*    line integrals are being computed during a forward projection. */

#ifdef TWO_87_MM
#define tindexcent  63.5
#endif

#ifdef TWO_00_MM
#define tindexcent  91.5
#endif

/*    d, the transverse-distance coordinate of the data, is assigned an */
/*    index from dindexmin to dindexmax, inclusive.  The geometric center */
/*    of the scanner, dindexcent, is at the boundary between two bins. */
/*    The first four parameters below refer to the padded array in which */
/*    most of the computations are done.  The "orig" parameters refer to */
/*    actual PETT VI data. */

#define dindexmin         0  
#define dindexmax       127  
#define numd            128  
#define dindexcent       63.5

#define dindexminorig     0  
#define dindexmaxorig   107  
#define numdorig        108  
#define dindexcentorig   53.5

/*    The following parameters are the minimum and maximum indices of the */
/*    quantized angles, the number of angles, and the first angle (in */
/*    radians).  Angles increment in a counter-clockwise direction. */

#define pi  3.141592654

#define angindexmin   0
#define angindexmax  71
#define numang       72
#define firsttheta   pi

/*    The following parameters are used to access a distance-angle (d,a) */
/*    array that is treated as a one-dimensional array.  The (d) index */
/*    changes fastest. */

#define daindexmin  (angindexmin * numd) + dindexmin
#define daindexmax  (angindexmax * numd) + dindexmax
#define numda  numd * numang

#define daindexminorig        (angindexmin * numdorig) + dindexminorig
#define daindexmaxorig        (angindexmax * numdorig) + dindexmaxorig
#define numdaorig  numdorig * numang

/*    The following parameters describe the sampling of the images and */
/*    data.  Note:  The size of an image pixel, pix, and the sampling */
/*    interval of the data in the distance direction, dsamp, were */
/*    intended to be 2.7 and 2.5 mm, respectively.  These revised values */
/*    are due to Tom Videen.  The parameter tsamp refers to sampling */
/*    along the time-of-flight direction - see the note that describes */
/*    the parameter tindexcent.  The parameter anginc is the angular */
/*    increment between quantized angles. */

#ifdef TWO_87_MM
#define pix    2.87
#define tsamp  pix 
#endif

#ifdef TWO_00_MM
#define pix    2.00
#define tsamp  pix 
#endif

#define dsamp  2.65740741
#define anginc  pi / numang

/*    The following parameters describe the one-dimensional measurement */
/*    error of PETT VI in the distance direction.  The measurement error */
/*    is approximated by a one-dimensional Gaussian that is truncated to */
/*    to zero beyond +/- numstdev standard deviations and that has a */
/*    full-width at half-maximum of fwhmblow mm when the scanner is */
/*    operated in the low-resolution mode and fwhmbhigh mm when the */
/*    scanner is operated in the high-resolution mode. */

#define fwhmbhigh   7.1
#define fwhmblow   11.7
#define numstdev    3.0

/*    These parameters are the number of bytes needed to store a real or */
/*    integer variable. */

#define intsize   4
#define realsize  4

/*    These parameters are the minimum and maximum indices of the arrays */
/*    used to store the masks. */

#define maskforwindexmin  daindexmin
#define maskforwindexmax  daindexmax
#define maskbackindexmin  (angindexmin * numy) + yindexmin
#define maskbackindexmax  (angindexmax * numy) + yindexmax

/*    This parameter is the number of bytes in the informational header */
/*    at the beginning of each file.  The data start immediately after */
/*    the header. */

#define nbhead  256

/*    This is the size in bytes of a single slice of data in a 7-slice */
/*    Concurrent data file. */

#define slicerecord  (123 * 256)
