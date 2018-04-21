#include <stdio.h>
#include <pam.h>

int main(int argc, char const *argv[])
{
	struct pam inpam, outpam;
	tuple **imgdata_in, **imgdata_out;
	FILE *INPIC, *OUTPIC;

	//initialize pam
	pm_init(argv[0], 0);

	//open file and prepare mem
	INPIC = fopen(argv[1],"r");
	pnm_readpaminit(INPIC, &inpam, sizeof(INPIC));
	imgdata_in = pnm_allocpamarray(&inpam);

	return 0;
}
