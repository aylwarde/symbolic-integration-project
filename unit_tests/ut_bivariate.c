#include <stdio.h>

#include "../bivariate_poly.h"
#include "../latex.h"

int main() {

	FILE *polyfile;
	
	polyfile = fopen("bivariate.txt", "r");

	if(polyfile ==NULL) {
		printf("Invalid File Path");
		exit(1);
	}

	int polys, i;

	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);

	bpoly *b_poly1, *b_poly2, *b_poly3;
	bpoly **result;

	//intializing two polynomials
	b_poly1 = initialize_bp(6);
	for(i=0; i<=6; ++i)
	{
		b_poly1->pcoefficients[i] = initialize_p(0);
		b_poly1->pcoefficients[i]->coefficients[0] = polyarray[0]->coefficients[i];
	}

	b_poly2 = initialize_bp(5);
	for(i=0; i <=5; ++i) {
		b_poly2->pcoefficients[i] = initialize_p(1);
       		b_poly2->pcoefficients[i]->coefficients[0] = polyarray[1]->coefficients[i];
		b_poly2->pcoefficients[i]->coefficients[1] = polyarray[2]->coefficients[i];
	}

	//test for display_bp
	display_bp(b_poly1);
	display_bp(b_poly2);

	//test for copy_bp
	b_poly3 = copy_bp(b_poly1);
	display_bp(b_poly3);
	free_bp(b_poly3);

	//test for pseudo_divide_bp
	result = pseudo_divide_bp(b_poly1, b_poly2);
	display_bp(result[1]);

	//test for variable change
	b_poly3 = variable_change(result[1]);
	display_bp(b_poly3);

	//test for free_bp
	//free_bp(b_poly1);
	//display_bp(b_poly1); //should return segmentation fault

	//test for latex_bivariate_poly in latex.h
	printf("%s\n", (log_bivariate_poly(result[1], "t", "x", "$$", "$$"))->string);
	printf("%s\n", (log_bivariate_poly(b_poly3, "x", "t", "$$", "$$"))->string);

return 0;
}


