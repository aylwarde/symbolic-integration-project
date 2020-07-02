#include "../latex.h"
#include "../lazard_rioboo_trager.h"

//Tests the lazard rioboo trager algorithm so input must be a proper rational with squarefree denom
int main() {

	FILE *polyfile; 

	polyfile = fopen("trivariate.txt", "r");

	if(polyfile==NULL) {
		printf("Error: Invalid file read\n");
		exit(0);
	}

	//read in num and denom, initialize rational
	int polys;
	logpart *result;
	STRING *string_result;
	rational *integrand;

	poly **input = from_file_p(polyfile, &polys);
	fclose(polyfile);
	integrand = init_r(input[0], input[1]);
	
	result = int_rational_log_part(integrand);
	string_result = latex_Logs(result, "a", "x","\\mathbb{\\overline{Q}}", "$$", "$$");

	//write to file
	write_to_file("lrt_output.txt", string_result);

	/*
	//print to terminal
	printf("%s", string_result->string);
	*/

return 0;
}




