#include <stdio.h>
#include <string.h>
#include "ppm.h"
#include "filter.h"

int main(int argc, char const *argv[])
{
	ppmimg *in_img, *out_img;
	enum filter ftr;

    if (argc < 3)
    {
        printf("FORMAT:\nsmoothing <input filename> <output filename> [filter]\n");
        printf("FILTER OPTIONS:\n(0)identity (1)edge1 (2)edge2 (3)edge3 (4)sharp (5)box (6)gauss1 (7)gauss2\n");
        return -1;
    }

	in_img = ppm_read(argv[1]);
	out_img = ppm_copy(in_img);
	ftr = atoi(argv[3]);

	filter_image(out_img,ftr);

	ppm_write(out_img,argv[2]);

	ppm_free(in_img);
	ppm_free(out_img);

	return 0;
}