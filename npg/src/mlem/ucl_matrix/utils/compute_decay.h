/*
 * original Mediman code 
 *

void ImageAdm::computeDecay() {
    if (IsotopHalfLife== 0)
        _decay = 1;
    else {
        int inj = injecttime[0]*3600 + 60*injecttime[1] + injecttime[2];
        int acq = 3600*acqustarttime[0]+60*acqustarttime[1]+acqustarttime[2];
        float acqsta = acq-inj;
        float t12 = IsotopHalfLife;
        float lambda = log(2.)/t12;
        float corr = 1.;
        if (acquisitionlength != 0) {
            corr = 1. - exp(-lambda*acquisitionlength);
            corr /= (lambda*acquisitionlength);
        }
        float dec1 = exp((double)(lambda*(acquisitiontime+acqsta)));
        _decay = dec1/corr;
    }
}
*/

/*
decay_factor = factor_1*factor_2
	- where factor_1 is the correction factor for the elapsed time between
	  injection time and frame acquisition start time (DT) and
	- factor_2 is the correction factor within the frame acquisition
	  duration time (dt).

	- factor_1 = exp(lambda*DT)
	- factor_2 = (lambda*dt)/(1-exp(lambda*dt));
		- where lambda = log(2)/half_life;
			-where log() compute the natural (base e) logarithm
*/

#include <math.h>
#include <sys/time.h>
#include "matrix.h"
static float compute_decay(half_life, DT, dt)
float half_life;
float  DT, dt; /* times in secondes */

{
	float lambda=0.0, factor_1=1.0, factor_2=1.0;

	if (half_life == 0) return 1.0;
	lambda = log(2.0)/half_life;
	if (dt > 0) factor_2 = (lambda*dt)/(1.0 - exp(-lambda*dt));
	factor_1  = exp(lambda*DT);
	return (float)(factor_1*factor_2);
}

static float frame_decay(file, inj_time, frame)
MatrixFile* file;
int inj_time;
int frame;
{
	float half_life, DT, dt;
	MatrixData *matrix;
	Main_header *mh;
	Image_subheader* imh;
	struct tm tm;

	half_life = file->mhptr->isotope_halflife;
	matrix = matrix_read( file, mat_numcod(frame,1,1,0,0), MAT_SUB_HEADER);
	if (!matrix) matrix_perror(file->fname);
	mh = file->mhptr;
	tm = *localtime(&mh->scan_start_time);
    DT = tm.tm_hour*3600 + tm.tm_min*60 + tm.tm_sec - inj_time;
	imh = (Image_subheader*)matrix->shptr;
	dt = imh->frame_duration; /* millisecs */
	dt /= 1000;				 /* secs */
	free_matrix_data(matrix);
	return compute_decay(half_life,DT,dt);
}
