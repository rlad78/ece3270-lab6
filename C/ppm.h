#ifndef PPM_H_
#define PPM_H_ 

typedef struct
{
	// char[255] name;
	char magic[2];
	unsigned int height, width, colormax;
	char ***data;
	unsigned short bmult;
} ppmimg;

ppmimg * ppm_read(const char * filename);
void ppm_write(ppmimg * ppmstruct, char *filename);
void ppm_free(ppmimg * ppmstruct);

#endif
