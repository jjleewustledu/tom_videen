#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mtio.h>
#include <sys/stropts.h>
 
#define TBLOCK 512
#define NAMSIZ 100

union hblock {
   char dummy[TBLOCK];
   struct header {
      char name[NAMSIZ];
      char mode[8];
      char uid[8];
      char gid[8];
      char size[12];
      char mtime[12];
      char chksum[8];
      char linkflag;
      char linkname[NAMSIZ];
   } dbuf;
};

char *blocker_buffer=0;
int blocker_blksize;
int blocker_out;
int blocker_in;
int blocker_blkptr;

blocker(size,in_fd)
int size, in_fd;
{
	int bytesread;

   blocker_blkptr = 0;
   blocker_in = in_fd;
   blocker_out = -1;
   blocker_blksize = size * 512;
   blocker_buffer = malloc(blocker_blksize);
	if (!blocker_buffer) {
		fprintf(stderr,"can't allocate buffer space\n");
		exit(1);
	}
   bzero(blocker_buffer,blocker_blksize);


   bytesread = read(blocker_in,blocker_buffer,blocker_blksize);
   if (bytesread < 0)
   {
      perror("blocker read");
      exit(1);
   }
   if (bytesread < blocker_blksize)
   {	
      fprintf(stderr," warning, blksize = %d reduced performance will result\n",
		bytesread);
      blocker_blksize = bytesread;
   } else fprintf(stderr,"blksize = %d bytes\n", bytesread);
}

int blkread(inbuf,size)
char *inbuf;
int size;
{
	int bytesleft;
   if (blocker_blkptr < 0)
      return (-1);
   bytesleft = blocker_blksize - blocker_blkptr;
   if (bytesleft < size)
   {
      memcpy(inbuf,&blocker_buffer[blocker_blkptr],bytesleft);
      												/* fill'er up	*/
      if (read(blocker_in,blocker_buffer,blocker_blksize) < 0)
      {
         perror("read");
         blocker_blkptr = -1;
         return (bytesleft);
      }
      memcpy(&inbuf[bytesleft],blocker_buffer,size - bytesleft);
      blocker_blkptr = size - bytesleft;
   }
   else
   {
      memcpy(inbuf,&blocker_buffer[blocker_blkptr],size);  /* save what we've got */
      blocker_blkptr += size;
   }
   return (size);
}

void blocker_write(inbuf)
char *inbuf;
{
	write(blocker_out,inbuf,512);
}

int comparenames(name1, name2)
char *name1, *name2;
{
   char *tmp = 0;
   char *cptr = name1;
   while (tmp = strchr(cptr,'/'))
      cptr = tmp + 1;

   name1 = cptr;

   cptr = name2;
   while (tmp = strchr(cptr,'/'))
      cptr = tmp + 1;

   name2 = cptr;

   return (strcmp(name1,name2));
}

static int verbose = 0;
static int mode6 = 0;
#include <errno.h>
#include <sys/errno.h>

int tableofcontents(in)
int in;
{
   int retval = 0;
   char tarbuffer[512], *filename;
   union hblock header;
   int done = 0;
   int bytesread = 0;
   int numblks;
   int size;
   int i;

   for (;;)
   {
      bzero((char*)&header,512);

      bytesread = blkread((char *)&header,512);
      if (bytesread < 0) return 0;
      if (bytesread != 512)
      {
         fprintf(stderr,"ctar: incomplete header, unexpected EOF\n");
         return 0;
      }

      sscanf(header.dbuf.size,"%o",&size);
      numblks = size / 512;
      if (size % 512) numblks++;

      filename = header.dbuf.name;
      if (!strlen(filename)) break;

	printf("t %s %d blocks \n",filename,numblks);

      if (!mode6) 
      {
         for (i = 0; i < numblks; i++)
         {
            if (blkread(tarbuffer,512) < 0)
            {
               fprintf(stderr,"error, unexpected EOF\n");
               return (0);
            }
         }
      }
      else  /* 6.5 compatibility mode (no padding) */
      {
         for (i = 0; i < size;)
         {
            int bytesLeft = size - i;
            int bytesToRead = 512;
            if (bytesLeft < 512)
               bytesToRead = bytesLeft;
            i += bytesToRead;
   
            if (blkread(tarbuffer,bytesToRead) < 0)
            {
               fprintf(stderr,"error, unexpected EOF\n");
               return (0);
            }
         }
      }
   }

   return (retval);
}

int extractfiles(filenames, count, in_fd)
char **filenames;
int count, in_fd;
{
   int extractall = !count;  /* if no files specified, extract everything */
   int retval = 0;
   union hblock header;
   int done = 0;
   int numblks;
   int size;
   int i;
	char *filename;
   char tarbuffer[10240];
   int namesmatch;
	int out_fd, bytestowrite;

   for (;;)
   {
      bzero((char*)&header,512);
   
      blkread((char *)&header,512);
      sscanf(header.dbuf.size,"%o",&size);
      numblks = size / 512;
      if (size % 512) numblks++;

      filename = header.dbuf.name;
      if (!strlen(filename)) break;

      if (verbose) fprintf(stderr,"t %s %d blocks\n",filename,numblks);

      namesmatch = !count;
      for (i = 0; i < count && !namesmatch; i++)
         if (!comparenames(filenames[i],filename))
            namesmatch = 1;

      if (!namesmatch && !extractall)
         continue;
   
      if (verbose)  fprintf(stderr,"x %s %d blocks\n",filename,numblks);

   
      out_fd  = open(filename, O_WRONLY|O_CREAT,0664);
      if (out_fd < 0) perror(filename);

      if (!mode6) /* standard tar format (padded into 512 byte blocks) */
      {
         for (i = 0; i < numblks; i++)
         {
            blkread(tarbuffer,512);

            bytestowrite = 512;
            if (size - (i * 512) < 512)
               bytestowrite = size - (i * 512);

            if (write(out_fd,tarbuffer,bytestowrite) != bytestowrite)
				perror(filename);
         }
      }
      else  /* 6.5 compatibility mode (no padding) */
      {
         for (i = 0; i < size;)
         {
            int bytesLeft = size - i;
            int bytesToRead = 512;
            if (bytesLeft < 512)
               bytesToRead = bytesLeft;
            i += bytesToRead;
   
            blkread(tarbuffer,bytesToRead);

            if (write(out_fd,tarbuffer,bytesToRead) != bytesToRead)
				perror(filename);
         }
      }
      close(out_fd);
   }

   return (retval);
}

int flagset(flag, argc, argv)
char *flag;
int *argc;
char **argv;
{
   int i;

   for (i = 1; i < *argc; i++)
   {
      if (!strcmp(flag,argv[i]))
      {
         for (;argv[i+1]; i++)
         {
            argv[i] = argv[i+1];
         }
         argv[i] = argv[i+1];
         *argc = *argc - 1;
         return (1);
      }
   }

   return (0);
}
int flagval(flag, argc, argv, result)
char *flag;
int *argc;
char **argv;
char *result;
{
   int i;

   for (i = 1; i < *argc; i++)
   {
      if (!strcmp(flag,argv[i]))
      {
         strcpy(result,argv[i+1]);
         for (;argv[i+2]; i++)
         {
            argv[i] = argv[i+2];
         }
         argv[i] = argv[i+2];
         *argc = *argc - 2;
         return (1);
      }
   }

   return (0);
}


void main(argc, argv)
int argc;
char **argv;
{
   int i;
   int create = 0;
   int compare = 0;
   int extract = 0;
   int exitvalue = 0;
   int contents = 0;
   char cmd[128];
   char blockbuf[32];
   int blockingfactor=128;
   char outname[256];
   char study[128];
   int studynum = 0;

   if (argc < 2)
   {
      fprintf(stderr,"usage: ctar [-a][-b num][-x][-c][-v][-d][-s study][-f filename] files...\n");
      fprintf(stderr," -x extract named files from archive\n");
      fprintf(stderr," -t table of contents\n");
      fprintf(stderr," -b specify blocking factor (create only)\n");
      fprintf(stderr," -6 6.5 compatibility mode (extract & contents only)\n");
      fprintf(stderr," -v verify named files to their corresponding archive\n");
      fprintf(stderr," -d debug (verbose)\n");
      fprintf(stderr," -f use 'filename' as tar device\n");
      fprintf(stderr," -s moves tape to study # [1-N]\n");
      fprintf(stderr," -a appends to end of tape\n");
      fprintf(stderr," The use of -x, -v, -t and -c are mutually exclusive\n");
      fprintf(stderr," When using the -a and -s commands, you must specify\n");
      fprintf(stderr," a non rewinding device (i.e. /dev/nrst0).\n");
      exit(1);
   }

   if (flagset("-d",&argc,argv)) verbose = 1;

   if (flagset("-v",&argc,argv)) compare = 1;

   if (flagset("-6",&argc,argv)) mode6 = 1;

   if (flagset("-x",&argc,argv)) extract = 1;

   if (flagset("-t",&argc,argv)) contents = 1;

   if (extract && compare || contents && extract || contents && compare)
   {
      fprintf(stderr,"can't perform multiple operations at the same time\n");
      exit(1);
   }

   if (flagval("-b",&argc,argv,blockbuf))
   {
      blockingfactor = atoi(blockbuf);
      if (verbose)
         fprintf(stderr,"using blocking factor %d\n", blockingfactor);
   }

   strcpy(outname,"");
   flagval("-f",&argc,argv,outname);
   if (verbose)
      fprintf(stderr,"using %s\n",outname);

   if (flagset("-a",&argc,argv))
   {
	  sprintf(cmd,"mt -f %s eom", outname);
      if (strlen(outname));  /* only valid if they specified a device */
         system(cmd); 
   }

   strcpy(study,"");
   flagval("-s",&argc,argv,study);
   if (strlen(study))
   {
      studynum = atoi(study);

      if (verbose)
         fprintf(stderr,"seeking to saveset number %d\n",studynum);
	  sprintf(cmd,"mt -f %s rewind", outname);
      system(cmd);
	  sprintf(cmd,"mt -f %s fsf %d", outname, 2*studynum);
      system(cmd);
   }

   if (extract)
   {
      int in = 0;
      if (strlen(outname))
         in = open(outname,O_RDONLY,0444);
      if (in < 0)
      {
         perror(outname);
         exit(1);
      }
      blocker(blockingfactor,in);
      extractfiles(&argv[1],argc - 1,in);
	  close(in);
   }
   else if (contents)
   {
      int in = 0;
      if (strlen(outname))
         in = open(outname,O_RDONLY,0444);
      if (in < 0)
      {
         perror(outname);
         exit(1);
      }
      blocker(128,in);
      tableofcontents(in);
	  close(in);
   }
   exit(exitvalue);
}
