#include <stdio.h>
#include "ppm.h"

/* opens ppm file and imports relevant data to a ppming struct
 */
ppming * ppm_read(char * filename){
	int i, temp_int;
	char[2] temp_string2;
	ppmimg *ppm_out;
	FILE * ppm_file;

	//allocate ppm struct
	ppm_out = (ppmimg*) malloc(sizeof(ppmimg));

	//open file and read header info
	ppm_file = fopen(argv[1],"r");
	fread(temp_string2, sizeof(char), 2, ppm_out);
	if (temp_string2[0] != 'P' || temp_string2[1] != '6')
	{
		printf("%s is not a PPM image.\n", filename);
		return 0;
	}
	fseek(ppm_out, 1, SEEK_CUR); // move past whitespace



	return ppm_out;
}

/* frees all data related to the given ppm struct
 */
void ppm_free(ppmimg * ppmstruct){

}



// ## TESTING AREA ##
int main(int argc, char const *argv[])
{
	FILE *INPIC, *OUTPIC;



	return 0;
}

