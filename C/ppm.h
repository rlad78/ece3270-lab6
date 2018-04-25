#ifndef PPM_H_
#define PPM_H_ 

enum format
{
	RGB, sRGB, BW, sBW, INVALID
};

typedef struct
{
	// char[255] name;
	enum format type;
	char magic[2];
	unsigned int height, width, colormax;
	char ***data;
	unsigned short bmult;
} ppmimg;

ppmimg * ppm_read(const char * filename);
void ppm_write(ppmimg * ppmstruct, const char *filename);
void ppm_free(ppmimg * ppmstruct);
ppmimg * ppm_header_copy(ppmimg * ppmstruct);
ppmimg * ppm_copy(ppmimg * ppmstruct);

#endif
