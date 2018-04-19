char parity(char in){

	int count = 0;
	
	if((in & 1) == 1) count++;
	if(((in>>1) & 1) == 1) count++;
	if(((in>>2) & 1) == 1) count++;
	if(((in>>3) & 1) == 1) count++;

	if(count % 2 == 0)
		return 0;
	else
		return 1;
}
