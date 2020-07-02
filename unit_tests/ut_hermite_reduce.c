#include <stdio.h>
#include <stdlib.h>

#include "../hermite_reduce.h"
#include "../latex.h"

//Test for hermite reduction on a PROPER rational
int main()
{
	FILE *polyfile;
	rational *input;
	rational **result;

	polyfile = fopen("hermite_reduce.txt", "r");
	
	if(polyfile == NULL) {
		
			printf("Invalid File Path\n");
			exit(1);
	}
	
	int polys;
	
	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);
	
	input = init_r(polyarray[0], polyarray[1]);

	result = hermite_reduce(input);
	STRING *output;
	output = integrate_rational(input, "x", "$$", "=");
	append_to_string(output, latex_rational(result[0], "x" ,"", "+")->string);
	append_to_string(output, integrate_rational(result[1], "x", "", "$$")->string);

	write_to_file("hermite_output.txt", output);
return 0;	
}

