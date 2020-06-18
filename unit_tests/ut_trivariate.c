#include "../complex_polynomial.h"
#include "../trivariate_poly.h"
#include "../latex.h"

int main() { 

	FILE *polyfile;
	poly **input;

	polyfile = fopen("trivariate.txt", "r");

	if(polyfile==NULL) {
		printf("Invalid File Path\n");
		exit(0);
	}
	
	int polys;
	logpart *result;
	rational *integrand;

	input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);

	result = int_rational_log_part(integrand);
	
	bpoly **c_res;
	tpoly **c_arg;

	c_res = poly_complexify(result->roots[0]);
	c_arg = bpoly_complexify(result->arguments[0]);
	tpoly **gcd = ext_euclid_tp(c_arg[0], c_arg[1]);

	STRING *output; 
	output = latex_trivariate_poly(c_arg[0], "u", "v", "x", "$$", "$$");
	append_to_string(output, 
			latex_trivariate_poly(c_arg[1], "u", "v", "x", "$$", "$$")->string);	
	append_to_string(output, latex_trivariate_poly
			(multiply_tp(c_arg[0], c_arg[1]), "u", "v", "x", "$$", "$$")->string);	
	append_to_string(output, 
			latex_trivariate_poly(gcd[0], "u", "v", "x", "$$", "$$")->string);	
	append_to_string(output, 
			latex_trivariate_poly(gcd[1], "u", "v", "x", "$$", "$$")->string);	
	append_to_string(output, 
			latex_trivariate_poly(gcd[2], "u", "v", "x", "$$", "$$")->string);	
	write_to_file("tri_output.txt", output);



return 0;
}
