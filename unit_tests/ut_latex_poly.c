#include "../latex.h" 

int main() {

	FILE *polyfile;

	polyfile = fopen("polytest.txt", "r");
	
	if(polyfile == NULL) {
		printf("Error: Invalid Path File");
		exit(1);
	}

	poly **input;
	int polys, i;

	input = from_file_p(polyfile, &polys);

	fclose(polyfile);
	
	//test printing, integrating, log argument for poly strings
	for(i=0; i<polys; ++i) {
		printf("%s\n", (latex_poly(input[i], "x", "$$", "$$"))->string);
		printf("%s\n", (integrate_poly(input[i], "x", "$$", "$$"))->string);
		printf("%s\n", (log_poly(input[i], "x", "$$", "$$"))->string);
	}


	rational *rat_poly;

	//test printing, integrating, log argument for rationals
	rat_poly = init_r(input[0], input[1]);
	printf("%s\n", (latex_rational(rat_poly, "y", "$$", "$$"))->string);
	printf("%s\n", (integrate_rational(rat_poly, "y", "$$", "$$"))->string);
	printf("%s\n", (log_rational(rat_poly, "z", "$$", "$$"))->string);

	//test write to file
	write_to_file("output.txt", integrate_rational(rat_poly, "x", "$$", "$$"));

	//test sum over poly
	printf("%s\n", (sum_over_poly_sol(input[1], "a", "$$", "$$"))->string);

	
return 0;
}
