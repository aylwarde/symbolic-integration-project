#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "polynomials.h"

int main()
{
	int deg1, deg2;
	poly *poly1, *poly2;
	poly **division;

	printf("Enter degree of polynomial 1\n");
	scanf("%d", &deg1);
	poly1 = initialize_p(deg1);
	printf("Enter coefficients of polynomial in descending degree,including zero coefficients\n");
	assign_coeffs_p(poly1);
	
	printf("Enter degree of polynomial 2\n");
	scanf("%d", &deg2);
	poly2 = initialize_p(deg2);
	printf("Enter coefficients of polynomial in descending degree,including zero coefficients\n");
	assign_coeffs_p(poly2);


	division = divide_p(poly1, poly2);
	display_p(division[0]);
	display_p(division[1]);


return 0;	
}

