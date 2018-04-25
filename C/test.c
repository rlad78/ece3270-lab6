#include <stdio.h>
#include "ppm.h"

int main(int argc, char const *argv[])
{
	ppmimg *in_img, *out_img;

	if (argc < 2)
	{
		printf("Provide image filename!\n");
		return -1;
	}

	in_img = ppm_read(argv[1]);
	out_img = ppm_copy(in_img);
	ppm_write(out_img, argv[2]);

	printf("width = %u height = %u colormax = %u\nfirst pixel = %c\n", 
		in_img->width, in_img->height, in_img->colormax, in_img->data[0][0][0]);

	ppm_free(in_img);
	ppm_free(out_img);

	return 0;
}