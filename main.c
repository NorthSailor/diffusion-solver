#include <stdio.h>
#include "matrix.h"

int main(int argc, char const *argv[])
{
	printf("Diffusion Solver 0.0.1\n");
#ifdef USE_DOUBLES
	printf("Double precision: Yes\n");
#else
	printf("Double precision: No\n");
#endif
	return 0;
}
