#include "../latex.h"
#include "../Rintegration.h"

//test for integration of rational polys, including log to arc tan coversion for real output
//note: integrate folder has multiple examples
int main() {

	FILE *polyfile;

	polyfile = fopen("integrate/integrate_latex4.txt", "r");

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
	string_result = integrate_rational_string_full(integrand, "a", "b",  "x", "", "");

	write_to_file("full_output.txt", string_result);

	return 0;
}
