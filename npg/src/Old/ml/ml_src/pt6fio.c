int	mod = 0644;


fcreat_(name, fdes)

/*  Purpose:	Create a new file.					*/
/*									*/
/*  Usage:	call fcreat(name, fdes)					*/
/*									*/
/*		where							*/
/*									*/
/*		name is the name of the file to be created.		*/
/*			(must have trailing null) (input)		*/
/*									*/
/*		fdes is the logical unit number returned by fcreat.	*/
/*			(output)					*/
/*									*/
/*  Note:	If the file already exists, the old file will be	*/
/*		opened and assigned a length of zero.			*/

char	*name;
int	*fdes;
{
if ((*fdes = creat(name, mod)) < 0) {
	printf("Could not create file %s \n", name);
	exit(1);
}
}



fopen_(name, fdes)

/*  Purpose:	Open a file with read/write privilege.			*/
/*									*/
/*  Usage:	call fopen(name, fdes)					*/
/*									*/
/*		where							*/
/*									*/
/*		name is the name of the file to be opened.		*/
/*			(must have trailing null) (input)		*/
/*									*/
/*		fdes is the logical unit number returned by fopen.	*/
/*			(output)					*/
/*									*/
/*  Note:	A file opened with fopen will retain its old length,	*/
/*		so it is advisable to use fcreat to open an old file	*/
/*		if you want to ensure that there is no old data at the	*/
/*		end of the file.					*/

char	*name;
int	*fdes;
{
if ((*fdes = open(name, 2)) < 0) {
	printf("Could not open file %s \n", name);
	exit(1);
}
}



fopenr_(name, fdes)

/*  Purpose:	Open a file with read-only privilege.			*/
/*									*/
/*  Usage:	call fopenr(name, fdes)					*/
/*									*/
/*		where							*/
/*									*/
/*		name is the name of the file to be opened.		*/
/*			(must have trailing null) (input)		*/
/*									*/
/*		fdes is the logical unit number returned by fopenr.	*/
/*			(output)					*/
/*									*/
/*  Note:	A file opened with fopenr will retain its old length,	*/
/*		so it is advisable to use fcreat to open an old file	*/
/*		if you want to ensure that there is no old data at the	*/
/*		end of the file.					*/

char	*name;
int	*fdes;
{
if ((*fdes = open(name, 0)) < 0) {
	printf("Could not open file %s \n", name);
	exit(1);
}
}



fclose_(fdes)

/*  Purpose:	Close a file.						*/
/*									*/
/*  Usage:	call fclose(fdes)					*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/

int	*fdes;
{
close(*fdes);
}



funlink_(name)

/*  Purpose:	Delete a file.						*/
/*									*/
/*  Usage:	call funlink(name)					*/
/*									*/
/*		where							*/
/*									*/
/*		name is the name of the file to be deleted.		*/
/*			(must have trailing null) (input)		*/

char	*name;
{
unlink(*name);
}



fread_(fdes, buf, count, eof, rcnt)

/*  Purpose:	Read from a file into an array.				*/
/*									*/
/*  Usage:	call fread(fdes, buf, count, eof, rcnt)			*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/
/*									*/
/*		buf is the name of the array.  (output)			*/
/*									*/
/*		count is the number of bytes to read.  (input)		*/
/*									*/
/*		eof is an end-of-file flag that is set to 0 if no	*/
/*			error occured or 1 if the end of file was	*/
/*			reached.  (output)				*/
/*									*/
/*		rcnt is the number of bytes actually read.  (output)	*/

char	*buf;		/* Pointer to buffer		 */
int	*count,		/* Number of bytes to read	 */
	*eof,		/* End-of-file flag		 */
	*fdes,		/* File descriptor		 */
	*rcnt;		/* Number of bytes actually read */
{
if ((*rcnt = read(*fdes, buf, *count)) != *count) {
	*eof = 1;
	if (*rcnt != 0) {
		printf("Bytes read (%d) not equal to bytes requested (%d) \n",
			*rcnt, *count);
	}
}
else	{
	*eof = 0;
}
}



freadnm_(fdes, buf, count, eof, rcnt)

/*  Purpose:	Read from a file into an array.  No message is printed	*/
/*		if an error occurs.					*/
/*									*/
/*  Usage:	call freadnm(fdes, buf, count, eof, rcnt)		*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/
/*									*/
/*		buf is the name of the array.  (output)			*/
/*									*/
/*		count is the number of bytes to read.  (input)		*/
/*									*/
/*		eof is an end-of-file flag that is set to 0 if no	*/
/*			error occured or 1 if the end of file was	*/
/*			reached.  (output)				*/
/*									*/
/*		rcnt is the number of bytes actually read.  (output)	*/

char	*buf;		/* Pointer to buffer		 */
int	*count,		/* Number of bytes to read	 */
	*eof,		/* End-of-file flag		 */
	*fdes,		/* File descriptor		 */
	*rcnt;		/* Number of bytes actually read */
{
if ((*rcnt = read(*fdes, buf, *count)) != *count) {
	*eof = 1;
}
else	{
	*eof = 0;
}
}



fwrite_(fdes, buf, count)

/*  Purpose:	Write from an array to a file.				*/
/*									*/
/*  Usage:	call fwrite(fdes, buf, count)				*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/
/*									*/
/*		buf is the name of the array.  (input)			*/
/*									*/
/*		count is the number of bytes to write.  (input)		*/

char	*buf;		/* Pointer to buffer	  	 */
int	*count,		/* Number of bytes to write	 */
	*fdes;		/* File descriptor	  	 */
{
int	i;
if ((i = write(*fdes, buf, *count)) != *count) {
	printf("Bytes written (%d) not equal to bytes requested (%d) \n",
		i, *count);
}
}



frew_(fdes)

/*  Purpose:	Rewind the pointer so that the next read or write	*/
/*		starts at the beginning of the file.			*/
/*									*/
/*  Usage:	call frew(fdes)						*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/

int	*fdes;
{
lseek(*fdes, (long)0, 0);
}



fseek_(fdes, offset)

/*  Purpose:	Seek from the beginning of the file.			*/
/*									*/
/*  Usage:	call fseek(fdes, offset)				*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/
/*									*/
/*		offset is the number of bytes from the beginning of	*/
/*			the file.  (input)				*/

int	*fdes;
long	*offset;
{
lseek(*fdes, *offset, 0);
}



fskeof_(fdes, offset)

/*  Purpose:	Seek from the end of a file.				*/
/*									*/
/*  Usage:	call fskeof(fdes, offset)				*/
/*									*/
/*		where							*/
/*									*/
/*		fdes is the logical unit number of the file.  (input)	*/
/*									*/
/*		offset is the number of bytes from the end of the	*/
/*			file (must be negative).  (input)		*/

int	*fdes;
long	*offset;
{
lseek(*fdes, *offset, 2);
}
