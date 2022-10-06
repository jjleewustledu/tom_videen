/* Driver for routine scrsho */

#include <stdio.h>
#include "nr.h"

static float fx(x)
float x;
{
	return bessj0(x);
}

main()
{
	scrsho(fx);
	return 0;
}
/* (C) Copr. 1986-92 Numerical Recipes Software |oV'41.5.){2p49. */
