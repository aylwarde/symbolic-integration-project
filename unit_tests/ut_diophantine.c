#include <stdio.h>
#include <stdlib.h>

#include "../euclidean_algorithms.h"

int main()
{
	FILE *polyfile;
	poly **result;

	polyfile = fopen("book_examples/euclid_algorithms.txt", "r");
	
	if(polyfile == NULL) {
		
			printf("Invalid File Path\n");
			exit(1);
	}
	
	int polys;
	
	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);

	result = extended_euclidean_diophantine(polyarray[0], polyarray[1], polyarray[2]);
	display_p(result[0]);
	display_p(result[1]);
	
	free_p(result[0]);
	free_p(result[1]);
	free(result);
	free_p(polyarray[0]);
	free_p(polyarray[1]);
	free_p(polyarray[2]);
	free(polyarray);

	
return 0;	
}

