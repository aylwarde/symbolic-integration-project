#include "../latex.h"
#include "../Rintergration.h"

int main() {

	FILE *polyfile;

	polyfile = fopen("integrate_latex.txt", "r");

	if(polyfile == NULL) {
		printf("Error: Invalid file read\n");
		exit(0);
	}

	//read in num and denom, initialize rational
	int polys;
	STRING *string_result;
	rational *integrand;

	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);	
	string_result = integrate_rational_string(integrand, "a", "x", "$$", "$$");

	write_to_file("integral_output.txt", string_result);

	return 0;
}
	
