#ifndef __NCproj_defs_module
#define __NCproj_defs_module 1

#define  SMALL     0.1    
#define  VERYSMALL 1e-15
typedef struct ProjCoeff_struct
  {
  int     nrproj;    
  float   angle;     
  float   scale;     
  int     cosscale;  
  int     N;         
  int     bigN;      
  int   *recindex1;  
  int   *recindex2;  
  float *recweight1; 
  float *recweight2; 
  float *bigsino;    
  int *linefirst1; 
  int *linefirst2; 
  int *linelast1;  
  int *linelast2;  
  int *lineoffset; 
  int   *bigindex1;  
  int   *bigindex2;  
  float *bigweight1; 
  float *bigweight2; 
  int   *smallindex1;  
  int   *smallindex2;  
  float *smallweight1; 
  float *smallweight2; 
  float  *worksino;
  float  *calcsino;
  float  *workrecon;
  float  *backatten;
  int     backatten_ok;
  } s_ProjCoeff;
typedef s_ProjCoeff  *p_ProjCoeff;
typedef struct AbsPrior_struct
  {
  int     number;     
  float   weight;     
  float  *mean;       
  float  *std;        
  float  *width;      
  int     allocated;  
  float  *gprior;     
  float  *g2prior;    
  } s_AbsPrior;
typedef s_AbsPrior *p_AbsPrior;
typedef struct MarkovPrior_struct
  {
  char     functiontype;   
  int      neighborhood;   
  float    delta;
  float    weight;         
  int      allocated;      
  float   *gprior;         
  float    g2prior;        
  } s_MarkovPrior;
typedef s_MarkovPrior  *p_MarkovPrior;

#endif /* #ifndef __NCproj_defs_module */
#ifndef __NCprojcoeff_module
#define __NCprojcoeff_module 1

#define __NCprojcoeff_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCprojcoeff_first_and_last(
    int     *first,
    int     *last,
    int     *colarray,
    int     *rowarray,
    int     coloffset,
    int     rowoffset,
    int     N,
    float   scale,
    float   radius)
;
extern 
p_ProjCoeff NCprojcoeff(
  int   nrdet,      
  int   nrproj,     
  float startangle, 
  float deltaangle, 
  float radius)     
;

#endif /* #ifndef __NCprojcoeff_module */
#ifndef __NCproj_free_module
#define __NCproj_free_module 1

#define __NCproj_free_module 1
extern 
void NCproj_free(p_ProjCoeff *projcoeff)
;

#endif /* #ifndef __NCproj_free_module */
#ifndef __NCproj_tobig_module
#define __NCproj_tobig_module 1

#define __NCproj_tobig_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_tobigsino(
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
;

#endif /* #ifndef __NCproj_tobig_module */
#ifndef __NCproj_frombig_module
#define __NCproj_frombig_module 1

#define __NCproj_frombig_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_frombigsino(
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
;

#endif /* #ifndef __NCproj_frombig_module */
#ifndef __NCproj_forward_module
#define __NCproj_forward_module 1

#define __NCproj_forward_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_forward(
  float       *img,
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
;

#endif /* #ifndef __NCproj_forward_module */
#ifndef __NCproj_back_module
#define __NCproj_back_module 1

#define __NCproj_back_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_back(
  float       *img,
  float       *sinogram,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
;

#endif /* #ifndef __NCproj_back_module */
#ifndef __NCproj_attforward_module
#define __NCproj_attforward_module 1

#define __NCproj_attforward_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_attforward(
  float       *img,        
  float       *sinogram,   
  float       *attenimg,   
  int          projnum,    
  p_ProjCoeff  projcoeff   
  )
;

#endif /* #ifndef __NCproj_attforward_module */
#ifndef __NCproj_attback_module
#define __NCproj_attback_module 1

#define __NCproj_attback_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_attback(
  float       *img,        
  float       *sinogram,   
  float       *attenimg,   
  int          projnum,    
  p_ProjCoeff  projcoeff   
  )
;

#endif /* #ifndef __NCproj_attback_module */
#ifndef __NCsino_subset_module
#define __NCsino_subset_module 1

#define __NCsino_subset_module 1
extern 
int* NCsino_subset(  
  int    nrsubsets,  
  int    nrangles,   
  int   *subsetnum,  
  int  **subsetmask, 
  int   *previous    
  )
;

#endif /* #ifndef __NCsino_subset_module */
#ifndef __NCml_module
#define __NCml_module 1

#define __NCml_module 1
extern 
void NCml(
  float       *recon,      
  float       *sino,       
  float       *atten,      
  int         *subset,     
  float       *likelihood, 
  p_ProjCoeff  prcf)       
;

#endif /* #ifndef __NCml_module */
#ifndef __NCmlosem_module
#define __NCmlosem_module 1

#define __NCmlosem_module 1
extern 
void NCmlosem(
  float        *recon,  
  float        *sino,   
  float        *atten,  
  int           nriter, 
  int           nrsubsets, 
  float       **likelihood, 
  p_ProjCoeff   prcf,       
  int           verbose)  
;

#endif /* #ifndef __NCmlosem_module */
#ifndef __NCextend_nrm_module
#define __NCextend_nrm_module 1

#define __NCextend_nrm_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
float NCextend_nrm(
                  float  *nrmout,  
                  float  *nrmin,   
                  int     nrdet,   
                  int     nrproj,  
                  float   toosmall) 
;

#endif /* #ifndef __NCextend_nrm_module */
#ifndef __NCproj_clr_module
#define __NCproj_clr_module 1

#define __NCproj_clr_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_clrfov(
  float       *img,
  int          nrows,
  int          ncols,
  float        value
  )
;

#endif /* #ifndef __NCproj_clr_module */
#ifndef __NCproj_backspectmu_module
#define __NCproj_backspectmu_module 1

#define __NCproj_backspectmu_module 1
extern 
void NCproj_backspectmu(
  float       *img,        
  float       *totweight,  
  float       *sinogram,   
  float       *actimg,     
  float       *attenimg,   
  int          projnum,    
  p_ProjCoeff  projcoeff   
  )
;

#endif /* #ifndef __NCproj_backspectmu_module */
#ifndef __NCprojcoeff2_module
#define __NCprojcoeff2_module 1

#define __NCprojcoeff2_module 1
extern 
p_ProjCoeff NCprojcoeff2(
  int   nrdet,      
  int   nrproj,     
  float startangle, 
  float deltaangle, 
  float radius,     
  float *coroffset  
  )
;

#endif /* #ifndef __NCprojcoeff2_module */
#ifndef __NCml_spect_module
#define __NCml_spect_module 1

#define __NCml_spect_module 1
extern 
void NCml_spect(
  float       *recon,      
  float       *sino,       
  float       *sensit,     
  float       *attenimg,   
  int         *subset,     
  float       *likelihood, 
  p_ProjCoeff  prcf)       
;

#endif /* #ifndef __NCml_spect_module */
#ifndef __NCprojcoeff3_module
#define __NCprojcoeff3_module 1

#define __NCprojcoeff3_module 1
extern 
p_ProjCoeff NCprojcoeff3(
  int   nrdet,      
  int   nrproj,     
  float startangle, 
  float deltaangle, 
  float radius,     
  float *coroffset, 
  float  arc_radius 
  )
;

#endif /* #ifndef __NCprojcoeff3_module */
#ifndef __NCdcoffset_module
#define __NCdcoffset_module 1

#define __NCdcoffset_module 1
extern 
float* NCdcoffset(           
  float         *sinogram,   
  p_ProjCoeff    projcoeff,  
  float         *sensit,     
  float          radius,     
  float          value       
  )
;

#endif /* #ifndef __NCdcoffset_module */
#ifndef __NClsrecon_module
#define __NClsrecon_module 1

#define __NClsrecon_module 1
extern 
void NClsrecon(
  float       *recon,      
  float       *sino,       
  float       *variance,   
  float       *atten,      
  int         *subset,     
  float       relaxation,  
  float       *likelihood, 
  int         emission,    
  float     **backmlem,    
  p_ProjCoeff  prcf)       
;

#endif /* #ifndef __NClsrecon_module */
#ifndef __NClsosrecon_module
#define __NClsosrecon_module 1

#define __NClsosrecon_module 1
extern 
void NClsosrecon(
  float        *recon,    
  float        *sino,     
  float        *variance,   
  float        *atten,  
  int           nriter, 
  int           nrsubsets, 
  float         relaxation, 
  float       **likelihood, 
  int         emission,    
  float     **backmlem,    
  p_ProjCoeff   prcf,       
  int           verbose)  
;

#endif /* #ifndef __NClsosrecon_module */
#ifndef __NCproj_clearedge_module
#define __NCproj_clearedge_module 1

#define __NCproj_clearedge_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_clearedge(
  float       *img,
  int          projnum,
  p_ProjCoeff  projcoeff
  )
;

#endif /* #ifndef __NCproj_clearedge_module */
#ifndef __NCmltrans_module
#define __NCmltrans_module 1

#define __NCmltrans_module 1
extern 
void NCmltrans(
  float         *recon,       
  float         *sino,        
  float         *blank,       
  float         *contam,      
  int           *subset,      
  float          relaxation,  
  float         *likelihood,  
  p_ProjCoeff    prcf,        
  p_AbsPrior     absprior,    
  p_MarkovPrior  markovprior, 
  int            verbose) 
;

#endif /* #ifndef __NCmltrans_module */
#ifndef __NCmlostr_module
#define __NCmlostr_module 1

#define __NCmlostr_module 1
extern 
void NCmlostr(
  float        *recon,       
  float        *sino,        
  float        *blank,       
  float        *contam,      
  int           nriter,      
  int           nrsubsets,   
  float         relaxation,  
  float       **likelihood,  
  p_ProjCoeff   prcf,        
  p_AbsPrior    absprior,    
  p_MarkovPrior markovprior, 
  int           verbose)     
;

#endif /* #ifndef __NCmlostr_module */
#ifndef __NCgradprior_module
#define __NCgradprior_module 1

#define __NCgradprior_module 1
extern 
void NCgradprior(
  float    *gprior,     
  float    *g2prior,    
  float    *img,        
  int       nrpixels,   
  int       nrmodes,    
  float    *priormean,  
  float    *priorstd,   
  float    *priorwidth) 
;

#endif /* #ifndef __NCgradprior_module */
#ifndef __NCmarkov_module
#define __NCmarkov_module 1

#define __NCmarkov_module 1
extern 
void NCmarkov(
  float    *gprior,       
  float    *g2prior,      
  float    *img,          
  int       ncols,        
  int       nrows,        
  float     delta,        
  int       neighborhood, 
  char      functiontype) 
;

#endif /* #ifndef __NCmarkov_module */
#ifndef __NCzoom_image_module
#define __NCzoom_image_module 1

#define __NCzoom_image_module 1
extern 
float NCzoom_image(
   float   *image_out,
   float   *image_in,
   float    pixel_size,
   int      dim_in_x,
   int      dim_in_y,
   int      dim_out_x,
   int      dim_out_y,
   float    orig_zoom,
   float    x_off,
   float    y_off,
   float    degrees,
   int      verbose)
;

#endif /* #ifndef __NCzoom_image_module */
#ifndef __NCproj_maxim_module
#define __NCproj_maxim_module 1

#define __NCproj_maxim_module 1
#include "NCproj_bproj.h"
#include "math.h"
extern 
void NCproj_maxim(
  float       *img,        
  float       *sinogram,   
  int          projnum,    
  p_ProjCoeff  projcoeff   
  )
;

#endif /* #ifndef __NCproj_maxim_module */
#ifndef __NCgauss_module
#define __NCgauss_module 1

#include <math.h>
extern 
void NCgauss( 
             float *in,        
             float *out,       
             int   nrrows,     
             int   nrcols,     
             int   nrplanes,   
             float pixelsize,  
             float fwhm_mm,      
             int   n_sigmas,     
             int   copy_border,  
             int   mirror_border,
             int   axis          
             )
;

#endif /* #ifndef __NCgauss_module */
