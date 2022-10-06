/*$Id: package.c,v 1.2 1995/10/31 14:54:34 tom Exp $*/
/*$Log: package.c,v $
 * Revision 1.2  1995/10/31  14:54:34  tom
 * remove petutil parent directory from include statements
 *
 * Revision 1.1  1995/10/10  15:41:35  ty7777
 * Initial revision
 **/

/*______________________________________________________________________	
	Function: pkg_message

	Parameters: 
		Variable argument lists accepted, see description of 
		variables in code for acceptable combinations.

	Description:
		Outputs program status messages to the predesignated
		output stream PKG_STREAM. Messages may have text labels. The
		list of available labels is documented in "package.h".

		This function accepts a variable argument list. The first few
		arguments represent program reference information such as
		the program name, the function name where the message 
		facility was called from, the system call that was being
		executed (if any), and any supporting string or variable
		information. The supporting information is specified in a 
		way similar to a call to "printf". There is first a format
		statement which accepts a subset of the control commands for
		the format statement of printf. Second there is a list of
		arguments matching the specified control commands in the 
		format statement.

		For a full explanation of how the variable argument list
		function work see the man page on "varargs".

	Author:
		M. Todd Gamble
		(12/20/90)

	Modifications:
		Modified pkg_alloc so that it returns char * instead of void *. 

		Tom (Tongzeng) Yang
		(05/03/1991)
	Pending:
______________________________________________________________________________*/		

#include <package.h>

static char rcsid [] = "$Header: /home/npggw/tom/src/libpetf77/RCS/package.c,v 1.2 1995/10/31 14:54:34 tom Exp $";

void	pkg_message (va_alist)
va_dcl
{
	va_list		ap;		/* a pointer to the argument list */
	PKG_LABEL	type;		/* the message label ID */
	char		*function;	/* the calling function name */
	char		*call;		/* the system call being made */
	char		*format;	/* a printf style format list */
	int		i;		/* a counter */
	int		count;		/* a counter */

	va_start(ap);			/* initialize the arg list pointer */

	type = va_arg(ap, PKG_LABEL);	/* get the first argument */
	function = va_arg(ap, char *); 	/* get the second argument */
	call = va_arg(ap, char *); 	/* get the third argument */
	format	= va_arg(ap, char *); 	/* get the fourth argument */

					/* all other arguments after this are */
					/* optional and will be determined */
					/* by the requirements of the format */
					/* list. */

	count = strlen(format);		/* get ready to step thru fmt list */

					/* print appropriate label */
	fprintf (PKG_STREAM,"%s", (char *) pkg_message_labels[(int) type]);

	i = 0;
	while (i<count) {
					/* test to see if the next string */
					/* is a format command. */
					/* if it is then process the command */
					/* by finding its corresponding arg */
					/* in the list and printing it */
		if (*format == '%') {
			format++;
			i++;
			switch(*format) {
			/* string */
			case 's': 	
				fprintf(PKG_STREAM,"%s",va_arg(ap,char *));
				break;
			/* decimal integer */
			case 'd':	
				fprintf(PKG_STREAM,"%d",va_arg(ap,int));
				break;
			/* float */
			case 'f':	
				fprintf(PKG_STREAM,"%f",va_arg(ap,float));
				break;
			/* character */
			case 'c':	
				fprintf(PKG_STREAM,"%c",va_arg(ap,char));
				break;
			/* hex integer */
			case 'x':	
				fprintf(PKG_STREAM,"%x",va_arg(ap,int));
				break;
			/* scientific notation integer */
			case 'e':	
				fprintf(PKG_STREAM,"%e",va_arg(ap,double));
				break;
			/* long modifier */
			case 'l':	
				format++;
				i++;
				switch(*format) {
				/* long integer */
				case 'd':	
					fprintf(PKG_STREAM,"%d",va_arg(ap,long));
					break;
				/* long float */
				case 'f':	
					fprintf(PKG_STREAM,"%f",va_arg(ap,double));
					break;
				/* error */
				default:
					fprintf (PKG_STREAM,"\nInvalid format ");
					fprintf (PKG_STREAM,":pkg_message().\n");
					exit(FAIL);
				}
			/* error */
			default:
				fprintf(PKG_STREAM,"\nInvalid format ");
				fprintf(PKG_STREAM,":pkg_message().\n");
				exit(FAIL);
			}	
		}
		else {
			fprintf(PKG_STREAM,"%c",*format);
		}
		format++;
		i++;
	}
	va_end(ap);			/* close arg list pointer */


	/* display reference variable info */
	if (call != (char *) NULL) {
		fprintf(PKG_STREAM,"PERROR:   ");
		fprintf(PKG_STREAM,"%s:%s",call,"errorno");
	}
}


/*_________________________________________________________________________	
	Function: pkg_malloc

	Parameters:
		size: size_t: amount of memory (in bytes) to allocate
		function: char *: function name of caller
		variable: char *: variable name attempting to alloc for

	Description:
		Performs memory allocation and error reporting, abstracting it
		from the calling program.

	Author:
		M. Todd Gamble
_________________________________________________________________________*/

char	*pkg_malloc (size, function, variable)
size_t		size;
char		*function;
char		*variable;
{
	char		*ptr;

	ptr = (char *) malloc (size);
	if (ptr == (char *) NULL) 
	{
		pkg_message (PKG_ERROR, function, "pkg_malloc",
		"Can't allocate %d bytes of memory in function %s for variable %s.\n",
				size, function, variable);
		exit (FAIL);
	}
	return (ptr);
}


/*_________________________________________________________________________	
	Function: pkg_read

	Parameters:
		fd: int: UNIX file descriptor to read from
		buf: char *: buffer to read into from fd
		nbytes: unsigned int: number of bytes to read
		function: char *: name of calling function
		variable: char *: name of buffer variable

	Description:
		Performs a blocking read on the file descriptor specified by
		fd. fd is assumed to have been previously opened. The routine
		waits until nbytes can be read from fd or an error occurs.

	Author:
		M. Todd Gamble
_________________________________________________________________________*/

void	pkg_read(fd,buf,nbytes,function,variable)
int		fd;
char		*buf;
unsigned int	nbytes;
char		*function;
char		*variable;
{
	int	rval;
	int	count;
	char	*ptr;


	count = 0;
	ptr = buf;

	while (count < nbytes) 
	{
		rval = read (fd, ptr, nbytes - count);

		if (rval == -1) 
		{
			pkg_message (PKG_ERROR, function, "pkg_read",
				"Can't read from fd:%d into variable:%s.\n", fd, variable);
			exit (FAIL);
		}
		count += rval;
		ptr += rval;
	}
}
