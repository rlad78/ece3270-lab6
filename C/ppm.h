#ifndef PPM_H_
#define PPM_H_ 

typedef struct
{
	// char[255] name;
	int height, width, colormax;
	char ***data;
} ppmimg;

ppming * ppm_read(char * filename);
void ppm_free(ppmimg * ppmstruct);

#endif
