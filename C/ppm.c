#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ppm.h"

// PRIVATE PROTOTYPES
void eat_whitespace(FILE * F);

/* opens ppm file and imports relevant data to a ppming struct
 */
ppmimg * ppm_read(const char * filename){
	int i, j, k;
	ppmimg *ppm_obj;
	FILE * ppm_file;
	char ***picdata;

	//allocate ppm struct
	ppm_obj = (ppmimg*) malloc(sizeof(ppmimg));

// ## READ HEADER INFO ##
	ppm_file = fopen(filename,"r");

	// check and store magic number
	fread(&(ppm_obj->magic), sizeof(char), 2, ppm_file);
	if (ppm_obj->magic[0] != 'P' || 
		(ppm_obj->magic[1] != '6' && ppm_obj->magic[1] != '5'))
	{
		printf("%s is not a PPM image.\n", filename);
		return 0;
	}
	eat_whitespace(ppm_file);

	// store width
	fscanf(ppm_file, "%u", &(ppm_obj->width));
	eat_whitespace(ppm_file);

	// store height
	fscanf(ppm_file, "%u", &(ppm_obj->height));
	eat_whitespace(ppm_file);

	// store colormax
	fscanf(ppm_file, "%u", &(ppm_obj->colormax));
	eat_whitespace(ppm_file);

	// determine bmult
	if (ppm_obj->colormax > 0 && ppm_obj->colormax < 256)
	{
		ppm_obj->bmult = 1;
	}
	else if (ppm_obj->colormax < 65536)
	{
		ppm_obj->bmult = 2;
	}
	else
	{
		printf("Invalid colormax of %u\n", ppm_obj->colormax);
		return 0;
	}

// ## STORE DATA
	// allocate space for data and store it
	picdata = (char***) malloc(sizeof(char**)*ppm_obj->height);
	for (i = 0; i < ppm_obj->height; i++)
	{
		picdata[i] = (char**) malloc(sizeof(char*)*ppm_obj->width);
		for (j = 0; j < ppm_obj->width; j++)
		{
			picdata[i][j] = (char*) malloc(sizeof(char)*3*ppm_obj->bmult);
			for (k = 0; k < 3; k++)
			{
				picdata[i][j][k*ppm_obj->bmult] = fgetc(ppm_file);
				if(ppm_obj->bmult > 1) 
				{
					picdata[i][j][(k*ppm_obj->bmult)+1] = fgetc(ppm_file);
				}
			}
		}
	}

	ppm_obj->data = picdata;
	return ppm_obj;
}

/* frees all data related to the given ppm struct
 */
void ppm_free(ppmimg * ppmstruct){
	int i,j,k;

	for (i = 0; i < ppmstruct->height; i++)
	{
		for (j = 0; j < ppmstruct->width; j++)
		{
			free(ppmstruct->data[i][j]);
		}
		free(ppmstruct->data[i])
	}
	free(ppmstruct->data);
	free(ppmstruct);
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

	printf("width = %u height = %u colormax = %u\n", 
		in_img->width, in_img->height, in_img->colormax);

	ppm_free(in_img);

	return 0;
}

