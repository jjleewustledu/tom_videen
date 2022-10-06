#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <memory.h>

#ifdef HP
void            getch (ch)
#else
void            getch_ (ch)
#endif
	char           *ch;
{
	char            oldch;
	struct termio   otty, ntty;
	int             fd;

	fd = 0;					 /* use stdin */
	oldch = *ch;				 /* save the old value */

	if (ioctl (fd, TCGETA, &otty) == -1) {
		perror ("1:getc");
		exit (-1);
	}
	memcpy (&ntty, &otty, sizeof (struct termio));

	ntty.c_iflag &= ~INLCR;			 /* Don't map NL to CR on input */
	ntty.c_iflag &= ~ICRNL;			 /* Don't map CR to NL on input */
	ntty.c_lflag &= ~ICANON;		 /* Disable canonical processing */
	ntty.c_cc[VMIN] = 1;			 /* don't wait */
	ntty.c_cc[VTIME] = 1;			 /* Set timeout to 0.1 seconds */

	if (ioctl (fd, TCSETA, &ntty) == -1) {
		perror ("2:getc");
		exit (-1);
	}
	read (fd, ch, 1);

	if (ioctl (fd, TCSETA, &otty) == -1) {
		perror ("3:getc");
		exit (-1);
	}
	fflush (stdout);
}
