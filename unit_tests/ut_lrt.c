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

	poly **roots;
	bpoly **arguments;
	int outlen, i;

	int_rational_log_part(integrand, roots, arguments, &outlen);
	printf("%d\n", outlen);
	return 0;
}
