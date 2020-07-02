#include <stdio.h>
#include <stdlib.h>
#include "../complex_polynomial.h"
#include "../latex.h"

//testing complexification of univariate and trivariate polys
//input rational proper poly w/ squarefree denom
int main() {
	
	FILE *polyfile;
	poly **polyarray;

	polyfile = fopen("trivariate.txt", "r");

	if(polyfile==NULL) {
		printf("Invalid file path\n");
	}

	int polys, i, len;
	logpart *result;
	rational *integrand;
	
	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);

	result = int_rational_log_part(integrand); //why we have specf for rational input
	STRING *output;
	output = latex_bivariate_poly(poly_complexify(result->roots[0])[0], "u", "v", "$$" ,"$$");
	append_to_string(output, latex_bivariate_poly(poly_complexify(
					result->roots[0])[1], "u", "v", "$$", "$$")->string);
	append_to_string(output,
			latex_trivariate_poly(bpoly_complexify
				(result->arguments[0])[0], "u", "v", "x", "$$", "$$")->string);
	append_to_string(output,
			latex_trivariate_poly(bpoly_complexify
				(result->arguments[0])[1], "u", "v", "x", "$$", "$$")->string);
	
	write_to_file("complex_output.txt", output);	



	return 0;
}
