#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../polynomials.h"

int main()
{
	FILE *polyfile;
	poly **division;
	
	polyfile = fopen("book_examples/euclid_division.txt", "r");
	
	if(polyfile == NULL) {
	
		printf("Invalid File Path\n");
		exit(1);
	}
	
	int polys;
	
	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);

	division = divide_p(polyarray[0], polyarray[1]);
	print_p(division[0]);
	print_p(division[1]);

	free_p(polyarray[0]);
	free_p(polyarray[1]);
	free_p(division[0]);
	free_p(division[1]);
	free(division);
	free(polyarray);

return 0;	
}

