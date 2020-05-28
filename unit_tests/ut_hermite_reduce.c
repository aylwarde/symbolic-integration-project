#include <stdio.h>
#include <stdlib.h>

#include "../hermite_reduce.h"

int main()
{
	FILE *polyfile;
	rational *input;
	rational **result;

	polyfile = fopen("hermite_reduce.txt", "r");
	
	if(polyfile == NULL) {
		
			printf("Invalid File Path\n");
			exit(1);
	}
	
	int polys;
	
	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);
	
	input = init_r(polyarray[0], polyarray[1]);

	result = hermite_reduce(input);
	printf("Given f:\n");
	print_r(input);

	printf("We have that the integral of f is equal to g plus the integral of h, with g:\n");
	print_r(result[0]);
	
	printf("And h:\n");
	print_r(result[1]);
	
	/*
	free_p(result[0]);
	free_p(result[1]);
	free(result);
	free_p(polyarray[0]);
	free_p(polyarray[1]);
	free(polyarray);

	*/
return 0;	
}

