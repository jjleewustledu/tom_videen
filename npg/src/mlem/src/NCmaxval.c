#include <stdio.h>

main(argc, argv)

int    argc;
char  *argv[];

  {
  int    i;
  double  maxval;
  double  value;

  for (i = 1; i < argc; i++)
    {
    sscanf(argv[i], "%lf", &value);
    if (i == 1)
      {
      maxval = value;
      }
    else
      {
      if (maxval < value)
        maxval = value;
      }
    }
  printf("%lg\n", maxval);
  }

