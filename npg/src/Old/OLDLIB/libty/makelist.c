/*$Id: makelist.c,v 1.6 1995/11/10 16:28:42 tom Exp $*/
/*$Log: makelist.c,v $
 * Revision 1.6  1995/11/10  16:28:42  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.5  1995/10/13  17:52:06  ty7777
 * use matrix7.h.
 *
 * Revision 1.4  1995/10/13  17:28:57  tom
 * *** empty log message ***
 *
 * Revision 1.3  1994/03/03  20:31:13  ty7777
 * Replace id with header.
 *
 * Revision 1.2  1993/07/28  19:35:51  ty7777
 * Adding rcsid.
 *
 * Revision 1.1  1993/03/03  20:37:30  ty7777
 * Initial revision
 **/

/*     @(#)make_list.c	2.1   2/24/92     */

#ifndef lint
static char sccsid [] = "@(#)makelist.c	2.1   2/24/92  Copyright 1989-1991 CTI PET Systems, Inc.";
#endif

static char rcsid [] = "$Header: /home/npggw/tom/src/libnpg/RCS/makelist.c,v 1.6 1995/11/10 16:28:42 tom Exp $";

/*_____________________________________________________________________________________
	description	make_list() takes the input string listbuff and
			parses this string to create the list of matrix
			numbers represented by listbuff that are in the
			matrix files given by f1 and f2.

	input		f1 and f2 are the names of the files for which a list of
			matrix numbers is to be made, and listbuff is a string
			specifying the matrix numbers.

	output		On succesful exit, make_list() leaves listbuff intact,
			creates and fills in an Mlist structure, sets *dif equal
			to the difference between the number of matrices requested
			and the number actually found,and returns a pointer to the
			structure. If an error occurs, such as a syntax error in the 
			input string, file i/o error, or a memory allocation
			error, make_list() returns a NULL pointer. The memory
			allocated by make_list() should be free'd after it is used.

	called by	called by any function needing a list of specified matrix
			numbers from a matrix file

	calls		parse_matnum(), mat_read_main_header(),
			mat_lookup(), qsort(), init_Mlist()
	
	syntax		The syntax for listbuff is as follows :

			   "frame list;plane list;gate list;bed list",

			where each list consists of numbers separated by
			spaces, ranges of numbers are indicated by using
			'-', and '*' is used to indicate that the number of
			frames, planes, gates, or beds indicated in the file
			header is to be used.

	example		matlist = make_list(file1, file2, "1 2 3-10;*;3;*", &dif),
		        where file1 and file2 are the names of the files from which the list
			is made.

	written by	Will Tribbey	6/3/91

	modification history :

	WHO		DATE		CHANGE

_______________________________________________________________________________________*/

#include <fcntl.h>
#include <petutil.h>
#include <matrix7.h>

dc_err	dcerror;

Mlist *make_list (f1, f2, listbuff, dif)
	char	f1 [], f2 [], listbuff [];
	int	*dif;
{
	MatrixFile	*matrix_open ();

	MatrixFile	*fp1, *fp2;
	Mlist		*matlist; /*   pointer to mlist structure           */
	char		*p,
			frame [256],  /*   buffer for frame string   */
			plane [256],  /*   buffer for plane string   */
			gate [256],   /*   buffer for gate string    */
			bed [256],    /*   buffer for bed string     */
			buff [256];
	int		*fr,	  /*   pointer for array of frame numbers   */
			*pl,      /*   pointer for array of plane numbers   */
			*gt,	  /*   pointer for array of gate numbers    */
			*bd,	  /*   pointer for array of bed numbers     */
			headfr,	  /*   number of frames in headers	    */
			headpl,	  /*   number of planes in headers	    */
			headgt,	  /*   number of gates in headers	    */
			headbd,	  /*   number of bed positions in headers   */
			nframes,  /*   number of frames			    */
			nplanes,  /*   number of planes			    */
			ngates,   /*   number of gates			    */
			nbeds,    /*   number of bed positions		    */
			nreq,	  /*   number of matrix numbers requested   */
			i, j, k, l, m, fr1, p1, g1, b1, fr2, p2, g2, b2,
			mat_get_spec (),
			nmats = 0, matnum,
			matflag, er,
			fctr = 0, pctr = 0, gctr = 0, bctr = 0;

	/*
	 * if files cannot be opened, return NULL pointer   
	 */
	er	= mat_get_spec (f1, &fr1, &p1, &g1, &b1);
	if (er == ERROR) 
	{
		strcpy (buff, "make_list:");
		strcat (buff, f1);
		set_dclib_err (buff, DCLIB_FILE_OPEN_ER);
		return (Mlist_NULL);
	}

	fp1	= matrix_open (f1, "r");
	if (!fp1) 
	{
		strcpy (buff, "make_list:") ;
		strcat (buff, f1) ;
		set_dclib_err (buff, DCLIB_FILE_OPEN_ER);   
		return (Mlist_NULL);
	}

	if (f2) 
	{
		er	= mat_get_spec (f2, &fr2, &p2, &g2, &b2);
		if (er == ERROR) 
		{
			strcpy (buff, "make_list:");
			strcat (buff, f2);
			set_dclib_err (buff, DCLIB_FILE_OPEN_ER);
			return (Mlist_NULL);
		}
		fp2	= matrix_open (f2, "r");
		if (!fp2) 
		{
			matrix_close (fp1);
			strcpy (buff, "make_list:");
			strcat (buff, f2);
			set_dclib_err (buff, DCLIB_FILE_OPEN_ER);   
			return (Mlist_NULL);
		}
		headfr	= inf (fr1, fr2);
		headpl	= inf (p1, p2);
		headgt	= inf (g1, g2);
		headbd	= inf (b1, b2);
	} else 
	{
		fp2	= (MatrixFile *) NULL;
		headfr	= fr1;
		headpl	= p1;
		headgt	= g1;
		headbd	= b1;
	}

	/*   
	 * parse listbuff for frame, plane, gate, and bed strings   
	 */
	strcpy (buff, listbuff);
	if (!parse_list (buff, frame, plane, gate, bed)) 
	{
		set_dclib_err ("make_list:parse_list", DCLIB_LIST_SYNTAX_ER);
		return (Mlist_NULL);
	}

	/* parse frame, plane, gate, and bed strings for       
	 * number of frames, planes, gates, and beds           
	 */
	fr	= parse_matnum (frame, headfr, 0, &nframes);
        pl	= parse_matnum (plane, headpl, 0, &nplanes);
	gt	= parse_matnum (gate, headgt, 0, &ngates);
	bd	= parse_matnum (bed, headbd, 1, &nbeds);

	if (!fr || !pl || !gt || !bd) 
	{
		free (fr);
		free (pl);
		free (gt);
		free (bd);	
		set_dclib_err ("make_list:parse_matnum", DCLIB_MATNUM_LIST_ER);
		return (Mlist_NULL);
	}

	/*
	 * allocate memory for Mlist structure   
	 */
	matlist	= (Mlist *) malloc (sizeof (Mlist));
	if (!matlist) 
	{
		set_dclib_err ("make_list", DCLIB_MEM_ALLOC_ER);	   
		return (Mlist_NULL);
	}
	init_Mlist (matlist);
	nreq		= nframes * nplanes * ngates * nbeds;
	matlist->list	= (int *) malloc (nreq * sizeof (int));
	if (!matlist->list) 
	{
		set_dclib_err ("make_list", DCLIB_MEM_ALLOC_ER);	   
		free (matlist);
		return (Mlist_NULL);
	}

	/*   
	 * make list of matrix numbers   
	 */
	make_matlist (fp1, fp2, fr, nframes, pl, nplanes, gt, ngates, bd, nbeds, matlist);

	/*   
	 * find the actual number of frames, planes,   
	 * gates, and bed positions found              
	 */
	make_num_list (fr, nframes, pl, nplanes, gt, ngates, bd, nbeds, matlist);

	/*   
	 * sort matrix numbers into ascending order   
	 */
	qsort ((char*)matlist->list, nmats, sizeof (int), numcomp);  

	/*   
	 * clean-up and return   
	 */
	matrix_close (fp1);
	if (fp2)
	{
		matrix_close (fp2);   
	}
	free (fr);
	free (pl);
	free (gt);
	free (bd);
	*dif	= nreq-matlist->nmats;

	if (!matlist->nmats) 
	{
		set_dclib_err ("make_list", DCLIB_NO_MATS_FOUND_ER);
		free_Mlist (matlist);
		return (Mlist_NULL) ;
	}
   
	return (matlist);
}

/*_________________________________________________________________________
	description	free_Mlist is used to free memory allocated
			by make_list(). The pointer in the Mlist structure
			that points to the list of matrix numbers is free'd
			as well as the memory pointed to by mlist.

	input		mlist must be a pointer previously returned by
			make_list()

	called by	any program that has called make_list()

	written by	Will Tribbey	3/4/91

	modification history :

	WHO		DATE		CHANGE

_______________________________________________________________________*/
void free_Mlist (mlist)
	Mlist	*mlist;

{
	/*  
	 * check for allocated memory   
	 */
	if (mlist->list)   
		free (mlist->list);

	free (mlist);

	return;
}

/*_________________________________________________________________________
	description	set_dclib_err () initializes the global variable
		        dcerror which is used to print error messages to 
			stderr

	input		msg1 and msg2 contain strings for the text of
			of the error message. err is an integer containing
			an error code.

	called by	datacalc library routines

	written by	Will Tribbey	3/4/91

	modification history :

	WHO		DATE		CHANGE

_______________________________________________________________________*/
void set_dclib_err (msg, err)
	char	msg [];
	int	err;
{
	strcpy (dcerror.msg, msg);
	dcerror.errn	= err;
}

/*__________________________________________________________________
	description	init_Mlist() initializes the Mlist
			structure pointed to by mlist.

	input		mlist is a pointer to an Mlist structure.

	called by	any program that needs to initailize an
			Mlist structure.

	written by	Will Tribbey	3/11/91

	modification history :

	WHO		DATE		CHANGE

________________________________________________________________*/
void init_Mlist (mlist)
	Mlist  *mlist;
{
	if (mlist == Mlist_NULL)  /*  return if NULL pointer  */
		return;

	mlist->nmats  = 0;
	mlist->frames = 0;
	mlist->planes = 0;
	mlist->gates  = 0;
	mlist->beds   = 0;
	mlist->list   = (int*) 0;

	return;
}


	
