#include <stdio.h>
#include <stdlib.h>
#include "../complex_polynomial.h"
#include "../latex.h"

int main() {
	
	FILE *polyfile;
	poly **polyarray;

	polyfile = fopen("lrt_latex.txt", "r");

	if(polyfile==NULL) {
		printf("Invalid file path\n");
	}

	int polys, i, len;
	logpart *result;
	rational *integrand;
	
	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);

	result = int_rational_log_part(integrand);
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
	bpoly *resultant;
	int k;

	resultant =bsubresultant(poly_complexify(result->roots[0])[0], 
			poly_complexify(result->roots[0])[1], &k)[0];
	append_to_string(output, latex_bivariate_poly(resultant, "u", "v", "$$", "$$")->string);	
	
	write_to_file("complex_output.txt", output);	



	return 0;
}
