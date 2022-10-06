#define	CMASK	0xff				 /* for making char's > 0 */
#define	EOF	(-1)

/*________________________________________________________
Function:	getcnw_ ().
Description:	unbuffered single character input.
Author:		Tom Yang
Date:		05/07/1992
________________________________________________________*/

#ifdef HP
void            getcnw (ch)
#else
void            getcnw_ (ch)
#endif
	char           *ch;
{
	char            c;

	*ch = (read (0, &c, 1) > 0) ? c & CMASK : EOF;
}
