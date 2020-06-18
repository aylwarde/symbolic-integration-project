#include <stdio.h>
#include <stdlib.h>

#include "../polynomials.h"

int main()
{
	int deg1, deg2;
	poly *poly1, *poly2, *gcd;
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


	gcd = gcd_p(poly1, poly2);
	print_p(gcd);


return 0;	
}

