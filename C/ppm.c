#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ppm.h"

// PRIVATE PROTOTYPES
void eat_whitespace(FILE * F);

/* opens ppm file and imports relevant data to a ppming struct
 */
ppmimg * ppm_read(const char * filename){
	int i, temp_int;
	char temp_string2[2];
	ppmimg *ppm_out;
	FILE * ppm_file;

	//allocate ppm struct
	ppm_out = (ppmimg*) malloc(sizeof(ppmimg));

// ## READ HEADER INFO ##
	ppm_file = fopen(filename,"r");

	// check magic number
	fread(temp_string2, sizeof(char), 2, ppm_file);
	if (temp_string2[0] != 'P' || (temp_string2[1] != '6' && temp_string2[1] != '5'))
	{
		printf("%s is not a PPM image.\n", filename);
		return 0;
	}
	eat_whitespace(ppm_file);

	// store width
	fscanf(ppm_file, "%d", &(ppm_out->width));
	eat_whitespace(ppm_file);

	// store height
	fscanf(ppm_file, "%d", &(ppm_out->height));
	eat_whitespace(ppm_file);

	// store colormax
	fscanf(ppm_file, "%d", &(ppm_out->colormax));
	eat_whitespace(ppm_file);

// ## STORE DATA
	// allocate space for data

	// store data

	return ppm_out;
}

/* frees all data related to the given ppm struct
 */
void ppm_free(ppmimg * ppmstruct){

}

/* moves the file pointer past any whitespace
*/
void eat_whitespace(FILE * F){
	char c;
	do
	{
		c = fgetc(F);
	} while (isspace(c));
	fseek(F,-1,SEEK_CUR); //move pointer back once it's not whitespace
}

// ## TESTING AREA ##
int main(int argc, char const *argv[])
{
	ppmimg *in_img;

	if (argc < 2)
	{
		printf("Provide image filename!\n");
		return -1;
	}

	in_img = ppm_read(argv[1]);

	printf("width = %d height = %d colormax = %d\n", 
		in_img->width, in_img->height, in_img->colormax);

	return 0;
}

