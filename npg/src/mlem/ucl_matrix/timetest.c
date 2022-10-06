#include <time.h>
#include <stdio.h>
main()
{
	time_t t;
	t=time(&t);
	printf("%s\n",asctime(t));
}
	
