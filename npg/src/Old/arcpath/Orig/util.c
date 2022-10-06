#include <stdio.h>
#include <arc.h>
#include <arcfind.h>

struct fnode   *mkfnode (base, path, fname)
	char           *path, *base, *fname;
{
	struct fnode   *node;
	char           *ptr;
	int             plen;
	plen = strlen (base) + 1 + strlen (path) + 1 + strlen (fname) + 1;
	if ((node = (struct fnode *) malloc (sizeof (struct fnode))) == NULL)
		return ((struct fnode *) NULL);
	if ((ptr = (char *) malloc (plen)) == NULL) {
		free (node);
		return ((struct fnode *) NULL);
	}
	node->path = ptr;
	sprintf (node->path, "%s/%s/%s", base, path, fname);
	node->next = (struct fnode *) NULL;
	return (node);
}

void            freefnode (node)
	struct fnode   *node;
{
	if (node->path != (char *) NULL)
		free (node->path);
	free (node);
}

void            freefnodes (root)
	struct fnode   *root;
{
	struct fnode   *current;
	while (root != (struct fnode *) NULL) {
		current = root->next;
		free (root);
		root = current;
	}
}
