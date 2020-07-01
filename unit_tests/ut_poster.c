#include "../latex.h"
#include "../Rintegration.h"


//This is so we can see what input works best for out poster, importantly, the input must be a 
//proper rational
int main() {

	FILE *polyfile;
	poly **input;

	polyfile = fopen("integrate/integrate_latex2.txt", "r");
	
	if(polyfile == NULL) {
		printf("Invalid File Path\n");
		exit(0);
	}

	int polys;
	rational *integrand;
	rational **hermite;
	logpart *log_ans;
	realtrans *atan_part;

	input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);
	hermite = hermite_reduce(integrand);
	log_ans = int_rational_log_part(hermite[1]);
	atan_part = logpart_to_realtrans(log_ans);

	STRING *hermite_result; 	
	hermite_result = integrate_rational(integrand, "x", "$$", "=");
	append_to_string(hermite_result, latex_rational(hermite[0], "x", "", "+")->string);
	append_to_string(hermite_result, integrate_rational(hermite[1], "x", "", "$$")->string);
	write_to_file("hermite_poster.txt", hermite_result);

	STRING *log_result;
	log_result = integrate_rational_string(hermite[1], "a", "x", "$$", "$$");
	write_to_file("log_poster.txt", log_result);

	STRING *atan_result; // this one will probably need some dollar removing
	atan_result = integrate_rational_string_full(hermite[1], "u", "v", "x", "", "");
	write_to_file("atan_poster.txt", atan_result); 	

	return 0;
}




