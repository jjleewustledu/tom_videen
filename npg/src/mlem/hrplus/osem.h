#ifndef osem_h
#define osem_h

/* Definition of scanner parameters */
extern float tilt;

/* Definition of scan parameters */
extern int radial_pixels;
extern int views;
extern float z_size;
extern float sino_sampling;
extern int x_off;
extern int y_off;
extern float x_size;
extern int z_pixels;
extern int frame_start_time;
extern int frame_duration;
extern int corrections_applied;

#define MAXSUBSETS 32
#define PI 3.14159265358979323846
#define DMAXPOWER 10

/* Definition image parameters */
#define          x_pixels        128
#define          y_pixels        128

/* centre of the slice */
#define		z_off		0
#define		r_interleave	0.0
#define         interp_factor	4


/* Definition of data types*/
typedef float **SLICE;
typedef SLICE *IMAGE;
typedef float **SINOGRAM;
#endif


