#include <stdio.h>
#include "ppm.h"
#include "filter.h"

int main(int argc, char const *argv[])
{
	ppmimg *in_img, *out_img;

	if (argc < 3)
	{
		printf("Provide image filenames!\n");
		return -1;
	}

	in_img = ppm_read(argv[1]);
	out_img = ppm_copy(in_img);

	filter_image(out_img,EDGE3);

	ppm_write(out_img,argv[2]);

	ppm_free(in_img);
	ppm_free(out_img);

	return 0;
}