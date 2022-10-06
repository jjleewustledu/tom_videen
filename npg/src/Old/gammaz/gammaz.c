/*$Id: gammaz.c,v 1.11 1995/04/28 20:08:36 ty7777 Exp $*/
/*$Log: gammaz.c,v $
 * Revision 1.11  1995/04/28  20:08:36  ty7777
 * Added .3sp file tail.
 *
 * Revision 1.9  1994/05/27  14:33:31  ty7777
 * Modified by Tom Yang to read the new af3d output file and write the output
 * in x, y, and z format.
 *
 * Revision 1.8  1994/05/26  21:47:44  ty7777
 * Modified to read the new af3d output.
 *
 * Revision 1.7  1994/05/19  20:34:30  ty7777
 * The order of af3d output is changed.
 *
 * Revision 1.6  1992/09/22  22:39:33  ty7777
 * Reading .3sp and .3sn files as input.
 *
 * Revision 1.5  1992/09/17  14:56:20  ty7777
 * gammaz command line input working version.
 *
 * Revision 1.4  1992/09/17  14:05:19  ty7777
 * Command line input, before 3dp and 3dn files worked.
 **/

#define MAXPOINTS		100000	/* maximum data points allowed */
#define ZPOINTS			10000	/* maximum z-test points allowed */
#define MAXFILES		50	/* maximum input files allowed */
#define VERSION			3.0	/* program version */
#define MAXLINE			256	/* maximum number of characters in a line */

typedef struct			/* structure for holding z-test  */
{
	float	z;		/* z value                       */
	float	x;		/* x value                       */
	float	y;		/* y value                       */
	float	max;		/* maximum value                 */
	float	z_score;	/* z score value                 */
	float	significance;	/* significance level            */
} sig_numbers;

#include <stdio.h>
#include <string.h>
#include <math.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/Old/gammaz/RCS/gammaz.c,v 1.11 1995/04/28 20:08:36 ty7777 Exp $";

/*______________________________________________________________________

 program:	gammaz         H. Rebecca Kimmel           25 January 1989

 Description:	This program is based on the original Gammaz program written
		by Lewis Greer and Peter Fox. The original program was 
		written in Fortran for the Current Computers used by PET.
		GammaZ uses data output from AF3D and calculates the mean,
		standard deviation, gamma 1 and gamma 2 t-statistic, 
		t-statistic, skew, kurtosis, and z-test.

 Calls:		get_info 
		data_analysis
		write_info

 History:	Ported to the Sun Unix environment by Tom Yang on 
		Sep. 14, 1992.
		Changed by Tom Yang on Sep. 16, 1992 to take command
		line arguments.
______________________________________________________________________*/

main (argc, argv)
	int	argc;
	char	**argv;
{
	sig_numbers	z_array [ZPOINTS];		/* array holding z-test results */
	float		p1 [3],				/* .01 significance prob. of leptokurt */
			p2 [3],				/* .05 significance prob. of leptokurt */
			prob [3],			/* probability of leptokurtosis */
			cutoff,				/* cutoff for z-test */
			mean,				/* mean value for data points */
			ar [4][MAXPOINTS],		/* array holding af3d data points */
			qrob,				/* indicates skew */
			q1,				/* .01 signif. level for skew */
			q2,				/* .05 signif. level for skew */
			g2 [3],				/* gamma 2 - bottom, all, top curve */
			t2 [3],				/* tscore 2 - bottom, all, top curve */
			sd [3],				/* standard deviation - bottom, all, top */
			g1, t1;				/* gamma 1 and t-stat. for all points */
	int		zcount,				/* number of values in the z-test array */
			npos, nneg,			/* number of pos/neg data points */
			n [3],				/* number of data points at bottom, all, top */
			nkurt [3],			/* nkurt [1] = sample size nkurt [2] = 1% */
							/* significance nkurt [3] = 5% significance */
			nskew,				/* number of points used for skew */
			filenum,			/* number of input files read in */
			sarp,				/* flag indicating sarp is/not wanted */
			ztest,				/* flag indicating ztest desired */
			output_list [4],		/* array holding types of output wanted */
							/* [0] is console-this happens always */
							/* [1] is to an output file */
							/* [2] is to the printer */
			status,				/* error status */
			midpoint;			/* flag specifying mean or at 0 */
	char		outfile [80],			/* name of output file */
			filenames [MAXFILES][80];	/* array holding input filename*/

	/*
	 * Get command line optional parameters.
	 */
	filenum	= 2;
	strcpy (filenames [0], argv [1]);
	strcpy (filenames [1], argv [2]);
	cutoff	= atof (argv [3]);
	strcpy (outfile, argv [4]);

	/*
	 * Default parameters.
	 */
	sarp		= -1;
	midpoint	= 0;
	output_list [0]	= 1;
	output_list [1]	= 2;
	output_list [3]	= -1;
	ztest		= -1;

	/* 
	 * Initialize data counter array. 
	 */                   
	n[0]	= 0; 
	n[1]	= 0; 
	n[2]	= 0; 

	/* 
	 * Get input information and read in data.
	 */
	status	= get_info (n, ar, filenames, &filenum); 

	if (status != -2) /* if no fatal errors, do data analysis */
	{
		data_analysis (ar, n, &midpoint, &ztest, &sarp, sd, g2, t2, &npos, &nneg,
			&mean, &g1, &t1, &q1, &q2, nkurt, p1, p2, prob, &qrob, &nskew,
			z_array, &cutoff, &zcount);

		write_info (n, npos, nneg, mean, sd, g1, t1, midpoint, qrob, nskew, q1, q2, g2,
			t2, prob, nkurt, p1, p2, filenames, filenum, outfile, output_list, z_array,
			cutoff, ztest, zcount);
	}
}


/*______________________________________________________________________

Function:	get_info    H. Rebecca Kimmel           25 January 1989

Description:	This function reads in data from the .3dp and .3dn files.

Calls:		yes_no
		error_msg
		gets

Called by:	main

History:	Rewritten by Tom Yang. 
		09/16/1992.
______________________________________________________________________*/

get_info (n, ar, filenames, filenum)
	char	filenames [MAXFILES][80];	/* array holding input filenames */
	float	ar [4][MAXPOINTS];		/* array containing af3d data */
	int	*filenum,			/* number of input filenames read in */
		n [3];				/* number of points at bottom, top, all */
{
	int	status;				/* error status */
	int	split;				/* flag indicating files data is in */

	split	= 1;
	status	= rfile (&split, filenames [0], n, ar);

	split	= -1;
	status	= rfile (&split, filenames [1], n, ar);

	return (status);
}


/*_____________________________________________________________________

Function:	yes_no        H. Rebecca Kimmel            25 January 1989

Description:	This routine gets the answer from the user and sets the flag
		to either true or false.

Called by:	get_info

HIstory:
______________________________________________________________________*/

yes_no (q)
	char *q;	/* question */
{
	char	answ,	/* the actual answer from the user */
		ans;	/* the previous answer,since newline is read */
	int	answer;	/* flag returned to calling routine */

	answ	= ' ';
	printf ("\n%s :  ", q);
	while ((ans = getchar ()) != '\n')
		answ	= ans;
	if (answ == 'n'|| answ == 'N')
		answer	=  0; 
	else
		answer	= -1;

	return(answer) ;
}


/*___________________________________________________________________

Function:	rfile         H. Rebecca Kimmel          25 January 1989

Description:	This routine opens a file and reads in the data. If an 
		error is encountered, an error message is sent and the 
		routine terminates. The input file is in SPED format.

Called by:	get_info

History:	Modified by Tom Yang on Sep. 15, 1992 to read new af3d
		output.
____________________________________________________________________*/
rfile (split, filename, n, ar)
	float	ar [4][MAXPOINTS];	/* array containing data points read in */
	char	filename [80];		/* name of file containing data points */
	int	*split,			/* indicates if pos and neg files to be read */
		n [3];			/* number of points:bottom, all, top */
{
	char	header [MAXLINE];	/* header from file */
	int	status,		/* error status flag */
		i,		/* counter */
		xx, yy;		/* counter for rows and columns */
	FILE	*fp,		/* file pointer to input file */
		*fopen();	/* C function for opening files */

	status	= 0;   
	fp	= fopen (filename, "r");
	if (fp == NULL)                /* see if file exists are readable */
	{
		status	= error_msg (0);
		return (status);
	}

	i	= 0;
	fgets (header, MAXLINE, fp);
	fgets (header, MAXLINE, fp);
	sscanf (header, "%d, %d", &xx, &yy);
	/*
	fscanf (fp, "%d, %d", &xx, &yy);
	*/

	if (yy != 4 && yy != 1)
	{
		status	= error_msg (1);
		fclose (fp);
		return (status);
	}

	if ((n[1] + xx) > MAXPOINTS)  /* don't read more than array can hold*/
	{
		status	= error_msg (2);
		fclose (fp);
		return (status);
	}

	fgets (header, MAXLINE, fp);
	fgets (header, MAXLINE, fp);
	fgets (header, MAXLINE, fp);

	for (i = n [1]; i < (n [1] + xx); i++)
	{
		if (yy != 1)
			fscanf (fp, "%f %f %f %f", 
				&ar [1][i], &ar [2][i], &ar [0][i], &ar [3][i]);
		else
		{
			fscanf (fp, "%f", &ar [3][i]);
			ar [0][i]	= 0.0;
			ar [1][i]	= 0.0;
			ar [2][i]	= 0.0;
		}

		if (*split == -1 && ar [3][i] > 0.0) /* negate value in neg file*/
			ar [3][i]	= ar [3][i] * -1.0;
		if (ar [3][i] == 0.0)
		{
			printf ("%f %f %f %f\n", ar [0][i], ar [1][i], ar [2][i], ar [3][i]);
			status	= error_msg (3);
			status	= error_msg (1);
			fclose (fp);
			return (status);
		}
	} 

	n [1]	+= xx;
	fclose (fp);

	return (status);
}        


/*____________________________________________________________________

FUNCTION : DATA ANALYSIS      H. Rebecca Kimmel       25 January 1989

INTENT   : This function performs the different statistical analysis on
           the data.

CALLED BY: MAIN
CALLS    : ZTEST
           ERROR_MSG

HISTORY  :
_____________________________________________________________________*/

data_analysis (ar, n, midpoint, ztest, sarp, sd, g2, t2, npos, nneg, mean, g1, t1,
		q1, q2, nkurt, p1, p2, prob, qrob, nskew, z_array, cutoff, zcount)

	sig_numbers	z_array [ZPOINTS];	/* array to hold ztest results */

	float		ar [4][MAXPOINTS],	/* array holding data points for analysis */
			prob [3],		/* probability of leptokurtosis */
			sd [3],			/* standard deviation-bottom, all, top */
			g2 [3],			/* gamma 2 -bottom, all, top curve */
			t2 [3],			/* tscore 2 - bottom, all, top curve */
			*q1,			/* .01 signif. level for skew */
			*q2,			/* .05 signif. level for skew */
			*g1,			/* gamma 1 for all points */
			*t1,			/* t-stat for all points */
			*mean,			/* mean value for data points */
			*qrob,			/* Indicates skew */
			*cutoff,		/* cutoff for z-test */
			p1 [3],			/* .01 signif.prob of leptokurtosis */
			p2 [3];			/* .05 signif.prob of leptokurt-bot, all, top*/

	int		*midpoint,		/* flag specifying mean or at 0 */
			*npos,			/* number of positive data points */
			*nneg,			/* number of negative data points */
			*nskew,			/* number of points used for skew */
			*ztest,			/* flag indicating z-test determination */
			*sarp,			/* flag indicating sarp transformation */
			*zcount,		/* number of data points in the z-array */
			nkurt[3],		/* sample size,1%sig, 5% significance */
			n[3];			/* number of data points@bottom, all, top */

{
	/*
	 * KURTOSIS TABLE      SAMPLE   0.1   0.5
	 *                          SIZE     SIG   SIG                      
	 */
	static float kurt [27][3] = {50.0, 4.88, 3.99,
                                75.0, 4.59, 3.87,
                               100.0, 4.39, 3.77,
                               125.0, 4.24, 3.71,
                               150.0, 4.13, 3.65,
          		       200.0, 3.98, 3.57,
                               250.0, 3.87, 3.52,
                               300.0, 3.79, 3.47,
                               350.0, 3.72, 3.44,
                               400.0, 3.67, 3.41,
                               450.0, 3.63, 3.39,
                               500.0, 3.60, 3.37,
                               550.0, 3.57, 3.35,
                               600.0, 3.54, 3.34,
                               650.0, 3.52, 3.33,
                               700.0, 3.50, 3.31,
                               750.0, 3.48, 3.30,
                               800.0, 3.46, 3.29,
                               850.0, 3.45, 3.28,
                               900.0, 3.43, 3.28,
                               950.0, 3.42, 3.27,
                              1000.0, 3.41, 3.26,
                              1200.0, 3.37, 3.24,
                              1400.0, 3.34, 3.22,
                              1600.0, 3.32, 3.21,
                              1800.0, 3.30, 3.20,
                              2000.0, 3.28, 3.18 } ;

	/*
	 * SKEW TABLE        SAMPLE 0.01  0.05
         *                   SIZE  SIG.  SIG.               
	 */
	static float skew[21][3] = {25.,1.061, .711,
                              30., .982, .661,
                              35., .921, .621,
                              40., .869, .587,
                              45., .825, .558,
                              50., .787, .553,
                              60., .723, .492,
                              70., .673, .459,
                              80., .631, .432,
                              90., .596, .409,
                             100., .567, .389,
                             125., .508, .350,
                             150., .464, .321,
                             175., .430, .298,
                             200., .403, .280,
                             250., .360, .251,
                             300., .329, .230,
                             350., .305, .213,
                             400., .285, .200,
                             450., .269, .188,
                             500., .255, .179 };

	float	moment [3][3],	/* moment array */ 
		temp_moment,	/* temporary moment buffer */
		tdiv,		/* temp.divisor for skew diff */
		tmp,		/* temp value */
		s1, s2,		/* place holders for kurt diff */
		nn,		/* number of points */
		jfloat;		/* convert counter to float */
	int	i,		/* counter                      */
		j,		/* counter                      */
		status,		/* error status code            */
		temp;		/* temp value                   */

	/* 
	 * initialize the moment array 
	 */
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			moment [i][j]	= 0;

	/* 
	 * initialize probability array 
	 */
	prob [0]	= -1.0;
	prob [1]	= -1.0;
	prob [2]	= -1.0;

	/*
	 * calculate mean 
	 */
	*mean	= 0;
	*npos	= 0;
	*nneg	= 0;

	for (i = 0; i < n [1]; i++)
	{
		*mean	+= ar[3][i];
		if (ar [3][i] > 0.0)
			++*npos;
		else if (ar [3][i] < 0.0)
			++*nneg;
	}
	*mean	= *mean / n [1];

	/* 
	 * calculate moment  
	 */
	for (i = 0; i < n [1]; i++)
	{
		if ((ar [3][i] - *midpoint * *mean) >= 0.0)
			temp	= 2;
		else
			temp	= 0;
		++n [temp];
		for (j = 0; j < 3; j++)
		{
			jfloat			= j;
			temp_moment		= pow ((ar [3][i] - *midpoint * *mean), (jfloat + 2.0));
			moment [1][j]		+= temp_moment;
			moment [temp][j]	+= temp_moment ;
		}
	}
  
	for (i = 0; i < 3; i++)
	{
		nn	= n [i];
		for (j = 0; j < 3; j++)
			moment [i][j]	= moment [i][j] /nn;
	}
  
	for (i = 0; i < 3; i++)
	{
		sd [i]	= sqrt (moment [i][0]);
		g2 [i]	= (moment [i][2] / (pow (sd [i], 4.0))) - 3.0;
		tmp	= 24.0 * n [i] * pow ((n [i] - 1.0), 2.0);
		tmp	= tmp / ((n [i] - 3.0) * (n [i] - 2.0) * (n [i] + 3.0) * (n [i] + 5.0));
		tmp	= sqrt (tmp);
		t2 [i]	= g2 [i] / tmp;
	}

  *g1 = moment[1][1]/pow(sd[1],3.0) ;
  tmp = sqrt((6.*n[1]*(n[1]-1.))/((n[1]-2.)*(n[1]+1.)*(n[1]+3.))) ;
  *t1 = *g1/tmp ;
  for (j=0;j<3;j++)
    {if (n[j] < 50)
       {status   = error_msg(4) ;   
        nkurt[j] = 50           ;
       }
     else
       nkurt[j] = n[j] ;
     temp = 1 ;
     if (n[j] > 2000) 
       prob[j] = 0.0 ;
    }
  for (j=0;j<3;j++)
    {if (prob[j] != 0)
      {for (i=1;i<27 && kurt[i][0] < nkurt[j];i++) ;
       s2     = kurt[i][0]-nkurt[j]                ;
       s1     = nkurt[j]-kurt[i-1][0]              ;
       p1[j]  = (s1*kurt[i][1]+s2*kurt[i-1][1])/(kurt[i][0]-kurt[i-1][0]);
       p2[j]  = (s1*kurt[i][2]+s2*kurt[i-1][2])/(kurt[i][0]-kurt[i-1][0]);
       p1[j] -= 3 ;
       p2[j] -= 3 ;
       if (g2[j] >= p1[j])
         prob[j] = 1. ;
       else
         if (g2[j] >= p2[j])
           prob[j] = 5. ;
         else
           prob[j] = 100. ;
      }
    }
 if (n[1] < 25)
   *nskew = 25   ;
 else
   *nskew = n[1] ;
 if (n[1] < 500)
   {for (i=1;i<21 && skew[i][0] < *nskew;i++)         ;
    s2   = skew[i][0] - *nskew                        ;
    s1   = *nskew - skew[i-1][0]                      ;
    tdiv = skew[i][0] - skew[i-1][0]                  ;
    *q1  = (s1 * skew[i][1] + s2 * skew[i-1][1])/tdiv ;
    *q2  = (s1 * skew[i][2] + s2 * skew[i-1][2])/tdiv ;
    if (*g1 >= *q1)
      *qrob = 1.0;
    else
      if (*g1 >= *q2)
        *qrob = 5.0  ; 
      else
        *qrob = 100.0;
   }
 else
   *qrob = 0.0 ;

	if (*ztest)
		z_test (z_array, ar, n, mean, midpoint, sd, cutoff, sarp, zcount); 
}

/**********************************************************************

FUNCTION : AGAUSS           H. Rebecca Kimmel        25 January 1989

INTENT   : This function computes the integral of the Gaussian curve 
           from Bevington, "Data Reduction and Error Analysis for the 
           Physical Sciences",p.48.

CALLED BY: Z_TEST         

HISTORY  :
**********************************************************************/

float agauss (x,average,sigma)

float x      ,
      average,
      sigma  ;
{
#define CONSTANT 0.0000000001
 double denom ,
        term  ,
        sum   ,
        y2    ,
        z     ;

 float gauss  ;

 z     = fabs((x-average))/sigma  ;

 if (z <= 0) 
   gauss = 0. ;
 else 
   {term   = 0.7071067812 * z      ; 
    sum    = term                  ; 
    y2     = (z*z)/2.              ;
    denom  = 1.                    ;
    denom  = denom + 2.            ;
    term   = term * (y2*2./denom)  ;
    sum    = sum + term            ;
    while ((term/sum - CONSTANT) > 0)
      { denom = denom + 2.        ;
        term  = term*(y2*2./denom);
        sum   = sum + term        ;
      }
    gauss  = 1.128379167*sum* exp(-y2) ;
   }
 return (gauss) ;
}

/**********************************************************************

FUNCTION : ERROR_MSG      H. Rebecca Kimmel     25 January 1989

INTENT   : This function prints out an error message to the user then
           returns a status code to the calling routine indicating
           whether or not the error was fatal.

CALLED BY: GET_INFO
           GET_DATA
           RFILE
           DATA_ANALYSIS

HISTORY  :

**********************************************************************/

error_msg(number)

int number ;

{
 switch (number)
   {
    case 0: printf ("*** ERROR 0 *** File not found\n") ;
            return(-2) ; 
    case 1: printf ("*** ERROR 1 *** Format of file incorrect! File not read in\n") ;
            return(-1) ;
    case 2: printf ("*** ERROR 2 *** Number of data points exceeds MAXPOINTS %d\n",MAXPOINTS) ;
            return(-1) ;
    case 3: printf ("*** ERROR 3 *** Data contains a peak at 0.0\n") ; 
            return(-1) ;
    case 4: printf ("*** WARNING 4  *** Analysis continued with subminimal points\n") ;
            return (-1) ;
    case 5: printf ("*** ERROR 5 *** No files entered --- Program Terminating\n") ;
            return (-2) ;
    case 6: printf ("*** ERROR 6 *** Need to enter a negative data file ****\n") ;
            return (-2) ;
    case 7: printf ("*** ERROR 7 *** Maximum files (%d) have been entered ****\n");
            return (-1) ;
   }
}


/*____________________________________________________________________

Function:	write_info         H. Rebecca Kimmel    25 January 1989

Description:	This function writes out the results to the output locations
		specified by the user.  This includes the terminal, the line 
		printer and a file on a hard disk.

Called by:	main
Cales:		screen_output
		file_output 

History:
_____________________________________________________________________*/

char tmp_file [80]	= {'t', 'm', 'p', 'f', 'i', 'l', 'e'};	/*tmpfilename */
write_info (n, npos, nneg, mean, sd, g1, t1, midpoint, qrob, nskew, q1, q2, g2, t2,
		prob, nkurt, p1, p2, filenames, filenum, outfile, output_list,
		z_array, cutoff, ztest, zcount)
	sig_numbers	z_array [ZPOINTS];		/* array containing z test values */
	char		filenames [MAXFILES][80],	/* array containing input filenames */
			outfile [80];			/* name of output file-user specified */
	float		sd [3],				/* standard deviations */
			g2 [3],				/* gamma 2 statistics */
			t2 [3],				/* t-statistics */
			p1 [3],				/* 0.01 significance leptokurtosis */
			p2 [3],				/* 0.05 significance leptokurtosis */
			g1,				/* gamma 1 statistic of all points */
			t1,				/* gamma 1 t-statistic of all points */
			q1,				/* 0.01 significance of skew */
			q2,				/* 0.05 significance of skew */
			mean,				/* mean value of all points */
			qrob,				/* indication of skew */
			cutoff,				/* cutoff value used for z test */
			prob [3];			/* indication of kurtosis */
	int		midpoint,			/* flag indicating how mean calc'd */
			npos,				/* number of positive data points */
			nneg,				/* number of negative data points */
			nskew,				/* number of points used for skew */
			ztest,				/* flag indicating z test determin */
			nkurt [3],			/* number of points used for kurtosis */
			output_list [4],		/* types of output desired by user */
			filenum,			/* number of input filenames */
			n [3];				/* number of data points */
{
	int	done,	/* flag indicating done with output */
		output, /* value from output file */
		index;	/* pointer into output array */

	done	= 0;  
	index	= 0;
	output	= output_list [index];
	switch (output)
	{
		case 1:		/* send the output to the terminal console */
			break;

		case 2:		/* send the output to the disk file-user specified */
			file_output (n, npos, nneg, mean, sd, g1, t1, midpoint,
				qrob, nskew, q1, q2, g2, t2, prob, nkurt, p1,
				p2, outfile, z_array, cutoff, ztest, zcount,
				filenames, filenum);
			break;

		case 3:		/* send the output to the printer-user requested */
			file_output (n, npos, nneg, mean, sd, g1, t1, midpoint,
				qrob, nskew, q1, q2, g2, t2, prob, nkurt, p1,
				p2, tmp_file, z_array, cutoff, ztest, zcount,
				filenames, filenum);
				system ("pr tmpfile | lp");
			system ("rm tmpfile");
			break;
		case -1:
			done = -1;
			break ;
	}
	while (!done)
	{
		index++;
		output	= output_list [index];
		switch (output)
		{
			case 1:
				screen_output (n, npos, nneg, mean, sd, g1, t1, midpoint,
						qrob, nskew, q1, q2, g2, t2, prob, nkurt, p1,
						p2, z_array, cutoff, ztest, zcount,
						filenames, filenum);
				break;
			case 2:  
				file_output (n, npos, nneg, mean, sd, g1, t1, midpoint,
						qrob, nskew, q1, q2, g2, t2, prob, nkurt, p1,
						p2, outfile, z_array, cutoff, ztest, zcount,
						filenames, filenum);
				break;
			case 3:  
				file_output (n, npos, nneg, mean, sd, g1, t1, midpoint,
						qrob, nskew, q1, q2, g2, t2, prob, nkurt, p1,
						p2, tmp_file, z_array, cutoff, ztest, zcount,
						filenames, filenum);
				system ("pr tmpfile | lp");
				system ("rm tmpfile");
				break;
			case -1:
				done	= -1;
				break;
		}
	}
} 


/*____________________________________________________________________

FUNCTION  : FILE_OUTPUT     H. Rebecca Kimmel         25 January 1989

INTENT    : This function outputs the results to an output file
            specified by the user.

CALLED BY : WRITE_INFO

HISTORY   : Modified by Tom Yang for Sun Workstation on Sep. 14, 1992.
_____________________________________________________________________*/
file_output (n, npos, nneg, mean, sd, g1, t1, midpoint, qrob, nskew, q1, q2, g2, t2,
	prob, nkurt, p1, p2, outfile, z_array, cutoff, ztest, zcount, filenames, filenum) 
	sig_numbers	z_array [ZPOINTS];	/* ALL VARIABLES DEFN IN WRITE_INFO */
	char		filenames [MAXFILES][80],
			outfile [80];
	float		sd [3],
			g2 [3],
			t2 [3],
			p1 [3],
			p2 [3],
			g1,
			t1,
			q1,
			q2,
			mean,
			qrob,
			cutoff,
			prob [3];
	int		midpoint,
			npos,
			nneg,
			nskew,
			ztest,
			zcount,
			filenum,
			nkurt [3],
			n [3];
{
	int	j;
	FILE	*fp;
	FILE	*af3dfp;
	char	line [MAXLINE];

	fp	= fopen (outfile, "w");

	fprintf (fp, "Input files:\n");
	for (j = 0; j < filenum; j++)
		fprintf (fp, "%s\n", filenames [j]);
	fprintf (fp, "\n\n                           GammaZ v %.1f   09/21/92\n\n", VERSION);
	fprintf (fp, "Number of data points:     %8d\n", n [1]);
	fprintf (fp, "Number of positive points: %8d\n", npos);
	fprintf (fp, "Number of negative points: %8d\n\n", nneg);
	fprintf (fp, "Mean:                      %8.4f\n\n", mean);
	if (midpoint == 1)
	{
		fprintf (fp, "Distribution is split at the mean\n");
		fprintf (fp, "Number of points above the mean: %8d\n", n[2]);
		fprintf (fp, "Number of points below the mean: %8d\n\n", n[0]);
	}
	else
		fprintf (fp, "Distribution is split at 0\n\n");

	fprintf (fp, "Standard deviation of all points:              %8.4f\n", sd [1]);
	fprintf (fp, "SD of the top of the curve:                    %8.4f\n", sd [2]);
	fprintf (fp, "SD of the bottom of the curve:                 %8.4f\n\n", sd [0]);

	fprintf (fp, "Gamma 1 statistic of all points:    %8.4f\n", g1);
	fprintf (fp, "Gamma 1 t-statistic for all points: %8.4f\n\n", t1); 
	if (qrob == 0.0)
		fprintf (fp, "Too many points to test skew\n");
	else
	{
		if (qrob > 5)
			fprintf (fp, "There is no significant skew.\n");
		else
			fprintf (fp, "There is a better than %.2f %% significance of skew.\n", qrob);

		fprintf (fp, " For %d points significance levels are:\n", nskew);

		if (nskew != n[1])
			fprintf (fp, "         Note: %d actual points!\n", n[1]);

		fprintf (fp, "    0.01 : %8.4f     0.05 :   %8.4f\n", q1, q2);
	}

	fprintf (fp, "\nGamma 2 statistic of all points:       %8.4f\n", g2[1]);
	fprintf (fp, "Gamma 2 of the top of the curve:       %8.4f\n", g2 [2]);
	fprintf (fp, "Gamma 2 of the bottom of the curve:    %8.4f\n\n", g2 [0]);
	fprintf (fp, "Gamma 2 t-statistic for all points:    %8.4f\n", t2 [1]);
	fprintf (fp, "t-statistic for the top points:        %8.4f\n", t2 [2]);
	fprintf (fp, "t-statistic for the bottom points:     %8.4f\n", t2 [0]);
	fprintf (fp, "Nota bene: Do NOT use the t-statistic for less than 2000 points.\n\n");
	fprintf (fp, "Kurtosis:\n\n");

	for (j = 0; j < 3; j++)
	{
		if (j == 0)
			fprintf (fp, "Bottom of curve:\n");
		else
			if (j == 1)
				fprintf (fp, "All points:\n");
		else          
			fprintf (fp, "Top of curve:\n") ;

		if (prob[j] == 0.0)
			fprintf (fp, "Too many points to test kurtosis, please use t-statistic\n");
		else
			if (prob[j] > 5.0)
				fprintf (fp, "There is no significant leptokurtosis.\n");
			else
		fprintf (fp, "There is a better than %.1f %% significance of leptokurtosis.\n", prob[j]);

		fprintf (fp, " For %d points, significance levels are:\n", nkurt [j]);

		if (nkurt[j] != n[j])
			fprintf (fp, "(Note: %d actual points!)\n", n[j]);

		fprintf (fp, "0.01 : %8.4f   0.05 : %8.4f\n\n", p1[j], p2[j]);
	}

	if (ztest)
	{
		fprintf (fp, "Cutoff at %.4f\n\n", cutoff) ;
		fprintf (fp, "   x        y        z        max     z-score    significance level\n");
		for (j = 0; j < zcount; j++)
			fprintf (fp, "%6.1f   %6.1f   %6.1f   %7.2f   %7.3f       %9.3f\n",
				z_array[j].x, z_array[j].y, z_array[j].z, z_array[j].max,
				z_array[j].z_score, z_array[j].significance);
	}

	af3dfp	= fopen (filenames [0], "r");
	while (fgets (line, MAXLINE, af3dfp) != NULL)
	{
		if (strstr (line, "af3d_par") != NULL
		|| strstr (line, "Processing Stream") != NULL 
		|| strstr (line, "PROCESSING STREAM") != NULL)
			break;
	}
	fprintf (fp, "\n%s", line);

	while (fgets (line, MAXLINE, af3dfp) != NULL)
	{
		fprintf (fp, "%s", line);
	}

	fclose (af3dfp); 

	fclose (fp); 
}

/***********************************************************************

FUNCTION  : SCREEN_OUTPUT      H. Rebecca Kimmel 25 January 1989

INTENT    : This function outputs the results to the screen if requested
            by the user.  The default is currently set to yes.

CALLED BY : WRITE_INFO

HISTORY   :
***********************************************************************/
screen_output (n,npos,nneg,mean,sd,g1,t1,midpoint,qrob,nskew,q1,q2,g2,
               t2,prob,nkurt,p1,p2,z_array,cutoff,ztest,zcount,
               filenames,filenum) 

char filenames[MAXFILES][80] ; /* ALL VARIABLES DEFINED IN WRITE INFO */
sig_numbers z_array[ZPOINTS] ;

float sd[3]   ,
      g2[3]   ,
      t2[3]   ,
      p1[3]   ,
      p2[3]   ,
      g1      ,
      t1      ,
      q1      ,
      q2      ,
      mean    ,
      qrob    ,
      cutoff  ,
      prob[3] ;

int   midpoint ,
      npos     ,
      nneg     ,
      ztest    ,
      filenum  ,
      zcount   ,
      nskew    ,
      nkurt[3] ,
      n[3]     ;

{
 int j         ;


 fprintf (stdout,"\n\n                           GammaZ  %.1f\n\n\n",VERSION);
 fprintf (stdout,"Input filenames are:\n\n") ;
 for (j=0; j<filenum; j++)
   fprintf (stdout,"   %s\n",filenames[j]) ;
 fprintf (stdout,"\n") ;
 fprintf (stdout,"Number of data points    : %8d\n",n[1]);
 fprintf (stdout,"Number of positive points: %8d\n",npos) ;
 fprintf (stdout,"Number of negative points: %8d\n\n",nneg) ;
 fprintf (stdout,"                     Mean: %8.4f\n\n",mean) ;
 if (midpoint == 1)
   {fprintf (stdout,"Distribution is split at the mean\n") ;
    fprintf (stdout,"Number of points above the mean: %8d\n",n[2]) ;
    fprintf (stdout,"Number of points below the mean: %8d\n\n",n[0]) ;
   }
 else
   fprintf (stdout,"Distribution is split at 0\n\n") ;
 fprintf (stdout,"Standard deviation of all points             : %8.4f\n",sd[1])   ;
 fprintf (stdout,"Standard deviation at the top of the curve   : %8.4f\n",sd[2])   ;
 fprintf (stdout,"Standard deviation at the bottom of the curve: %8.4f\n\n",sd[0]) ;
 fprintf (stdout,"Gamma 1 statistic of all points   : %8.4f\n",g1)                 ;
 fprintf (stdout,"Gamma 1 t-statistic for all points: %8.4f\n\n",t1)               ; 
 if (qrob == 0.0)
   fprintf (stdout,"Too many points to test skew\n") ;
 else
   {if (qrob > 5)
      fprintf (stdout,"There is no significant skew.\n") ;
    else
      fprintf (stdout,"There is a better than %.2f %% significance of skew.\n",qrob) ;
    fprintf (stdout,"For %d points significance levels are:\n",nskew) ;
    if (nskew != n[1])
      fprintf (stdout,"         Note: %d actual points!\n",n[1]) ;
    fprintf (stdout,"    0.01: %8.4f     0.05:   %8.4f\n",q1,q2) ;
   }
 fprintf (stdout,"\nGamma 2 statistic of all points      : %8.4f\n",g2[1]) ;
 fprintf (stdout,"Gamma 2 of the top of the curve      : %8.4f\n",g2[2])   ;
 fprintf (stdout,"Gamma 2 of the bottom of the curve   : %8.4f\n\n",g2[0]) ;
 fprintf (stdout,"Gamma 2 t-statistic for all points   : %8.4f\n",t2[1])   ;
 fprintf (stdout,"Gamma 2 t-statistic for top points   : %8.4f\n",t2[2])   ;
 fprintf (stdout,"Gamma 2 t-statistic for bottom points: %8.4f\n",t2[0])   ;
 fprintf (stdout,"Nota bene: Do NOT use the t-statistic for less than 2000 points.\n\n");
 fprintf (stdout,"Kurtosis:\n\n") ;
 for (j=0;j<3;j++)
   {if (j == 0)
      fprintf (stdout,"Bottom of curve:\n") ;
    else
      if (j == 1)
        fprintf (stdout,"All points:\n") ;
      else          
        fprintf (stdout,"Top of curve:\n") ;
    if (prob[j] == 0.0)
      fprintf (stdout,"Too many points to test kurtosis,please use t-statistic\n") ;
    else
      if (prob[j] > 5.)
        fprintf (stdout,"There is no significant leptokurtosis\n") ;
      else
        fprintf (stdout,"There is a better than %.2f %% significance of leptokurtosis\n",prob[j]) ;
    fprintf (stdout,"For %d points, significance levels are:\n",nkurt[j]) ;
    if (nkurt[j] != n[j])
       fprintf (stdout,"(Note: %d actual points!)\n",n[j]) ;
    fprintf (stdout,"0.01: %8.4f   0.05: %8.4f\n\n",p1[j],p2[j]) ;
   }
	if (ztest)
	{
		fprintf (stdout, "                        Results of Z Test\n\n");
		fprintf (stdout, "Cutoff at: %f\n\n", cutoff) ;
		fprintf (stdout, "   Z        X        Y        MAX     Z-SCORE    SIGNIFICANCE\n");
		fprintf (stdout, "                                                    LEVEL\n");
		for (j = 0; j < zcount; j++)
			fprintf (stdout, "%6.1f   %6.1f   %6.1f   %7.2f   %7.3f    %9.3f\n",
				z_array[j].z, z_array[j].x, z_array[j].y, z_array[j].max,
				z_array[j].z_score, z_array[j].significance);
	}
}


/*____________________________________________________________________

Function:	z_test         H. Rebecca Kimmel          25 January 1989

Description:	This function performs a z-test on the af3d data. If the
		user requests it, the data is translated into sarp space.

Caled by:	data_analysis
Calls:		agauss

History:	Modified by Tom Yang to read .3sp and .3sn files.
		09/22/1992.
____________________________________________________________________*/

z_test (z_array, ar, n, mean, midpoint, sd, cutoff, sarp, zcount) 

	sig_numbers	z_array [ZPOINTS];	/* array holding z-test results */
	float		ar [4][MAXPOINTS],	/* array holding data points */
			sd [3],			/* standard deviation              */
			*cutoff,		/* cutoff point for significance   */
			*mean;			/* mean of data points             */
	int		n [3],			/* number of points                */
			*zcount,		/* number of values in z-array     */
			*sarp,			/* flag indicating sarp transform. */
			*midpoint;		/* flag indicating mean determin.  */

{
	int	i, j, k, count;		/* counters                        */
	float	temp,			/* temporary place holder          */
		mid_mean,		/* holder for mean * midpoint      */
		significance;		/* z-test significance             */

	/* 
	 * Sort values in array. 
	 */
	for (i = 0; i < n[1]; i++)
	{
		for (j = 1; j < n[1]; j++)
		{
			if (ar[3][j] > ar[3][j - 1])
				for (k = 0; k < 4; k++)
				{
					temp		= ar[k][j];
					ar[k][j]	= ar[k][j - 1];
					ar[k][j - 1]	= temp;
				}
		}
	}

	*zcount		= 0;
	mid_mean	= *mean * *midpoint;
	for (i = 0; i < n[1]; i++) 
	{
		significance	= 1.0 - (agauss(ar[3][i], mid_mean, sd[1])); 
		if (significance <= *cutoff) 
		{
			if (*sarp)
			{
				z_array [*zcount].z		= ar [0][i];
				z_array [*zcount].x		= ar[1][i];
				z_array [*zcount].y		= ar[2][i];
				z_array [*zcount].max		= ar[3][i];
				z_array [*zcount].z_score	= (ar[3][i] - mid_mean) / sd[1];
				z_array [*zcount].significance	= significance;
			}
			++*zcount;
		}
	}
}
