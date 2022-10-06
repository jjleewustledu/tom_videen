#ifndef _ARCFIND
#define _ARCFIND

extern int SYBERR;

struct fnode {
	char *path;
	struct fnode *next;
};

struct fnode *allfiles();
struct fnode *mkfnode();
void freefnode();
void freefnodes();

#endif
