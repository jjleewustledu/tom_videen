#include <signal.h>
#include <string.h>

char            template_name[255];
char            command[255];

#ifdef HP
cleanup (filename)
#else
cleanup_ (filename)
#endif
	char           *filename;
{
	int             onintr ();

	strcpy (template_name, filename);

	if (signal (SIGINT, SIG_IGN) != SIG_IGN)
		signal (SIGINT, onintr);
}

onintr ()
{
#ifdef HP
	void            xramexit ();
#else
	void            xramexit_ ();
#endif

	strcpy (command, "rm ");
	strcat (command, template_name);

#ifdef HP
	xramexit (command);
#else
	xramexit_ (command);
#endif
}
