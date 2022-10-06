/*$Id: read_header2.c,v 1.2 1995/11/10 16:28:51 tom Exp $*/
/*$Header: /home/npggw/tom/src/libnpg/RCS/read_header2.c,v 1.2 1995/11/10 16:28:51 tom Exp $*/

/* Copyright 1993 Roger P. Woods, M.D. */
/* Modified 8/4/93*/

/****************************************************************/
/* float read_header						*/
/*								*/
/*  This routine should be customized to the particular data	*/
/*   type that you want to read.				*/
/*								*/
/*  There should be sufficient flexibility here to allow you	*/
/*    to deal with headers and images in the same file, 	*/
/*    headers and images in two separate files. You can even	*/
/*    deal with images all in separate files provided that	*/
/*    you create a global header that will contain the total	*/
/*    file dimensions in this last case.			*/
/*								*/
/*  Note that the plane numbering starts at zero, not at one	*/
/*   consequently, the planes will end at one less than the	*/
/*   z_dim that this routine loads into struct key_info *stats	*/
/*								*/
/*  If you need to know how many bits/pixel are wanted (e.g 	*/
/*	for rescaling), use sizeof(my_pixels) to find out.	*/
/*								*/
/*  If you need to know the maximum representable number, use	*/
/* 	MAX_POSS_VALUE (defined in AIR.h).			*/
/*								*/
/*  The integer variable 'flag' is designed to carry additional	*/
/*	coded information to the routine that actually reads	*/
/*	the data.						*/
/*								*/
/*								*/
/*  Generically, 						*/
/*    when plane<0 this routine should:				*/
/*	1. open the header file	using *fp_hdr			*/
/*	2. read data into struct key_info *stats		*/
/*		use the # bits/pixel for actual data on disk, 	*/
/*		not what you expect bits/pixel to become	*/
/*		once the data is loaded.			*/
/*	3. if *fp_img is not NULL				*/
/*		close *fp_img					*/
/*		set *fp_img to NULL				*/
/*	4. set *flag as required by read_image unless you	*/
/*		plan to do this plane by plane			*/
/*	5. if successful:					*/
/*		leave *fp_hdr open				*/
/*		return global scaling factor			*/
/*	6. if not successful:					*/
/*		close *fp_hdr and make it NULL			*/
/*		(don't close if you couldn't open)		*/
/*		return 0 (zero)					*/
/*								*/
/*    when plane>=0 this routine should:			*/
/*	1. always return unsuccessful if *fp_hdr is NULL	*/
/*		(even if you don't need info from *fp_hdr)	*/
/*	2. if *fp_img is NULL:					*/
/*		open image file containing plane specified	*/
/*         if *fp_img is not NULL:				*/
/*		possibly close and reopen *fp_img, iff each	*/
/*		plane is stored in a separate file		*/
/*	3. position the image file pointer to read plane	*/
/*	4. if not successful:					*/
/*		close *fp_img and make it NULL			*/
/*		close *fp_hdr and make it NULL			*/
/*		(don't close if already NULL or unopenable)	*/
/*	5. return multiplicative scaling factor for plane	*/
/*		(use, if any, will be defined in read routine)	*/
/*								*/
/*    calling routine should close *fp_hdr and *fp_img after	*/
/*	its final call to this routine				*/
/*								*/
/*								*/
/*  Guidelines for routines calling this function:		*/
/*								*/
/*  1. Your first call to load a given image set should always 	*/
/*	be with planes<0 (this will open and read the header)	*/
/*  2. Check for error after first call by testing whether	*/
/*	*fp_hdr is NULL						*/
/*  3. You can allocate memory after the first call		*/
/*  4. If you are loading multiple planes from the same data	*/
/*	set, don't close *fp_hdr between calls.			*/
/*  5. If you are making sequential calls to this function	*/
/*	to read sequential planes, assume that the function	*/
/*	may ignore the plane number and just get you the next	*/
/*	plane if *fp_img is left open during later calls;	*/
/*	however, don't plan for this to be the case.		*/
/*	Operationally, this means that if you want another	*/
/*	plane, but it is not one higher than the last one	*/
/*	that you just read from *fp_img, then close *fp_img	*/
/*	and make it NULL before the later calls			*/
/*  6. Check for error after subsequent calls by testing	*/
/*	whether *fp_img is NULL					*/
/*  7. Close *fp_img and *fp_hdr after last plane is read 	*/
/*	unless they are NULL (test each separately)		*/
/*  8. Note that you must also close *fp_hdr if you		*/
/*	only made a single call to routine with planes<0	*/
/*  9. Don't forget to verify and/or correct bits/pixel once	*/
/*	loading of data is completed so that it will reflect	*/
/*	the data in RAM, not the data on the disk		*/
/*								*/
/* Calls: clip_suffix()						*/
/****************************************************************/

#include <AIR.h>
#include <ANALYZE.h>

float read_header2(loadfile,fp_hdr,fp_img,stats,plane,flag)

char		*loadfile;
FILE 		**fp_hdr;
FILE 		**fp_img;
struct key_info *stats;
int		plane;
int		*flag;

{

	/*This version is designed to read ANALYZE format files		*/
	/*If *fp_img is already open,it takes advantage of the		*/
	/* concatenation of sequential planes				*/
	/* and ignores the requested plane number if it is >0 on the	*/
	/* presumption that preceding plane has been read and FILE *fp_img*/
	/* advanced automatically accordingly.				*/
	/* In other words, if planes >0 and *fp_img!=NULL,this routine	*/
	/*  does nothing except return 1.				*/

	struct dsr hdr;
	char filename[128];
	void clip_suffix();
	float scale;

	if(plane<0){
		if(*fp_img!=NULL){
			fclose(*fp_img);
			*fp_img=NULL;
		}
		strcpy(filename,loadfile);
		clip_suffix(filename);
		strcat(filename,HDR_SUFF);
		if((*fp_hdr=fopen(filename,"rb"))==NULL){
			printf("cannot open file %s for input\n",filename);
			return 0;
		}

		if(fread((char *)&hdr,1,sizeof(struct dsr),*fp_hdr)!=sizeof(struct dsr)){
			printf("file read error for file %s\n",filename);
			fclose(*fp_hdr);
			*fp_hdr==NULL;
			return 0;
		}

		/*Copy required data from ANALYZE header struct*/
		stats->bits=hdr.dime.bitpix;
		stats->x_dim=hdr.dime.dim[1];
		stats->y_dim=hdr.dime.dim[2];
		stats->z_dim=hdr.dime.dim[3];
		stats->x_size=hdr.dime.pixdim[1];
		stats->y_size=hdr.dime.pixdim[2];
		stats->z_size=hdr.dime.pixdim[3];

		if(stats->bits!=8 && stats->bits!=16){
			printf("Can't read ANALYZE data with %i bits/pixel (%s)\n",stats->bits,filename);
			fclose(*fp_hdr);
			*fp_hdr==NULL;
			return 0;
		}


		flag[0]=hdr.dime.glmin;
		flag[1]=hdr.dime.glmax;
		

		/*Calculate global rescaling factor if image is 16 bit and compilation is 8 bit*/
		if(stats->bits==16 && sizeof(my_pixels)==1){
			if(hdr.dime.glmin>=0){
				if(hdr.dime.glmax>32767){
					return 65535.0/hdr.dime.glmax;
				}
				else{
					return 32767.0/hdr.dime.glmax;
				}
			}
			else{
				return 65535.0/(hdr.dime.glmax+32678);
			}
		}

			
		return 1;
	}

	else{
		/* If initialization wasn't done--reject*/
		if(*fp_hdr==NULL){
			if(*fp_img!=NULL){
				fclose(*fp_img);
				*fp_img=NULL;
			}
			printf("Coding error--failure to initialize read_header\n");
			return 0;
		}

		scale=1;

		/* If image file isn't already open, do so now and find plane*/
		if(*fp_img==NULL){
			strcpy(filename,loadfile);
			clip_suffix(filename);
			strcat(filename,IMG_SUFF);
			if((*fp_img=fopen(filename,"rb"))==NULL){
				printf("cannot open file %s for input\n",filename);
				fclose(*fp_hdr); /* Yes, hdr not img */
				return 0;
			}
			if(plane>0){
				if(fseek(*fp_img,(long)plane*(stats->bits/8)*stats->x_dim*stats->y_dim,0)<0){
					printf("unable to seek to plane %i in file %s\n",plane,filename);
					fclose(*fp_img);
					*fp_img==NULL;
					fclose(*fp_hdr);
					*fp_hdr==NULL;
					return 0;
				}
				return scale;
			}
		}
		/* Since image file is already open, we should be ready to read next plane*/
		return scale;
	}
}
