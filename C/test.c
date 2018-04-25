#include <stdio.h>
#include "ppm.h"

int main(int argc, char const *argv[])
{
	ppmimg *in_img;

	if (argc < 2)
	{
		printf("Provide image filename!\n");
		return -1;
	}

	in_img = ppm_read(argv[1]);

	printf("width = %u height = %u colormax = %u\nfirst pixel = %c\n", 
		in_img->width, in_img->height, in_img->colormax, in_img->data[0][0][0]);

	ppm_free(in_img);

	return 0;
}