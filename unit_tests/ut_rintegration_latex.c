#include "../latex.h"
#include "../Rintegration.h"


//This tests the integration algorithm without log to arctan conversion,
//Answer is expressed as rational polynomials + log terms w/ arguments that are polynomials w/ coeffsover the algebraic closure of Q
//note: integrate folder contains multiple examples, and output is txt file for latex 
int main() {

	FILE *polyfile;

	polyfile = fopen("integrate/integrate_latex.txt", "r");

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
	
