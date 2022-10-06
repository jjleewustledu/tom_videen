#include <stdlib.h>
#include <stdio.h>
#include <arc.h>
#include <string.h>
#include <unistd.h>
#include <arcfind.h>

extern int      optind;

static int      ftype (fname)
	char           *fname;
{
	extern int      common_pet, pet_data, pet_image, mri_data, mri_image;
	char           *ptr, ext[MAXFNAME];
	ptr = strrchr (fname, '.');
	if (ptr == NULL || *(ptr + 1) == '\0')
		*ext = '\0';
	else
		strcpy (ext, (ptr + 1));
	return (check_ext (ext));
}

main (argc, argv)
	int             argc;
	char          **argv;
{
	struct fnode   *root, *current;
	char            c, supp[MAXFNAME], fname[MAXFNAME];
	char            scanner[MAXTOKEN], acq[MAXTOKEN], recon[MAXTOKEN], fltr[MAXTOKEN];

	*scanner = *acq = *recon = *fltr = '\0';

	if (optind == argc) {
		printf ("Syntax error, you must supply a file name!\n");
		exit (1);
	}
	for (; optind < argc; optind++) {
		strcpy (fname, argv[optind]);

		switch (ftype (fname)) {
		case COMMON_PET:
			sprintf (supp, ",data,");
			break;
		case PET_DATA:
		case PET_IMAGE:
			sprintf (supp, ",%s,%s,%s,%s,",
				 (*scanner) ? scanner : "%",
				 (*acq) ? acq : "%",
				 (*recon) ? recon : "%",
				 (*fltr) ? fltr : "%");
			break;
		case MRI_DATA:
		case MRI_IMAGE:
		default:
			strcpy (supp, "%");
			break;
		}

		root = (struct fnode *) allfiles (fname, supp);
		current = root;
		if (root == NULL)
			exit (1);
		while (current != NULL) {
			if (access (current->path, R_OK) == 0)
				printf ("%s\n", current->path);
			current = current->next;
		}
		freefnodes (root);
	}
	exit (0);
}
