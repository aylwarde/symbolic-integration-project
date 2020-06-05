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

	for(i=0; i<polys; ++i) {
		printf("%s\n", (latex_poly(input[i], "x", "$$", "$$"))->string);
		printf("%s\n", (integrate_poly(input[i], "x", "$$", "$$"))->string);
	}


	rational *rat_poly;

	rat_poly = init_r(input[0], input[1]);
	printf("%s\n", (latex_rational(rat_poly, "y", "$$", "$$"))->string);
	printf("%s\n", (integrate_rational(rat_poly, "y", "$$", "$$"))->string);
	
return 0;
}
