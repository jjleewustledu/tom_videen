/*$Id: rfa.c,v 1.4 1999/12/13 22:02:19 tom Exp $*/
/*$Log: rfa.c,v $
 *Revision 1.4  1999/12/13 22:02:19  tom
 *partially cleaned up with lint
 *
 * Revision 1.3  1996/09/25  16:28:32  tom
 * build in Solaris 2
 *
 * Revision 1.2  1994/03/03  17:37:01  ty7777
 * Change from if to header.
 **/

/*    @(#)rfa.c	2.9 6/17/92    */
/* rfa.c
* collection of modules to allow rfa commands to be issued to the acquisition
* server as defined in the defaults database /Ecat/EcatAcqServer
*
* rfaDelete(), rfaDir(), rfaRename(), rfaFormat(), rfaMagpie(), rfaCopy(),
* isThisAnACSFile(), getWildCardList(), parseAcsDirEntry(), wildCardMatches(),
* star(), getWildCardStatsList()
*
* t.w. jansto		10/24/90
*/

#ifndef lint
static char     rcsid[] = "$Header: /home/npggw/tom/src/libcti/RCS/rfa.c,v 1.4 1999/12/13 22:02:19 tom Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include "ecatRfa.h"

int             rfaMagpie (squawk)
	int             squawk;
{
	int             resp;
	bool_t          doAcsRfaCommand ();

	doAcsRfaCommand (RBUG, xdr_int, &squawk, xdr_int, &resp);
	return (resp);
}

int             rfaFormat (device)
	char           *device;
{
	int             resp;
	bool_t          doAcsRfaCommand ();

	doAcsRfaCommand (RFMT, xdr_wrapstring, &device, xdr_int, &resp);
	return (resp);
}

int             rfaRename (oldName, newName)
	char           *oldName, *newName;
{
	int             resp;
	RREN_args       renArgs;
	bool_t          doAcsRfaCommand ();

	renArgs.oldName = (char *) malloc (strlen (oldName));
	renArgs.newName = (char *) malloc (strlen (newName));
	strcpy (renArgs.oldName, oldName);
	strcpy (renArgs.newName, newName);
	doAcsRfaCommand (RREN, xdr_RREN_args, &renArgs, xdr_int, &resp);
	free (renArgs.oldName);
	free (renArgs.newName);
	return (resp);
}

unsigned long   rfaCopy (oldName, newName, sizeOfFileInBytes)
	char           *oldName, *newName;
	unsigned long   sizeOfFileInBytes;
{
	unsigned long   resp;
	RCPY_args       copyArgs;
	bool_t          doAcsRfaCommand ();

	copyArgs.oldName = (char *) malloc (strlen (oldName));
	copyArgs.newName = (char *) malloc (strlen (newName));
	strcpy (copyArgs.oldName, oldName);
	strcpy (copyArgs.newName, newName);
	copyArgs.fileSize = sizeOfFileInBytes;
	doAcsRfaCommand (RCPY, xdr_RCPY_args, &copyArgs, xdr_u_long, &resp);
	free (copyArgs.oldName);
	free (copyArgs.newName);
	return (resp);
}

int             rfaDelete (deleteFile)
	char           *deleteFile;
{
	int             resp;
	bool_t          doAcsRfaCommand ();

	doAcsRfaCommand (RDEL, xdr_wrapstring, &deleteFile, xdr_int, &resp);
	return (resp);
}

int             rfaDir (dirString, dirLine, entryNo, freeChunk)
	char           *dirString, *dirLine;
	int            *entryNo, *freeChunk;
{
	RDIR_args       dirArgs;
	RDIR_resp       dirResp;
	bool_t          doAcsRfaCommand ();

	dirArgs.dirPath = dirString;
	dirArgs.currDirNo = *entryNo;
	dirResp.dirEntry = dirLine;
	doAcsRfaCommand (RDIR, xdr_RDIR_args, &dirArgs, xdr_RDIR_resp, &dirResp);
	*entryNo = dirResp.nextDirNo;
	*freeChunk = dirResp.freeChunk;
	return (TRUE);
}

int             isThisAnACSFile (path)
	char           *path;
{
	int             i;
	char            device[8];

	for (i = 0; i < 7; i++) {
		sprintf (device, "/sd%d/", i);
		if (strncmp (path, device, strlen (device)) == 0)
			return (1);
	}
	return (0);
}

struct acsFileEntry {
	int             fileSize;
	char            creationDate[32];
	char            fileName[64];
	char            device[8];
};

int             getWildCardList (wildCard, filesThatMatch)
	char           *wildCard, *filesThatMatch[];
{
	int             numberThatMatch = 0, entryNo = 0, biggestPiece, wildCardLength = 0;
	char            dirChunk[64], *pathStart, *pathEnd, *path, *targetFile, *wildCardSpec;
	struct acsFileEntry *acsDirEntry, *parseAcsDirEntry ();
	DIR            *dirEntry;
	struct dirent  *dp;
	struct stat     fStats;

	if (isThisAnACSFile (wildCard)) {	 /** build list from ACS files */
		if (rfaDir (wildCard, dirChunk, &entryNo, &biggestPiece)) {
			while (strcmp (dirChunk, "")) {
				acsDirEntry = parseAcsDirEntry (dirChunk);
				pathStart = index (wildCard, '/');
				pathEnd = rindex (wildCard, '/');
				bzero (acsDirEntry->device, 8);
				strncpy (acsDirEntry->device, wildCard, pathEnd - pathStart + 1);
				filesThatMatch[numberThatMatch] = (char *) calloc (1,
										   (strlen (acsDirEntry->fileName) + strlen (acsDirEntry->device) + 1));
				strcpy (filesThatMatch[numberThatMatch], acsDirEntry->device);
				strcat (filesThatMatch[numberThatMatch], acsDirEntry->fileName);
				if (!(rfaDir (wildCard, dirChunk, &entryNo, &biggestPiece)))
					break;
				numberThatMatch += 1;
			}
		}
		return (numberThatMatch);
	} else
		/** build list from this side */
	{
		pathStart = index (wildCard, '/');
		pathEnd = rindex (wildCard, '/');
		if (pathStart == pathEnd) {
			path = ".";
			wildCardSpec = wildCard;
		} else {
			if (!(path = (char *) malloc (pathEnd - pathStart + 1))) {
				fprintf (stderr,
				"getWildCardList:Error...unable to malloc space for file path\n");
				return (numberThatMatch);
			}
			bzero (path, pathEnd - pathStart + 2);
			strncpy (path, wildCard, pathEnd - pathStart + 1);
			wildCardLength = strlen (wildCard) - (int) (pathEnd - pathStart + 1);
			if (!(wildCardSpec = (char *) malloc (wildCardLength))) {
				fprintf (stderr,
					 "getWildCardList:Error...unable to malloc space for wild card spec\n");
				return (numberThatMatch);
			}
			bzero (wildCardSpec, wildCardLength + 1);
			strncpy (wildCardSpec, pathEnd + 1, wildCardLength);
			/** can we stat the path? */
			if (!stat (path, &fStats)) {
				/** is it a directory? */
				if (!(S_ISDIR (fStats.st_mode))) {
					free (path);
					free (wildCardSpec);
					return (numberThatMatch);	/* no matches, since we
									 * don't have a directory
									 * specified */
				}
			}
		}
		if ((dirEntry = opendir (path)) == NULL) {
			fprintf (stderr, "getWildCardList:Error...unable to open %s\n", path);
			free (path);
			free (wildCardSpec);
			return (numberThatMatch);
		}
		if (!(targetFile = (char *) malloc (256))) {
			fprintf (stderr, "getWildCardList:Error...unable to malloc space for file spec\n");
			free (path);
			free (wildCardSpec);
			return (numberThatMatch);
		}
		for (dp = readdir (dirEntry); dp != NULL; dp = readdir (dirEntry)) {
			/** skip the . and .. directory entries */
			if (!strncmp (dp->d_name, ".", strlen (".")) ||
			    !strncmp (dp->d_name, ".", strlen (".")))
				continue;
			if (strncmp (path, "..", strlen ("..")))
				sprintf (targetFile, "%s/%s", path, dp->d_name);
			else
				sprintf (targetFile, "%s", dp->d_name);
			if (stat (targetFile, &fStats))
				fprintf (stderr, "getWildCardList:Error...unable to stat file %s\n", targetFile);
			if (wildCardMatches (dp->d_name, wildCardSpec)) {
				filesThatMatch[numberThatMatch] = (char *) calloc (1,
						       (strlen (dp->d_name) + strlen (path) + 1));
				if (strncmp (path, ".", strlen ("."))) {
					strcpy (filesThatMatch[numberThatMatch], path);
					strcat (filesThatMatch[numberThatMatch], dp->d_name);
				} else
					strcat (filesThatMatch[numberThatMatch], dp->d_name);
				numberThatMatch += 1;
			}
		}
		closedir (dirEntry);
		free (path);
		free (wildCardSpec);
		free (targetFile);
		return (numberThatMatch);
	}
}

struct acsFileEntry *
                parseAcsDirEntry (entry)
	char           *entry;
{
	struct acsFileEntry *file;

	char           *dateStart, *dateEnd;

	if (entry[0] == 0)
		return ((struct acsFileEntry *) NULL);
	file = (struct acsFileEntry *) malloc (sizeof (struct acsFileEntry));
	if (!file) {
		fprintf (stderr, "parseACSDirEntry:ERROR...unable to malloc space for structure\n");
		return ((struct acsFileEntry *) NULL);
	}
	bzero (file->creationDate, 32);
	bzero (file->fileName, 64);
	file->fileSize = atoi (entry);
	dateStart = index (entry, ' ');
	dateEnd = rindex (entry, ' ');
	strcpy (file->fileName, dateEnd + 1);
	strncpy (file->creationDate, dateStart, dateEnd - dateStart);
	return (file);
}

int             wildCardMatches (file, reqfile)
	char           *file;			 /* file name to be looked at */

	char           *reqfile;		 /* requested file name */
{
	int             star ();

	for (; *reqfile; file++, reqfile++) {
		switch (*reqfile) {
		default:
			if (*file != *reqfile)
				return (FALSE);
			continue;
		case '*':
			return (*++reqfile ? star (file, reqfile) : TRUE);
		}
	}
	return (*file == '\0');
}

star (s, p)
	char           *s;			 /* partial file pointer string */

	char           *p;			 /* partial requested file wild carded pattern */
{
	while (wildCardMatches (s, p) == FALSE)
		if (*++s == '\0')
			return (FALSE);
	return (TRUE);
}

struct fileStats {
	char           *fileName;
	char           *copyFileName;
	unsigned long   fileSize;
};

int             getWildCardStatsList (wildCard, filesThatMatch, outputName)
	char           *wildCard, *outputName;
	struct fileStats *filesThatMatch[];
{
	int             numberThatMatch = 0, entryNo = 0, biggestPiece, wildCardNewName = 0, wildCardLength = 0;
	char            dirChunk[64], *pathStart, *pathEnd, *path, *token, *targetFile, *wildCardSpec;
	struct acsFileEntry *acsDirEntry, *parseAcsDirEntry ();
	DIR            *dirEntry;
	struct dirent  *dp;
	struct stat     fStats;

	if ((token = strpbrk (outputName, "*"))) {	/* check if he wants us to generate the
							 * name */
		*token = '\0';			 /* NULL out the '*' */
		wildCardNewName = 1;
	}
	if (isThisAnACSFile (wildCard)) {	 /** build list from ACS files */
		if (rfaDir (wildCard, dirChunk, &entryNo, &biggestPiece)) {
			while (strcmp (dirChunk, "")) {
				acsDirEntry = parseAcsDirEntry (dirChunk);
				pathStart = index (wildCard, '/');
				pathEnd = rindex (wildCard, '/');
				bzero (acsDirEntry->device, 8);
				strncpy (acsDirEntry->device, wildCard, pathEnd - pathStart + 1);
				filesThatMatch[numberThatMatch] =
					(struct fileStats *) calloc (1, sizeof (struct fileStats));
				filesThatMatch[numberThatMatch]->fileName = (char *) calloc (1,
								 (strlen (acsDirEntry->fileName) +
							       strlen (acsDirEntry->device) + 1));
				strcpy (filesThatMatch[numberThatMatch]->fileName, acsDirEntry->device);
				strcat (filesThatMatch[numberThatMatch]->fileName, acsDirEntry->fileName);
				filesThatMatch[numberThatMatch]->fileSize = acsDirEntry->fileSize;
				if (wildCardNewName) {
					filesThatMatch[numberThatMatch]->copyFileName =
						(char *) calloc (1, (strlen (acsDirEntry->fileName) +
								     strlen (outputName) + 1));
					sprintf (filesThatMatch[numberThatMatch]->copyFileName, "%s%s",
						 outputName, acsDirEntry->fileName);
				} else {
					filesThatMatch[numberThatMatch]->copyFileName =
						(char *) calloc (1, strlen (outputName) + 1);
					strcpy (filesThatMatch[numberThatMatch]->copyFileName,
						outputName);
				}
				if (!(rfaDir (wildCard, dirChunk, &entryNo, &biggestPiece)))
					break;
				numberThatMatch += 1;
			}
		}
		return (numberThatMatch);
	} else
		/** build list from this side */
	{
		pathStart = index (wildCard, '/');
		pathEnd = rindex (wildCard, '/');
		if (pathStart == pathEnd) {
			path = ".";
			wildCardSpec = wildCard;
		} else {
			if (!(path = (char *) malloc (pathEnd - pathStart + 1))) {
				fprintf (stderr,
				"getWildCardList:Error...unable to malloc space for file path\n");
				return (numberThatMatch);
			}
			bzero (path, pathEnd - pathStart + 2);
			strncpy (path, wildCard, pathEnd - pathStart + 1);
			wildCardLength = strlen (wildCard) - (int) (pathEnd - pathStart + 1);
			if (!(wildCardSpec = (char *) malloc (wildCardLength))) {
				fprintf (stderr,
					 "getWildCardList:Error...unable to malloc space for wild card spec\n");
				return (numberThatMatch);
			}
			bzero (wildCardSpec, wildCardLength + 1);
			strncpy (wildCardSpec, pathEnd + 1, wildCardLength);
			/** can we stat the path? */
			if (!stat (path, &fStats)) {
				/** is it a directory? */
				if (!(S_ISDIR (fStats.st_mode))) {
					free (path);
					free (wildCardSpec);
					return (numberThatMatch);	/* no matches, since we
									 * don't have a directory
									 * specified */
				}
			}
		}
		if ((dirEntry = opendir (path)) == NULL) {
			fprintf (stderr, "getWildCardList:Error...unable to open %s\n", path);
			free (path);
			free (wildCardSpec);
			return (numberThatMatch);
		}
		if (!(targetFile = (char *) malloc (256))) {
			fprintf (stderr, "getWildCardList:Error...unable to malloc space for file spec\n");
			free (path);
			free (wildCardSpec);
			return (numberThatMatch);
		}
		for (dp = readdir (dirEntry); dp != NULL; dp = readdir (dirEntry)) {
			/** skip the . and .. directory entries */
			if (!strncmp (dp->d_name, ".", strlen (".")) ||
			    !strncmp (dp->d_name, ".", strlen (".")))
				continue;
			if (strncmp (path, "..", strlen ("..")))
				sprintf (targetFile, "%s/%s", path, dp->d_name);
			else
				sprintf (targetFile, "%s", dp->d_name);
			if (stat (targetFile, &fStats))
				fprintf (stderr,
				  "getWildCardList:Error...unable to stat file %s\n", targetFile);
			if (wildCardMatches (dp->d_name, wildCardSpec)) {
				filesThatMatch[numberThatMatch] =
					(struct fileStats *) calloc (1, sizeof (struct fileStats));
				filesThatMatch[numberThatMatch]->fileName = (char *) calloc (1,
						       (strlen (dp->d_name) + strlen (path) + 1));
				if (strncmp (path, ".", strlen ("."))) {
					strcpy (filesThatMatch[numberThatMatch]->fileName, path);
					strcat (filesThatMatch[numberThatMatch]->fileName, dp->d_name);
				} else
					strcat (filesThatMatch[numberThatMatch]->fileName, dp->d_name);
				filesThatMatch[numberThatMatch]->fileSize = fStats.st_size;
				if (wildCardNewName) {
					filesThatMatch[numberThatMatch]->copyFileName =
						(char *) calloc (1, (strlen (dp->d_name) +
								     strlen (outputName) + 1));
					sprintf (filesThatMatch[numberThatMatch]->copyFileName, "%s%s",
						 outputName, dp->d_name);
				} else {
					filesThatMatch[numberThatMatch]->copyFileName =
						(char *) calloc (1, strlen (outputName) + 1);
					strcpy (filesThatMatch[numberThatMatch]->copyFileName,
						outputName);
				}
				numberThatMatch += 1;
			}
		}
		closedir (dirEntry);
		free (path);
		free (wildCardSpec);
		free (targetFile);
		return (numberThatMatch);
	}
}
