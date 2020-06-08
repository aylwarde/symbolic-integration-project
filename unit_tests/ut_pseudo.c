#include <stdio.h>

#include "../bivariate_poly.h"

int main() {

	FILE *polyfile;
	
	polyfile = fopen("pseudo.txt", "r");

	if(polyfile ==NULL) {
		printf("Invalid File Path");
		exit(1);
	}

	int polys, i;

	poly **polyarray = from_file_p(polyfile, &polys);
	fclose(polyfile);

	bpoly *b_poly1, *b_poly2, *b_poly3;
	bpoly **result;

	b_poly1 = initialize_bp(6);
	for(i=0; i<=6; ++i)
	{
		b_poly1->coefficients[i] = initialize_p(0);
		b_poly1->coefficients[i]->coefficients[0] = polyarray[0]->coefficients[i];
	}

	b_poly2 = initialize_bp(5);
	for(i=0; i <=5; ++i) {
		b_poly2->coefficients[i] = initialize_p(1);
       		b_poly2->coefficients[i]->coefficients[0] = polyarray[1]->coefficients[i];
		b_poly2->coefficients[i]->coefficients[1] = polyarray[2]->coefficients[i];
	}

	result = poly_pseudo_divide(b_poly1, b_poly2);

	for(i=0; i<=4; ++i) {
	       display_p(result[1]->coefficients[i]);
	}
	
	printf("\n");
	//////
	

	b_poly3 = perspective_change(result[1]);
	for(i=0; i<=2; ++i) {
		display_p(b_poly3->coefficients[i]);
	}

return 0;
}


