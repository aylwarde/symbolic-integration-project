#include "../latex.h"
#include "../lazard_rioboo_trager.h"

int main() {

	FILE *polyfile; 

	polyfile = fopen("lrt_latex.txt", "r");

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
	string_result = latex_Logs(result, "a", "x", "$$", "$$");

	//write to file
	write_to_file("lrt_output.txt", string_result);

	/*
	//print to terminal
	printf("%s", string_result->string);
	*/

return 0;
}




