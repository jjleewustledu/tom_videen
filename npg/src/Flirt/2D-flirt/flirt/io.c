#include "flirt.h"
#include "io.h"

/* -------------------------------------------------------------------------- */
void readTIFF(char *name, array2D out, int m1, int m2){
  int j=0;
  int k1,k2;
	
	
  uint32 w, h;
  uint16 orient;
  size_t npixels;
  uint32* raster;
  TIFF* tif;
  
  //printf("%s\n",name);
  
  tif = TIFFOpen(name, "r");
  if (tif) {
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    npixels = w * h;
    
    if ( ( out == NULL) || (w != m1) || (h != m2)) {
      fprintf(stderr,
	      "\n\nStorage for image is not malloced"
	      " or has illegal shape!\n\n");
      exit(1);
      
    }
#if 0
    *m1 = w;
    *m2 = h;
    
    if (*out != NULL) {
      /* dummy = (double *)calloc(w*h,sizeof(double));
      (*out) = (double **)calloc(w,sizeof(double*));
      if ((*out) == NULL) printf("can't allocate memory\n");
      for (j=0;j<w;j++) (*out)[j] = &(dummy[j*h]);*/
    } else {
      fprintf(stderr,"\n\nStorage for image not malloced!!!\n\n");
      exit(1);
    }
#endif
		
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster != NULL) {
      // set the origin of the image. 
      TIFFGetField(tif, TIFFTAG_ORIENTATION, &orient);
      //printf("Orientation: %x\n",orient);
      //if (TIFFReadRGBAImageOriented(tif, h, w, raster, ORIENTATION_TOPLEFT, 0)) {
      if (TIFFReadRGBAImageOriented(tif, w, h, raster, orient, 0)) {
	for( k2=h-1; k2>-1; k2--) {
	  for(k1=0;k1<w;k1++) {
	    out[k1][k2] = ((double)TIFFGetR(raster[j]) + 
			   (double)TIFFGetG(raster[j]) + 
			   (double)TIFFGetB(raster[j]))/3;
	    j++;
	  }
	}
      }
    }
    _TIFFfree(raster);
    TIFFClose(tif);
  }
}
/* -------------------------------------------------------------------------- */

void readTIFFsize(char *name, int *m1, int *m2) {
	uint32 w, h;
	TIFF* tif;
	
	tif = TIFFOpen(name, "r");
	if (tif) {
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		*m1 = w;
		*m2 = h;
		
		TIFFClose(tif);
	}
	else {
		printf("readTIFFsize: Cannot open file: %s\n", name);
	}
}


/* -------------------------------------------------------------------------- */
void writeTIFF(char *name, array2D in, int m1, int m2) {
	TIFF *out;
	
	int sampleperpixel = 1;
	char *image;
	tsize_t linebytes = sampleperpixel * m1; // length in memory of one row of pixel in the image. 
	unsigned char *buf = NULL;        // buffer used to store the row of pixel information for writing to file
	int j1,j2,tmp=0;
	uint32 row;
	
	if ( (out=TIFFOpen(name, "w")) == NULL) {
		printf("Could not open file %s for writing\n",name);
		return;
	};	
	
	image = (char*)malloc(m1*m2*sampleperpixel*sizeof(char));
	
	// copy data from double in to char image
	for (j2=0;j2<m2;j2++)
		for (j1=0;j1<m1;j1++)
			image[tmp++]=(char)in[j1][j2];
	
	
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, m1);   // set the width of the image
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, m2); // set the height of the image
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);  // set number of channels per pixel
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    // set the size of the channels
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT); // set the origin of the image.
																 //   Some other essential fields to set that you do not have to understand for now.
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	
	
	//    Allocating memory to store the pixels of current row
	if (TIFFScanlineSize(out)<linebytes)
		buf = (unsigned char *)_TIFFmalloc(linebytes);
	else
		buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));
	
	// We set the strip size of the file to be size of one row of pixels
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, m1*sampleperpixel));
	
	//Now writing image to the file one strip at a time
	for (row = 0; row < m2; row++)
    {
		// check the index here, and figure out why not using h*linebytes
		memcpy(buf, &image[(m2-row-1)*linebytes], linebytes);
		if (TIFFWriteScanline(out, buf, row, 0) < 0)
			break;
    }
	
	
	
	TIFFClose(out);
	if (buf)
		_TIFFfree(buf);
	
	free(image);
}



/* -------------------------------------------------------------------------- */
int string2int(char* digit) {
	int result = 0;
	
	//--- Convert each digit char and add into result.
	while (*digit >= '0' && *digit <='9') {
		result = (result * 10) + (*digit - '0');
		digit++;
	}
	
	return(result);
}


/* -------------------------------------------------------------------------- */
double string2double(char* digit) {
	double result = 0;
	double factor = 1.0;
	
	//--- Convert each digit char and add into result.
	while (*digit >= '0' && *digit <='9') {
		result = (result * 10) + (*digit - '0');
		digit++;
	}
	//--- convert each digit chad and add into result
	if (*digit=='.'){
		digit++;
		while (*digit >= '0' && *digit <='9') {
			factor *= 0.1;
			result += factor * (*digit - '0');
			digit++;
		}
	}
	
	return(result);
}


/* -------------------------------------------------------------------------- */
void getMainParameters(char *fileName, 
					   int *m1, 
					   int *m2,
					   double *h1,
					   double *h2,
					   char *fileNameT, 
					   char *fileNameR,
					   char *fileNameOut,
					   char *fileNameU,
					   char *smootherName,
					   int *maxIter,
					   double *alpha,
					   double *tau,
					   double *mu,
					   double *lambda)
{
	FILE *ptr;
	char puffer[255];
	char *varName, *varValue;
	int pf = 0; //printflag
	
	strncpy(fileName,fileName,strlen(fileName)-1);
	printf("opening/reading parameter file '%s'...",fileName);
	
	if((ptr=fopen(fileName,"r")) == NULL)
    {
		fprintf(stderr,"can't open file '%s'\n",fileName);
		exit (0);
    }
	//printf("done.\n");
	
	//printf("reading parameter file...");
	
	if (pf) printf("\n");
	
	while(fgets(puffer,255,ptr)){
		if (pf) printf("puffer=\"%s\"\n",puffer);
		if (puffer[0]!='%'){
			
			varName  = strtok(puffer," \t=:\n");
			
			if (varName != NULL) {
				varValue = strtok(NULL," \t=:\n");
				if (varValue != NULL) {
					if (pf) printf("varName=\"%s\", varValue=\"%s\"\n",varName,varValue);
					
					if (!strncmp(varName,"h1",strlen(varName))){
						*h1 = string2double(varValue);
						if (pf) printf("h1 -- %f\n",*h1);
					}
					else if (!strncmp(varName,"h2",strlen(varName))){
						*h2 = string2double(varValue);
						if (pf) printf("h2 -- %f\n",*h2);
					}
					else if (!strncmp(varName,"fileNameT",strlen(varName))){
						strncpy(fileNameT,varValue,strlen(varValue));
						if (pf) printf("fileNameT -- \"%s\"\n",fileNameT);
					}
					else if (!strncmp(varName,"fileNameR",strlen(varName))){
						strncpy(fileNameR,varValue,strlen(varValue));
						if (pf) printf("fileNameR -- \"%s\"\n",fileNameR);
					}
					else if (!strncmp(varName,"fileNameOut",strlen(varName))){
						strncpy(fileNameOut,varValue,strlen(varValue));
						if (pf) printf("fileNameOut -- \"%s\"\n",fileNameOut);
					}
					else if (!strncmp(varName,"fileNameU",strlen(varName))){
						strncpy(fileNameU,varValue,strlen(varValue));
						if (pf) printf("fileNameU -- \"%s\"\n",fileNameU);
					}
					else if (!strncmp(varName,"regType",strlen(varName))){
						strncpy(smootherName,varValue,strlen(varValue));
						if (pf) printf("regType -- \"%s\"\n",smootherName);
					}
					else if (!strncmp(varName,"maxIter",strlen(varName))){
						*maxIter = string2int(varValue);
						if (pf) printf("maxIter -- %i\n",*maxIter);
					}
					else if (!strncmp(varName,"alpha",strlen(varName))){
						*alpha = string2double(varValue);
						if (pf) printf("alpha -- %f\n",*alpha);
					}
					else if (!strncmp(varName,"tau",strlen(varName))){
						*tau = string2double(varValue);
						if (pf) printf("tau -- %f\n",*tau);
					} 
					else if (!strncmp(varName,"mu",strlen(varName))){
						*mu = string2double(varValue);
						if (pf) printf("mu -- %f\n",*mu);
					} 
					else if (!strncmp(varName,"lambda",strlen(varName))){
						*lambda = string2double(varValue);
						if (pf) printf("lambda -- %f\n",*lambda);
					} 
					else printf("\n\nunknown parameter <<%s>>\n\n",varName);
				}			
			}
		}
	}
    
	//printf(" done.\n");
	//printf("closing parameter file...");	
	fclose(ptr);
	printf(" done.\n");
	printf("read image sizes from file %s...",fileNameR);
	readTIFFsize(fileNameR,m1,m2);
	printf(" done.\n");
}
