/*
 * File: 	allfiles.c
 * Creator: Fred Kuhns
 * Date: 	Tue Mar 19 12:54:26 CST 1996
 */

#include <stdio.h>
#include <stdlib.h>
#include <riis/syb.h>
#include <arc.h>
#include <arcfind.h>

extern DBPROCESS *DBPROC;

struct fnode   *allfiles (fname, supp)
	char           *fname, *supp;
{
	char            cmd[MAXCMD];
	struct selectret cols[2];
	struct fnode   *froot, *fcurrent;
	struct RETROWS *root, *current;

	froot = fcurrent = (struct fnode *) NULL;

	DBPROC = syblogin (ARCIFILE,
			   DBSERVER,
			   DBHOST,
			   ARCUSER,
			   ARCPASSWD,
			   ARCAPP,
			   DBNAME);
	if (DBPROC == NULL) {
		printf ("Unable to log in to DB server ... try again later!\n\n");
		exit (1);
	}
	/* initialize column descriptors used un sybrows */
	cols[0].vartype = (int) NTBSTRINGBIND;
	cols[0].varlen = 25;
	cols[0].varaddr = (BYTE *) malloc (26);	 /* space for \0 */

	cols[1].vartype = (int) NTBSTRINGBIND;
	cols[1].varlen = 255;
	cols[1].varaddr = (BYTE *) malloc (256); /* space for \0 */

	/* initialize select statement */
	sprintf (cmd,
		 "select base_dir, path from archive_id, archive_location, archive_file where file_name = '%s' and file_status = 'VALID' and location = 'ARCHIVE' and file_supp like '%s' and archive_file.file_id = archive_location.file_id and archive_file.id = archive_id.id", fname, supp);

	/* runit, and collect rows */
	root = (struct RETROWS *) selectrows (cmd, cols, 2, DBPROC);

	/* free allocated memory for column */
	if (cols[0].varaddr)
		free (cols[0].varaddr);
	if (cols[1].varaddr)
		free (cols[1].varaddr);

	if (root == NULL) {
		switch (SYBERR) {
		case DBINIT:
			printf ("Unable to initialize DB strucutures!\n");
			break;
		case DBCONNECT:
			printf ("Unable to connect to DB server!\n");
			break;
		case DBGENERAL:
			printf ("A general DB error has occured!\n");
			break;
		case NOHIT:
			printf ("No files named %s were found in the DB!\n", fname);
			break;
		default:
			printf ("An unknown error has occured while looking for file %s!\n", fname);
			break;
		}
	} else {
		current = root;
		while (current != NULL) {
			if (froot == NULL) {
				froot = mkfnode ((char *) current->cols[0].varaddr,
						 (char *) current->cols[1].varaddr,
						 fname);
				fcurrent = froot;
			} else {
				fcurrent->next = mkfnode ((char *) current->cols[0].varaddr,
							  (char *) current->cols[1].varaddr,
							  fname);
				if (fcurrent->next != NULL)
					fcurrent = fcurrent->next;
			}
			current = current->next;
		}
	}
	freerows (root);
	return (froot);
}
