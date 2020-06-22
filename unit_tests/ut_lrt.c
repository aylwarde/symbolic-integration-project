#include <stdio.h>

#include "../polynomials.h"
#include "../lazard_rioboo_trager.h"

int main() {
	FILE *polyfile;

	polyfile = fopen("book_examples/lrt.txt", "r");

	if(polyfile==NULL) {
		printf("Error: Invalid File Path\n");
		exit(0);
	}

	poly **input;
	int polys;
	rational *integrand;

	input = from_file_p(polyfile, &polys);
	integrand = init_r(input[0], input[1]);

	logpart *result;
	int i;

	result = int_rational_log_part(integrand);

	for(i=0; i<result->num; ++i) {
		print_p(result->roots[i]);
		printf("\n");
		print_bp(result->arguments[i]);
	}
	return 0;
}
